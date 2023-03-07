//
// Copyright (c) 2023, Zoe J. Bare
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

#include <Harlequin.h>


//----------------------------------------------------------------------------------------------------------------------

inline HqVmInit GetDefaultHqVmInit(void* const pUserData, HqMessageCallback onMessageFn, const int reportLevel)
{
	auto dummyMsg = [](void*, int, const char*)
	{
		// Ignore all messages.
	};

	HqVmInit output;
	output.common.report.onMessageFn = onMessageFn ? onMessageFn : dummyMsg;
	output.common.report.pUserData = pUserData;
	output.common.report.reportLevel = reportLevel;
	output.gcThreadStackSize = HQ_VM_THREAD_DEFAULT_STACK_SIZE;
	output.gcTimeSliceMs = HQ_VM_GC_DEFAULT_TIME_SLICE_MS;
	output.gcTimeWaitMs = HQ_VM_GC_DEFAULT_TIME_WAIT_MS;
	output.gcEnableThread = false;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------
