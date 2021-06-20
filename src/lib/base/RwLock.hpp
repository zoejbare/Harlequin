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
	#include "rwlock-impl/RwLockWin32.hpp"

#elif defined(XENON_PLATFORM_LINUX) \
	|| defined(XENON_PLATFORM_MAC_OS) \
	|| defined(XENON_PLATFORM_ANDROID) \
	|| defined(XENON_PLATFORM_PS4)
	#include "rwlock-impl/RwLockPosix.hpp"

#elif defined(XENON_PLATFORM_PS3)
	#include "../../../../XenonScriptImpl-PS3/lib/base/rwlock/RwLock.hpp"

#else
	#error "XenonRwLock not implemented for this platform"

#endif

//----------------------------------------------------------------------------------------------------------------------

extern "C"
{
	void _XenonRwLockImplCreate(XenonInternalRwLock&);
	void _XenonRwLockImplDispose(XenonInternalRwLock&);

	bool _XenonRwLockImplTryReadLock(XenonInternalRwLock&);
	void _XenonRwLockImplReadLock(XenonInternalRwLock&);
	void _XenonRwLockImplReadUnlock(XenonInternalRwLock&);

	bool _XenonRwLockImplTryWriteLock(XenonInternalRwLock&);
	void _XenonRwLockImplWriteLock(XenonInternalRwLock&);
	void _XenonRwLockImplWriteUnlock(XenonInternalRwLock&);
}

//----------------------------------------------------------------------------------------------------------------------

struct XENON_BASE_API XenonRwLock
{
	static XenonRwLock Create()
	{
		XenonRwLock output;
		_XenonRwLockImplCreate(output.obj);
		return output;
	}

	static void Dispose(XenonRwLock& rwlock)
	{
		_XenonRwLockImplDispose(rwlock.obj);
	}

	static bool TryReadLock(XenonRwLock& rwlock)
	{
		return _XenonRwLockImplTryReadLock(rwlock.obj);
	}

	static void ReadLock(XenonRwLock& rwlock)
	{
		_XenonRwLockImplReadLock(rwlock.obj);
	}

	static void ReadUnlock(XenonRwLock& rwlock)
	{
		_XenonRwLockImplReadUnlock(rwlock.obj);
	}

	static bool TryWriteLock(XenonRwLock& rwlock)
	{
		return _XenonRwLockImplTryWriteLock(rwlock.obj);
	}

	static void WriteLock(XenonRwLock& rwlock)
	{
		_XenonRwLockImplWriteLock(rwlock.obj);
	}

	static void WriteUnlock(XenonRwLock& rwlock)
	{
		_XenonRwLockImplWriteUnlock(rwlock.obj);
	}

	XenonInternalRwLock obj;
};

//----------------------------------------------------------------------------------------------------------------------

class XENON_BASE_API XenonScopedReadLock
{
public:

	XenonScopedReadLock() = delete;
	XenonScopedReadLock(const XenonScopedReadLock&) = delete;
	XenonScopedReadLock(XenonScopedReadLock&&) = delete;

	explicit XenonScopedReadLock(XenonRwLock& rwlock)
		: m_pRwLock(&rwlock)
	{
		XenonRwLock::ReadLock(*m_pRwLock);
	}

	~XenonScopedReadLock()
	{
		XenonRwLock::ReadUnlock(*m_pRwLock);
	}


private:

	XenonRwLock* m_pRwLock;
};

//----------------------------------------------------------------------------------------------------------------------

class XENON_BASE_API XenonScopedWriteLock
{
public:

	XenonScopedWriteLock() = delete;
	XenonScopedWriteLock(const XenonScopedWriteLock&) = delete;
	XenonScopedWriteLock(XenonScopedWriteLock&&) = delete;

	explicit XenonScopedWriteLock(XenonRwLock& rwlock)
		: m_pRwLock(&rwlock)
	{
		XenonRwLock::WriteLock(*m_pRwLock);
	}

	~XenonScopedWriteLock()
	{
		XenonRwLock::WriteUnlock(*m_pRwLock);
	}


private:

	XenonRwLock* m_pRwLock;
};

//----------------------------------------------------------------------------------------------------------------------
