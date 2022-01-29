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

int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

#if defined(XENON_PLATFORM_WINDOWS)
	// This enables tracking of global heap allocations.  If any are leaked, they will show up in the
	// Visual Studio output window on application exit.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	XenonCompilerHandle hCompiler = XENON_COMPILER_HANDLE_NULL;
	XenonCompilerInit init;

	init.common.report.onMessageFn = OnMessageReported;
	init.common.report.pUserData = nullptr;
	init.common.report.reportLevel = XENON_MESSAGE_TYPE_VERBOSE;

	// Create the compiler context.
	int result = XenonCompilerCreate(&hCompiler, init);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Xenon compiler context: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_FATAL, msg);
		return 1;
	}

	XenonProgramWriterHandle hProgramWriter = XENON_PROGRAM_WRITER_HANDLE_NULL;

	// Create the program writer.
	result = XenonProgramWriterCreate(&hProgramWriter, hCompiler);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to create Xenon program writer: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_FATAL, msg);
		return 1;
	}

	XenonSerializerHandle hFileSerializer = XENON_SERIALIZER_HANDLE_NULL;
	XenonSerializerHandle hMainFuncSerializer = XENON_SERIALIZER_HANDLE_NULL;
	XenonSerializerHandle hSubFuncSerializer = XENON_SERIALIZER_HANDLE_NULL;
	XenonSerializerHandle hExcFuncSerializer = XENON_SERIALIZER_HANDLE_NULL;

	XenonSerializerCreate(&hFileSerializer, XENON_SERIALIZER_MODE_WRITER);
	XenonSerializerCreate(&hMainFuncSerializer, XENON_SERIALIZER_MODE_WRITER);
	XenonSerializerCreate(&hSubFuncSerializer, XENON_SERIALIZER_MODE_WRITER);
	XenonSerializerCreate(&hExcFuncSerializer, XENON_SERIALIZER_MODE_WRITER);

	const int endianness = XENON_ENDIAN_ORDER_LITTLE;

	// Set the endianness of each serializer.
	XenonSerializerSetEndianness(hFileSerializer, endianness);
	XenonSerializerSetEndianness(hMainFuncSerializer, endianness);
	XenonSerializerSetEndianness(hSubFuncSerializer, endianness);
	XenonSerializerSetEndianness(hExcFuncSerializer, endianness);

	const char* const mainFuncSignature = "void App.Program.Main()";
	const char* const subFuncSignature = "int32 App.Program.DoWork(float64)";
	const char* const excFuncSignature = "void App.Program.ExceptionHandlingTest()";
	const char* const nativePrintFuncSignature = "void App.Program.PrintString(string)";
	const char* const nativeDecrementFuncSignature = "(int32, bool) App.Program.Decrement(int32)";
	const char* const opAddStringSignature = XenonGetBuiltInFunctionSignature(XENON_BUILT_IN_OP_ADD_STRING);
	const char* const opCastInt32ToStringSignature = XenonGetBuiltInFunctionSignature(XENON_BUILT_IN_OP_CAST_INT32_TO_STRING);
	const char* const opCastInt64ToStringSignature = XenonGetBuiltInFunctionSignature(XENON_BUILT_IN_OP_CAST_INT64_TO_STRING);
	const char* const opLenStringSignature = XenonGetBuiltInFunctionSignature(XENON_BUILT_IN_OP_LEN_STRING);
	const char* const globalVariableName = "globalTestVar";
	const char* const localVariableName = "localTestVar";
	const char* const objectTypeName = "App.ClassType";
	const char* const objectMemberName = "memberTest";

	uint32_t constIndex0;
	XenonProgramWriterAddConstantNull(hProgramWriter, &constIndex0);

	uint32_t constIndex1;
	XenonProgramWriterAddConstantInt32(hProgramWriter, 123, &constIndex1);

	uint32_t constIndex2;
	XenonProgramWriterAddConstantFloat64(hProgramWriter, 1.2345, &constIndex2);

	uint32_t constIndex3;
	XenonProgramWriterAddConstantFloat64(hProgramWriter, 2.3456, &constIndex3);

	uint32_t constIndex4;
	XenonProgramWriterAddConstantString(hProgramWriter, "this is ", &constIndex4);

	uint32_t constIndex5;
	XenonProgramWriterAddConstantString(hProgramWriter, "a test string", &constIndex5);

	uint32_t constIndex6;
	XenonProgramWriterAddConstantString(hProgramWriter, globalVariableName, &constIndex6);

	uint32_t constIndex7;
	XenonProgramWriterAddConstantString(hProgramWriter, subFuncSignature, &constIndex7);

	uint32_t constIndex8;
	XenonProgramWriterAddConstantString(hProgramWriter, nativePrintFuncSignature, &constIndex8);

	uint32_t constIndex9;
	XenonProgramWriterAddConstantString(hProgramWriter, localVariableName, &constIndex9);

	uint32_t constIndex10;
	XenonProgramWriterAddConstantString(hProgramWriter, opAddStringSignature, &constIndex10);

	uint32_t constIndex11;
	XenonProgramWriterAddConstantString(hProgramWriter, opCastInt32ToStringSignature, &constIndex11);

	uint32_t constIndex12;
	XenonProgramWriterAddConstantString(hProgramWriter, objectTypeName, &constIndex12);

	uint32_t constIndex13;
	XenonProgramWriterAddConstantString(hProgramWriter, objectMemberName, &constIndex13);

	uint32_t constIndex14;
	XenonProgramWriterAddConstantString(hProgramWriter, nativeDecrementFuncSignature, &constIndex14);

	uint32_t constIndex15;
	XenonProgramWriterAddConstantInt32(hProgramWriter, 5, &constIndex15);

	uint32_t constIndex16;
	XenonProgramWriterAddConstantString(hProgramWriter, opCastInt64ToStringSignature, &constIndex16);

	uint32_t constIndex17;
	XenonProgramWriterAddConstantString(hProgramWriter, opLenStringSignature, &constIndex17);

	uint32_t constIndex18;
	XenonProgramWriterAddConstantString(hProgramWriter, excFuncSignature, &constIndex18);

	uint32_t constIndex19;
	XenonProgramWriterAddConstantString(hProgramWriter, "This is an exception string", &constIndex19);

	uint32_t constIndex20;
	XenonProgramWriterAddConstantString(hProgramWriter, "The exception was handled successfully!", &constIndex20);

	// Add the program globals.
	XenonProgramWriterAddGlobal(hProgramWriter, globalVariableName, constIndex4);

	// Write the program object types.
	XenonProgramWriterAddObjectType(hProgramWriter, objectTypeName);
	XenonProgramWriterAddObjectMember(hProgramWriter, objectTypeName, objectMemberName, XENON_VALUE_TYPE_INT32, nullptr);

	// void App.Program.Main()
	{
		XenonBytecodeWriteLoadConstant(hMainFuncSerializer, 0, constIndex0);
		XenonBytecodeWriteLoadConstant(hMainFuncSerializer, 1, constIndex1);
		XenonBytecodeWriteLoadConstant(hMainFuncSerializer, 2, constIndex2);
		XenonBytecodeWriteLoadConstant(hMainFuncSerializer, 3, constIndex3);
		XenonBytecodeWriteLoadConstant(hMainFuncSerializer, 4, constIndex4);
		XenonBytecodeWriteLoadConstant(hMainFuncSerializer, 5, constIndex5);
		XenonBytecodeWriteLoadConstant(hMainFuncSerializer, 6, constIndex6);
		XenonBytecodeWriteNop(hMainFuncSerializer);

		XenonBytecodeWriteLoadGlobal(hMainFuncSerializer, 7, constIndex6);
		XenonBytecodeWriteStoreGlobal(hMainFuncSerializer, constIndex6, 5);

		XenonBytecodeWriteStoreParam(hMainFuncSerializer, 0, 3);
		XenonBytecodeWriteCall(hMainFuncSerializer, constIndex7);
		XenonBytecodeWritePullParam(hMainFuncSerializer, 8, 0);

		XenonBytecodeWriteCall(hMainFuncSerializer, constIndex18);

		XenonBytecodeWriteReturn(hMainFuncSerializer);

		const void* const pMainFuncData = XenonSerializerGetRawStreamPointer(hMainFuncSerializer);
		const size_t mainFuncLength = XenonSerializerGetStreamLength(hMainFuncSerializer);

		XenonProgramWriterAddFunction(hProgramWriter, mainFuncSignature, pMainFuncData, mainFuncLength, 0, 0);
	}

	// int32 App.Program.DoWork(float64)
	{
		XenonBytecodeWritePullParam(hSubFuncSerializer, 0, 0);

		XenonBytecodeWriteLoadLocal(hSubFuncSerializer, 1, constIndex9);
		XenonBytecodeWriteStoreLocal(hSubFuncSerializer, constIndex9, 0);

		XenonBytecodeWritePush(hSubFuncSerializer, 0);
		XenonBytecodeWriteYield(hSubFuncSerializer);
		XenonBytecodeWritePop(hSubFuncSerializer, 1);

		XenonBytecodeWriteInitObject(hSubFuncSerializer, 2, constIndex12);
		XenonBytecodeWriteLoadConstant(hSubFuncSerializer, 1, constIndex15);
		XenonBytecodeWriteStoreObject(hSubFuncSerializer, 2, 1, 0);

		XenonBytecodeWriteLoadConstant(hSubFuncSerializer, 0, constIndex4);
		XenonBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);

		XenonBytecodeWriteLoadConstant(hSubFuncSerializer, 1, constIndex5);
		XenonBytecodeWriteStoreParam(hSubFuncSerializer, 1, 1);

		XenonBytecodeWriteCall(hSubFuncSerializer, constIndex10);
		XenonBytecodeWriteLoadParam(hSubFuncSerializer, 3, 0);
		XenonBytecodeWriteCall(hSubFuncSerializer, constIndex8);

		XenonBytecodeWriteStoreParam(hSubFuncSerializer, 0, 3);
		XenonBytecodeWriteCall(hSubFuncSerializer, constIndex17);
		XenonBytecodeWriteCall(hSubFuncSerializer, constIndex16);
		XenonBytecodeWriteCall(hSubFuncSerializer, constIndex8);

		XenonBytecodeWriteLoadConstant(hSubFuncSerializer, 0, constIndex15);
		XenonBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		XenonBytecodeWriteStoreParam(hSubFuncSerializer, 1, 0);

		const size_t loopOffsetStart = XenonSerializerGetStreamPosition(hSubFuncSerializer);

		XenonBytecodeWriteLoadObject(hSubFuncSerializer, 0, 2, 0);

		XenonBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		XenonBytecodeWriteCall(hSubFuncSerializer, constIndex11);
		XenonBytecodeWriteCall(hSubFuncSerializer, constIndex8);

		XenonBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		XenonBytecodeWriteCall(hSubFuncSerializer, constIndex14);
		XenonBytecodeWritePullParam(hSubFuncSerializer, 0, 0);
		XenonBytecodeWriteStoreObject(hSubFuncSerializer, 2, 0, 0);

		XenonBytecodeWritePullParam(hSubFuncSerializer, 0, 1);
		XenonBytecodeWriteBranchIfTrue(hSubFuncSerializer, 0, 14);

		const size_t loopOffsetEnd = XenonSerializerGetStreamPosition(hSubFuncSerializer);
		const int32_t loopRelativeOffset = int32_t(loopOffsetEnd - loopOffsetStart);

		XenonBytecodeWriteBranch(hSubFuncSerializer, -loopRelativeOffset);

		XenonBytecodeWriteLoadObject(hSubFuncSerializer, 0, 2, 0);
		XenonBytecodeWriteBranchIfFalse(hSubFuncSerializer, 0, 10);
		XenonBytecodeWriteAbort(hSubFuncSerializer);

		XenonBytecodeWriteStoreParam(hSubFuncSerializer, 0, 0);
		XenonBytecodeWriteReturn(hSubFuncSerializer);

		const void* const pFuncData = XenonSerializerGetRawStreamPointer(hSubFuncSerializer);
		const size_t funcLength = XenonSerializerGetStreamLength(hSubFuncSerializer);

		XenonProgramWriterAddFunction(hProgramWriter, subFuncSignature, pFuncData, funcLength, 1, 1);
		XenonProgramWriterAddLocalVariable(hProgramWriter, subFuncSignature, localVariableName, constIndex2);
	}

	// void App.Program.PrintString(string)
	{
		XenonProgramWriterAddNativeFunction(hProgramWriter, nativePrintFuncSignature, 1, 0);
	}

	// (int32, bool) App.Program.Decrement(int32)
	{
		XenonProgramWriterAddNativeFunction(hProgramWriter, nativeDecrementFuncSignature, 1, 2);
	}

	// void App.Program.ExceptionHandlingTest()
	{
		size_t tryBlockStart;
		size_t tryBlockEnd;
		size_t exceptionHandlerStart;

		// Write the 'try' block.
		{
			tryBlockStart = XenonSerializerGetStreamPosition(hExcFuncSerializer);

			XenonBytecodeWriteInitArray(hExcFuncSerializer, 1, 4);
			XenonBytecodeWriteLoadConstant(hExcFuncSerializer, 0, constIndex19);
			XenonBytecodeWriteRaise(hExcFuncSerializer, 0);
			XenonBytecodeWriteAbort(hExcFuncSerializer);

			tryBlockEnd = XenonSerializerGetStreamPosition(hExcFuncSerializer);
		}

		XenonBytecodeWriteNop(hExcFuncSerializer);
		XenonBytecodeWriteNop(hExcFuncSerializer);
		XenonBytecodeWriteNop(hExcFuncSerializer);

		// Write the exception handling section.
		{
			exceptionHandlerStart = XenonSerializerGetStreamPosition(hExcFuncSerializer);

			XenonBytecodeWriteCall(hExcFuncSerializer, constIndex8);
			XenonBytecodeWriteBranch(hExcFuncSerializer, 8);
		}

		XenonBytecodeWriteNop(hExcFuncSerializer);
		XenonBytecodeWriteNop(hExcFuncSerializer);
		XenonBytecodeWriteNop(hExcFuncSerializer);

		XenonBytecodeWriteLoadConstant(hExcFuncSerializer, 0, constIndex20);
		XenonBytecodeWriteStoreParam(hExcFuncSerializer, 0, 0);
		XenonBytecodeWriteCall(hExcFuncSerializer, constIndex8);

		XenonBytecodeWriteReturn(hExcFuncSerializer);

		const void* const pFuncData = XenonSerializerGetRawStreamPointer(hExcFuncSerializer);
		const size_t funcLength = XenonSerializerGetStreamLength(hExcFuncSerializer);

		XenonProgramWriterAddFunction(hProgramWriter, excFuncSignature, pFuncData, funcLength, 0, 0);

		// Setup the exception handling data on the function.
		{
			uint32_t tryBlockId = 0;

			XenonProgramWriterAddGuardedBlock(hProgramWriter, excFuncSignature, tryBlockStart, tryBlockEnd - tryBlockStart, &tryBlockId);
			XenonProgramWriterAddExceptionHandler(hProgramWriter, excFuncSignature, tryBlockId, exceptionHandlerStart, XENON_VALUE_TYPE_STRING, nullptr);
		}
	}

	result = XenonProgramWriterSerialize(hProgramWriter, hCompiler, hFileSerializer);
	if(result == XENON_SUCCESS)
	{
		const void* const pFileData = XenonSerializerGetRawStreamPointer(hFileSerializer);
		const size_t fileLength = XenonSerializerGetStreamLength(hFileSerializer);

		const char* const outputFilename = "test.xc";

		char msg[128];
		snprintf(msg, sizeof(msg), "Writing file: %s", outputFilename);
		OnMessageReported(nullptr, XENON_MESSAGE_TYPE_INFO, msg);

		FILE* const pOutputFile = fopen(outputFilename, "wb");
		fwrite(pFileData, fileLength, 1, pOutputFile);
		fclose(pOutputFile);
	}

	XenonSerializerDispose(&hMainFuncSerializer);
	XenonSerializerDispose(&hSubFuncSerializer);
	XenonSerializerDispose(&hExcFuncSerializer);
	XenonSerializerDispose(&hFileSerializer);

	// Dispose of the program writer.
	result = XenonProgramWriterDispose(&hProgramWriter);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Xenon program writer: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_WARNING, msg);
	}

	// Dispose of the compiler context.
	result = XenonCompilerDispose(&hCompiler);
	if(result != XENON_SUCCESS)
	{
		char msg[128];
		snprintf(msg, sizeof(msg) - 1, "Failed to dispose of Xenon compiler context: error=\"%s\"", XenonGetErrorCodeString(result));
		OnMessageReported(NULL, XENON_MESSAGE_TYPE_WARNING, msg);
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
