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

#include "XenonScript.h"

#include <assert.h>
#include <stdio.h>

#include <deque>

//----------------------------------------------------------------------------------------------------------------------

void OnMessageReported(void* const pUserData, const int messageType, const char* const message)
{
	(void) pUserData;

	if(!message)
	{
		return;
	}

	const char* tag = NULL;

	switch(messageType)
	{
		case XENON_MESSAGE_TYPE_VERBOSE:
			tag = "V";
			break;

		case XENON_MESSAGE_TYPE_INFO:
			tag = "I";
			break;

		case XENON_MESSAGE_TYPE_WARNING:
			tag = "W";
			break;

		case XENON_MESSAGE_TYPE_ERROR:
			tag = "E";
			break;

		case XENON_MESSAGE_TYPE_FATAL:
			tag = "!";
			break;

		default:
			assert(false);
			break;
	}

	fprintf((messageType >= XENON_MESSAGE_TYPE_ERROR) ? stderr : stdout, "[%s] %s\n", tag, message);
}

//----------------------------------------------------------------------------------------------------------------------

void OnDependencyRequested(void* const pUserData, const char* const programName)
{
	std::deque<const char*>* pDependencies = reinterpret_cast<std::deque<const char*>*>(pUserData);

	pDependencies->push_back(programName);
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

#if defined(XENON_PLATFORM_WINDOWS)
	// This enables tracking of global heap allocations.  If any are leaked, they will show up in the
	// Visual Studio output window on application exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonVmInit init;

	std::deque<const char*> dependencies;

	init.common.report.onMessageFn = OnMessageReported;
	init.common.report.pUserData = nullptr;
	init.common.report.reportLevel = XENON_MESSAGE_TYPE_VERBOSE;

	init.dependency.onRequestFn = OnDependencyRequested;
	init.dependency.pUserData = &dependencies;

	// Create the VM context.
	int result = XenonVmCreate(&hVm, init);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Xenon VM context: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_FATAL, msg);
		return 1;
	}

	// Load the test program.
	if(XenonVmLoadProgramFromFile(hVm, "test.xc") == XENON_SUCCESS)
	{
		XenonFunctionHandle hFunc = XenonVmGetFunction(hVm, "void main()");
		XenonExecutionHandle hExec = XENON_EXECUTION_HANDLE_NULL;

		XenonExecutionCreate(&hExec, hVm, hFunc);
		XenonExecutionRun(hExec, XENON_RUN_TO_COMPLETION);
		XenonExecutionDispose(&hExec);
	}

	// Dispose of the VM context.
	result = XenonVmDispose(&hVm);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Xenon VM context: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_WARNING, msg);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
