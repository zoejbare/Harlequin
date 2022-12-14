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

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include <Windows.h>

//----------------------------------------------------------------------------------------------------------------------

struct _HqInternalFiberConfig
{
	HqInternalFiber* pObj;
	HqFiberConfig data;
};

//----------------------------------------------------------------------------------------------------------------------

extern "C" __forceinline void* _HqWin32FiberGetSelf()
{
	void* pContext = ConvertThreadToFiber(nullptr);
	if(!pContext)
	{
		// The calling context has already been converted to a fiber.
		pContext = GetCurrentFiber();
	}

	return pContext;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" __declspec(noreturn) void WINAPI _HqWin32FiberEntryPoint(void* const pArg)
{
	// Copy internal fiber config.
	_HqInternalFiberConfig config = *reinterpret_cast<_HqInternalFiberConfig*>(pArg);

	// Bootstrapping is complete, return to finish initialization.
	assert(config.pObj->pReturnContext != nullptr);
	SwitchToFiber(config.pObj->pReturnContext);

	// Call the fiber function.
	config.data.mainFn(config.data.pArg);

	void* const pReturnContext = config.pObj->pReturnContext;
	assert(pReturnContext != nullptr);

	config.pObj->pReturnContext = nullptr;
	config.pObj->running = true;
	config.pObj->completed = true;

	// Yield fiber execution to the context that ran it.
	SwitchToFiber(pReturnContext);
	abort();
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplCreate(HqInternalFiber& obj, const HqFiberConfig& fiberConfig)
{
	assert(obj.pFiberContext == nullptr);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	const size_t minStackSize = sysInfo.dwPageSize;
	const size_t stackSizeModPageSize = fiberConfig.stackSize % sysInfo.dwPageSize;

	// Round the stack size up to the nearest page (unless the input stack size is already aligned to the page size).
	size_t usableStackSize = (stackSizeModPageSize != 0)
		? fiberConfig.stackSize - stackSizeModPageSize + sysInfo.dwPageSize
		: fiberConfig.stackSize;

	if(usableStackSize < minStackSize)
	{
		// Cap the page size so it doesn't fall below a minimum threshold.
		usableStackSize = minStackSize;
	}

	// Make a copy of the input config data for the fiber.
	_HqInternalFiberConfig internalConfig;

	internalConfig.pObj = &obj;
	internalConfig.data = fiberConfig;
	internalConfig.data.stackSize = usableStackSize;

	// Create the native fiber.
	obj.pFiberContext = CreateFiber(usableStackSize, _HqWin32FiberEntryPoint, &internalConfig);
	assert(obj.pFiberContext != nullptr);

	// Bootstrap the fiber by running the first few lines of its entry point function.
	obj.pReturnContext = _HqWin32FiberGetSelf();
	SwitchToFiber(obj.pFiberContext);

	obj.pReturnContext = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplDispose(HqInternalFiber& obj)
{
	assert(obj.pFiberContext != nullptr);

	// Clean up the internal fiber resources.
	DeleteFiber(obj.pFiberContext);

	obj = HqInternalFiber();
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplRun(HqInternalFiber& obj)
{
	assert(obj.pFiberContext != nullptr);

	if(!obj.completed)
	{
		assert(!obj.running);
		assert(obj.pReturnContext == nullptr);

		// Save the point where we'll return to once the fiber yields.
		obj.pReturnContext = _HqWin32FiberGetSelf();
		obj.running = true;

		// Switch the active context to running the fiber.
		SwitchToFiber(obj.pFiberContext);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplWait(HqInternalFiber& obj)
{
	assert(obj.pFiberContext != nullptr);

	if(!obj.completed)
	{
		assert(obj.running);
		assert(obj.pReturnContext != nullptr);

		// Cache the return context.
		void* const pReturnContext = obj.pReturnContext;

		obj.pReturnContext = nullptr;
		obj.running = false;

		// Yield fiber execution back to the context that ran it.
		SwitchToFiber(pReturnContext);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _HqFiberImplIsRunning(HqInternalFiber& obj)
{
	assert(obj.pFiberContext != nullptr);

	return obj.running;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _HqFiberImplIsComplete(HqInternalFiber& obj)
{
	assert(obj.pFiberContext != nullptr);

	return obj.completed;
}

//----------------------------------------------------------------------------------------------------------------------
