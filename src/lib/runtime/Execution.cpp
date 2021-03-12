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

	XenonFrame::HandleStack::Initialize(pOutput->frameStack, XENON_VM_FRAME_STACK_SIZE);
	XenonValue::HandleArray::Initialize(pOutput->registers);
	XenonValue::HandleArray::Reserve(pOutput->registers, XENON_VM_IO_REGISTER_COUNT);

	pOutput->registers.count = XENON_VM_IO_REGISTER_COUNT;

	// Initialize each value in the I/O register set.
	for(size_t i = 0; i < pOutput->registers.count; ++i)
	{
		pOutput->registers.pData[i] = XENON_VALUE_HANDLE_NULL;
	}

	// Push the entry point frame to the frame stack.
	int result = XenonFrame::HandleStack::Push(pOutput->frameStack, pOutput->hCurrentFrame);
	if(result != XENON_SUCCESS)
	{
		XenonFrame::Dispose(pOutput->hCurrentFrame);
		XenonExecution::Dispose(pOutput);

		return XENON_EXECUTION_HANDLE_NULL;
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::Dispose(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Dispose of all active frames.
	for(size_t i = 0; i < hExec->frameStack.nextIndex; ++i)
	{
		XenonFrame::Dispose(hExec->frameStack.memory.pData[i]);
	}

	// Dispose of all I/O registers.
	for(size_t i = 0; i < hExec->registers.count; ++i)
	{
		XenonValueDispose(hExec->registers.pData[i]);
	}

	XenonFrame::HandleStack::Dispose(hExec->frameStack);
	XenonValue::HandleArray::Dispose(hExec->registers);

	delete hExec;
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

void XenonExecution::SetIoRegister(XenonExecutionHandle hExec, XenonValueHandle hValue, const size_t index)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(index < XENON_VM_IO_REGISTER_COUNT);

	// Get a reference to the input value first, just in case it
	// happens to be the value already in this register slot.
	XenonValueHandle hValueRef = XenonValueReference(hValue);

	// Dispose of whatever might currently be in this register slot.
	XenonValueDispose(hExec->registers.pData[index]);

	hExec->registers.pData[index] = hValueRef;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonExecution::GetIoRegister(XenonExecutionHandle hExec, const size_t index)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(index < XENON_VM_IO_REGISTER_COUNT);

	return XenonValueReference(hExec->registers.pData[index]);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecution::ResolveFrameStack(
	XenonExecutionHandle hExec,
	XenonCallbackResolveFrame onResolveFrameFn,
	void* const pUserData
)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(onResolveFrameFn != nullptr);

	for(size_t i = 0; i < hExec->frameStack.nextIndex; ++i)
	{
		// Traverse the frame stack in reverse.
		XenonFrameHandle hFrame = hExec->frameStack.memory.pData[hExec->frameStack.nextIndex - i - 1];

		onResolveFrameFn(pUserData, hFrame);
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonExecution::RunStep(XenonExecutionHandle hExec)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	const uint8_t opCode = XenonDecoder::LoadUint8(hExec->hCurrentFrame->decoder);

	XenonVm::ExecuteOpCode(hExec->hVm, hExec, opCode);
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
