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

#include "../RuntimeUtil.hpp"

#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------------------------------

namespace Function
{
	constexpr const char* const main = "void main()";
}

//----------------------------------------------------------------------------------------------------------------------

struct ExecStatus
{
	bool yield;
	bool running;
	bool complete;
	bool exception;
	bool abort;
};

//----------------------------------------------------------------------------------------------------------------------

void SetupFunctionSerializer(HqSerializerHandle& output, const int endianness)
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

void FinalizeFunctionSerializer(
	HqSerializerHandle hSerializer,
	HqModuleWriterHandle hModuleWriter,
	const char* const functionSignature)
{
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

void GetExecutionStatus(ExecStatus& output, HqExecutionHandle hExec)
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

TEST(_HQ_TEST_NAME(TestOpCodes), Return)
{
	std::vector<uint8_t> bytecode;

	// Compiling with just an empty init function is enough to test the RETURN opcode since
	// that gets added to the module by default if no other bytecode is supplied for it.
	CompileBytecode(bytecode, nullptr);
	ASSERT_GT(bytecode.size(), 0u);

	ProcessBytecode("TestOpCodes", nullptr, nullptr, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestOpCodes), Nop)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeNopInstrResult = HqBytecodeWriteNop(hFuncSerializer);
		ASSERT_EQ(writeNopInstrResult, HQ_SUCCESS);

		// Write a RETURN instruction just in case something goes wrong.
		const int writeReturnInstrResult = HqBytecodeWriteReturn(hFuncSerializer);
		ASSERT_EQ(writeReturnInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_TRUE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestOpCodes), Abort)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
		ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

		// Write a RETURN instruction just in case something goes wrong.
		const int writeReturnInstrResult = HqBytecodeWriteReturn(hFuncSerializer);
		ASSERT_EQ(writeReturnInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_TRUE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestOpCodes), Yield)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Write a RETURN instruction just in case something goes wrong.
		const int writeReturnInstrResult = HqBytecodeWriteReturn(hFuncSerializer);
		ASSERT_EQ(writeReturnInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------
