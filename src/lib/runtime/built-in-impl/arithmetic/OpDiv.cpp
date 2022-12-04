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

void HqBuiltIn::OpDivBool(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateBool(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (bool)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivInt8(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateInt8(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (int8)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivInt16(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateInt16(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (int16)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivInt32(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateInt32(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (int32)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivInt64(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateInt64(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (int64)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivUint8(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateUint8(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (uint8)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivUint16(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateUint16(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (uint16)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivUint32(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateUint32(hVm, left / right);
		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (uint32)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivUint64(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateUint64(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (uint64)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivFloat32(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0.0f)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateFloat32(hVm, left / right);
		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (float32)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqBuiltIn::OpDivFloat64(HqExecutionHandle hExec, HqFunctionHandle, void*)
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

	if(right != 0.0)
	{
		// Create the output result and store it to an I/O register.
		HqValueHandle hOutput = HqValueCreateFloat64(hVm, left / right);

		HqExecutionSetIoRegister(hExec, hOutput, 0);
		HqValueGcExpose(hOutput);
	}
	else
	{
		// Raise the divide-by-zero script exception.
		HqExecutionRaiseStandardException(
			hExec,
			HQ_EXCEPTION_SEVERITY_NORMAL,
			HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
			"Divide-by-zero error (float64)"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------
