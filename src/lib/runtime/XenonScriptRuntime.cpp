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

#include "../XenonScript.h"

#include "../base/Mutex.hpp"
#include "../base/String.hpp"

#include "Execution.hpp"
#include "Object.hpp"
#include "Program.hpp"
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

int XenonVmCreate(XenonVmHandle* phOutVm, XenonVmInit init)
{
	if(!phOutVm
		|| (*phOutVm)
		|| init.common.report.reportLevel < XENON_MESSAGE_TYPE_VERBOSE
		|| init.common.report.reportLevel > XENON_MESSAGE_TYPE_FATAL
		|| !init.dependency.onRequestFn
		|| init.gcThreadStackSize < XENON_VM_THREAD_MINIMUM_STACK_SIZE
		|| init.gcMaxIterationCount == 0)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonVm* const pVm = XenonVm::Create(init);

	// The message has to be printed *after* creating the VM state since the state is used in this function.
	XenonReportMessage(&pVm->report, XENON_MESSAGE_TYPE_VERBOSE, "Initializing Xenon runtime");

	(*phOutVm) = pVm;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmDispose(XenonVmHandle* phVm)
{
	if(!phVm || !(*phVm))
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonVmHandle hVm = (*phVm);

	XenonReportMessage(&hVm->report, XENON_MESSAGE_TYPE_VERBOSE, "Releasing Xenon runtime");
	XenonVm::Dispose(hVm);

	(*phVm) = XENON_VM_HANDLE_NULL;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmGetReportHandle(XenonVmHandle hVm, XenonReportHandle* phOutReport)
{
	if(!hVm || !phOutReport || *phOutReport)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*phOutReport) = &hVm->report;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmGetProgram(XenonVmHandle hVm, XenonProgramHandle* phOutProgram, const char* programName)
{
	if(!hVm || !phOutProgram || *phOutProgram || !programName || programName[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create a string object since we need that to look up into the map.
	XenonString* const pName = XenonString::Create(programName);
	if(!pName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	int result;
	(*phOutProgram) = XenonVm::GetProgram(hVm, pName, &result);

	// We no longer need the string object.
	XenonString::Release(pName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmGetProgramCount(XenonVmHandle hVm, size_t* pOutCount)
{
	if(!hVm || !pOutCount)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutCount) = XENON_MAP_FUNC_SIZE(hVm->programs);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmListPrograms(XenonVmHandle hVm, XenonCallbackIterateProgram onIterateFn, void* pUserData)
{
	if(!hVm || !onIterateFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Call the callback for each program we currently have loaded.
	for(auto& kv : hVm->programs)
	{
		if(!onIterateFn(pUserData, XENON_MAP_ITER_VALUE(kv)))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmGetFunction(XenonVmHandle hVm, XenonFunctionHandle* phOutFunction, const char* signature)
{
	if(!hVm
		|| !phOutFunction
		|| *phOutFunction
		|| !signature
		|| signature[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create a string object since we need that to look up into the map.
	XenonString* const pSig = XenonString::Create(signature);
	if(!pSig)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	int result;
	(*phOutFunction) = XenonVm::GetFunction(hVm, pSig, &result);

	// We no longer need the string object.
	XenonString::Release(pSig);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmGetFunctionCount(XenonVmHandle hVm, size_t* pOutCount)
{
	if(!hVm || !pOutCount)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutCount) = XENON_MAP_FUNC_SIZE(hVm->functions);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmListFunctions(XenonVmHandle hVm, XenonCallbackIterateFunction onIterateFn, void* pUserData)
{
	if(!hVm || !onIterateFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Call the callback for each function we currently have loaded.
	for(auto& kv : hVm->functions)
	{
		if(!onIterateFn(pUserData, XENON_MAP_ITER_VALUE(kv)))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmSetGlobalVariable(XenonVmHandle hVm, XenonValueHandle hValue, const char* variableName)
{
	if(!hVm || !variableName || variableName[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(!hValue)
	{
		hValue = XenonValue::CreateNull();
	}

	if(hValue->type != XENON_VALUE_TYPE_NULL && hVm != hValue->hVm)
	{
		return XENON_ERROR_MISMATCH;
	}

	// Create a string object to lookup into the map.
	XenonString* const pVariableName = XenonString::Create(variableName);
	if(!pVariableName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	XenonScopedReadLock gcLock(hVm->gcRwLock);

	const int result = XenonVm::SetGlobalVariable(hVm, hValue, pVariableName);

	// Dispose of the string object since we no longer need it.
	XenonString::Release(pVariableName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmGetGlobalVariable(XenonVmHandle hVm, XenonValueHandle* phOutValue, const char* variableName)
{
	if(!hVm
		|| !phOutValue
		|| !variableName
		|| variableName[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create a string object in order to look up into the global map.
	XenonString* const pGlobalName = XenonString::Create(variableName);
	if(!pGlobalName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	int result;
	(*phOutValue) = XenonVm::GetGlobalVariable(hVm, pGlobalName, &result);

	XenonString::Release(pGlobalName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmGetGlobalVariableCount(XenonVmHandle hVm, size_t* pOutCount)
{
	if(!hVm || !pOutCount)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutCount) = XENON_MAP_FUNC_SIZE(hVm->globals);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmListGlobalVariables(XenonVmHandle hVm, XenonCallbackIterateVariable onIterateFn, void* pUserData)
{
	if(!hVm || !onIterateFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Call the callback for each global variable we currently have loaded.
	for(auto& kv : hVm->globals)
	{
		XenonString* const pGlobalName = XENON_MAP_ITER_KEY(kv);
		XenonValueHandle hValue = XENON_MAP_ITER_VALUE(kv);

		if(!onIterateFn(pUserData, pGlobalName->data, hValue))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmListObjectSchemas(XenonVmHandle hVm, XenonCallbackIterateString onIterateFn, void* pUserData)
{
	if(!hVm || !onIterateFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Call the callback for each object schema we currently have loaded.
	for(auto& kv : hVm->objectSchemas)
	{
		XenonString* const pObjectTypeName = XENON_MAP_ITER_KEY(kv);

		if(!onIterateFn(pUserData, pObjectTypeName->data))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmLoadProgram(
	XenonVmHandle hVm,
	const char* programName,
	const void* pProgramFileData,
	size_t programFileSize
)
{
	if(!hVm || !programName || programName[0] == '\0' || !pProgramFileData || programFileSize == 0)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create a string to be the key in the program map.
	XenonString* const pProgramName = XenonString::Create(programName);
	if(!pProgramName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Check if a program with this name has already been loaded.
	if(XENON_MAP_FUNC_CONTAINS(hVm->programs, pProgramName))
	{
		XenonString::Release(pProgramName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	// Attempt to load the program file.
	XenonProgramHandle hProgram = XenonProgram::Create(hVm, pProgramName, pProgramFileData, programFileSize);
	if(!hProgram)
	{
		XenonString::Release(pProgramName);
		return XENON_ERROR_FAILED_TO_OPEN_FILE;
	}

	// Map the program inside the VM state.
	XENON_MAP_FUNC_INSERT(hVm->programs, pProgramName, hProgram);

	// Report the program dependencies to the user code.
	for(auto& kv : hProgram->dependencies)
	{
		XenonString* const pDependencyName = XENON_MAP_ITER_KEY(kv);

		// Only report the dependency if it hasn't already been loaded.
		if(!XENON_MAP_FUNC_CONTAINS(hVm->programs, pDependencyName))
		{
			hVm->dependency.onRequestFn(hVm->dependency.pUserData, pDependencyName->data);
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmLoadProgramFromFile(XenonVmHandle hVm, const char* programName, const char* filePath)
{
	if(!hVm || !programName || programName[0] == '\0' || !filePath || filePath[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create a string to be the key in the program map.
	XenonString* const pProgramName = XenonString::Create(programName);
	if(!pProgramName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Check if a program with this name has already been loaded.
	if(XENON_MAP_FUNC_CONTAINS(hVm->programs, pProgramName))
	{
		XenonString::Release(pProgramName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	// Attempt to load the program file.
	XenonProgram* const hProgram = XenonProgram::Create(hVm, pProgramName, filePath);
	if(!hProgram)
	{
		XenonString::Release(pProgramName);
		return XENON_ERROR_FAILED_TO_OPEN_FILE;
	}

	// Report the program dependencies to the user code.
	for(auto& kv : hProgram->dependencies)
	{
		XenonString* const pDependencyName = XENON_MAP_ITER_KEY(kv);

		// Only report the dependency if it hasn't already been loaded.
		if(!XENON_MAP_FUNC_CONTAINS(hVm->programs, pDependencyName))
		{
			hVm->dependency.onRequestFn(hVm->dependency.pUserData, pDependencyName->data);
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramGetName(XenonProgramHandle hProgram, const char** pOutName)
{
	if(!hProgram || !pOutName)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutName) = hProgram->pName->data;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramGetEndianness(XenonProgramHandle hProgram, int* pOutEndianness)
{
	if(!hProgram || !pOutEndianness)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutEndianness) = hProgram->endianness;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramGetFunctionCount(XenonProgramHandle hProgram, size_t* pOutCount)
{
	if(!hProgram || !pOutCount)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutCount) = XENON_MAP_FUNC_SIZE(hProgram->functions);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramListFunctions(XenonProgramHandle hProgram, XenonCallbackIterateString onIterateFn, void* pUserData)
{
	if(!hProgram || !onIterateFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Call the callback for each function signature in the program.
	for(auto& kv : hProgram->functions)
	{
		XenonString* const pFunctionSignature = XENON_MAP_ITER_KEY(kv);

		if(!onIterateFn(pUserData, pFunctionSignature->data))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramGetGlobalVariableCount(XenonProgramHandle hProgram, size_t* pOutCount)
{
	if(!hProgram || !pOutCount)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutCount) = XENON_MAP_FUNC_SIZE(hProgram->globals);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramListGlobalVariables(XenonProgramHandle hProgram, XenonCallbackIterateString onIterateFn, void* pUserData)
{
	if(!hProgram || !onIterateFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Call the callback for each global variable name in the program.
	for(auto& kv : hProgram->globals)
	{
		XenonString* const pGlobalName = XENON_MAP_ITER_KEY(kv);

		if(!onIterateFn(pUserData, pGlobalName->data))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionGetSignature(XenonFunctionHandle hFunction, const char** pOutSignature)
{
	if(!hFunction || !pOutSignature)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutSignature) = hFunction->pSignature->data;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionGetIsNative(XenonFunctionHandle hFunction, bool* pOutNative)
{
	if(!hFunction || !pOutNative)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutNative) = hFunction->isNative;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionGetParameterCount(XenonFunctionHandle hFunction, uint16_t* pOutCount)
{
	if(!hFunction || !pOutCount)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutCount) = hFunction->numParameters;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionGetReturnValueCount(XenonFunctionHandle hFunction, uint16_t* pOutCount)
{
	if(!hFunction || !pOutCount)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutCount) = hFunction->numReturnValues;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionGetBytecodeOffset(XenonFunctionHandle hFunction, uint32_t* pOutOffset)
{
	if(!hFunction || !pOutOffset)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	(*pOutOffset) = hFunction->bytecodeOffsetStart;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionGetNativeBinding(XenonFunctionHandle hFunction, XenonNativeFunction* pOutNativeFn)
{
	if(!hFunction || !pOutNativeFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(!hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	(*pOutNativeFn) = hFunction->nativeFn;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionGetNativeUserData(XenonFunctionHandle hFunction, void** ppOutUserData)
{
	if(!hFunction || !ppOutUserData)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(!hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	(*ppOutUserData) = hFunction->pNativeUserData;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionSetNativeBinding(XenonFunctionHandle hFunction, XenonNativeFunction nativeFn, void* pUserData)
{
	if(!hFunction || !nativeFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(!hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	hFunction->nativeFn = nativeFn;
	hFunction->pNativeUserData = pUserData;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionDisassemble(XenonFunctionHandle hFunction, XenonCallbackOpDisasm onDisasmFn, void* pUserData)
{
	if(!hFunction || !onDisasmFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	XenonDisassemble disasm;

	disasm.hProgram = hFunction->hProgram;
	disasm.onDisasmFn = onDisasmFn;
	disasm.pUserData = pUserData;

	XenonDecoder::Initialize(disasm.decoder, hFunction->hProgram, hFunction->bytecodeOffsetStart);

	// Iterate through each instruction.
	for(;;)
	{
		const uintptr_t offset = uintptr_t(disasm.decoder.ip - hFunction->hProgram->code.pData);
		const uint8_t opCode = XenonDecoder::LoadUint8(disasm.decoder);

		disasm.opcodeOffset = offset;

		XenonVm::DisassembleOpCode(disasm.hProgram->hVm, disasm, opCode);

		if(opCode == XENON_OP_CODE_RETURN)
		{
			// The RETURN opcode indicates the end of the function.
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionCreate(
	XenonExecutionHandle* phOutExecution,
	XenonVmHandle hVm,
	XenonFunctionHandle hEntryPoint
)
{
	if(!phOutExecution
		|| (*phOutExecution)
		|| !hVm
		|| !hEntryPoint)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create a new execution context with the provided script function as the entry point.
	XenonExecutionHandle hExec = XenonExecution::Create(hVm, hEntryPoint);
	if(!hExec)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	(*phOutExecution) = hExec;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionDispose(XenonExecutionHandle* phExecution)
{
	if(!phExecution || !(*phExecution))
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonExecutionHandle hExec = (*phExecution);

	XenonExecution::DetachFromVm(hExec);

	(*phExecution) = XENON_EXECUTION_HANDLE_NULL;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionRun(XenonExecutionHandle hExec, int runMode)
{
	if(!hExec || runMode < XENON_RUN_STEP || runMode > XENON_RUN_CONTINUOUS)
	{
		return XENON_ERROR_INVALID_ARG;
	}
	else if(!hExec->hCurrentFrame)
	{
		return XENON_ERROR_SCRIPT_NO_FUNCTION;
	}

	XenonExecution::Run(hExec, runMode);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionYield(XenonExecutionHandle hExec)
{
	if(!hExec)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	hExec->yield = true;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionRaiseException(XenonExecutionHandle hExec)
{
	if(!hExec)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	hExec->exception = true;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionGetVm(XenonExecutionHandle hExec, XenonVmHandle* phOutVm)
{
	if(!hExec || !phOutVm)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*phOutVm) = hExec->hVm;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionGetStatus(XenonExecutionHandle hExec, int statusType, bool* pOutStatus)
{
	if(!hExec || !pOutStatus)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	switch(statusType)
	{
		case XENON_EXEC_STATUS_YIELD:
			(*pOutStatus) = hExec->yield;
			break;

		case XENON_EXEC_STATUS_RUNNING:
			(*pOutStatus) = (hExec->started && !hExec->finished) || hExec->exception;
			break;

		case XENON_EXEC_STATUS_COMPLETE:
			(*pOutStatus) = hExec->finished || hExec->exception;
			break;

		case XENON_EXEC_STATUS_EXCEPTION:
			(*pOutStatus) = hExec->exception;
			break;

		case XENON_EXEC_STATUS_ABORT:
			(*pOutStatus) = hExec->abort;
			break;

		default:
			return XENON_ERROR_INVALID_TYPE;
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionHasUnhandledExceptionOccurred(XenonExecutionHandle hExec, bool* pOutException)
{
	if(!hExec || !pOutException)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutException) = hExec->exception;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionGetFrameStackDepth(XenonExecutionHandle hExec, size_t* pOutDepth)
{
	if(!hExec || !pOutDepth)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutDepth) = hExec->frameStack.nextIndex;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionResolveFrameStack(
	XenonExecutionHandle hExec,
	XenonCallbackIterateFrame onIterateFn,
	void* pUserData
)
{
	if(!hExec || !onIterateFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	for(size_t i = 0; i < hExec->frameStack.nextIndex; ++i)
	{
		// Traverse the frame stack in reverse.
		XenonFrameHandle hFrame = hExec->frameStack.memory.pData[hExec->frameStack.nextIndex - i - 1];

		if(!onIterateFn(pUserData, hFrame))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionGetCurrentFrame(XenonExecutionHandle hExec, XenonFrameHandle* phOutFrame)
{
	if(!hExec || !phOutFrame || *phOutFrame)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*phOutFrame) = hExec->hCurrentFrame;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionSetIoRegister(XenonExecutionHandle hExec, XenonValueHandle hValue, int registerIndex)
{
	if(!hExec
		|| registerIndex < 0
		|| registerIndex >= XENON_VM_IO_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(!hValue)
	{
		hValue = XenonValue::CreateNull();
	}

	if(hValue->type != XENON_VALUE_TYPE_NULL
		&& hExec->hVm != hValue->hVm)
	{
		return XENON_ERROR_MISMATCH;
	}

	return XenonExecution::SetIoRegister(hExec, hValue, registerIndex);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionGetIoRegister(XenonExecutionHandle hExec, XenonValueHandle* phOutValue, int registerIndex)
{
	if(!hExec
		|| !phOutValue
		|| registerIndex < 0
		|| registerIndex >= XENON_VM_IO_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonScopedReadLock gcLock(hExec->hVm->gcRwLock);

	int result;
	XenonValueHandle hValue = XenonExecution::GetIoRegister(hExec, registerIndex, &result);

	// Guard the value against being garbage collected.
	XenonValue::SetAutoMark(hValue, true);

	(*phOutValue) = hValue;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameGetFunction(XenonFrameHandle hFrame, XenonFunctionHandle* phOutFunction)
{
	if(!hFrame || !phOutFunction || (*phOutFunction))
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*phOutFunction) = hFrame->hFunction;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameGetBytecodeOffset(XenonFrameHandle hFrame, uint32_t* pOutOffset)
{
	if(!hFrame || !pOutOffset)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	(*pOutOffset) = uint32_t(hFrame->decoder.cachedIp - hFrame->hFunction->hProgram->code.pData);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFramePushValue(XenonFrameHandle hFrame, XenonValueHandle hValue)
{
	if(!hFrame)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	if(!hValue)
	{
		hValue = XenonValue::CreateNull();
	}

	if(hValue->type != XENON_VALUE_TYPE_NULL
		&& XenonFunction::GetVm(hFrame->hFunction) != hValue->hVm)
	{
		return XENON_ERROR_MISMATCH;
	}

	return XenonFrame::PushValue(hFrame, hValue);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFramePopValue(XenonFrameHandle hFrame, XenonValueHandle* phOutValue)
{
	if(!hFrame)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	XenonValueHandle hValue = XENON_VALUE_HANDLE_NULL;
	int result = XenonFrame::PopValue(hFrame, &hValue);

	if(phOutValue)
	{
		// Guard the value against being garbage collected.
		XenonValue::SetAutoMark(hValue, true);

		(*phOutValue) = hValue;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFramePeekValue(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, int stackIndex)
{
	if(!hFrame || !phOutValue || *phOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	XenonValueHandle hValue = XENON_VALUE_HANDLE_NULL;
	XenonFrame::PeekValue(hFrame, &hValue, stackIndex);

	// Guard the value against being garbage collected.
	XenonValue::SetAutoMark(hValue, true);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameSetGpRegister(XenonFrameHandle hFrame, XenonValueHandle hValue, int registerIndex)
{
	if(!hFrame || registerIndex < 0 || registerIndex >= XENON_VM_GP_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	if(!hValue)
	{
		hValue = XenonValue::CreateNull();
	}

	XenonVmHandle hVm = XenonFunction::GetVm(hFrame->hFunction);
	if(!hVm)
	{
		return XENON_ERROR_INVALID_DATA;
	}

	if(hValue->type != XENON_VALUE_TYPE_NULL
		&& hVm != hValue->hVm)
	{
		return XENON_ERROR_MISMATCH;
	}

	XenonScopedReadLock gcLock(hVm->gcRwLock);

	return XenonFrame::SetGpRegister(hFrame, hValue, registerIndex);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameGetGpRegister(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, int registerIndex)
{
	if(!hFrame
		|| !phOutValue
		|| registerIndex < 0
		|| registerIndex >= XENON_VM_GP_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	XenonVmHandle hVm = XenonFunction::GetVm(hFrame->hFunction);
	if(!hVm)
	{
		return XENON_ERROR_INVALID_DATA;
	}

	XenonScopedReadLock gcLock(hVm->gcRwLock);

	int result;
	XenonValueHandle hValue = XenonFrame::GetGpRegister(hFrame, registerIndex, &result);

	// Guard the value against being garbage collected.
	XenonValue::SetAutoMark(hValue, true);

	(*phOutValue) = hValue;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameSetLocalVariable(XenonFrameHandle hFrame, XenonValueHandle hValue, const char* variableName)
{
	if(!hFrame || !hValue || !variableName || variableName[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	if(!hValue)
	{
		hValue = XenonValue::CreateNull();
	}

	XenonVmHandle hVm = XenonFunction::GetVm(hFrame->hFunction);
	if(!hVm)
	{
		return XENON_ERROR_INVALID_DATA;
	}

	if(hValue->type != XENON_VALUE_TYPE_NULL
		&& hVm != hValue->hVm)
	{
		return XENON_ERROR_MISMATCH;
	}

	XenonString* const pVarName = XenonString::Create(variableName);
	if(!pVarName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	XenonScopedReadLock gcLock(hVm->gcRwLock);

	const int result = XenonFrame::SetLocalVariable(hFrame, hValue, pVarName);

	XenonString::Release(pVarName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameGetLocalVariable(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, const char* variableName)
{
	if(!hFrame || !phOutValue || !variableName || variableName[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	// Create a string object to use for the map lookup.
	XenonString* const pVarName = XenonString::Create(variableName);
	if(!pVarName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	XenonVmHandle hVm = XenonFunction::GetVm(hFrame->hFunction);
	if(!hVm)
	{
		return XENON_ERROR_INVALID_DATA;
	}

	XenonScopedReadLock gcLock(hVm->gcRwLock);

	int result;
	XenonValueHandle hValue = XenonFrame::GetLocalVariable(hFrame, pVarName, &result);

	// Dispose of the string object now that we no longer need it.
	XenonString::Release(pVarName);

	// Guard the value against being garbage collected.
	XenonValue::SetAutoMark(hValue, true);

	(*phOutValue) = hValue;

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameGetLocalVariableCount(XenonFrameHandle hFrame, size_t* pOutCount)
{
	if(!hFrame || !pOutCount)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	(*pOutCount) = XENON_MAP_FUNC_SIZE(hFrame->locals);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameListLocalVariables(XenonFrameHandle hFrame, XenonCallbackIterateVariable onIterateFn, void* pUserData)
{
	if(!hFrame || !onIterateFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	for(auto& kv : hFrame->locals)
	{
		XenonString* const pLocalName = XENON_MAP_ITER_KEY(kv);
		XenonValueHandle hValue = XENON_MAP_ITER_VALUE(kv);

		if(!onIterateFn(pUserData, pLocalName->data, hValue))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateBool(XenonVmHandle hVm, bool value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateBool(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateInt8(XenonVmHandle hVm, int8_t value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateInt8(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateInt16(XenonVmHandle hVm, int16_t value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateInt16(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateInt32(XenonVmHandle hVm, int32_t value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateInt32(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateInt64(XenonVmHandle hVm, int64_t value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateInt64(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateUint8(XenonVmHandle hVm, uint8_t value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateUint8(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateUint16(XenonVmHandle hVm, uint16_t value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateUint16(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateUint32(XenonVmHandle hVm, uint32_t value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateUint32(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateUint64(XenonVmHandle hVm, uint64_t value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateUint64(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateFloat32(XenonVmHandle hVm, float value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateFloat32(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateFloat64(XenonVmHandle hVm, double value)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateFloat64(hVm, value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateNull()
{
	return XenonValue::CreateNull();
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateString(XenonVmHandle hVm, const char* const string)
{
	if(!hVm)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateString(hVm, string ? string : "");
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateObject(XenonVmHandle hVm, const char* const typeName)
{
	if(!hVm || !typeName || typeName[0] == '\0')
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	XenonString* const pTypeName = XenonString::Create(typeName);

	int result;
	XenonObject* const pSchema = XenonVm::GetObjectSchema(hVm, pTypeName, &result);

	// Release the type name string now that we're done with it.
	XenonString::Release(pTypeName);

	// Make sure we were able to successfully retrieved the object template.
	if(result != XENON_SUCCESS)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	return XenonValue::CreateObject(hVm, pSchema);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCopy(XenonVmHandle hVm, XenonValueHandle hValue)
{
	return XenonValue::Copy(hVm, hValue);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonValuePreserve(XenonValueHandle hValue)
{
	XenonValue::SetAutoMark(hValue, true);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonValueAbandon(XenonValueHandle hValue)
{
	XenonValue::SetAutoMark(hValue, false);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsPrimitiveType(XenonValueHandle hValue)
{
	switch(hValue->type)
	{
		case XENON_VALUE_TYPE_BOOL:
		case XENON_VALUE_TYPE_INT8:
		case XENON_VALUE_TYPE_INT16:
		case XENON_VALUE_TYPE_INT32:
		case XENON_VALUE_TYPE_INT64:
		case XENON_VALUE_TYPE_UINT8:
		case XENON_VALUE_TYPE_UINT16:
		case XENON_VALUE_TYPE_UINT32:
		case XENON_VALUE_TYPE_UINT64:
		case XENON_VALUE_TYPE_FLOAT32:
		case XENON_VALUE_TYPE_FLOAT64:
			return true;

		default:
			break;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsBool(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_BOOL);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsInt8(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_INT8);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsInt16(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_INT16);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsInt32(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_INT32);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsInt64(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_INT64);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsUint8(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_UINT8);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsUint16(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_UINT16);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsUint32(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_UINT32);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsUint64(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_UINT64);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsFloat32(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_FLOAT32);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsFloat64(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_FLOAT64);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsNull(XenonValueHandle hValue)
{
	return !hValue || (hValue->type == XENON_VALUE_TYPE_NULL);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsString(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_STRING);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueIsObject(XenonValueHandle hValue)
{
	return hValue && (hValue->type == XENON_VALUE_TYPE_OBJECT);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValueGetBool(XenonValueHandle hValue)
{
	if(XenonValueIsString(hValue))
	{
		return hValue->as.boolean;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------

int8_t XenonValueGetInt8(XenonValueHandle hValue)
{
	if(XenonValueIsInt8(hValue))
	{
		return hValue->as.int8;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int16_t XenonValueGetInt16(XenonValueHandle hValue)
{
	if(XenonValueIsInt16(hValue))
	{
		return hValue->as.int16;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t XenonValueGetInt32(XenonValueHandle hValue)
{
	if(XenonValueIsInt32(hValue))
	{
		return hValue->as.int32;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int64_t XenonValueGetInt64(XenonValueHandle hValue)
{
	if(XenonValueIsInt64(hValue))
	{
		return hValue->as.int64;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uint8_t XenonValueGetUint8(XenonValueHandle hValue)
{
	if(XenonValueIsUint8(hValue))
	{
		return hValue->as.uint8;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uint16_t XenonValueGetUint16(XenonValueHandle hValue)
{
	if(XenonValueIsUint16(hValue))
	{
		return hValue->as.uint16;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonValueGetUint32(XenonValueHandle hValue)
{
	if(XenonValueIsUint32(hValue))
	{
		return hValue->as.uint32;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t XenonValueGetUint64(XenonValueHandle hValue)
{
	if(XenonValueIsUint64(hValue))
	{
		return hValue->as.uint64;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

float XenonValueGetFloat32(XenonValueHandle hValue)
{
	if(XenonValueIsFloat32(hValue))
	{
		return hValue->as.float32;
	}

	return 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------

double XenonValueGetFloat64(XenonValueHandle hValue)
{
	if(XenonValueIsFloat64(hValue))
	{
		return hValue->as.float64;
	}

	return 0.0;
}

//----------------------------------------------------------------------------------------------------------------------

const char* XenonValueGetString(XenonValueHandle hValue)
{
	if(XenonValueIsString(hValue))
	{
		return hValue->as.pString->data ? hValue->as.pString->data : "";
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonValueGetStringLength(XenonValueHandle hValue)
{
	if(XenonValueIsString(hValue))
	{
		return hValue->as.pString->length;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonValueGetStringHash(XenonValueHandle hValue)
{
	if(XenonValueIsString(hValue))
	{
		return hValue->as.pString->hash;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

const char* XenonValueGetObjectTypeName(XenonValueHandle hValue)
{
	if(XenonValueIsObject(hValue))
	{
		return hValue->as.pObject->pTypeName->data;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonValueGetObjectMemberCount(XenonValueHandle hValue)
{
	if(XenonValueIsObject(hValue))
	{
		return hValue->as.pObject->members.count;
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueGetObjectMemberValue(XenonValueHandle hValue, const char* memberName)
{
	if(XenonValueIsObject(hValue) && memberName && memberName[0] != '\0')
	{
		XenonObject* const pScriptObject = hValue->as.pObject;
		XenonString* const pMemberName = XenonString::Create(memberName);

		int result;

		const XenonObject::MemberDefinition memberDef = XenonObject::GetMemberDefinition(pScriptObject, pMemberName, &result);
		if(result != XENON_SUCCESS)
		{
			return XENON_VALUE_HANDLE_NULL;
		}

		// Release the member name string now that we don't need it anymore.
		XenonString::Release(pMemberName);

		XenonValueHandle hMemberValue = XenonObject::GetMemberValue(pScriptObject, memberDef.bindingIndex, &result);
		if(result != XENON_SUCCESS)
		{
			return XENON_VALUE_HANDLE_NULL;
		}

		// Guard the value against being garbage collected.
		XenonValue::SetAutoMark(hMemberValue, true);

		return hMemberValue;
	}

	return XENON_VALUE_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

uint8_t XenonValueGetObjectMemberType(XenonValueHandle hValue, const char* memberName)
{
	if(XenonValueIsObject(hValue) && memberName && memberName[0] != '\0')
	{
		XenonObject* const pScriptObject = hValue->as.pObject;
		XenonString* const pMemberName = XenonString::Create(memberName);

		int result;

		const XenonObject::MemberDefinition memberDef = XenonObject::GetMemberDefinition(pScriptObject, pMemberName, &result);
		if(!result)
		{
			return XENON_VALUE_TYPE_NULL;
		}

		// Release the member name string now that we don't need it anymore.
		XenonString::Release(pMemberName);

		return memberDef.valueType;
	}

	return XENON_VALUE_TYPE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonValueSetObjectMemberValue(XenonValueHandle hValue, const char* memberName, XenonValueHandle hMemberValue)
{
	if(XenonValueIsObject(hValue) && memberName && memberName[0] != '\0')
	{
		XenonObject* const pScriptObject = hValue->as.pObject;
		XenonString* const pMemberName = XenonString::Create(memberName);

		int result;

		const XenonObject::MemberDefinition memberDef = XenonObject::GetMemberDefinition(pScriptObject, pMemberName, &result);
		if(result != XENON_SUCCESS)
		{
			return result;
		}

		// Release the member name string now that we don't need it anymore.
		XenonString::Release(pMemberName);

		XenonObject::SetMemberValue(pScriptObject, memberDef.bindingIndex, hMemberValue);

		return XENON_SUCCESS;
	}

	return XENON_ERROR_INVALID_TYPE;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonValueListObjectMembers(XenonValueHandle hValue, XenonCallbackIterateObjectMember onIterateFn, void* pUserData)
{
	if(XenonValueIsObject(hValue))
	{
		// Iterate through each member definition on the object.
		for(auto& kv : hValue->as.pObject->definitions)
		{
			XenonString* const pMemberName = XENON_MAP_ITER_KEY(kv);
			XenonObject::MemberDefinition& memberDef = XENON_MAP_ITER_VALUE(kv);

			if(!onIterateFn(pUserData, pMemberName->data, memberDef.valueType))
			{
				break;
			}
		}

		return XENON_SUCCESS;
	}

	return XENON_ERROR_INVALID_TYPE;
}

//----------------------------------------------------------------------------------------------------------------------

}
