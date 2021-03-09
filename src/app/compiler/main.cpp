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
#include "XenonOpCode.h"

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
	XenonSerializerHandle hBytecodeSerializer = XENON_SERIALIZER_HANDLE_NULL;

	XenonSerializerCreate(&hFileSerializer, XENON_SERIALIZER_MODE_WRITER);
	XenonSerializerCreate(&hBytecodeSerializer, XENON_SERIALIZER_MODE_WRITER);

	XenonValueHandle hValueA = XenonValueCreateFloat64(1.2345);
	XenonValueHandle hValueB = XenonValueCreateFloat64(2.3456);
	XenonValueHandle hValueC = XenonValueCreateString("this is ");
	XenonValueHandle hValueD = XenonValueCreateString("a test string");
	XenonValueHandle hValueE = XenonValueCreateString("globalTestVar");

	uint32_t constIndexNull;
	XenonProgramWriterAddConstant(hProgramWriter, XenonValueCreateNull(), &constIndexNull);

	uint32_t constIndex0;
	XenonProgramWriterAddConstant(hProgramWriter, hValueA, &constIndex0);

	uint32_t constIndex1;
	XenonProgramWriterAddConstant(hProgramWriter, hValueB, &constIndex1);

	uint32_t constIndex2;
	XenonProgramWriterAddConstant(hProgramWriter, hValueC, &constIndex2);

	uint32_t constIndex3;
	XenonProgramWriterAddConstant(hProgramWriter, hValueD, &constIndex3);

	uint32_t constIndex4;
	XenonProgramWriterAddConstant(hProgramWriter, hValueE, &constIndex4);

	XenonProgramWriterAddGlobal(hProgramWriter, XenonValueGetString(hValueE), constIndex2);

	// hValueA
	{
		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_LOAD_CONSTANT);
		XenonSerializerWriteUint32(hBytecodeSerializer, 0);
		XenonSerializerWriteUint32(hBytecodeSerializer, constIndex0);

		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_DEBUG_GP_REG);
		XenonSerializerWriteUint32(hBytecodeSerializer, 0);
	}

	// hValueB
	{
		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_LOAD_CONSTANT);
		XenonSerializerWriteUint32(hBytecodeSerializer, 1);
		XenonSerializerWriteUint32(hBytecodeSerializer, constIndex1);

		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_DEBUG_GP_REG);
		XenonSerializerWriteUint32(hBytecodeSerializer, 1);
	}

	// hValueC
	{
		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_LOAD_CONSTANT);
		XenonSerializerWriteUint32(hBytecodeSerializer, 0);
		XenonSerializerWriteUint32(hBytecodeSerializer, constIndex2);

		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_DEBUG_GP_REG);
		XenonSerializerWriteUint32(hBytecodeSerializer, 0);
	}

	// hValueD
	{
		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_LOAD_CONSTANT);
		XenonSerializerWriteUint32(hBytecodeSerializer, 1);
		XenonSerializerWriteUint32(hBytecodeSerializer, constIndex3);

		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_DEBUG_GP_REG);
		XenonSerializerWriteUint32(hBytecodeSerializer, 1);
	}

	// globalTestVar
	{
		XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_LOAD_GLOBAL);
		XenonSerializerWriteUint32(hBytecodeSerializer, 2);
		XenonSerializerWriteUint32(hBytecodeSerializer, constIndexNull);
		XenonSerializerWriteUint32(hBytecodeSerializer, constIndex4);
	}

	XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_PUSH);
	XenonSerializerWriteUint32(hBytecodeSerializer, 0);

	XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_POP);
	XenonSerializerWriteUint32(hBytecodeSerializer, 3);

	XenonSerializerWriteUint8(hBytecodeSerializer, XENON_OP_CODE_RETURN);

	const void* const pBytecodeData = XenonSerializerGetRawStreamPointer(hBytecodeSerializer);
	const size_t bytecodeLength = XenonSerializerGetStreamLength(hBytecodeSerializer);

	XenonProgramWriterAddFunction(hProgramWriter, "void main()", pBytecodeData, bytecodeLength, 0, 0);
	XenonProgramWriterSerialize(hProgramWriter, hCompiler, hFileSerializer, XENON_ENDIAN_MODE_NATIVE);

	const void* const pFileData = XenonSerializerGetRawStreamPointer(hFileSerializer);
	const size_t fileLength = XenonSerializerGetStreamLength(hFileSerializer);

	FILE* const pOutputFile = fopen("test.xc", "wb");
	fwrite(pFileData, fileLength, 1, pOutputFile);
	fclose(pOutputFile);

	XenonSerializerDispose(&hBytecodeSerializer);
	XenonSerializerDispose(&hFileSerializer);

	XenonValueDispose(hValueA);
	XenonValueDispose(hValueB);
	XenonValueDispose(hValueC);
	XenonValueDispose(hValueD);

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
