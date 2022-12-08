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

#include "../common/ThreadMainCallback.hpp"

//----------------------------------------------------------------------------------------------------------------------

#if defined(HQ_PLATFORM_WINDOWS)
	#include "thread-impl/ThreadWin32.hpp"
	#undef Yield

#elif defined(HQ_PLATFORM_LINUX) \
	|| defined(HQ_PLATFORM_MAC_OS) \
	|| defined(HQ_PLATFORM_ANDROID) \
	|| defined(HQ_PLATFORM_PS4) \
	|| defined(HQ_PLATFORM_PS5)
	#include "thread-impl/ThreadPosix.hpp"

#elif defined(HQ_PLATFORM_PS3)
	#include "../../../_support/Harlequin-PS3/lib/base/thread/Thread.hpp"

#elif defined(HQ_PLATFORM_PSVITA)
	#include "../../../_support/Harlequin-PSVita/lib/base/thread/Thread.hpp"

#else
	#error "HqThread not implemented for this platform"

#endif

//----------------------------------------------------------------------------------------------------------------------

struct HqThreadConfig
{
	HqThreadMainCallback mainFn;

	void* pArg;

	uint32_t stackSize;
	char name[64];
};

//----------------------------------------------------------------------------------------------------------------------

extern "C"
{
	void _HqThreadImplCreate(HqInternalThread&, const HqThreadConfig&);
	void _HqThreadImplGetCurrentThread(HqInternalThread&);
	void _HqThreadImplJoin(HqInternalThread&, int32_t*);
	void _HqThreadImplSleep(uint32_t);
	void _HqThreadImplYield();

	bool _HqThreadImplEqual(const HqInternalThread&, const HqInternalThread&);
	bool _HqThreadImplIsInitialized(const HqInternalThread&);
	bool _HqThreadImplIsReal(const HqInternalThread&);
}

//----------------------------------------------------------------------------------------------------------------------

struct HQ_BASE_API HqThread
{
	static void Create(HqThread& thread, const HqThreadConfig& threadConfig)
	{
		_HqThreadImplCreate(thread.obj, threadConfig);
	}

	static HqThread GetCurrentThread()
	{
		HqThread output;
		_HqThreadImplGetCurrentThread(output.obj);
		return output;
	}

	static void Join(HqThread& thread, int32_t* const pOutReturnValue)
	{
		_HqThreadImplJoin(thread.obj, pOutReturnValue);
	}

	static void Sleep(uint32_t ms)
	{
		_HqThreadImplSleep(ms);
	}

	static void Yield()
	{
		_HqThreadImplYield();
	}

	static bool IsInitialized(const HqThread& thread)
	{
		return _HqThreadImplIsInitialized(thread.obj);
	}

	static bool IsReal(const HqThread& thread)
	{
		return _HqThreadImplIsReal(thread.obj);
	}

	bool operator==(const HqThread& other) const
	{
		return _HqThreadImplEqual(obj, other.obj);
	}

	bool operator!=(const HqThread& other) const
	{
		return !_HqThreadImplEqual(obj, other.obj);
	}

	HqInternalThread obj;
};

//----------------------------------------------------------------------------------------------------------------------
