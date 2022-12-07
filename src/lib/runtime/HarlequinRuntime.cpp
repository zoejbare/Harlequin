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

#include "../Harlequin.h"

#include "../base/Mutex.hpp"
#include "../base/String.hpp"

#include "../common/OpCodeEnum.hpp"

#include "Execution.hpp"
#include "Program.hpp"
#include "ScriptObject.hpp"
#include "Vm.hpp"
#include "Value.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" {

//----------------------------------------------------------------------------------------------------------------------

int HqVmCreate(HqVmHandle* phOutVm, HqVmInit init)
{
	if(!phOutVm
		|| (*phOutVm)
		|| init.common.report.reportLevel < HQ_MESSAGE_TYPE_VERBOSE
		|| init.common.report.reportLevel > HQ_MESSAGE_TYPE_FATAL
		|| init.gcThreadStackSize < HQ_VM_THREAD_MINIMUM_STACK_SIZE
		|| init.gcMaxTimeSliceMs == 0)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqVm* const pVm = HqVm::Create(init);

	// The message has to be printed *after* creating the VM state since the state is used in this function.
	HqReportMessage(&pVm->report, HQ_MESSAGE_TYPE_VERBOSE, "Initializing Harlequin runtime");

	(*phOutVm) = pVm;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmDispose(HqVmHandle* phVm)
{
	if(!phVm || !(*phVm))
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqVmHandle hVm = (*phVm);

	HqReportMessage(&hVm->report, HQ_MESSAGE_TYPE_VERBOSE, "Releasing Harlequin runtime");
	HqVm::Dispose(hVm);

	(*phVm) = HQ_VM_HANDLE_NULL;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmGetReportHandle(HqVmHandle hVm, HqReportHandle* phOutReport)
{
	if(!hVm || !phOutReport || *phOutReport)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*phOutReport) = &hVm->report;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmGetProgram(HqVmHandle hVm, HqProgramHandle* phOutProgram, const char* programName)
{
	if(!hVm || !phOutProgram || *phOutProgram || !programName || programName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Create a string object since we need that to look up into the map.
	HqString* const pName = HqString::Create(programName);
	if(!pName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	int result;
	(*phOutProgram) = HqVm::GetProgram(hVm, pName, &result);

	// We no longer need the string object.
	HqString::Release(pName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmGetProgramCount(HqVmHandle hVm, size_t* pOutCount)
{
	if(!hVm || !pOutCount)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutCount) = HQ_MAP_FUNC_SIZE(hVm->programs);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmListPrograms(HqVmHandle hVm, HqCallbackIterateProgram onIterateFn, void* pUserData)
{
	if(!hVm || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Call the callback for each program we currently have loaded.
	for(auto& kv : hVm->programs)
	{
		if(!onIterateFn(pUserData, HQ_MAP_ITER_VALUE(kv)))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmGetFunction(HqVmHandle hVm, HqFunctionHandle* phOutFunction, const char* signature)
{
	if(!hVm
		|| !phOutFunction
		|| *phOutFunction
		|| !signature
		|| signature[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Create a string object since we need that to look up into the map.
	HqString* const pSig = HqString::Create(signature);
	if(!pSig)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	int result;
	(*phOutFunction) = HqVm::GetFunction(hVm, pSig, &result);

	// We no longer need the string object.
	HqString::Release(pSig);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmGetFunctionCount(HqVmHandle hVm, size_t* pOutCount)
{
	if(!hVm || !pOutCount)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutCount) = HQ_MAP_FUNC_SIZE(hVm->functions);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmListFunctions(HqVmHandle hVm, HqCallbackIterateFunction onIterateFn, void* pUserData)
{
	if(!hVm || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Call the callback for each function we currently have loaded.
	for(auto& kv : hVm->functions)
	{
		if(!onIterateFn(pUserData, HQ_MAP_ITER_VALUE(kv)))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmSetGlobalVariable(HqVmHandle hVm, HqValueHandle hValue, const char* variableName)
{
	if(!hVm || !variableName || variableName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(!hValue)
	{
		hValue = HqValue::CreateNull();
	}

	if(hValue->type != HQ_VALUE_TYPE_NULL && hVm != hValue->hVm)
	{
		return HQ_ERROR_MISMATCH;
	}

	// Create a string object to lookup into the map.
	HqString* const pVariableName = HqString::Create(variableName);
	if(!pVariableName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	HqScopedReadLock gcLock(hVm->gc.rwLock);

	const int result = HqVm::SetGlobalVariable(hVm, hValue, pVariableName);

	// Dispose of the string object since we no longer need it.
	HqString::Release(pVariableName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmGetGlobalVariable(HqVmHandle hVm, HqValueHandle* phOutValue, const char* variableName)
{
	if(!hVm
		|| !phOutValue
		|| !variableName
		|| variableName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Create a string object in order to look up into the global map.
	HqString* const pGlobalName = HqString::Create(variableName);
	if(!pGlobalName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	int result;
	(*phOutValue) = HqVm::GetGlobalVariable(hVm, pGlobalName, &result);

	HqString::Release(pGlobalName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmGetGlobalVariableCount(HqVmHandle hVm, size_t* pOutCount)
{
	if(!hVm || !pOutCount)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutCount) = HQ_MAP_FUNC_SIZE(hVm->globals);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmListGlobalVariables(HqVmHandle hVm, HqCallbackIterateVariable onIterateFn, void* pUserData)
{
	if(!hVm || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Call the callback for each global variable we currently have loaded.
	for(auto& kv : hVm->globals)
	{
		HqString* const pGlobalName = HQ_MAP_ITER_KEY(kv);
		HqValueHandle hValue = HQ_MAP_ITER_VALUE(kv);

		if(!onIterateFn(pUserData, pGlobalName->data, hValue))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmListObjectSchemas(HqVmHandle hVm, HqCallbackIterateString onIterateFn, void* pUserData)
{
	if(!hVm || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Call the callback for each object schema we currently have loaded.
	for(auto& kv : hVm->objectSchemas)
	{
		HqString* const pObjectTypeName = HQ_MAP_ITER_KEY(kv);

		if(!onIterateFn(pUserData, pObjectTypeName->data))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmLoadProgram(
	HqVmHandle hVm,
	const char* const programName,
	const void* const pProgramFileData,
	const size_t programFileSize
)
{
	if(!hVm || !programName || programName[0] == '\0' || !pProgramFileData || programFileSize == 0)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Create a string to be the key in the program map.
	HqString* const pProgramName = HqString::Create(programName);
	if(!pProgramName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Check if a program with this name has already been loaded.
	if(HQ_MAP_FUNC_CONTAINS(hVm->programs, pProgramName))
	{
		HqString::Release(pProgramName);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	// Attempt to load the program file.
	HqProgramHandle hProgram = HqProgram::Create(hVm, pProgramName, pProgramFileData, programFileSize);
	if(!hProgram)
	{
		HqString::Release(pProgramName);
		return HQ_ERROR_FAILED_TO_OPEN_FILE;
	}

	// Map the program inside the VM state.
	HQ_MAP_FUNC_INSERT(hVm->programs, pProgramName, hProgram);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVmInitializePrograms(HqVmHandle hVm, HqExecutionHandle* phOutExecution)
{
	if(!hVm || !phOutExecution || (*phOutExecution) != HQ_EXECUTION_HANDLE_NULL)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	for(auto& kv : hVm->programs)
	{
		HqProgramHandle hProgram = HQ_MAP_ITER_VALUE(kv);

		if(hProgram->hInitFunction)
		{
			// Create an execution context for running the program's initialize function.
			const int execCreateResult = HqExecutionCreate(phOutExecution, hVm, hProgram->hInitFunction);
			assert(execCreateResult == HQ_SUCCESS); (void) execCreateResult;

			// Run the initializer function to completion.
			const int runResult = HqExecutionRun(*phOutExecution, HQ_RUN_CONTINUOUS);
			assert(runResult == HQ_SUCCESS); (void) runResult;

			// Check for any unhandled exceptions that occurred when running the initializer.
			bool exception = false;
			HqExecutionHasUnhandledExceptionOccurred(*phOutExecution, &exception);

			if(exception)
			{
				// When script exceptions do occur, return immediately to allow the user code to
				// query the execution context for more details on what went wrong.
				break;
			}

			// We're done with the execution context.
			HqExecutionDispose(phOutExecution);

			// Now that we've called the program's initializer function, we won't be calling it again,
			// so we can dispose of it.
			HqFunction::Dispose(hProgram->hInitFunction);
			hProgram->hInitFunction = HQ_FUNCTION_HANDLE_NULL;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramGetVm(HqProgramHandle hProgram, HqVmHandle* phOutVm)
{
	if(!hProgram || !phOutVm)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*phOutVm) = hProgram->hVm;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramGetName(HqProgramHandle hProgram, const char** pOutName)
{
	if(!hProgram || !pOutName)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutName) = hProgram->pName->data;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramGetEndianness(HqProgramHandle hProgram, int* pOutEndianness)
{
	if(!hProgram || !pOutEndianness)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutEndianness) = hProgram->endianness;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramGetFunctionCount(HqProgramHandle hProgram, size_t* pOutCount)
{
	if(!hProgram || !pOutCount)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutCount) = HQ_MAP_FUNC_SIZE(hProgram->functions);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramListFunctions(HqProgramHandle hProgram, HqCallbackIterateString onIterateFn, void* const pUserData)
{
	if(!hProgram || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Call the callback for each function signature in the program.
	for(auto& kv : hProgram->functions)
	{
		HqString* const pFunctionSignature = HQ_MAP_ITER_KEY(kv);

		if(!onIterateFn(pUserData, pFunctionSignature->data))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramGetGlobalVariableCount(HqProgramHandle hProgram, size_t* pOutCount)
{
	if(!hProgram || !pOutCount)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutCount) = HQ_MAP_FUNC_SIZE(hProgram->globals);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramListGlobalVariables(
	HqProgramHandle hProgram,
	HqCallbackIterateString onIterateFn,
	void* const pUserData
)
{
	if(!hProgram || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Call the callback for each global variable name in the program.
	for(auto& kv : hProgram->globals)
	{
		HqString* const pGlobalName = HQ_MAP_ITER_KEY(kv);

		if(!onIterateFn(pUserData, pGlobalName->data))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramListDependencies(HqProgramHandle hProgram, HqCallbackIterateString onIterateFn, void* const pUserData)
{
	if(!hProgram || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Call the callback for each dependency name referenced by the program.
	for(auto& kv : hProgram->dependencies)
	{
		HqString* const pDependencyName = HQ_MAP_ITER_KEY(kv);

		if(!onIterateFn(pUserData, pDependencyName->data))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramListUnloadedDependencies(
	HqProgramHandle hProgram,
	HqCallbackIterateString onIterateFn,
	void* const pUserData
)
{
	if(!hProgram || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Call the callback for only the dependency names referenced by the program that have not been loaded.
	for(auto& kv : hProgram->dependencies)
	{
		HqString* const pDependencyName = HQ_MAP_ITER_KEY(kv);

		// Check the VM to see if the dependent program has been loaded.
		if(!HQ_MAP_FUNC_CONTAINS(hProgram->hVm->programs, pDependencyName))
		{
			if(!onIterateFn(pUserData, pDependencyName->data))
			{
				break;
			}
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionGetProgram(HqFunctionHandle hFunction, HqProgramHandle* phOutProgram)
{
	if(!hFunction || !phOutProgram)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*phOutProgram) = hFunction->hProgram;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionGetSignature(HqFunctionHandle hFunction, const char** pOutSignature)
{
	if(!hFunction || !pOutSignature)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutSignature) = hFunction->pSignature->data;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionGetIsNative(HqFunctionHandle hFunction, bool* pOutNative)
{
	if(!hFunction || !pOutNative)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutNative) = hFunction->isNative;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionGetParameterCount(HqFunctionHandle hFunction, uint16_t* pOutCount)
{
	if(!hFunction || !pOutCount)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutCount) = hFunction->numParameters;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionGetReturnValueCount(HqFunctionHandle hFunction, uint16_t* pOutCount)
{
	if(!hFunction || !pOutCount)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutCount) = hFunction->numReturnValues;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionGetBytecodeOffset(HqFunctionHandle hFunction, uint32_t* pOutOffset)
{
	if(!hFunction || !pOutOffset)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	(*pOutOffset) = hFunction->bytecodeOffsetStart;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionGetNativeBinding(HqFunctionHandle hFunction, HqNativeFunction* pOutNativeFn)
{
	if(!hFunction || !pOutNativeFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(!hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	(*pOutNativeFn) = hFunction->nativeFn;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionGetNativeUserData(HqFunctionHandle hFunction, void** ppOutUserData)
{
	if(!hFunction || !ppOutUserData)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(!hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	(*ppOutUserData) = hFunction->pNativeUserData;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionSetNativeBinding(HqFunctionHandle hFunction, HqNativeFunction nativeFn, void* pUserData)
{
	if(!hFunction || !nativeFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(!hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	hFunction->nativeFn = nativeFn;
	hFunction->pNativeUserData = pUserData;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFunctionDisassemble(HqFunctionHandle hFunction, HqCallbackOpDisasm onDisasmFn, void* pUserData)
{
	if(!hFunction || !onDisasmFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	HqDisassemble disasm;

	disasm.hProgram = hFunction->hProgram;
	disasm.onDisasmFn = onDisasmFn;
	disasm.pUserData = pUserData;

	HqDecoder::Initialize(disasm.decoder, hFunction->hProgram, hFunction->bytecodeOffsetStart);

	// Iterate through each instruction.
	for(;;)
	{
		const uintptr_t offset = uintptr_t(disasm.decoder.ip - hFunction->hProgram->code.pData);
		const uint8_t opCode = HqDecoder::LoadUint8(disasm.decoder);

		disasm.opcodeOffset = offset;

		HqVm::DisassembleOpCode(disasm.hProgram->hVm, disasm, opCode);

		if(opCode == HQ_OP_CODE_RETURN)
		{
			// The RETURN opcode indicates the end of the function.
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionCreate(
	HqExecutionHandle* phOutExecution,
	HqVmHandle hVm,
	HqFunctionHandle hEntryPoint
)
{
	if(!phOutExecution
		|| (*phOutExecution)
		|| !hVm
		|| !hEntryPoint)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Create a new execution context with the provided script function as the entry point.
	HqExecutionHandle hExec = HqExecution::Create(hVm, hEntryPoint);
	if(!hExec)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	(*phOutExecution) = hExec;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionDispose(HqExecutionHandle* phExecution)
{
	if(!phExecution || !(*phExecution))
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqExecutionHandle hExec = (*phExecution);

	HqExecution::DetachFromVm(hExec);

	(*phExecution) = HQ_EXECUTION_HANDLE_NULL;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionRun(HqExecutionHandle hExec, int runMode)
{
	if(!hExec || runMode < HQ_RUN_STEP || runMode > HQ_RUN_CONTINUOUS)
	{
		return HQ_ERROR_INVALID_ARG;
	}
	else if(!hExec->hCurrentFrame)
	{
		return HQ_ERROR_SCRIPT_NO_FUNCTION;
	}

	HqExecution::Run(hExec, runMode);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionYield(HqExecutionHandle hExec)
{
	if(!hExec)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	hExec->yield = true;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionRaiseStandardException(
	HqExecutionHandle hExec,
	const int severity,
	const int exceptionType,
	const char* const message,
	...
)
{
	if(!hExec
		|| severity < 0
		|| severity >= HQ_EXCEPTION_SEVERITY__COUNT
		|| exceptionType < 0
		|| exceptionType >= HQ_STANDARD_EXCEPTION__COUNT)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	const char* formattedMessage = nullptr;
	{
		va_list vl;

		// Resolve the formatted args to a va_list.
		va_start(vl, message);
		formattedMessage = HqString::RawFormatVarArgs(message, vl);
		va_end(vl);
	}

	if(!formattedMessage)
	{
		// Failed to allocate the formatted message string.
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Lock the GC thread since we need to create a value for the exception and handle it in the execution context.
	HqScopedReadLock gcLock(hExec->hVm->gc.rwLock);

	HqValueHandle hException = HqVm::CreateStandardException(
		hExec->hVm,
		exceptionType,
		formattedMessage
	);

	// Free the formatted message string now that we're done with it.
	HqMemFree((void*)(formattedMessage));

	HqExecution::RaiseException(hExec, hException, severity);
	HqValue::SetAutoMark(hException, false);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionRaiseException(HqExecutionHandle hExec, HqValueHandle hValue, int severity)
{
	if(!hExec || severity < 0 || severity >= HQ_EXCEPTION_SEVERITY__COUNT)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Lock the GC thread since we need to handle the exception in the execution context.
	HqScopedReadLock gcLock(hExec->hVm->gc.rwLock);

	HqExecution::RaiseException(hExec, hValue, severity);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionGetVm(HqExecutionHandle hExec, HqVmHandle* phOutVm)
{
	if(!hExec || !phOutVm)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*phOutVm) = hExec->hVm;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionGetStatus(HqExecutionHandle hExec, int statusType, bool* pOutStatus)
{
	if(!hExec || !pOutStatus)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	switch(statusType)
	{
		case HQ_EXEC_STATUS_YIELD:
			(*pOutStatus) = hExec->yield;
			break;

		case HQ_EXEC_STATUS_RUNNING:
			(*pOutStatus) = (hExec->started && !hExec->finished) || hExec->exception;
			break;

		case HQ_EXEC_STATUS_COMPLETE:
			(*pOutStatus) = hExec->finished || hExec->exception;
			break;

		case HQ_EXEC_STATUS_EXCEPTION:
			(*pOutStatus) = hExec->exception;
			break;

		case HQ_EXEC_STATUS_ABORT:
			(*pOutStatus) = hExec->abort;
			break;

		default:
			return HQ_ERROR_INVALID_TYPE;
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionHasUnhandledExceptionOccurred(HqExecutionHandle hExec, bool* pOutException)
{
	if(!hExec || !pOutException)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutException) = hExec->exception;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionGetFrameStackDepth(HqExecutionHandle hExec, size_t* pOutDepth)
{
	if(!hExec || !pOutDepth)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutDepth) = hExec->frameStack.nextIndex;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionResolveFrameStack(
	HqExecutionHandle hExec,
	HqCallbackIterateFrame onIterateFn,
	void* pUserData
)
{
	if(!hExec || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	for(size_t i = 0; i < hExec->frameStack.nextIndex; ++i)
	{
		// Traverse the frame stack in reverse.
		HqFrameHandle hFrame = hExec->frameStack.memory.pData[hExec->frameStack.nextIndex - i - 1];

		if(!onIterateFn(pUserData, hFrame))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionGetCurrentFrame(HqExecutionHandle hExec, HqFrameHandle* phOutFrame)
{
	if(!hExec || !phOutFrame || *phOutFrame)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*phOutFrame) = hExec->hCurrentFrame;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionSetIoRegister(HqExecutionHandle hExec, HqValueHandle hValue, int registerIndex)
{
	if(!hExec
		|| registerIndex < 0
		|| registerIndex >= HQ_VM_IO_REGISTER_COUNT)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(!hValue)
	{
		hValue = HqValue::CreateNull();
	}

	if(hValue->type != HQ_VALUE_TYPE_NULL
		&& hExec->hVm != hValue->hVm)
	{
		return HQ_ERROR_MISMATCH;
	}

	return HqExecution::SetIoRegister(hExec, hValue, registerIndex);
}

//----------------------------------------------------------------------------------------------------------------------

int HqExecutionGetIoRegister(HqExecutionHandle hExec, HqValueHandle* phOutValue, int registerIndex)
{
	if(!hExec
		|| !phOutValue
		|| registerIndex < 0
		|| registerIndex >= HQ_VM_IO_REGISTER_COUNT)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqScopedReadLock gcLock(hExec->hVm->gc.rwLock);

	int result;
	HqValueHandle hValue = HqExecution::GetIoRegister(hExec, registerIndex, &result);

	// Guard the value against being garbage collected.
	HqValue::SetAutoMark(hValue, true);

	(*phOutValue) = hValue;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrameGetFunction(HqFrameHandle hFrame, HqFunctionHandle* phOutFunction)
{
	if(!hFrame || !phOutFunction || (*phOutFunction))
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*phOutFunction) = hFrame->hFunction;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrameGetBytecodeOffset(HqFrameHandle hFrame, uint32_t* pOutOffset)
{
	if(!hFrame || !pOutOffset)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	(*pOutOffset) = uint32_t(hFrame->decoder.cachedIp - hFrame->hFunction->hProgram->code.pData);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFramePushValue(HqFrameHandle hFrame, HqValueHandle hValue)
{
	if(!hFrame)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	if(!hValue)
	{
		hValue = HqValue::CreateNull();
	}

	if(hValue->type != HQ_VALUE_TYPE_NULL
		&& HqFunction::GetVm(hFrame->hFunction) != hValue->hVm)
	{
		return HQ_ERROR_MISMATCH;
	}

	return HqFrame::PushValue(hFrame, hValue);
}

//----------------------------------------------------------------------------------------------------------------------

int HqFramePopValue(HqFrameHandle hFrame, HqValueHandle* phOutValue)
{
	if(!hFrame)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
	int result = HqFrame::PopValue(hFrame, &hValue);

	if(phOutValue)
	{
		// Guard the value against being garbage collected.
		HqValue::SetAutoMark(hValue, true);

		(*phOutValue) = hValue;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFramePeekValue(HqFrameHandle hFrame, HqValueHandle* phOutValue, int stackIndex)
{
	if(!hFrame || !phOutValue || *phOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
	HqFrame::PeekValue(hFrame, &hValue, stackIndex);

	// Guard the value against being garbage collected.
	HqValue::SetAutoMark(hValue, true);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrameSetGpRegister(HqFrameHandle hFrame, HqValueHandle hValue, int registerIndex)
{
	if(!hFrame || registerIndex < 0 || registerIndex >= HQ_VM_GP_REGISTER_COUNT)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	if(!hValue)
	{
		hValue = HqValue::CreateNull();
	}

	HqVmHandle hVm = HqFunction::GetVm(hFrame->hFunction);
	if(!hVm)
	{
		return HQ_ERROR_INVALID_DATA;
	}

	if(hValue->type != HQ_VALUE_TYPE_NULL
		&& hVm != hValue->hVm)
	{
		return HQ_ERROR_MISMATCH;
	}

	HqScopedReadLock gcLock(hVm->gc.rwLock);

	return HqFrame::SetGpRegister(hFrame, hValue, registerIndex);
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrameGetGpRegister(HqFrameHandle hFrame, HqValueHandle* phOutValue, int registerIndex)
{
	if(!hFrame
		|| !phOutValue
		|| registerIndex < 0
		|| registerIndex >= HQ_VM_GP_REGISTER_COUNT)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	HqVmHandle hVm = HqFunction::GetVm(hFrame->hFunction);
	if(!hVm)
	{
		return HQ_ERROR_INVALID_DATA;
	}

	HqScopedReadLock gcLock(hVm->gc.rwLock);

	int result;
	HqValueHandle hValue = HqFrame::GetGpRegister(hFrame, registerIndex, &result);

	// Guard the value against being garbage collected.
	HqValue::SetAutoMark(hValue, true);

	(*phOutValue) = hValue;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrameSetLocalVariable(HqFrameHandle hFrame, HqValueHandle hValue, const char* variableName)
{
	if(!hFrame || !hValue || !variableName || variableName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	if(!hValue)
	{
		hValue = HqValue::CreateNull();
	}

	HqVmHandle hVm = HqFunction::GetVm(hFrame->hFunction);
	if(!hVm)
	{
		return HQ_ERROR_INVALID_DATA;
	}

	if(hValue->type != HQ_VALUE_TYPE_NULL
		&& hVm != hValue->hVm)
	{
		return HQ_ERROR_MISMATCH;
	}

	HqString* const pVarName = HqString::Create(variableName);
	if(!pVarName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	HqScopedReadLock gcLock(hVm->gc.rwLock);

	const int result = HqFrame::SetLocalVariable(hFrame, hValue, pVarName);

	HqString::Release(pVarName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrameGetLocalVariable(HqFrameHandle hFrame, HqValueHandle* phOutValue, const char* variableName)
{
	if(!hFrame || !phOutValue || !variableName || variableName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	// Create a string object to use for the map lookup.
	HqString* const pVarName = HqString::Create(variableName);
	if(!pVarName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	HqVmHandle hVm = HqFunction::GetVm(hFrame->hFunction);
	if(!hVm)
	{
		return HQ_ERROR_INVALID_DATA;
	}

	HqScopedReadLock gcLock(hVm->gc.rwLock);

	int result;
	HqValueHandle hValue = HqFrame::GetLocalVariable(hFrame, pVarName, &result);

	// Dispose of the string object now that we no longer need it.
	HqString::Release(pVarName);

	// Guard the value against being garbage collected.
	HqValue::SetAutoMark(hValue, true);

	(*phOutValue) = hValue;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrameGetLocalVariableCount(HqFrameHandle hFrame, size_t* pOutCount)
{
	if(!hFrame || !pOutCount)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	(*pOutCount) = HQ_MAP_FUNC_SIZE(hFrame->locals);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqFrameListLocalVariables(HqFrameHandle hFrame, HqCallbackIterateVariable onIterateFn, void* pUserData)
{
	if(!hFrame || !onIterateFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	for(auto& kv : hFrame->locals)
	{
		HqString* const pLocalName = HQ_MAP_ITER_KEY(kv);
		HqValueHandle hValue = HQ_MAP_ITER_VALUE(kv);

		if(!onIterateFn(pUserData, pLocalName->data, hValue))
		{
			break;
		}
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateBool(HqVmHandle hVm, bool value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateBool(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateInt8(HqVmHandle hVm, int8_t value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateInt8(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateInt16(HqVmHandle hVm, int16_t value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateInt16(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateInt32(HqVmHandle hVm, int32_t value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateInt32(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateInt64(HqVmHandle hVm, int64_t value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateInt64(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateUint8(HqVmHandle hVm, uint8_t value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateUint8(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateUint16(HqVmHandle hVm, uint16_t value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateUint16(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateUint32(HqVmHandle hVm, uint32_t value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateUint32(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateUint64(HqVmHandle hVm, uint64_t value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateUint64(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateFloat32(HqVmHandle hVm, float value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateFloat32(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateFloat64(HqVmHandle hVm, double value)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateFloat64(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateNull()
{
	return HqValue::CreateNull();
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateString(HqVmHandle hVm, const char* const string)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateString(hVm, string ? string : "");
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateFunction(HqVmHandle hVm, HqFunctionHandle hFunction)
{
	if(!hVm || !hFunction)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateFunction(hVm, hFunction);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateObject(HqVmHandle hVm, const char* const typeName)
{
	if(!hVm || !typeName || typeName[0] == '\0')
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	HqString* const pTypeName = HqString::Create(typeName);

	int result;
	HqScriptObject* const pSchema = HqVm::GetObjectSchema(hVm, pTypeName, &result);

	// Release the type name string now that we're done with it.
	HqString::Release(pTypeName);

	// Make sure we were able to successfully retrieved the object template.
	if(result != HQ_SUCCESS)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateObject(hVm, pSchema);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateArray(HqVmHandle hVm, const size_t count)
{
	if(!hVm)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateArray(hVm, count);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCreateNative(
	HqVmHandle hVm,
	void* pNativeObject,
	HqCallbackNativeValueCopy onCopy,
	HqCallbackNativeValueDestruct onDestruct,
	HqCallbackNativeValueEqual onTestEqual,
	HqCallbackNativeValueLessThan onTestLessThan
)
{
	if(!hVm || !onCopy || !onDestruct || !onTestEqual || !onTestLessThan)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	return HqValue::CreateNative(hVm, pNativeObject, onCopy, onDestruct, onTestEqual, onTestLessThan);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueCopy(HqVmHandle hVm, HqValueHandle hValue)
{
	return HqValue::Copy(hVm, hValue);
}

//----------------------------------------------------------------------------------------------------------------------

int HqValueGcProtect(HqValueHandle hValue)
{
	if(!HqValue::CanBeMarked(hValue))
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	HqValue::SetAutoMark(hValue, true);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqValueGcExpose(HqValueHandle hValue)
{
	if(!HqValue::CanBeMarked(hValue))
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	HqValue::SetAutoMark(hValue, false);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsPrimitiveType(HqValueHandle hValue)
{
	switch(hValue->type)
	{
		case HQ_VALUE_TYPE_BOOL:
		case HQ_VALUE_TYPE_INT8:
		case HQ_VALUE_TYPE_INT16:
		case HQ_VALUE_TYPE_INT32:
		case HQ_VALUE_TYPE_INT64:
		case HQ_VALUE_TYPE_UINT8:
		case HQ_VALUE_TYPE_UINT16:
		case HQ_VALUE_TYPE_UINT32:
		case HQ_VALUE_TYPE_UINT64:
		case HQ_VALUE_TYPE_FLOAT32:
		case HQ_VALUE_TYPE_FLOAT64:
			return true;

		default:
			break;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsBool(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_BOOL);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsInt8(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_INT8);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsInt16(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_INT16);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsInt32(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_INT32);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsInt64(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_INT64);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsUint8(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_UINT8);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsUint16(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_UINT16);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsUint32(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_UINT32);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsUint64(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_UINT64);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsFloat32(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_FLOAT32);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsFloat64(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_FLOAT64);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsNull(HqValueHandle hValue)
{
	return !hValue || (hValue->type == HQ_VALUE_TYPE_NULL);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsString(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_STRING);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsFunction(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_FUNCTION);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsObject(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_OBJECT);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsArray(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_ARRAY);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueIsNative(HqValueHandle hValue)
{
	return hValue && (hValue->type == HQ_VALUE_TYPE_NATIVE);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqValueGetBool(HqValueHandle hValue)
{
	if(HqValueIsString(hValue))
	{
		return hValue->as.boolean;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------

int8_t HqValueGetInt8(HqValueHandle hValue)
{
	if(HqValueIsInt8(hValue))
	{
		return hValue->as.int8;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int16_t HqValueGetInt16(HqValueHandle hValue)
{
	if(HqValueIsInt16(hValue))
	{
		return hValue->as.int16;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqValueGetInt32(HqValueHandle hValue)
{
	if(HqValueIsInt32(hValue))
	{
		return hValue->as.int32;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int64_t HqValueGetInt64(HqValueHandle hValue)
{
	if(HqValueIsInt64(hValue))
	{
		return hValue->as.int64;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uint8_t HqValueGetUint8(HqValueHandle hValue)
{
	if(HqValueIsUint8(hValue))
	{
		return hValue->as.uint8;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uint16_t HqValueGetUint16(HqValueHandle hValue)
{
	if(HqValueIsUint16(hValue))
	{
		return hValue->as.uint16;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t HqValueGetUint32(HqValueHandle hValue)
{
	if(HqValueIsUint32(hValue))
	{
		return hValue->as.uint32;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t HqValueGetUint64(HqValueHandle hValue)
{
	if(HqValueIsUint64(hValue))
	{
		return hValue->as.uint64;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

float HqValueGetFloat32(HqValueHandle hValue)
{
	if(HqValueIsFloat32(hValue))
	{
		return hValue->as.float32;
	}

	return 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------

double HqValueGetFloat64(HqValueHandle hValue)
{
	if(HqValueIsFloat64(hValue))
	{
		return hValue->as.float64;
	}

	return 0.0;
}

//----------------------------------------------------------------------------------------------------------------------

const char* HqValueGetString(HqValueHandle hValue)
{
	if(HqValueIsString(hValue))
	{
		return hValue->as.pString->data ? hValue->as.pString->data : "";
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqValueGetFunction(HqValueHandle hValue)
{
	if(HqValueIsFunction(hValue))
	{
		return hValue->as.hFunction;
	}

	return HQ_FUNCTION_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqValueGetNative(HqValueHandle hValue)
{
	if(HqValueIsNative(hValue))
	{
		return hValue->as.native.pObject;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqValueGetStringLength(HqValueHandle hValue)
{
	if(HqValueIsString(hValue))
	{
		return hValue->as.pString->length;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqValueGetStringHash(HqValueHandle hValue)
{
	if(HqValueIsString(hValue))
	{
		return hValue->as.pString->hash;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

const char* HqValueGetObjectTypeName(HqValueHandle hValue)
{
	if(HqValueIsObject(hValue))
	{
		return hValue->as.pObject->pTypeName->data;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqValueGetObjectMemberCount(HqValueHandle hValue)
{
	if(HqValueIsObject(hValue))
	{
		return hValue->as.pObject->members.count;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValueGetObjectMemberValue(HqValueHandle hValue, const char* memberName)
{
	if(HqValueIsObject(hValue) && memberName && memberName[0] != '\0')
	{
		HqScriptObject* const pScriptObject = hValue->as.pObject;
		HqString* const pMemberName = HqString::Create(memberName);

		int result;

		const HqScriptObject::MemberDefinition memberDef = HqScriptObject::GetMemberDefinition(pScriptObject, pMemberName, &result);
		if(result != HQ_SUCCESS)
		{
			return HQ_VALUE_HANDLE_NULL;
		}

		// Release the member name string now that we don't need it anymore.
		HqString::Release(pMemberName);

		HqValueHandle hMemberValue = HqScriptObject::GetMemberValue(pScriptObject, memberDef.bindingIndex, &result);
		if(result != HQ_SUCCESS)
		{
			return HQ_VALUE_HANDLE_NULL;
		}

		// Guard the value against being garbage collected.
		HqValue::SetAutoMark(hMemberValue, true);

		return hMemberValue;
	}

	return HQ_VALUE_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

uint8_t HqValueGetObjectMemberType(HqValueHandle hValue, const char* memberName)
{
	if(HqValueIsObject(hValue) && memberName && memberName[0] != '\0')
	{
		HqScriptObject* const pScriptObject = hValue->as.pObject;
		HqString* const pMemberName = HqString::Create(memberName);

		int result;

		const HqScriptObject::MemberDefinition memberDef = HqScriptObject::GetMemberDefinition(pScriptObject, pMemberName, &result);
		if(!result)
		{
			return HQ_VALUE_TYPE_NULL;
		}

		// Release the member name string now that we don't need it anymore.
		HqString::Release(pMemberName);

		return memberDef.valueType;
	}

	return HQ_VALUE_TYPE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

int HqValueSetObjectMemberValue(HqValueHandle hValue, const char* memberName, HqValueHandle hMemberValue)
{
	if(HqValueIsObject(hValue) && memberName && memberName[0] != '\0')
	{
		HqScriptObject* const pScriptObject = hValue->as.pObject;
		HqString* const pMemberName = HqString::Create(memberName);

		int result;

		const HqScriptObject::MemberDefinition memberDef = HqScriptObject::GetMemberDefinition(pScriptObject, pMemberName, &result);
		if(result != HQ_SUCCESS)
		{
			return result;
		}

		// Release the member name string now that we don't need it anymore.
		HqString::Release(pMemberName);

		HqScriptObject::SetMemberValue(pScriptObject, memberDef.bindingIndex, hMemberValue);

		return HQ_SUCCESS;
	}

	return HQ_ERROR_INVALID_TYPE;
}

//----------------------------------------------------------------------------------------------------------------------

int HqValueListObjectMembers(HqValueHandle hValue, HqCallbackIterateObjectMember onIterateFn, void* pUserData)
{
	if(HqValueIsObject(hValue))
	{
		// Iterate through each member definition on the object.
		for(auto& kv : hValue->as.pObject->definitions)
		{
			HqString* const pMemberName = HQ_MAP_ITER_KEY(kv);
			HqScriptObject::MemberDefinition& memberDef = HQ_MAP_ITER_VALUE(kv);

			if(!onIterateFn(pUserData, pMemberName->data, memberDef.valueType))
			{
				break;
			}
		}

		return HQ_SUCCESS;
	}

	return HQ_ERROR_INVALID_TYPE;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqValueGetObjectUserData(HqValueHandle hValue)
{
	if(HqValueIsObject(hValue))
	{
		HqScriptObject* const pScriptObject = hValue->as.pObject;

		return pScriptObject->pUserData;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

int HqValueSetObjectUserData(HqValueHandle hValue, void* pUserData)
{
	if(!HqValueIsObject(hValue))
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	HqScriptObject* const pScriptObject = hValue->as.pObject;

	pScriptObject->pUserData = pUserData;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqValueGetArrayLength(HqValueHandle hValue, size_t* const pOutIndex)
{
	if(!pOutIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(!HqValueIsArray(hValue))
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	(*pOutIndex) = hValue->as.array.count;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqValueGetArrayElement(HqValueHandle hValue, size_t index, HqValueHandle* phOutElementValue)
{
	if(!phOutElementValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(!HqValueIsArray(hValue))
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	if(index >= hValue->as.array.count)
	{
		return HQ_ERROR_INDEX_OUT_OF_RANGE;
	}

	(*phOutElementValue) = hValue->as.array.pData[index];

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqValueSetArrayElement(HqValueHandle hValue, size_t index, HqValueHandle hElementValue)
{
	if(!HqValueIsArray(hValue))
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	if(index >= hValue->as.array.count)
	{
		return HQ_ERROR_INDEX_OUT_OF_RANGE;
	}

	hValue->as.array.pData[index] = hElementValue;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

}