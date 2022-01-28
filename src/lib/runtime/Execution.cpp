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

#include <assert.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------

XenonExecutionHandle XenonExecution::Create(XenonVmHandle hVm, XenonFunctionHandle hEntryPoint)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(hEntryPoint != XENON_FUNCTION_HANDLE_NULL);

	XenonExecution* const pOutput = new XenonExecution();
	assert(pOutput != XENON_EXECUTION_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->endianness = XenonGetPlatformEndianMode();
	pOutput->yield = false;
	pOutput->started = false;
	pOutput->finished = false;
	pOutput->exception = false;

	XenonScopedWriteLock gcLock(hVm->gcRwLock);

	// Initialize the GC proxy to make this object visible to the garbage collector.
	XenonGcProxy::Initialize(pOutput->gcProxy, hVm->gc, prv_onGcDiscovery, prv_onGcDestruct, pOutput, false);

	XenonFrame::HandleStack::Initialize(pOutput->frameStack, XENON_VM_FRAME_STACK_SIZE);
	XenonValue::HandleArray::Initialize(pOutput->registers);
	XenonValue::HandleArray::Reserve(pOutput->registers, XENON_VM_IO_REGISTER_COUNT);

	pOutput->registers.count = XENON_VM_IO_REGISTER_COUNT;

	// Initialize each value in the I/O register set.
	for(size_t i = 0; i < pOutput->registers.count; ++i)
	{
		pOutput->registers.pData[i] = XenonValue::CreateNull();
	}

	// Create the first frame using the entry point function.
	pOutput->hCurrentFrame = XenonFrame::Create(pOutput, hEntryPoint);
	if(!pOutput->hCurrentFrame)
	{
		return XENON_EXECUTION_HANDLE_NULL;
	}

	// Push the entry point frame to the frame stack.
	int result = XenonFrame::HandleStack::Push(pOutput->frameStack, pOutput->hCurrentFrame);
	if(result != XENON_SUCCESS)
	{
		return XENON_EXECUTION_HANDLE_NULL;
	}

	// Map the execution context to the VM used to create it.
	XENON_MAP_FUNC_INSERT(hVm->executionContexts, pOutput, false);

	// Keep the execution context alive indefinitely until we're ready to dispose of it.
	pOutput->gcProxy.autoMark = true;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::ReleaseWithNoDetach(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Clearing the 'auto-mark' flag will allow the garbage
	// collector to destruct the execution context.
	hExec->gcProxy.autoMark = false;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::DetachFromVm(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	ReleaseWithNoDetach(hExec);

	XenonVmHandle hVm = hExec->hVm;
	XenonScopedWriteLock gcLock(hVm->gcRwLock);

	// Unlink the execution context from the VM.
	XENON_MAP_FUNC_REMOVE(hVm->executionContexts, hExec);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecution::PushFrame(XenonExecutionHandle hExec, XenonFunctionHandle hFunction)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	XenonFrameHandle hFrame = XenonFrame::Create(hExec, hFunction);
	if(!hFrame)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	int result = XenonFrame::HandleStack::Push(hExec->frameStack, hFrame);
	if(result == XENON_SUCCESS)
	{
		hExec->hCurrentFrame = hFrame;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecution::PopFrame(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	XenonFrameHandle hFrame = XENON_FRAME_HANDLE_NULL;
	int result = hExec->frameStack.Pop(hExec->frameStack, &hFrame);

	hExec->hCurrentFrame = (hExec->frameStack.nextIndex > 0)
		? hExec->frameStack.memory.pData[hExec->frameStack.nextIndex - 1]
		: nullptr;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------;

int XenonExecution::SetIoRegister(XenonExecutionHandle hExec, XenonValueHandle hValue, const size_t index)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(index < XENON_VM_IO_REGISTER_COUNT);

	if(index >= XENON_VM_IO_REGISTER_COUNT)
	{
		return XENON_ERROR_INDEX_OUT_OF_RANGE;
	}

	hExec->registers.pData[index] = hValue;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonExecution::GetIoRegister(XenonExecutionHandle hExec, const size_t index, int* const pOutResult)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(pOutResult != nullptr);

	if(index >= XENON_VM_IO_REGISTER_COUNT)
	{
		(*pOutResult) = XENON_ERROR_INDEX_OUT_OF_RANGE;
		return XENON_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;
	return hExec->registers.pData[index];
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::Run(XenonExecutionHandle hExec, const int runMode)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(runMode == XENON_RUN_STEP || runMode == XENON_RUN_CONTINUOUS);

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
		case XENON_RUN_STEP:
		{
			prv_runStep(hExec);
			break;
		}

		case XENON_RUN_CONTINUOUS:
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

void XenonExecution::RaiseException(XenonExecutionHandle hExec, XenonValueHandle hValue, const int severity)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(severity >= 0);
	assert(severity < XENON_EXCEPTION_SEVERITY__COUNT);

	// The exception value will always be stored to I/O register index 0.
	hExec->registers.pData[0] = hValue;

	if(severity == XENON_EXCEPTION_SEVERITY_FATAL)
	{
		// Force this exception to be unhandled.
		hExec->exception = true;
	}
	else
	{
		auto blockReverseSortFunc = [](
			XenonGuardedBlock* const pLeft,
			XenonGuardedBlock* const pRight
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
			XenonFrameHandle hFrame,
			uint32_t* pOutHandlerOffset
		) -> bool
		{
			uint32_t currentOffset = 0;
			if(XenonFrameGetBytecodeOffset(hFrame, &currentOffset) != XENON_SUCCESS)
			{
				// Failing to get the current offset within the frame means that it
				// can't possibly handle this (or any) exception.
				return false;
			}

			XenonGuardedBlock::Stack validBlocks;
			XenonGuardedBlock::Stack::Initialize(validBlocks, hFrame->hFunction->guardedBlocks.count);

			// Find all the guarded blocks that encapsulate the instruction that raised the exception.
			for(size_t blockIndex = 0; blockIndex < hFrame->hFunction->guardedBlocks.count; ++blockIndex)
			{
				XenonGuardedBlock* pBlock = hFrame->hFunction->guardedBlocks.pData[blockIndex];

				if(currentOffset >= pBlock->bytecodeOffsetStart
					&& currentOffset <= pBlock->bytecodeOffsetEnd)
				{
					XenonGuardedBlock::Stack::Push(validBlocks, pBlock);
				}
			}

			// Sort the guarded blocks in reverse order since we need to start with the most nested block first.
			std::sort(validBlocks.memory.pData, validBlocks.memory.pData + validBlocks.nextIndex, blockReverseSortFunc);

			// Search the blocks for an exception handler that is capable of handling the raised value.
			for(size_t blockIndex = 0; blockIndex < validBlocks.nextIndex; ++blockIndex)
			{
				XenonGuardedBlock* const pBlock = validBlocks.memory.pData[blockIndex];

				// Check each handler on the current block.
				for(size_t handlerIndex = 0; handlerIndex < pBlock->handlers.count; ++handlerIndex)
				{
					XenonExceptionHandler* const pHandler = pBlock->handlers.pData[handlerIndex];

					// Check if the general type of the handler matches the raised value.
					if(pHandler->type == hValue->type)
					{
						// If the value is an object, we need to also compare the class type name to
						// verify this handler will handle the exact object type that was raised.
						if(hValue->type != XENON_VALUE_TYPE_OBJECT ||
							(
								hValue->type == XENON_VALUE_TYPE_OBJECT
								&& XenonString::Compare(pHandler->pClassName, hValue->as.pObject->pTypeName)
							)
						)
						{
							// This is the handler we'll use.
							XenonGuardedBlock::Stack::Dispose(validBlocks);

							(*pOutHandlerOffset) = pHandler->offset;
							return true;
						}
					}
				}
			}

			// No acceptable handler was found for this frame.
			XenonGuardedBlock::Stack::Dispose(validBlocks);

			return false;
		};

		const size_t frameStackLength = hExec->frameStack.nextIndex;
		if(frameStackLength > 0)
		{
			// Search the frame stack from the end to find a handler for the raised value.
			size_t frameIndex = frameStackLength - 1;
			for(;; --frameIndex)
			{
				XenonFrameHandle hFrame = hExec->frameStack.memory.pData[frameIndex];

				uint32_t handlerOffset = 0;

				if(findExceptionHandler(hFrame, &handlerOffset))
				{
					// We found the frame that will handle the exception, now we need
					// to actually pop the frame stack until we get to that frame.
					while(hExec->hCurrentFrame != hFrame)
					{
						const int popFrameResult = XenonExecution::PopFrame(hExec);

						// Sanity check: There should not be any errors on popping the frame stack.
						assert(popFrameResult == XENON_SUCCESS);
						(void) popFrameResult;
					}

					// Sanity check: The current frame should never be null after popping the frame stack.
					assert(hExec->hCurrentFrame != XENON_FRAME_HANDLE_NULL);

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

void XenonExecution::RaiseFatalStandardException(XenonExecutionHandle hExec, const int type, const char* const msg)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(type >= 0);
	assert(type < XENON_STANDARD_EXCEPTION__COUNT);
	assert(msg != nullptr);

	XenonValueHandle hThrowValue = XenonVm::CreateStandardException(hExec->hVm, type, msg);

	XenonExecution::RaiseException(hExec, hThrowValue, XENON_EXCEPTION_SEVERITY_FATAL);
	XenonValue::SetAutoMark(hThrowValue, false);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::prv_runStep(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	XenonScopedReadLock gcLock(hExec->hVm->gcRwLock);

	XenonFrameHandle hFrame = hExec->hCurrentFrame;

	// Save the current instruction pointer position at the start of the opcode that will now be executed.
	hFrame->decoder.cachedIp = hFrame->decoder.ip;

	const uint8_t opCode = XenonDecoder::LoadUint8(hFrame->decoder);
	XenonVm::ExecuteOpCode(hExec->hVm, hExec, opCode);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::prv_onGcDiscovery(XenonGarbageCollector& gc, void* const pOpaque)
{
	XenonExecutionHandle hExec = reinterpret_cast<XenonExecutionHandle>(pOpaque);
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Discover all active frames in the frame stack.
	const size_t stackSize = XenonFrame::HandleStack::GetCurrentSize(hExec->frameStack);
	for(size_t i = 0; i < stackSize; ++i)
	{
		XenonFrameHandle hFrame = hExec->frameStack.memory.pData[i];

		XenonGarbageCollector::MarkObject(gc, &hFrame->gcProxy);
	}

	// Discover values held in the I/O registers.
	for(size_t i = 0; i < hExec->registers.count; ++i)
	{
		XenonValueHandle hValue = hExec->registers.pData[i];

		if(XenonValue::CanBeMarked(hValue))
		{
			XenonGarbageCollector::MarkObject(gc, &hValue->gcProxy);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::prv_onGcDestruct(void* pObject)
{
	XenonExecutionHandle hExec = reinterpret_cast<XenonExecutionHandle>(pObject);
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	XenonFrame::HandleStack::Dispose(hExec->frameStack);
	XenonValue::HandleArray::Dispose(hExec->registers);

	delete hExec;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonExecution::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
