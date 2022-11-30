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

static uint16_t GetDataFromParam(XenonExecutionHandle hExec)
{
	// Get the parameter operand.
	XenonValueHandle hParam;
	XenonExecutionGetIoRegister(hExec, &hParam, 0);

	// Extract the parameter data.
	const uint16_t param = XenonValueGetUint8(hParam);

	// Release the input parameter value now that we have its data.
	XenonValueGcExpose(hParam);

	return param;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToBool(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateBool(hVm, (param == 0) ? false : true);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToInt8(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateInt8(hVm, int8_t(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToInt16(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateInt16(hVm, int16_t(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToInt32(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateInt32(hVm, int32_t(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToInt64(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateInt64(hVm, int64_t(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToUint8(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateUint8(hVm, uint8_t(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToUint32(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateUint32(hVm, uint32_t(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToUint64(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateUint64(hVm, uint64_t(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToFloat32(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateFloat32(hVm, float(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToFloat64(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateFloat64(hVm, double(param));
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpCastUint16ToString(XenonExecutionHandle hExec, XenonFunctionHandle, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);

	// Get the parameter data.
	const uint16_t param = GetDataFromParam(hExec);

	// Get the VM associated with the execution context.
	XenonVmHandle hVm;
	XenonExecutionGetVm(hExec, &hVm);

	char str[6];
	snprintf(str, sizeof(str), "%" PRIu16, param);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateString(hVm, str);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
	XenonValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------
