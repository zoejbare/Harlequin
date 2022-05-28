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
#include <stdlib.h>

#include <deque>
#include <map>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------

#define PROGRAM_RESULT_SUCCESS 0
#define PROGRAM_RESULT_FAILURE 1

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

	FILE* const pStream = (messageType >= XENON_MESSAGE_TYPE_ERROR) ? stderr : stdout;

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

static std::map<void*, size_t> allocations;

static size_t maxAllocSize = 0;
static size_t minAllocSize = size_t(-1);
static size_t peakMemUsage = 0;
static size_t allocationCount = 0;
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

	++allocationCount;
};

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		OnMessageReported(nullptr, XENON_MESSAGE_TYPE_FATAL, "Missing required 'filepath' argument");
		return PROGRAM_RESULT_FAILURE;
	}

#if defined(XENON_PLATFORM_WINDOWS)
	// This enables tracking of global heap allocations.  If any are leaked, they will show up in the
	// Visual Studio output window on application exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	auto trackedAlloc = [](const size_t size) -> void*
	{
		void* const pMem = malloc(size);

		allocations.insert(std::pair<void*, size_t>(pMem, size));
		OnAlloc(size);

		++mallocCount;

		return pMem;
	};

	auto trackedRealloc = [](void* const pOldMem, const size_t newSize) -> void*
	{
		void* const pNewMem = realloc(pOldMem, newSize);

		currentTotalSize -= allocations[pOldMem];

		allocations.erase(pOldMem);
		allocations.insert(std::pair<void*, size_t>(pNewMem, newSize));
		OnAlloc(newSize);

		++reallocCount;

		return pNewMem;
	};

	auto trackedFree = [](void* const pMem)
	{
		currentTotalSize -= allocations[pMem];

		allocations.erase(pMem);
		free(pMem);
	};

	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonVmInit vmInit;

	std::deque<const char*> dependencies;

	vmInit.common.report.onMessageFn = OnMessageReported;
	vmInit.common.report.pUserData = nullptr;
	vmInit.common.report.reportLevel = XENON_MESSAGE_TYPE_VERBOSE;

	vmInit.gcThreadStackSize = XENON_VM_THREAD_DEFAULT_STACK_SIZE;
	vmInit.gcMaxIterationCount = XENON_VM_GC_DEFAULT_ITERATION_COUNT;

	XenonMemAllocator allocator;
	allocator.allocFn = trackedAlloc;
	allocator.reallocFn = trackedRealloc;
	allocator.freeFn = trackedFree;

	XenonMemSetAllocator(allocator);

	const uint64_t timerFrequency = XenonHiResTimerGetFrequency();
	const uint64_t overallTimeStart = XenonHiResTimerGetTimestamp();

	const uint64_t createVmTimeStart = overallTimeStart;

	// Create the VM context.
	const int createVmResult = XenonVmCreate(&hVm, vmInit);
	if(createVmResult != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg), "Failed to create Xenon VM context: error=\"%s\"", XenonGetErrorCodeString(createVmResult));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_FATAL, msg);
		return PROGRAM_RESULT_FAILURE;
	}

	XenonReportHandle hReport = XENON_REPORT_HANDLE_NULL;
	XenonVmGetReportHandle(hVm, &hReport);

	const uint64_t createVmTimeEnd = XenonHiResTimerGetTimestamp();
	const uint64_t createVmTimeSlice = createVmTimeEnd - createVmTimeStart;

	const uint64_t loadProgramTimeStart = createVmTimeEnd;

	std::vector<uint8_t> fileData;

	// Load the program file.
	{
		XenonSerializerHandle hFileSerializer = XENON_SERIALIZER_HANDLE_NULL;

		// Create the serializer we'll use to read the program file data.
		const int createFileSerializerResult = XenonSerializerCreate(&hFileSerializer, XENON_SERIALIZER_MODE_READER);
		if(createFileSerializerResult != XENON_SUCCESS)
		{
			char msg[128];
			snprintf(msg, sizeof(msg), "Failed to create Xenon serializer: error=\"%s\"", XenonGetErrorCodeString(createFileSerializerResult));
			OnMessageReported(NULL, XENON_MESSAGE_TYPE_FATAL, msg);
			return PROGRAM_RESULT_FAILURE;
		}

		const int readProgramFileResult = XenonSerializerLoadStreamFromFile(hFileSerializer, argv[1]);

		const void* const pFileData = XenonSerializerGetRawStreamPointer(hFileSerializer);
		const size_t fileSize = XenonSerializerGetStreamLength(hFileSerializer);

		if(pFileData && fileSize > 0)
		{
			// Resize the file data vector, then copy the contents of the file to it.
			fileData.resize(fileSize);
			memcpy(fileData.data(), pFileData, fileSize);
		}

		XenonSerializerDispose(&hFileSerializer);

		if(readProgramFileResult != XENON_SUCCESS)
		{
			char msg[128];
			snprintf(msg, sizeof(msg), "Failed to create Xenon serializer: error=\"%s\"", XenonGetErrorCodeString(createFileSerializerResult));
			OnMessageReported(NULL, XENON_MESSAGE_TYPE_FATAL, msg);
			return PROGRAM_RESULT_FAILURE;
		}
	}

	const uint64_t loadProgramTimeEnd = XenonHiResTimerGetTimestamp();
	const uint64_t loadProgramTimeSlice = loadProgramTimeEnd - loadProgramTimeStart;

	uint64_t disassembleTimeSlice = 0;
	uint64_t createExecTimeSlice = 0;
	uint64_t runProgramTimeSlice = 0;
	uint64_t disposeExecTimeSlice = 0;

	int applicationResult = PROGRAM_RESULT_SUCCESS;

	const int loadProgramResult = XenonVmLoadProgram(hVm, "test", fileData.data(), fileData.size());
	fileData.clear();

	if(loadProgramResult == XENON_SUCCESS)
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

		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Disassembling ...\n");

		const uint64_t disassembleTimeStart = XenonHiResTimerGetTimestamp();

		// Iterate all the programs to disassemble them.
		XenonVmListPrograms(hVm, iterateProgram, hVm);

		const uint64_t disassembleTimeEnd = XenonHiResTimerGetTimestamp();
		disassembleTimeSlice = disassembleTimeEnd - disassembleTimeStart;

		const char* const entryPoint = "void App.Program.Main()";

		XenonFunctionHandle hEntryFunc = XENON_FUNCTION_HANDLE_NULL;
		XenonExecutionHandle hExec = XENON_EXECUTION_HANDLE_NULL;

		XenonVmGetFunction(hVm, &hEntryFunc, entryPoint);

		const uint64_t createExecTimeStart = XenonHiResTimerGetTimestamp();

		// Create an execution context that will run the program's entry point function.
		bool createExecResult = XenonExecutionCreate(&hExec, hVm, hEntryFunc);

		const uint64_t createExecTimeEnd = XenonHiResTimerGetTimestamp();
		createExecTimeSlice = createExecTimeEnd - createExecTimeStart;

		if(createExecResult == XENON_SUCCESS)
		{
			XenonFunctionHandle hNativePrintFunc = XENON_FUNCTION_HANDLE_NULL;
			XenonVmGetFunction(hVm, &hNativePrintFunc, "void App.Program.PrintString(string)");

			if(hNativePrintFunc != XENON_FUNCTION_HANDLE_NULL)
			{
				auto printString = [](XenonExecutionHandle hExec, XenonFunctionHandle, void*)
				{
					XenonValueHandle hInputParam = XENON_VALUE_HANDLE_NULL;
					XenonExecutionGetIoRegister(hExec, &hInputParam, 0);

					const char* const inputParam = XenonValueGetString(hInputParam);

					printf("> \"%s\"\n", inputParam);
					XenonValueAbandon(hInputParam);
				};

				XenonFunctionSetNativeBinding(hNativePrintFunc, printString, nullptr);
			}

			XenonFunctionHandle hNativeDecrementFunc = XENON_FUNCTION_HANDLE_NULL;
			XenonVmGetFunction(hVm, &hNativeDecrementFunc, "(int32, bool) App.Program.Decrement(int32)");

			if(hNativeDecrementFunc != XENON_FUNCTION_HANDLE_NULL)
			{
				auto decrement = [](XenonExecutionHandle hExec, XenonFunctionHandle, void*)
				{
					XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
					XenonExecutionGetVm(hExec, &hVm);

					XenonValueHandle hInputParam = XENON_VALUE_HANDLE_NULL;
					XenonExecutionGetIoRegister(hExec, &hInputParam, 0);

					if(XenonValueIsInt32(hInputParam))
					{
						const int32_t output = XenonValueGetInt32(hInputParam) - 1;

						XenonValueHandle hOutputParam = XenonValueCreateInt32(hVm, output);
						XenonExecutionSetIoRegister(hExec, hOutputParam, 0);
						XenonValueAbandon(hOutputParam);

						if(output <= 0)
						{
							hOutputParam = XenonValueCreateBool(hVm, true);
						}
						else
						{
							hOutputParam = XenonValueCreateNull();
						}

						XenonExecutionSetIoRegister(hExec, hOutputParam, 1);
						XenonValueAbandon(hOutputParam);
					}
					else
					{
						XenonExecutionRaiseStandardException(
							hExec,
							XENON_EXCEPTION_SEVERITY_NORMAL,
							XENON_STANDARD_EXCEPTION_TYPE_ERROR,
							"Type mismatch; expected int32"
						);
					}
				};

				XenonFunctionSetNativeBinding(hNativeDecrementFunc, decrement, nullptr);
			}

			XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Executing script function: \"%s\"", entryPoint);

			bool status;

			//allocationCount = 0;
			//mallocCount = 0;
			//reallocCount= 0;

			const uint64_t runProgramTimeStart = XenonHiResTimerGetTimestamp();
			int result = XENON_SUCCESS;

			// Run the script until it has completed.
			for(;;)
			{
				result = XenonExecutionRun(hExec, XENON_RUN_CONTINUOUS);
				if(result != XENON_SUCCESS)
				{
					XenonReportMessage(hReport, XENON_MESSAGE_TYPE_ERROR, "Error occurred while executing script: \"%s\"", XenonGetErrorCodeString(result));
					break;
				}

				// Check if there was an unhandled exception raised.
				result = XenonExecutionGetStatus(hExec, XENON_EXEC_STATUS_EXCEPTION, &status);
				if(result != XENON_SUCCESS)
				{
					XenonReportMessage(hReport, XENON_MESSAGE_TYPE_ERROR, "Error occurred while retrieving exception status: \"%s\"", XenonGetErrorCodeString(result));
					break;
				}
				if(status)
				{
					XenonReportMessage(hReport, XENON_MESSAGE_TYPE_ERROR, "Unhandled exception occurred");

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

					applicationResult = PROGRAM_RESULT_FAILURE;
					break;
				}

				// Check if the script has finished running.
				result = XenonExecutionGetStatus(hExec, XENON_EXEC_STATUS_COMPLETE, &status);
				if(result != XENON_SUCCESS)
				{
					XenonReportMessage(hReport, XENON_MESSAGE_TYPE_ERROR, "Error occurred while retrieving completion status: \"%s\"", XenonGetErrorCodeString(result));
					break;
				}
				if(status)
				{
					XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Finished executing script");
					break;
				}

				// Check if the script has been aborted.
				result = XenonExecutionGetStatus(hExec, XENON_EXEC_STATUS_ABORT, &status);
				if(result != XENON_SUCCESS)
				{
					XenonReportMessage(hReport, XENON_MESSAGE_TYPE_ERROR, "Error occurred while retrieving abort status: \"%s\"", XenonGetErrorCodeString(result));
					break;
				}
				if(status)
				{
					XenonReportMessage(hReport, XENON_MESSAGE_TYPE_WARNING, "Script execution aborted");
					break;
				}
			}

			const uint64_t runProgramTimeEnd = XenonHiResTimerGetTimestamp();
			runProgramTimeSlice = runProgramTimeEnd - runProgramTimeStart;

			const uint64_t disposeExecTimeStart = runProgramTimeEnd;

			XenonExecutionDispose(&hExec);

			const uint64_t disposeExecTimeEnd = XenonHiResTimerGetTimestamp();
			disposeExecTimeSlice = disposeExecTimeEnd - disposeExecTimeStart;
		}
	}
	else
	{
		applicationResult = PROGRAM_RESULT_FAILURE;
	}

	const uint64_t disposeVmTimeStart = XenonHiResTimerGetTimestamp();

	// Dispose of the VM context.
	const int disposeVmResult = XenonVmDispose(&hVm);
	if(disposeVmResult != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg), "Failed to dispose of Xenon VM context: error=\"%s\"", XenonGetErrorCodeString(disposeVmResult));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_WARNING, msg);
	}

	const uint64_t disposeVmTimeEnd = XenonHiResTimerGetTimestamp();
	const uint64_t disposeVmTimeSlice = disposeVmTimeEnd - disposeVmTimeStart;

	if(!allocations.empty())
	{
		char msg[128];
		snprintf(msg, sizeof(msg), "Leaked script allocations: %zu", allocations.size());
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_ERROR, msg);

		applicationResult = PROGRAM_RESULT_FAILURE;
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
		allocationCount,
		mallocCount,
		reallocCount
	);

	const uint64_t overallTimeEnd = XenonHiResTimerGetTimestamp();
	const uint64_t overallTimeSlice = overallTimeEnd - overallTimeStart;

	const double convertTimeToMs = 1000.0 / double(timerFrequency);

	// Output the timing metrics.
	printf(
		"Timing metrics:\n"
		"\tTotal time: %f ms\n"
		"\tCreate VM time: %f ms\n"
		"\tDispose VM time: %f ms\n"
		"\tCreate exec-context time: %f ms\n"
		"\tDispose exec-context time: %f ms\n"
		"\tLoad program time: %f ms\n"
		"\tRun program time: %f ms\n"
		"\tDisassemble time: %f ms\n",
		double(overallTimeSlice) * convertTimeToMs,
		double(createVmTimeSlice) * convertTimeToMs,
		double(disposeVmTimeSlice) * convertTimeToMs,
		double(createExecTimeSlice) * convertTimeToMs,
		double(disposeExecTimeSlice) * convertTimeToMs,
		double(loadProgramTimeSlice) * convertTimeToMs,
		double(runProgramTimeSlice) * convertTimeToMs,
		double(disassembleTimeSlice) * convertTimeToMs
	);

	return applicationResult;
}

//----------------------------------------------------------------------------------------------------------------------
