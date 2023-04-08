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

#ifndef _HARLEQUIN_H_
#define _HARLEQUIN_H_

/*---------------------------------------------------------------------------------------------------------------------*/

#include "HqBuildEnv.h"

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#define HQ_VERSION_MAJOR 0
#define HQ_VERSION_MINOR 0
#define HQ_VERSION_PATCH 0

/*---------------------------------------------------------------------------------------------------------------------*/

#define HQ_VM_FRAME_STACK_SIZE 1024
#define HQ_VM_VALUE_STACK_SIZE 64

#define HQ_VM_IO_REGISTER_COUNT 48
#define HQ_VM_GP_REGISTER_COUNT 64
#define HQ_VM_VR_REGISTER_COUNT 256

#define HQ_VM_THREAD_MINIMUM_STACK_SIZE 262144
#define HQ_VM_THREAD_DEFAULT_STACK_SIZE 1048576

#define HQ_VM_GC_DEFAULT_TIME_SLICE_MS 8
#define HQ_VM_GC_DEFAULT_TIME_WAIT_MS  3

/*---------------------------------------------------------------------------------------------------------------------*/

enum HqErrorCodeEnum
{
	HQ_SUCCESS = 0,

	HQ_ERROR_NOT_IMPLEMENTED = -99,
	HQ_ERROR_UNSPECIFIED_FAILURE = -1,
	HQ_ERROR_INVALID_ARG = -2,
	HQ_ERROR_INVALID_TYPE = -3,
	HQ_ERROR_INVALID_DATA = -4,
	HQ_ERROR_INVALID_RANGE = -5,
	HQ_ERROR_INVALID_OPERATION = -6,
	HQ_ERROR_BAD_ALLOCATION = -7,
	HQ_ERROR_DUPLICATE = -8,
	HQ_ERROR_NON_EXISTENT = -9,

	/**! Deprecated - use HQ_ERROR_DUPLICATE instead */
	HQ_ERROR_KEY_ALREADY_EXISTS = HQ_ERROR_DUPLICATE,
	/**! Deprecated - use HQ_ERROR_NON_EXISTENT instead */
	HQ_ERROR_KEY_DOES_NOT_EXIST = HQ_ERROR_NON_EXISTENT,

	HQ_ERROR_FAILED_TO_OPEN_FILE = -10,
	HQ_ERROR_STREAM_END = -11,
	HQ_ERROR_STACK_EMPTY = -12,
	HQ_ERROR_STACK_FULL = -13,
	HQ_ERROR_INDEX_OUT_OF_RANGE = -14,
	HQ_ERROR_SCRIPT_NO_FUNCTION = -15,
	HQ_ERROR_MISMATCH = -16,
	HQ_ERROR_UNKNOWN_ID = -17,
	HQ_ERROR_NO_WRITE = -18,
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

HQ_BASE_API const char* HqGetEndiannessString(int endianness);

HQ_BASE_API const char* HqGetValueTypeString(int valueType);

HQ_BASE_API int HqGetPlatformEndianness();

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

HQ_BASE_API int HqSerializerWriteBool8(HqSerializerHandle hSerializer, bool value);

HQ_BASE_API int HqSerializerWriteBool16(HqSerializerHandle hSerializer, bool value);

HQ_BASE_API int HqSerializerWriteBool32(HqSerializerHandle hSerializer, bool value);

HQ_BASE_API int HqSerializerWriteBool64(HqSerializerHandle hSerializer, bool value);

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

HQ_BASE_API int HqSerializerReadBool8(HqSerializerHandle hSerializer, bool* pOutValue);

HQ_BASE_API int HqSerializerReadBool16(HqSerializerHandle hSerializer, bool* pOutValue);

HQ_BASE_API int HqSerializerReadBool32(HqSerializerHandle hSerializer, bool* pOutValue);

HQ_BASE_API int HqSerializerReadBool64(HqSerializerHandle hSerializer, bool* pOutValue);

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

HQ_BASE_API uint64_t HqClockGetFrequency();

HQ_BASE_API uint64_t HqClockGetTimestamp();

/*---------------------------------------------------------------------------------------------------------------------*/

typedef struct
{
	uint16_t major;
	uint16_t minor;
	uint16_t patch;
} HqSysVersion;

typedef struct
{
	char name[32];
	char dllExt[8];

	int bits;
	char pathSep;
} HqSysPlatformInfo;

typedef void (*HqListDirectoryCallback)(void*, const char*, const char*);

typedef void* HqDllHandle;

#define HQ_DLL_HANDLE_NULL ((HqDllHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_BASE_API int HqSysGetVersion(HqSysVersion* pOutVersion);

HQ_BASE_API bool HqSysIsFile(const char* path);

HQ_BASE_API bool HqSysIsDirectory(const char* path);

HQ_BASE_API bool HqSysIsExecutable(const char* path);

HQ_BASE_API void HqSysListDirectory(
	void* pUserData,
	const char* rootPath,
	HqListDirectoryCallback onFileFound,
	HqListDirectoryCallback onDirFound
);

HQ_BASE_API int HqSysGetInfo(HqSysPlatformInfo* pOutInfo);

HQ_BASE_API int HqSysOpenLibrary(HqDllHandle* phOutDll, const char* dllPath);

HQ_BASE_API int HqSysCloseLibrary(HqDllHandle* phDll);

HQ_BASE_API void* HqSysGetSymbol(HqDllHandle hDll, const char* symbolName);

/*---------------------------------------------------------------------------------------------------------------------*/

#if HQ_LIB_RUNTIME

/*---------------------------------------------------------------------------------------------------------------------*/

enum HqRunModeEnum
{
	HQ_RUN_STEP,
	HQ_RUN_FULL,
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

typedef struct
{
	HqCommonInit common;

	uint32_t gcThreadStackSize;
	uint32_t gcTimeSliceMs;
	uint32_t gcTimeWaitMs;

	bool gcEnableThread;
} HqVmInit;

typedef struct
{
	HqSysVersion version;
} HqDllRuntimeInfo;

typedef struct HqVm* HqVmHandle;
typedef struct HqModule* HqModuleHandle;
typedef struct HqFunction* HqFunctionHandle;
typedef struct HqExecution* HqExecutionHandle;
typedef struct HqFrame* HqFrameHandle;
typedef struct HqValue* HqValueHandle;

typedef int (*HqDllEntryPoint)(const HqDllRuntimeInfo*, HqVmHandle);

typedef void (*HqNativeFunction)(HqExecutionHandle, HqFunctionHandle, void*);

typedef void (*HqCallbackOpDisasm)(void*, const char*, uintptr_t);

typedef void (*HqCallbackNativeValueCopy)(void**, void*);
typedef void (*HqCallbackNativeValueDestruct)(void*);

typedef bool (*HqCallbackIterateFrame)(void*, HqFrameHandle);
typedef bool (*HqCallbackIterateModule)(void*, HqModuleHandle);
typedef bool (*HqCallbackIterateFunction)(void*, HqFunctionHandle);
typedef bool (*HqCallbackIterateVariable)(void*, const char*, HqValueHandle);
typedef bool (*HqCallbackIterateString)(void*, const char*);
typedef bool (*HqCallbackIterateStringWithIndex)(void*, const char*, size_t);
typedef bool (*HqCallbackIterateObjectMember)(void*, const char*, int);

#define HQ_VM_HANDLE_NULL        ((HqVmHandle)0)
#define HQ_MODULE_HANDLE_NULL    ((HqModuleHandle)0)
#define HQ_FUNCTION_HANDLE_NULL  ((HqFunctionHandle)0)
#define HQ_EXECUTION_HANDLE_NULL ((HqExecutionHandle)0)
#define HQ_FRAME_HANDLE_NULL     ((HqFrameHandle)0)
#define HQ_VALUE_HANDLE_NULL     ((HqValueHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqVmCreate(HqVmHandle* phOutVm, HqVmInit init);

HQ_MAIN_API int HqVmDispose(HqVmHandle* phVm);

HQ_MAIN_API int HqVmRunGarbageCollector(HqVmHandle hVm, int runMode);

HQ_MAIN_API int HqVmGetReportHandle(HqVmHandle hVm, HqReportHandle* phOutReport);

HQ_MAIN_API int HqVmGetModule(HqVmHandle hVm, HqModuleHandle* phOutModule, const char* moduleName);

HQ_MAIN_API int HqVmGetModuleCount(HqVmHandle hVm, size_t* pOutCount);

HQ_MAIN_API int HqVmListModules(HqVmHandle hVm, HqCallbackIterateModule onIterateFn, void* pUserData);

HQ_MAIN_API int HqVmGetFunction(HqVmHandle hVm, HqFunctionHandle* phOutFunction, const char* signature);

HQ_MAIN_API int HqVmGetFunctionCount(HqVmHandle hVm, size_t* pOutCount);

HQ_MAIN_API int HqVmListFunctions(HqVmHandle hVm, HqCallbackIterateFunction onIterateFn, void* pUserData);

HQ_MAIN_API int HqVmSetGlobalVariable(HqVmHandle hVm, HqValueHandle hValue, const char* variableName);

HQ_MAIN_API int HqVmGetGlobalVariable(HqVmHandle hVm, HqValueHandle* phOutValue, const char* variableName);

HQ_MAIN_API int HqVmGetGlobalVariableCount(HqVmHandle hVm, size_t* pOutCount);

HQ_MAIN_API int HqVmListGlobalVariables(HqVmHandle hVm, HqCallbackIterateVariable onIterateFn, void* pUserData);

HQ_MAIN_API int HqVmListObjectSchemas(HqVmHandle hVm, HqCallbackIterateString onIterateFn, void* pUserData);

HQ_MAIN_API int HqVmLoadModule(
	HqVmHandle hVm,
	const char* moduleName,
	const void* pModuleFileData,
	size_t moduleFileSize
);

HQ_MAIN_API int HqVmInitializeModules(HqVmHandle hVm, HqExecutionHandle* phOutExec);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqModuleGetVm(HqModuleHandle hModule, HqVmHandle* phOutVm);

HQ_MAIN_API int HqModuleGetName(HqModuleHandle hModule, const char** pOutName);

HQ_MAIN_API int HqModuleGetFunctionCount(HqModuleHandle hModule, size_t* pOutCount);

HQ_MAIN_API int HqModuleListFunctions(
	HqModuleHandle hModule,
	HqCallbackIterateFunction onIterateFn,
	void* pUserData
);

HQ_MAIN_API int HqModuleGetStringCount(HqModuleHandle hModule, size_t* pOutCount);

HQ_MAIN_API int HqModuleListStrings(
	HqModuleHandle hModule,
	HqCallbackIterateStringWithIndex onIterateFn,
	void* pUserData
);

HQ_MAIN_API int HqModuleGetGlobalVariableCount(HqModuleHandle hModule, size_t* pOutCount);

HQ_MAIN_API int HqModuleListGlobalVariables(
	HqModuleHandle hModule,
	HqCallbackIterateString onIterateFn,
	void* pUserData
);

HQ_MAIN_API int HqModuleListDependencies(
	HqModuleHandle hModule,
	HqCallbackIterateString onIterateFn,
	void* pUserData
);

HQ_MAIN_API int HqModuleListUnloadedDependencies(
	HqModuleHandle hModule,
	HqCallbackIterateString onIterateFn,
	void* pUserData
);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqFunctionGetModule(HqFunctionHandle hFunction, HqModuleHandle* phOutModule);

HQ_MAIN_API int HqFunctionGetSignature(HqFunctionHandle hFunction, const char** pOutSignature);

HQ_MAIN_API int HqFunctionIsNative(HqFunctionHandle hFunction, bool* pOutNative);

HQ_MAIN_API int HqFunctionIsInit(HqFunctionHandle hFunction, bool* pOutInit);

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

HQ_MAIN_API int HqExecutionCreate(HqExecutionHandle* phOutExec, HqVmHandle hVm);

HQ_MAIN_API int HqExecutionDispose(HqExecutionHandle* phExec);

HQ_MAIN_API int HqExecutionInitialize(HqExecutionHandle hExec, HqFunctionHandle hEntryPoint);

HQ_MAIN_API int HqExecutionReset(HqExecutionHandle hExec);

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

HQ_MAIN_API int HqExecutionSetIoRegister(HqExecutionHandle hExec, HqValueHandle hValue, uint32_t registerIndex);

HQ_MAIN_API int HqExecutionGetIoRegister(HqExecutionHandle hExec, HqValueHandle* phOutValue, uint32_t registerIndex);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqFrameGetFunction(HqFrameHandle hFrame, HqFunctionHandle* phOutFunction);

HQ_MAIN_API int HqFrameGetBytecodeOffset(HqFrameHandle hFrame, uint32_t* pOutOffset);

HQ_MAIN_API int HqFramePushValue(HqFrameHandle hFrame, HqValueHandle hValue);

HQ_MAIN_API int HqFramePopValue(HqFrameHandle hFrame, HqValueHandle* phOutValue);

HQ_MAIN_API int HqFramePeekValue(HqFrameHandle hFrame, HqValueHandle* phOutValue, uint32_t stackIndex);

HQ_MAIN_API int HqFrameSetGpRegister(HqFrameHandle hFrame, HqValueHandle hValue, uint32_t registerIndex);

HQ_MAIN_API int HqFrameGetGpRegister(HqFrameHandle hFrame, HqValueHandle* phOutValue, uint32_t registerIndex);

HQ_MAIN_API int HqFrameSetVrRegister(HqFrameHandle hFrame, HqValueHandle hValue, uint32_t registerIndex);

HQ_MAIN_API int HqFrameGetVrRegister(HqFrameHandle hFrame, HqValueHandle* phOutValue, uint32_t registerIndex);

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

HQ_MAIN_API HqValueHandle HqValueCreateString(HqVmHandle hVm, const char* const string);

HQ_MAIN_API HqValueHandle HqValueCreateFunction(HqVmHandle hVm, HqFunctionHandle hFunction);

HQ_MAIN_API HqValueHandle HqValueCreateObject(HqVmHandle hVm, const char* const typeName);

HQ_MAIN_API HqValueHandle HqValueCreateArray(HqVmHandle hVm, size_t count);

HQ_MAIN_API HqValueHandle HqValueCreateNative(
	HqVmHandle hVm,
	void* pNativeObject,
	HqCallbackNativeValueCopy onCopy,
	HqCallbackNativeValueDestruct onDestruct
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

HQ_MAIN_API int HqValueGetObjectMemberType(HqValueHandle hValue, const char* memberName, uint8_t* pOutType);

HQ_MAIN_API int HqValueSetObjectMemberValue(HqValueHandle hValue, const char* memberName, HqValueHandle hMemberValue);

HQ_MAIN_API int HqValueListObjectMembers(HqValueHandle hValue, HqCallbackIterateObjectMember onIterateFn, void* pUserData);

HQ_MAIN_API void* HqValueGetObjectUserData(HqValueHandle hValue);

HQ_MAIN_API int HqValueSetObjectUserData(HqValueHandle hValue, void* pUserData);

HQ_MAIN_API size_t HqValueGetArrayLength(HqValueHandle hValue);

HQ_MAIN_API HqValueHandle HqValueGetArrayElement(HqValueHandle hValue, size_t index);

HQ_MAIN_API int HqValueSetArrayElement(HqValueHandle hValue, size_t index, HqValueHandle hElementValue);

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* HQ_LIB_RUNTIME */

/*---------------------------------------------------------------------------------------------------------------------*/

#ifdef HQ_LIB_TOOL

/*---------------------------------------------------------------------------------------------------------------------*/

typedef struct HqToolContext*   HqToolContextHandle;
typedef struct HqModuleWriter*  HqModuleWriterHandle;

typedef struct
{
	HqCommonInit common;
} HqToolContextInit;

#define HQ_TOOL_CONTEXT_HANDLE_NULL   ((HqToolContextHandle)0)
#define HQ_MODULE_WRITER_HANDLE_NULL  ((HqModuleWriterHandle)0)

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqToolContextCreate(HqToolContextHandle* phOutToolCtx, HqToolContextInit init);

HQ_MAIN_API int HqToolContextDispose(HqToolContextHandle* phToolCtx);

HQ_MAIN_API int HqToolContextGetReportHandle(HqToolContextHandle hToolCtx, HqReportHandle* phOutReport);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqModuleWriterCreate(HqModuleWriterHandle* phOutModuleWriter, HqToolContextHandle hToolContext);

HQ_MAIN_API int HqModuleWriterDispose(HqModuleWriterHandle* phModuleWriter);

HQ_MAIN_API int HqModuleWriterAddDependency(HqModuleWriterHandle hModuleWriter, const char* moduleName);

HQ_MAIN_API int HqModuleWriterAddString(HqModuleWriterHandle hModuleWriter, const char* value, uint32_t* pOutIndex);

HQ_MAIN_API int HqModuleWriterAddObjectType(HqModuleWriterHandle hModuleWriter, const char* objectTypeName);

HQ_MAIN_API int HqModuleWriterAddObjectMember(
	HqModuleWriterHandle hModuleWriter,
	const char* objectTypeName,
	const char* memberName,
	uint8_t memberValueType,
	uint32_t* pOutIndex
);

HQ_MAIN_API int HqModuleWriterAddGlobal(HqModuleWriterHandle hModuleWriter, const char* variableName);

HQ_MAIN_API int HqModuleWriterSetModuleInitFunction(
	HqModuleWriterHandle hModuleWriter,
	const void* pBytecode,
	size_t bytecodeLength
);

HQ_MAIN_API int HqModuleWriterAddFunction(
	HqModuleWriterHandle hModuleWriter,
	const char* functionSignature,
	const void* pFunctionBytecode,
	size_t bytecodeLength,
	uint16_t numParameters,
	uint16_t numReturnValues
);

HQ_MAIN_API int HqModuleWriterAddNativeFunction(
	HqModuleWriterHandle hModuleWriter,
	const char* functionSignature,
	uint16_t numParameters,
	uint16_t numReturnValues
);

HQ_MAIN_API int HqModuleWriterAddGuardedBlock(
	HqModuleWriterHandle hModuleWriter,
	const char* functionSignature,
	size_t bytecodeOffset,
	size_t bytecodeLength,
	uint32_t* pOutBlockId
);

HQ_MAIN_API int HqModuleWriterAddExceptionHandler(
	HqModuleWriterHandle hModuleWriter,
	const char* functionSignature,
	uint32_t blockId,
	size_t bytecodeOffset,
	int handledType,
	const char* className
);

HQ_MAIN_API int HqModuleWriterSerialize(
	HqModuleWriterHandle hModuleWriter,
	HqSerializerHandle hSerializer
);

/*---------------------------------------------------------------------------------------------------------------------*/

HQ_MAIN_API int HqBytecodeWriteNop(HqSerializerHandle hSerializer);

HQ_MAIN_API int HqBytecodeWriteAbort(HqSerializerHandle hSerializer);

HQ_MAIN_API int HqBytecodeWriteReturn(HqSerializerHandle hSerializer);

HQ_MAIN_API int HqBytecodeWriteYield(HqSerializerHandle hSerializer);

HQ_MAIN_API int HqBytecodeWriteCall(HqSerializerHandle hSerializer, uint32_t stringIndex);

HQ_MAIN_API int HqBytecodeWriteCallValue(HqSerializerHandle hSerializer, uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWriteRaise(HqSerializerHandle hSerializer, uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWriteLoadImmNull(HqSerializerHandle hSerializer,uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWriteLoadImmBool(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	bool value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmI8(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int8_t value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmI16(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int16_t value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmI32(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int32_t value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmI64(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int64_t value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmU8(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint8_t value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmU16(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint16_t value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmU32(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmU64(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint64_t value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmF32(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	float value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmF64(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	double value
);

HQ_MAIN_API int HqBytecodeWriteLoadImmStr(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t stringIndex
);

HQ_MAIN_API int HqBytecodeWriteLoadGlobal(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t stringIndex
);

HQ_MAIN_API int HqBytecodeWriteLoadParam(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t ioRegIndex
);

HQ_MAIN_API int HqBytecodeWriteLoadVariable(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t vrRegIndex
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
	uint32_t gpArrIdxRegIndex
);

HQ_MAIN_API int HqBytecodeWriteStoreGlobal(
	HqSerializerHandle hSerializer,
	uint32_t stringIndex,
	uint32_t gpRegIndex
);

HQ_MAIN_API int HqBytecodeWriteStoreParam(
	HqSerializerHandle hSerializer,
	uint32_t ioRegIndex,
	uint32_t gpRegIndex
);

HQ_MAIN_API int HqBytecodeWriteStoreVariable(
	HqSerializerHandle hSerializer,
	uint32_t vrRegIndex,
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
	uint32_t gpArrIdxRegIndex
);

HQ_MAIN_API int HqBytecodeWritePush(HqSerializerHandle hSerializer, uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWritePop(HqSerializerHandle hSerializer, uint32_t gpRegIndex);

HQ_MAIN_API int HqBytecodeWriteInitObject(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t stringIndex
);

HQ_MAIN_API int HqBytecodeWriteInitArray(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t initialCount
);

HQ_MAIN_API int HqBytecodeWriteInitFunction(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	uint32_t stringIndex
);

HQ_MAIN_API int HqBytecodeWriteJump(HqSerializerHandle hSerializer, int32_t offset);

HQ_MAIN_API int HqBytecodeWriteJumpIfTrue(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int32_t offset
);

HQ_MAIN_API int HqBytecodeWriteJumpIfFalse(
	HqSerializerHandle hSerializer,
	uint32_t gpRegIndex,
	int32_t offset
);

HQ_MAIN_API int HqBytecodeWriteLength(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteAdd(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteSub(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteMul(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteDiv(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteMod(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteExp(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteBitAnd(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteBitOr(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteBitXor(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteBitNot(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteLeftShift(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteRightShift(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteLeftRotate(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteRightRotate(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastInt8(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastInt16(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastInt32(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastInt64(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastUint8(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastUint16(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastUint32(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastUint64(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastFloat32(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastFloat64(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastBool(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCastString(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCompareEqual(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCompareNotEqual(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCompareLess(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCompareLessEqual(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCompareGreater(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCompareGreaterEqual(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
);

HQ_MAIN_API int HqBytecodeWriteTest(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteMove(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

HQ_MAIN_API int HqBytecodeWriteCopy(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
);

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* HQ_LIB_TOOL */

//----------------------------------------------------------------------------------------------------------------------

enum HqValueType
{
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

#ifdef __cplusplus
}
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* _HARLEQUIN_H_ */
