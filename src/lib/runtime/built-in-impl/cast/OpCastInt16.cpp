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

#include "../../BuiltInDecl.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

static int16_t GetDataFromParam(HqExecutionHandle hExec)
{
	// Get the parameter operand.
	HqValueHandle hParam;
	HqExecutionGetIoRegister(hExec, &hParam, 0);

	// Extract the parameter data.
	const int16_t param = HqValueGetInt16(hParam);

	// Release the input parameter value now that we have its data.
	HqValueGcExpose(hParam);

	return param;
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToBool(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateBool(hVm, (param == 0) ? false : true);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToInt8(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateInt8(hVm, int8_t(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToInt32(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateInt32(hVm, int32_t(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToInt64(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateInt64(hVm, int64_t(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToUint8(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateUint8(hVm, uint8_t(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToUint16(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateUint16(hVm, uint16_t(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToUint32(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateUint32(hVm, uint32_t(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToUint64(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateUint64(hVm, uint64_t(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToFloat32(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateFloat32(hVm, float(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToFloat64(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateFloat64(hVm, double(param));
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpCastInt16ToString(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const int16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	HqVmHandle hVm;
	HqExecutionGetVm(hExec, &hVm);

	char str[7];
	snprintf(str, sizeof(str), "%" PRId16, param);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateString(hVm, str);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------
