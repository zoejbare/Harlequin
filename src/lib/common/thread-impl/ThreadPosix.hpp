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
	static XenonThread Create(XenonThreadMainCallback threadMainFn, void* const pObject)
	{
		assert(threadMainFn != nullptr);

		struct Args
		{
			XenonThreadMainCallback callback;
			void* pObject;
		};

		auto threadMainWrapper = [](void* pOpaqueArgs) -> void*
		{
			Args args = *reinterpret_cast<Args*>(pOpaqueArgs);

			XenonMemFree(pOpaqueArgs);
			args.callback(args.pObject);

			return nullptr;
		};

		Args* const pArgs = reinterpret_cast<Args*>(XenonMemAlloc(sizeof(Args)));

		pArgs->callback = threadMainFn;
		pArgs->pObject = pObject;

		XenonThread output;

		const int createResult = pthread_create(&output.obj, nullptr, threadMainWrapper, pArgs);
		assert(createResult == 0);

		return output;
	}

	static void Join(XenonThread& thread)
	{
		const int joinResult = pthread_join(thread.obj, nullptr);
		assert(joinResult == 0);
	}

	static void Sleep(uint32_t ms)
	{
		usleep(ms * 1000);
	}

	pthread_t obj;
};

/*---------------------------------------------------------------------------------------------------------------------*/
