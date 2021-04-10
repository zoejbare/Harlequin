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
#include "OpDecl.hpp"

#include <assert.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------

XenonVmHandle XenonVm::Create(const XenonVmInit& init)
{
	XenonVm* const pOutput = new XenonVm();
	if(!pOutput)
	{
		return XENON_VM_HANDLE_NULL;
	}

	pOutput->report.onMessageFn = init.common.report.onMessageFn;
	pOutput->report.pUserData = init.common.report.pUserData;
	pOutput->report.level = init.common.report.reportLevel;
	pOutput->dependency.onRequestFn = init.dependency.onRequestFn;
	pOutput->dependency.pUserData = init.dependency.pUserData;

	// Initialize the garbage collector.
	XenonGarbageCollector::Initialize(pOutput->gc, pOutput);

	// Initialize the opcode array.
	OpCodeArray::Initialize(pOutput->opCodes);
	OpCodeArray::Reserve(pOutput->opCodes, XENON_OP_CODE__TOTAL_COUNT);

	pOutput->opCodes.count = XENON_OP_CODE__TOTAL_COUNT;

	#define XENON_BIND_OP_CODE(opCode, name) \
		pOutput->opCodes.pData[XENON_OP_CODE_ ## opCode].execFn = OpCodeExec_ ## name; \
		pOutput->opCodes.pData[XENON_OP_CODE_ ## opCode].disasmFn = OpCodeDisasm_ ## name

	XENON_BIND_OP_CODE(NOP, Nop);
	XENON_BIND_OP_CODE(RETURN, Return);
	XENON_BIND_OP_CODE(YIELD, Yield);
	XENON_BIND_OP_CODE(CALL, Call);
	XENON_BIND_OP_CODE(LOAD_CONSTANT, LoadConstant);
	XENON_BIND_OP_CODE(LOAD_GLOBAL, LoadGlobal);
	XENON_BIND_OP_CODE(LOAD_LOCAL, LoadLocal);
	XENON_BIND_OP_CODE(LOAD_PARAM, LoadParam);
	XENON_BIND_OP_CODE(STORE_GLOBAL, StoreGlobal);
	XENON_BIND_OP_CODE(STORE_LOCAL, StoreLocal);
	XENON_BIND_OP_CODE(STORE_PARAM, StoreParam);
	XENON_BIND_OP_CODE(PUSH, Push);
	XENON_BIND_OP_CODE(POP, Pop);

	#undef XENON_BIND_OP_CODE

	pOutput->gcThread = XenonThread::Create(prv_gcThreadMain, pOutput, init.gcThreadStackSize);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonVm::Dispose(XenonVmHandle hVm)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	hVm->isShuttingDown = true;

	int32_t threadReturnValue = 0;

	// Wait for the GC thread to exit.
	XenonThread::Join(hVm->gcThread, &threadReturnValue);

	if(threadReturnValue != XENON_SUCCESS)
	{
		XenonReportMessage(
			&hVm->report,
			XENON_MESSAGE_TYPE_ERROR,
			"Garbage collection thread exited abnormally: error=\"%s\"",
			XenonGetErrorCodeString(threadReturnValue)
		);
	}

	// Clean up each loaded program.
	for(auto& kv : hVm->programs)
	{
		XenonString::Release(kv.key);
		XenonProgram::Dispose(kv.value);
	}

	// Clean up each loaded function.
	for(auto& kv : hVm->functions)
	{
		XenonString::Release(kv.key);
		XenonFunction::Dispose(kv.value);
	}

	// Clean up each loaded global.
	for(auto& kv : hVm->globals)
	{
		XenonString::Release(kv.key);
		XenonValue::Release(kv.value);
	}

	// Clean up each active execution context.
	for(auto& kv : hVm->executionContexts)
	{
		XenonExecution::Release(kv.key);
	}

	XenonGarbageCollector::Dispose(hVm->gc);
	OpCodeArray::Dispose(hVm->opCodes);

	delete hVm;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVm::SetGlobalVariable(XenonVmHandle hVm, XenonValueHandle hValue, XenonString* const pVariableName)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(hValue != XENON_VALUE_HANDLE_NULL);
	assert(pVariableName != nullptr);

	auto kv = hVm->globals.find(pVariableName);
	if(kv == hVm->globals.end())
	{
		return XENON_ERROR_KEY_DOES_NOT_EXIST;
	}

	XenonValueHandle hValueRef = XenonValueReference(hValue);
	XenonValueHandle hOldValue = kv->value;

	XenonValueDispose(hOldValue);

	kv->value = hValueRef;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

XenonProgramHandle XenonVm::GetProgram(XenonVmHandle hVm, XenonString* const pProgramName, int* const pOutResult)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pProgramName != nullptr);
	assert(pOutResult != nullptr);

	if(!hVm->programs.Contains(pProgramName))
	{
		(*pOutResult) = XENON_ERROR_KEY_DOES_NOT_EXIST;
		return XENON_PROGRAM_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;
	return hVm->programs.Get(pProgramName);
}

//----------------------------------------------------------------------------------------------------------------------

XenonFunctionHandle XenonVm::GetFunction(XenonVmHandle hVm, XenonString* const pFunctionSignature, int* const pOutResult)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pFunctionSignature != nullptr);
	assert(pOutResult != nullptr);

	if(!hVm->functions.Contains(pFunctionSignature))
	{
		(*pOutResult) = XENON_ERROR_KEY_DOES_NOT_EXIST;
		return XENON_FUNCTION_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;
	return hVm->functions.Get(pFunctionSignature);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonVm::GetGlobalVariable(XenonVmHandle hVm, XenonString* const pVariableName, int* const pOutResult)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pVariableName != nullptr);
	assert(pOutResult != nullptr);

	if(!hVm->globals.Contains(pVariableName))
	{
		(*pOutResult) = XENON_ERROR_KEY_DOES_NOT_EXIST;
		return XENON_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;
	return XenonValueReference(hVm->globals.Get(pVariableName));
}

//----------------------------------------------------------------------------------------------------------------------

void XenonVm::ExecuteOpCode(XenonVmHandle hVm, XenonExecutionHandle hExec, const int opCode)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(opCode >= 0);
	assert(opCode < XENON_OP_CODE__TOTAL_COUNT);

	const OpCode& opCodeData = hVm->opCodes.pData[opCode];

	opCodeData.execFn(hExec);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonVm::DisassembleOpCode(XenonVmHandle hVm, XenonDisassemble& disasm, const int opCode)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(disasm.decoder.ip != nullptr);
	assert(disasm.onDisasmFn != nullptr);
	assert(opCode >= 0);
	assert(opCode < XENON_OP_CODE__TOTAL_COUNT);

	const OpCode& opCodeData = hVm->opCodes.pData[opCode];

	opCodeData.disasmFn(disasm);
}

//----------------------------------------------------------------------------------------------------------------------

int32_t XenonVm::prv_gcThreadMain(void* const pArg)
{
	XenonVmHandle hVm = reinterpret_cast<XenonVmHandle>(pArg);
	assert(hVm != XENON_VM_HANDLE_NULL);

	while(!hVm->isShuttingDown)
	{
		// TODO: Run the garbage collector here.
		// TODO: Need a separate timing mechanism to keep the garbage collector from running too much while not forcing long sleep times.
		XenonThread::Sleep(100);
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonVm::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonVm::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
