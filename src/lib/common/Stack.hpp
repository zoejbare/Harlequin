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

#include "../XenonScript.h"

#include "Array.hpp"

//----------------------------------------------------------------------------------------------------------------------

template <typename T>
struct XenonStack
{
	XenonArray<T> memory;

	size_t nextIndex;

	static void Initialize(XenonStack& output, const size_t size)
	{
		// Initialize the stack's backing memory and reserve the desired size.
		XenonArray<T>::Initialize(output.memory);
		XenonArray<T>::Reserve(output.memory, size);

		output.memory.count = size;
		output.nextIndex = 0;
	}

	static void Dispose(XenonStack& stack)
	{
		XenonArray<T>::Dispose(stack.memory);
	}

	static int Push(XenonStack& stack, T& data)
	{
		const size_t stackNextIndex = stack.nextIndex;

		// Make sure there is room in the stack for this push.
		if(stackNextIndex >= stack.memory.count)
		{
			return XENON_ERROR_STACK_FULL;
		}

		// Copy the input value to the stack to decouple the stack value's memory
		// from where ever it's coming from.
		stack.memory.pData[stackNextIndex] = data;
		stack.nextIndex = stackNextIndex + 1;

		return XENON_SUCCESS;
	}

	static int Pop(XenonStack& stack, T* const pOutData)
	{
		const size_t stackNextIndex = stack.nextIndex;

		// Make sure the stack is not empty before attempting to pop from it.
		if(stackNextIndex == 0)
		{
			return XENON_ERROR_STACK_EMPTY;
		}

		const size_t stackTopIndex = stackNextIndex - 1;

		(*pOutData) = stack.memory.pData[stackTopIndex];
		stack.nextIndex = stackTopIndex;

		return XENON_SUCCESS;
	}

	static int Pop(XenonStack& stack)
	{
		// Make sure the stack is not empty before attempting to pop from it.
		if(stack.nextIndex == 0)
		{
			return XENON_ERROR_STACK_EMPTY;
		}

		--stack.nextIndex;

		return XENON_SUCCESS;
	}

	static int Peek(XenonStack& stack, T* const pOutData, const size_t index)
	{
		if(stack.nextIndex == 0)
		{
			return XENON_ERROR_STACK_EMPTY;
		}

		if(index >= stack.nextIndex)
		{
			return XENON_ERROR_INDEX_OUT_OF_RANGE;
		}

		// The input index will access the stack in reverse order and relative to the end.
		// From the user's perspective, this will simulate accurate stack behavior.
		const size_t stackPeekIndex = stack.nextIndex - index - 1;

		(*pOutData) = stack.memory.pData[stackPeekIndex];

		return XENON_SUCCESS;
	}

	static size_t GetCurrentSize(XenonStack& stack)
	{
		return stack.nextIndex;
	}

	static size_t GetMaximumSize(XenonStack& stack)
	{
		return stack.memory.count;
	}

	static bool IsEmpty(XenonStack& stack)
	{
		return stack.nextIndex == 0;
	}
};

//----------------------------------------------------------------------------------------------------------------------
