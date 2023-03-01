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

#include "../Harlequin.h"

#include "../common/FiberMainCallback.hpp"

//----------------------------------------------------------------------------------------------------------------------

#if defined(HQ_PLATFORM_WINDOWS)
	#include "fiber-impl/FiberWin32.hpp"

#elif defined(HQ_PLATFORM_LINUX) \
	|| defined(HQ_PLATFORM_MAC_OS) \
	|| defined(HQ_PLATFORM_ANDROID)
	#include "fiber-impl/FiberPosix.hpp"

#elif defined(HQ_PLATFORM_PS3)
	#include "../../../../Harlequin-PS3/lib/base/Fiber.hpp"

#elif defined(HQ_PLATFORM_PS4) || defined(HQ_PLATFORM_PS5)
	#include "../../../../Harlequin-PS4/lib/base/Fiber.hpp"

#elif defined(HQ_PLATFORM_PSVITA)
	#include "../../../../Harlequin-PSVita/lib/base/Fiber.hpp"

#else
	#error "HqFiber not implemented for this platform"

#endif

//----------------------------------------------------------------------------------------------------------------------

struct HqFiberConfig
{
	HqFiberMainCallback mainFn;

	void* pArg;

	size_t stackSize;
	char name[64];
};

//----------------------------------------------------------------------------------------------------------------------

extern "C"
{
	void _HqFiberImplCreate(HqInternalFiber&, const HqFiberConfig&);
	void _HqFiberImplDispose(HqInternalFiber&);
	bool _HqFiberImplRun(HqInternalFiber&);
	bool _HqFiberImplWait(HqInternalFiber&);
	bool _HqFiberImplIsRunning(HqInternalFiber&);
	bool _HqFiberImplIsComplete(HqInternalFiber&);
}

//----------------------------------------------------------------------------------------------------------------------

struct HQ_BASE_API HqFiber
{
	static void Create(HqFiber& fiber, const HqFiberConfig& fiberConfig)
	{
		_HqFiberImplCreate(fiber.obj, fiberConfig);
	}

	static void Dispose(HqFiber& fiber)
	{
		_HqFiberImplDispose(fiber.obj);
	}

	static bool Run(HqFiber& fiber)
	{
		return _HqFiberImplRun(fiber.obj);
	}

	static bool Wait(HqFiber& fiber)
	{
		return _HqFiberImplWait(fiber.obj);
	}

	static bool IsRunning(HqFiber& fiber)
	{
		return _HqFiberImplIsRunning(fiber.obj);
	}

	static bool IsComplete(HqFiber& fiber)
	{
		return _HqFiberImplIsComplete(fiber.obj);
	}

	HqInternalFiber obj;
};

//----------------------------------------------------------------------------------------------------------------------
