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

#include "../base/String.hpp"
#include "../common/ValueType.hpp"

#include "Execution.hpp"
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
		|| !init.dependency.onRequestFn)
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

	(*pOutCount) = hVm->programs.Size();

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
		if(!onIterateFn(pUserData, kv.value))
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

	(*pOutCount) = hVm->functions.Size();

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
		if(!onIterateFn(pUserData, kv.value))
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

	// Create a string object to lookup into the map.
	XenonString* const pVariableName = XenonString::Create(variableName);
	if(!pVariableName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

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
		|| *phOutValue
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

	(*pOutCount) = hVm->globals.Size();

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
		if(!onIterateFn(pUserData, kv.key->data, kv.value))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmGetObjectProfile(XenonVmHandle hVm, XenonValueHandle* phOutObjectProfile, const char* objectTypeName)
{
	// TODO: Implement this
	(void) hVm;
	(void) phOutObjectProfile;
	(void) objectTypeName;

	return XENON_ERROR_UNSPECIFIED_FAILURE;
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
	if(hVm->programs.Contains(pProgramName))
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
	hVm->programs.Insert(pProgramName, hProgram);

	// Report the program dependencies to the user code.
	for(auto& kv : hProgram->dependencies)
	{
		// Only report the dependency if it hasn't already been loaded.
		if(!hVm->programs.Contains(kv.key))
		{
			hVm->dependency.onRequestFn(hVm->dependency.pUserData, kv.key->data);
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
	XenonString* const pProgramName = XenonString::Create(filePath);
	if(!pProgramName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Check if a program with this name has already been loaded.
	if(hVm->programs.Contains(pProgramName))
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

	// Map the program inside the VM state.
	hVm->programs.Insert(pProgramName, hProgram);

	// Report the program dependencies to the user code.
	for(auto& kv : hProgram->dependencies)
	{
		// Only report the dependency if it hasn't already been loaded.
		if(!hVm->programs.Contains(kv.key))
		{
			hVm->dependency.onRequestFn(hVm->dependency.pUserData, kv.key->data);
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

	(*pOutCount) = hProgram->functions.Size();

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
		if(!onIterateFn(pUserData, kv.key->data))
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

	(*pOutCount) = hProgram->globals.Size();

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
		if(!onIterateFn(pUserData, kv.key->data))
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

	(*pOutOffset) = hFunction->offset;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionGetNativeBinding(XenonFunctionHandle hFunction, XenonNativeFunction* pOutBinding)
{
	if(!hFunction || !pOutBinding)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(!hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	(*pOutBinding) = hFunction->nativeBinding;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFunctionSetNativeBinding(XenonFunctionHandle hFunction, XenonNativeFunction bindingFn)
{
	if(!hFunction || !bindingFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(!hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	hFunction->nativeBinding = bindingFn;

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
	disasm.decoder = XenonDecoder::Construct(hFunction->hProgram, hFunction->offset);

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

	XenonExecutionHandle hExec = XenonExecution::Create(hVm, hEntryPoint);
	if(!hExec)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Use the execution map as a set to keep track of all execution contexts that belong to the input VM.
	hVm->executionContexts.Insert(hExec, false);

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

	hExec->hVm->executionContexts.Delete(hExec);
	XenonExecution::Dispose(hExec);

	(*phExecution) = XENON_EXECUTION_HANDLE_NULL;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionRun(XenonExecutionHandle hExec, int runMode)
{
	if(!hExec || runMode < XENON_RUN_STEP || runMode > XENON_RUN_LOOP)
	{
		return XENON_ERROR_INVALID_ARG;
	}
	else if(!hExec->hCurrentFrame)
	{
		return XENON_ERROR_SCRIPT_NO_FUNCTION;
	}

	// Set the 'started' flag to indicate that execution has started.
	// We also reset the 'yield' flag here since it's only useful for
	// pausing execution and we no longer need it paused until the
	// next yield.
	hExec->started = true;
	hExec->yield = false;

	switch(runMode)
	{
		case XENON_RUN_STEP:
			XenonExecution::RunStep(hExec);
			break;

		case XENON_RUN_LOOP:
			while(!hExec->finished && !hExec->exception && !hExec->yield)
			{
				XenonExecution::RunStep(hExec);
			}
			break;

		default:
			// This should never happen.
			assert(false);
			return XENON_ERROR_INVALID_ARG;
	}

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

int XenonExecutionGetStatus(XenonExecutionHandle hExec, bool* pOutStatus, int statusType)
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
		|| !hValue
		|| registerIndex < 0
		|| registerIndex >= XENON_VM_IO_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonExecution::SetIoRegister(hExec, hValue, registerIndex);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionGetIoRegister(XenonExecutionHandle hExec, XenonValueHandle* phOutValue, int registerIndex)
{
	if(!hExec
		|| !phOutValue
		|| *phOutValue
		|| registerIndex < 0
		|| registerIndex >= XENON_VM_IO_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	int result;
	(*phOutValue) = XenonExecution::GetIoRegister(hExec, registerIndex, &result);

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
		if(*phOutValue)
		{
			XenonValueDispose(*phOutValue);
		}

		(*phOutValue) = XenonValueReference(hValue);
	}

	XenonValueDispose(hValue);

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

	return XenonFrame::PeekValue(hFrame, phOutValue, stackIndex);
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

	return XenonFrame::SetGpRegister(hFrame, hValue, registerIndex);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameGetGpRegister(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, int registerIndex)
{
	if(!hFrame
		|| !phOutValue
		|| *phOutValue
		|| registerIndex < 0
		|| registerIndex >= XENON_VM_GP_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hFrame->hFunction->isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	int result;
	(*phOutValue) = XenonFrame::GetGpRegister(hFrame, registerIndex, &result);

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

	XenonString* const pVarName = XenonString::Create(variableName);
	if(!pVarName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	const int result = XenonFrame::SetLocalVariable(hFrame, hValue, pVarName);

	XenonString::Release(pVarName);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFrameGetLocalVariable(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, const char* variableName)
{
	if(!hFrame || !phOutValue || *phOutValue || !variableName || variableName[0] == '\0')
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

	int result;
	(*phOutValue) = XenonFrame::GetLocalVariable(hFrame, pVarName, &result);

	// Dispose of the string object now that we no longer need it.
	XenonString::Release(pVarName);

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

	(*pOutCount) = hFrame->locals.Size();

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
		if(!onIterateFn(pUserData, kv.key->data, kv.value))
		{
			break;
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateBool(bool value)
{
	return XenonValue::CreateBool(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateInt8(int8_t value)
{
	return XenonValue::CreateInt8(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateInt16(int16_t value)
{
	return XenonValue::CreateInt16(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateInt32(int32_t value)
{
	return XenonValue::CreateInt32(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateInt64(int64_t value)
{
	return XenonValue::CreateInt64(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateUint8(uint8_t value)
{
	return XenonValue::CreateUint8(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateUint16(uint16_t value)
{
	return XenonValue::CreateUint16(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateUint32(uint32_t value)
{
	return XenonValue::CreateUint32(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateUint64(uint64_t value)
{
	return XenonValue::CreateUint64(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateFloat32(float value)
{
	return XenonValue::CreateFloat32(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateFloat64(double value)
{
	return XenonValue::CreateFloat64(value);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateNull()
{
	return XenonValue::CreateNull();
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateString(const char* const string)
{
	return XenonValue::CreateString(string ? string : "");
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCreateObject(XenonValueHandle hObjectProfile)
{
	(void) hObjectProfile;
	// TODO: Implement support for script objects.
	assert(false);
	return XenonValue::CreateNull();
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueReference(XenonValueHandle hValue)
{
	if(!hValue)
	{
		return XENON_VALUE_HANDLE_NULL;
	}

	XenonValue::AddRef(hValue);

	return hValue;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValueCopy(XenonValueHandle hValue)
{
	return XenonValue::Copy(hValue);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonValueDispose(XenonValueHandle hValue)
{
	if(!hValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonValue::Release(hValue);

	return XENON_SUCCESS;
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

}
