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

#include "../common/ThreadMainCallback.hpp"

//----------------------------------------------------------------------------------------------------------------------

#if defined(XENON_PLATFORM_WINDOWS)
	#include "thread-impl/ThreadWin32.hpp"
	#undef Yield

#elif defined(XENON_PLATFORM_LINUX) \
	|| defined(XENON_PLATFORM_MAC_OS) \
	|| defined(XENON_PLATFORM_ANDROID) \
	|| defined(XENON_PLATFORM_PS4)
	#include "thread-impl/ThreadPosix.hpp"

#else
	#error "Threads not implemented for this platform"

#endif

//----------------------------------------------------------------------------------------------------------------------

struct XenonThreadConfig
{
	XenonThreadMainCallback mainFn;

	void* pArg;

	uint32_t stackSize;
	char name[64];
};

//----------------------------------------------------------------------------------------------------------------------

struct XENON_BASE_API XenonThread
{
	static XenonThread Create(const XenonThreadConfig& threadConfig);
	static void Join(XenonThread& thread, int32_t* const pOutReturnValue);
	static void Sleep(uint32_t ms);
	static void Yield();

	XenonInternalThread obj;
	uint32_t id;
};

//----------------------------------------------------------------------------------------------------------------------
