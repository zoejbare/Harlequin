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

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulBool(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const bool left = HqValueGetBool(hLeft);
	const bool right = HqValueGetBool(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateBool(hVm, left && right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulInt8(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int8_t left = HqValueGetInt8(hLeft);
	const int8_t right = HqValueGetInt8(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateInt8(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulInt16(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int16_t left = HqValueGetInt16(hLeft);
	const int16_t right = HqValueGetInt16(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateInt16(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulInt32(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int32_t left = HqValueGetInt32(hLeft);
	const int32_t right = HqValueGetInt32(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateInt32(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulInt64(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const int64_t left = HqValueGetInt64(hLeft);
	const int64_t right = HqValueGetInt64(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateInt64(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulUint8(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint8_t left = HqValueGetUint8(hLeft);
	const uint8_t right = HqValueGetUint8(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateUint8(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulUint16(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint16_t left = HqValueGetUint16(hLeft);
	const uint16_t right = HqValueGetUint16(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateUint16(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulUint32(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint32_t left = HqValueGetUint32(hLeft);
	const uint32_t right = HqValueGetUint32(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateUint32(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulUint64(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const uint64_t left = HqValueGetUint64(hLeft);
	const uint64_t right = HqValueGetUint64(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateUint64(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulFloat32(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const float left = HqValueGetFloat32(hLeft);
	const float right = HqValueGetFloat32(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateFloat32(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpMulFloat64(HqExecutionHandle hExec, HqFunctionHandle, void*)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);

	// Get the VM associated with the input execution context.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionGetVm(hExec, &hVm);

	// Get the left operand value.
	HqValueHandle hLeft = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hLeft, 0);

	// Get the right operand value.
	HqValueHandle hRight = HQ_VALUE_HANDLE_NULL;
	HqExecutionGetIoRegister(hExec, &hRight, 1);

	// Extract the value data needed for the operation.
	const double left = HqValueGetFloat64(hLeft);
	const double right = HqValueGetFloat64(hRight);

	// Release the input parameter values now that we have their data.
	HqValueGcExpose(hLeft);
	HqValueGcExpose(hRight);

	// Create the output result and store it to an I/O register.
	HqValueHandle hOutput = HqValueCreateFloat64(hVm, left * right);
	HqExecutionSetIoRegister(hExec, hOutput, 0);
	HqValueGcExpose(hOutput);
}

//----------------------------------------------------------------------------------------------------------------------
