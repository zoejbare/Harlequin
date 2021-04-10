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
#include <process.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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

		auto threadEntryPoint = [](void* pOpaqueArgs) -> uint32_t
		{
			Args args = *reinterpret_cast<Args*>(pOpaqueArgs);

			// Now that we have a copy of the input args, we can free their backing memory.
			XenonMemFree(pOpaqueArgs);

			// Call the read thread main function.
			const int32_t result = args.callback(args.pObject);

			// Exit the thread with the value returned by the callback.
			_endthreadex(uint32_t(result));
			return 0;
		};

		Args* const pArgs = reinterpret_cast<Args*>(XenonMemAlloc(sizeof(Args)));

		pArgs->callback = threadMainFn;
		pArgs->pObject = pObject;

		XenonThread output;

		// Create and start the thread.
		output.hObj = HANDLE(_beginthreadex(nullptr, stackSize, threadEntryPoint, pArgs, 0, &output.id));
		assert(output.hObj != nullptr);

		return output;
	}

	static void Join(XenonThread& thread, int32_t* const pOutReturnValue)
	{
		// Wait for the thread to exit.
		WaitForSingleObject(thread.hObj, INFINITE);

		// Get the thread's return value.
		DWORD result = 0;
		GetExitCodeThread(thread.hObj, &result);

		// Free the thread's internal resources.
		CloseHandle(thread.hObj);

		if(pOutReturnValue)
		{
			(*pOutReturnValue) = int32_t(result);
		}
	}

	static void Sleep(uint32_t ms)
	{
		::Sleep(ms);
	}

	HANDLE hObj;
	uint32_t id;
};

/*---------------------------------------------------------------------------------------------------------------------*/
