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
#include <locale.h>
#include <stdio.h>

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

	fprintf((messageType >= HQ_MESSAGE_TYPE_ERROR) ? stderr : stdout, "[%s] %s\n", tag, message);
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	// Set the program locale to the environment default.
	setlocale(LC_ALL, "");

#if defined(HQ_PLATFORM_WINDOWS)
	// This enables tracking of global heap allocations.  If any are leaked, they will show up in the
	// Visual Studio output window on application exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	HqCompilerHandle hCompiler = HQ_COMPILER_HANDLE_NULL;
	HqCompilerInit init;

	init.common.report.onMessageFn = OnMessageReported;
	init.common.report.pUserData = nullptr;
	init.common.report.reportLevel = HQ_MESSAGE_TYPE_VERBOSE;

	// Create the compiler context.
	int result = HqCompilerCreate(&hCompiler, init);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Harlequin compiler context: error=\"%s\"", HqGetErrorCodeString(result));
		OnMessageReported(NULL, HQ_MESSAGE_TYPE_FATAL, msg);
		return 1;
	}

	HqModuleWriterHandle hModuleWriter = HQ_MODULE_WRITER_HANDLE_NULL;

	// Create the module writer.
	result = HqModuleWriterCreate(&hModuleWriter, hCompiler);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Harlequin module writer: error=\"%s\"", HqGetErrorCodeString(result));
		OnMessageReported(NULL, HQ_MESSAGE_TYPE_FATAL, msg);
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
		HqBytecodeWriteLoadImmNull(hMainFuncSerializer, 0);
		HqBytecodeWriteLoadImmI8(hMainFuncSerializer, 1, 20);
		HqBytecodeWriteLoadImmI16(hMainFuncSerializer, 2, 31);
		HqBytecodeWriteLoadImmI32(hMainFuncSerializer, 3, 123);
		HqBytecodeWriteLoadImmI64(hMainFuncSerializer, 4, 53);
		HqBytecodeWriteLoadImmU8(hMainFuncSerializer, 5, 23);
		HqBytecodeWriteLoadImmU16(hMainFuncSerializer, 6, 75);
		HqBytecodeWriteLoadImmU32(hMainFuncSerializer, 7, 86);
		HqBytecodeWriteLoadImmU64(hMainFuncSerializer, 8, 97);
		HqBytecodeWriteLoadImmF32(hMainFuncSerializer, 9, 3.1415926535897932384626433832795f);
		HqBytecodeWriteLoadImmF64(hMainFuncSerializer, 10, 1.2345);
		HqBytecodeWriteLoadImmF64(hMainFuncSerializer, 11, 2.3456);
		HqBytecodeWriteLoadImmStr(hMainFuncSerializer, 12, stringIndex0);
		HqBytecodeWriteLoadImmStr(hMainFuncSerializer, 13, stringIndex1);
		HqBytecodeWriteLoadImmStr(hMainFuncSerializer, 14, stringIndex2);
		HqBytecodeWriteNop(hMainFuncSerializer);

		HqBytecodeWriteTest(hMainFuncSerializer, 20, 0);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 1);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 2);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 3);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 4);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 5);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 6);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 7);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 8);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 9);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 10);
		HqBytecodeWriteTest(hMainFuncSerializer, 20, 12);

		HqBytecodeWriteLoadImmI8(hMainFuncSerializer, 20, 7);
		HqBytecodeWriteMod(hMainFuncSerializer, 20, 1, 20);

		HqBytecodeWriteLoadImmF32(hMainFuncSerializer, 20, 0.5772156649015328606065120900824f);
		HqBytecodeWriteMod(hMainFuncSerializer, 20, 9, 20);

		HqBytecodeWriteLoadImmI32(hMainFuncSerializer, 20, 3);
		HqBytecodeWriteExp(hMainFuncSerializer, 20, 3, 20);

		HqBytecodeWriteLoadImmF32(hMainFuncSerializer, 20, 2.7182818284590452353602874713527f);
		HqBytecodeWriteExp(hMainFuncSerializer, 20, 9, 20);

		HqBytecodeWriteLoadImmI8(hMainFuncSerializer, 20, 100);
		HqBytecodeWriteBitAnd(hMainFuncSerializer, 20, 1, 20);

		HqBytecodeWriteLoadImmI8(hMainFuncSerializer, 20, 100);
		HqBytecodeWriteBitOr(hMainFuncSerializer, 20, 1, 20);

		HqBytecodeWriteLoadImmI8(hMainFuncSerializer, 20, 100);
		HqBytecodeWriteBitXor(hMainFuncSerializer, 20, 1, 20);

		HqBytecodeWriteLoadImmU8(hMainFuncSerializer, 20, 3);
		HqBytecodeWriteLeftShift(hMainFuncSerializer, 20, 5, 20);

		HqBytecodeWriteLoadImmU8(hMainFuncSerializer, 20, 2);
		HqBytecodeWriteRightShift(hMainFuncSerializer, 20, 5, 20);

		HqBytecodeWriteLoadImmU8(hMainFuncSerializer, 20, 2);
		HqBytecodeWriteLeftRotate(hMainFuncSerializer, 20, 5, 20);

		HqBytecodeWriteLoadImmU8(hMainFuncSerializer, 20, 4);
		HqBytecodeWriteRightRotate(hMainFuncSerializer, 20, 5, 20);

		HqBytecodeWriteLoadGlobal(hMainFuncSerializer, 15, stringIndex2);
		HqBytecodeWriteStoreGlobal(hMainFuncSerializer, stringIndex2, 13);

		HqBytecodeWriteStoreParam(hMainFuncSerializer, 0, 11);
		HqBytecodeWriteCall(hMainFuncSerializer, stringIndex3);
		HqBytecodeWriteLoadParam(hMainFuncSerializer, 8, 0);

		HqBytecodeWriteInitFunction(hMainFuncSerializer, 0, stringIndex13);
		HqBytecodeWriteCallValue(hMainFuncSerializer, 0);

		HqBytecodeWriteReturn(hMainFuncSerializer);

		const void* const pMainFuncData = HqSerializerGetRawStreamPointer(hMainFuncSerializer);
		const size_t mainFuncLength = HqSerializerGetStreamLength(hMainFuncSerializer);

		HqModuleWriterAddFunction(hModuleWriter, mainFuncSignature, pMainFuncData, mainFuncLength, 0, 0);
	}

	// int32 Program.DoWork(float64)
	{
		HqBytecodeWriteLoadImmF64(hSubFuncSerializer, 0, 1.2345);
		HqBytecodeWriteMove(hSubFuncSerializer, 1, 0);

		HqBytecodeWritePush(hSubFuncSerializer, 0);
		HqBytecodeWriteYield(hSubFuncSerializer);
		HqBytecodeWritePop(hSubFuncSerializer, 1);

		HqBytecodeWriteInitObject(hSubFuncSerializer, 2, stringIndex8);
		HqBytecodeWriteLoadImmI32(hSubFuncSerializer, 1, 5);
		HqBytecodeWriteStoreObject(hSubFuncSerializer, 2, 1, 0);

		HqBytecodeWriteMove(hSubFuncSerializer, 3, 2);
		HqBytecodeWriteCompare(hSubFuncSerializer, 3, 2, 3);

		HqBytecodeWriteCopy(hSubFuncSerializer, 3, 2);
		HqBytecodeWriteCompare(hSubFuncSerializer, 3, 2, 3);

		HqBytecodeWriteLoadImmStr(hSubFuncSerializer, 0, stringIndex0);
		HqBytecodeWriteLoadImmStr(hSubFuncSerializer, 1, stringIndex1);
		HqBytecodeWriteAdd(hSubFuncSerializer, 0, 0, 1);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeWriteLength(hSubFuncSerializer, 1, 0);
		HqBytecodeWriteCastString(hSubFuncSerializer, 1, 1);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 1);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeWriteLoadImmI32(hSubFuncSerializer, 0, 5);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 1, 0);

		const size_t loopOffsetStart = HqSerializerGetStreamPosition(hSubFuncSerializer);

		HqBytecodeWriteLoadObject(hSubFuncSerializer, 0, 2, 0);
		HqBytecodeWriteCastString(hSubFuncSerializer, 1, 0);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 1);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex10);
		HqBytecodeWriteLoadParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteStoreObject(hSubFuncSerializer, 2, 0, 0);

		HqBytecodeWriteLoadParam(hSubFuncSerializer, 0, 1);
		HqBytecodeWriteJumpIfTrue(hSubFuncSerializer, 0, 20);

		const size_t loopOffsetEnd = HqSerializerGetStreamPosition(hSubFuncSerializer);
		const int32_t loopRelativeOffset = int32_t(loopOffsetEnd - loopOffsetStart);

		HqBytecodeWriteJump(hSubFuncSerializer, -loopRelativeOffset);

		HqBytecodeWriteLoadObject(hSubFuncSerializer, 0, 2, 0);
		HqBytecodeWriteJumpIfFalse(hSubFuncSerializer, 0, 16);
		HqBytecodeWriteAbort(hSubFuncSerializer);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteReturn(hSubFuncSerializer);

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

			HqBytecodeWriteInitArray(hExcFuncSerializer, 0, 4);
			HqBytecodeWriteLoadImmStr(hExcFuncSerializer, 1, stringIndex14);
			HqBytecodeWriteLoadImmStr(hExcFuncSerializer, 2, stringIndex8);
			HqBytecodeWriteStoreArray(hExcFuncSerializer, 0, 1, 1);
			HqBytecodeWriteStoreArray(hExcFuncSerializer, 0, 2, 3);
			HqBytecodeWriteLoadImmNull(hExcFuncSerializer, 2);

			HqBytecodeWriteLength(hExcFuncSerializer, 1, 0);

			HqBytecodeWriteLoadArray(hExcFuncSerializer, 0, 0, 1);
			HqBytecodeWriteRaise(hExcFuncSerializer, 0);
			HqBytecodeWriteAbort(hExcFuncSerializer);

			tryBlockEnd = HqSerializerGetStreamPosition(hExcFuncSerializer);
		}

		HqBytecodeWriteNop(hExcFuncSerializer);
		HqBytecodeWriteNop(hExcFuncSerializer);
		HqBytecodeWriteNop(hExcFuncSerializer);

		// Write the exception handling section.
		{
			exceptionHandlerStart = HqSerializerGetStreamPosition(hExcFuncSerializer);

			HqBytecodeWriteCall(hExcFuncSerializer, stringIndex4);
			HqBytecodeWriteJump(hExcFuncSerializer, 8);
		}

		HqBytecodeWriteNop(hExcFuncSerializer);
		HqBytecodeWriteNop(hExcFuncSerializer);
		HqBytecodeWriteNop(hExcFuncSerializer);

		HqBytecodeWriteLoadImmStr(hExcFuncSerializer, 0, stringIndex15);
		HqBytecodeWriteStoreParam(hExcFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hExcFuncSerializer, stringIndex4);

		HqBytecodeWriteReturn(hExcFuncSerializer);

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

	HqReportHandle hReport = HQ_REPORT_HANDLE_NULL;
	HqCompilerGetReportHandle(hCompiler, &hReport);

	result = HqModuleWriterSerialize(hModuleWriter, hReport, hFileSerializer);
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
		OnMessageReported(NULL, HQ_MESSAGE_TYPE_WARNING, msg);
	}

	// Dispose of the compiler context.
	result = HqCompilerDispose(&hCompiler);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Harlequin compiler context: error=\"%s\"", HqGetErrorCodeString(result));
		OnMessageReported(NULL, HQ_MESSAGE_TYPE_WARNING, msg);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
