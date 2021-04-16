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

#include "../BuiltIn.hpp"

#include <assert.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubBool(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const bool left = XenonValueGetBool(hLeft);
	const bool right = XenonValueGetBool(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateBool(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubInt8(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int8_t left = XenonValueGetInt8(hLeft);
	const int8_t right = XenonValueGetInt8(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateInt8(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubInt16(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int16_t left = XenonValueGetInt16(hLeft);
	const int16_t right = XenonValueGetInt16(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateInt16(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubInt32(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int32_t left = XenonValueGetInt32(hLeft);
	const int32_t right = XenonValueGetInt32(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateInt32(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubInt64(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int64_t left = XenonValueGetInt64(hLeft);
	const int64_t right = XenonValueGetInt64(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateInt64(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubUint8(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint8_t left = XenonValueGetUint8(hLeft);
	const uint8_t right = XenonValueGetUint8(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateUint8(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubUint16(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint16_t left = XenonValueGetUint16(hLeft);
	const uint16_t right = XenonValueGetUint16(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateUint16(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubUint32(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint32_t left = XenonValueGetUint32(hLeft);
	const uint32_t right = XenonValueGetUint32(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateUint32(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubUint64(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint64_t left = XenonValueGetUint64(hLeft);
	const uint64_t right = XenonValueGetUint64(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateUint64(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubFloat32(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const float left = XenonValueGetFloat32(hLeft);
	const float right = XenonValueGetFloat32(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateFloat32(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonBuiltIn::OpSubFloat64(XenonExecutionHandle hExec, XenonFunctionHandle hFunction, void*)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	XenonValueHandle hLeft = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	XenonValueHandle hRight = XENON_VALUE_HANDLE_NULL;
	XenonExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const double left = XenonValueGetFloat64(hLeft);
	const double right = XenonValueGetFloat64(hRight);

	// Release the input parameter values now that we have their data.
	XenonValueDispose(hLeft);
	XenonValueDispose(hRight);

	// Create the output result and store it to an I/O register.
	XenonValueHandle hOutput = XenonValueCreateFloat64(hVm, left - right);
	XenonExecutionSetIoRegister(hExec, hOutput, 0);
}

//----------------------------------------------------------------------------------------------------------------------
