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
#include "Vm.hpp"

#include "../base/Mutex.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

HqFrameHandle HqFrame::Create(HqExecutionHandle hExec, HqFunctionHandle hFunction)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	HqFrame* const pOutput = new HqFrame();
	assert(pOutput != HQ_FRAME_HANDLE_NULL);

	// No need to lock the garbage collector here since only the execution context is allowed to create frames
	// and it will be handling the lock for us.
	HqGcProxy::Initialize(pOutput->gcProxy, hExec->hVm->gc, prv_onGcDiscovery, prv_onGcDestruct, pOutput, false);

	pOutput->hExec = hExec;
	pOutput->hFunction = hFunction;

	if(hFunction->isNative)
	{
		// Initialize the value structures to avoid deleting garbage memory on clean up.
		// No other work needs to be done for native functions since this is intended to
		// be just a dummy frame.
		HqValue::HandleStack::Initialize(pOutput->stack, 0);
		HqValue::HandleArray::Initialize(pOutput->registers);
	}
	else
	{
		// Setup the register array.
		HqValue::HandleStack::Initialize(pOutput->stack, HQ_VM_FRAME_STACK_SIZE);
		HqValue::HandleArray::Initialize(pOutput->registers);
		HqValue::HandleArray::Reserve(pOutput->registers, HQ_VM_GP_REGISTER_COUNT);

		pOutput->registers.count = HQ_VM_GP_REGISTER_COUNT;

		// Initialize each register value.
		for(size_t i = 0; i < pOutput->registers.count; ++i)
		{
			pOutput->registers.pData[i] = HqValue::CreateNull();
		}

		// Build the local table for the new frame. This will intentionally copy each value from the function's local table
		// so any changes made the variables in the frame will not affect the prototypes in the function.
		for(auto& kv : hFunction->locals)
		{
			HqString* const pKey = HQ_MAP_ITER_KEY(kv);
			HqValueHandle hValue = HQ_MAP_ITER_VALUE(kv);

			HQ_MAP_FUNC_INSERT(pOutput->locals, pKey, HqValueCopy(hExec->hVm, hValue));
		}

		HqDecoder::Initialize(pOutput->decoder, hFunction->hProgram, hFunction->bytecodeOffsetStart);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrame::PushValue(HqFrameHandle hFrame, HqValueHandle hValue)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(hValue != HQ_VALUE_HANDLE_NULL);

	return HqValue::HandleStack::Push(hFrame->stack, hValue);
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
	assert(hValue != HQ_VALUE_HANDLE_NULL);
	assert(index < HQ_VM_GP_REGISTER_COUNT);

	hFrame->registers.pData[index] = hValue;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrame::SetLocalVariable(HqFrameHandle hFrame, HqValueHandle hValue, HqString* const pVariableName)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(hValue != HQ_VALUE_HANDLE_NULL);
	assert(pVariableName != nullptr);

	auto kv = hFrame->locals.find(pVariableName);
	if(kv == hFrame->locals.end())
	{
		return HQ_ERROR_KEY_DOES_NOT_EXIST;
	}

	HQ_MAP_ITER_PTR_VALUE(kv) = hValue;

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

HqValueHandle HqFrame::GetLocalVariable(HqFrameHandle hFrame, HqString* const pVariableName, int* const pOutResult)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(pVariableName != nullptr);
	assert(pOutResult != nullptr);

	if(!HQ_MAP_FUNC_CONTAINS(hFrame->locals, pVariableName))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return HQ_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return HQ_MAP_FUNC_GET(hFrame->locals, pVariableName);
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::prv_onGcDiscovery(HqGarbageCollector& gc, void* const pOpaque)
{
	HqFrameHandle hFrame = reinterpret_cast<HqFrameHandle>(pOpaque);
	assert(hFrame != HQ_FRAME_HANDLE_NULL);

	// Discover values in the stack.
	size_t stackSize = HqValue::HandleStack::GetCurrentSize(hFrame->stack);
	for(size_t i = 0; i < stackSize; ++i)
	{
		HqValueHandle hValue = hFrame->stack.memory.pData[i];

		if(HqValue::CanBeMarked(hValue))
		{
			HqGarbageCollector::MarkObject(gc, &hValue->gcProxy);
		}
	}

	// Discover values held in the general purpose registers.
	for(size_t i = 0; i < hFrame->registers.count; ++i)
	{
		HqValueHandle hValue = hFrame->registers.pData[i];

		if(HqValue::CanBeMarked(hValue))
		{
			HqGarbageCollector::MarkObject(gc, &hValue->gcProxy);
		}
	}

	// Discover the local variable values.
	for(auto& kv : hFrame->locals)
	{
		HqValueHandle hValue = HQ_MAP_ITER_VALUE(kv);

		if(HqValue::CanBeMarked(hValue))
		{
			HqGarbageCollector::MarkObject(gc, &hValue->gcProxy);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::prv_onGcDestruct(void* const pOpaque)
{
	HqFrameHandle hFrame = reinterpret_cast<HqFrameHandle>(pOpaque);
	assert(hFrame != HQ_FRAME_HANDLE_NULL);

	HqValue::HandleStack::Dispose(hFrame->stack);
	HqValue::HandleArray::Dispose(hFrame->registers);

	delete hFrame;
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
