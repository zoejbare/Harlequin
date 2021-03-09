/**
 * Copyright (c) 2021, Zoe J. Bare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#ifndef _XENON_SCRIPT_H_
#define _XENON_SCRIPT_H_

/*---------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>

#include <sys/types.h>

#include "XenonBuildEnv.h"

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#define XENON_VM_FRAME_STACK_SIZE 1024
#define XENON_VM_VALUE_STACK_SIZE 128

#define XENON_VM_IO_REGISTER_COUNT 64
#define XENON_VM_GP_REGISTER_COUNT 64

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonErrorCode
{
	XENON_SUCCESS = 0,

	XENON_ERROR_INVALID_ARG = -1,
	XENON_ERROR_INVALID_TYPE = -2,
	XENON_ERROR_BAD_ALLOCATION = -3,
	XENON_ERROR_KEY_ALREADY_EXISTS = -4,
	XENON_ERROR_KEY_DOES_NOT_EXIST = -5,
	XENON_ERROR_FAILED_TO_OPEN_FILE = -6,
	XENON_ERROR_STREAM_END = -7,
	XENON_ERROR_STACK_EMPTY = -8,
	XENON_ERROR_STACK_FULL = -9,
	XENON_ERROR_INDEX_OUT_OF_RANGE = -10,
	XENON_ERROR_SCRIPT_NO_FUNCTION = -11,
};

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonEndianMode
{
	XENON_ENDIAN_MODE_UNKNOWN,
	XENON_ENDIAN_MODE_NATIVE,
	XENON_ENDIAN_MODE_LITTLE,
	XENON_ENDIAN_MODE_BIG,
};

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_BASE_API const char* XenonGetErrorCodeString(int errorCode);

XENON_BASE_API const char* XenonGetEndianModeString(int endianMode);

XENON_BASE_API int XenonGetPlatformEndianMode();

/*---------------------------------------------------------------------------------------------------------------------*/

typedef void* (*XenonCallbackMemAlloc)(size_t);
typedef void* (*XenonCallbackMemRealloc)(void*, size_t);
typedef void  (*XenonCallbackMemFree)(void*);

typedef struct
{
	XenonCallbackMemAlloc allocFn;
	XenonCallbackMemRealloc reallocFn;
	XenonCallbackMemFree freeFn;
} XenonMemAllocator;

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonMessageType
{
	XENON_MESSAGE_TYPE_VERBOSE,
	XENON_MESSAGE_TYPE_INFO,
	XENON_MESSAGE_TYPE_WARNING,
	XENON_MESSAGE_TYPE_ERROR,
	XENON_MESSAGE_TYPE_FATAL,
};

typedef void (*XenonMessageCallback)(void*, int, const char*);

typedef struct XenonReport* XenonReportHandle;

#define XENON_REPORT_HANDLE_NULL ((XenonReportHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

typedef struct
{
	XenonMessageCallback onMessageFn;
	void* pUserData;
	int reportLevel;
} XenonReportInit;

typedef struct
{
	XenonReportInit report;
} XenonCommonInit;

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_BASE_API XenonMemAllocator XenonMemGetDefaultAllocator();

XENON_BASE_API XenonMemAllocator XenonMemGetAllocator();

XENON_BASE_API int XenonMemSetAllocator(XenonMemAllocator allocator);

XENON_BASE_API void* XenonMemAlloc(size_t size);

XENON_BASE_API void* XenonMemRealloc(void* pMem, size_t size);

XENON_BASE_API int XenonMemFree(void* pMem);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_BASE_API int XenonReportMessage(XenonReportHandle hReport, int messageType, const char* fmt, ...);

XENON_BASE_API int XenonReportSetLevel(XenonReportHandle hReport, int messageType);

XENON_BASE_API int XenonReportGetLevel(XenonReportHandle hReport);

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonValueType
{
	XENON_VALUE_TYPE_BOOL,

	XENON_VALUE_TYPE_INT8,
	XENON_VALUE_TYPE_INT16,
	XENON_VALUE_TYPE_INT32,
	XENON_VALUE_TYPE_INT64,

	XENON_VALUE_TYPE_UINT8,
	XENON_VALUE_TYPE_UINT16,
	XENON_VALUE_TYPE_UINT32,
	XENON_VALUE_TYPE_UINT64,

	XENON_VALUE_TYPE_FLOAT32,
	XENON_VALUE_TYPE_FLOAT64,

	XENON_VALUE_TYPE_NULL,
	XENON_VALUE_TYPE_STRING,
	XENON_VALUE_TYPE_OBJECT,

	XENON_VALUE_TYPE__MAX_VALUE = XENON_VALUE_TYPE_OBJECT,
};

typedef struct XenonValue* XenonValueHandle;

#define XENON_VALUE_HANDLE_NULL ((XenonValueHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_BASE_API XenonValueHandle XenonValueCreateBool(bool value);

XENON_BASE_API XenonValueHandle XenonValueCreateInt8(int8_t value);

XENON_BASE_API XenonValueHandle XenonValueCreateInt16(int16_t value);

XENON_BASE_API XenonValueHandle XenonValueCreateInt32(int32_t value);

XENON_BASE_API XenonValueHandle XenonValueCreateInt64(int64_t value);

XENON_BASE_API XenonValueHandle XenonValueCreateUint8(uint8_t value);

XENON_BASE_API XenonValueHandle XenonValueCreateUint16(uint16_t value);

XENON_BASE_API XenonValueHandle XenonValueCreateUint32(uint32_t value);

XENON_BASE_API XenonValueHandle XenonValueCreateUint64(uint64_t value);

XENON_BASE_API XenonValueHandle XenonValueCreateFloat32(float value);

XENON_BASE_API XenonValueHandle XenonValueCreateFloat64(double value);

XENON_BASE_API XenonValueHandle XenonValueCreateNull();

XENON_BASE_API XenonValueHandle XenonValueCreateString(const char* const string);

XENON_BASE_API XenonValueHandle XenonValueCreateObject(XenonValueHandle hObjectProfile);

XENON_BASE_API XenonValueHandle XenonValueReference(XenonValueHandle hValue);

XENON_BASE_API XenonValueHandle XenonValueCopy(XenonValueHandle hValue);

XENON_BASE_API int XenonValueDispose(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsPrimitiveType(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsBool(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsInt8(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsInt16(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsInt32(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsInt64(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsUint8(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsUint16(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsUint32(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsUint64(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsFloat32(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsFloat64(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsNull(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsString(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueIsObject(XenonValueHandle hValue);

XENON_BASE_API bool XenonValueGetBool(XenonValueHandle hValue);

XENON_BASE_API int8_t XenonValueGetInt8(XenonValueHandle hValue);

XENON_BASE_API int16_t XenonValueGetInt16(XenonValueHandle hValue);

XENON_BASE_API int32_t XenonValueGetInt32(XenonValueHandle hValue);

XENON_BASE_API int64_t XenonValueGetInt64(XenonValueHandle hValue);

XENON_BASE_API uint8_t XenonValueGetUint8(XenonValueHandle hValue);

XENON_BASE_API uint16_t XenonValueGetUint16(XenonValueHandle hValue);

XENON_BASE_API uint32_t XenonValueGetUint32(XenonValueHandle hValue);

XENON_BASE_API uint64_t XenonValueGetUint64(XenonValueHandle hValue);

XENON_BASE_API float XenonValueGetFloat32(XenonValueHandle hValue);

XENON_BASE_API double XenonValueGetFloat64(XenonValueHandle hValue);

XENON_BASE_API const char* XenonValueGetString(XenonValueHandle hValue);

XENON_BASE_API size_t XenonValueGetStringLength(XenonValueHandle hValue);

XENON_BASE_API size_t XenonValueGetStringHash(XenonValueHandle hValue);

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonSerializerMode
{
	XENON_SERIALIZER_MODE_UNKNOWN,
	XENON_SERIALIZER_MODE_READER,
	XENON_SERIALIZER_MODE_WRITER,
};

typedef struct XenonSerializer* XenonSerializerHandle;

#define XENON_SERIALIZER_HANDLE_NULL ((XenonSerializerHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_BASE_API int XenonSerializerCreate(XenonSerializerHandle* phOutSerializer, int mode);

XENON_BASE_API int XenonSerializerDispose(XenonSerializerHandle* phSerializer);

XENON_BASE_API int XenonSerializerGetMode(XenonSerializerHandle hSerializer);

XENON_BASE_API int XenonSerializerGetEndianness(XenonSerializerHandle hSerializer);

XENON_BASE_API size_t XenonSerializerGetStreamLength(XenonSerializerHandle hSerializer);

XENON_BASE_API size_t XenonSerializerGetStreamPosition(XenonSerializerHandle hSerializer);

XENON_BASE_API void* XenonSerializerGetRawStreamPointer(XenonSerializerHandle hSerializer);

XENON_BASE_API int XenonSerializerSetEndianness(XenonSerializerHandle hSerializer, int endianness);

XENON_BASE_API int XenonSerializerSetStreamPosition(XenonSerializerHandle hSerializer, size_t position);

XENON_BASE_API int XenonSerializerLoadStreamFromFile(XenonSerializerHandle hSerializer, const char* filePath);

XENON_BASE_API int XenonSerializerLoadStreamFromBuffer(XenonSerializerHandle hSerializer, const void* pBuffer, size_t bufferLength);

XENON_BASE_API int XenonSerializerSaveStreamToFile(XenonSerializerHandle hSerializer, const char* filePath, bool append);

XENON_BASE_API int XenonSerializerSaveStreamToBuffer(XenonSerializerHandle hSerializer, void* pBuffer, size_t* pBufferLength);

XENON_BASE_API int XenonSerializerWriteBool(XenonSerializerHandle hSerializer, bool value);

XENON_BASE_API int XenonSerializerWriteInt8(XenonSerializerHandle hSerializer, int8_t value);

XENON_BASE_API int XenonSerializerWriteInt16(XenonSerializerHandle hSerializer, int16_t value);

XENON_BASE_API int XenonSerializerWriteInt32(XenonSerializerHandle hSerializer, int32_t value);

XENON_BASE_API int XenonSerializerWriteInt64(XenonSerializerHandle hSerializer, int64_t value);

XENON_BASE_API int XenonSerializerWriteUint8(XenonSerializerHandle hSerializer, uint8_t value);

XENON_BASE_API int XenonSerializerWriteUint16(XenonSerializerHandle hSerializer, uint16_t value);

XENON_BASE_API int XenonSerializerWriteUint32(XenonSerializerHandle hSerializer, uint32_t value);

XENON_BASE_API int XenonSerializerWriteUint64(XenonSerializerHandle hSerializer, uint64_t value);

XENON_BASE_API int XenonSerializerWriteFloat32(XenonSerializerHandle hSerializer, float value);

XENON_BASE_API int XenonSerializerWriteFloat64(XenonSerializerHandle hSerializer, double value);

XENON_BASE_API int XenonSerializerWriteBuffer(XenonSerializerHandle hSerializer, size_t bufferLength, const void* pBuffer);

XENON_BASE_API int XenonSerializerReadBool(XenonSerializerHandle hSerializer, bool* pOutValue);

XENON_BASE_API int XenonSerializerReadInt8(XenonSerializerHandle hSerializer, int8_t* pOutValue);

XENON_BASE_API int XenonSerializerReadInt16(XenonSerializerHandle hSerializer, int16_t* pOutValue);

XENON_BASE_API int XenonSerializerReadInt32(XenonSerializerHandle hSerializer, int32_t* pOutValue);

XENON_BASE_API int XenonSerializerReadInt64(XenonSerializerHandle hSerializer, int64_t* pOutValue);

XENON_BASE_API int XenonSerializerReadUint8(XenonSerializerHandle hSerializer, uint8_t* pOutValue);

XENON_BASE_API int XenonSerializerReadUint16(XenonSerializerHandle hSerializer, uint16_t* pOutValue);

XENON_BASE_API int XenonSerializerReadUint32(XenonSerializerHandle hSerializer, uint32_t* pOutValue);

XENON_BASE_API int XenonSerializerReadUint64(XenonSerializerHandle hSerializer, uint64_t* pOutValue);

XENON_BASE_API int XenonSerializerReadFloat32(XenonSerializerHandle hSerializer, float* pOutValue);

XENON_BASE_API int XenonSerializerReadFloat64(XenonSerializerHandle hSerializer, double* pOutValue);

XENON_BASE_API int XenonSerializerReadBuffer(XenonSerializerHandle hSerializer, size_t bufferLength, void* pOutBuffer);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_BASE_API int16_t XenonEndianSwapInt16(int16_t value);

XENON_BASE_API int32_t XenonEndianSwapInt32(int32_t value);

XENON_BASE_API int64_t XenonEndianSwapInt64(int64_t value);

XENON_BASE_API uint16_t XenonEndianSwapUint16(uint16_t value);

XENON_BASE_API uint32_t XenonEndianSwapUint32(uint32_t value);

XENON_BASE_API uint64_t XenonEndianSwapUint64(uint64_t value);

XENON_BASE_API float XenonEndianSwapFloat32(float value);

XENON_BASE_API double XenonEndianSwapFloat64(double value);

/*---------------------------------------------------------------------------------------------------------------------*/

#if XENON_LIB_RUNTIME

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenoRunMode
{
	XENON_RUN_STEP,
	XENON_RUN_TO_COMPLETION,
};

typedef struct XenonVm* XenonVmHandle;
typedef struct XenonFunction* XenonFunctionHandle;
typedef struct XenonExecution* XenonExecutionHandle;
typedef struct XenonFrame* XenonFrameHandle;
typedef struct XenonProgram* XenonProgramHandle;

typedef void (*XenonCallbackProgramDependency)(void*, const char*);
typedef void (*XenonCallbackResolveFrame)(void*, XenonFrameHandle);

typedef struct
{
	XenonCallbackProgramDependency onRequestFn;
	void* pUserData;
} XenonDependencyInit;

typedef struct
{
	XenonCommonInit common;
	XenonDependencyInit dependency;
} XenonVmInit;

#define XENON_VM_HANDLE_NULL        ((XenonVmHandle)0)
#define XENON_FUNCTION_HANDLE_NULL  ((XenonFunctionHandle)0)
#define XENON_EXECUTION_HANDLE_NULL ((XenonExecutionHandle)0)
#define XENON_FRAME_HANDLE_NULL     ((XenonFrameHandle)0)
#define XENON_PROGRAM_HANDLE_NULL   ((XenonProgramHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonVmCreate(XenonVmHandle* phOutVm, XenonVmInit init);

XENON_MAIN_API int XenonVmDispose(XenonVmHandle* phVm);

XENON_MAIN_API XenonReportHandle XenonVmGetReportHandle(XenonVmHandle hVm);

XENON_MAIN_API XenonFunctionHandle XenonVmGetFunction(XenonVmHandle hVm, const char* signature);

XENON_MAIN_API XenonValueHandle XenonVmGetObjectProfie(XenonVmHandle hVm, const char* objectTypeName);

XENON_MAIN_API int XenonVmLoadProgram(
	XenonVmHandle hVm,
	const char* programName,
	const void* pProgramFileData,
	const size_t programFileSize
);

XENON_MAIN_API int XenonVmLoadProgramFromFile(XenonVmHandle hVm, const char* filePath);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonExecutionCreate(
	XenonExecutionHandle* phOutExecution,
	XenonVmHandle hVm,
	XenonFunctionHandle hEntryPoint
);

XENON_MAIN_API int XenonExecutionDispose(XenonExecutionHandle* phExecution);

XENON_MAIN_API int XenonExecutionRun(XenonExecutionHandle hExec, int runMode);

XENON_MAIN_API int XenonExecutionYield(XenonExecutionHandle hExec);

XENON_MAIN_API bool XenonExecutionHasYielded(XenonExecutionHandle hExec);

XENON_MAIN_API bool XenonExecutionHasStarted(XenonExecutionHandle hExec);

XENON_MAIN_API bool XenonExecutionHasFinished(XenonExecutionHandle hExec);

XENON_MAIN_API bool XenonExecutionHasExceptionOccurred(XenonExecutionHandle hExec);

XENON_MAIN_API int XenonExecutionResolveFrameStack(
	XenonExecutionHandle hExec,
	XenonCallbackResolveFrame onResolveFrameFn,
	void* pUserData
);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonFramePushValue(XenonFrameHandle hFrame, XenonValueHandle hValue);

XENON_MAIN_API int XenonFramePopValue(XenonFrameHandle hFrame, XenonValueHandle* phOutValue);

XENON_MAIN_API int XenonFramePeekValue(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, int stackIndex);

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* XENON_LIB_RUNTIME */

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef XENON_LIB_COMPILER

/*---------------------------------------------------------------------------------------------------------------------*/

typedef struct XenonCompiler* XenonCompilerHandle;
typedef struct XenonProgramWriter* XenonProgramWriterHandle;

typedef struct
{
	XenonCommonInit common;
} XenonCompilerInit;

#define XENON_COMPILER_HANDLE_NULL       ((XenonCompilerHandle)0)
#define XENON_PROGRAM_WRITER_HANDLE_NULL ((XenonProgramWriterHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonCompilerCreate(XenonCompilerHandle* phOutCompiler, XenonCompilerInit init);

XENON_MAIN_API int XenonCompilerDispose(XenonCompilerHandle* phCompiler);

XENON_MAIN_API XenonReportHandle XenonCompilerGetReportHandle(XenonCompilerHandle hCompiler);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonProgramWriterCreate(XenonProgramWriterHandle* phOutProgramWriter, XenonCompilerHandle hCompiler);

XENON_MAIN_API int XenonProgramWriterDispose(XenonProgramWriterHandle* phProgramWriter);

XENON_MAIN_API int XenonProgramWriterAddDependency(XenonProgramWriterHandle hProgramWriter, const char* programName);

XENON_MAIN_API int XenonProgramWriterAddConstant(XenonProgramWriterHandle hProgramWriter, XenonValueHandle hValue, uint32_t* pOutConstantIndex);

XENON_MAIN_API int XenonProgramWriterAddGlobal(XenonProgramWriterHandle hProgramWriter, const char* variableName, uint32_t constantIndex);

XENON_MAIN_API int XenonProgramWriterAddFunction(
	XenonProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	const void* pFunctionBytecode,
	size_t bytecodeLength,
	uint16_t numParameters,
	uint16_t numReturnValues
);

XENON_MAIN_API int XenonProgramWriterAddLocalVariable(
	XenonProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	const char* variableName,
	uint32_t constantIndex
);

XENON_MAIN_API int XenonProgramWriterSerialize(
	XenonProgramWriterHandle hProgramWriter,
	XenonCompilerHandle hCompiler,
	XenonSerializerHandle hSerializer,
	int endianness
);

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* XENON_LIB_COMPILER */

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* _XENON_SCRIPT_H_ */
