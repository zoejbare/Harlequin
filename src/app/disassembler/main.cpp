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

#include "Harlequin.h"

#include "../common/MemoryHandler.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <deque>

#if defined(HQ_PLATFORM_WINDOWS)
	#include <crtdbg.h>
#endif

#if !defined(HQ_PLATFORM_PS3) && !defined(HQ_PLATFORM_PSVITA)
	#include <memory.h>
#endif

#if !defined(HQ_PLATFORM_PSVITA)
	#include <locale.h>
#endif

#define CXXOPTS_NO_EXCEPTIONS
#define CXXOPTS_NO_RTTI
#include <cxxopts.hpp>

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

void OnMessageReported(void*, const int messageType, const char* const message)
{
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

	FILE* const pStream = (messageType >= HQ_MESSAGE_TYPE_WARNING) ? stderr : stdout;

	fprintf(pStream, "[%s] %s\n", tag, message);
	fflush(pStream);
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
#if !defined(HQ_PLATFORM_PSVITA)
	// Set the program locale to the environment default.
	setlocale(LC_ALL, "");
#endif

#if defined(HQ_PLATFORM_WINDOWS)
	// This enables tracking of global heap allocations.  If any are leaked, they will show up in the
	// Visual Studio output window on application exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	cxxopts::Options options(
#if defined(HQ_PLATFORM_WINDOWS)
		"hqd.exe",
#else
		"hqd",
#endif
		"Utility for disassembling compiled Harlequin modules"
	);

	options
		.custom_help("[options...]")
		.positional_help("<file>")
		.allow_unrecognised_options();

	// Add the options.
	options.add_options()
		("h,help", "Display this help text")
		("file", "File path of module to disassemble", cxxopts::value<std::string>(), "<file>")
		("o,output", "Write the module disassembly to a file", cxxopts::value<std::string>(), "file")
		("s,strings", "Dump the module string table")
		("d,deps", "Dump a list of the module dependencies")
		("q,quiet", "Disable all logging exception errors")
		("v,verbose", "Enable verbose logging (overrides -q/--quiet)");

	// Define which of the above arguments are positional.
	options.parse_positional({ "file" });

	// Parse the application's command line arguments.
	cxxopts::ParseResult args = options.parse(argc, argv);

	if(args.count("help"))
	{
		// Print the help text, then exit.
		printf("%s\n", options.help({ "" }).c_str());
		fflush(stdout);
		return APPLICATION_RESULT_SUCCESS;
	}

	// Check for the <file> argument.
	if(args.count("file") == 0)
	{
		char msg[48];
		snprintf(msg, sizeof(msg), "Missing required argument: <file>");
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, msg);
		return APPLICATION_RESULT_FAILURE;
	}

	struct UtilConfig
	{
		FILE* pOutputStream;

		const char* inputFilePath;
		const char* outputFilePath;
		const char* moduleName;

		bool dumpStrings;
		bool dumpDeps;

		bool quietLogging;
		bool verboseLogging;
	};

	// Get the input file path from the command line and make sure it's not empty.
	const std::string inputFilePath = args["file"].as<std::string>();
	if(inputFilePath.size() == 0)
	{
		char msg[32];
		snprintf(msg, sizeof(msg), "%s", "Input file path is empty");
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, msg);
		return APPLICATION_RESULT_FAILURE;
	}

	std::string outputFilePath;
	if(args.count("output"))
	{
		// Get the output file path from the command line and make sure it's not empty.
		outputFilePath = args["output"].as<std::string>();
		if(outputFilePath.size() == 0)
		{
			char msg[32];
			snprintf(msg, sizeof(msg), "%s", "Output file path is empty");
			OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, msg);
			return APPLICATION_RESULT_FAILURE;
		}
	}

	UtilConfig utilConfig;

	// The disassembly output stream defaults to stdout.
	utilConfig.pOutputStream = stdout;

	// Cache the raw strings for the file paths.
	utilConfig.inputFilePath = inputFilePath.c_str();
	utilConfig.outputFilePath = (outputFilePath.size() > 0) ? outputFilePath.c_str() : nullptr;

	// Attempt to resolve the input file path to its base filename to use for the module name.
	utilConfig.moduleName = strrchr(utilConfig.inputFilePath, '/');

#if defined(HQ_PLATFORM_WINDOWS)
	if(!utilConfig.moduleName)
	{
		utilConfig.moduleName = strrchr(utilConfig.inputFilePath, '\\');
	}
#endif

	if(utilConfig.moduleName)
	{
		// Move ahead by one so the '/' is not included at the start of the name.
		utilConfig.moduleName += 1;
	}
	else
	{
		// No directories were specified in the file path, so we can use it directly as the module name.
		utilConfig.moduleName = utilConfig.inputFilePath;
	}

	utilConfig.dumpStrings = (args.count("strings") > 0);
	utilConfig.dumpDeps = (args.count("deps") > 0);

	utilConfig.quietLogging = (args.count("quiet") > 0);
	utilConfig.verboseLogging = (args.count("verbose") > 0);

	// Show a warning if "-q" and "-v" have been used together.
	if(utilConfig.quietLogging && utilConfig.verboseLogging)
	{
		char msg[96];
		snprintf(msg, sizeof(msg), "Quiet logging and verbose logging are both enabled; verbose logging will be selected");
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_WARNING, msg);
	}

	// Handle any required platform initialization.
	_HqAppInit();

	MemoryHandler& memory = MemoryHandler::Instance;

	// Initialize the Harlequin memory handler.
	memory.Initialize();

	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqVmInit vmInit;

	std::deque<const char*> dependencies;

	vmInit.common.report.onMessageFn = OnMessageReported;
	vmInit.common.report.pUserData = nullptr;
	vmInit.common.report.reportLevel = utilConfig.verboseLogging
		? HQ_MESSAGE_TYPE_VERBOSE
		: utilConfig.quietLogging
			? HQ_MESSAGE_TYPE_ERROR
			: HQ_MESSAGE_TYPE_INFO;

	vmInit.gcThreadStackSize = HQ_VM_THREAD_DEFAULT_STACK_SIZE;
	vmInit.gcTimeSliceMs = HQ_VM_GC_DEFAULT_TIME_SLICE_MS;
	vmInit.gcTimeWaitMs = HQ_VM_GC_DEFAULT_TIME_WAIT_MS;
	vmInit.gcEnableThread = false;

	// Create the VM context.
	{
		const int createVmResult = HqVmCreate(&hVm, vmInit);
		if(createVmResult != HQ_SUCCESS)
		{
			char msg[128];
			snprintf(msg, sizeof(msg), "Failed to create Harlequin VM context: error='%s'", HqGetErrorCodeString(createVmResult));
			OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, msg);
			_HqAppShutdown();
			return APPLICATION_RESULT_FAILURE;
		}
	}

	HqReportHandle hReport = HQ_REPORT_HANDLE_NULL;
	HqVmGetReportHandle(hVm, &hReport);

	// Load the module file.
	{
		HqSerializerHandle hFileSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Create the serializer we'll use to read the module file data.
		const int createFileSerializerResult = HqSerializerCreate(&hFileSerializer, HQ_SERIALIZER_MODE_READER);
		if(createFileSerializerResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to create Harlequin serializer: error='%s'",
				HqGetErrorCodeString(createFileSerializerResult)
			);
			HqVmDispose(&hVm);
			_HqAppShutdown();
			return APPLICATION_RESULT_FAILURE;
		}

		std::vector<uint8_t> fileData;

		HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Reading module file to memory");

		const int readModuleFileResult = HqSerializerLoadStreamFromFile(hFileSerializer, utilConfig.inputFilePath);
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
				"Failed to read module from disk"
					": file='%s'"
					", error='%s'",
				utilConfig.inputFilePath,
				HqGetErrorCodeString(readModuleFileResult)
			);
			HqVmDispose(&hVm);
			_HqAppShutdown();
			return APPLICATION_RESULT_FAILURE;
		}

		// Load the module data into the VM.
		const int loadModuleResult = HqVmLoadModule(hVm, utilConfig.moduleName, &fileData[0], fileData.size());
		if(loadModuleResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to load module"
					": file='%s'"
					", error='%s'",
				utilConfig.inputFilePath,
				HqGetErrorCodeString(loadModuleResult)
			);
			HqVmDispose(&hVm);
			_HqAppShutdown();
			return APPLICATION_RESULT_FAILURE;
		}
	}

	// Generate the script module disassembly.
	{
		auto iterateString = [](void* const pUserData, const char* const string, const size_t index) -> bool
		{
			const UtilConfig& config = *reinterpret_cast<UtilConfig*>(pUserData);
			const size_t length = strlen(string);

			fprintf(
				config.pOutputStream,
				"        s(%" PRIuPTR "): \"%.48s\"%s\n",
				index,
				string,
				(length > 48) ? " ..." : ""
			);
			return true;
		};

		auto iterateDependencies = [](void* const pUserData, const char* const dependency) -> bool
		{
			const UtilConfig& config = *reinterpret_cast<UtilConfig*>(pUserData);

			fprintf(config.pOutputStream, "        %s\n", dependency);
			return true;
		};

		static auto onDisasm = [](void* const pUserData, const char* const asmLine, const uintptr_t offset)
		{
			const UtilConfig& config = *reinterpret_cast<UtilConfig*>(pUserData);

			fprintf(config.pOutputStream, "            0x%08" PRIXPTR ": %s\n", offset, asmLine);
		};

		auto iterateFunction = [](void* const pUserData, HqFunctionHandle hFunc) -> bool
		{
			const UtilConfig& config = *reinterpret_cast<UtilConfig*>(pUserData);

			const char* signature = nullptr;
			HqFunctionGetSignature(hFunc, &signature);

			fprintf(config.pOutputStream, "        %s\n", signature);

			bool isNative = false;
			HqFunctionIsNative(hFunc, &isNative);

			if(isNative)
			{
				fprintf(config.pOutputStream, "            <native call>\n");
			}
			else
			{
				HqFunctionDisassemble(hFunc, onDisasm, pUserData);
			}

			fprintf(config.pOutputStream, "\n");
			return true;
		};

		HqModuleHandle hModule = HQ_MODULE_HANDLE_NULL;

		const int getModuleResult = HqVmGetModule(hVm, &hModule, utilConfig.moduleName);
		if(getModuleResult != HQ_SUCCESS)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_FATAL,
				"Failed to retrieve module"
					": name='%s'"
					", error='%s'",
				utilConfig.moduleName,
				HqGetErrorCodeString(getModuleResult)
			);
			HqVmDispose(&hVm);
			_HqAppShutdown();
			return APPLICATION_RESULT_FAILURE;
		}

		HqReportMessage(hReport, HQ_MESSAGE_TYPE_INFO, "Disassembling module ...");

		if(utilConfig.outputFilePath)
		{

			// Attempt to open the output file.
			utilConfig.pOutputStream = fopen(utilConfig.outputFilePath, "w");
			if(!utilConfig.pOutputStream)
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_FATAL,
					"Failed to open output file"
						": file='%s'",
					utilConfig.outputFilePath
				);
				HqVmDispose(&hVm);
				_HqAppShutdown();
				return APPLICATION_RESULT_FAILURE;
			}
		}

		fprintf(utilConfig.pOutputStream, "[Module: \"%s\"]", utilConfig.moduleName);

		if(utilConfig.dumpStrings)
		{
			fprintf(utilConfig.pOutputStream, "\n    [strings]\n");
			HqModuleListStrings(hModule, iterateString, &utilConfig);
		}

		if(utilConfig.dumpDeps)
		{
			fprintf(utilConfig.pOutputStream, "\n    [dependencies]\n");
			HqModuleListDependencies(hModule, iterateDependencies, &utilConfig);
		}

		fprintf(utilConfig.pOutputStream, "\n    [code]\n");

		// Iterate each function within in the module to disassemble them.
		HqModuleListFunctions(hModule, iterateFunction, &utilConfig);

		// Flush the output stream to finish writing all the disassembly text.
		fflush(utilConfig.pOutputStream);

		if(utilConfig.pOutputStream != stdout)
		{
			// Close the file handle, then reset the stream back to stdout.
			fclose(utilConfig.pOutputStream);
			utilConfig.pOutputStream = stdout;

			HqReportMessage(hReport, HQ_MESSAGE_TYPE_INFO, "Module disassembly written to file: %s", utilConfig.outputFilePath);
		}
	}

	// Dispose of the VM context.
	{
		const int disposeVmResult = HqVmDispose(&hVm);
		if(disposeVmResult != HQ_SUCCESS)
		{
			char msg[128];
			snprintf(
				msg,
				sizeof(msg),
				"Failed to dispose of Harlequin VM context"
					": error='%s'",
				HqGetErrorCodeString(disposeVmResult)
			);
			OnMessageReported(nullptr, HQ_MESSAGE_TYPE_WARNING, msg);
		}
	}

	// Check for memory leaks.
	if(memory.HasActiveAllocations())
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Memory leaks detected: %zu, totalSize=%zu", memory.GetActiveCount(), memory.GetCurrentSize());
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_ERROR, msg);
	}

	if(utilConfig.verboseLogging)
	{
		memory.PrintStats(stdout);
	}

	// Shutdown the platform-specific, internal systems.
	_HqAppShutdown();

	return APPLICATION_RESULT_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------
