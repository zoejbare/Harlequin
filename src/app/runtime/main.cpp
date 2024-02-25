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

#include "Harlequin.h"

#include "../common/MemoryHandler.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(HQ_PLATFORM_WINDOWS)
	#include <crtdbg.h>
	#include <malloc.h>
	#define alloca _alloca
#endif

#if defined(HQ_PLATFORM_LINUX) || defined(HQ_PLATFORM_MAC_OS)
	#include <alloca.h>
#endif

#if !defined(HQ_PLATFORM_PS3) && !defined(HQ_PLATFORM_PSVITA)
	#include <memory.h>
#endif

#if !defined(HQ_PLATFORM_PSVITA)
	#include <locale.h>
#endif

#include <deque>
#include <map>
#include <vector>

#if !defined(HQ_PLATFORM_PS3)
	#include <mutex>
#endif

//----------------------------------------------------------------------------------------------------------------------

#define APPLICATION_RESULT_SUCCESS 0
#define APPLICATION_RESULT_FAILURE 1

//----------------------------------------------------------------------------------------------------------------------

// Some platforms need the heap size set explicitly because it's too small by default.
#ifdef _HQ_LIBC_HEAP_SIZE_VAR
	size_t _HQ_LIBC_HEAP_SIZE_VAR = 10 * 1024 * 1024;
#endif

#ifdef _HQ_REQUIRES_PLATFORM_INIT_FUNCS
extern "C"
{
	void _HqRuntimeAppPlatformInitialize();
	void _HqRuntimeAppPlatformShutdown();
}
#endif

inline void _HqAppInit()
{
#ifdef _HQ_REQUIRES_PLATFORM_INIT_FUNCS
	_HqRuntimeAppPlatformInitialize();
#endif
}

inline void _HqAppShutdown()
{
#ifdef _HQ_REQUIRES_PLATFORM_INIT_FUNCS
	_HqRuntimeAppPlatformShutdown();
#endif
}

//----------------------------------------------------------------------------------------------------------------------

// Disabling the GC thread requires user code to manually call the API function for invoking the GC.
#define _GC_THREAD_ENABLED 1

// Setting the test iterations to anything above 1 will do special logic to add an iteration loop and remove some log prints.
#define _STRESS_TEST_ITERATIONS 1
#define _STRESS_TEST_ENABLED    (_STRESS_TEST_ITERATIONS > 1)

//----------------------------------------------------------------------------------------------------------------------

void OnMessageReported(void* const pUserData, const int messageType, const char* const message)
{
	(void) pUserData;

	if(!message)
	{
		return;
	}

	const char* tag = nullptr;

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

	constexpr size_t stackBufferMaxSize = 256;

	const size_t bufferSize = strlen(message) + 6;
	char* const temp = (bufferSize > stackBufferMaxSize)
		? reinterpret_cast<char*>(HqMemAlloc(sizeof(char) * bufferSize))
		: reinterpret_cast<char*>(alloca(bufferSize));

	snprintf(temp, bufferSize, "[%s] %s\n", tag, message);

	FILE* const pStream = (messageType >= HQ_MESSAGE_TYPE_WARNING) ? stderr : stdout;

	fprintf(pStream, "%s", temp);
	fflush(pStream);

#if defined(HQ_PLATFORM_WINDOWS)
	OutputDebugStringA(temp);
#endif

	if(bufferSize > stackBufferMaxSize)
	{
		HqMemFree(temp);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OnDependencyRequested(void* const pUserData, const char* const moduleName)
{
	std::deque<const char*>* pDependencies = reinterpret_cast<std::deque<const char*>*>(pUserData);

	pDependencies->push_back(moduleName);
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
#if !defined(HQ_PLATFORM_PSVITA)
	// Set the program locale to the environment default.
	setlocale(LC_ALL, "");
#endif

#if defined(HQ_PLATFORM_WINDOWS) && defined(_DEBUG)
	// This enables tracking of global heap allocations. If any are leaked,
	// they will show up in the Visual Studio output window on application exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2659); // Uncomment this with the allocation order number when debugging a specific allocation.
#endif

	if(argc < 2)
	{
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, "Missing required 'filepath' argument");
		return APPLICATION_RESULT_FAILURE;
	}

	// Handle any required platform initialization.
	_HqAppInit();

	MemoryHandler& memory = MemoryHandler::Instance;

	// Initialize the Harlequin memory handler.
	memory.Initialize();

	auto iterateCallstackFrame = [](void* const pUserData, HqFrameHandle hFrame) -> bool
	{
		bool& isTopFrame = *reinterpret_cast<bool*>(pUserData);

		HqFunctionHandle hFunction = HQ_FUNCTION_HANDLE_NULL;
		HqFrameGetFunction(hFrame, &hFunction);

		const char* functionSignature = nullptr;
		HqFunctionGetSignature(hFunction, &functionSignature);

		bool isNative = false;
		HqFunctionIsNative(hFunction, &isNative);

		uint32_t offset = 0;
		HqFrameGetBytecodeOffset(hFrame, &offset);

		HqNativeFunction nativeBinding = nullptr;
		HqFunctionGetNativeBinding(hFunction, &nativeBinding);

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

	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqVmInit vmInit;

	std::deque<const char*> dependencies;

	vmInit.common.report.onMessageFn = OnMessageReported;
	vmInit.common.report.pUserData = nullptr;
	vmInit.common.report.reportLevel = HQ_MESSAGE_TYPE_VERBOSE;

	vmInit.gcThreadStackSize = HQ_VM_THREAD_DEFAULT_STACK_SIZE;
	vmInit.gcTimeSliceMs = HQ_VM_GC_DEFAULT_TIME_SLICE_MS;
	vmInit.gcTimeWaitMs = HQ_VM_GC_DEFAULT_TIME_WAIT_MS;
	vmInit.gcEnableThread = _GC_THREAD_ENABLED;

	const uint64_t timerFrequency = HqClockGetFrequency();
	const uint64_t overallTimeStart = HqClockGetTimestamp();

	uint64_t createVmTimeSlice = 0;
	uint64_t loadModuleTimeSlice = 0;
	uint64_t initModulesTimeSlice = 0;
	uint64_t createExecTimeSlice = 0;
	uint64_t initExecTimeSlice = 0;
	uint64_t disposeExecTimeSlice = 0;
	uint64_t disposeVmTimeSlice = 0;

	uint64_t runModuleTotalTime = 0;
	uint64_t resetExecTotalTime = 0;

	uint64_t totalApplicationTime = 0;
#if !_GC_THREAD_ENABLED
	uint64_t totalManualGcTime = 0;
#endif

	// Create the VM context.
	{
		const uint64_t timeStart = HqClockGetTimestamp();

		const int createVmResult = HqVmCreate(&hVm, vmInit);
		if(createVmResult != HQ_SUCCESS)
		{
			char msg[128];
			snprintf(msg, sizeof(msg), "Failed to create Harlequin VM context: error=\"%s\"", HqGetErrorCodeString(createVmResult));
			OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, msg);
			return APPLICATION_RESULT_FAILURE;
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		createVmTimeSlice = timeEnd - timeStart;
	}

	HqReportHandle hReport = HQ_REPORT_HANDLE_NULL;
	HqVmGetReportHandle(hVm, &hReport);


	const char* const scriptFilePath = argv[1];

	// Load the module file.
	{
		const uint64_t timeStart = HqClockGetTimestamp();

		HqSerializerHandle hFileSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Create the serializer we'll use to read the module file data.
		const int createFileSerializerResult = HqSerializerCreate(&hFileSerializer, HQ_SERIALIZER_MODE_READER);
		if(createFileSerializerResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to create Harlequin serializer: error=\"%s\"",
				HqGetErrorCodeString(createFileSerializerResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		std::vector<uint8_t> fileData;

		const int readModuleFileResult = HqSerializerLoadStreamFromFile(hFileSerializer, scriptFilePath);
		if(readModuleFileResult == HQ_SUCCESS)
		{
			const void* const pFileData = HqSerializerGetRawStreamPointer(hFileSerializer);
			const size_t fileSize = HqSerializerGetStreamLength(hFileSerializer);

			if(pFileData && fileSize > 0)
			{
				// Resize the file data vector, then copy the contents of the file to it.
				fileData.resize(fileSize);
				memcpy(&fileData[0], pFileData, fileSize);
			}
		}

		HqSerializerDispose(&hFileSerializer);

		if(readModuleFileResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to read compiled binary: error=\"%s\"",
				HqGetErrorCodeString(readModuleFileResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		// Load the module data into the VM.
		const int loadModuleResult = HqVmLoadModule(hVm, "test", &fileData[0], fileData.size());
		if(loadModuleResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to load script module: \"%s\", error=\"%s\"",
				scriptFilePath,
				HqGetErrorCodeString(loadModuleResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		loadModuleTimeSlice = timeEnd - timeStart;
	}

	// Initialize the loaded modules.
	{
		HqExecutionHandle hInitExec = HQ_EXECUTION_HANDLE_NULL;

		const uint64_t timeStart = HqClockGetTimestamp();

		const int initModulesResult = HqVmInitializeModules(hVm, &hInitExec);
		if(initModulesResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to initialize script modules: error=\"%s\"",
				HqGetErrorCodeString(initModulesResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		initModulesTimeSlice = timeEnd - timeStart;
	}
	// Bind the native functions to the script VM.
	{
		HqFunctionHandle hNativePrintFunc = HQ_FUNCTION_HANDLE_NULL;
		HqVmGetFunction(hVm, &hNativePrintFunc, "void Program.PrintString(string)");

		if(hNativePrintFunc != HQ_FUNCTION_HANDLE_NULL)
		{
			auto printString = [](HqExecutionHandle hExec, HqFunctionHandle, void*)
			{
				HqValueHandle hInputParam = HQ_VALUE_HANDLE_NULL;
				HqExecutionGetIoRegister(hExec, &hInputParam, 0);

				const char* const inputParam = HqValueGetString(hInputParam);

#if !_STRESS_TEST_ENABLED
				printf("> \"%s\"\n", inputParam);
#else
				((void) inputParam);
#endif

				HqValueGcExpose(hInputParam);
			};

			HqFunctionSetNativeBinding(hNativePrintFunc, printString, nullptr);
		}

		HqFunctionHandle hNativeDecrementFunc = HQ_FUNCTION_HANDLE_NULL;
		HqVmGetFunction(hVm, &hNativeDecrementFunc, "(int32, bool) Program.Decrement(int32)");

		if(hNativeDecrementFunc != HQ_FUNCTION_HANDLE_NULL)
		{
			auto decrement = [](HqExecutionHandle hExec, HqFunctionHandle, void*)
			{
				HqVmHandle hVm = HQ_VM_HANDLE_NULL;
				HqExecutionGetVm(hExec, &hVm);

				HqValueHandle hInputParam = HQ_VALUE_HANDLE_NULL;
				HqExecutionGetIoRegister(hExec, &hInputParam, 0);

				if(HqValueIsInt32(hInputParam))
				{
					const int32_t output = HqValueGetInt32(hInputParam) - 1;

					HqValueHandle hOutputParam = HqValueCreateInt32(hVm, output);

					HqExecutionSetIoRegister(hExec, hOutputParam, 0);
					HqValueGcExpose(hOutputParam);

					if(output <= 0)
					{
						hOutputParam = HqValueCreateBool(hVm, true);
					}
					else
					{
						hOutputParam = HQ_VALUE_HANDLE_NULL;
					}

					HqExecutionSetIoRegister(hExec, hOutputParam, 1);
					HqValueGcExpose(hOutputParam);
				}
				else
				{
					HqExecutionRaiseStandardException(
						hExec,
						HQ_EXCEPTION_SEVERITY_NORMAL,
						HQ_STANDARD_EXCEPTION_TYPE_ERROR,
						"Type mismatch; expected int32"
					);
				}
			};

			HqFunctionSetNativeBinding(hNativeDecrementFunc, decrement, nullptr);
		}
	}

	const char* const entryPointFuncName = "void Program.Main()";

	HqFunctionHandle hEntryFunc = HQ_FUNCTION_HANDLE_NULL;
	HqExecutionHandle hExec = HQ_EXECUTION_HANDLE_NULL;

	// Create the execution context that will run the module's entry point function.
	{
		const uint64_t timeStart = HqClockGetTimestamp();

		const int createExecResult = HqExecutionCreate(&hExec, hVm);
		if(createExecResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to create execution context: error=\"%s\"",
				HqGetErrorCodeString(createExecResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		createExecTimeSlice = timeEnd - timeStart;
	}

	// Find the script entry point function.
	const int getEntryPointResult = HqVmGetFunction(hVm, &hEntryFunc, entryPointFuncName);
	if(getEntryPointResult != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_FATAL,
			"Failed to find script function: \"%s\", error=\"%s\"",
			entryPointFuncName,
			HqGetErrorCodeString(getEntryPointResult)
		);
		HqVmDispose(&hVm);
		return APPLICATION_RESULT_FAILURE;
	}

	// Initialize the execution context with the entry point function.
	{
		const uint64_t timeStart = HqClockGetTimestamp();

		const int initExecResult = HqExecutionInitialize(hExec, hEntryFunc);
		if(initExecResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to initialize execution context with script function: \"%s\", error=\"%s\"",
				entryPointFuncName,
				HqGetErrorCodeString(initExecResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		initExecTimeSlice = timeEnd - timeStart;
	}

	HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Executing script function: \"%s\"", entryPointFuncName);

	// Run the test script.
#if _STRESS_TEST_ENABLED
	for(size_t testIter = 0; testIter < _STRESS_TEST_ITERATIONS; ++testIter)
#endif
	{
		{
			const uint64_t timeStart = HqClockGetTimestamp();

			const int resetExecResult = HqExecutionReset(hExec);
			if(resetExecResult != HQ_SUCCESS)
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_FATAL,
					"Failed to reset execution context: error=\"%s\"",
					HqGetErrorCodeString(resetExecResult)
				);
				HqVmDispose(&hVm);
				return APPLICATION_RESULT_FAILURE;
			}

			const uint64_t timeEnd = HqClockGetTimestamp();
			resetExecTotalTime += timeEnd - timeStart;
		}

		bool status;

		// Run the script.
		{
			int result = HQ_SUCCESS;

			const uint64_t timeStart = HqClockGetTimestamp();

			// Keep the script running until it has completed.
			for(;;)
			{
				result = HqExecutionRun(hExec, HQ_RUN_FULL);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Error occurred while executing script: \"%s\"", HqGetErrorCodeString(result));
					break;
				}

				// Check if there was an unhandled exception raised.
				result = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_EXCEPTION, &status);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Error occurred while retrieving exception status: \"%s\"", HqGetErrorCodeString(result));
					break;
				}
				if(status)
				{
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Unhandled exception occurred");

					printf("\n<Callstack>\n");

					bool isTopFrame = true;
					HqExecutionResolveFrameStack(hExec, iterateCallstackFrame, &isTopFrame);

					printf("\n");

					HqVmDispose(&hVm);
					return APPLICATION_RESULT_FAILURE;
				}

				// Check if the script has finished running.
				result = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_COMPLETE, &status);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Error occurred while retrieving completion status: \"%s\"", HqGetErrorCodeString(result));
					break;
				}
				if(status)
				{
	#if !_STRESS_TEST_ENABLED
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Finished executing script");
	#endif
					break;
				}

				// Check if the script has been aborted.
				result = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_ABORT, &status);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Error occurred while retrieving abort status: \"%s\"", HqGetErrorCodeString(result));
					break;
				}
				if(status)
				{
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_WARNING, "Script execution aborted");
					break;
				}
			}

			const uint64_t timeEnd = HqClockGetTimestamp();
			runModuleTotalTime += timeEnd - timeStart;
		}

#if !_GC_THREAD_ENABLED
		// Run the garbage collector.
		{
			const uint64_t timeStart = HqClockGetTimestamp();
			HqVmRunGarbageCollector(hVm, HQ_RUN_FULL);
			const uint64_t timeEnd = HqClockGetTimestamp();

			totalManualGcTime += timeEnd - timeStart;
		}
#endif
	}

	// Dispose of the execution context now that we're finished running scripts.
	{
		const uint64_t timeStart = HqClockGetTimestamp();

		const int disposeExecResult = HqExecutionDispose(&hExec);
		if(disposeExecResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_WARNING,
				"Failed to dispose of Harlequin execution context: error=\"%s\"",
				HqGetErrorCodeString(disposeExecResult)
			);
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		disposeExecTimeSlice = timeEnd - timeStart;
	}

	// Dispose of the VM context.
	{
		const uint64_t timeStart = HqClockGetTimestamp();

		const int disposeVmResult = HqVmDispose(&hVm);
		if(disposeVmResult != HQ_SUCCESS)
		{
			char msg[128];
			snprintf(msg, sizeof(msg), "Failed to dispose of Harlequin VM context: error=\"%s\"", HqGetErrorCodeString(disposeVmResult));
			OnMessageReported(nullptr, HQ_MESSAGE_TYPE_WARNING, msg);
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		disposeVmTimeSlice = timeEnd - timeStart;
	}

	OnMessageReported(
		nullptr,
		HQ_MESSAGE_TYPE_VERBOSE,
#ifdef HQ_BUILD_STATIC_LIB
		"=== Static library build ==="
#else
		"=== Dynamic library build ==="
#endif
	);

	int applicationResult = APPLICATION_RESULT_SUCCESS;

	// Check for memory leaks.
	if(memory.HasActiveAllocations())
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Memory leaks detected: %zu, totalSize=%zu", memory.GetActiveCount(), memory.GetCurrentSize());
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_ERROR, msg);
	}

	// Dump the memory stats.
	memory.PrintStats(stdout);

	const uint64_t overallTimeEnd = HqClockGetTimestamp();
	totalApplicationTime = overallTimeEnd - overallTimeStart;

	const double convertTimeToMs = 1000.0 / double(timerFrequency);

	// Output the timing metrics.
	printf(
		"\nTiming metrics:\n"
		"  [VM]\n"
		"    Create:  %f ms\n"
		"    Dispose: %f ms\n"
		"  [Exec]\n"
		"    Create:  %f ms\n"
		"    Init:    %f ms\n"
		"    Reset:   %f ms\n"
		"    Dispose: %f ms\n"
		"  [Module]\n"
		"    Init: %f ms\n"
		"    Load: %f ms\n"
		"    Run:  %f ms\n"
		"  [Misc]\n"
		"    Total application: %f ms\n"
#if _GC_THREAD_ENABLED
		"    Total manual GC:   n/a\n",
#else
		"    Total manual GC:   %f ms\n",
#endif

		double(createVmTimeSlice) * convertTimeToMs,
		double(disposeVmTimeSlice) * convertTimeToMs,

		double(createExecTimeSlice) * convertTimeToMs,
		double(initExecTimeSlice) * convertTimeToMs,
		double(resetExecTotalTime) * convertTimeToMs,
		double(disposeExecTimeSlice) * convertTimeToMs,

		double(initModulesTimeSlice) * convertTimeToMs,
		double(loadModuleTimeSlice) * convertTimeToMs,
		double(runModuleTotalTime) * convertTimeToMs,

		double(totalApplicationTime) * convertTimeToMs
#if !_GC_THREAD_ENABLED
		, double(totalManualGcTime) * convertTimeToMs
#endif
	);

	// Shutdown the platform-specific, internal systems.
	_HqAppShutdown();

	return applicationResult;
}

//----------------------------------------------------------------------------------------------------------------------
