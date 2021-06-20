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

extern "C" void _XenonThreadImplCreate(XenonInternalThread& obj, const XenonThreadConfig& threadConfig)
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

	// Create and start the thread.
	obj.handle = HANDLE(_beginthreadex(nullptr, threadConfig.stackSize, threadEntryPoint, pConfig, 0, &obj.id));
	assert(obj.handle != nullptr);

	obj.real = true;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplGetCurrentThread(XenonInternalThread& obj)
{
	// This will return a pseudo handle which will NOT equal the original handle value.
	// This means the handle value itself is not portable between threads and is only
	// valid on the thread where it is retrieved. However, the retrieved thread ID
	// should be portable.
	obj.handle = ::GetCurrentThread();
	obj.id = ::GetCurrentThreadId();
	obj.real = false;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplJoin(XenonInternalThread& obj, int32_t* const pOutReturnValue)
{
	assert(obj.handle != nullptr);
	assert(obj.real);

	// Wait for the thread to exit.
	WaitForSingleObject(obj.handle, INFINITE);

	// Get the thread's return value.
	DWORD result = 0;
	GetExitCodeThread(obj.handle, &result);

	// Free the thread's internal resources.
	CloseHandle(obj.handle);

	obj = XenonInternalThread();

	if(pOutReturnValue)
	{
		(*pOutReturnValue) = int32_t(result);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplSleep(uint32_t ms)
{
	::Sleep(ms);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _XenonThreadImplYield()
{
	YieldProcessor();
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _XenonThreadImplEqual(const XenonInternalThread& left, const XenonInternalThread& right)
{
	return left.handle
		&& right.handle
		&& left.id == right.id;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _XenonThreadImplIsInitialized(const XenonInternalThread& obj)
{
	return obj.handle != nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _XenonThreadImplIsReal(const XenonInternalThread& obj)
{
	return obj.real;
}

//----------------------------------------------------------------------------------------------------------------------
