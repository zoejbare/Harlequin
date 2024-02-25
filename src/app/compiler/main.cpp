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
#include <locale.h>
#include <stdio.h>
#include <string.h>

#if defined(HQ_PLATFORM_WINDOWS)
	#include <malloc.h>
	#define alloca _alloca
#else
	#include <alloca.h>
#endif

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

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	// Set the program locale to the environment default.
	setlocale(LC_ALL, "");

	MemoryHandler& memory = MemoryHandler::Instance;

	// Initialize the Harlequin memory handler.
	memory.Initialize();

	HqDevContextHandle hCtx = HQ_DEV_CONTEXT_HANDLE_NULL;
	HqDevContextInit init;

	init.common.report.onMessageFn = OnMessageReported;
	init.common.report.pUserData = nullptr;
	init.common.report.reportLevel = HQ_MESSAGE_TYPE_VERBOSE;

	// Create the develop context.
	int result = HqDevContextCreate(&hCtx, init);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Harlequin develop context: error=\"%s\"", HqGetErrorCodeString(result));
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, msg);
		return 1;
	}

	HqModuleWriterHandle hModuleWriter = HQ_MODULE_WRITER_HANDLE_NULL;

	// Create the module writer.
	result = HqModuleWriterCreate(&hModuleWriter, hCtx);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Harlequin module writer: error=\"%s\"", HqGetErrorCodeString(result));
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_FATAL, msg);
		return 1;
	}

	HqSerializerHandle hFileSerializer = HQ_SERIALIZER_HANDLE_NULL;
	HqSerializerHandle hMainFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;
	HqSerializerHandle hSubFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;
	HqSerializerHandle hExcFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

	HqSerializerCreate(&hFileSerializer, HQ_SERIALIZER_MODE_WRITER);
	HqSerializerCreate(&hMainFuncSerializer, HQ_SERIALIZER_MODE_WRITER);
	HqSerializerCreate(&hSubFuncSerializer, HQ_SERIALIZER_MODE_WRITER);
	HqSerializerCreate(&hExcFuncSerializer, HQ_SERIALIZER_MODE_WRITER);

	const int endianness = HQ_ENDIAN_ORDER_BIG;

	// Set the endianness of each serializer.
	HqSerializerSetEndianness(hFileSerializer, endianness);
	HqSerializerSetEndianness(hMainFuncSerializer, endianness);
	HqSerializerSetEndianness(hSubFuncSerializer, endianness);
	HqSerializerSetEndianness(hExcFuncSerializer, endianness);

	assert(HqSerializerGetEndianness(hFileSerializer) == HqSerializerGetEndianness(hSubFuncSerializer));
	assert(HqSerializerGetEndianness(hFileSerializer) == HqSerializerGetEndianness(hMainFuncSerializer));
	assert(HqSerializerGetEndianness(hFileSerializer) == HqSerializerGetEndianness(hExcFuncSerializer));

	const char* const mainFuncSignature = "void Program.Main()";
	const char* const subFuncSignature = "int32 Program.DoWork(float64)";
	const char* const excFuncSignature = "void Program.ExceptionHandlingTest()";
	const char* const nativePrintFuncSignature = "void Program.PrintString(string)";
	const char* const nativeDecrementFuncSignature = "(int32, bool) Program.Decrement(int32)";
	const char* const globalVariableName = "globalTestVar";
	const char* const objectTypeName = "Program.TestClass";
	const char* const objectMemberName = "memberTest";

	uint32_t stringIndex0; HqModuleWriterAddString(hModuleWriter, "this is ", &stringIndex0);
	uint32_t stringIndex1; HqModuleWriterAddString(hModuleWriter, "a test string", &stringIndex1);
	uint32_t stringIndex2; HqModuleWriterAddString(hModuleWriter, globalVariableName, &stringIndex2);
	uint32_t stringIndex3; HqModuleWriterAddString(hModuleWriter, subFuncSignature, &stringIndex3);
	uint32_t stringIndex4; HqModuleWriterAddString(hModuleWriter, nativePrintFuncSignature, &stringIndex4);
	uint32_t stringIndex8; HqModuleWriterAddString(hModuleWriter, objectTypeName, &stringIndex8);
	uint32_t stringIndex9; HqModuleWriterAddString(hModuleWriter, objectMemberName, &stringIndex9);
	uint32_t stringIndex10; HqModuleWriterAddString(hModuleWriter, nativeDecrementFuncSignature, &stringIndex10);
	uint32_t stringIndex13; HqModuleWriterAddString(hModuleWriter, excFuncSignature, &stringIndex13);
	uint32_t stringIndex14; HqModuleWriterAddString(hModuleWriter, "This is an exception string", &stringIndex14);
	uint32_t stringIndex15; HqModuleWriterAddString(hModuleWriter, "The exception was handled successfully!", &stringIndex15);

	// Add the module globals.
	HqModuleWriterAddGlobal(hModuleWriter, globalVariableName);

	// Write the module object types.
	HqModuleWriterAddObjectType(hModuleWriter, objectTypeName);
	HqModuleWriterAddObjectMember(hModuleWriter, objectTypeName, objectMemberName, HQ_VALUE_TYPE_INT32, nullptr);

	// void Program.Main()
	{
		HqBytecodeEmitLoadImmNull(hMainFuncSerializer, 0);
		HqBytecodeEmitLoadImmI8(hMainFuncSerializer, 1, 20);
		HqBytecodeEmitLoadImmI16(hMainFuncSerializer, 2, 31);
		HqBytecodeEmitLoadImmI32(hMainFuncSerializer, 3, 123);
		HqBytecodeEmitLoadImmI64(hMainFuncSerializer, 4, 53);
		HqBytecodeEmitLoadImmU8(hMainFuncSerializer, 5, 23);
		HqBytecodeEmitLoadImmU16(hMainFuncSerializer, 6, 75);
		HqBytecodeEmitLoadImmU32(hMainFuncSerializer, 7, 86);
		HqBytecodeEmitLoadImmU64(hMainFuncSerializer, 8, 97);
		HqBytecodeEmitLoadImmF32(hMainFuncSerializer, 9, 3.1415926535897932384626433832795f);
		HqBytecodeEmitLoadImmF64(hMainFuncSerializer, 10, 1.2345);
		HqBytecodeEmitLoadImmF64(hMainFuncSerializer, 11, 2.3456);
		HqBytecodeEmitLoadImmStr(hMainFuncSerializer, 12, stringIndex0);
		HqBytecodeEmitLoadImmStr(hMainFuncSerializer, 13, stringIndex1);
		HqBytecodeEmitLoadImmStr(hMainFuncSerializer, 14, stringIndex2);
		HqBytecodeEmitNop(hMainFuncSerializer);

		HqBytecodeEmitTest(hMainFuncSerializer, 20, 0);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 1);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 2);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 3);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 4);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 5);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 6);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 7);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 8);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 9);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 10);
		HqBytecodeEmitTest(hMainFuncSerializer, 20, 12);

		HqBytecodeEmitLoadImmI8(hMainFuncSerializer, 20, 7);
		HqBytecodeEmitMod(hMainFuncSerializer, 20, 1, 20);

		HqBytecodeEmitLoadImmF32(hMainFuncSerializer, 20, 0.5772156649015328606065120900824f);
		HqBytecodeEmitMod(hMainFuncSerializer, 20, 9, 20);

		HqBytecodeEmitLoadImmI32(hMainFuncSerializer, 20, 3);
		HqBytecodeEmitExp(hMainFuncSerializer, 20, 3, 20);

		HqBytecodeEmitLoadImmF32(hMainFuncSerializer, 20, 2.7182818284590452353602874713527f);
		HqBytecodeEmitExp(hMainFuncSerializer, 20, 9, 20);

		HqBytecodeEmitLoadImmI8(hMainFuncSerializer, 20, 100);
		HqBytecodeEmitBitAnd(hMainFuncSerializer, 20, 1, 20);

		HqBytecodeEmitLoadImmI8(hMainFuncSerializer, 20, 100);
		HqBytecodeEmitBitOr(hMainFuncSerializer, 20, 1, 20);

		HqBytecodeEmitLoadImmI8(hMainFuncSerializer, 20, 100);
		HqBytecodeEmitBitXor(hMainFuncSerializer, 20, 1, 20);

		HqBytecodeEmitLoadImmU8(hMainFuncSerializer, 20, 3);
		HqBytecodeEmitLeftShift(hMainFuncSerializer, 20, 5, 20);

		HqBytecodeEmitLoadImmU8(hMainFuncSerializer, 20, 2);
		HqBytecodeEmitRightShift(hMainFuncSerializer, 20, 5, 20);

		HqBytecodeEmitLoadImmU8(hMainFuncSerializer, 20, 2);
		HqBytecodeEmitLeftRotate(hMainFuncSerializer, 20, 5, 20);

		HqBytecodeEmitLoadImmU8(hMainFuncSerializer, 20, 4);
		HqBytecodeEmitRightRotate(hMainFuncSerializer, 20, 5, 20);

		HqBytecodeEmitLoadGlobal(hMainFuncSerializer, 15, stringIndex2);
		HqBytecodeEmitStoreGlobal(hMainFuncSerializer, stringIndex2, 13);

		HqBytecodeEmitStoreParam(hMainFuncSerializer, 0, 11);
		HqBytecodeEmitCall(hMainFuncSerializer, stringIndex3);
		HqBytecodeEmitLoadParam(hMainFuncSerializer, 8, 0);

		HqBytecodeEmitInitFunction(hMainFuncSerializer, 0, stringIndex13);
		HqBytecodeEmitCallValue(hMainFuncSerializer, 0);

		HqBytecodeEmitReturn(hMainFuncSerializer);

		const void* const pMainFuncData = HqSerializerGetRawStreamPointer(hMainFuncSerializer);
		const size_t mainFuncLength = HqSerializerGetStreamLength(hMainFuncSerializer);

		HqModuleWriterAddFunction(hModuleWriter, mainFuncSignature, pMainFuncData, mainFuncLength, 0, 0);
	}

	// int32 Program.DoWork(float64)
	{
		HqBytecodeEmitLoadImmF64(hSubFuncSerializer, 0, 1.2345);
		HqBytecodeEmitMove(hSubFuncSerializer, 1, 0);

		HqBytecodeEmitPush(hSubFuncSerializer, 0);
		HqBytecodeEmitYield(hSubFuncSerializer);
		HqBytecodeEmitPop(hSubFuncSerializer, 1);

		HqBytecodeEmitInitObject(hSubFuncSerializer, 2, stringIndex8);
		HqBytecodeEmitLoadImmI32(hSubFuncSerializer, 1, 5);
		HqBytecodeEmitStoreObject(hSubFuncSerializer, 2, 1, 0);

		HqBytecodeEmitMove(hSubFuncSerializer, 3, 2);
		HqBytecodeEmitCompareEqual(hSubFuncSerializer, 3, 2, 3);

		HqBytecodeEmitCopy(hSubFuncSerializer, 3, 2);
		HqBytecodeEmitCompareEqual(hSubFuncSerializer, 3, 2, 3);

		HqBytecodeEmitLoadImmStr(hSubFuncSerializer, 0, stringIndex0);
		HqBytecodeEmitLoadImmStr(hSubFuncSerializer, 1, stringIndex1);
		HqBytecodeEmitAdd(hSubFuncSerializer, 0, 0, 1);

		HqBytecodeEmitStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeEmitCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeEmitLength(hSubFuncSerializer, 1, 0);
		HqBytecodeEmitCastString(hSubFuncSerializer, 1, 1);

		HqBytecodeEmitStoreParam(hSubFuncSerializer, 0, 1);
		HqBytecodeEmitCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeEmitLoadImmI32(hSubFuncSerializer, 0, 5);
		HqBytecodeEmitStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeEmitStoreParam(hSubFuncSerializer, 1, 0);

		const size_t loopOffsetStart = HqSerializerGetStreamPosition(hSubFuncSerializer);

		HqBytecodeEmitLoadObject(hSubFuncSerializer, 0, 2, 0);
		HqBytecodeEmitCastString(hSubFuncSerializer, 1, 0);

		HqBytecodeEmitStoreParam(hSubFuncSerializer, 0, 1);
		HqBytecodeEmitCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeEmitStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeEmitCall(hSubFuncSerializer, stringIndex10);
		HqBytecodeEmitLoadParam(hSubFuncSerializer, 0, 0);
		HqBytecodeEmitStoreObject(hSubFuncSerializer, 2, 0, 0);

		HqBytecodeEmitLoadParam(hSubFuncSerializer, 0, 1);
		HqBytecodeEmitJumpIfTrue(hSubFuncSerializer, 0, 20);

		const size_t loopOffsetEnd = HqSerializerGetStreamPosition(hSubFuncSerializer);
		const int32_t loopRelativeOffset = int32_t(loopOffsetEnd - loopOffsetStart);

		HqBytecodeEmitJump(hSubFuncSerializer, -loopRelativeOffset);

		HqBytecodeEmitLoadObject(hSubFuncSerializer, 0, 2, 0);
		HqBytecodeEmitJumpIfFalse(hSubFuncSerializer, 0, 16);
		HqBytecodeEmitAbort(hSubFuncSerializer);

		HqBytecodeEmitStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeEmitReturn(hSubFuncSerializer);

		const void* const pFuncData = HqSerializerGetRawStreamPointer(hSubFuncSerializer);
		const size_t funcLength = HqSerializerGetStreamLength(hSubFuncSerializer);

		HqModuleWriterAddFunction(hModuleWriter, subFuncSignature, pFuncData, funcLength, 1, 1);
	}

	// void Program.PrintString(string)
	{
		HqModuleWriterAddNativeFunction(hModuleWriter, nativePrintFuncSignature, 1, 0);
	}

	// (int32, bool) Program.Decrement(int32)
	{
		HqModuleWriterAddNativeFunction(hModuleWriter, nativeDecrementFuncSignature, 1, 2);
	}

	// void Program.ExceptionHandlingTest()
	{
		size_t tryBlockStart;
		size_t tryBlockEnd;
		size_t exceptionHandlerStart;

		// Write the 'try' block.
		{
			tryBlockStart = HqSerializerGetStreamPosition(hExcFuncSerializer);

			HqBytecodeEmitInitArray(hExcFuncSerializer, 0, 4);
			HqBytecodeEmitLoadImmStr(hExcFuncSerializer, 1, stringIndex14);
			HqBytecodeEmitLoadImmStr(hExcFuncSerializer, 2, stringIndex8);
			HqBytecodeEmitLoadImmU8(hExcFuncSerializer, 3, 1);
			HqBytecodeEmitStoreArray(hExcFuncSerializer, 0, 1, 3);
			HqBytecodeEmitLoadImmU16(hExcFuncSerializer, 3, 3);
			HqBytecodeEmitStoreArray(hExcFuncSerializer, 0, 2, 3);
			HqBytecodeEmitLoadImmNull(hExcFuncSerializer, 2);

			HqBytecodeEmitLength(hExcFuncSerializer, 1, 0);

			HqBytecodeEmitLoadImmU32(hExcFuncSerializer, 3, 1);
			HqBytecodeEmitLoadArray(hExcFuncSerializer, 0, 0, 3);
			HqBytecodeEmitRaise(hExcFuncSerializer, 0);
			HqBytecodeEmitAbort(hExcFuncSerializer);

			tryBlockEnd = HqSerializerGetStreamPosition(hExcFuncSerializer);
		}

		HqBytecodeEmitNop(hExcFuncSerializer);
		HqBytecodeEmitNop(hExcFuncSerializer);
		HqBytecodeEmitNop(hExcFuncSerializer);

		// Write the exception handling section.
		{
			exceptionHandlerStart = HqSerializerGetStreamPosition(hExcFuncSerializer);

			HqBytecodeEmitCall(hExcFuncSerializer, stringIndex4);
			HqBytecodeEmitJump(hExcFuncSerializer, 8);
		}

		HqBytecodeEmitNop(hExcFuncSerializer);
		HqBytecodeEmitNop(hExcFuncSerializer);
		HqBytecodeEmitNop(hExcFuncSerializer);

		HqBytecodeEmitLoadImmStr(hExcFuncSerializer, 0, stringIndex15);
		HqBytecodeEmitStoreParam(hExcFuncSerializer, 0, 0);
		HqBytecodeEmitCall(hExcFuncSerializer, stringIndex4);

		HqBytecodeEmitReturn(hExcFuncSerializer);

		const void* const pFuncData = HqSerializerGetRawStreamPointer(hExcFuncSerializer);
		const size_t funcLength = HqSerializerGetStreamLength(hExcFuncSerializer);

		HqModuleWriterAddFunction(hModuleWriter, excFuncSignature, pFuncData, funcLength, 0, 0);

		// Setup the exception handling data on the function.
		{
			uint32_t tryBlockId = 0;

			HqModuleWriterAddGuardedBlock(hModuleWriter, excFuncSignature, tryBlockStart, tryBlockEnd - tryBlockStart, &tryBlockId);
			HqModuleWriterAddExceptionHandler(hModuleWriter, excFuncSignature, tryBlockId, exceptionHandlerStart, HQ_VALUE_TYPE_STRING, nullptr);
		}
	}

	result = HqModuleWriterSerialize(hModuleWriter, hFileSerializer);
	if(result == HQ_SUCCESS)
	{
		const void* const pFileData = HqSerializerGetRawStreamPointer(hFileSerializer);
		const size_t fileLength = HqSerializerGetStreamLength(hFileSerializer);

		const char* const outputFilename = "test.bin";

		char msg[128];
		snprintf(msg, sizeof(msg), "Writing file: %s", outputFilename);
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_INFO, msg);

		FILE* const pOutputFile = fopen(outputFilename, "wb");
		fwrite(pFileData, fileLength, 1, pOutputFile);
		fclose(pOutputFile);
	}

	HqSerializerDispose(&hMainFuncSerializer);
	HqSerializerDispose(&hSubFuncSerializer);
	HqSerializerDispose(&hExcFuncSerializer);
	HqSerializerDispose(&hFileSerializer);

	// Dispose of the module writer.
	result = HqModuleWriterDispose(&hModuleWriter);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Harlequin module writer: error=\"%s\"", HqGetErrorCodeString(result));
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_WARNING, msg);
	}

	// Dispose of the develop context.
	result = HqDevContextDispose(&hCtx);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Harlequin develop context: error=\"%s\"", HqGetErrorCodeString(result));
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_WARNING, msg);
	}

	// Check for memory leaks.
	if(memory.HasActiveAllocations())
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Memory leaks detected: %zu, totalSize=%zu", memory.GetActiveCount(), memory.GetCurrentSize());
		OnMessageReported(nullptr, HQ_MESSAGE_TYPE_ERROR, msg);
	}

	// Shut down the memory handler.
	memory.Shutdown();

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
