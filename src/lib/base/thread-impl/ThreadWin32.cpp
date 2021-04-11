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
#include <process.h>

//----------------------------------------------------------------------------------------------------------------------

XenonThread XenonThread::Create(const XenonThreadConfig& threadConfig)
{
	assert(threadConfig.mainFn != nullptr);
	assert(threadConfig.stackSize >= XENON_VM_THREAD_MINIMUM_STACK_SIZE);

	auto threadEntryPoint = [](void* pOpaqueConfig) -> uint32_t
	{
		auto setThreadName = [](const char* const threadName)
		{
			#pragma pack(push,8)
			typedef struct tagTHREADNAME_INFO
			{
				DWORD dwType; // Must be 0x1000.
				LPCSTR szName; // Pointer to name (in user addr space).
				DWORD dwThreadID; // Thread ID (-1=caller thread).
				DWORD dwFlags; // Reserved for future use, must be zero.
			} THREADNAME_INFO;
			#pragma pack(pop)

			THREADNAME_INFO info;
			info.dwType = 0x1000;
			info.szName = threadName;
			info.dwThreadID = DWORD(-1);
			info.dwFlags = 0;

			__try
			{
				RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*) &info);
			}
			__except(EXCEPTION_CONTINUE_EXECUTION)
			{
			}
		};

		// Copy thread config.
		XenonThreadConfig config = *reinterpret_cast<XenonThreadConfig*>(pOpaqueConfig);

		// Free their backing memory for the entry point input argument.
		XenonMemFree(pOpaqueConfig);

		if(config.name[0] != '\0')
		{
			setThreadName(config.name);
		}

		// Call the read thread main function.
		const int32_t result = config.mainFn(config.pArg);

		// Exit the thread with the value returned by the callback.
		_endthreadex(uint32_t(result));
		return 0;
	};

	XenonThreadConfig* const pConfig =
		reinterpret_cast<XenonThreadConfig*>(XenonMemAlloc(sizeof(XenonThreadConfig)));

	(*pConfig) = threadConfig;

	XenonThread output;

	// Create and start the thread.
	output.obj.handle = HANDLE(_beginthreadex(nullptr, threadConfig.stackSize, threadEntryPoint, pConfig, 0, &output.obj.id));
	assert(output.obj.handle != nullptr);

	output.obj.real = true;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

XenonThread XenonThread::GetCurrentThread()
{
	XenonThread thread;

	// This will return a pseudo handle which will NOT equal the original handle value.
	// This means the handle value itself is not portable between threads and it only
	// valid on the thread where it is retrieved. However, the retrieved thread ID
	// should be portable.
	thread.obj.handle = ::GetCurrentThread();
	thread.obj.id = ::GetCurrentThreadId();
	thread.obj.real = false;

	return thread;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonThread::Equal(const XenonThread& left, const XenonThread& right)
{
	return left.obj.handle
		&& right.obj.handle
		&& left.obj.id == right.obj.id;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonThread::IsInitialized(const XenonThread& thread)
{
	return thread.obj.handle != nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonThread::IsReal(const XenonThread& thread)
{
	return thread.obj.real;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonThread::Join(XenonThread& thread, int32_t* const pOutReturnValue)
{
	assert(thread.obj.handle != nullptr);
	assert(thread.obj.real);

	// Wait for the thread to exit.
	WaitForSingleObject(thread.obj.handle, INFINITE);

	// Get the thread's return value.
	DWORD result = 0;
	GetExitCodeThread(thread.obj.handle, &result);

	// Free the thread's internal resources.
	CloseHandle(thread.obj.handle);

	thread.obj = XenonInternalThread();

	if(pOutReturnValue)
	{
		(*pOutReturnValue) = int32_t(result);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonThread::Sleep(uint32_t ms)
{
	::Sleep(ms);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonThread::Yield()
{
	YieldProcessor();
}

//----------------------------------------------------------------------------------------------------------------------
