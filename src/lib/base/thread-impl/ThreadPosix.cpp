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

#include "../Thread.hpp"

#include <assert.h>
#include <unistd.h>

#include <sys/types.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplCreate(XenonInternalThread& obj, const XenonThreadConfig& threadConfig)
{
	assert(threadConfig.mainFn != nullptr);
	assert(threadConfig.stackSize >= XENON_VM_THREAD_MINIMUM_STACK_SIZE);
	assert(threadConfig.name[0] != '\0');

	auto threadEntryPoint = [](void* pOpaqueConfig) -> void*
	{
		// Copy thread config.
		XenonThreadConfig config = *reinterpret_cast<XenonThreadConfig*>(pOpaqueConfig);

		// Free their backing memory for the entry point input argument.
		XenonMemFree(pOpaqueConfig);

		// Call the read thread main function.
		const int32_t result = config.mainFn(config.pArg);

		// Use integer casting magic to make the return value look like an opaque pointer.
		// No dynamic allocation required!
		void* const pReturnValue = reinterpret_cast<void*>(result);
		pthread_exit(pReturnValue);

		return nullptr;
	};

	XenonThreadConfig* const pConfig =
		reinterpret_cast<XenonThreadConfig*>(XenonMemAlloc(sizeof(XenonThreadConfig)));

	(*pConfig) = threadConfig;

	pthread_attr_t attr;

	// Create a thread attributes object where the thread properties will be set.
	const int attrInitResult = pthread_attr_init(&attr);
	assert(attrInitResult == 0); (void) attrInitResult;

	// Configure the thread stack size.
	pthread_attr_setstacksize(&attr, size_t(threadConfig.stackSize));

	// Create and start the thread.
	const int threadCreateResult = pthread_create(&obj.handle, &attr, threadEntryPoint, pConfig);
	assert(threadCreateResult == 0); (void) threadCreateResult;

	// Destroy the thread attributes since they are no longer needed.
	// This will not affect the thread since it creates its own copy.
	const int attrDestroyResult = pthread_attr_destroy(&attr);
	assert(attrDestroyResult == 0); (void) attrDestroyResult;

	// Setting the thread name through the pthread interface is only supported on a few platforms.
#if defined(XENON_PLATFORM_LINUX) || defined(XENON_PLATFORM_ANDROID)
	if(threadConfig.name[0] != '\0')
	{
		pthread_setname_np(obj.handle, threadConfig.name);
	}
#endif

	obj.initialized = true;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplGetCurrentThread(XenonInternalThread& obj)
{
	obj.handle = pthread_self();
	obj.initialized = true;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplJoin(XenonInternalThread& obj, int32_t* const pOutReturnValue)
{
	assert(obj.initialized);

	void* pThreadResult = nullptr;

	const int joinResult = pthread_join(obj.handle, &pThreadResult);
	assert(joinResult == 0); (void) joinResult;

	obj.initialized = false;

	if(pOutReturnValue)
	{
		// Unpack the thread's return value.
		(*pOutReturnValue) = int32_t(reinterpret_cast<intptr_t>(pThreadResult));
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplSleep(uint32_t ms)
{
	usleep(ms * 1000);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplYield()
{
	sched_yield();
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _XenonThreadImplEqual(const XenonInternalThread& left, const XenonInternalThread& right)
{
	return left.initialized
		&& right.initialized
		&& pthread_equal(left.handle, right.handle) != 0;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _XenonThreadImplIsInitialized(const XenonInternalThread& obj)
{
	return obj.initialized;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _XenonThreadImplIsReal(const XenonInternalThread&)
{
	// Pthreads are never psuedo handles.
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
