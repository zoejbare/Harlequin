//
// Copyright (c) 2021, Zoe J. Bare
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//

#include "Execution.hpp"
#include "Function.hpp"
#include "Program.hpp"
#include "Vm.hpp"

#include "../base/Mutex.hpp"
#include "../common/OpCodeEnum.hpp"

#include <algorithm>

#include <assert.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

HqExecutionHandle HqExecution::Create(HqVmHandle hVm)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqExecution* const pOutput = new HqExecution();
	assert(pOutput != HQ_EXECUTION_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->hFunction = HQ_FUNCTION_HANDLE_NULL;
	pOutput->hCurrentFrame = HQ_FRAME_HANDLE_NULL;
	pOutput->pExceptionLocation = nullptr;
	pOutput->endianness = HqGetPlatformEndianMode();
	pOutput->lastOpCode = UINT_MAX;
	pOutput->runMode = HQ_RUN_STEP;
	pOutput->frameStackDirty = false;
	pOutput->stateBits = 0;
	pOutput->firstRun = true;
	pOutput->created = false;

	// Initialize the GC proxy to make this object visible to the garbage collector.
	HqGcProxy::Initialize(pOutput->gcProxy, hVm->gc, prv_onGcDiscovery, prv_onGcDestruct, pOutput, false);

	HqFrame::HandleStack::Initialize(pOutput->frameStack, HQ_VM_FRAME_STACK_SIZE);
	HqFrame::HandleStack::Initialize(pOutput->framePool, HQ_VM_FRAME_STACK_SIZE);
	HqValue::HandleArray::Initialize(pOutput->registers);
	HqValue::HandleArray::Reserve(pOutput->registers, HQ_VM_IO_REGISTER_COUNT);

	pOutput->registers.count = HQ_VM_IO_REGISTER_COUNT;

	// Initialize each value in the I/O register set.
	memset(pOutput->registers.pData, 0, sizeof(HqValueHandle) * pOutput->registers.count);

	// Create the main execution fiber.
	prv_createMainFiber(pOutput);

	// Keep the execution context alive indefinitely until we're ready to dispose of it.
	pOutput->gcProxy.autoMark = true;

	// The execution context has finished being created.
	pOutput->created = true;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::Dispose(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Clearing the 'auto-mark' flag will allow the garbage
	// collector to destruct the execution context.
	hExec->gcProxy.autoMark = false;

	// Clearing the VM from the execution context will
	// indicate that the context is no longer in use.
	hExec->hVm = HQ_VM_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecution::Initialize(HqExecutionHandle hExec, HqFunctionHandle hEntryPoint)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(hEntryPoint != HQ_FUNCTION_HANDLE_NULL);

	if(hExec->hFunction != hEntryPoint)
	{
		// Make sure the 'frameStackDirty' flag is set so the reset that follows will
		// clear the existing frame stack to re-initialize it with the new function.
		hExec->frameStackDirty = true;
	}

	hExec->hFunction = hEntryPoint;

	return Reset(hExec);
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecution::Reset(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	if(!hExec->firstRun)
	{
		// Initialize each value in the I/O register set.
		memset(hExec->registers.pData, 0, sizeof(HqValueHandle) * hExec->registers.count);

		// Only re-create the main fiber if execution was stopped somewhere in an instruction.
		if(hExec->state.exception || hExec->state.abort || hExec->state.yield)
		{
			// Dispose of the existing fiber before re-creating it.
			HqFiber::Dispose(hExec->mainFiber);

			prv_createMainFiber(hExec);
		}

		hExec->firstRun = true;
	}

	if(hExec->frameStackDirty)
	{
		// Clear the entire frame stack.
		while(PopFrame(hExec) == HQ_SUCCESS) {}

		if(hExec->hFunction)
		{
			// Push the entry point frame to the frame stack.
			int pushEntryFrameResult = PushFrame(hExec, hExec->hFunction);
			if(pushEntryFrameResult == HQ_ERROR_BAD_ALLOCATION)
			{
				return HQ_ERROR_BAD_ALLOCATION;
			}
			else
			{
				// We expect only success by this point.
				assert(pushEntryFrameResult == HQ_SUCCESS);
			}
		}

		hExec->frameStackDirty = false;
	}

	hExec->pExceptionLocation = nullptr;
	hExec->stateBits = 0;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecution::PushFrame(HqExecutionHandle hExec, HqFunctionHandle hFunction)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	// Get an unused frame.
	HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
	if(HqFrame::HandleStack::Pop(hExec->framePool, &hFrame) != HQ_SUCCESS)
	{
		// There are no unused frames in the pool, so we need to create a new one.
		hFrame = HqFrame::Create(hExec);
	}
	if(!hFrame)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Initialize the frame with the supplied function.
	HqFrame::Initialize(hFrame, hFunction);

	// Push the new frame onto the active frame stack.
	int result = HqFrame::HandleStack::Push(hExec->frameStack, hFrame);
	if(result == HQ_SUCCESS)
	{
		hExec->hCurrentFrame = hFrame;
	}

	hExec->frameStackDirty = true;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecution::PopFrame(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;

	int result = hExec->frameStack.Pop(hExec->frameStack, &hFrame);
	if(result == HQ_SUCCESS)
	{
		hExec->hCurrentFrame = (hExec->frameStack.nextIndex > 0)
			? hExec->frameStack.memory.pData[hExec->frameStack.nextIndex - 1]
			: nullptr;

		// Reset the state of the old frame, then push it to the unused frame pool.
		HqFrame::Reset(hFrame);
		result = hExec->framePool.Push(hExec->framePool, hFrame);
	}

	hExec->frameStackDirty = true;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------;

int HqExecution::SetIoRegister(HqExecutionHandle hExec, HqValueHandle hValue, const size_t index)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(index < HQ_VM_IO_REGISTER_COUNT);

	if(index >= HQ_VM_IO_REGISTER_COUNT)
	{
		return HQ_ERROR_INDEX_OUT_OF_RANGE;
	}

	hExec->registers.pData[index] = hValue;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqExecution::GetIoRegister(HqExecutionHandle hExec, const size_t index, int* const pOutResult)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(pOutResult != nullptr);

	if(index >= HQ_VM_IO_REGISTER_COUNT)
	{
		(*pOutResult) = HQ_ERROR_INDEX_OUT_OF_RANGE;
		return HQ_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return hExec->registers.pData[index];
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::Run(HqExecutionHandle hExec, const int runMode)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(runMode == HQ_RUN_STEP || runMode == HQ_RUN_CONTINUOUS);

	if(hExec->state.finished || hExec->state.exception || hExec->state.abort)
	{
		// Do nothing if the script is no longer executing.
		return;
	}

	// Set the run mode so the fiber context knows how many instructions
	// it needs to process in this iteration.
	hExec->runMode = runMode;

	// Set the 'started' flag to indicate that execution has started.
	// We also reset the 'yield' flag here since it's only useful for
	// indicating that execution was halted on a YIELD instruction the
	// last time around.
	hExec->state.started = true;
	hExec->state.yield = false;

	hExec->firstRun = false;
	hExec->frameStackDirty = true;

	// Run an iteration of the instruction processing fiber context.
	HqFiber::Run(hExec->mainFiber);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::Pause(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	HqFiber::Wait(hExec->mainFiber);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::RaiseException(HqExecutionHandle hExec, HqValueHandle hValue, const int severity)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(severity >= 0);
	assert(severity < HQ_EXCEPTION_SEVERITY__COUNT);

	// The exception value will always be stored to I/O register index 0.
	hExec->registers.pData[0] = hValue;

	if(severity == HQ_EXCEPTION_SEVERITY_FATAL)
	{
		// Force this exception to be unhandled.
		hExec->state.exception = true;

		// Yield execution when a fatal exception is raised.
		HqExecution::Pause(hExec);
	}
	else
	{
		auto blockReverseSortFunc = [](
			HqGuardedBlock* const pLeft,
			HqGuardedBlock* const pRight
		) -> bool
		{
			if(pLeft->bytecodeOffsetStart > pRight->bytecodeOffsetStart)
			{
				// Earlier guarded blocks are sorted after the blocks that come after them.
				return true;
			}
			else if(pLeft->bytecodeOffsetStart == pRight->bytecodeOffsetStart)
			{
				// Nested blocks are sorted *before* the blocks that contain them.
				if(pLeft->bytecodeOffsetEnd < pRight->bytecodeOffsetEnd)
				{
					return true;
				}
			}

			return false;
		};

		auto findExceptionHandler = [&hValue, &blockReverseSortFunc](
			HqFrameHandle hFrame,
			uint32_t* pOutHandlerOffset
		) -> bool
		{
			const uint8_t valueType = hValue 
				? uint8_t(hValue->type) 
				: uint8_t(HQ_VALUE_TYPE_OBJECT);

			uint32_t currentOffset = 0;
			if(HqFrameGetBytecodeOffset(hFrame, &currentOffset) != HQ_SUCCESS)
			{
				// Failing to get the current offset within the frame means that it
				// can't possibly handle this (or any) exception.
				return false;
			}

			HqGuardedBlock::Stack validBlocks;
			HqGuardedBlock::Stack::Initialize(validBlocks, hFrame->hFunction->guardedBlocks.count);

			// Find all the guarded blocks that encapsulate the instruction that raised the exception.
			for(size_t blockIndex = 0; blockIndex < hFrame->hFunction->guardedBlocks.count; ++blockIndex)
			{
				HqGuardedBlock* pBlock = hFrame->hFunction->guardedBlocks.pData[blockIndex];

				if(currentOffset >= pBlock->bytecodeOffsetStart
					&& currentOffset <= pBlock->bytecodeOffsetEnd)
				{
					HqGuardedBlock::Stack::Push(validBlocks, pBlock);
				}
			}

			// Sort the guarded blocks in reverse order since we need to start with the most nested block first.
			std::sort(validBlocks.memory.pData, validBlocks.memory.pData + validBlocks.nextIndex, blockReverseSortFunc);

			// Search the blocks for an exception handler that is capable of handling the raised value.
			for(size_t blockIndex = 0; blockIndex < validBlocks.nextIndex; ++blockIndex)
			{
				HqGuardedBlock* const pBlock = validBlocks.memory.pData[blockIndex];

				// Check each handler on the current block.
				for(size_t handlerIndex = 0; handlerIndex < pBlock->handlers.count; ++handlerIndex)
				{
					HqExceptionHandler* const pHandler = pBlock->handlers.pData[handlerIndex];

					// Check if the general type of the handler matches the raised value.
					if(pHandler->type == valueType)
					{
						// If the value is an object, we need to also compare the class type name to
						// verify this handler will handle the exact object type that was raised.
						if(valueType != HQ_VALUE_TYPE_OBJECT ||
							(
								valueType == HQ_VALUE_TYPE_OBJECT
									&& (!hValue || HqString::Compare(pHandler->pClassName, hValue->as.pObject->pTypeName))
							)
						)
						{
							// This is the handler we'll use.
							HqGuardedBlock::Stack::Dispose(validBlocks);

							(*pOutHandlerOffset) = pHandler->offset;
							return true;
						}
					}
				}
			}

			// No acceptable handler was found for this frame.
			HqGuardedBlock::Stack::Dispose(validBlocks);

			return false;
		};

		const size_t frameStackLength = hExec->frameStack.nextIndex;
		if(frameStackLength > 0)
		{
			// Search the frame stack from the end to find a handler for the raised value.
			size_t frameIndex = frameStackLength - 1;
			for(;; --frameIndex)
			{
				HqFrameHandle hFrame = hExec->frameStack.memory.pData[frameIndex];

				uint32_t handlerOffset = 0;

				if(findExceptionHandler(hFrame, &handlerOffset))
				{
					// We found the frame that will handle the exception, now we need
					// to actually pop the frame stack until we get to that frame.
					while(hExec->hCurrentFrame != hFrame)
					{
						const int popFrameResult = PopFrame(hExec);

						// Sanity check: There should not be any errors on popping the frame stack.
						assert(popFrameResult == HQ_SUCCESS);
						(void) popFrameResult;
					}

					// Sanity check: The current frame should never be null after popping the frame stack.
					assert(hExec->hCurrentFrame != HQ_FRAME_HANDLE_NULL);

					// Set the instruction pointer to the start of the exception handler.
					hExec->hCurrentFrame->decoder.cachedIp = hExec->hCurrentFrame->hFunction->hProgram->code.pData + handlerOffset;
					hExec->hCurrentFrame->decoder.ip = hExec->hCurrentFrame->decoder.cachedIp;

					break;
				}

				if(frameIndex == 0)
				{
					// There are either no exception handlers in the frame stack
					// or none capable of handling this exception.
					hExec->state.exception = true;

					// Yield execution for unhandled exceptions.
					Pause(hExec);
					break;
				}
			}
		}
		else
		{
			// There are no frames on the stack, so there is nothing that can catch this exception.
			hExec->state.exception = true;

			Pause(hExec);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::RaiseOpCodeException(HqExecutionHandle hExec, const int type, const char* const fmt, ...)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(type >= 0);
	assert(type < HQ_STANDARD_EXCEPTION__COUNT);
	assert(fmt != nullptr);

	char msg[256];
	{
		va_list vl;

		// Write the formatted message to the temporary string.
		va_start(vl, fmt);
		vsnprintf(msg, (sizeof(msg) / sizeof(char)) - 1, fmt, vl);
		va_end(vl);
	}

	HqValueHandle hThrowValue = HqVm::CreateStandardException(hExec->hVm, type, msg);

	RaiseException(hExec, hThrowValue, HQ_EXCEPTION_SEVERITY_FATAL);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::prv_createMainFiber(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	HqFiberConfig runFiberConfig;
	runFiberConfig.mainFn = prv_runFiberLoop;
	runFiberConfig.pArg = hExec;
	runFiberConfig.stackSize = HQ_VM_THREAD_MINIMUM_STACK_SIZE;
	strncpy(runFiberConfig.name, "ExecMainFiber", sizeof(runFiberConfig.name) - 1);

	// Create the fiber context that will be used for running scripts.
	HqFiber::Create(hExec->mainFiber, runFiberConfig);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::prv_runFiberLoop(void* const pArg)
{
	HqExecutionHandle hExec = reinterpret_cast<HqExecutionHandle>(pArg);

	// We can loop forever here since execution will be yielded to the calling thread/fiber.
	// When a yield occurs, this will stop executing in-place.
	for(;;)
	{
		// Cache the run mode to decrease access time in a continuous run loop.
		const uint32_t runMode = hExec->runMode;

		// We only need to check if the execution has finished because when a fatal error
		// occurs or a script aborts, they'll immediately yield the fiber and the execution
		// context won't be able to run it again.
		while(!hExec->state.finished)
		{
			prv_runStep(hExec);

			if(runMode == HQ_RUN_STEP)
			{
				// When manually stepping instructions, we should only process a single
				// instruction per iteration.
				break;
			}
		}

		if(hExec->lastOpCode != HQ_OP_CODE_YIELD)
		{
			// Force the fiber context to yield if we've run out of script instructions to process for this iteration.
			// However, we only do this when the last opcode was NOT a yield itself, otherwise the moment the script
			// execution is resumed, it would immediately yield again when we instead want to go to the next opcode.
			Pause(hExec);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::prv_runStep(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	HqScopedReadLock gcLock(hExec->hVm->gc.rwLock, hExec->hVm->isGcThreadEnabled);

	HqFrameHandle hFrame = hExec->hCurrentFrame;

	// Save the current instruction pointer position at the start of the opcode that will now be executed.
	hFrame->decoder.cachedIp = hFrame->decoder.ip;

	const uint32_t opCode = HqDecoder::LoadUint32(hFrame->decoder);

	// Cache the opcode prior to executing it so we can query it in the event of a fiber yield.
	hExec->lastOpCode = opCode;

	HqVm::ExecuteOpCode(hExec->hVm, hExec, opCode);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::prv_onGcDiscovery(HqGarbageCollector& gc, void* const pOpaque)
{
	HqExecutionHandle hExec = reinterpret_cast<HqExecutionHandle>(pOpaque);
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Visit all active frames.
	const size_t activeStackSize = HqFrame::HandleStack::GetCurrentSize(hExec->frameStack);
	for(size_t i = 0; i < activeStackSize; ++i)
	{
		HqFrameHandle hFrame = hExec->frameStack.memory.pData[i];

		HqGarbageCollector::MarkObject(gc, &hFrame->gcProxy);
	}

	// Visit all unused frames.
	const size_t unusedStackSize = HqFrame::HandleStack::GetCurrentSize(hExec->framePool);
	for(size_t i = 0; i < unusedStackSize; ++i)
	{
		HqFrameHandle hFrame = hExec->framePool.memory.pData[i];

		HqGarbageCollector::MarkObject(gc, &hFrame->gcProxy);
	}

	// Discover values held in the I/O registers.
	for(size_t i = 0; i < hExec->registers.count; ++i)
	{
		HqValueHandle hValue = hExec->registers.pData[i];

		if(HqValue::CanBeMarked(hValue))
		{
			HqGarbageCollector::MarkObject(gc, &hValue->gcProxy);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::prv_onGcDestruct(void* pObject)
{
	HqExecutionHandle hExec = reinterpret_cast<HqExecutionHandle>(pObject);
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	HqFrame::HandleStack::Dispose(hExec->frameStack);
	HqFrame::HandleStack::Dispose(hExec->framePool);
	HqValue::HandleArray::Dispose(hExec->registers);

	delete hExec;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqExecution::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
