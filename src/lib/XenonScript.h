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
#include <stdlib.h>

#include "XenonBuildEnv.h"

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#define XENON_VM_FRAME_STACK_SIZE 1024
#define XENON_VM_VALUE_STACK_SIZE 128

#define XENON_VM_IO_REGISTER_COUNT 128
#define XENON_VM_GP_REGISTER_COUNT 64

#define XENON_VM_THREAD_MINIMUM_STACK_SIZE 262144
#define XENON_VM_THREAD_DEFAULT_STACK_SIZE 1048576

#define XENON_VM_GC_DEFAULT_ITERATION_COUNT 32

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonErrorCodeEnum
{
	XENON_SUCCESS = 0,

	XENON_ERROR_UNSPECIFIED_FAILURE = -1,
	XENON_ERROR_INVALID_ARG = -2,
	XENON_ERROR_INVALID_TYPE = -3,
	XENON_ERROR_INVALID_DATA = -4,
	XENON_ERROR_INVALID_RANGE = -5,
	XENON_ERROR_BAD_ALLOCATION = -6,
	XENON_ERROR_KEY_ALREADY_EXISTS = -7,
	XENON_ERROR_KEY_DOES_NOT_EXIST = -8,
	XENON_ERROR_FAILED_TO_OPEN_FILE = -9,
	XENON_ERROR_STREAM_END = -10,
	XENON_ERROR_STACK_EMPTY = -11,
	XENON_ERROR_STACK_FULL = -12,
	XENON_ERROR_INDEX_OUT_OF_RANGE = -13,
	XENON_ERROR_SCRIPT_NO_FUNCTION = -14,
	XENON_ERROR_MISMATCH = -15,
	XENON_ERROR_UNKNOWN_ID = -16,
	XENON_ERROR_NO_WRITE = -17,
};

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonEndianOrderEnum
{
	XENON_ENDIAN_ORDER_UNKNOWN,
	XENON_ENDIAN_ORDER_NATIVE,
	XENON_ENDIAN_ORDER_LITTLE,
	XENON_ENDIAN_ORDER_BIG,
};

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_BASE_API const char* XenonGetErrorCodeString(int errorCode);

XENON_BASE_API const char* XenonGetEndianModeString(int endianness);

XENON_BASE_API const char* XenonGetValueTypeString(int valueType);

XENON_BASE_API int XenonGetPlatformEndianMode();

XENON_BASE_API const char* XenonGetBuiltInFunctionSignature(int builtInFunctionId);

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

enum XenonMessageTypeEnum
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

XENON_BASE_API const char* XenonFormatString(const char* fmt, ...);

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonSerializerModeEnum
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

XENON_BASE_API uint64_t XenonHiResTimerGetFrequency();

XENON_BASE_API uint64_t XenonHiResTimerGetTimestamp();

/*---------------------------------------------------------------------------------------------------------------------*/

#if XENON_LIB_RUNTIME

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenoRunModeEnum
{
	XENON_RUN_STEP,
	XENON_RUN_CONTINUOUS,
};

enum XenonExecStatusEnum
{
	XENON_EXEC_STATUS_YIELD,
	XENON_EXEC_STATUS_RUNNING,
	XENON_EXEC_STATUS_COMPLETE,
	XENON_EXEC_STATUS_EXCEPTION,
	XENON_EXEC_STATUS_ABORT,
};

enum XenonStandardExceptionEnum
{
	XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
	XENON_STANDARD_EXCEPTION_TYPE_ERROR,
	XENON_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,

	XENON_STANDARD_EXCEPTION__COUNT,
};

enum XenonExceptionSeverity
{
	XENON_EXCEPTION_SEVERITY_NORMAL,
	XENON_EXCEPTION_SEVERITY_FATAL,

	XENON_EXCEPTION_SEVERITY__COUNT,
};

typedef struct XenonVm* XenonVmHandle;
typedef struct XenonProgram* XenonProgramHandle;
typedef struct XenonFunction* XenonFunctionHandle;
typedef struct XenonExecution* XenonExecutionHandle;
typedef struct XenonFrame* XenonFrameHandle;
typedef struct XenonValue* XenonValueHandle;

typedef void (*XenonNativeFunction)(XenonExecutionHandle, XenonFunctionHandle, void*);

typedef void (*XenonCallbackProgramDependency)(void*, const char*);
typedef void (*XenonCallbackOpDisasm)(void*, const char*, uintptr_t);

typedef void (*XenonCallbackNativeValueCopy)(void**, const void*);
typedef void (*XenonCallbackNativeValueDestruct)(void*);
typedef bool (*XenonCallbackNativeValueEqual)(const void*, const void*);
typedef bool (*XenonCallbackNativeValueLessThan)(const void*, const void*);

typedef bool (*XenonCallbackIterateFrame)(void*, XenonFrameHandle);
typedef bool (*XenonCallbackIterateProgram)(void*, XenonProgramHandle);
typedef bool (*XenonCallbackIterateFunction)(void*, XenonFunctionHandle);
typedef bool (*XenonCallbackIterateVariable)(void*, const char*, XenonValueHandle);
typedef bool (*XenonCallbackIterateString)(void*, const char*);
typedef bool (*XenonCallbackIterateObjectMember)(void*, const char*, int);

typedef struct
{
	XenonCallbackProgramDependency onRequestFn;
	void* pUserData;
} XenonDependencyInit;

typedef struct
{
	XenonCommonInit common;
	XenonDependencyInit dependency;

	uint32_t gcThreadStackSize;
	uint32_t gcMaxIterationCount;
} XenonVmInit;

#define XENON_VM_HANDLE_NULL        ((XenonVmHandle)0)
#define XENON_PROGRAM_HANDLE_NULL   ((XenonProgramHandle)0)
#define XENON_FUNCTION_HANDLE_NULL  ((XenonFunctionHandle)0)
#define XENON_EXECUTION_HANDLE_NULL ((XenonExecutionHandle)0)
#define XENON_FRAME_HANDLE_NULL     ((XenonFrameHandle)0)
#define XENON_VALUE_HANDLE_NULL     ((XenonValueHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonVmCreate(XenonVmHandle* phOutVm, XenonVmInit init);

XENON_MAIN_API int XenonVmDispose(XenonVmHandle* phVm);

XENON_MAIN_API int XenonVmGetReportHandle(XenonVmHandle hVm, XenonReportHandle* phOutReport);

XENON_MAIN_API int XenonVmGetProgram(XenonVmHandle hVm, XenonProgramHandle* phOutProgram, const char* programName);

XENON_MAIN_API int XenonVmGetProgramCount(XenonVmHandle hVm, size_t* pOutCount);

XENON_MAIN_API int XenonVmListPrograms(XenonVmHandle hVm, XenonCallbackIterateProgram onIterateFn, void* pUserData);

XENON_MAIN_API int XenonVmGetFunction(XenonVmHandle hVm, XenonFunctionHandle* phOutFunction, const char* signature);

XENON_MAIN_API int XenonVmGetFunctionCount(XenonVmHandle hVm, size_t* pOutCount);

XENON_MAIN_API int XenonVmListFunctions(XenonVmHandle hVm, XenonCallbackIterateFunction onIterateFn, void* pUserData);

XENON_MAIN_API int XenonVmSetGlobalVariable(XenonVmHandle hVm, XenonValueHandle hValue, const char* variableName);

XENON_MAIN_API int XenonVmGetGlobalVariable(XenonVmHandle hVm, XenonValueHandle* phOutValue, const char* variableName);

XENON_MAIN_API int XenonVmGetGlobalVariableCount(XenonVmHandle hVm, size_t* pOutCount);

XENON_MAIN_API int XenonVmListGlobalVariables(XenonVmHandle hVm, XenonCallbackIterateVariable onIterateFn, void* pUserData);

XENON_MAIN_API int XenonVmListObjectSchemas(XenonVmHandle hVm, XenonCallbackIterateString onIterateFn, void* pUserData);

XENON_MAIN_API int XenonVmLoadProgram(
	XenonVmHandle hVm,
	const char* programName,
	const void* pProgramFileData,
	const size_t programFileSize
);

XENON_MAIN_API int XenonVmLoadProgramFromFile(XenonVmHandle hVm, const char* programName, const char* filePath);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonProgramGetVm(XenonProgramHandle hProgram, XenonVmHandle* phOutVm);

XENON_MAIN_API int XenonProgramGetName(XenonProgramHandle hProgram, const char** pOutName);

XENON_MAIN_API int XenonProgramGetEndianness(XenonProgramHandle hProgram, int* pOutEndianness);

XENON_MAIN_API int XenonProgramGetFunctionCount(XenonProgramHandle hProgram, size_t* pOutCount);

XENON_MAIN_API int XenonProgramListFunctions(XenonProgramHandle hProgram, XenonCallbackIterateString onIterateFn, void* pUserData);

XENON_MAIN_API int XenonProgramGetGlobalVariableCount(XenonProgramHandle hProgram, size_t* pOutCount);

XENON_MAIN_API int XenonProgramListGlobalVariables(XenonProgramHandle hProgram, XenonCallbackIterateString onIterateFn, void* pUserData);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonFunctionGetProgram(XenonFunctionHandle hFunction, XenonProgramHandle* phOutProgram);

XENON_MAIN_API int XenonFunctionGetSignature(XenonFunctionHandle hFunction, const char** pOutSignature);

XENON_MAIN_API int XenonFunctionGetIsNative(XenonFunctionHandle hFunction, bool* pOutNative);

XENON_MAIN_API int XenonFunctionGetParameterCount(XenonFunctionHandle hFunction, uint16_t* pOutCount);

XENON_MAIN_API int XenonFunctionGetReturnValueCount(XenonFunctionHandle hFunction, uint16_t* pOutCount);

XENON_MAIN_API int XenonFunctionGetBytecodeOffset(XenonFunctionHandle hFunction, uint32_t* pOutOffset);

XENON_MAIN_API int XenonFunctionGetNativeBinding(XenonFunctionHandle hFunction, XenonNativeFunction* pOutNativeFn);

XENON_MAIN_API int XenonFunctionGetNativeUserData(XenonFunctionHandle hFunction, void** ppOutUerData);

XENON_MAIN_API int XenonFunctionSetNativeBinding(
	XenonFunctionHandle hFunction,
	XenonNativeFunction nativeFn,
	void* pUserData
);

XENON_MAIN_API int XenonFunctionDisassemble(
	XenonFunctionHandle hFunction,
	XenonCallbackOpDisasm onDisasmFn,
	void* pUserData
);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonExecutionCreate(
	XenonExecutionHandle* phOutExecution,
	XenonVmHandle hVm,
	XenonFunctionHandle hEntryPoint
);

XENON_MAIN_API int XenonExecutionDispose(XenonExecutionHandle* phExecution);

XENON_MAIN_API int XenonExecutionRun(XenonExecutionHandle hExec, int runMode);

XENON_MAIN_API int XenonExecutionYield(XenonExecutionHandle hExec);

XENON_MAIN_API int XenonExecutionRaiseStandardException(
	XenonExecutionHandle hExec,
	int severity,
	int exceptionType,
	const char* message,
	...
);

XENON_MAIN_API int XenonExecutionRaiseException(XenonExecutionHandle hExec, XenonValueHandle hValue, int severity);

XENON_MAIN_API int XenonExecutionGetVm(XenonExecutionHandle hExec, XenonVmHandle* phOutVm);

XENON_MAIN_API int XenonExecutionGetStatus(XenonExecutionHandle hExec, int statusType, bool* pOutStatus);

XENON_MAIN_API int XenonExecutionHasUnhandledExceptionOccurred(XenonExecutionHandle hExec, bool* pOutException);

XENON_MAIN_API int XenonExecutionGetFrameStackDepth(XenonExecutionHandle hExec, size_t* pOutDepth);

XENON_MAIN_API int XenonExecutionResolveFrameStack(
	XenonExecutionHandle hExec,
	XenonCallbackIterateFrame onIterateFn,
	void* pUserData
);

XENON_MAIN_API int XenonExecutionGetCurrentFrame(XenonExecutionHandle hExec, XenonFrameHandle* phOutFrame);

XENON_MAIN_API int XenonExecutionSetIoRegister(XenonExecutionHandle hExec, XenonValueHandle hValue, int registerIndex);

XENON_MAIN_API int XenonExecutionGetIoRegister(XenonExecutionHandle hExec, XenonValueHandle* phOutValue, int registerIndex);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonFrameGetFunction(XenonFrameHandle hFrame, XenonFunctionHandle* phOutFunction);

XENON_MAIN_API int XenonFrameGetBytecodeOffset(XenonFrameHandle hFrame, uint32_t* pOutOffset);

XENON_MAIN_API int XenonFramePushValue(XenonFrameHandle hFrame, XenonValueHandle hValue);

XENON_MAIN_API int XenonFramePopValue(XenonFrameHandle hFrame, XenonValueHandle* phOutValue);

XENON_MAIN_API int XenonFramePeekValue(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, int stackIndex);

XENON_MAIN_API int XenonFrameSetGpRegister(XenonFrameHandle hFrame, XenonValueHandle hValue, int registerIndex);

XENON_MAIN_API int XenonFrameGetGpRegister(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, int registerIndex);

XENON_MAIN_API int XenonFrameSetLocalVariable(XenonFrameHandle hFrame, XenonValueHandle hValue, const char* variableName);

XENON_MAIN_API int XenonFrameGetLocalVariable(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, const char* variableName);

XENON_MAIN_API int XenonFrameGetLocalVariableCount(XenonFrameHandle hFrame, size_t* pOutCount);

XENON_MAIN_API int XenonFrameListLocalVariables(XenonFrameHandle hFrame, XenonCallbackIterateVariable onIterateFn, void* pUserData);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API XenonValueHandle XenonValueCreateBool(XenonVmHandle hVm, bool value);

XENON_MAIN_API XenonValueHandle XenonValueCreateInt8(XenonVmHandle hVm, int8_t value);

XENON_MAIN_API XenonValueHandle XenonValueCreateInt16(XenonVmHandle hVm, int16_t value);

XENON_MAIN_API XenonValueHandle XenonValueCreateInt32(XenonVmHandle hVm, int32_t value);

XENON_MAIN_API XenonValueHandle XenonValueCreateInt64(XenonVmHandle hVm, int64_t value);

XENON_MAIN_API XenonValueHandle XenonValueCreateUint8(XenonVmHandle hVm, uint8_t value);

XENON_MAIN_API XenonValueHandle XenonValueCreateUint16(XenonVmHandle hVm, uint16_t value);

XENON_MAIN_API XenonValueHandle XenonValueCreateUint32(XenonVmHandle hVm, uint32_t value);

XENON_MAIN_API XenonValueHandle XenonValueCreateUint64(XenonVmHandle hVm, uint64_t value);

XENON_MAIN_API XenonValueHandle XenonValueCreateFloat32(XenonVmHandle hVm, float value);

XENON_MAIN_API XenonValueHandle XenonValueCreateFloat64(XenonVmHandle hVm, double value);

XENON_MAIN_API XenonValueHandle XenonValueCreateNull();

XENON_MAIN_API XenonValueHandle XenonValueCreateString(XenonVmHandle hVm, const char* const string);

XENON_MAIN_API XenonValueHandle XenonValueCreateObject(XenonVmHandle hVm, const char* const typeName);

XENON_MAIN_API XenonValueHandle XenonValueCreateArray(XenonVmHandle hVm, size_t count);

XENON_MAIN_API XenonValueHandle XenonValueCreateNative(
	XenonVmHandle hVm,
	void* pNativeObject,
	XenonCallbackNativeValueCopy onCopy,
	XenonCallbackNativeValueDestruct onDestruct,
	XenonCallbackNativeValueEqual onTestEqual,
	XenonCallbackNativeValueLessThan onTestLessThan
);

XENON_MAIN_API XenonValueHandle XenonValueCopy(XenonVmHandle hVm, XenonValueHandle hValue);

XENON_MAIN_API void XenonValueAbandon(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsPrimitiveType(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsBool(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsInt8(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsInt16(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsInt32(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsInt64(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsUint8(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsUint16(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsUint32(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsUint64(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsFloat32(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsFloat64(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsNull(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsNative(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsString(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsObject(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueIsArray(XenonValueHandle hValue);

XENON_MAIN_API bool XenonValueGetBool(XenonValueHandle hValue);

XENON_MAIN_API int8_t XenonValueGetInt8(XenonValueHandle hValue);

XENON_MAIN_API int16_t XenonValueGetInt16(XenonValueHandle hValue);

XENON_MAIN_API int32_t XenonValueGetInt32(XenonValueHandle hValue);

XENON_MAIN_API int64_t XenonValueGetInt64(XenonValueHandle hValue);

XENON_MAIN_API uint8_t XenonValueGetUint8(XenonValueHandle hValue);

XENON_MAIN_API uint16_t XenonValueGetUint16(XenonValueHandle hValue);

XENON_MAIN_API uint32_t XenonValueGetUint32(XenonValueHandle hValue);

XENON_MAIN_API uint64_t XenonValueGetUint64(XenonValueHandle hValue);

XENON_MAIN_API float XenonValueGetFloat32(XenonValueHandle hValue);

XENON_MAIN_API double XenonValueGetFloat64(XenonValueHandle hValue);

XENON_MAIN_API void* XenonValueGetNative(XenonValueHandle hValue);

XENON_MAIN_API const char* XenonValueGetString(XenonValueHandle hValue);

XENON_MAIN_API size_t XenonValueGetStringLength(XenonValueHandle hValue);

XENON_MAIN_API size_t XenonValueGetStringHash(XenonValueHandle hValue);

XENON_MAIN_API const char* XenonValueGetObjectTypeName(XenonValueHandle hValue);

XENON_MAIN_API size_t XenonValueGetObjectMemberCount(XenonValueHandle hValue);

XENON_MAIN_API XenonValueHandle XenonValueGetObjectMemberValue(XenonValueHandle hValue, const char* memberName);

XENON_MAIN_API uint8_t XenonValueGetObjectMemberType(XenonValueHandle hValue, const char* memberName);

XENON_MAIN_API int XenonValueSetObjectMemberValue(XenonValueHandle hValue, const char* memberName, XenonValueHandle hMemberValue);

XENON_MAIN_API int XenonValueListObjectMembers(XenonValueHandle hValue, XenonCallbackIterateObjectMember onIterateFn, void* pUserData);

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

typedef bool (*XenonCallbackIterateBuiltInFunction)(void*, int, const char*);

#define XENON_COMPILER_HANDLE_NULL       ((XenonCompilerHandle)0)
#define XENON_PROGRAM_WRITER_HANDLE_NULL ((XenonProgramWriterHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonCompilerCreate(XenonCompilerHandle* phOutCompiler, XenonCompilerInit init);

XENON_MAIN_API int XenonCompilerDispose(XenonCompilerHandle* phCompiler);

XENON_MAIN_API int XenonCompilerGetReportHandle(XenonCompilerHandle hCompiler, XenonReportHandle* phOutReport);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonProgramWriterCreate(XenonProgramWriterHandle* phOutProgramWriter, XenonCompilerHandle hCompiler);

XENON_MAIN_API int XenonProgramWriterDispose(XenonProgramWriterHandle* phProgramWriter);

XENON_MAIN_API int XenonProgramWriterAddDependency(XenonProgramWriterHandle hProgramWriter, const char* programName);

XENON_MAIN_API int XenonProgramWriterAddConstantNull(XenonProgramWriterHandle hProgramWriter, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantBool(XenonProgramWriterHandle hProgramWriter, bool value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantInt8(XenonProgramWriterHandle hProgramWriter, int8_t value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantInt16(XenonProgramWriterHandle hProgramWriter, int16_t value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantInt32(XenonProgramWriterHandle hProgramWriter, int32_t value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantInt64(XenonProgramWriterHandle hProgramWriter, int64_t value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantUint8(XenonProgramWriterHandle hProgramWriter, uint8_t value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantUint16(XenonProgramWriterHandle hProgramWriter, uint16_t value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantUint32(XenonProgramWriterHandle hProgramWriter, uint32_t value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantUint64(XenonProgramWriterHandle hProgramWriter, uint64_t value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantFloat32(XenonProgramWriterHandle hProgramWriter, float value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantFloat64(XenonProgramWriterHandle hProgramWriter, double value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddConstantString(XenonProgramWriterHandle hProgramWriter, const char* value, uint32_t* pOutIndex);

XENON_MAIN_API int XenonProgramWriterAddObjectType(XenonProgramWriterHandle hProgramWriter, const char* objectTypeName);

XENON_MAIN_API int XenonProgramWriterAddObjectMember(
	XenonProgramWriterHandle hProgramWriter,
	const char* objectTypeName,
	const char* memberName,
	uint8_t memberValueType,
	uint32_t* pOutIndex
);

XENON_MAIN_API int XenonProgramWriterAddGlobal(XenonProgramWriterHandle hProgramWriter, const char* variableName, uint32_t constantIndex);

XENON_MAIN_API int XenonProgramWriterAddFunction(
	XenonProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	const void* pFunctionBytecode,
	size_t bytecodeLength,
	uint16_t numParameters,
	uint16_t numReturnValues
);

XENON_MAIN_API int XenonProgramWriterAddNativeFunction(
	XenonProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	uint16_t numParameters,
	uint16_t numReturnValues
);

XENON_MAIN_API int XenonProgramWriterAddLocalVariable(
	XenonProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	const char* variableName,
	uint32_t constantIndex
);

XENON_MAIN_API int XenonProgramWriterAddGuardedBlock(
	XenonProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	size_t bytecodeOffset,
	size_t bytecodeLength,
	uint32_t* pOutBlockId
);

XENON_MAIN_API int XenonProgramWriterAddExceptionHandler(
	XenonProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	uint32_t blockId,
	size_t bytecodeOffset,
	int handledType,
	const char* className
);

XENON_MAIN_API int XenonProgramWriterSerialize(
	XenonProgramWriterHandle hProgramWriter,
	XenonCompilerHandle hCompiler,
	XenonSerializerHandle hSerializer
);

/*---------------------------------------------------------------------------------------------------------------------*/

XENON_MAIN_API int XenonBytecodeWriteNop(XenonSerializerHandle hSerializer);

XENON_MAIN_API int XenonBytecodeWriteAbort(XenonSerializerHandle hSerializer);

XENON_MAIN_API int XenonBytecodeWriteReturn(XenonSerializerHandle hSerializer);

XENON_MAIN_API int XenonBytecodeWriteYield(XenonSerializerHandle hSerializer);

XENON_MAIN_API int XenonBytecodeWriteCall(XenonSerializerHandle hSerializer, uint32_t constantIndex);

XENON_MAIN_API int XenonBytecodeWriteRaise(XenonSerializerHandle hSerializer, uint32_t gpRegIndex);

XENON_MAIN_API int XenonBytecodeWriteLoadConstant(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

XENON_MAIN_API int XenonBytecodeWriteLoadGlobal(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

XENON_MAIN_API int XenonBytecodeWriteLoadLocal(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

XENON_MAIN_API int XenonBytecodeWriteLoadParam(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t ioRegIndex
);

XENON_MAIN_API int XenonBytecodeWriteLoadObject(
	XenonSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t memberIndex
);

XENON_MAIN_API int XenonBytecodeWriteStoreGlobal(
	XenonSerializerHandle hSerializer,
	uint32_t constantIndex,
	uint32_t gpRegIndex
);

XENON_MAIN_API int XenonBytecodeWriteStoreLocal(
	XenonSerializerHandle hSerializer,
	uint32_t constantIndex,
	uint32_t gpRegIndex
);

XENON_MAIN_API int XenonBytecodeWriteStoreParam(
	XenonSerializerHandle hSerializer,
	uint32_t ioRegIndex,
	uint32_t gpRegIndex
);

XENON_MAIN_API int XenonBytecodeWriteStoreObject(
	XenonSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t memberIndex
);

XENON_MAIN_API int XenonBytecodeWritePullGlobal(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

XENON_MAIN_API int XenonBytecodeWritePullLocal(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

XENON_MAIN_API int XenonBytecodeWritePullParam(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t ioRegIndex
);

XENON_MAIN_API int XenonBytecodeWritePullObject(
	XenonSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t memberIndex
);

XENON_MAIN_API int XenonBytecodeWritePush(XenonSerializerHandle hSerializer, uint32_t gpRegIndex);

XENON_MAIN_API int XenonBytecodeWritePop(XenonSerializerHandle hSerializer, uint32_t gpRegIndex);

XENON_MAIN_API int XenonBytecodeWriteInitObject(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

XENON_MAIN_API int XenonBytecodeWriteInitArray(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t initialCount
);

XENON_MAIN_API int XenonBytecodeWriteBranch(XenonSerializerHandle hSerializer, int32_t offset);

XENON_MAIN_API int XenonBytecodeWriteBranchIfTrue(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int32_t offset
);

XENON_MAIN_API int XenonBytecodeWriteBranchIfFalse(
	XenonSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int32_t offset
);

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* XENON_LIB_COMPILER */

//----------------------------------------------------------------------------------------------------------------------

enum XenonValueType
{
	XENON_VALUE_TYPE_NULL,

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

	XENON_VALUE_TYPE_BOOL,

	XENON_VALUE_TYPE_STRING,
	XENON_VALUE_TYPE_OBJECT,
	XENON_VALUE_TYPE_ARRAY,
	XENON_VALUE_TYPE_NATIVE,

	XENON_VALUE_TYPE__MAX_VALUE = XENON_VALUE_TYPE_NATIVE,
};

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonBuiltInEnum
{
	XENON_BUILT_IN_OP_ADD_BOOL,
	XENON_BUILT_IN_OP_ADD_INT8,
	XENON_BUILT_IN_OP_ADD_INT16,
	XENON_BUILT_IN_OP_ADD_INT32,
	XENON_BUILT_IN_OP_ADD_INT64,
	XENON_BUILT_IN_OP_ADD_UINT8,
	XENON_BUILT_IN_OP_ADD_UINT16,
	XENON_BUILT_IN_OP_ADD_UINT32,
	XENON_BUILT_IN_OP_ADD_UINT64,
	XENON_BUILT_IN_OP_ADD_FLOAT32,
	XENON_BUILT_IN_OP_ADD_FLOAT64,
	XENON_BUILT_IN_OP_ADD_STRING,

	XENON_BUILT_IN_OP_SUB_BOOL,
	XENON_BUILT_IN_OP_SUB_INT8,
	XENON_BUILT_IN_OP_SUB_INT16,
	XENON_BUILT_IN_OP_SUB_INT32,
	XENON_BUILT_IN_OP_SUB_INT64,
	XENON_BUILT_IN_OP_SUB_UINT8,
	XENON_BUILT_IN_OP_SUB_UINT16,
	XENON_BUILT_IN_OP_SUB_UINT32,
	XENON_BUILT_IN_OP_SUB_UINT64,
	XENON_BUILT_IN_OP_SUB_FLOAT32,
	XENON_BUILT_IN_OP_SUB_FLOAT64,

	XENON_BUILT_IN_OP_MUL_BOOL,
	XENON_BUILT_IN_OP_MUL_INT8,
	XENON_BUILT_IN_OP_MUL_INT16,
	XENON_BUILT_IN_OP_MUL_INT32,
	XENON_BUILT_IN_OP_MUL_INT64,
	XENON_BUILT_IN_OP_MUL_UINT8,
	XENON_BUILT_IN_OP_MUL_UINT16,
	XENON_BUILT_IN_OP_MUL_UINT32,
	XENON_BUILT_IN_OP_MUL_UINT64,
	XENON_BUILT_IN_OP_MUL_FLOAT32,
	XENON_BUILT_IN_OP_MUL_FLOAT64,

	XENON_BUILT_IN_OP_DIV_BOOL,
	XENON_BUILT_IN_OP_DIV_INT8,
	XENON_BUILT_IN_OP_DIV_INT16,
	XENON_BUILT_IN_OP_DIV_INT32,
	XENON_BUILT_IN_OP_DIV_INT64,
	XENON_BUILT_IN_OP_DIV_UINT8,
	XENON_BUILT_IN_OP_DIV_UINT16,
	XENON_BUILT_IN_OP_DIV_UINT32,
	XENON_BUILT_IN_OP_DIV_UINT64,
	XENON_BUILT_IN_OP_DIV_FLOAT32,
	XENON_BUILT_IN_OP_DIV_FLOAT64,

	XENON_BUILT_IN_OP_CAST_BOOL_TO_INT8,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_INT16,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_INT32,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_INT64,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_BOOL_TO_STRING,

	XENON_BUILT_IN_OP_CAST_INT8_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_INT8_TO_INT16,
	XENON_BUILT_IN_OP_CAST_INT8_TO_INT32,
	XENON_BUILT_IN_OP_CAST_INT8_TO_INT64,
	XENON_BUILT_IN_OP_CAST_INT8_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_INT8_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_INT8_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_INT8_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_INT8_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_INT8_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_INT8_TO_STRING,

	XENON_BUILT_IN_OP_CAST_INT16_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_INT16_TO_INT8,
	XENON_BUILT_IN_OP_CAST_INT16_TO_INT32,
	XENON_BUILT_IN_OP_CAST_INT16_TO_INT64,
	XENON_BUILT_IN_OP_CAST_INT16_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_INT16_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_INT16_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_INT16_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_INT16_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_INT16_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_INT16_TO_STRING,

	XENON_BUILT_IN_OP_CAST_INT32_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_INT32_TO_INT8,
	XENON_BUILT_IN_OP_CAST_INT32_TO_INT16,
	XENON_BUILT_IN_OP_CAST_INT32_TO_INT64,
	XENON_BUILT_IN_OP_CAST_INT32_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_INT32_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_INT32_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_INT32_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_INT32_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_INT32_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_INT32_TO_STRING,

	XENON_BUILT_IN_OP_CAST_INT64_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_INT64_TO_INT8,
	XENON_BUILT_IN_OP_CAST_INT64_TO_INT16,
	XENON_BUILT_IN_OP_CAST_INT64_TO_INT32,
	XENON_BUILT_IN_OP_CAST_INT64_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_INT64_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_INT64_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_INT64_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_INT64_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_INT64_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_INT64_TO_STRING,

	XENON_BUILT_IN_OP_CAST_UINT8_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_INT8,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_INT16,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_INT32,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_INT64,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_UINT8_TO_STRING,

	XENON_BUILT_IN_OP_CAST_UINT16_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_INT8,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_INT16,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_INT32,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_INT64,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_UINT16_TO_STRING,

	XENON_BUILT_IN_OP_CAST_UINT32_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_INT8,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_INT16,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_INT32,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_INT64,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_UINT32_TO_STRING,

	XENON_BUILT_IN_OP_CAST_UINT64_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_INT8,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_INT16,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_INT32,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_INT64,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_UINT64_TO_STRING,

	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_INT8,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_INT16,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_INT32,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_INT64,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_FLOAT64,
	XENON_BUILT_IN_OP_CAST_FLOAT32_TO_STRING,

	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_INT8,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_INT16,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_INT32,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_INT64,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_FLOAT64_TO_STRING,

	XENON_BUILT_IN_OP_CAST_STRING_TO_BOOL,
	XENON_BUILT_IN_OP_CAST_STRING_TO_INT8,
	XENON_BUILT_IN_OP_CAST_STRING_TO_INT16,
	XENON_BUILT_IN_OP_CAST_STRING_TO_INT32,
	XENON_BUILT_IN_OP_CAST_STRING_TO_INT64,
	XENON_BUILT_IN_OP_CAST_STRING_TO_UINT8,
	XENON_BUILT_IN_OP_CAST_STRING_TO_UINT32,
	XENON_BUILT_IN_OP_CAST_STRING_TO_UINT16,
	XENON_BUILT_IN_OP_CAST_STRING_TO_UINT64,
	XENON_BUILT_IN_OP_CAST_STRING_TO_FLOAT32,
	XENON_BUILT_IN_OP_CAST_STRING_TO_FLOAT64,

	XENON_BUILT_IN_OP_LEN_STRING,

	XENON_BUILT_IN__TOTAL_COUNT,
	XENON_BUILT_IN__FOCE_DWORD = 0x7FFFFFFFul,
};

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* _XENON_SCRIPT_H_ */
