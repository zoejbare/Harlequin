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

	// Initialize the execution context array.
	HqExecution::HandleArray::Initialize(pOutput->executionContexts);

	// Allocate the VM data maps.
	EmbeddedExceptionMap::Allocate(pOutput->embeddedExceptions);
	HqProgram::StringToHandleMap::Allocate(pOutput->programs);
	HqFunction::StringToHandleMap::Allocate(pOutput->functions);
	HqValue::StringToHandleMap::Allocate(pOutput->globals);
	HqScriptObject::StringToPtrMap::Allocate(pOutput->objectSchemas);

	prv_setupOpCodes(pOutput);
	prv_setupBuiltIns(pOutput);
	prv_setupEmbeddedExceptions(pOutput);

	HqMutex::Create(pOutput->lock);

	HqThreadConfig threadConfig;
	threadConfig.mainFn = prv_gcThreadMain;
	threadConfig.pArg = pOutput;
	threadConfig.stackSize = init.gcThreadStackSize;
	snprintf(threadConfig.name, sizeof(threadConfig.name), "%s", "HqGarbageCollector");

	if(pOutput->gc.maxTimeSlice > 0)
	{
		// Setting the GC time slice to 0 is the same as disabling garbage collection on a separate thread.
		HqThread::Create(pOutput->gcThread, threadConfig);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqVm::Dispose(HqVmHandle hVm)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	// Dispose of VM resources while locked.
	{
		HqScopedMutex vmLock(hVm->lock);

		hVm->isShuttingDown = true;

		if(hVm->gc.maxTimeSlice > 0)
		{
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
		}

		// Clean up each loaded program.
		{
			HqProgram::StringToHandleMap::Iterator iter;
			while(HqProgram::StringToHandleMap::IterateNext(hVm->programs, iter))
			{
				HqString::Release(iter.pData->key);
				HqProgram::Dispose(iter.pData->value);
			}

			HqProgram::StringToHandleMap::Dispose(hVm->programs);
		}

		// Clean up each loaded function.
		{
			HqFunction::StringToHandleMap::Iterator iter;
			while(HqFunction::StringToHandleMap::IterateNext(hVm->functions, iter))
			{
				HqString::Release(iter.pData->key);
				HqFunction::Dispose(iter.pData->value);
			}

			HqFunction::StringToHandleMap::Dispose(hVm->functions);
		}

		// Clean up each loaded global.
		{
			HqValue::StringToHandleMap::Iterator iter;
			while(HqValue::StringToHandleMap::IterateNext(hVm->globals, iter))
			{
				HqString::Release(iter.pData->key);
			}

			HqValue::StringToHandleMap::Dispose(hVm->globals);
		}

		// Clean up each loaded object schema.
		{
			HqScriptObject::StringToPtrMap::Iterator iter;
			while(HqScriptObject::StringToPtrMap::IterateNext(hVm->objectSchemas, iter))
			{
				HqString::Release(iter.pData->key);
				HqScriptObject::Dispose(iter.pData->value);
			}

			HqScriptObject::StringToPtrMap::Dispose(hVm->objectSchemas);
		}

		// Dispose of each embedded exception.
		{
			EmbeddedExceptionMap::Iterator iter;
			while(EmbeddedExceptionMap::IterateNext(hVm->embeddedExceptions, iter))
			{
				HqScriptObject::Dispose(iter.pData->value);
			}

			EmbeddedExceptionMap::Dispose(hVm->embeddedExceptions);
		}

		// Clean up each active execution context.
		for(size_t i = 0; i < hVm->executionContexts.count; ++i)
		{
			HqExecution::Dispose(hVm->executionContexts.pData[i]);
		}

		OpCodeArray::Dispose(hVm->opCodes);
		HqExecution::HandleArray::Dispose(hVm->executionContexts);
		HqGarbageCollector::Dispose(hVm->gc);
	}

	// Dispose of the VM mutex after it has been unlocked.
	HqMutex::Dispose(hVm->lock);

	delete hVm;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqVm::AttachExec(HqVmHandle hVm, HqExecutionHandle hExec)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	const size_t insertIndex = hVm->executionContexts.count;

	HqExecution::HandleArray::Reserve(hVm->executionContexts, insertIndex + 1);
	if(!hVm->executionContexts.pData)
	{
		return false;
	}

	hVm->executionContexts.pData[insertIndex] = hExec;
	++hVm->executionContexts.count;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

void HqVm::DetachExec(HqVmHandle hVm, HqExecutionHandle hExec)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	for(size_t i = 0; i < hVm->executionContexts.count; ++i)
	{
		if(hVm->executionContexts.pData[i] == hExec)
		{
			const size_t finalIndex = hVm->executionContexts.count - 1;

			if((hVm->executionContexts.count > 1) && (finalIndex > i))
			{
				// Keep the array of execution contexts tightly packed by swapping
				// into the current index the context at the very end of the array.
				hVm->executionContexts.pData[i] = hVm->executionContexts.pData[finalIndex];
			}

			--hVm->executionContexts.count;

			break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

int HqVm::SetGlobalVariable(HqVmHandle hVm, HqValueHandle hValue, HqString* const pVariableName)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hValue != HQ_VALUE_HANDLE_NULL);
	assert(pVariableName != nullptr);

	if(!HqValue::StringToHandleMap::Set(hVm->globals, pVariableName, hValue))
	{
		return HQ_ERROR_KEY_DOES_NOT_EXIST;
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

HqProgramHandle HqVm::GetProgram(HqVmHandle hVm, HqString* const pProgramName, int* const pOutResult)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pProgramName != nullptr);
	assert(pOutResult != nullptr);

	HqProgramHandle hOutput = HQ_PROGRAM_HANDLE_NULL;
	if(!HqProgram::StringToHandleMap::Get(hVm->programs, pProgramName, hOutput))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return HQ_PROGRAM_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return hOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqVm::GetFunction(HqVmHandle hVm, HqString* const pFunctionSignature, int* const pOutResult)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pFunctionSignature != nullptr);
	assert(pOutResult != nullptr);

	HqFunctionHandle hOutput = HQ_FUNCTION_HANDLE_NULL;
	if(!HqFunction::StringToHandleMap::Get(hVm->functions, pFunctionSignature, hOutput))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return HQ_FUNCTION_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return hOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqVm::GetGlobalVariable(HqVmHandle hVm, HqString* const pVariableName, int* const pOutResult)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pVariableName != nullptr);
	assert(pOutResult != nullptr);

	HqValueHandle hOutput = HQ_VALUE_HANDLE_NULL;
	if(!HqValue::StringToHandleMap::Get(hVm->globals, pVariableName, hOutput))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return HQ_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = HQ_SUCCESS;
	return hOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqScriptObject* HqVm::GetObjectSchema(HqVmHandle hVm, HqString* const pTypeName, int* const pOutResult)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pTypeName != nullptr);
	assert(pOutResult != nullptr);

	HqScriptObject* pOutput = nullptr;
	if(!HqScriptObject::StringToPtrMap::Get(hVm->objectSchemas, pTypeName, pOutput))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
		return nullptr;
	}

	(*pOutResult) = HQ_SUCCESS;
	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqVm::CreateStandardException(HqVmHandle hVm, const int exceptionType, const char* const message)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(exceptionType >= 0);
	assert(exceptionType < HQ_STANDARD_EXCEPTION__COUNT);

	HqScriptObject* pSchema = nullptr;
	if(!EmbeddedExceptionMap::Get(hVm->embeddedExceptions, exceptionType, pSchema))
	{
		return nullptr;
	}

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
