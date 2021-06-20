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

//----------------------------------------------------------------------------------------------------------------------

#if defined(XENON_PLATFORM_WINDOWS)
	#include "mutex-impl/MutexWin32.hpp"

#elif defined(XENON_PLATFORM_LINUX) \
	|| defined(XENON_PLATFORM_MAC_OS) \
	|| defined(XENON_PLATFORM_ANDROID) \
	|| defined(XENON_PLATFORM_PS4)
	#include "mutex-impl/MutexPosix.hpp"

#elif defined(XENON_PLATFORM_PS3)
	#include "../../../../XenonScriptImpl-PS3/lib/base/mutex/Mutex.hpp"

#else
	#error "XenonMutex not implemented for this platform"

#endif

//----------------------------------------------------------------------------------------------------------------------

extern "C"
{
	void _XenonMutexImplCreate(XenonInternalMutex&);
	void _XenonMutexImplDispose(XenonInternalMutex&);
	bool _XenonMutexImplTryLock(XenonInternalMutex&);
	void _XenonMutexImplLock(XenonInternalMutex&);
	void _XenonMutexImplUnlock(XenonInternalMutex&);
}

//----------------------------------------------------------------------------------------------------------------------

struct XENON_BASE_API XenonMutex
{
	static XenonMutex Create()
	{
		XenonMutex output;
		_XenonMutexImplCreate(output.obj);
		return output;
	}

	static void Dispose(XenonMutex& mutex)
	{
		_XenonMutexImplDispose(mutex.obj);
	}

	static bool TryLock(XenonMutex& mutex)
	{
		return _XenonMutexImplTryLock(mutex.obj);
	}

	static void Lock(XenonMutex& mutex)
	{
		_XenonMutexImplLock(mutex.obj);
	}

	static void Unlock(XenonMutex& mutex)
	{
		_XenonMutexImplUnlock(mutex.obj);
	}

	XenonInternalMutex obj;
};

//----------------------------------------------------------------------------------------------------------------------

class XENON_BASE_API XenonScopedMutex
{
public:

	XenonScopedMutex() = delete;
	XenonScopedMutex(const XenonScopedMutex&) = delete;
	XenonScopedMutex(XenonScopedMutex&&) = delete;

	explicit inline XenonScopedMutex(XenonMutex& mutex)
		: m_pMutex(&mutex)
	{
		XenonMutex::Lock(*m_pMutex);
	}

	inline ~XenonScopedMutex()
	{
		XenonMutex::Unlock(*m_pMutex);
	}


private:

	XenonMutex* m_pMutex;
};

//----------------------------------------------------------------------------------------------------------------------
