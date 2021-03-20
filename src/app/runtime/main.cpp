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

#if defined(XENON_PLATFORM_WINDOWS)
	#include <crtdbg.h>
#endif

#include <assert.h>
#include <inttypes.h>
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
	if(XenonVmLoadProgramFromFile(hVm, "test", "test.xc") == XENON_SUCCESS)
	{
		auto iterateProgram = [](void* const pUserData, XenonProgramHandle hProgram) -> bool
		{
			auto iterateFunction = [](void* const pUserData, const char* const signature) -> bool
			{
				auto onDisasm = [](void*, const char* const asmLine, const uintptr_t offset)
				{
					printf("\t\t0x%08" PRIXPTR ": %s\n", offset, asmLine);
				};

				XenonVmHandle hVm = reinterpret_cast<XenonVmHandle>(pUserData);
				XenonFunctionHandle hFunction = XENON_FUNCTION_HANDLE_NULL;

				XenonVmGetFunction(hVm, &hFunction, signature);

				assert(hFunction != XENON_FUNCTION_HANDLE_NULL);
				printf("\t%s\n", signature);

				bool isNative = false;
				XenonFunctionGetIsNative(hFunction, &isNative);

				if(isNative)
				{
					printf("\t\t<native call>\n");
				}
				else
				{
					XenonFunctionDisassemble(hFunction, onDisasm, nullptr);
				}

				printf("\n");
				return true;
			};

			const char* programName = nullptr;
			XenonProgramGetName(hProgram, &programName);

			printf("[Program: \"%s\"]\n", programName);

			// Iterate each function within in the program.
			XenonProgramListFunctions(hProgram, iterateFunction, pUserData);

			return true;
		};

		OnMessageReported(nullptr, XENON_MESSAGE_TYPE_VERBOSE, "Disassembling ...\n");

		// Iterate all the programs to disassemble them.
		XenonVmListPrograms(hVm, iterateProgram, hVm);

		const char* const entryPoint = "void Program.Main()";

		XenonFunctionHandle hEntryFunc = XENON_FUNCTION_HANDLE_NULL;
		XenonExecutionHandle hExec = XENON_EXECUTION_HANDLE_NULL;

		XenonVmGetFunction(hVm, &hEntryFunc, entryPoint);

		if(XenonExecutionCreate(&hExec, hVm, hEntryFunc) == XENON_SUCCESS)
		{
			XenonFunctionHandle hNativeFunc = XENON_FUNCTION_HANDLE_NULL;
			XenonVmGetFunction(hVm, &hNativeFunc, "string Program.DoWorkNative(string, string)");

			if(hNativeFunc != XENON_FUNCTION_HANDLE_NULL)
			{
				auto doWorkNative = [](XenonExecutionHandle, XenonFunctionHandle)
				{
					OnMessageReported(nullptr, XENON_MESSAGE_TYPE_INFO, ">>> This is a native call made from script");
				};

				XenonFunctionSetNativeBinding(hNativeFunc, doWorkNative);
			}

			char msg[256];
			snprintf(msg, sizeof(msg), "Executing script function: \"%s\"", entryPoint);
			OnMessageReported(nullptr, XENON_MESSAGE_TYPE_VERBOSE, msg);

			bool status;

			// Run the script until it has completed.
			for(;;)
			{
				result = XenonExecutionRun(hExec, XENON_RUN_LOOP);
				if(result != XENON_SUCCESS)
				{
					snprintf(msg, sizeof(msg), "Error occurred while executing script: \"%s\"", XenonGetErrorCodeString(result));
					OnMessageReported(nullptr, XENON_MESSAGE_TYPE_ERROR, msg);
					break;
				}

				// Check if there was an unhandled exception raised.
				result = XenonExecutionGetStatus(hExec, &status, XENON_EXEC_STATUS_EXCEPTION);
				if(result != XENON_SUCCESS)
				{
					snprintf(msg, sizeof(msg), "Error occurred while retrieving exception status: \"%s\"", XenonGetErrorCodeString(result));
					OnMessageReported(nullptr, XENON_MESSAGE_TYPE_ERROR, msg);
					break;
				}
				if(status)
				{
					OnMessageReported(nullptr, XENON_MESSAGE_TYPE_ERROR, "Unhandled exception occurred");

					auto iterateFrame = [](void* const pUserData, XenonFrameHandle hFrame) -> bool
					{
						bool& isTopFrame = *reinterpret_cast<bool*>(pUserData);

						XenonFunctionHandle hFunction = XENON_FUNCTION_HANDLE_NULL;
						XenonFrameGetFunction(hFrame, &hFunction);

						const char* functionSignature = nullptr;
						XenonFunctionGetSignature(hFunction, &functionSignature);

						bool isNative = false;
						XenonFunctionGetIsNative(hFunction, &isNative);

						uint32_t offset = 0;
						XenonFrameGetBytecodeOffset(hFrame, &offset);

						XenonNativeFunction nativeBinding = nullptr;
						XenonFunctionGetNativeBinding(hFunction, &nativeBinding);

						char frameMsg[512];
						snprintf(
							frameMsg,
							sizeof(frameMsg),
							"%s%s [%s: 0x%" PRIXPTR "]",
							isTopFrame ? "" : "... ",
							functionSignature,
							isNative ? "ptr" : "offset",
							isNative ? reinterpret_cast<uintptr_t>(nativeBinding) : offset
						);

						printf("%s\n", frameMsg);

						isTopFrame = false;

						return true;
					};

					printf("\n<Callstack>\n");

					bool isTopFrame = true;
					XenonExecutionResolveFrameStack(hExec, iterateFrame, &isTopFrame);

					printf("\n");
					break;
				}

				// Check if the script has finished running.
				result = XenonExecutionGetStatus(hExec, &status, XENON_EXEC_STATUS_COMPLETE);
				if(result != XENON_SUCCESS)
				{
					snprintf(msg, sizeof(msg), "Error occurred while retrieving completion status: \"%s\"", XenonGetErrorCodeString(result));
					OnMessageReported(nullptr, XENON_MESSAGE_TYPE_ERROR, msg);
					break;
				}
				if(status)
				{
					OnMessageReported(nullptr, XENON_MESSAGE_TYPE_VERBOSE, "Finished executing script");
					break;
				}
			}

			XenonExecutionDispose(&hExec);
		}
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
