//
// Copyright (c) 2022, Zoe J. Bare
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

#ifdef __APPLE__
	#define _XOPEN_SOURCE
#endif

#pragma push_macro("_FORTIFY_SOURCE")
#pragma push_macro("__USE_FORTIFY_LEVEL")

// Remove fortification for the setjump/longjmp calls because they'll raise SIGABRT incorrectly.
// The reason is because they think the stack gets messed up by jumping around between contexts
// like we do, however this is a false positive for us because the fiber context already has its
// own dedicated stack allocated on the heap.
#undef _FORTIFY_SOURCE
#undef __USE_FORTIFY_LEVEL
#define __USE_FORTIFY_LEVEL 0

#include <setjmp.h>

#pragma pop_macro("__USE_FORTIFY_LEVEL")
#pragma pop_macro("_FORTIFY_SOURCE")

//----------------------------------------------------------------------------------------------------------------------

struct HQ_BASE_API HqInternalFiber
{
	HqInternalFiber()
		: pStack(nullptr)
		, pUsableStack(nullptr)
		, fiberJump()
		, returnJump()
		, usableStackSize(0)
		, totalStackSize(0)
#ifdef _HQ_ENABLE_VALGRIND
		, valgrindStackId(-1)
#endif
		, running(false)
		, completed(false)
	{
	}

	void* pStack;
	void* pUsableStack;

	jmp_buf fiberJump;
	jmp_buf returnJump;

	size_t usableStackSize;
	size_t totalStackSize;

#ifdef _HQ_ENABLE_VALGRIND
	int valgrindStackId;
#endif

	volatile bool running;
	volatile bool completed;
};

//----------------------------------------------------------------------------------------------------------------------
