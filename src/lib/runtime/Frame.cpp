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
#include "Program.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonFrameHandle XenonFrame::Create(XenonFunctionHandle hFunction)
{
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	XenonFrame* const pOutput = new XenonFrame();

	// Setup the register array.
	XenonValue::HandleStack::Initialize(pOutput->stack, XENON_VM_FRAME_STACK_SIZE);
	XenonValue::HandleArray::Initialize(pOutput->registers);
	XenonValue::HandleArray::Reserve(pOutput->registers, XENON_VM_GP_REGISTER_COUNT);

	pOutput->registers.count = XENON_VM_GP_REGISTER_COUNT;

	// Initialize each register value.
	for(size_t i = 0; i < pOutput->registers.count; ++i)
	{
		pOutput->registers.pData[i] = XENON_VALUE_HANDLE_NULL;
	}

	// Build the local table for the new frame. This will intentionally copy each value from the function's local table
	// so any changes made the variables in the frame will not affect the prototypes in the function.
	for(auto& kv : hFunction->locals)
	{
		pOutput->locals.Insert(kv.key, XenonValueCopy(kv.value));
	}

	pOutput->hFunction = hFunction;
	pOutput->decoder = XenonDecoder::Construct(hFunction->hProgram, hFunction->offset);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonFrame::Dispose(XenonFrameHandle hFrame)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);

	// Dispose of any values left over in the stack.
	for(size_t i = 0; i < hFrame->stack.nextIndex; ++i)
	{
		XenonValueDispose(hFrame->stack.memory.pData[i]);
	}

	// Dispose of the values in the registers.
	for(size_t i = 0; i < hFrame->registers.count; ++i)
	{
		XenonValueDispose(hFrame->registers.pData[i]);
	}

	// Dispose of the values used to back the locals.
	for(auto& kv : hFrame->locals)
	{
		XenonValueDispose(kv.value);
	}

	XenonValue::HandleStack::Dispose(hFrame->stack);
	XenonValue::HandleArray::Dispose(hFrame->registers);

	delete hFrame;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::PushValue(XenonFrameHandle hFrame, XenonValueHandle hValue)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(hValue != XENON_VALUE_HANDLE_NULL);

	// Track the value by adding a reference.
	XenonValueHandle hValueRef = XenonValueReference(hValue);

	int result = XenonValue::HandleStack::Push(hFrame->stack, hValueRef);
	if(result != XENON_SUCCESS)
	{
		// If the value could not be pushed, dispose of the reference to avoid leaking memory.
		XenonValueDispose(hValueRef);
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::PopValue(XenonFrameHandle hFrame, XenonValueHandle* const phOutValue)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(phOutValue != nullptr);
	assert(*phOutValue == XENON_VALUE_HANDLE_NULL);

	// Pop the stack, returning the value that was popped. The calling code will be responsible for releasing it.
	return XenonValue::HandleStack::Pop(hFrame->stack, phOutValue);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::PeekValue(
	XenonFrameHandle hFrame,
	XenonValueHandle* const phOutValue,
	const size_t index
)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(phOutValue != nullptr);
	assert(*phOutValue == XENON_VALUE_HANDLE_NULL);

	XenonValueHandle hValueRef;

	int result = XenonValue::HandleStack::Peek(hFrame->stack, &hValueRef, index);
	if(result != XENON_SUCCESS)
	{
		return result;
	}

	// Take a reference to the peeked value to make sure it stays alive if it gets popped from the stack.
	(*phOutValue) = XenonValueReference(hValueRef);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::SetGpRegister(XenonFrameHandle hFrame, XenonValueHandle hValue, const uint32_t index)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(hValue != XENON_VALUE_HANDLE_NULL);
	assert(index < XENON_VM_GP_REGISTER_COUNT);

	// Get a reference to the input value first, just in case it
	// happens to be the value already in this register slot.
	XenonValueHandle hValueRef = XenonValueReference(hValue);

	// Dispose of whatever might currently be in this register slot.
	XenonValueDispose(hFrame->registers.pData[index]);

	hFrame->registers.pData[index] = hValueRef;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrame::SetLocalVariable(XenonFrameHandle hFrame, XenonValueHandle hValue, XenonString* const pVariableName)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(hValue != XENON_VALUE_HANDLE_NULL);
	assert(pVariableName != nullptr);

	auto kv = hFrame->locals.find(pVariableName);
	if(kv == hFrame->locals.end())
	{
		return XENON_ERROR_KEY_DOES_NOT_EXIST;
	}

	XenonValueHandle hValueRef = XenonValueReference(hValue);
	XenonValueHandle hOldValue = kv->value;

	XenonValueDispose(hOldValue);

	kv->value = hValueRef;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonFrame::GetGpRegister(XenonFrameHandle hFrame, const uint32_t index, int* const pOutResult)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(pOutResult != nullptr);

	if(index >= XENON_VM_GP_REGISTER_COUNT)
	{
		(*pOutResult) = XENON_ERROR_INDEX_OUT_OF_RANGE;
		return XENON_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;
	return XenonValueReference(hFrame->registers.pData[index]);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonFrame::GetLocalVariable(XenonFrameHandle hFrame, XenonString* const pVariableName, int* const pOutResult)
{
	assert(hFrame != XENON_FRAME_HANDLE_NULL);
	assert(pVariableName != nullptr);
	assert(pOutResult != nullptr);

	if(!hFrame->locals.Contains(pVariableName))
	{
		(*pOutResult) = XENON_ERROR_KEY_DOES_NOT_EXIST;
		return XENON_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;
	return XenonValueReference(hFrame->locals.Get(pVariableName));
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonFrame::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonFrame::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
