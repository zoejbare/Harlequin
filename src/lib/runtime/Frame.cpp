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

	// No need to lock the garbage collector here since only the execution context is allowed to create frames
	// and it will be handling the lock for us.
	HqGcProxy::Initialize(pOutput->gcProxy, hExec->hVm->gc, prv_onGcDiscovery, prv_onGcDestruct, pOutput, false, true);

	pOutput->hExec = hExec;

	// Initialize the value stack and registers.
	HqValue::HandleStack::Initialize(pOutput->stack, HQ_VM_FRAME_STACK_SIZE);
	HqValue::HandleArray::Initialize(pOutput->registers);

	// Reserve enough space for all the registers.
	HqValue::HandleArray::Reserve(pOutput->registers, HQ_VM_GP_REGISTER_COUNT);

	pOutput->stack.nextIndex = 0;
	pOutput->registers.count = HQ_VM_GP_REGISTER_COUNT;

	// Allocate the local variable map.
	HqValue::StringToHandleMap::Allocate(pOutput->locals);

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
	if(!hFunction->isNative)
	{
		// Build the local table for the new frame. This will intentionally copy each value from the function's local table
		// so any changes made the variables in the frame will not affect the prototypes in the function.
		HqFunction::StringToBoolMap::Iterator iter;
		while(HqFunction::StringToBoolMap::IterateNext(hFunction->locals, iter))
		{
			HqString* const pKey = iter.pData->key;
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;

			HqValue::StringToHandleMap::Insert(hFrame->locals, pKey, hValue);
		}

		HqDecoder::Initialize(hFrame->decoder, hFunction->hModule, hFunction->bytecodeOffsetStart);

		// The frame is now active, so we need to assume it can have GC data references at any time.
		hFrame->active = true;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::Reset(HqFrameHandle hFrame)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);

	hFrame->hFunction = HQ_FUNCTION_HANDLE_NULL;

	// Initialize each register value.
	memset(hFrame->registers.pData, 0, sizeof(HqValueHandle) * hFrame->registers.count);

	// Remove all local variables.
	HqValue::StringToHandleMap::Clear(hFrame->locals);

	// When the frame has been reset, it implicitly references no GC data.
	hFrame->active = false;
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

int HqFrame::SetLocalVariable(HqFrameHandle hFrame, HqValueHandle hValue, HqString* const pVariableName)
{
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	assert(pVariableName != nullptr);

	if(!HqValue::StringToHandleMap::Set(hFrame->locals, pVariableName, hValue))
	{
		return HQ_ERROR_KEY_DOES_NOT_EXIST;
	}

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

	HqValueHandle hOutput = HQ_VALUE_HANDLE_NULL;
	if(!HqValue::StringToHandleMap::Get(hFrame->locals, pVariableName, hOutput))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return HQ_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return hOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::prv_onGcDiscovery(HqGarbageCollector& gc, void* const pOpaque)
{
	HqFrameHandle hFrame = reinterpret_cast<HqFrameHandle>(pOpaque);
	assert(hFrame != HQ_FRAME_HANDLE_NULL);
	
	if(hFrame->active)
	{
		// Discover values in the stack.
		size_t stackSize = HqValue::HandleStack::GetCurrentSize(hFrame->stack);
		for(size_t i = 0; i < stackSize; ++i)
		{
			HqValueHandle hValue = hFrame->stack.memory.pData[i];

			if(hValue)
			{
				HqGarbageCollector::MarkObject(gc, &hValue->gcProxy);
			}
		}

		// Discover values held in the general purpose registers.
		for(size_t i = 0; i < hFrame->registers.count; ++i)
		{
			HqValueHandle hValue = hFrame->registers.pData[i];

			if(hValue)
			{
				HqGarbageCollector::MarkObject(gc, &hValue->gcProxy);
			}
		}

		// Discover the local variable values.
		{
			HqValue::StringToHandleMap::Iterator iter;
			while(HqValue::StringToHandleMap::IterateNext(hFrame->locals, iter))
			{
				HqValueHandle hValue = iter.pData->value;

				if(hValue)
				{
					HqGarbageCollector::MarkObject(gc, &hValue->gcProxy);
				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqFrame::prv_onGcDestruct(void* const pOpaque)
{
	HqFrameHandle hFrame = reinterpret_cast<HqFrameHandle>(pOpaque);
	assert(hFrame != HQ_FRAME_HANDLE_NULL);

	// Dispose of the value stack and registers.
	HqValue::HandleStack::Dispose(hFrame->stack);
	HqValue::HandleArray::Dispose(hFrame->registers);

	// Dispose of the local variable map.
	HqValue::StringToHandleMap::Dispose(hFrame->locals);

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
