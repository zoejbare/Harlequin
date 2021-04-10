/**
 * Copyright (c) 2021, Zoe J. Bare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

/*---------------------------------------------------------------------------------------------------------------------*/

#include "../ThreadMainCallback.hpp"

#include <assert.h>
#include <pthread.h>
#include <unistd.h>

/*---------------------------------------------------------------------------------------------------------------------*/

struct XenonThread
{
	static XenonThread Create(XenonThreadMainCallback threadMainFn, void* const pObject, uint32_t stackSize)
	{
		assert(threadMainFn != nullptr);
		assert(stackSize >= XENON_VM_THREAD_MINIMUM_STACK_SIZE);

		struct Args
		{
			XenonThreadMainCallback callback;
			void* pObject;
		};

		auto threadEntryPoint = [](void* pOpaqueArgs) -> void*
		{
			Args args = *reinterpret_cast<Args*>(pOpaqueArgs);

			// Now that we have a copy of the input args, we can free their backing memory.
			XenonMemFree(pOpaqueArgs);

			// Call the read thread main function.
			const int32_t result = args.callback(args.pObject);

			// Use integer casting magic to make the return value look like an opaque pointer.
			// No dynamic allocation required!
			void* const pReturnValue = reinterpret_cast<void*>(result);
			pthread_exit(pReturnValue);

			return nullptr;
		};

		Args* const pArgs = reinterpret_cast<Args*>(XenonMemAlloc(sizeof(Args)));

		pArgs->callback = threadMainFn;
		pArgs->pObject = pObject;

		XenonThread output;
		pthread_attr_t attr;

		// Create a thread attributes object where the thread properties will be set.
		const int attrInitResult = pthread_attr_init(&attr);
		assert(attrInitResult == 0);

		// Configure the thread stack size.
		pthread_attr_setstacksize(&attr, size_t(stackSize));

		// Create and start the thread.
		const int threadCreateResult = pthread_create(&output.obj, &attr, threadEntryPoint, pArgs);
		assert(threadCreateResult == 0);

		// Destroy the thread attributes since they are no longer needed.
		// This will not affect the thread since it creates its own copy.
		const int attrDestroyResult = pthread_attr_destroy(&attr);
		assert(attrDestroyResult == 0);

		return output;
	}

	static void Join(XenonThread& thread, int32_t* const pOutReturnValue)
	{
		void* pThreadResult = nullptr;

		const int joinResult = pthread_join(thread.obj, &pThreadResult);
		assert(joinResult == 0);

		if(pOutReturnValue)
		{
			// Unpack the thread's return value.
			(*pOutReturnValue) = int32_t(reinterpret_cast<intptr_t>(pThreadResult));
		}
	}

	static void Sleep(uint32_t ms)
	{
		usleep(ms * 1000);
	}

	pthread_t obj;
};

/*---------------------------------------------------------------------------------------------------------------------*/
