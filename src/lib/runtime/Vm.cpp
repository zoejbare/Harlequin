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

#include "Vm.hpp"

#include "../base/Clock.hpp"
#include "../common/OpCodeEnum.hpp"

#include <assert.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

HqVmHandle HqVm::Create(const HqVmInit& init)
{
	HqVm* const pOutput = new HqVm();
	assert(pOutput != HQ_VM_HANDLE_NULL);

	pOutput->report.onMessageFn = init.common.report.onMessageFn;
	pOutput->report.pUserData = init.common.report.pUserData;
	pOutput->report.level = init.common.report.reportLevel;

	// Initialize the garbage collector.
	HqGarbageCollector::Initialize(pOutput->gc, pOutput, init.gcMaxTimeSliceMs);

	// Initialize the opcode array.
	OpCodeArray::Initialize(pOutput->opCodes);
	OpCodeArray::Reserve(pOutput->opCodes, HQ_OP_CODE__TOTAL_COUNT);

	pOutput->opCodes.count = HQ_OP_CODE__TOTAL_COUNT;

	prv_setupOpCodes(pOutput);
	prv_setupBuiltIns(pOutput);
	prv_setupEmbeddedExceptions(pOutput);

	HqThreadConfig threadConfig;
	threadConfig.mainFn = prv_gcThreadMain;
	threadConfig.pArg = pOutput;
	threadConfig.stackSize = init.gcThreadStackSize;
	snprintf(threadConfig.name, sizeof(threadConfig.name), "%s", "HqGarbageCollector");

	pOutput->gcThread = HqThread::Create(threadConfig);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqVm::Dispose(HqVmHandle hVm)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	hVm->isShuttingDown = true;

	int32_t threadReturnValue = 0;

	// Wait for the GC thread to exit.
	HqThread::Join(hVm->gcThread, &threadReturnValue);

	if(threadReturnValue != HQ_SUCCESS)
	{
		HqReportMessage(
			&hVm->report,
			HQ_MESSAGE_TYPE_ERROR,
			"Garbage collection thread exited abnormally: error=\"%s\"",
			HqGetErrorCodeString(threadReturnValue)
		);
	}

	// Clean up each loaded program.
	for(auto& kv : hVm->programs)
	{
		HqString::Release(HQ_MAP_ITER_KEY(kv));
		HqProgram::Dispose(HQ_MAP_ITER_VALUE(kv));
	}

	// Clean up each loaded function.
	for(auto& kv : hVm->functions)
	{
		HqString::Release(HQ_MAP_ITER_KEY(kv));
		HqFunction::Dispose(HQ_MAP_ITER_VALUE(kv));
	}

	// Clean up each loaded global.
	for(auto& kv : hVm->globals)
	{
		HqString::Release(HQ_MAP_ITER_KEY(kv));
	}

	// Clean up each loaded object schema.
	for(auto& kv : hVm->objectSchemas)
	{
		HqString::Release(HQ_MAP_ITER_KEY(kv));
		HqScriptObject::Dispose(HQ_MAP_ITER_VALUE(kv));
	}

	// Clean up each active execution context.
	for(auto& kv : hVm->executionContexts)
	{
		HqExecution::ReleaseWithNoDetach(HQ_MAP_ITER_KEY(kv));
	}

	// Dispose of each embedded exception.
	for(auto& kv : hVm->embeddedExceptions)
	{
		HqScriptObject::Dispose(HQ_MAP_ITER_VALUE(kv));
	}

	HQ_MAP_FUNC_CLEAR(hVm->programs);
	HQ_MAP_FUNC_CLEAR(hVm->functions);
	HQ_MAP_FUNC_CLEAR(hVm->globals);
	HQ_MAP_FUNC_CLEAR(hVm->objectSchemas);
	HQ_MAP_FUNC_CLEAR(hVm->executionContexts);
	HQ_MAP_FUNC_CLEAR(hVm->embeddedExceptions);

	HqGarbageCollector::Dispose(hVm->gc);
	OpCodeArray::Dispose(hVm->opCodes);

	delete hVm;
}

//----------------------------------------------------------------------------------------------------------------------

int HqVm::SetGlobalVariable(HqVmHandle hVm, HqValueHandle hValue, HqString* const pVariableName)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hValue != HQ_VALUE_HANDLE_NULL);
	assert(pVariableName != nullptr);

	auto kv = hVm->globals.find(pVariableName);
	if(kv == hVm->globals.end())
	{
		return HQ_ERROR_KEY_DOES_NOT_EXIST;
	}

	HQ_MAP_ITER_PTR_VALUE(kv) = hValue;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

HqProgramHandle HqVm::GetProgram(HqVmHandle hVm, HqString* const pProgramName, int* const pOutResult)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pProgramName != nullptr);
	assert(pOutResult != nullptr);

	if(!HQ_MAP_FUNC_CONTAINS(hVm->programs, pProgramName))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return HQ_PROGRAM_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return HQ_MAP_FUNC_GET(hVm->programs, pProgramName);
}

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqVm::GetFunction(HqVmHandle hVm, HqString* const pFunctionSignature, int* const pOutResult)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pFunctionSignature != nullptr);
	assert(pOutResult != nullptr);

	if(!HQ_MAP_FUNC_CONTAINS(hVm->functions, pFunctionSignature))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return HQ_FUNCTION_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return HQ_MAP_FUNC_GET(hVm->functions, pFunctionSignature);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqVm::GetGlobalVariable(HqVmHandle hVm, HqString* const pVariableName, int* const pOutResult)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pVariableName != nullptr);
	assert(pOutResult != nullptr);

	if(!HQ_MAP_FUNC_CONTAINS(hVm->globals, pVariableName))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return HQ_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return HQ_MAP_FUNC_GET(hVm->globals, pVariableName);
}

//----------------------------------------------------------------------------------------------------------------------

HqScriptObject* HqVm::GetObjectSchema(HqVmHandle hVm, HqString* const pTypeName, int* const pOutResult)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pTypeName != nullptr);
	assert(pOutResult != nullptr);

	if(!HQ_MAP_FUNC_CONTAINS(hVm->objectSchemas, pTypeName))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return nullptr;
	}

	(*pOutResult) = HQ_SUCCESS;
	return HQ_MAP_FUNC_GET(hVm->objectSchemas, pTypeName);
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqVm::CreateStandardException(HqVmHandle hVm, const int exceptionType, const char* const message)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(exceptionType >= 0);
	assert(exceptionType < HQ_STANDARD_EXCEPTION__COUNT);

	if(!HQ_MAP_FUNC_CONTAINS(hVm->embeddedExceptions, exceptionType))
	{
		return nullptr;
	}

	HqScriptObject* const pSchema = HQ_MAP_FUNC_GET(hVm->embeddedExceptions, exceptionType);

	HqValueHandle hExceptionValue = HqValue::CreateObject(hVm, pSchema);
	HqValueHandle hMessageValue = HqValue::CreateString(hVm, message);

	HqScriptObject::SetMemberValue(hExceptionValue->as.pObject, 0, hMessageValue);

	HqValue::SetAutoMark(hExceptionValue, false);
	HqValue::SetAutoMark(hMessageValue, false);

	return hExceptionValue;
}

//----------------------------------------------------------------------------------------------------------------------

void HqVm::ExecuteOpCode(HqVmHandle hVm, HqExecutionHandle hExec, const int opCode)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(opCode >= 0);
	assert(opCode < HQ_OP_CODE__TOTAL_COUNT);

	const OpCode& opCodeData = hVm->opCodes.pData[opCode];

	opCodeData.execFn(hExec);
}

//----------------------------------------------------------------------------------------------------------------------

void HqVm::DisassembleOpCode(HqVmHandle hVm, HqDisassemble& disasm, const int opCode)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(disasm.decoder.ip != nullptr);
	assert(disasm.onDisasmFn != nullptr);
	assert(opCode >= 0);
	assert(opCode < HQ_OP_CODE__TOTAL_COUNT);

	const OpCode& opCodeData = hVm->opCodes.pData[opCode];

	opCodeData.disasmFn(disasm);
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqVm::prv_gcThreadMain(void* const pArg)
{
	HqVmHandle hVm = reinterpret_cast<HqVmHandle>(pArg);
	assert(hVm != HQ_VM_HANDLE_NULL);

	while(!hVm->isShuttingDown)
	{
		// Force a very small sleep to deprioritize the GC thread.
		HqThread::Sleep(2);

		// Run a step of the garbage collector.
		HqGarbageCollector::RunPhase(hVm->gc);
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqVm::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqVm::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
