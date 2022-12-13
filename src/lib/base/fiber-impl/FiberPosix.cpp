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

#include "../Fiber.hpp"

#include <assert.h>
#include <memory.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

#include <sys/mman.h>

#ifdef _HQ_ENABLE_VALGRIND
	#include <valgrind/valgrind.h>
	#ifdef MINSIGSTKSZ
		#undef MINSIGSTKSZ
		#define MINSIGSTKSZ (32 * 1024)
	#endif
#endif

//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------

struct _HqInternalFiberConfig
{
	HqInternalFiber* pObj;
	HqFiberConfig data;
};

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberEntryPoint(void* const pArgLsb, void* const pArgMsb)
{
	const uintptr_t address =
#ifdef HQ_DATA_WIDTH_64_BIT
		// Reassemble the input address.
		reinterpret_cast<uintptr_t>(pArgLsb) | (reinterpret_cast<uintptr_t>(pArgMsb) << 32);
#else
		reinterpret_cast<uintptr_t>(pArgLsb); (void) pArgMsg;
#endif

	// Copy internal fiber config.
	_HqInternalFiberConfig config = *reinterpret_cast<_HqInternalFiberConfig*>(address);

	// Now that the fiber context has been established, we can go back to finish out the initialization call.
	// The next time the fiber is run, it will resume from this point.
	if(_setjmp(config.pObj->fiberJump) == 0)
	{
		_longjmp(config.pObj->returnJump, 1);
	}

	// Call the fiber function.
	config.data.mainFn(config.data.pArg);

	config.pObj->running = false;
	config.pObj->completed = true;

	// Yield fiber execution to the context that ran it.
	_longjmp(config.pObj->returnJump, 1);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplCreate(HqInternalFiber& obj, const HqFiberConfig& fiberConfig)
{
	assert(obj.pStack == nullptr);

	const size_t pageSize = getpagesize();
	const size_t minStackSize = (MINSIGSTKSZ > pageSize) ? MINSIGSTKSZ : pageSize;
	const size_t stackSizeModPageSize = fiberConfig.stackSize % pageSize;

	// Round the stack size up to the nearest page (unless the input stack size is already aligned to the page size).
	size_t usableStackSize = (stackSizeModPageSize != 0)
		? fiberConfig.stackSize - stackSizeModPageSize + pageSize
		: fiberConfig.stackSize;

	if(usableStackSize < minStackSize)
	{
		// Cap the page size so it doesn't fall below a minimum threshold.
		usableStackSize = MINSIGSTKSZ;
	}

	// Extend the stack size by two full pages so we can do some memory protection on both ends of the stack.
	const size_t totalStackSize = usableStackSize + (pageSize * 2);

	_HqInternalFiberConfig internalConfig;
	internalConfig.pObj = &obj;
	internalConfig.data = fiberConfig;
	internalConfig.data.stackSize = usableStackSize;

	// Allocate the fiber's stack.
	const int stackAllocResult = posix_memalign(&obj.pStack, pageSize, totalStackSize);
	assert(stackAllocResult == 0); (void) stackAllocResult;

	// Disable access to the page just prior to the beginning of stack memory.
	const int stackHeadProtectResult = mprotect(obj.pStack, pageSize, PROT_NONE);
	assert(stackHeadProtectResult == 0); (void) stackHeadProtectResult;

	// Disable access to the page after the end of stack memory.
	const int stackTailProtectResult = mprotect(reinterpret_cast<uint8_t*>(obj.pStack) + pageSize + usableStackSize, pageSize, PROT_NONE);
	assert(stackTailProtectResult == 0); (void) stackTailProtectResult;

	obj.pUsableStack = reinterpret_cast<uint8_t*>(obj.pStack) + pageSize;
	obj.usableStackSize = usableStackSize;
	obj.totalStackSize = totalStackSize;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

	// Get the current context for the sake of setting up the fiber context.
	ucontext_t tempContext;
	getcontext(&tempContext);

	// Set the fiber stack to the temporary context.
	tempContext.uc_stack.ss_sp = obj.pUsableStack;
	tempContext.uc_stack.ss_size = obj.usableStackSize;

	// Save our position in the current context so we can return here after setting up the fiber context.
	if(_setjmp(obj.returnJump) == 0)
	{
		const uintptr_t argAddress = reinterpret_cast<uintptr_t>(&internalConfig);

#ifdef HQ_DATA_WIDTH_64_BIT
		// The makecontext spec states that arguments passed to the target function should be no larger than 32-bit integers
		// for portability since some platforms will only pass up to 32-bits per argument. Some Linux platforms will be fine
		// passing 64-bit arguments, but others won't (e.g., macOS targeting specifically the M1 architecture).
		const uint32_t argLsb = argAddress & 0xFFFFFFFFul;
		const uint32_t argMsb = (argAddress >> 32) & 0xFFFFFFFFul;
#else
		const uint32_t argLsb = uint32_t(argAddress);
		const uint32_t argMsg = 0;
#endif
		// Bootstrap the fiber context to get it going. After the fiber context is running,
		// we won't be using the ucontext API again. The reason is that it is extremely slow,
		// though the setjmp+longjmp APIs are much faster.
		makecontext(&tempContext, reinterpret_cast<void(*)()>(_HqFiberEntryPoint), 2, argLsb, argMsb);
		setcontext(&tempContext);
	}

#pragma GCC diagnostic pop

#ifdef _HQ_ENABLE_VALGRIND
	obj.valgrindStackId = VALGRIND_STACK_REGISTER(obj.pUsableStack, reinterpret_cast<uint8_t*>(obj.pUsableStack) + usableStackSize);
#endif
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplDispose(HqInternalFiber& obj)
{
	assert(obj.pStack != nullptr);

#ifdef _HQ_ENABLE_VALGRIND
	VALGRIND_STACK_DEREGISTER(obj.valgrindStackId);
#endif

	// Make the entire contents of the stack allocation accessible again.
	const int unprotectStackResult = mprotect(obj.pStack, obj.totalStackSize, PROT_WRITE | PROT_READ);
	assert(unprotectStackResult == 0); (void) unprotectStackResult;

	free(obj.pStack);

	obj = HqInternalFiber();
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplRun(HqInternalFiber& obj)
{
	assert(obj.pStack != nullptr);

	if(!obj.completed)
	{
		assert(!obj.running);

		// Save the point where we'll return to once the fiber yields.
		if(_setjmp(obj.returnJump) == 0)
		{
			obj.running = true;

			// Switch the active context to running the fiber.
			_longjmp(obj.fiberJump, 1);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplWait(HqInternalFiber& obj)
{
	assert(obj.pStack != nullptr);

	if(!obj.completed)
	{
		assert(obj.running);

		// Save the current point in the fiber's execution so we can return here when the fiber is resumed.
		if(_setjmp(obj.fiberJump) == 0)
		{
			obj.running = false;

			// Yield fiber execution back to the context that ran it.
			_longjmp(obj.returnJump, 1);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _HqFiberImplIsComplete(HqInternalFiber& obj)
{
	assert(obj.pStack != nullptr);

	return obj.completed;
}

//----------------------------------------------------------------------------------------------------------------------
