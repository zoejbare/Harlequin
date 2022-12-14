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
	#include "rwlock-impl/RwLockWin32.hpp"

#elif defined(HQ_PLATFORM_LINUX) \
	|| defined(HQ_PLATFORM_MAC_OS) \
	|| defined(HQ_PLATFORM_ANDROID)
	#include "rwlock-impl/RwLockPosix.hpp"

#elif defined(HQ_PLATFORM_PS3)
	#include "../../../_support/ps3/lib/base/RwLock.hpp"

#elif defined(HQ_PLATFORM_PS4) || defined(HQ_PLATFORM_PS5)
	#include "../../../_support/ps4/lib/base/RwLock.hpp"

#elif defined(HQ_PLATFORM_PSVITA)
	#include "../../../_support/psvita/lib/base/RwLock.hpp"

#else
	#error "HqRwLock not implemented for this platform"

#endif

//----------------------------------------------------------------------------------------------------------------------

extern "C"
{
	void _HqRwLockImplCreate(HqInternalRwLock&);
	void _HqRwLockImplDispose(HqInternalRwLock&);

	bool _HqRwLockImplTryReadLock(HqInternalRwLock&);
	void _HqRwLockImplReadLock(HqInternalRwLock&);
	void _HqRwLockImplReadUnlock(HqInternalRwLock&);

	bool _HqRwLockImplTryWriteLock(HqInternalRwLock&);
	void _HqRwLockImplWriteLock(HqInternalRwLock&);
	void _HqRwLockImplWriteUnlock(HqInternalRwLock&);
}

//----------------------------------------------------------------------------------------------------------------------

struct HQ_BASE_API HqRwLock
{
	static void Create(HqRwLock& rwLock)
	{
		_HqRwLockImplCreate(rwLock.obj);
	}

	static void Dispose(HqRwLock& rwlock)
	{
		_HqRwLockImplDispose(rwlock.obj);
	}

	static bool TryReadLock(HqRwLock& rwlock)
	{
		return _HqRwLockImplTryReadLock(rwlock.obj);
	}

	static void ReadLock(HqRwLock& rwlock)
	{
		_HqRwLockImplReadLock(rwlock.obj);
	}

	static void ReadUnlock(HqRwLock& rwlock)
	{
		_HqRwLockImplReadUnlock(rwlock.obj);
	}

	static bool TryWriteLock(HqRwLock& rwlock)
	{
		return _HqRwLockImplTryWriteLock(rwlock.obj);
	}

	static void WriteLock(HqRwLock& rwlock)
	{
		_HqRwLockImplWriteLock(rwlock.obj);
	}

	static void WriteUnlock(HqRwLock& rwlock)
	{
		_HqRwLockImplWriteUnlock(rwlock.obj);
	}

	HqInternalRwLock obj;
};

//----------------------------------------------------------------------------------------------------------------------

class HQ_BASE_API HqScopedReadLock
{
public:

	HqScopedReadLock() = delete;
	HqScopedReadLock(const HqScopedReadLock&) = delete;
	HqScopedReadLock(HqScopedReadLock&&) = delete;

	explicit HqScopedReadLock(HqRwLock& rwlock, const bool condition = true)
		: m_pRwLock(&rwlock)
		, m_condition(condition)
	{
		if(m_condition)
		{
			HqRwLock::ReadLock(*m_pRwLock);
		}
	}

	~HqScopedReadLock()
	{
		if(m_condition)
		{
			HqRwLock::ReadUnlock(*m_pRwLock);
		}
	}


private:

	HqRwLock* m_pRwLock;
	bool m_condition;
};

//----------------------------------------------------------------------------------------------------------------------

class HQ_BASE_API HqScopedWriteLock
{
public:

	HqScopedWriteLock() = delete;
	HqScopedWriteLock(const HqScopedWriteLock&) = delete;
	HqScopedWriteLock(HqScopedWriteLock&&) = delete;

	explicit HqScopedWriteLock(HqRwLock& rwlock, const bool condition = true)
		: m_pRwLock(&rwlock)
		, m_condition(condition)
	{
		if(m_condition)
		{
			HqRwLock::WriteLock(*m_pRwLock);
		}
	}

	~HqScopedWriteLock()
	{
		if(m_condition)
		{
			HqRwLock::WriteUnlock(*m_pRwLock);
		}
	}


private:

	HqRwLock* m_pRwLock;
	bool m_condition;
};

//----------------------------------------------------------------------------------------------------------------------
