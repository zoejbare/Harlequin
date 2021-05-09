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

#include "../RwLock.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonRwLock XenonRwLock::Create()
{
	XenonRwLock output;

	InitializeSRWLock(&output.obj.lock);
	output.obj.initialized = true;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::Dispose(XenonRwLock& rwlock)
{
	assert(rwlock.obj.initialized);

	// SRWLOCK has no explicit destruction.
	rwlock.obj = XenonInternalRwLock();
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonRwLock::TryReadLock(XenonRwLock& mutex)
{
	assert(mutex.obj.initialized);

	return TryAcquireSRWLockShared(&mutex.obj.lock) == TRUE;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::ReadLock(XenonRwLock& mutex)
{
	assert(mutex.obj.initialized);

	AcquireSRWLockShared(&mutex.obj.lock);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::ReadUnlock(XenonRwLock& mutex)
{
	assert(mutex.obj.initialized);

	ReleaseSRWLockShared(&mutex.obj.lock);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonRwLock::TryWriteLock(XenonRwLock& mutex)
{
	assert(mutex.obj.initialized);

	return TryAcquireSRWLockExclusive(&mutex.obj.lock) == TRUE;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::WriteLock(XenonRwLock& mutex)
{
	assert(mutex.obj.initialized);

	AcquireSRWLockExclusive(&mutex.obj.lock);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::WriteUnlock(XenonRwLock& mutex)
{
	assert(mutex.obj.initialized);

	ReleaseSRWLockExclusive(&mutex.obj.lock);
}

//----------------------------------------------------------------------------------------------------------------------
