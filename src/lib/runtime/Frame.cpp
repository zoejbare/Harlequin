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

#include "Frame.hpp"
#include "Module.hpp"
#include "Vm.hpp"

#include "../base/Mutex.hpp"

#include <assert.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

HqFrame* HqFrame::Create(HqExecutionHandle hExec)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	HqFrame* const pOutput = new HqFrame();
	assert(pOutput != nullptr);

	pOutput->hExec = hExec;

	// Initialize the value stack and registers.
	HqValue::HandleStack::Initialize(pOutput->stack, HQ_VM_FRAME_STACK_SIZE);
	HqValue::HandleArray::Initialize(pOutput->registers);
	HqValue::HandleArray::Initialize(pOutput->variables);

	// Reserve enough space for all the registers.
	HqValue::HandleArray::Reserve(pOutput->registers, HQ_VM_GP_REGISTER_COUNT);
	HqValue::HandleArray::Reserve(pOutput->variables, HQ_VM_VR_REGISTER_COUNT);

	pOutput->stack.nextIndex = 0;
	pOutput->registers.count = HQ_VM_GP_REGISTER_COUNT;

	// Reset the state of the value stack and registers.
	Reset(pOutput);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::Initialize(HqFrameHandle hFrame, HqFunctionHandle hFunction)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	hFrame->hFunction = hFunction;

	// Native functions are effectively represented as dummy frames, so they need no other initialization.
	if(hFunction->type != HqFunction::Type::Native)
	{
		HqDecoder::Initialize(hFrame->decoder, hFunction->hModule->code.pData, hFunction->bytecodeOffsetStart);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::Dispose(HqFrameHandle hFrame)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);

	// Dispose of the value stack and registers.
	HqValue::HandleStack::Dispose(hFrame->stack);
	HqValue::HandleArray::Dispose(hFrame->registers);
	HqValue::HandleArray::Dispose(hFrame->variables);

	delete hFrame;
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::Reset(HqFrameHandle hFrame)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);

	hFrame->hFunction = HQ_FUNCTION_HANDLE_NULL;

	// Clear the stack and registers to prevent the garbage collector from
	// attempting to collect non-null values that happen to be garbage memory
	// or old values from a previous frame.
	memset(hFrame->stack.memory.pData, 0, sizeof(HqValueHandle) * HQ_VM_VALUE_STACK_SIZE);
	memset(hFrame->registers.pData, 0, sizeof(HqValueHandle) * HQ_VM_GP_REGISTER_COUNT);
	memset(hFrame->variables.pData, 0, sizeof(HqValueHandle) * HQ_VM_VR_REGISTER_COUNT);
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrame::PushValue(HqFrameHandle hFrame, HqValueHandle hValue)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);

	HqValueHandle hValueToPush = hValue;

	return HqValue::HandleStack::Push(hFrame->stack, hValueToPush);
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrame::PopValue(HqFrameHandle hFrame, HqValueHandle* const phOutValue)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(phOutValue != nullptr);
	assert(*phOutValue == HQ_VALUE_HANDLE_NULL);

	// Pop the stack, returning the value that was popped. The calling code will be responsible for releasing it.
	return HqValue::HandleStack::Pop(hFrame->stack, phOutValue);
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrame::PeekValue(
	HqFrameHandle hFrame,
	HqValueHandle* const phOutValue,
	const size_t index
)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(phOutValue != nullptr);
	assert(*phOutValue == HQ_VALUE_HANDLE_NULL);

	HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;

	int result = HqValue::HandleStack::Peek(hFrame->stack, &hValue, index);
	if(result != HQ_SUCCESS)
	{
		return result;
	}

	(*phOutValue) = hValue;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrame::SetGpRegister(HqFrameHandle hFrame, HqValueHandle hValue, const uint32_t index)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(index < HQ_VM_GP_REGISTER_COUNT);

	hFrame->registers.pData[index] = hValue;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrame::SetVrRegister(HqFrameHandle hFrame, HqValueHandle hValue, const uint32_t index)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(index < HQ_VM_VR_REGISTER_COUNT);

	hFrame->variables.pData[index] = hValue;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqFrame::GetGpRegister(HqFrameHandle hFrame, const uint32_t index, int* const pOutResult)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(pOutResult != nullptr);

	if(index >= HQ_VM_GP_REGISTER_COUNT)
	{
		(*pOutResult) = HQ_ERROR_INDEX_OUT_OF_RANGE;
		return HQ_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return hFrame->registers.pData[index];
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqFrame::GetVrRegister(HqFrameHandle hFrame, const uint32_t index, int* const pOutResult)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(pOutResult != nullptr);

	if(index >= HQ_VM_VR_REGISTER_COUNT)
	{
		(*pOutResult) = HQ_ERROR_INDEX_OUT_OF_RANGE;
		return HQ_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return hFrame->variables.pData[index];
}

//----------------------------------------------------------------------------------------------------------------------

void* HqFrame::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
