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

//----------------------------------------------------------------------------------------------------------------------

#if defined(HQ_PLATFORM_WINDOWS)
	#include "mutex-impl/MutexWin32.hpp"

#elif defined(HQ_PLATFORM_LINUX) \
	|| defined(HQ_PLATFORM_MAC_OS) \
	|| defined(HQ_PLATFORM_ANDROID)
	#include "mutex-impl/MutexPosix.hpp"

#elif defined(HQ_PLATFORM_PS3)
	#include "../../../_support/ps3/lib/base/Mutex.hpp"

#elif defined(HQ_PLATFORM_PS4) || defined(HQ_PLATFORM_PS5)
	#include "../../../_support/ps4/lib/base/Mutex.hpp"

#elif defined(HQ_PLATFORM_PSVITA)
	#include "../../../_support/psvita/lib/base/Mutex.hpp"

#else
	#error "HqMutex not implemented for this platform"

#endif

//----------------------------------------------------------------------------------------------------------------------

extern "C"
{
	void _HqMutexImplCreate(HqInternalMutex&);
	void _HqMutexImplDispose(HqInternalMutex&);
	bool _HqMutexImplTryLock(HqInternalMutex&);
	void _HqMutexImplLock(HqInternalMutex&);
	void _HqMutexImplUnlock(HqInternalMutex&);
}

//----------------------------------------------------------------------------------------------------------------------

struct HQ_BASE_API HqMutex
{
	static void Create(HqMutex& mutex)
	{
		_HqMutexImplCreate(mutex.obj);
	}

	static void Dispose(HqMutex& mutex)
	{
		_HqMutexImplDispose(mutex.obj);
	}

	static bool TryLock(HqMutex& mutex)
	{
		return _HqMutexImplTryLock(mutex.obj);
	}

	static void Lock(HqMutex& mutex)
	{
		_HqMutexImplLock(mutex.obj);
	}

	static void Unlock(HqMutex& mutex)
	{
		_HqMutexImplUnlock(mutex.obj);
	}

	HqInternalMutex obj;
};

//----------------------------------------------------------------------------------------------------------------------

class HQ_BASE_API HqScopedMutex
{
public:

	HqScopedMutex() = delete;
	HqScopedMutex(const HqScopedMutex&) = delete;
	HqScopedMutex(HqScopedMutex&&) = delete;

	explicit inline HqScopedMutex(HqMutex& mutex, const bool condition = true)
		: m_pMutex(&mutex)
		, m_condition(condition)
	{
		if(m_condition)
		{
			HqMutex::Lock(*m_pMutex);
		}
	}

	inline ~HqScopedMutex()
	{
		if(m_condition)
		{
			HqMutex::Unlock(*m_pMutex);
		}
	}


private:

	HqMutex* m_pMutex;
	bool m_condition;
};

//----------------------------------------------------------------------------------------------------------------------
