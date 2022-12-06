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

//----------------------------------------------------------------------------------------------------------------------

#define APPLICATION_RESULT_SUCCESS 0
#define APPLICATION_RESULT_FAILURE 1

// Needed by some of the Playstation platforms so we can use malloc without running out of memory.
size_t sceLibcHeapSize = 10 * 1024 * 1024;

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

static size_t maxAllocSize = 0;
static size_t minAllocSize = size_t(-1);
static size_t peakMemUsage = 0;
static size_t activeAllocCount = 0;
static size_t totalAllocCount = 0;
static size_t mallocCount = 0;
static size_t reallocCount = 0;

static size_t currentTotalSize = 0;

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

		++activeAllocCount;
		++totalAllocCount;
		++mallocCount;

		OnAlloc(size);

		return pMem + 1;
	};

	auto trackedRealloc = [](void* const pOldMem, const size_t newSize) -> void*
	{
		assert(newSize > 0);

		size_t* const pAlloc = (pOldMem) ? (reinterpret_cast<size_t*>(pOldMem) - 1) : nullptr;
		const size_t oldSize = (pAlloc) ? (*pAlloc) : 0;

		size_t* const pNewMem = reinterpret_cast<size_t*>(realloc(pAlloc, newSize + sizeof(size_t)));
		assert(pNewMem != nullptr);

		(*pNewMem) = newSize;

		currentTotalSize -= oldSize;

		if(oldSize == 0)
		{
			++activeAllocCount;
			++totalAllocCount;
		}

		++reallocCount;

		OnAlloc(newSize);

		return pNewMem + 1;
	};

	auto trackedFree = [](void* const pMem)
	{
		size_t* const pAlloc = (pMem) ? (reinterpret_cast<size_t*>(pMem) - 1) : nullptr;
		const size_t size = (pAlloc) ? (*pAlloc) : 0;

		if(pAlloc)
		{
			currentTotalSize -= size;

			assert(activeAllocCount > 0);
			--activeAllocCount;

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
	vmInit.gcMaxTimeSliceMs = HQ_VM_GC_DEFAULT_TIME_SLICE_MS;

	HqMemAllocator allocator;
	allocator.allocFn = trackedAlloc;
	allocator.reallocFn = trackedRealloc;
	allocator.freeFn = trackedFree;

	HqMemSetAllocator(allocator);

	const uint64_t timerFrequency = HqClockGetFrequency();
	const uint64_t overallTimeStart = HqClockGetTimestamp();

	const uint64_t createVmTimeStart = overallTimeStart;

	// Create the VM context.
	const int createVmResult = HqVmCreate(&hVm, vmInit);
	if(createVmResult != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg), "Failed to create Harlequin VM context: error=\"%s\"", HqGetErrorCodeString(createVmResult));
		OnMessageReported(NULL, HQ_MESSAGE_TYPE_FATAL, msg);
		return APPLICATION_RESULT_FAILURE;
	}

	HqReportHandle hReport = HQ_REPORT_HANDLE_NULL;
	HqVmGetReportHandle(hVm, &hReport);

	const uint64_t createVmTimeEnd = HqClockGetTimestamp();
	const uint64_t createVmTimeSlice = createVmTimeEnd - createVmTimeStart;

	const uint64_t loadProgramTimeStart = createVmTimeEnd;

	std::vector<uint8_t> fileData;

	// Load the program file.
	{
		HqSerializerHandle hFileSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Create the serializer we'll use to read the program file data.
		const int createFileSerializerResult = HqSerializerCreate(&hFileSerializer, HQ_SERIALIZER_MODE_READER);
		if(createFileSerializerResult != HQ_SUCCESS)
		{
			char msg[128];
			snprintf(msg, sizeof(msg), "Failed to create Harlequin serializer: error=\"%s\"", HqGetErrorCodeString(createFileSerializerResult));
			OnMessageReported(NULL, HQ_MESSAGE_TYPE_FATAL, msg);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}

		const int readProgramFileResult = HqSerializerLoadStreamFromFile(hFileSerializer, argv[1]);

		const void* const pFileData = HqSerializerGetRawStreamPointer(hFileSerializer);
		const size_t fileSize = HqSerializerGetStreamLength(hFileSerializer);

		if(pFileData && fileSize > 0)
		{
			// Resize the file data vector, then copy the contents of the file to it.
			fileData.resize(fileSize);
			memcpy(&fileData[0], pFileData, fileSize);
		}

		HqSerializerDispose(&hFileSerializer);

		if(readProgramFileResult != HQ_SUCCESS)
		{
			char msg[128];
			snprintf(msg, sizeof(msg), "Failed to read compiled binary: error=\"%s\"", HqGetErrorCodeString(readProgramFileResult));
			OnMessageReported(NULL, HQ_MESSAGE_TYPE_FATAL, msg);
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}
	}

	const int loadProgramResult = HqVmLoadProgram(hVm, "test", &fileData[0], fileData.size());

	const uint64_t loadProgramTimeEnd = HqClockGetTimestamp();
	const uint64_t loadProgramTimeSlice = loadProgramTimeEnd - loadProgramTimeStart;

	uint64_t initProgramsTimeSlice = 0;
	uint64_t disassembleTimeSlice = 0;
	uint64_t createExecTimeSlice = 0;
	uint64_t runProgramTimeSlice = 0;
	uint64_t disposeExecTimeSlice = 0;

	int applicationResult = APPLICATION_RESULT_SUCCESS;

	// Clear the cached file data now that we no longer need it.
	fileData.clear();

	// Initialize the loaded programs.
	{
		const uint64_t initProgramsTimeStart = HqClockGetTimestamp();

		HqExecutionHandle hInitExec = HQ_EXECUTION_HANDLE_NULL;
		HqVmInitializePrograms(hVm, &hInitExec);

		const uint64_t initProgramsTimeEnd = HqClockGetTimestamp();
		initProgramsTimeSlice = initProgramsTimeEnd - initProgramsTimeStart;

		if(hInitExec != HQ_EXECUTION_HANDLE_NULL)
		{
			HqVmDispose(&hVm);
			return APPLICATION_RESULT_FAILURE;
		}
	}

	if(loadProgramResult == HQ_SUCCESS)
	{
		auto iterateProgram = [](void* const pUserData, HqProgramHandle hProgram) -> bool
		{
			auto iterateFunction = [](void* const pUserData, const char* const signature) -> bool
			{
				auto onDisasm = [](void*, const char* const asmLine, const uintptr_t offset)
				{
					printf("\t\t0x%08" PRIXPTR ": %s\n", offset, asmLine);
				};

				HqVmHandle hVm = reinterpret_cast<HqVmHandle>(pUserData);
				HqFunctionHandle hFunction = HQ_FUNCTION_HANDLE_NULL;

				HqVmGetFunction(hVm, &hFunction, signature);

				assert(hFunction != HQ_FUNCTION_HANDLE_NULL);
				printf("\t%s\n", signature);

				bool isNative = false;
				HqFunctionGetIsNative(hFunction, &isNative);

				if(isNative)
				{
					printf("\t\t<native call>\n");
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

			// Iterate each function within in the program.
			HqProgramListFunctions(hProgram, iterateFunction, pUserData);

			return true;
		};

		HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Disassembling ...\n");

		const uint64_t disassembleTimeStart = HqClockGetTimestamp();

		// Iterate all the programs to disassemble them.
		HqVmListPrograms(hVm, iterateProgram, hVm);

		const uint64_t disassembleTimeEnd = HqClockGetTimestamp();
		disassembleTimeSlice = disassembleTimeEnd - disassembleTimeStart;

		const char* const entryPoint = "void App.Program.Main()";

		HqFunctionHandle hEntryFunc = HQ_FUNCTION_HANDLE_NULL;
		HqExecutionHandle hExec = HQ_EXECUTION_HANDLE_NULL;

		HqVmGetFunction(hVm, &hEntryFunc, entryPoint);

		const uint64_t createExecTimeStart = HqClockGetTimestamp();

		// Create an execution context that will run the program's entry point function.
		bool createExecResult = HqExecutionCreate(&hExec, hVm, hEntryFunc);

		const uint64_t createExecTimeEnd = HqClockGetTimestamp();
		createExecTimeSlice = createExecTimeEnd - createExecTimeStart;

		if(createExecResult == HQ_SUCCESS)
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

					printf("> \"%s\"\n", inputParam);
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

			HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Executing script function: \"%s\"", entryPoint);

			bool status;

			const uint64_t runProgramTimeStart = HqClockGetTimestamp();
			int result = HQ_SUCCESS;

			// Run the script until it has completed.
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

					applicationResult = APPLICATION_RESULT_FAILURE;
					break;
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
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Finished executing script");
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

			const uint64_t runProgramTimeEnd = HqClockGetTimestamp();
			runProgramTimeSlice = runProgramTimeEnd - runProgramTimeStart;

			const uint64_t disposeExecTimeStart = runProgramTimeEnd;

			HqExecutionDispose(&hExec);

			const uint64_t disposeExecTimeEnd = HqClockGetTimestamp();
			disposeExecTimeSlice = disposeExecTimeEnd - disposeExecTimeStart;
		}
	}
	else
	{
		applicationResult = APPLICATION_RESULT_FAILURE;
	}

	const uint64_t disposeVmTimeStart = HqClockGetTimestamp();

	// Dispose of the VM context.
	const int disposeVmResult = HqVmDispose(&hVm);
	if(disposeVmResult != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg), "Failed to dispose of Harlequin VM context: error=\"%s\"", HqGetErrorCodeString(disposeVmResult));
		OnMessageReported(NULL, HQ_MESSAGE_TYPE_WARNING, msg);
	}

	const uint64_t disposeVmTimeEnd = HqClockGetTimestamp();
	const uint64_t disposeVmTimeSlice = disposeVmTimeEnd - disposeVmTimeStart;

	if(activeAllocCount != 0)
	{
		char msg[128];
		snprintf(msg, sizeof(msg), "Leaked script allocations: %zu", activeAllocCount);
		OnMessageReported(NULL, HQ_MESSAGE_TYPE_ERROR, msg);

		applicationResult = APPLICATION_RESULT_FAILURE;
	}

	// Output memory allocation stats.
	printf(
		"Memory Stats:\n"
		"\tMin allocation size: %zu\n"
		"\tMax allocation size: %zu\n"
		"\tPeak memory usage: %zu\n"
		"\tTotal allocation count: %zu\n"
		"\tMalloc() call count: %zu\n"
		"\tRealloc() call count: %zu\n",
		minAllocSize,
		maxAllocSize,
		peakMemUsage,
		totalAllocCount,
		mallocCount,
		reallocCount
	);

	const uint64_t overallTimeEnd = HqClockGetTimestamp();
	const uint64_t overallTimeSlice = overallTimeEnd - overallTimeStart;

	const double convertTimeToMs = 1000.0 / double(timerFrequency);

	// Output the timing metrics.
	printf(
		"Timing metrics:\n"
		"\tTotal time: %f ms\n"
		"\tCreate VM time: %f ms\n"
		"\tDispose VM time: %f ms\n"
		"\tInit programs time: %f ms\n"
		"\tCreate exec-context time: %f ms\n"
		"\tDispose exec-context time: %f ms\n"
		"\tLoad program time: %f ms\n"
		"\tRun program time: %f ms\n"
		"\tDisassemble time: %f ms\n",
		double(overallTimeSlice) * convertTimeToMs,
		double(createVmTimeSlice) * convertTimeToMs,
		double(disposeVmTimeSlice) * convertTimeToMs,
		double(initProgramsTimeSlice) * convertTimeToMs,
		double(createExecTimeSlice) * convertTimeToMs,
		double(disposeExecTimeSlice) * convertTimeToMs,
		double(loadProgramTimeSlice) * convertTimeToMs,
		double(runProgramTimeSlice) * convertTimeToMs,
		double(disassembleTimeSlice) * convertTimeToMs
	);

	return applicationResult;
}

//----------------------------------------------------------------------------------------------------------------------
