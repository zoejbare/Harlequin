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

#ifndef _HQ_SCRIPT_H_
#define _HQ_SCRIPT_H_

/*---------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "HqBuildEnv.h"

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#define HQ_VM_FRAME_STACK_SIZE 1024
#define HQ_VM_VALUE_STACK_SIZE 128

#define HQ_VM_IO_REGISTER_COUNT 128
#define HQ_VM_GP_REGISTER_COUNT 64

#define HQ_VM_THREAD_MINIMUM_STACK_SIZE 262144
#define HQ_VM_THREAD_DEFAULT_STACK_SIZE 1048576

#define HQ_VM_GC_DEFAULT_ITERATION_COUNT 64

/*---------------------------------------------------------------------------------------------------------------------*/

enum HqErrorCodeEnum
{
	HQ_SUCCESS = 0,

	HQ_ERROR_UNSPECIFIED_FAILURE = -1,
	HQ_ERROR_INVALID_ARG = -2,
	HQ_ERROR_INVALID_TYPE = -3,
	HQ_ERROR_INVALID_DATA = -4,
	HQ_ERROR_INVALID_RANGE = -5,
	HQ_ERROR_BAD_ALLOCATION = -6,
	HQ_ERROR_KEY_ALREADY_EXISTS = -7,
	HQ_ERROR_KEY_DOES_NOT_EXIST = -8,
	HQ_ERROR_FAILED_TO_OPEN_FILE = -9,
	HQ_ERROR_STREAM_END = -10,
	HQ_ERROR_STACK_EMPTY = -11,
	HQ_ERROR_STACK_FULL = -12,
	HQ_ERROR_INDEX_OUT_OF_RANGE = -13,
	HQ_ERROR_SCRIPT_NO_FUNCTION = -14,
	HQ_ERROR_MISMATCH = -15,
	HQ_ERROR_UNKNOWN_ID = -16,
	HQ_ERROR_NO_WRITE = -17,
};

/*---------------------------------------------------------------------------------------------------------------------*/

enum HqEndianOrderEnum
{
	HQ_ENDIAN_ORDER_UNKNOWN,
	HQ_ENDIAN_ORDER_NATIVE,
	HQ_ENDIAN_ORDER_LITTLE,
	HQ_ENDIAN_ORDER_BIG,
};

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_BASE_API const char* HqGetErrorCodeString(int errorCode);

HQ_BASE_API const char* HqGetEndianModeString(int endianness);

HQ_BASE_API const char* HqGetValueTypeString(int valueType);

HQ_BASE_API int HqGetPlatformEndianMode();

/*---------------------------------------------------------------------------------------------------------------------*/

typedef void* (*HqCallbackMemAlloc)(size_t);
typedef void* (*HqCallbackMemRealloc)(void*, size_t);
typedef void  (*HqCallbackMemFree)(void*);

typedef struct
{
	HqCallbackMemAlloc allocFn;
	HqCallbackMemRealloc reallocFn;
	HqCallbackMemFree freeFn;
} HqMemAllocator;

/*---------------------------------------------------------------------------------------------------------------------*/

enum HqMessageTypeEnum
{
	HQ_MESSAGE_TYPE_VERBOSE,
	HQ_MESSAGE_TYPE_INFO,
	HQ_MESSAGE_TYPE_WARNING,
	HQ_MESSAGE_TYPE_ERROR,
	HQ_MESSAGE_TYPE_FATAL,
};

typedef void (*HqMessageCallback)(void*, int, const char*);

typedef struct HqReport* HqReportHandle;

#define HQ_REPORT_HANDLE_NULL ((HqReportHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

typedef struct
{
	HqMessageCallback onMessageFn;
	void* pUserData;
	int reportLevel;
} HqReportInit;

typedef struct
{
	HqReportInit report;
} HqCommonInit;

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_BASE_API HqMemAllocator HqMemGetDefaultAllocator();

HQ_BASE_API HqMemAllocator HqMemGetAllocator();

HQ_BASE_API int HqMemSetAllocator(HqMemAllocator allocator);

HQ_BASE_API void* HqMemAlloc(size_t size);

HQ_BASE_API void* HqMemRealloc(void* pMem, size_t size);

HQ_BASE_API int HqMemFree(void* pMem);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_BASE_API int HqReportMessage(HqReportHandle hReport, int messageType, const char* fmt, ...);

HQ_BASE_API int HqReportSetLevel(HqReportHandle hReport, int messageType);

HQ_BASE_API int HqReportGetLevel(HqReportHandle hReport);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_BASE_API char* HqFormatString(const char* fmt, ...);

HQ_BASE_API char* HqGetBuiltInFunctionSignature(int builtInFunctionId);

/*---------------------------------------------------------------------------------------------------------------------*/

enum HqSerializerModeEnum
{
	HQ_SERIALIZER_MODE_UNKNOWN,
	HQ_SERIALIZER_MODE_READER,
	HQ_SERIALIZER_MODE_WRITER,
};

typedef struct HqSerializer* HqSerializerHandle;

#define HQ_SERIALIZER_HANDLE_NULL ((HqSerializerHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_BASE_API int HqSerializerCreate(HqSerializerHandle* phOutSerializer, int mode);

HQ_BASE_API int HqSerializerDispose(HqSerializerHandle* phSerializer);

HQ_BASE_API int HqSerializerGetMode(HqSerializerHandle hSerializer);

HQ_BASE_API int HqSerializerGetEndianness(HqSerializerHandle hSerializer);

HQ_BASE_API size_t HqSerializerGetStreamLength(HqSerializerHandle hSerializer);

HQ_BASE_API size_t HqSerializerGetStreamPosition(HqSerializerHandle hSerializer);

HQ_BASE_API void* HqSerializerGetRawStreamPointer(HqSerializerHandle hSerializer);

HQ_BASE_API int HqSerializerSetEndianness(HqSerializerHandle hSerializer, int endianness);

HQ_BASE_API int HqSerializerSetStreamPosition(HqSerializerHandle hSerializer, size_t position);

HQ_BASE_API int HqSerializerLoadStreamFromFile(HqSerializerHandle hSerializer, const char* filePath);

HQ_BASE_API int HqSerializerLoadStreamFromBuffer(HqSerializerHandle hSerializer, const void* pBuffer, size_t bufferLength);

HQ_BASE_API int HqSerializerSaveStreamToFile(HqSerializerHandle hSerializer, const char* filePath, bool append);

HQ_BASE_API int HqSerializerSaveStreamToBuffer(HqSerializerHandle hSerializer, void* pBuffer, size_t* pBufferLength);

HQ_BASE_API int HqSerializerWriteBool(HqSerializerHandle hSerializer, bool value);

HQ_BASE_API int HqSerializerWriteInt8(HqSerializerHandle hSerializer, int8_t value);

HQ_BASE_API int HqSerializerWriteInt16(HqSerializerHandle hSerializer, int16_t value);

HQ_BASE_API int HqSerializerWriteInt32(HqSerializerHandle hSerializer, int32_t value);

HQ_BASE_API int HqSerializerWriteInt64(HqSerializerHandle hSerializer, int64_t value);

HQ_BASE_API int HqSerializerWriteUint8(HqSerializerHandle hSerializer, uint8_t value);

HQ_BASE_API int HqSerializerWriteUint16(HqSerializerHandle hSerializer, uint16_t value);

HQ_BASE_API int HqSerializerWriteUint32(HqSerializerHandle hSerializer, uint32_t value);

HQ_BASE_API int HqSerializerWriteUint64(HqSerializerHandle hSerializer, uint64_t value);

HQ_BASE_API int HqSerializerWriteFloat32(HqSerializerHandle hSerializer, float value);

HQ_BASE_API int HqSerializerWriteFloat64(HqSerializerHandle hSerializer, double value);

HQ_BASE_API int HqSerializerWriteBuffer(HqSerializerHandle hSerializer, size_t bufferLength, const void* pBuffer);

HQ_BASE_API int HqSerializerReadBool(HqSerializerHandle hSerializer, bool* pOutValue);

HQ_BASE_API int HqSerializerReadInt8(HqSerializerHandle hSerializer, int8_t* pOutValue);

HQ_BASE_API int HqSerializerReadInt16(HqSerializerHandle hSerializer, int16_t* pOutValue);

HQ_BASE_API int HqSerializerReadInt32(HqSerializerHandle hSerializer, int32_t* pOutValue);

HQ_BASE_API int HqSerializerReadInt64(HqSerializerHandle hSerializer, int64_t* pOutValue);

HQ_BASE_API int HqSerializerReadUint8(HqSerializerHandle hSerializer, uint8_t* pOutValue);

HQ_BASE_API int HqSerializerReadUint16(HqSerializerHandle hSerializer, uint16_t* pOutValue);

HQ_BASE_API int HqSerializerReadUint32(HqSerializerHandle hSerializer, uint32_t* pOutValue);

HQ_BASE_API int HqSerializerReadUint64(HqSerializerHandle hSerializer, uint64_t* pOutValue);

HQ_BASE_API int HqSerializerReadFloat32(HqSerializerHandle hSerializer, float* pOutValue);

HQ_BASE_API int HqSerializerReadFloat64(HqSerializerHandle hSerializer, double* pOutValue);

HQ_BASE_API int HqSerializerReadBuffer(HqSerializerHandle hSerializer, size_t bufferLength, void* pOutBuffer);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_BASE_API int16_t HqEndianSwapInt16(int16_t value);

HQ_BASE_API int32_t HqEndianSwapInt32(int32_t value);

HQ_BASE_API int64_t HqEndianSwapInt64(int64_t value);

HQ_BASE_API uint16_t HqEndianSwapUint16(uint16_t value);

HQ_BASE_API uint32_t HqEndianSwapUint32(uint32_t value);

HQ_BASE_API uint64_t HqEndianSwapUint64(uint64_t value);

HQ_BASE_API float HqEndianSwapFloat32(float value);

HQ_BASE_API double HqEndianSwapFloat64(double value);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_BASE_API uint64_t HqHiResTimerGetFrequency();

HQ_BASE_API uint64_t HqHiResTimerGetTimestamp();

/*---------------------------------------------------------------------------------------------------------------------*/

#if HQ_LIB_RUNTIME

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenoRunModeEnum
{
	HQ_RUN_STEP,
	HQ_RUN_CONTINUOUS,
};

enum HqExecStatusEnum
{
	HQ_EXEC_STATUS_YIELD,
	HQ_EXEC_STATUS_RUNNING,
	HQ_EXEC_STATUS_COMPLETE,
	HQ_EXEC_STATUS_EXCEPTION,
	HQ_EXEC_STATUS_ABORT,
};

enum HqStandardExceptionEnum
{
	HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
	HQ_STANDARD_EXCEPTION_TYPE_ERROR,
	HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,

	HQ_STANDARD_EXCEPTION__COUNT,
};

enum HqExceptionSeverity
{
	HQ_EXCEPTION_SEVERITY_NORMAL,
	HQ_EXCEPTION_SEVERITY_FATAL,

	HQ_EXCEPTION_SEVERITY__COUNT,
};

typedef struct HqVm* HqVmHandle;
typedef struct HqProgram* HqProgramHandle;
typedef struct HqFunction* HqFunctionHandle;
typedef struct HqExecution* HqExecutionHandle;
typedef struct HqFrame* HqFrameHandle;
typedef struct HqValue* HqValueHandle;

typedef void (*HqNativeFunction)(HqExecutionHandle, HqFunctionHandle, void*);

typedef void (*HqCallbackProgramDependency)(void*, const char*);
typedef void (*HqCallbackOpDisasm)(void*, const char*, uintptr_t);

typedef void (*HqCallbackNativeValueCopy)(void**, const void*);
typedef void (*HqCallbackNativeValueDestruct)(void*);
typedef bool (*HqCallbackNativeValueEqual)(const void*, const void*);
typedef bool (*HqCallbackNativeValueLessThan)(const void*, const void*);

typedef bool (*HqCallbackIterateFrame)(void*, HqFrameHandle);
typedef bool (*HqCallbackIterateProgram)(void*, HqProgramHandle);
typedef bool (*HqCallbackIterateFunction)(void*, HqFunctionHandle);
typedef bool (*HqCallbackIterateVariable)(void*, const char*, HqValueHandle);
typedef bool (*HqCallbackIterateString)(void*, const char*);
typedef bool (*HqCallbackIterateObjectMember)(void*, const char*, int);

typedef struct
{
	HqCallbackProgramDependency onRequestFn;
	void* pUserData;
} HqDependencyInit;

typedef struct
{
	HqCommonInit common;

	uint32_t gcThreadStackSize;
	uint32_t gcMaxIterationCount;
} HqVmInit;

#define HQ_VM_HANDLE_NULL        ((HqVmHandle)0)
#define HQ_PROGRAM_HANDLE_NULL   ((HqProgramHandle)0)
#define HQ_FUNCTION_HANDLE_NULL  ((HqFunctionHandle)0)
#define HQ_EXECUTION_HANDLE_NULL ((HqExecutionHandle)0)
#define HQ_FRAME_HANDLE_NULL     ((HqFrameHandle)0)
#define HQ_VALUE_HANDLE_NULL     ((HqValueHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqVmCreate(HqVmHandle* phOutVm, HqVmInit init);

HQ_MAIN_API int HqVmDispose(HqVmHandle* phVm);

HQ_MAIN_API int HqVmGetReportHandle(HqVmHandle hVm, HqReportHandle* phOutReport);

HQ_MAIN_API int HqVmGetProgram(HqVmHandle hVm, HqProgramHandle* phOutProgram, const char* programName);

HQ_MAIN_API int HqVmGetProgramCount(HqVmHandle hVm, size_t* pOutCount);

HQ_MAIN_API int HqVmListPrograms(HqVmHandle hVm, HqCallbackIterateProgram onIterateFn, void* pUserData);

HQ_MAIN_API int HqVmGetFunction(HqVmHandle hVm, HqFunctionHandle* phOutFunction, const char* signature);

HQ_MAIN_API int HqVmGetFunctionCount(HqVmHandle hVm, size_t* pOutCount);

HQ_MAIN_API int HqVmListFunctions(HqVmHandle hVm, HqCallbackIterateFunction onIterateFn, void* pUserData);

HQ_MAIN_API int HqVmSetGlobalVariable(HqVmHandle hVm, HqValueHandle hValue, const char* variableName);

HQ_MAIN_API int HqVmGetGlobalVariable(HqVmHandle hVm, HqValueHandle* phOutValue, const char* variableName);

HQ_MAIN_API int HqVmGetGlobalVariableCount(HqVmHandle hVm, size_t* pOutCount);

HQ_MAIN_API int HqVmListGlobalVariables(HqVmHandle hVm, HqCallbackIterateVariable onIterateFn, void* pUserData);

HQ_MAIN_API int HqVmListObjectSchemas(HqVmHandle hVm, HqCallbackIterateString onIterateFn, void* pUserData);

HQ_MAIN_API int HqVmLoadProgram(
	HqVmHandle hVm,
	const char* programName,
	const void* pProgramFileData,
	size_t programFileSize
);

HQ_MAIN_API int HqVmInitializePrograms(HqVmHandle hVm, HqExecutionHandle* phOutExecution);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqProgramGetVm(HqProgramHandle hProgram, HqVmHandle* phOutVm);

HQ_MAIN_API int HqProgramGetName(HqProgramHandle hProgram, const char** pOutName);

HQ_MAIN_API int HqProgramGetEndianness(HqProgramHandle hProgram, int* pOutEndianness);

HQ_MAIN_API int HqProgramGetFunctionCount(HqProgramHandle hProgram, size_t* pOutCount);

HQ_MAIN_API int HqProgramListFunctions(
	HqProgramHandle hProgram,
	HqCallbackIterateString onIterateFn,
	void* pUserData
);

HQ_MAIN_API int HqProgramGetGlobalVariableCount(HqProgramHandle hProgram, size_t* pOutCount);

HQ_MAIN_API int HqProgramListGlobalVariables(
	HqProgramHandle hProgram,
	HqCallbackIterateString onIterateFn,
	void* pUserData
);

HQ_MAIN_API int HqProgramListDependencies(
	HqProgramHandle hProgram,
	HqCallbackIterateString onIterateFn,
	void* pUserData
);

HQ_MAIN_API int HqProgramListUnloadedDependencies(
	HqProgramHandle hProgram,
	HqCallbackIterateString onIterateFn,
	void* pUserData
);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqFunctionGetProgram(HqFunctionHandle hFunction, HqProgramHandle* phOutProgram);

HQ_MAIN_API int HqFunctionGetSignature(HqFunctionHandle hFunction, const char** pOutSignature);

HQ_MAIN_API int HqFunctionGetIsNative(HqFunctionHandle hFunction, bool* pOutNative);

HQ_MAIN_API int HqFunctionGetParameterCount(HqFunctionHandle hFunction, uint16_t* pOutCount);

HQ_MAIN_API int HqFunctionGetReturnValueCount(HqFunctionHandle hFunction, uint16_t* pOutCount);

HQ_MAIN_API int HqFunctionGetBytecodeOffset(HqFunctionHandle hFunction, uint32_t* pOutOffset);

HQ_MAIN_API int HqFunctionGetNativeBinding(HqFunctionHandle hFunction, HqNativeFunction* pOutNativeFn);

HQ_MAIN_API int HqFunctionGetNativeUserData(HqFunctionHandle hFunction, void** ppOutUerData);

HQ_MAIN_API int HqFunctionSetNativeBinding(
	HqFunctionHandle hFunction,
	HqNativeFunction nativeFn,
	void* pUserData
);

HQ_MAIN_API int HqFunctionDisassemble(
	HqFunctionHandle hFunction,
	HqCallbackOpDisasm onDisasmFn,
	void* pUserData
);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqExecutionCreate(
	HqExecutionHandle* phOutExecution,
	HqVmHandle hVm,
	HqFunctionHandle hEntryPoint
);

HQ_MAIN_API int HqExecutionDispose(HqExecutionHandle* phExecution);

HQ_MAIN_API int HqExecutionRun(HqExecutionHandle hExec, int runMode);

HQ_MAIN_API int HqExecutionYield(HqExecutionHandle hExec);

HQ_MAIN_API int HqExecutionRaiseStandardException(
	HqExecutionHandle hExec,
	int severity,
	int exceptionType,
	const char* message,
	...
);

HQ_MAIN_API int HqExecutionRaiseException(HqExecutionHandle hExec, HqValueHandle hValue, int severity);

HQ_MAIN_API int HqExecutionGetVm(HqExecutionHandle hExec, HqVmHandle* phOutVm);

HQ_MAIN_API int HqExecutionGetStatus(HqExecutionHandle hExec, int statusType, bool* pOutStatus);

HQ_MAIN_API int HqExecutionHasUnhandledExceptionOccurred(HqExecutionHandle hExec, bool* pOutException);

HQ_MAIN_API int HqExecutionGetFrameStackDepth(HqExecutionHandle hExec, size_t* pOutDepth);

HQ_MAIN_API int HqExecutionResolveFrameStack(
	HqExecutionHandle hExec,
	HqCallbackIterateFrame onIterateFn,
	void* pUserData
);

HQ_MAIN_API int HqExecutionGetCurrentFrame(HqExecutionHandle hExec, HqFrameHandle* phOutFrame);

HQ_MAIN_API int HqExecutionSetIoRegister(HqExecutionHandle hExec, HqValueHandle hValue, int registerIndex);

HQ_MAIN_API int HqExecutionGetIoRegister(HqExecutionHandle hExec, HqValueHandle* phOutValue, int registerIndex);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqFrameGetFunction(HqFrameHandle hFrame, HqFunctionHandle* phOutFunction);

HQ_MAIN_API int HqFrameGetBytecodeOffset(HqFrameHandle hFrame, uint32_t* pOutOffset);

HQ_MAIN_API int HqFramePushValue(HqFrameHandle hFrame, HqValueHandle hValue);

HQ_MAIN_API int HqFramePopValue(HqFrameHandle hFrame, HqValueHandle* phOutValue);

HQ_MAIN_API int HqFramePeekValue(HqFrameHandle hFrame, HqValueHandle* phOutValue, int stackIndex);

HQ_MAIN_API int HqFrameSetGpRegister(HqFrameHandle hFrame, HqValueHandle hValue, int registerIndex);

HQ_MAIN_API int HqFrameGetGpRegister(HqFrameHandle hFrame, HqValueHandle* phOutValue, int registerIndex);

HQ_MAIN_API int HqFrameSetLocalVariable(HqFrameHandle hFrame, HqValueHandle hValue, const char* variableName);

HQ_MAIN_API int HqFrameGetLocalVariable(HqFrameHandle hFrame, HqValueHandle* phOutValue, const char* variableName);

HQ_MAIN_API int HqFrameGetLocalVariableCount(HqFrameHandle hFrame, size_t* pOutCount);

HQ_MAIN_API int HqFrameListLocalVariables(HqFrameHandle hFrame, HqCallbackIterateVariable onIterateFn, void* pUserData);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API HqValueHandle HqValueCreateBool(HqVmHandle hVm, bool value);

HQ_MAIN_API HqValueHandle HqValueCreateInt8(HqVmHandle hVm, int8_t value);

HQ_MAIN_API HqValueHandle HqValueCreateInt16(HqVmHandle hVm, int16_t value);

HQ_MAIN_API HqValueHandle HqValueCreateInt32(HqVmHandle hVm, int32_t value);

HQ_MAIN_API HqValueHandle HqValueCreateInt64(HqVmHandle hVm, int64_t value);

HQ_MAIN_API HqValueHandle HqValueCreateUint8(HqVmHandle hVm, uint8_t value);

HQ_MAIN_API HqValueHandle HqValueCreateUint16(HqVmHandle hVm, uint16_t value);

HQ_MAIN_API HqValueHandle HqValueCreateUint32(HqVmHandle hVm, uint32_t value);

HQ_MAIN_API HqValueHandle HqValueCreateUint64(HqVmHandle hVm, uint64_t value);

HQ_MAIN_API HqValueHandle HqValueCreateFloat32(HqVmHandle hVm, float value);

HQ_MAIN_API HqValueHandle HqValueCreateFloat64(HqVmHandle hVm, double value);

HQ_MAIN_API HqValueHandle HqValueCreateNull();

HQ_MAIN_API HqValueHandle HqValueCreateString(HqVmHandle hVm, const char* const string);

HQ_MAIN_API HqValueHandle HqValueCreateFunction(HqVmHandle hVm, HqFunctionHandle hFunction);

HQ_MAIN_API HqValueHandle HqValueCreateObject(HqVmHandle hVm, const char* const typeName);

HQ_MAIN_API HqValueHandle HqValueCreateArray(HqVmHandle hVm, size_t count);

HQ_MAIN_API HqValueHandle HqValueCreateNative(
	HqVmHandle hVm,
	void* pNativeObject,
	HqCallbackNativeValueCopy onCopy,
	HqCallbackNativeValueDestruct onDestruct,
	HqCallbackNativeValueEqual onTestEqual,
	HqCallbackNativeValueLessThan onTestLessThan
);

HQ_MAIN_API HqValueHandle HqValueCopy(HqVmHandle hVm, HqValueHandle hValue);

HQ_MAIN_API int HqValueGcProtect(HqValueHandle hValue);

HQ_MAIN_API int HqValueGcExpose(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsPrimitiveType(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsBool(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsInt8(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsInt16(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsInt32(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsInt64(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsUint8(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsUint16(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsUint32(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsUint64(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsFloat32(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsFloat64(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsNull(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsString(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsFunction(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsObject(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsArray(HqValueHandle hValue);

HQ_MAIN_API bool HqValueIsNative(HqValueHandle hValue);

HQ_MAIN_API bool HqValueGetBool(HqValueHandle hValue);

HQ_MAIN_API int8_t HqValueGetInt8(HqValueHandle hValue);

HQ_MAIN_API int16_t HqValueGetInt16(HqValueHandle hValue);

HQ_MAIN_API int32_t HqValueGetInt32(HqValueHandle hValue);

HQ_MAIN_API int64_t HqValueGetInt64(HqValueHandle hValue);

HQ_MAIN_API uint8_t HqValueGetUint8(HqValueHandle hValue);

HQ_MAIN_API uint16_t HqValueGetUint16(HqValueHandle hValue);

HQ_MAIN_API uint32_t HqValueGetUint32(HqValueHandle hValue);

HQ_MAIN_API uint64_t HqValueGetUint64(HqValueHandle hValue);

HQ_MAIN_API float HqValueGetFloat32(HqValueHandle hValue);

HQ_MAIN_API double HqValueGetFloat64(HqValueHandle hValue);

HQ_MAIN_API const char* HqValueGetString(HqValueHandle hValue);

HQ_MAIN_API HqFunctionHandle HqValueGetFunction(HqValueHandle hValue);

HQ_MAIN_API void* HqValueGetNative(HqValueHandle hValue);

HQ_MAIN_API size_t HqValueGetStringLength(HqValueHandle hValue);

HQ_MAIN_API size_t HqValueGetStringHash(HqValueHandle hValue);

HQ_MAIN_API const char* HqValueGetObjectTypeName(HqValueHandle hValue);

HQ_MAIN_API size_t HqValueGetObjectMemberCount(HqValueHandle hValue);

HQ_MAIN_API HqValueHandle HqValueGetObjectMemberValue(HqValueHandle hValue, const char* memberName);

HQ_MAIN_API uint8_t HqValueGetObjectMemberType(HqValueHandle hValue, const char* memberName);

HQ_MAIN_API int HqValueSetObjectMemberValue(HqValueHandle hValue, const char* memberName, HqValueHandle hMemberValue);

HQ_MAIN_API int HqValueListObjectMembers(HqValueHandle hValue, HqCallbackIterateObjectMember onIterateFn, void* pUserData);

HQ_MAIN_API void* HqValueGetObjectUserData(HqValueHandle hValue);

HQ_MAIN_API int HqValueSetObjectUserData(HqValueHandle hValue, void* pUserData);

HQ_MAIN_API int HqValueGetArrayLength(HqValueHandle hValue, size_t* pOutLength);

HQ_MAIN_API int HqValueGetArrayElement(HqValueHandle hValue, size_t index, HqValueHandle* phOutElementValue);

HQ_MAIN_API int HqValueSetArrayElement(HqValueHandle hValue, size_t index, HqValueHandle hElementValue);

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* HQ_LIB_RUNTIME */

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef HQ_LIB_COMPILER

/*---------------------------------------------------------------------------------------------------------------------*/

typedef struct HqCompiler* HqCompilerHandle;
typedef struct HqProgramWriter* HqProgramWriterHandle;

typedef struct
{
	HqCommonInit common;
} HqCompilerInit;

typedef bool (*HqCallbackIterateBuiltInFunction)(void*, int, const char*);

#define HQ_COMPILER_HANDLE_NULL       ((HqCompilerHandle)0)
#define HQ_PROGRAM_WRITER_HANDLE_NULL ((HqProgramWriterHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqCompilerCreate(HqCompilerHandle* phOutCompiler, HqCompilerInit init);

HQ_MAIN_API int HqCompilerDispose(HqCompilerHandle* phCompiler);

HQ_MAIN_API int HqCompilerGetReportHandle(HqCompilerHandle hCompiler, HqReportHandle* phOutReport);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqProgramWriterCreate(HqProgramWriterHandle* phOutProgramWriter, HqCompilerHandle hCompiler);

HQ_MAIN_API int HqProgramWriterDispose(HqProgramWriterHandle* phProgramWriter);

HQ_MAIN_API int HqProgramWriterAddDependency(HqProgramWriterHandle hProgramWriter, const char* programName);

HQ_MAIN_API int HqProgramWriterAddConstantNull(HqProgramWriterHandle hProgramWriter, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantBool(HqProgramWriterHandle hProgramWriter, bool value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantInt8(HqProgramWriterHandle hProgramWriter, int8_t value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantInt16(HqProgramWriterHandle hProgramWriter, int16_t value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantInt32(HqProgramWriterHandle hProgramWriter, int32_t value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantInt64(HqProgramWriterHandle hProgramWriter, int64_t value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantUint8(HqProgramWriterHandle hProgramWriter, uint8_t value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantUint16(HqProgramWriterHandle hProgramWriter, uint16_t value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantUint32(HqProgramWriterHandle hProgramWriter, uint32_t value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantUint64(HqProgramWriterHandle hProgramWriter, uint64_t value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantFloat32(HqProgramWriterHandle hProgramWriter, float value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantFloat64(HqProgramWriterHandle hProgramWriter, double value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddConstantString(HqProgramWriterHandle hProgramWriter, const char* value, uint32_t* pOutIndex);

HQ_MAIN_API int HqProgramWriterAddObjectType(HqProgramWriterHandle hProgramWriter, const char* objectTypeName);

HQ_MAIN_API int HqProgramWriterAddObjectMember(
	HqProgramWriterHandle hProgramWriter,
	const char* objectTypeName,
	const char* memberName,
	uint8_t memberValueType,
	uint32_t* pOutIndex
);

HQ_MAIN_API int HqProgramWriterAddGlobal(HqProgramWriterHandle hProgramWriter, const char* variableName, uint32_t constantIndex);

HQ_MAIN_API int HqProgramWriterSetProgramInitFunction(
	HqProgramWriterHandle hProgramWriter,
	const void* pBytecode,
	size_t bytecodeLength
);

HQ_MAIN_API int HqProgramWriterAddFunction(
	HqProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	const void* pFunctionBytecode,
	size_t bytecodeLength,
	uint16_t numParameters,
	uint16_t numReturnValues
);

HQ_MAIN_API int HqProgramWriterAddNativeFunction(
	HqProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	uint16_t numParameters,
	uint16_t numReturnValues
);

HQ_MAIN_API int HqProgramWriterAddLocalVariable(
	HqProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	const char* variableName,
	uint32_t constantIndex
);

HQ_MAIN_API int HqProgramWriterAddGuardedBlock(
	HqProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	size_t bytecodeOffset,
	size_t bytecodeLength,
	uint32_t* pOutBlockId
);

HQ_MAIN_API int HqProgramWriterAddExceptionHandler(
	HqProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	uint32_t blockId,
	size_t bytecodeOffset,
	int handledType,
	const char* className
);

HQ_MAIN_API int HqProgramWriterSerialize(
	HqProgramWriterHandle hProgramWriter,
	HqCompilerHandle hCompiler,
	HqSerializerHandle hSerializer
);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqBytecodeWriteNop(HqSerializerHandle hSerializer);

HQ_MAIN_API int HqBytecodeWriteAbort(HqSerializerHandle hSerializer);

HQ_MAIN_API int HqBytecodeWriteReturn(HqSerializerHandle hSerializer);

HQ_MAIN_API int HqBytecodeWriteYield(HqSerializerHandle hSerializer);

HQ_MAIN_API int HqBytecodeWriteCall(HqSerializerHandle hSerializer, uint32_t constantIndex);

HQ_MAIN_API int HqBytecodeWriteCallValue(HqSerializerHandle hSerializer, uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWriteRaise(HqSerializerHandle hSerializer, uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWriteLoadConstant(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

HQ_MAIN_API int HqBytecodeWriteLoadGlobal(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

HQ_MAIN_API int HqBytecodeWriteLoadLocal(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

HQ_MAIN_API int HqBytecodeWriteLoadParam(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t ioRegIndex
);

HQ_MAIN_API int HqBytecodeWriteLoadObject(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t memberIndex
);

HQ_MAIN_API int HqBytecodeWriteLoadArray(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t arrayIndex
);

HQ_MAIN_API int HqBytecodeWriteStoreGlobal(
	HqSerializerHandle hSerializer,
	uint32_t constantIndex,
	uint32_t gpRegIndex
);

HQ_MAIN_API int HqBytecodeWriteStoreLocal(
	HqSerializerHandle hSerializer,
	uint32_t constantIndex,
	uint32_t gpRegIndex
);

HQ_MAIN_API int HqBytecodeWriteStoreParam(
	HqSerializerHandle hSerializer,
	uint32_t ioRegIndex,
	uint32_t gpRegIndex
);

HQ_MAIN_API int HqBytecodeWriteStoreObject(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t memberIndex
);

HQ_MAIN_API int HqBytecodeWriteStoreArray(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t arrayIndex
);

HQ_MAIN_API int HqBytecodeWritePullGlobal(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

HQ_MAIN_API int HqBytecodeWritePullLocal(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

HQ_MAIN_API int HqBytecodeWritePullParam(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t ioRegIndex
);

HQ_MAIN_API int HqBytecodeWritePullObject(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t memberIndex
);

HQ_MAIN_API int HqBytecodeWritePullArray(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex,
	uint32_t arrayIndex
);

HQ_MAIN_API int HqBytecodeWritePush(HqSerializerHandle hSerializer, uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWritePop(HqSerializerHandle hSerializer, uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWriteInitObject(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

HQ_MAIN_API int HqBytecodeWriteInitArray(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t initialCount
);

HQ_MAIN_API int HqBytecodeWriteInitFunction(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t constantIndex
);

HQ_MAIN_API int HqBytecodeWriteBranch(HqSerializerHandle hSerializer, int32_t offset);

HQ_MAIN_API int HqBytecodeWriteBranchIfTrue(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int32_t offset
);

HQ_MAIN_API int HqBytecodeWriteBranchIfFalse(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int32_t offset
);

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* HQ_LIB_COMPILER */

//----------------------------------------------------------------------------------------------------------------------

enum HqValueType
{
	HQ_VALUE_TYPE_NULL,

	HQ_VALUE_TYPE_INT8,
	HQ_VALUE_TYPE_INT16,
	HQ_VALUE_TYPE_INT32,
	HQ_VALUE_TYPE_INT64,

	HQ_VALUE_TYPE_UINT8,
	HQ_VALUE_TYPE_UINT16,
	HQ_VALUE_TYPE_UINT32,
	HQ_VALUE_TYPE_UINT64,

	HQ_VALUE_TYPE_FLOAT32,
	HQ_VALUE_TYPE_FLOAT64,

	HQ_VALUE_TYPE_BOOL,

	HQ_VALUE_TYPE_STRING,
	HQ_VALUE_TYPE_FUNCTION,
	HQ_VALUE_TYPE_OBJECT,
	HQ_VALUE_TYPE_ARRAY,
	HQ_VALUE_TYPE_NATIVE,

	HQ_VALUE_TYPE__MAX_VALUE = HQ_VALUE_TYPE_NATIVE,
};

/*---------------------------------------------------------------------------------------------------------------------*/

enum HqBuiltInEnum
{
	HQ_BUILT_IN_OP_ADD_BOOL,
	HQ_BUILT_IN_OP_ADD_INT8,
	HQ_BUILT_IN_OP_ADD_INT16,
	HQ_BUILT_IN_OP_ADD_INT32,
	HQ_BUILT_IN_OP_ADD_INT64,
	HQ_BUILT_IN_OP_ADD_UINT8,
	HQ_BUILT_IN_OP_ADD_UINT16,
	HQ_BUILT_IN_OP_ADD_UINT32,
	HQ_BUILT_IN_OP_ADD_UINT64,
	HQ_BUILT_IN_OP_ADD_FLOAT32,
	HQ_BUILT_IN_OP_ADD_FLOAT64,
	HQ_BUILT_IN_OP_ADD_STRING,

	HQ_BUILT_IN_OP_SUB_BOOL,
	HQ_BUILT_IN_OP_SUB_INT8,
	HQ_BUILT_IN_OP_SUB_INT16,
	HQ_BUILT_IN_OP_SUB_INT32,
	HQ_BUILT_IN_OP_SUB_INT64,
	HQ_BUILT_IN_OP_SUB_UINT8,
	HQ_BUILT_IN_OP_SUB_UINT16,
	HQ_BUILT_IN_OP_SUB_UINT32,
	HQ_BUILT_IN_OP_SUB_UINT64,
	HQ_BUILT_IN_OP_SUB_FLOAT32,
	HQ_BUILT_IN_OP_SUB_FLOAT64,

	HQ_BUILT_IN_OP_MUL_BOOL,
	HQ_BUILT_IN_OP_MUL_INT8,
	HQ_BUILT_IN_OP_MUL_INT16,
	HQ_BUILT_IN_OP_MUL_INT32,
	HQ_BUILT_IN_OP_MUL_INT64,
	HQ_BUILT_IN_OP_MUL_UINT8,
	HQ_BUILT_IN_OP_MUL_UINT16,
	HQ_BUILT_IN_OP_MUL_UINT32,
	HQ_BUILT_IN_OP_MUL_UINT64,
	HQ_BUILT_IN_OP_MUL_FLOAT32,
	HQ_BUILT_IN_OP_MUL_FLOAT64,

	HQ_BUILT_IN_OP_DIV_BOOL,
	HQ_BUILT_IN_OP_DIV_INT8,
	HQ_BUILT_IN_OP_DIV_INT16,
	HQ_BUILT_IN_OP_DIV_INT32,
	HQ_BUILT_IN_OP_DIV_INT64,
	HQ_BUILT_IN_OP_DIV_UINT8,
	HQ_BUILT_IN_OP_DIV_UINT16,
	HQ_BUILT_IN_OP_DIV_UINT32,
	HQ_BUILT_IN_OP_DIV_UINT64,
	HQ_BUILT_IN_OP_DIV_FLOAT32,
	HQ_BUILT_IN_OP_DIV_FLOAT64,

	HQ_BUILT_IN_OP_CAST_BOOL_TO_INT8,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_INT16,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_INT32,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_INT64,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_BOOL_TO_STRING,

	HQ_BUILT_IN_OP_CAST_INT8_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_INT8_TO_INT16,
	HQ_BUILT_IN_OP_CAST_INT8_TO_INT32,
	HQ_BUILT_IN_OP_CAST_INT8_TO_INT64,
	HQ_BUILT_IN_OP_CAST_INT8_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_INT8_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_INT8_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_INT8_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_INT8_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_INT8_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_INT8_TO_STRING,

	HQ_BUILT_IN_OP_CAST_INT16_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_INT16_TO_INT8,
	HQ_BUILT_IN_OP_CAST_INT16_TO_INT32,
	HQ_BUILT_IN_OP_CAST_INT16_TO_INT64,
	HQ_BUILT_IN_OP_CAST_INT16_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_INT16_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_INT16_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_INT16_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_INT16_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_INT16_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_INT16_TO_STRING,

	HQ_BUILT_IN_OP_CAST_INT32_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_INT32_TO_INT8,
	HQ_BUILT_IN_OP_CAST_INT32_TO_INT16,
	HQ_BUILT_IN_OP_CAST_INT32_TO_INT64,
	HQ_BUILT_IN_OP_CAST_INT32_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_INT32_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_INT32_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_INT32_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_INT32_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_INT32_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_INT32_TO_STRING,

	HQ_BUILT_IN_OP_CAST_INT64_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_INT64_TO_INT8,
	HQ_BUILT_IN_OP_CAST_INT64_TO_INT16,
	HQ_BUILT_IN_OP_CAST_INT64_TO_INT32,
	HQ_BUILT_IN_OP_CAST_INT64_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_INT64_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_INT64_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_INT64_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_INT64_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_INT64_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_INT64_TO_STRING,

	HQ_BUILT_IN_OP_CAST_UINT8_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_INT8,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_INT16,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_INT32,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_INT64,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_UINT8_TO_STRING,

	HQ_BUILT_IN_OP_CAST_UINT16_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_INT8,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_INT16,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_INT32,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_INT64,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_UINT16_TO_STRING,

	HQ_BUILT_IN_OP_CAST_UINT32_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_INT8,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_INT16,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_INT32,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_INT64,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_UINT32_TO_STRING,

	HQ_BUILT_IN_OP_CAST_UINT64_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_INT8,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_INT16,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_INT32,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_INT64,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_UINT64_TO_STRING,

	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_INT8,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_INT16,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_INT32,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_INT64,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_FLOAT64,
	HQ_BUILT_IN_OP_CAST_FLOAT32_TO_STRING,

	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_INT8,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_INT16,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_INT32,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_INT64,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_FLOAT64_TO_STRING,

	HQ_BUILT_IN_OP_CAST_STRING_TO_BOOL,
	HQ_BUILT_IN_OP_CAST_STRING_TO_INT8,
	HQ_BUILT_IN_OP_CAST_STRING_TO_INT16,
	HQ_BUILT_IN_OP_CAST_STRING_TO_INT32,
	HQ_BUILT_IN_OP_CAST_STRING_TO_INT64,
	HQ_BUILT_IN_OP_CAST_STRING_TO_UINT8,
	HQ_BUILT_IN_OP_CAST_STRING_TO_UINT32,
	HQ_BUILT_IN_OP_CAST_STRING_TO_UINT16,
	HQ_BUILT_IN_OP_CAST_STRING_TO_UINT64,
	HQ_BUILT_IN_OP_CAST_STRING_TO_FLOAT32,
	HQ_BUILT_IN_OP_CAST_STRING_TO_FLOAT64,

	HQ_BUILT_IN_OP_LEN_STRING,
	HQ_BUILT_IN_OP_LEN_ARRAY,

	HQ_BUILT_IN__TOTAL_COUNT,
	HQ_BUILT_IN__FOCE_DWORD = 0x7FFFFFFFul,
};

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* _HQ_SCRIPT_H_ */
