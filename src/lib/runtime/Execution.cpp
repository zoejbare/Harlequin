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
	hVm->executionContexts.Insert(pOutput, false);

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
	hVm->executionContexts.Delete(hExec);

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
