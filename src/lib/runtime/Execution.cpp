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

#include <algorithm>

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

HqExecutionHandle HqExecution::Create(HqVmHandle hVm, HqFunctionHandle hEntryPoint)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hEntryPoint != HQ_FUNCTION_HANDLE_NULL);

	HqExecution* const pOutput = new HqExecution();
	assert(pOutput != HQ_EXECUTION_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->endianness = HqGetPlatformEndianMode();
	pOutput->yield = false;
	pOutput->started = false;
	pOutput->finished = false;
	pOutput->exception = false;

	HqScopedWriteLock gcLock(hVm->gc.rwLock);

	// Initialize the GC proxy to make this object visible to the garbage collector.
	HqGcProxy::Initialize(pOutput->gcProxy, hVm->gc, prv_onGcDiscovery, prv_onGcDestruct, pOutput, false);

	HqFrame::HandleStack::Initialize(pOutput->frameStack, HQ_VM_FRAME_STACK_SIZE);
	HqValue::HandleArray::Initialize(pOutput->registers);
	HqValue::HandleArray::Reserve(pOutput->registers, HQ_VM_IO_REGISTER_COUNT);

	pOutput->registers.count = HQ_VM_IO_REGISTER_COUNT;

	// Initialize each value in the I/O register set.
	for(size_t i = 0; i < pOutput->registers.count; ++i)
	{
		pOutput->registers.pData[i] = HqValue::CreateNull();
	}

	// Create the first frame using the entry point function.
	pOutput->hCurrentFrame = HqFrame::Create(pOutput, hEntryPoint);
	if(!pOutput->hCurrentFrame)
	{
		return HQ_EXECUTION_HANDLE_NULL;
	}

	// Push the entry point frame to the frame stack.
	int result = HqFrame::HandleStack::Push(pOutput->frameStack, pOutput->hCurrentFrame);
	if(result != HQ_SUCCESS)
	{
		return HQ_EXECUTION_HANDLE_NULL;
	}

	// Map the execution context to the VM used to create it.
	HQ_MAP_FUNC_INSERT(hVm->executionContexts, pOutput, false);

	// Keep the execution context alive indefinitely until we're ready to dispose of it.
	pOutput->gcProxy.autoMark = true;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::ReleaseWithNoDetach(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Clearing the 'auto-mark' flag will allow the garbage
	// collector to destruct the execution context.
	hExec->gcProxy.autoMark = false;
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::DetachFromVm(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	ReleaseWithNoDetach(hExec);

	HqVmHandle hVm = hExec->hVm;
	HqScopedWriteLock gcLock(hVm->gc.rwLock);

	// Unlink the execution context from the VM.
	HQ_MAP_FUNC_REMOVE(hVm->executionContexts, hExec);
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecution::PushFrame(HqExecutionHandle hExec, HqFunctionHandle hFunction)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	HqFrameHandle hFrame = HqFrame::Create(hExec, hFunction);
	if(!hFrame)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	int result = HqFrame::HandleStack::Push(hExec->frameStack, hFrame);
	if(result == HQ_SUCCESS)
	{
		hExec->hCurrentFrame = hFrame;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecution::PopFrame(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
	int result = hExec->frameStack.Pop(hExec->frameStack, &hFrame);

	hExec->hCurrentFrame = (hExec->frameStack.nextIndex > 0)
		? hExec->frameStack.memory.pData[hExec->frameStack.nextIndex - 1]
		: nullptr;

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

	if(hExec->finished || hExec->exception || hExec->abort)
	{
		// Do nothing if the script is no longer executing.
		return;
	}

	// Set the 'started' flag to indicate that execution has started.
	// We also reset the 'yield' flag here since it's only useful for
	// pausing execution and we no longer need it paused until the
	// next yield.
	hExec->started = true;
	hExec->yield = false;

	switch(runMode)
	{
		case HQ_RUN_STEP:
		{
			prv_runStep(hExec);
			break;
		}

		case HQ_RUN_CONTINUOUS:
		{
			while(!hExec->finished && !hExec->exception && !hExec->yield && !hExec->abort)
			{
				prv_runStep(hExec);
			}
			break;
		}

		default:
			// This should never happen.
			assert(false);
	}
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
		hExec->exception = true;
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
					if(pHandler->type == hValue->type)
					{
						// If the value is an object, we need to also compare the class type name to
						// verify this handler will handle the exact object type that was raised.
						if(hValue->type != HQ_VALUE_TYPE_OBJECT ||
							(
								hValue->type == HQ_VALUE_TYPE_OBJECT
								&& HqString::Compare(pHandler->pClassName, hValue->as.pObject->pTypeName)
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
						const int popFrameResult = HqExecution::PopFrame(hExec);

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
					hExec->exception = true;
					break;
				}
			}
		}
		else
		{
			// There are no frames on the stack, so there is nothing that can catch this exception.
			hExec->exception = true;
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

	HqExecution::RaiseException(hExec, hThrowValue, HQ_EXCEPTION_SEVERITY_FATAL);
	HqValue::SetAutoMark(hThrowValue, false);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::prv_runStep(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	HqScopedReadLock gcLock(hExec->hVm->gc.rwLock);

	HqFrameHandle hFrame = hExec->hCurrentFrame;

	// Save the current instruction pointer position at the start of the opcode that will now be executed.
	hFrame->decoder.cachedIp = hFrame->decoder.ip;

	const uint8_t opCode = HqDecoder::LoadUint8(hFrame->decoder);
	HqVm::ExecuteOpCode(hExec->hVm, hExec, opCode);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExecution::prv_onGcDiscovery(HqGarbageCollector& gc, void* const pOpaque)
{
	HqExecutionHandle hExec = reinterpret_cast<HqExecutionHandle>(pOpaque);
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Discover all active frames in the frame stack.
	const size_t stackSize = HqFrame::HandleStack::GetCurrentSize(hExec->frameStack);
	for(size_t i = 0; i < stackSize; ++i)
	{
		HqFrameHandle hFrame = hExec->frameStack.memory.pData[i];

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
