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

#include "Util.h"

#include <assert.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

void _DummyMessageCallback(void* pUserData, int messageType, const char* message)
{
	(void) pUserData;
	(void) messageType;
	(void) message;

	// Ignore all messages.
}

//----------------------------------------------------------------------------------------------------------------------

void DefaultMessageCallback(void* const pUserData, const int messageType, const char* const message)
{
	(void) pUserData;

	if(!message)
	{
		return;
	}

	const char* tag = NULL;

	switch(messageType)
	{
		case HQ_MESSAGE_TYPE_VERBOSE:
			tag = "V";
			break;

		case HQ_MESSAGE_TYPE_INFO:
			tag = "I";
			break;

		case HQ_MESSAGE_TYPE_WARNING:
			tag = "W";
			break;

		case HQ_MESSAGE_TYPE_ERROR:
			tag = "E";
			break;

		case HQ_MESSAGE_TYPE_FATAL:
			tag = "!";
			break;

		default:
			assert(false);
			break;
	}

	fprintf((messageType >= HQ_MESSAGE_TYPE_ERROR) ? stderr : stdout, "[%s] %s\n", tag, message);

	if(messageType == HQ_MESSAGE_TYPE_FATAL)
	{
		// Fatal errors are unrecoverable by definition;
		// kill execution immediately when they occur.
		exit(1);
	}
}

//----------------------------------------------------------------------------------------------------------------------

HqVmInit GetDefaultHqVmInit(void* const pUserData, HqMessageCallback onMessageFn, const int reportLevel)
{
	HqVmInit output;
	output.common.report.onMessageFn = onMessageFn ? onMessageFn : _DummyMessageCallback;
	output.common.report.pUserData = pUserData;
	output.common.report.reportLevel = reportLevel;
	output.gcThreadStackSize = HQ_VM_THREAD_DEFAULT_STACK_SIZE;
	output.gcTimeSliceMs = HQ_VM_GC_DEFAULT_TIME_SLICE_MS;
	output.gcTimeWaitMs = HQ_VM_GC_DEFAULT_TIME_WAIT_MS;
	output.gcEnableThread = false;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

HqCompilerInit GetDefaultHqCompilerInit(void* const pUserData, HqMessageCallback onMessageFn, const int reportLevel)
{
	HqCompilerInit output;
	output.common.report.onMessageFn = onMessageFn ? onMessageFn : _DummyMessageCallback;
	output.common.report.pUserData = pUserData;
	output.common.report.reportLevel = reportLevel;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------
