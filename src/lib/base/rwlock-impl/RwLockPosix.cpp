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
	pthread_rwlockattr_t attr;

	// Create an attributes object for configuring the rwlock.
	const int attrInitResult = pthread_rwlockattr_init(&attr);
	assert(attrInitResult == 0);

	XenonRwLock output;

	// Create the rwlock.
	const int rwlockInitResult = pthread_rwlock_init(&output.obj.handle, &attr);
	assert(rwlockInitResult == 0);

	// Destroy the attributes since they are no longer needed.
	const int attrDestroyResult = pthread_rwlockattr_destroy(&attr);
	assert(attrDestroyResult == 0);

	output.obj.initialized = true;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::Dispose(XenonRwLock& rwlock)
{
	assert(rwlock.obj.initialized);

	const int rwlockDestroyResult = pthread_rwlock_destroy(&rwlock.obj.handle);
	assert(rwlockDestroyResult == 0);

	rwlock.obj.initialized = false;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonRwLock::TryReadLock(XenonRwLock& rwlock)
{
	assert(rwlock.obj.initialized);

	return pthread_rwlock_tryrdlock(&rwlock.obj.handle) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::ReadLock(XenonRwLock& rwlock)
{
	assert(rwlock.obj.initialized);

	pthread_rwlock_rdlock(&rwlock.obj.handle);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::ReadUnlock(XenonRwLock& rwlock)
{
	assert(rwlock.obj.initialized);

	pthread_rwlock_unlock(&rwlock.obj.handle);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonRwLock::TryWriteLock(XenonRwLock& rwlock)
{
	assert(rwlock.obj.initialized);

	return pthread_rwlock_trywrlock(&rwlock.obj.handle) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::WriteLock(XenonRwLock& rwlock)
{
	assert(rwlock.obj.initialized);

	pthread_rwlock_wrlock(&rwlock.obj.handle);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonRwLock::WriteUnlock(XenonRwLock& rwlock)
{
	assert(rwlock.obj.initialized);

	pthread_rwlock_unlock(&rwlock.obj.handle);
}

//----------------------------------------------------------------------------------------------------------------------
