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

#if defined(HQ_PLATFORM_WINDOWS)
	#include <crtdbg.h>
#endif

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(HQ_PLATFORM_PS3) && !defined(HQ_PLATFORM_PSVITA)
	#include <memory.h>
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

size_t sceLibcHeapSize = 10 * 1024 * 1024;

//----------------------------------------------------------------------------------------------------------------------

// Disabling the incremental GC thread requires user code to manually all the API function for invoking the GC.
#define _INCREMENTAL_GC_ENABLED 1

// Setting the test iterations to anything above 1 will do special logic to add an iteration loop and remove some log prints.
#define _STRESS_TEST_ITERATIONS 1
#define _STRESS_TEST_ENABLED    (_STRESS_TEST_ITERATIONS > 1)

// Enabling memory tracking will track memory metrics, but it will also slow things down a bit.
#define _MEM_STATS_ENABLED 1

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

	FILE* const pStream = (messageType >= HQ_MESSAGE_TYPE_ERROR) ? stderr : stdout;

	fprintf(pStream, "[%s] %s\n", tag, message);
	fflush(pStream);
}

//----------------------------------------------------------------------------------------------------------------------

void OnDependencyRequested(void* const pUserData, const char* const programName)
{
	std::deque<const char*>* pDependencies = reinterpret_cast<std::deque<const char*>*>(pUserData);

	pDependencies->push_back(programName);
}

//----------------------------------------------------------------------------------------------------------------------

#if _MEM_STATS_ENABLED
static size_t maxAllocSize = 0;
static size_t minAllocSize = ~size_t(0);
static size_t peakMemUsage = 0;
static size_t activeAllocCount = 0;
static size_t totalAllocCount = 0;
static size_t mallocCount = 0;
static size_t reallocCount = 0;

static size_t currentTotalSize = 0;

#if !defined(HQ_PLATFORM_PS3)
static std::mutex allocMtx;
#endif

static void OnAlloc(const size_t size)
{
	if(size > maxAllocSize)
	{
		maxAllocSize = size;
	}

	if(size < minAllocSize)
	{
		minAllocSize = size;
	}

	currentTotalSize += size;

	if(currentTotalSize >= peakMemUsage)
	{
		peakMemUsage = currentTotalSize;
	}
};
#endif

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, "Missing required 'filepath' argument");
		return APPLICATION_RESULT_FAILURE;
	}

#if defined(HQ_PLATFORM_WINDOWS)
	// This enables tracking of global heap allocations.  If any are leaked, they will show up in the
	// Visual Studio output window on application exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2659); // Uncomment this with the allocation order number when debugging a specific allocation.
#endif

	auto trackedAlloc = [](const size_t size) -> void*
	{
		assert(size > 0);

		size_t* const pMem = reinterpret_cast<size_t*>(malloc(size + sizeof(size_t)));
		assert(pMem != nullptr);

		(*pMem) = size;

#if _MEM_STATS_ENABLED
		// Update stats.
		{
	#if !defined(HQ_PLATFORM_PS3)
			allocMtx.lock();
	#endif

			++activeAllocCount;
			++totalAllocCount;
			++mallocCount;

			OnAlloc(size);
	#if !defined(HQ_PLATFORM_PS3)
			allocMtx.unlock();
	#endif
		}
#endif

		return pMem + 1;
	};

	auto trackedRealloc = [](void* const pOldMem, const size_t newSize) -> void*
	{
		assert(newSize > 0);

		size_t* const pAlloc = (pOldMem) ? (reinterpret_cast<size_t*>(pOldMem) - 1) : nullptr;

		size_t* const pNewMem = reinterpret_cast<size_t*>(realloc(pAlloc, newSize + sizeof(size_t)));
		assert(pNewMem != nullptr);

		(*pNewMem) = newSize;

#if _MEM_STATS_ENABLED
		// Update stats.
		{
			const size_t oldSize = (pAlloc) ? (*pAlloc) : 0;

	#if !defined(HQ_PLATFORM_PS3)
			allocMtx.lock();
	#endif

			currentTotalSize -= oldSize;

			if(oldSize == 0)
			{
				++activeAllocCount;
				++totalAllocCount;
				++mallocCount;
			}
			else
			{
				++reallocCount;
			}

			OnAlloc(newSize);

	#if !defined(HQ_PLATFORM_PS3)
			allocMtx.unlock();
	#endif
		}
#endif

		return pNewMem + 1;
	};

	auto trackedFree = [](void* const pMem)
	{
		size_t* const pAlloc = (pMem) ? (reinterpret_cast<size_t*>(pMem) - 1) : nullptr;

		if(pAlloc)
		{
#if _MEM_STATS_ENABLED
			// Update stats.
			{
				const size_t size = (pAlloc) ? (*pAlloc) : 0;

	#if !defined(HQ_PLATFORM_PS3)
				allocMtx.lock();
	#endif

				currentTotalSize -= size;

				assert(activeAllocCount > 0);
				--activeAllocCount;

	#if !defined(HQ_PLATFORM_PS3)
				allocMtx.unlock();
	#endif
			}
#endif

			free(pAlloc);
		}
	};

	auto iterateCallstackFrame = [](void* const pUserData, HqFrameHandle hFrame) -> bool
	{
		bool& isTopFrame = *reinterpret_cast<bool*>(pUserData);

		HqFunctionHandle hFunction = HQ_FUNCTION_HANDLE_NULL;
		HqFrameGetFunction(hFrame, &hFunction);

		const char* functionSignature = nullptr;
		HqFunctionGetSignature(hFunction, &functionSignature);

		bool isNative = false;
		HqFunctionGetIsNative(hFunction, &isNative);

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

#if _INCREMENTAL_GC_ENABLED
	vmInit.gcMaxTimeSliceMs = HQ_VM_GC_DEFAULT_TIME_SLICE_MS;
#else
	vmInit.gcMaxTimeSliceMs = 0;
#endif

	HqMemAllocator allocator;
	allocator.allocFn = trackedAlloc;
	allocator.reallocFn = trackedRealloc;
	allocator.freeFn = trackedFree;

	HqMemSetAllocator(allocator);

	const uint64_t timerFrequency = HqClockGetFrequency();
	const uint64_t overallTimeStart = HqClockGetTimestamp();

	uint64_t createVmTimeSlice = 0;
	uint64_t loadProgramTimeSlice = 0;
	uint64_t initProgramsTimeSlice = 0;
	uint64_t createExecTimeSlice = 0;
	uint64_t initExecTimeSlice = 0;
	uint64_t resetExecTimeSlice = 0;
	uint64_t runProgramTimeSlice = 0;
	uint64_t disposeExecTimeSlice = 0;
	uint64_t disposeVmTimeSlice = 0;
	uint64_t totalApplicationTime = 0;
	uint64_t totalDisassembleTime = 0;
	uint64_t totalManualGcTime = 0;

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


	std::vector<uint8_t> fileData;

	const char* const scriptFilePath = argv[1];

	// Load the program file.
	{
		const uint64_t timeStart = HqClockGetTimestamp();

		HqSerializerHandle hFileSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Create the serializer we'll use to read the program file data.
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

		const int readProgramFileResult = HqSerializerLoadStreamFromFile(hFileSerializer, scriptFilePath);
		if(readProgramFileResult == HQ_SUCCESS)
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

		if(readProgramFileResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to read compiled binary: error=\"%s\"",
				HqGetErrorCodeString(readProgramFileResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		// Load the program data into the VM.
		const int loadProgramResult = HqVmLoadProgram(hVm, "test", &fileData[0], fileData.size());
		if(loadProgramResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to load script program: \"%s\", error=\"%s\"",
				scriptFilePath,
				HqGetErrorCodeString(loadProgramResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		loadProgramTimeSlice = timeEnd - timeStart;
	}

	// Clear the cached file data now that we no longer need it.
	fileData.clear();

	// Initialize the loaded programs.
	{
		HqExecutionHandle hInitExec = HQ_EXECUTION_HANDLE_NULL;

		const uint64_t timeStart = HqClockGetTimestamp();

		const int initProgramsResult = HqVmInitializePrograms(hVm, &hInitExec);
		if(initProgramsResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to initialize script programs: error=\"%s\"",
				HqGetErrorCodeString(initProgramsResult)
			);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		const uint64_t timeEnd = HqClockGetTimestamp();
		initProgramsTimeSlice = timeEnd - timeStart;
	}

	auto iterateProgram = [](void* const pUserData, HqProgramHandle hProgram) -> bool
	{
		auto iterateString = [](void*, const char* const string, const size_t index) -> bool
		{
			const size_t length = strlen(string);

			printf(
				"        s(%" PRIuPTR "): \"%.48s\"%s\n",
				index,
				string,
				(length > 48) ? " ..." : ""
			);
			return true;
		};

		auto iterateFunction = [](void* const pUserData, const char* const signature) -> bool
		{
			auto onDisasm = [](void*, const char* const asmLine, const uintptr_t offset)
			{
				printf("            0x%08" PRIXPTR ": %s\n", offset, asmLine);
			};

			HqVmHandle hVm = reinterpret_cast<HqVmHandle>(pUserData);
			HqFunctionHandle hFunction = HQ_FUNCTION_HANDLE_NULL;

			HqVmGetFunction(hVm, &hFunction, signature);

			assert(hFunction != HQ_FUNCTION_HANDLE_NULL);
			printf("        %s\n", signature);

			bool isNative = false;
			HqFunctionGetIsNative(hFunction, &isNative);

			if(isNative)
			{
				printf("            <native call>\n");
			}
			else
			{
				HqFunctionDisassemble(hFunction, onDisasm, nullptr);
			}

			printf("\n");
			return true;
		};

		const char* programName = nullptr;
		HqProgramGetName(hProgram, &programName);

		printf("[Program: \"%s\"]\n", programName);
		printf("    [strings]\n");

		HqProgramListStrings(hProgram, iterateString, pUserData);

		printf("\n    [code]\n");

		// Iterate each function within in the program.
		HqProgramListFunctions(hProgram, iterateFunction, pUserData);

		return true;
	};

	HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Disassembling ...\n");

	// Generate the script program disassembly.
	{
		const uint64_t timeStart = HqClockGetTimestamp();

		// Iterate all the programs to disassemble them.
		HqVmListPrograms(hVm, iterateProgram, hVm);

		const uint64_t timeEnd = HqClockGetTimestamp();
		totalDisassembleTime = timeEnd - timeStart;
	}

	// Bind the native functions to the script VM.
	{
		HqFunctionHandle hNativePrintFunc = HQ_FUNCTION_HANDLE_NULL;
		HqVmGetFunction(hVm, &hNativePrintFunc, "void App.Program.PrintString(string)");

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
		HqVmGetFunction(hVm, &hNativeDecrementFunc, "(int32, bool) App.Program.Decrement(int32)");

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
						hOutputParam = HqValueCreateNull();
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

	const char* const entryPointFuncName = "void App.Program.Main()";

	HqFunctionHandle hEntryFunc = HQ_FUNCTION_HANDLE_NULL;
	HqExecutionHandle hExec = HQ_EXECUTION_HANDLE_NULL;

	// Create the execution context that will run the program's entry point function.
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
			resetExecTimeSlice = timeEnd - timeStart;
		}

		bool status;

		// Run the script.
		{
			int result = HQ_SUCCESS;

			const uint64_t timeStart = HqClockGetTimestamp();

			// Keep the script running until it has completed.
			for(;;)
			{
				result = HqExecutionRun(hExec, HQ_RUN_CONTINUOUS);
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
			runProgramTimeSlice = timeEnd - timeStart;
		}

#if !_INCREMENTAL_GC_ENABLED
		// Run the garbage collector.
		{
			const uint64_t timeStart = HqClockGetTimestamp();
			HqVmRunGarbageCollector(hVm);
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

#if _MEM_STATS_ENABLED
	if(activeAllocCount != 0)
	{
		char msg[128];
		snprintf(msg, sizeof(msg), "Leaked script allocations: %zu", activeAllocCount);
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_ERROR, msg);

		applicationResult = APPLICATION_RESULT_FAILURE;
	}

	// Output memory allocation stats.
	printf(
		"\nMemory Stats:\n"
		"  [Size]\n"
		"    Min:  %zu\n"
		"    Max:  %zu\n"
		"    Peak: %zu\n"
		"  [Count]\n"
		"    Total:     %zu\n"
		"    Malloc():  %zu\n"
		"    Realloc(): %zu\n",
		minAllocSize,
		maxAllocSize,
		peakMemUsage,
		totalAllocCount,
		mallocCount,
		reallocCount
	);
#endif

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
		"  [Program]\n"
		"    Init: %f ms\n"
		"    Load: %f ms\n"
		"    Run:  %f ms\n"
		"  [Misc]\n"
		"    Total application: %f ms\n"
		"    Total disassemble: %f ms\n"
		"    Total manual GC:   %f ms\n",

		double(createVmTimeSlice) * convertTimeToMs,
		double(disposeVmTimeSlice) * convertTimeToMs,

		double(createExecTimeSlice) * convertTimeToMs,
		double(initExecTimeSlice) * convertTimeToMs,
		double(resetExecTimeSlice) * convertTimeToMs,
		double(disposeExecTimeSlice) * convertTimeToMs,

		double(initProgramsTimeSlice) * convertTimeToMs,
		double(loadProgramTimeSlice) * convertTimeToMs,
		double(runProgramTimeSlice) * convertTimeToMs,

		double(totalApplicationTime) * convertTimeToMs,
		double(totalDisassembleTime) * convertTimeToMs,
		double(totalManualGcTime) * convertTimeToMs
	);

	return applicationResult;
}

//----------------------------------------------------------------------------------------------------------------------
