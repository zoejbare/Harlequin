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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "../Harlequin.h"

#include "Array.hpp"

//----------------------------------------------------------------------------------------------------------------------

template <typename T>
struct HqStack
{
	HqArray<T> memory;

	size_t nextIndex;

	static void Initialize(HqStack& output, const size_t size)
	{
		// Initialize the stack's backing memory and reserve the desired size.
		HqArray<T>::Initialize(output.memory);
		HqArray<T>::Reserve(output.memory, size);

		output.memory.count = size;
		output.nextIndex = 0;
	}

	static void Dispose(HqStack& stack)
	{
		HqArray<T>::Dispose(stack.memory);
	}

	static void Expand(HqStack& output, const size_t newSize)
	{
		if(output.memory.count < newSize)
		{
			HqArray<T>::Reserve(output.memory, newSize);

			output.memory.count = newSize;
		}
	}

	static int Push(HqStack& stack, T& data)
	{
		const size_t stackNextIndex = stack.nextIndex;

		// Make sure there is room in the stack for this push.
		if(stackNextIndex >= stack.memory.count)
		{
			return HQ_ERROR_STACK_FULL;
		}

		// Copy the input value to the stack to decouple the stack value's memory
		// from where ever it's coming from.
		stack.memory.pData[stackNextIndex] = data;
		stack.nextIndex = stackNextIndex + 1;

		return HQ_SUCCESS;
	}

	static int Pop(HqStack& stack, T* const pOutData)
	{
		const size_t stackNextIndex = stack.nextIndex;

		// Make sure the stack is not empty before attempting to pop from it.
		if(stackNextIndex == 0)
		{
			return HQ_ERROR_STACK_EMPTY;
		}

		const size_t stackTopIndex = stackNextIndex - 1;

		(*pOutData) = stack.memory.pData[stackTopIndex];
		stack.nextIndex = stackTopIndex;

		return HQ_SUCCESS;
	}

	static int Pop(HqStack& stack)
	{
		// Make sure the stack is not empty before attempting to pop from it.
		if(stack.nextIndex == 0)
		{
			return HQ_ERROR_STACK_EMPTY;
		}

		--stack.nextIndex;

		return HQ_SUCCESS;
	}

	static int Peek(HqStack& stack, T* const pOutData, const size_t index)
	{
		if(stack.nextIndex == 0)
		{
			return HQ_ERROR_STACK_EMPTY;
		}

		if(index >= stack.nextIndex)
		{
			return HQ_ERROR_INDEX_OUT_OF_RANGE;
		}

		// The input index will access the stack in reverse order and relative to the end.
		// From the user's perspective, this will simulate accurate stack behavior.
		const size_t stackPeekIndex = stack.nextIndex - index - 1;

		(*pOutData) = stack.memory.pData[stackPeekIndex];

		return HQ_SUCCESS;
	}

	static size_t GetCurrentSize(HqStack& stack)
	{
		return stack.nextIndex;
	}

	static size_t GetMaximumSize(HqStack& stack)
	{
		return stack.memory.count;
	}

	static bool IsEmpty(HqStack& stack)
	{
		return stack.nextIndex == 0;
	}
};

//----------------------------------------------------------------------------------------------------------------------
