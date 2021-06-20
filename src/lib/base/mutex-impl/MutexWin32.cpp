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

extern "C" void _XenonMutexImplCreate(XenonInternalMutex& obj)
{
	BOOL lockInitResult = InitializeCriticalSectionAndSpinCount(&obj.lock, 0x400);
	assert(lockInitResult == TRUE); (void) lockInitResult;

	obj.initialized = true;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonMutexImplDispose(XenonInternalMutex& obj)
{
	assert(obj.initialized);

	DeleteCriticalSection(&obj.lock);

	obj = XenonInternalMutex();
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _XenonMutexImplTryLock(XenonInternalMutex& obj)
{
	assert(obj.initialized);

	return TryEnterCriticalSection(&obj.lock) == TRUE;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonMutexImplLock(XenonInternalMutex& obj)
{
	assert(obj.initialized);

	EnterCriticalSection(&obj.lock);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonMutexImplUnlock(XenonInternalMutex& obj)
{
	assert(obj.initialized);

	LeaveCriticalSection(&obj.lock);
}

//----------------------------------------------------------------------------------------------------------------------
