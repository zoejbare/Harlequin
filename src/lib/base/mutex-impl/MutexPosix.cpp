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

#include "../Mutex.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonMutex XenonMutex::Create()
{
	pthread_mutexattr_t attr;

	// Create a mutex attributes object for configuring the mutex.
	const int attrInitResult = pthread_mutexattr_init(&attr);
	assert(attrInitResult == 0);

	// Set the mutex type to be recursive.
	const int attrSetTypeResult = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	assert(attrSetTypeResult == 0);

	XenonMutex output;

	// Create the mutex.
	const int mutexInitResult = pthread_mutex_init(&output.obj.handle, &attr);
	assert(mutexInitResult == 0);

	// Destroy the mutex attributes since they are no longer needed.
	const int attrDestroyResult = pthread_mutexattr_destroy(&attr);
	assert(attrDestroyResult == 0);

	output.obj.initialized = true;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonMutex::Dispose(XenonMutex& mutex)
{
	assert(mutex.obj.initialized);

	const int mutexDestroyResult = pthread_mutex_destroy(&mutex.obj.handle);
	assert(mutexDestroyResult == 0);

	mutex.obj.initialized = false;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonMutex::TryLock(XenonMutex& mutex)
{
	assert(mutex.obj.initialized);

	return pthread_mutex_trylock(&mutex.obj.handle) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonMutex::Lock(XenonMutex& mutex)
{
	assert(mutex.obj.initialized);

	pthread_mutex_lock(&mutex.obj.handle);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonMutex::Unlock(XenonMutex& mutex)
{
	assert(mutex.obj.initialized);

	pthread_mutex_unlock(&mutex.obj.handle);
}

//----------------------------------------------------------------------------------------------------------------------
