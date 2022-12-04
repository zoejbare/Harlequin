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

	const char* const mainFuncSignature = "void App.Program.Main()";
	const char* const subFuncSignature = "int32 App.Program.DoWork(float64)";
	const char* const excFuncSignature = "void App.Program.ExceptionHandlingTest()";
	const char* const nativePrintFuncSignature = "void App.Program.PrintString(string)";
	const char* const nativeDecrementFuncSignature = "(int32, bool) App.Program.Decrement(int32)";
	const char* const globalVariableName = "globalTestVar";
	const char* const localVariableName = "localTestVar";
	const char* const objectTypeName = "App.ClassType";
	const char* const objectMemberName = "memberTest";

	const char* const opAddStringSignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_ADD_STRING);
	const char* const opCastInt32ToStringSignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_CAST_INT32_TO_STRING);
	const char* const opCastInt64ToStringSignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_CAST_INT64_TO_STRING);
	const char* const opLenStringSignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_LEN_STRING);
	const char* const opLenArraySignature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_OP_LEN_ARRAY);

	uint32_t constIndex0; HqProgramWriterAddConstantNull(hProgramWriter, &constIndex0);
	uint32_t constIndex1; HqProgramWriterAddConstantInt32(hProgramWriter, 123, &constIndex1);
	uint32_t constIndex2; HqProgramWriterAddConstantFloat64(hProgramWriter, 1.2345, &constIndex2);
	uint32_t constIndex3; HqProgramWriterAddConstantFloat64(hProgramWriter, 2.3456, &constIndex3);
	uint32_t constIndex4; HqProgramWriterAddConstantString(hProgramWriter, "this is ", &constIndex4);
	uint32_t constIndex5; HqProgramWriterAddConstantString(hProgramWriter, "a test string", &constIndex5);
	uint32_t constIndex6; HqProgramWriterAddConstantString(hProgramWriter, globalVariableName, &constIndex6);
	uint32_t constIndex7; HqProgramWriterAddConstantString(hProgramWriter, subFuncSignature, &constIndex7);
	uint32_t constIndex8; HqProgramWriterAddConstantString(hProgramWriter, nativePrintFuncSignature, &constIndex8);
	uint32_t constIndex9; HqProgramWriterAddConstantString(hProgramWriter, localVariableName, &constIndex9);
	uint32_t constIndex10; HqProgramWriterAddConstantString(hProgramWriter, opAddStringSignature, &constIndex10);
	uint32_t constIndex11; HqProgramWriterAddConstantString(hProgramWriter, opCastInt32ToStringSignature, &constIndex11);
	uint32_t constIndex12; HqProgramWriterAddConstantString(hProgramWriter, objectTypeName, &constIndex12);
	uint32_t constIndex13; HqProgramWriterAddConstantString(hProgramWriter, objectMemberName, &constIndex13);
	uint32_t constIndex14; HqProgramWriterAddConstantString(hProgramWriter, nativeDecrementFuncSignature, &constIndex14);
	uint32_t constIndex15; HqProgramWriterAddConstantInt32(hProgramWriter, 5, &constIndex15);
	uint32_t constIndex16; HqProgramWriterAddConstantString(hProgramWriter, opCastInt64ToStringSignature, &constIndex16);
	uint32_t constIndex17; HqProgramWriterAddConstantString(hProgramWriter, opLenStringSignature, &constIndex17);
	uint32_t constIndex18; HqProgramWriterAddConstantString(hProgramWriter, excFuncSignature, &constIndex18);
	uint32_t constIndex19; HqProgramWriterAddConstantString(hProgramWriter, "This is an exception string", &constIndex19);
	uint32_t constIndex20; HqProgramWriterAddConstantString(hProgramWriter, "The exception was handled successfully!", &constIndex20);
	uint32_t constIndex21; HqProgramWriterAddConstantString(hProgramWriter, opLenArraySignature, &constIndex21);

	// Add the program globals.
	HqProgramWriterAddGlobal(hProgramWriter, globalVariableName, constIndex4);

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
		HqBytecodeWriteLoadConstant(hMainFuncSerializer, 0, constIndex0);
		HqBytecodeWriteLoadConstant(hMainFuncSerializer, 1, constIndex1);
		HqBytecodeWriteLoadConstant(hMainFuncSerializer, 2, constIndex2);
		HqBytecodeWriteLoadConstant(hMainFuncSerializer, 3, constIndex3);
		HqBytecodeWriteLoadConstant(hMainFuncSerializer, 4, constIndex4);
		HqBytecodeWriteLoadConstant(hMainFuncSerializer, 5, constIndex5);
		HqBytecodeWriteLoadConstant(hMainFuncSerializer, 6, constIndex6);
		HqBytecodeWriteNop(hMainFuncSerializer);

		HqBytecodeWriteLoadGlobal(hMainFuncSerializer, 7, constIndex6);
		HqBytecodeWriteStoreGlobal(hMainFuncSerializer, constIndex6, 5);

		HqBytecodeWriteStoreParam(hMainFuncSerializer, 0, 3);
		HqBytecodeWriteCall(hMainFuncSerializer, constIndex7);
		HqBytecodeWritePullParam(hMainFuncSerializer, 8, 0);

		HqBytecodeWriteInitFunction(hMainFuncSerializer, 0, constIndex18);
		HqBytecodeWriteCallValue(hMainFuncSerializer, 0);

		HqBytecodeWriteReturn(hMainFuncSerializer);

		const void* const pMainFuncData = HqSerializerGetRawStreamPointer(hMainFuncSerializer);
		const size_t mainFuncLength = HqSerializerGetStreamLength(hMainFuncSerializer);

		HqProgramWriterAddFunction(hProgramWriter, mainFuncSignature, pMainFuncData, mainFuncLength, 0, 0);
	}

	// int32 App.Program.DoWork(float64)
	{
		HqBytecodeWritePullParam(hSubFuncSerializer, 0, 0);

		HqBytecodeWriteLoadLocal(hSubFuncSerializer, 1, constIndex9);
		HqBytecodeWriteStoreLocal(hSubFuncSerializer, constIndex9, 0);

		HqBytecodeWritePush(hSubFuncSerializer, 0);
		HqBytecodeWriteYield(hSubFuncSerializer);
		HqBytecodeWritePop(hSubFuncSerializer, 1);

		HqBytecodeWriteInitObject(hSubFuncSerializer, 2, constIndex12);
		HqBytecodeWriteLoadConstant(hSubFuncSerializer, 1, constIndex15);
		HqBytecodeWriteStoreObject(hSubFuncSerializer, 2, 1, 0);

		HqBytecodeWriteLoadConstant(hSubFuncSerializer, 0, constIndex4);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);

		HqBytecodeWriteLoadConstant(hSubFuncSerializer, 1, constIndex5);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 1, 1);

		HqBytecodeWriteCall(hSubFuncSerializer, constIndex10);
		HqBytecodeWriteLoadParam(hSubFuncSerializer, 3, 0);
		HqBytecodeWriteCall(hSubFuncSerializer, constIndex8);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 3);
		HqBytecodeWriteCall(hSubFuncSerializer, constIndex17);
		HqBytecodeWriteCall(hSubFuncSerializer, constIndex16);
		HqBytecodeWriteCall(hSubFuncSerializer, constIndex8);

		HqBytecodeWriteLoadConstant(hSubFuncSerializer, 0, constIndex15);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteStoreParam(hSubFuncSerializer, 1, 0);

		const size_t loopOffsetStart = HqSerializerGetStreamPosition(hSubFuncSerializer);

		HqBytecodeWriteLoadObject(hSubFuncSerializer, 0, 2, 0);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hSubFuncSerializer, constIndex11);
		HqBytecodeWriteCall(hSubFuncSerializer, constIndex8);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hSubFuncSerializer, constIndex14);
		HqBytecodeWritePullParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteStoreObject(hSubFuncSerializer, 2, 0, 0);

		HqBytecodeWritePullParam(hSubFuncSerializer, 0, 1);
		HqBytecodeWriteBranchIfTrue(hSubFuncSerializer, 0, 14);

		const size_t loopOffsetEnd = HqSerializerGetStreamPosition(hSubFuncSerializer);
		const int32_t loopRelativeOffset = int32_t(loopOffsetEnd - loopOffsetStart);

		HqBytecodeWriteBranch(hSubFuncSerializer, -loopRelativeOffset);

		HqBytecodeWriteLoadObject(hSubFuncSerializer, 0, 2, 0);
		HqBytecodeWriteBranchIfFalse(hSubFuncSerializer, 0, 10);
		HqBytecodeWriteAbort(hSubFuncSerializer);

		HqBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		HqBytecodeWriteReturn(hSubFuncSerializer);

		const void* const pFuncData = HqSerializerGetRawStreamPointer(hSubFuncSerializer);
		const size_t funcLength = HqSerializerGetStreamLength(hSubFuncSerializer);

		HqProgramWriterAddFunction(hProgramWriter, subFuncSignature, pFuncData, funcLength, 1, 1);
		HqProgramWriterAddLocalVariable(hProgramWriter, subFuncSignature, localVariableName, constIndex2);
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
			HqBytecodeWriteLoadConstant(hExcFuncSerializer, 1, constIndex19);
			HqBytecodeWriteLoadConstant(hExcFuncSerializer, 2, constIndex12);
			HqBytecodeWriteStoreArray(hExcFuncSerializer, 0, 1, 1);
			HqBytecodeWriteStoreArray(hExcFuncSerializer, 0, 2, 3);
			HqBytecodeWriteLoadConstant(hExcFuncSerializer, 1, constIndex0);
			HqBytecodeWriteLoadConstant(hExcFuncSerializer, 2, constIndex0);

			HqBytecodeWriteStoreParam(hExcFuncSerializer, 0, 0);
			HqBytecodeWriteCall(hExcFuncSerializer, constIndex21);

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

			HqBytecodeWriteCall(hExcFuncSerializer, constIndex8);
			HqBytecodeWriteBranch(hExcFuncSerializer, 8);
		}

		HqBytecodeWriteNop(hExcFuncSerializer);
		HqBytecodeWriteNop(hExcFuncSerializer);
		HqBytecodeWriteNop(hExcFuncSerializer);

		HqBytecodeWriteLoadConstant(hExcFuncSerializer, 0, constIndex20);
		HqBytecodeWriteStoreParam(hExcFuncSerializer, 0, 0);
		HqBytecodeWriteCall(hExcFuncSerializer, constIndex8);

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
