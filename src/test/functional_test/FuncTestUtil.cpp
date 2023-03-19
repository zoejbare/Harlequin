//
// Copyright (c) 2023, Zoe J. Bare
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

#include "FuncTestUtil.hpp"
#include "Memory.hpp"

#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------------------------------

void Util::CompileBytecode(std::vector<uint8_t>& outBytecode, CompilerCallback callback)
{
	// Set the memory context so we have a better idea of where to look when memory validation failures occur.
	Memory::Instance.SetContext("compiler");

	constexpr int endianness = HQ_ENDIAN_ORDER_NATIVE;

	const HqCompilerInit init = GetDefaultHqCompilerInit(nullptr, DefaultMessageCallback, HQ_MESSAGE_TYPE_WARNING);

	// Create the compiler.
	HqCompilerHandle hCompiler = HQ_COMPILER_HANDLE_NULL;
	const int createCompilerResult = HqCompilerCreate(&hCompiler, init);
	ASSERT_EQ(createCompilerResult, HQ_SUCCESS);

	// Get the compiler's report handle.
	HqReportHandle hReport = HQ_REPORT_HANDLE_NULL;
	const int getReportHandleResult = HqCompilerGetReportHandle(hCompiler, &hReport);
	ASSERT_EQ(getReportHandleResult, HQ_SUCCESS);

	// Create the module writer.
	HqModuleWriterHandle hModuleWriter = HQ_MODULE_WRITER_HANDLE_NULL;
	const int createModuleWriterResult = HqModuleWriterCreate(&hModuleWriter, hCompiler);
	ASSERT_EQ(createModuleWriterResult, HQ_SUCCESS);

	if(callback)
	{
		callback(hModuleWriter, endianness);
	}

	// Create the file serializer.
	HqSerializerHandle hFileSerializer = HQ_SERIALIZER_HANDLE_NULL;
	const int createFileSerializerResult = HqSerializerCreate(&hFileSerializer, HQ_SERIALIZER_MODE_WRITER);
	ASSERT_EQ(createFileSerializerResult, HQ_SUCCESS);

	// Set the file serializer to the system's native endianness.
	const int setFileSerializerEndiannessResult = HqSerializerSetEndianness(hFileSerializer, endianness);
	ASSERT_EQ(setFileSerializerEndiannessResult, HQ_SUCCESS);

	// Write the bytecode to the serializer.
	const int writeBytecodeResult = HqModuleWriterSerialize(hModuleWriter, hReport, hFileSerializer);
	ASSERT_EQ(writeBytecodeResult, HQ_SUCCESS);

	const void* const pModuleData = HqSerializerGetRawStreamPointer(hFileSerializer);
	const size_t moduleLength = HqSerializerGetStreamLength(hFileSerializer);

	// Reserve space in the output vector, then copy the module file data to it.
	outBytecode.resize(moduleLength);
	memcpy(outBytecode.data(), pModuleData, moduleLength);

	// Dispose of the file serializer.
	const int disposeFileSerializerResult = HqSerializerDispose(&hFileSerializer);
	ASSERT_EQ(disposeFileSerializerResult, HQ_SUCCESS);

	// Dispose of the module writer.
	const int disposeModuleWriterResult = HqModuleWriterDispose(&hModuleWriter);
	ASSERT_EQ(disposeModuleWriterResult, HQ_SUCCESS);

	// Dispose of the compiler.
	const int disposeCompilerResult = HqCompilerDispose(&hCompiler);
	ASSERT_EQ(disposeCompilerResult, HQ_SUCCESS);

	// Verify all memory has been freed.
	Memory::Instance.Validate();
}

//----------------------------------------------------------------------------------------------------------------------

void Util::ProcessBytecode(
	const char* const moduleName,
	const char* const function,
	RuntimeCallback callback,
	const std::vector<uint8_t>& bytecode)
{
	// Set the memory context so we have a better idea of where to look when memory validation failures occur.
	Memory::Instance.SetContext("runtime");

	const HqVmInit init = GetDefaultHqVmInit(nullptr, DefaultMessageCallback, HQ_MESSAGE_TYPE_WARNING);

	// Create the runtime VM.
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	const int createVmResult = HqVmCreate(&hVm, init);
	ASSERT_EQ(createVmResult, HQ_SUCCESS);

	// Load the module.
	const int loadModuleResult = HqVmLoadModule(hVm, moduleName, bytecode.data(), bytecode.size());
	ASSERT_EQ(loadModuleResult, HQ_SUCCESS);

	// Initialize the module.
	HqExecutionHandle hExec = HQ_EXECUTION_HANDLE_NULL;
	const int initModulesResult = HqVmInitializeModules(hVm, &hExec);
	ASSERT_EQ(initModulesResult, HQ_SUCCESS);

	// TODO: Check the execution context from module initialization to make sure there were no errors.

	if(function)
	{
		// Retrieve the function from the VM.
		HqFunctionHandle hFunction = HQ_FUNCTION_HANDLE_NULL;
		const int getFunctionResult = HqVmGetFunction(hVm, &hFunction, function);
		ASSERT_EQ(getFunctionResult, HQ_SUCCESS);

		// Create a new execution context.
		hExec = HQ_EXECUTION_HANDLE_NULL;
		const int createExecResult = HqExecutionCreate(&hExec, hVm);
		ASSERT_EQ(createExecResult, HQ_SUCCESS);

		// Initialize the execution context with the function.
		const int initExecResult = HqExecutionInitialize(hExec, hFunction);
		ASSERT_EQ(initExecResult, HQ_SUCCESS);

		if(callback)
		{
			callback(hVm, hExec);
		}

		// Dispose of the execution context.
		const int disposeExecResult = HqExecutionDispose(&hExec);
		ASSERT_EQ(disposeExecResult, HQ_SUCCESS);
	}

	// Dispose of the runtime VM.
	const int disposeVmResult = HqVmDispose(&hVm);
	ASSERT_EQ(disposeVmResult, HQ_SUCCESS);

	// Verify all memory has been freed.
	Memory::Instance.Validate();
}

//----------------------------------------------------------------------------------------------------------------------

void Util::SetupFunctionSerializer(HqSerializerHandle& output, const int endianness)
{
	// Create a serializer for the function.
	HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;
	const int createFuncSerializerResult = HqSerializerCreate(&hFuncSerializer, HQ_SERIALIZER_MODE_WRITER);
	ASSERT_EQ(createFuncSerializerResult, HQ_SUCCESS);

	// Set the file serializer to the system's native endianness.
	const int setFuncSerializerEndiannessResult = HqSerializerSetEndianness(hFuncSerializer, endianness);
	ASSERT_EQ(setFuncSerializerEndiannessResult, HQ_SUCCESS);

	output = hFuncSerializer;
}

//----------------------------------------------------------------------------------------------------------------------

void Util::FinalizeFunctionSerializer(
	HqSerializerHandle& hSerializer,
	HqModuleWriterHandle hModuleWriter,
	const char* const functionSignature)
{
	// All functions should end with a RETURN opcode.
	const int writeReturnInstrResult = HqBytecodeWriteReturn(hSerializer);
	ASSERT_EQ(writeReturnInstrResult, HQ_SUCCESS);

	const void* const pFuncData = HqSerializerGetRawStreamPointer(hSerializer);
	const size_t funcLength = HqSerializerGetStreamLength(hSerializer);

	// Add the function to the module writer.
	const int addFunctionResult = HqModuleWriterAddFunction(hModuleWriter, functionSignature, pFuncData, funcLength, 0, 0);
	ASSERT_EQ(addFunctionResult, HQ_SUCCESS);

	// Dispose of the function serializer.
	const int disposeFuncSerializerResult = HqSerializerDispose(&hSerializer);
	ASSERT_EQ(disposeFuncSerializerResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

void Util::GetExecutionStatus(ExecStatus& output, HqExecutionHandle hExec)
{
	// Get the 'yielded' status.
	const int getYieldStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_YIELD, &output.yield);
	ASSERT_EQ(getYieldStatusResult, HQ_SUCCESS);

	// Get the 'running' status.
	const int getRunningStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_RUNNING, &output.running);
	ASSERT_EQ(getRunningStatusResult, HQ_SUCCESS);

	// Get the 'completed' status.
	const int getCompletedStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_COMPLETE, &output.complete);
	ASSERT_EQ(getCompletedStatusResult, HQ_SUCCESS);

	// Get the 'exception' status.
	const int getExceptionStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_EXCEPTION, &output.exception);
	ASSERT_EQ(getExceptionStatusResult, HQ_SUCCESS);

	// Get the 'aborted' status.
	const int getAbortStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_ABORT, &output.abort);
	ASSERT_EQ(getAbortStatusResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

void Util::GetCurrentFrame(HqFrameHandle& output, HqExecutionHandle hExec)
{
	// Get the current frame in the callstack.
	HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
	const int getFrameResult = HqExecutionGetCurrentFrame(hExec, &hFrame);
	ASSERT_EQ(getFrameResult, HQ_SUCCESS);
	ASSERT_NE(hFrame, HQ_FRAME_HANDLE_NULL);

	output = hFrame;
}

//----------------------------------------------------------------------------------------------------------------------

void Util::GetGpRegister(HqValueHandle& output, HqExecutionHandle hExec, const uint32_t gpRegIndex)
{
	HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
	GetCurrentFrame(hFrame, hExec);

	// Get the general-purpose register that has the value we want to inspect.
	HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
	const int getGpRegisterResult = HqFrameGetGpRegister(hFrame, &hValue, gpRegIndex);
	ASSERT_EQ(getGpRegisterResult, HQ_SUCCESS);

	output = hValue;
}

//----------------------------------------------------------------------------------------------------------------------

void Util::GetIoRegister(HqValueHandle& output, HqExecutionHandle hExec, const uint32_t ioRegIndex)
{
	// Get the general-purpose register that has the value we want to inspect.
	HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
	const int getIoRegisterResult = HqExecutionGetIoRegister(hExec, &hValue, ioRegIndex);
	ASSERT_EQ(getIoRegisterResult, HQ_SUCCESS);

	output = hValue;
}

//----------------------------------------------------------------------------------------------------------------------

void Util::GetStackValue(HqValueHandle& output, HqExecutionHandle hExec, const uint32_t stackIndex)
{
	HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
	GetCurrentFrame(hFrame, hExec);

	// Get the general-purpose register that has the value we want to inspect.
	HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
	const int peekStackValueResult = HqFramePeekValue(hFrame, &hValue, stackIndex);
	ASSERT_EQ(peekStackValueResult, HQ_SUCCESS);

	output = hValue;
}

//----------------------------------------------------------------------------------------------------------------------
