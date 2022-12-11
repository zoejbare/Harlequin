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

	HqProgramWriterHandle hProgramWriter = HQ_PROGRAM_WRITER_HANDLE_NULL;

	// Create the program writer.
	result = HqProgramWriterCreate(&hProgramWriter, hCompiler);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Harlequin program writer: error=\"%s\"", HqGetErrorCodeString(result));
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

	const int endianness = HQ_ENDIAN_ORDER_LITTLE;

	// Set the endianness of each serializer.
	HqSerializerSetEndianness(hFileSerializer, endianness);
	HqSerializerSetEndianness(hMainFuncSerializer, endianness);
	HqSerializerSetEndianness(hSubFuncSerializer, endianness);
	HqSerializerSetEndianness(hExcFuncSerializer, endianness);

	assert(HqSerializerGetEndianness(hFileSerializer) == HqSerializerGetEndianness(hSubFuncSerializer));
	assert(HqSerializerGetEndianness(hFileSerializer) == HqSerializerGetEndianness(hMainFuncSerializer));
	assert(HqSerializerGetEndianness(hFileSerializer) == HqSerializerGetEndianness(hExcFuncSerializer));

	const char* const mainFuncSignature = "void App.Program.Main()";
	const char* const subFuncSignature = "int32 App.Program.DoWork(float64)";
	const char* const excFuncSignature = "void App.Program.ExceptionHandlingTest()";
	const char* const nativePrintFuncSignature = "void App.Program.PrintString(string)";
	const char* const nativeDecrementFuncSignature = "(int32, bool) App.Program.Decrement(int32)";
	const char* const globalVariableName = "globalTestVar";
	const char* const localVariableName = "localTestVar";
	const char* const objectTypeName = "App.TestClass";
	const char* const objectMemberName = "memberTest";

	const char* const opAddStringSignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_ADD_STRING);
	const char* const opCastInt32ToStringSignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_CAST_INT32_TO_STRING);
	const char* const opCastInt64ToStringSignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_CAST_INT64_TO_STRING);
	const char* const opLenStringSignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_LEN_STRING);
	const char* const opLenArraySignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_LEN_ARRAY);

	uint32_t stringIndex0; HqProgramWriterAddString(hProgramWriter, "this is ", &stringIndex0);
	uint32_t stringIndex1; HqProgramWriterAddString(hProgramWriter, "a test string", &stringIndex1);
	uint32_t stringIndex2; HqProgramWriterAddString(hProgramWriter, globalVariableName, &stringIndex2);
	uint32_t stringIndex3; HqProgramWriterAddString(hProgramWriter, subFuncSignature, &stringIndex3);
	uint32_t stringIndex4; HqProgramWriterAddString(hProgramWriter, nativePrintFuncSignature, &stringIndex4);
	uint32_t stringIndex5; HqProgramWriterAddString(hProgramWriter, localVariableName, &stringIndex5);
	uint32_t stringIndex6; HqProgramWriterAddString(hProgramWriter, opAddStringSignature, &stringIndex6);
	uint32_t stringIndex7; HqProgramWriterAddString(hProgramWriter, opCastInt32ToStringSignature, &stringIndex7);
	uint32_t stringIndex8; HqProgramWriterAddString(hProgramWriter, objectTypeName, &stringIndex8);
	uint32_t stringIndex9; HqProgramWriterAddString(hProgramWriter, objectMemberName, &stringIndex9);
	uint32_t stringIndex10; HqProgramWriterAddString(hProgramWriter, nativeDecrementFuncSignature, &stringIndex10);
	uint32_t stringIndex11; HqProgramWriterAddString(hProgramWriter, opCastInt64ToStringSignature, &stringIndex11);
	uint32_t stringIndex12; HqProgramWriterAddString(hProgramWriter, opLenStringSignature, &stringIndex12);
	uint32_t stringIndex13; HqProgramWriterAddString(hProgramWriter, excFuncSignature, &stringIndex13);
	uint32_t stringIndex14; HqProgramWriterAddString(hProgramWriter, "This is an exception string", &stringIndex14);
	uint32_t stringIndex15; HqProgramWriterAddString(hProgramWriter, "The exception was handled successfully!", &stringIndex15);
	uint32_t stringIndex16; HqProgramWriterAddString(hProgramWriter, opLenArraySignature, &stringIndex16);

	// Add the program globals.
	HqProgramWriterAddGlobal(hProgramWriter, globalVariableName);

	// Write the program object types.
	HqProgramWriterAddObjectType(hProgramWriter, objectTypeName);
	HqProgramWriterAddObjectMember(hProgramWriter, objectTypeName, objectMemberName, HQ_VALUE_TYPE_INT32, nullptr);

	// Free the built-in operator function signatures.
	HqMemFree((void*)(opAddStringSignature));
	HqMemFree((void*)(opCastInt32ToStringSignature));
	HqMemFree((void*)(opCastInt64ToStringSignature));
	HqMemFree((void*)(opLenStringSignature));
	HqMemFree((void*)(opLenArraySignature));

	// void App.Program.Main()
	{
		HqBytecodeWriteLoadConstNull(hMainFuncSerializer, 0);
		HqBytecodeWriteLoadConstI8(hMainFuncSerializer, 1, 20);
		HqBytecodeWriteLoadConstI16(hMainFuncSerializer, 2, 31);
		HqBytecodeWriteLoadConstI32(hMainFuncSerializer, 3, 123);
		HqBytecodeWriteLoadConstI64(hMainFuncSerializer, 4, 53);
		HqBytecodeWriteLoadConstI8(hMainFuncSerializer, 5, 64);
		HqBytecodeWriteLoadConstI16(hMainFuncSerializer, 6, 75);
		HqBytecodeWriteLoadConstI32(hMainFuncSerializer, 1, 86);
		HqBytecodeWriteLoadConstI64(hMainFuncSerializer, 8, 97);
		HqBytecodeWriteLoadConstF32(hMainFuncSerializer, 9, 3.1419f);
		HqBytecodeWriteLoadConstF64(hMainFuncSerializer, 10, 1.2345);
		HqBytecodeWriteLoadConstF64(hMainFuncSerializer, 11, 2.3456);
		HqBytecodeWriteLoadConstStr(hMainFuncSerializer, 12, stringIndex0);
		HqBytecodeWriteLoadConstStr(hMainFuncSerializer, 13, stringIndex1);
		HqBytecodeWriteLoadConstStr(hMainFuncSerializer, 14, stringIndex2);
		HqBytecodeWriteNop(hMainFuncSerializer);

		HqBytecodeWriteLoadGlobal(hMainFuncSerializer, 15, stringIndex2);
		HqBytecodeWriteStoreGlobal(hMainFuncSerializer, stringIndex2, 13);

		HqBytecodeWriteStoreParam(hMainFuncSerializer, 0, 11);
		HqBytecodeWriteCall(hMainFuncSerializer, stringIndex3);
		HqBytecodeWritePullParam(hMainFuncSerializer, 8, 0);

		HqBytecodeWriteInitFunction(hMainFuncSerializer, 0, stringIndex13);
		HqBytecodeWriteCallValue(hMainFuncSerializer, 0);

		HqBytecodeWriteReturn(hMainFuncSerializer);

		const void* const pMainFuncData = HqSerializerGetRawStreamPointer(hMainFuncSerializer);
		const size_t mainFuncLength = HqSerializerGetStreamLength(hMainFuncSerializer);

		HqProgramWriterAddFunction(hProgramWriter, mainFuncSignature, pMainFuncData, mainFuncLength, 0, 0);
	}

	// int32 App.Program.DoWork(float64)
	{
		HqBytecodeWritePullParam(hSubFuncSerializer, 0, 0);

		HqBytecodeWriteLoadConstF64(hSubFuncSerializer, 0, 1.2345);
		HqBytecodeWriteStoreLocal(hSubFuncSerializer, stringIndex5, 0);
		HqBytecodeWriteLoadLocal(hSubFuncSerializer, 1, stringIndex5);

		HqBytecodeWritePush(hSubFuncSerializer, 0);
		HqBytecodeWriteYield(hSubFuncSerializer);
		HqBytecodeWritePop(hSubFuncSerializer, 1);

		HqBytecodeWriteInitObject(hSubFuncSerializer, 2, stringIndex8);
		HqBytecodeWriteLoadConstI32(hSubFuncSerializer, 1, 5);
		HqBytecodeWriteStoreObject(hSubFuncSerializer, 2, 1, 0);

		HqBytecodeWriteLoadConstStr(hSubFuncSerializer, 0, stringIndex0);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);

		HqBytecodeWriteLoadConstStr(hSubFuncSerializer, 1, stringIndex1);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 1, 1);

		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex6);
		HqBytecodeWriteLoadParam(hSubFuncSerializer, 3, 0);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 3);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex12);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex11);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeWriteLoadConstI32(hSubFuncSerializer, 0, 5);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 1, 0);

		const size_t loopOffsetStart = HqSerializerGetStreamPosition(hSubFuncSerializer);

		HqBytecodeWriteLoadObject(hSubFuncSerializer, 0, 2, 0);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex7);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex4);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hSubFuncSerializer, stringIndex10);
		HqBytecodeWritePullParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteStoreObject(hSubFuncSerializer, 2, 0, 0);

		HqBytecodeWritePullParam(hSubFuncSerializer, 0, 1);
		HqBytecodeWriteBranchIfTrue(hSubFuncSerializer, 0, 20);

		const size_t loopOffsetEnd = HqSerializerGetStreamPosition(hSubFuncSerializer);
		const int32_t loopRelativeOffset = int32_t(loopOffsetEnd - loopOffsetStart);

		HqBytecodeWriteBranch(hSubFuncSerializer, -loopRelativeOffset);

		HqBytecodeWriteLoadObject(hSubFuncSerializer, 0, 2, 0);
		HqBytecodeWriteBranchIfFalse(hSubFuncSerializer, 0, 16);
		HqBytecodeWriteAbort(hSubFuncSerializer);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteReturn(hSubFuncSerializer);

		const void* const pFuncData = HqSerializerGetRawStreamPointer(hSubFuncSerializer);
		const size_t funcLength = HqSerializerGetStreamLength(hSubFuncSerializer);

		HqProgramWriterAddFunction(hProgramWriter, subFuncSignature, pFuncData, funcLength, 1, 1);
		HqProgramWriterAddLocalVariable(hProgramWriter, subFuncSignature, localVariableName);
	}

	// void App.Program.PrintString(string)
	{
		HqProgramWriterAddNativeFunction(hProgramWriter, nativePrintFuncSignature, 1, 0);
	}

	// (int32, bool) App.Program.Decrement(int32)
	{
		HqProgramWriterAddNativeFunction(hProgramWriter, nativeDecrementFuncSignature, 1, 2);
	}

	// void App.Program.ExceptionHandlingTest()
	{
		size_t tryBlockStart;
		size_t tryBlockEnd;
		size_t exceptionHandlerStart;

		// Write the 'try' block.
		{
			tryBlockStart = HqSerializerGetStreamPosition(hExcFuncSerializer);

			HqBytecodeWriteInitArray(hExcFuncSerializer, 0, 4);
			HqBytecodeWriteLoadConstStr(hExcFuncSerializer, 1, stringIndex14);
			HqBytecodeWriteLoadConstStr(hExcFuncSerializer, 2, stringIndex8);
			HqBytecodeWriteStoreArray(hExcFuncSerializer, 0, 1, 1);
			HqBytecodeWriteStoreArray(hExcFuncSerializer, 0, 2, 3);
			HqBytecodeWriteLoadConstNull(hExcFuncSerializer, 1);
			HqBytecodeWriteLoadConstNull(hExcFuncSerializer, 2);

			HqBytecodeWriteStoreParam(hExcFuncSerializer, 0, 0);
			HqBytecodeWriteCall(hExcFuncSerializer, stringIndex16);

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
			HqBytecodeWriteBranch(hExcFuncSerializer, 8);
		}

		HqBytecodeWriteNop(hExcFuncSerializer);
		HqBytecodeWriteNop(hExcFuncSerializer);
		HqBytecodeWriteNop(hExcFuncSerializer);

		HqBytecodeWriteLoadConstStr(hExcFuncSerializer, 0, stringIndex15);
		HqBytecodeWriteStoreParam(hExcFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hExcFuncSerializer, stringIndex4);

		HqBytecodeWriteReturn(hExcFuncSerializer);

		const void* const pFuncData = HqSerializerGetRawStreamPointer(hExcFuncSerializer);
		const size_t funcLength = HqSerializerGetStreamLength(hExcFuncSerializer);

		HqProgramWriterAddFunction(hProgramWriter, excFuncSignature, pFuncData, funcLength, 0, 0);

		// Setup the exception handling data on the function.
		{
			uint32_t tryBlockId = 0;

			HqProgramWriterAddGuardedBlock(hProgramWriter, excFuncSignature, tryBlockStart, tryBlockEnd - tryBlockStart, &tryBlockId);
			HqProgramWriterAddExceptionHandler(hProgramWriter, excFuncSignature, tryBlockId, exceptionHandlerStart, HQ_VALUE_TYPE_STRING, nullptr);
		}
	}

	result = HqProgramWriterSerialize(hProgramWriter, hCompiler, hFileSerializer);
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

	// Dispose of the program writer.
	result = HqProgramWriterDispose(&hProgramWriter);
	if(result != HQ_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Harlequin program writer: error=\"%s\"", HqGetErrorCodeString(result));
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
