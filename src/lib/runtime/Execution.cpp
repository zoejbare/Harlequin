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
	if(!pOutput)
	{
		return XENON_EXECUTION_HANDLE_NULL;
	}

	// Create the first frame using the entry point function.
	pOutput->hCurrentFrame = XenonFrame::Create(hEntryPoint);
	if(!pOutput->hCurrentFrame)
	{
		delete pOutput;
		return XENON_EXECUTION_HANDLE_NULL;
	}

	pOutput->hVm = hVm;
	pOutput->endianness = XenonGetPlatformEndianMode();
	pOutput->yield = false;
	pOutput->started = false;
	pOutput->finished = false;
	pOutput->exception = false;

	XenonReference::Initialize(pOutput->ref, prv_onDestruct, pOutput);

	XenonFrame::HandleStack::Initialize(pOutput->frameStack, XENON_VM_FRAME_STACK_SIZE);
	XenonValue::HandleArray::Initialize(pOutput->registers);
	XenonValue::HandleArray::Reserve(pOutput->registers, XENON_VM_IO_REGISTER_COUNT);

	pOutput->registers.count = XENON_VM_IO_REGISTER_COUNT;

	// Initialize each value in the I/O register set.
	for(size_t i = 0; i < pOutput->registers.count; ++i)
	{
		pOutput->registers.pData[i] = XenonValue::CreateNull();
	}

	// Push the entry point frame to the frame stack.
	int result = XenonFrame::HandleStack::Push(pOutput->frameStack, pOutput->hCurrentFrame);
	if(result != XENON_SUCCESS)
	{
		XenonFrame::Dispose(pOutput->hCurrentFrame);
		XenonExecution::Release(pOutput);

		return XENON_EXECUTION_HANDLE_NULL;
	}

	XenonScopedMutex lock(hVm->gcLock);

	// Map the execution context to the VM used to create it.
	hVm->executionContexts.Insert(pOutput, false);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::DetachFromVm(XenonExecutionHandle hExec)
{
	XenonVmHandle hVm = hExec->hVm;

	XenonScopedMutex lock(hVm->gcLock);

	// Unlink the execution context from the VM.
	hVm->executionContexts.Delete(hExec);
	XenonExecution::Release(hExec);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::AddRef(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	XenonReference::AddRef(hExec->ref);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::Release(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	XenonReference::Release(hExec->ref);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecution::PushFrame(XenonExecutionHandle hExec, XenonFunctionHandle hFunction)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	XenonFrameHandle hFrame = XenonFrame::Create(hFunction);
	if(!hFrame)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	XenonScopedMutex lock(hExec->hVm->gcLock);

	int result = XenonFrame::HandleStack::Push(hExec->frameStack, hFrame);
	if(result != XENON_SUCCESS)
	{
		XenonFrame::Dispose(hFrame);
	}
	else
	{
		hExec->hCurrentFrame = hFrame;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecution::PopFrame(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	XenonScopedMutex lock(hExec->hVm->gcLock);

	XenonFrameHandle hFrame = XENON_FRAME_HANDLE_NULL;
	int result = hExec->frameStack.Pop(hExec->frameStack, &hFrame);

	if(hFrame)
	{
		// We don't need to do anything with the frame we popped, but it still needs to be cleaned up.
		XenonFrame::Dispose(hFrame);
	}

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

	XenonScopedMutex lock(hExec->hVm->gcLock);

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

	// Set the 'started' flag to indicate that execution has started.
	// We also reset the 'yield' flag here since it's only useful for
	// pausing execution and we no longer need it paused until the
	// next yield.
	hExec->started = true;
	hExec->yield = false;

	XenonVmHandle hVm = hExec->hVm;

	switch(runMode)
	{
		case XENON_RUN_STEP:
		{
			XenonScopedMutex lock(hVm->gcLock);

			prv_runStep(hExec);
			break;
		}

		case XENON_RUN_CONTINUOUS:
		{
			XenonScopedMutex lock(hVm->gcLock);

			while(!hExec->finished && !hExec->exception && !hExec->yield)
			{
				// TODO: Adding a timer here to release, then immediately re-acquire the gc lock after a certain
				//       amount of time to allow the garbage collector to have a chance to run during execution.
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

void XenonExecution::prv_runStep(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	XenonFrameHandle hFrame = hExec->hCurrentFrame;

	// Save the current instruction pointer position at the start of the opcode that will now be executed.
	hFrame->decoder.cachedIp = hFrame->decoder.ip;

	const uint8_t opCode = XenonDecoder::LoadUint8(hFrame->decoder);
	XenonVm::ExecuteOpCode(hExec->hVm, hExec, opCode);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::prv_onDestruct(void* pObject)
{
	XenonExecutionHandle hExec = reinterpret_cast<XenonExecutionHandle>(pObject);
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Dispose of all active frames.
	for(size_t i = 0; i < hExec->frameStack.nextIndex; ++i)
	{
		XenonFrame::Dispose(hExec->frameStack.memory.pData[i]);
	}

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
