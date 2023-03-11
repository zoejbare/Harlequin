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

TEST(_HQ_TEST_NAME(TestOpCodes), Abort)
{
	static const char* const mainFuncSig = "void main()";

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		// Create a serializer for the function.
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;
		const int createFuncSerializerResult = HqSerializerCreate(&hFuncSerializer, HQ_SERIALIZER_MODE_WRITER);
		ASSERT_EQ(createFuncSerializerResult, HQ_SUCCESS);

		// Set the file serializer to the system's native endianness.
		const int setFuncSerializerEndiannessResult = HqSerializerSetEndianness(hFuncSerializer, endianness);
		ASSERT_EQ(setFuncSerializerEndiannessResult, HQ_SUCCESS);

		// Write the instruction that we're going to test.
		const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
		ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

		// Write a RETURN instruction just in case something goes wrong.
		const int writeReturnInstrResult = HqBytecodeWriteReturn(hFuncSerializer);
		ASSERT_EQ(writeReturnInstrResult, HQ_SUCCESS);

		const void* const pFuncData = HqSerializerGetRawStreamPointer(hFuncSerializer);
		const size_t funcLength = HqSerializerGetStreamLength(hFuncSerializer);

		// Add the function to the module writer.
		const int addFunctionResult = HqModuleWriterAddFunction(hModuleWriter, mainFuncSig, pFuncData, funcLength, 0, 0);
		ASSERT_EQ(addFunctionResult, HQ_SUCCESS);

		// Dispose of the function serializer.
		const int disposeFuncSerializerResult = HqSerializerDispose(&hFuncSerializer);
		ASSERT_EQ(disposeFuncSerializerResult, HQ_SUCCESS);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		bool executionAborted = false;

		// Get the 'aborted' status.
		const int getAbortStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_ABORT, &executionAborted);
		ASSERT_EQ(getAbortStatusResult, HQ_SUCCESS);
		ASSERT_TRUE(executionAborted);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", mainFuncSig, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------
