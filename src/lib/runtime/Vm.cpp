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

	OpCodeArray::Initialize(pOutput->opCodes);
	OpCodeArray::Reserve(pOutput->opCodes, XENON_OP_CODE__TOTAL_COUNT);

	pOutput->opCodes.count = XENON_OP_CODE__TOTAL_COUNT;

	#define XENON_BIND_OP_CODE(opCode, name) \
		pOutput->opCodes.pData[XENON_OP_CODE_ ## opCode].implFn = OpCodeImpl_ ## name

	XENON_BIND_OP_CODE(NOP, Nop);
	XENON_BIND_OP_CODE(RETURN, Return);
	XENON_BIND_OP_CODE(YIELD, Yield);
	XENON_BIND_OP_CODE(DEBUG_GP_REG, DebugGpReg);
	XENON_BIND_OP_CODE(DEBUG_IO_REG, DebugIoReg);
	XENON_BIND_OP_CODE(DEBUG_STACK, DebugStack);
	XENON_BIND_OP_CODE(LOAD_CONSTANT, LoadConstant);
	XENON_BIND_OP_CODE(LOAD_GLOBAL, LoadGlobal);
	XENON_BIND_OP_CODE(LOAD_LOCAL, LoadLocal);
	XENON_BIND_OP_CODE(PUSH, Push);
	XENON_BIND_OP_CODE(POP, Pop);
	XENON_BIND_OP_CODE(GET_PARAM, GetParam);
	XENON_BIND_OP_CODE(SET_PARAM, SetParam);

	#undef XENON_BIND_OP_CODE

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonVm::Dispose(XenonVmHandle hVm)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	// Clean up each loaded program.
	for(auto& kv : hVm->programs)
	{
		XenonProgramHandle hProgram = kv.value;

		XenonString::Dispose(kv.key);
		XenonProgram::Dispose(hProgram);
	}

	// Clean up each active execution context.
	for(auto& kv : hVm->executionContexts)
	{
		XenonExecution::Dispose(kv.key);
	}

	OpCodeArray::Dispose(hVm->opCodes);

	delete hVm;
}

//----------------------------------------------------------------------------------------------------------------------

XenonProgram* XenonVm::GetProgram(XenonVmHandle hVm, XenonString* const pProgramName)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pProgramName != nullptr);

	return hVm->programs.Get(pProgramName, nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

XenonFunctionHandle XenonVm::GetFunction(XenonVmHandle hVm, XenonString* const pFunctionSignature)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pFunctionSignature != nullptr);

	return hVm->functions.Get(pFunctionSignature, XENON_FUNCTION_HANDLE_NULL);
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonVm::GetGlobalVariable(XenonVmHandle hVm, XenonString* const pGlobalName)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pGlobalName != nullptr);

	return XenonValueReference(hVm->globals.Get(pGlobalName, XENON_VALUE_HANDLE_NULL));
}

//----------------------------------------------------------------------------------------------------------------------

void XenonVm::RunOpCode(XenonVmHandle hVm, XenonExecutionHandle hExec, const int opCode)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(opCode >= 0);
	assert(opCode < XENON_OP_CODE__TOTAL_COUNT);

	const OpCode& opCodeData = hVm->opCodes.pData[opCode];

	opCodeData.implFn(hExec);
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
