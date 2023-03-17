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

#include "../Memory.hpp"
#include "../RuntimeUtil.hpp"

#include <compiler/JumpInstruction.hpp>
#include <gtest/gtest.h>

#include <math.h>

//----------------------------------------------------------------------------------------------------------------------

namespace Function
{
	static constexpr const char* const main = "void main()";
}

//----------------------------------------------------------------------------------------------------------------------

class _HQ_TEST_NAME(TestOpCodes)
	: public ::testing::Test
{
public:

	virtual void TearDown() override
	{
		// Force the memory handler to reset after each test.
		Memory::Instance.Reset();
	}


protected:

	struct ExecStatus
	{
		bool yield;
		bool running;
		bool complete;
		bool exception;
		bool abort;
	};

	static void _setupFunctionSerializer(HqSerializerHandle& output, const int endianness)
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

	static void _finalizeFunctionSerializer(
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

	static void _getExecutionStatus(ExecStatus& output, HqExecutionHandle hExec)
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

	static void _getCurrentFrame(HqFrameHandle& output, HqExecutionHandle hExec)
	{
		// Get the current frame in the callstack.
		HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
		const int getFrameResult = HqExecutionGetCurrentFrame(hExec, &hFrame);
		ASSERT_EQ(getFrameResult, HQ_SUCCESS);
		ASSERT_NE(hFrame, HQ_FRAME_HANDLE_NULL);

		output = hFrame;
	}

	static void _getGpRegister(HqValueHandle& output, HqExecutionHandle hExec, const int gpRegIndex)
	{
		HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
		_getCurrentFrame(hFrame, hExec);

		// Get the general-purpose register that has the value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		const int getGpRegisterResult = HqFrameGetGpRegister(hFrame, &hValue, gpRegIndex);
		ASSERT_EQ(getGpRegisterResult, HQ_SUCCESS);

		output = hValue;
	}

	static void _getIoRegister(HqValueHandle& output, HqExecutionHandle hExec, const int ioRegIndex)
	{
		// Get the general-purpose register that has the value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		const int getIoRegisterResult = HqExecutionGetIoRegister(hExec, &hValue, ioRegIndex);
		ASSERT_EQ(getIoRegisterResult, HQ_SUCCESS);

		output = hValue;
	}

	static void _getStackValue(HqValueHandle& output, HqExecutionHandle hExec, const int stackIndex)
	{
		HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
		_getCurrentFrame(hFrame, hExec);

		// Get the general-purpose register that has the value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		const int peekStackValueResult = HqFramePeekValue(hFrame, &hValue, stackIndex);
		ASSERT_EQ(peekStackValueResult, HQ_SUCCESS);

		output = hValue;
	}
};

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Return)
{
	std::vector<uint8_t> bytecode;

	// Compiling with just an empty init function is enough to test the RETURN opcode since
	// that gets added to the module by default if no other bytecode is supplied for it.
	CompileBytecode(bytecode, nullptr);
	ASSERT_GT(bytecode.size(), 0u);

	ProcessBytecode("TestOpCodes", nullptr, nullptr, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Nop)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeNopInstrResult = HqBytecodeWriteNop(hFuncSerializer);
		ASSERT_EQ(writeNopInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Abort)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
		ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Yield)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
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

TEST_F(_HQ_TEST_NAME(TestOpCodes), LoadImm)
{
	static constexpr int8_t testValueDataI8 = 123;
	static constexpr int16_t testValueDataI16 = 12345;
	static constexpr int32_t testValueDataI32 = 1234578;
	static constexpr int64_t testValueDataI64 = 12345678901ll;

	static constexpr uint8_t testValueDataU8 = 234;
	static constexpr uint16_t testValueDataU16 = 23456;
	static constexpr uint32_t testValueDataU32 = 2345789u;
	static constexpr uint64_t testValueDataU64 = 23456789012ull;

	static constexpr float testValueDataF32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueDataF64 = 6.283185307179586476925286766559;

	static constexpr const char* const testValueDataStr = "test string data";

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add the string value to the module string table.
		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, testValueDataStr, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write a LOAD_IMM_NULL instruction.
		const int writeLoadNullInstrResult = HqBytecodeWriteLoadImmNull(hFuncSerializer, 0);
		ASSERT_EQ(writeLoadNullInstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_BOOL instruction.
		const int writeLoadBoolInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 1, true);
		ASSERT_EQ(writeLoadBoolInstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_I8 instruction.
		const int writeLoadI8InstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 2, testValueDataI8);
		ASSERT_EQ(writeLoadI8InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_I16 instruction.
		const int writeLoadI16InstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 3, testValueDataI16);
		ASSERT_EQ(writeLoadI16InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_I32 instruction.
		const int writeLoadI32InstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 4, testValueDataI32);
		ASSERT_EQ(writeLoadI32InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_I64 instruction.
		const int writeLoadI64InstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 5, testValueDataI64);
		ASSERT_EQ(writeLoadI64InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_U8 instruction.
		const int writeLoadU8InstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 6, testValueDataU8);
		ASSERT_EQ(writeLoadU8InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_U16 instruction.
		const int writeLoadU16InstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 7, testValueDataU16);
		ASSERT_EQ(writeLoadU16InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_U32 instruction.
		const int writeLoadU32InstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 8, testValueDataU32);
		ASSERT_EQ(writeLoadU32InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_U64 instruction.
		const int writeLoadU64InstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 9, testValueDataU64);
		ASSERT_EQ(writeLoadU64InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_F32 instruction.
		const int writeLoadF32InstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 10, testValueDataF32);
		ASSERT_EQ(writeLoadF32InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_F64 instruction.
		const int writeLoadF64InstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 11, testValueDataF64);
		ASSERT_EQ(writeLoadF64InstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_STR instruction.
		const int writeLoadStrInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 12, 0);
		ASSERT_EQ(writeLoadStrInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the GP registers in the frame.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the null value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_EQ(hValue, HQ_VALUE_HANDLE_NULL);
		}

		// Verify the boolean value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Verify the int8 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), testValueDataI8);
		}

		// Verify the int16 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), testValueDataI16);
		}

		// Verify the int32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueDataI32);
		}

		// Verify the int64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), testValueDataI64);
		}

		// Verify the uint8 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), testValueDataU8);
		}

		// Verify the uint16 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), testValueDataU16);
		}

		// Verify the uint32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), testValueDataU32);
		}

		// Verify the uint64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), testValueDataU64);
		}

		// Verify the float32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), testValueDataF32);
		}

		// Verify the float64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 11);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), testValueDataF64);
		}

		// Verify the string value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 12);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STRCASEEQ(HqValueGetString(hValue), testValueDataStr);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), LoadGlobal_StoreGlobal)
{
	static constexpr const char* const globalName = "testValue";
	static constexpr int32_t testValueData = 12345678;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add the string value to the module string table.
		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, globalName, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Add the global variable to the module.
		const int addGlobalResult = HqModuleWriterAddGlobal(hModuleWriter, globalName);
		ASSERT_EQ(addGlobalResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write a LOAD_IMM instruction so we have data to put in the global variable.
		const int writeLoadImmInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
		ASSERT_EQ(writeLoadImmInstrResult, HQ_SUCCESS);

		// Write the STORE_GLOBAL instruction to give the global variable some data.
		const int writeStoreInstrResult = HqBytecodeWriteStoreGlobal(hFuncSerializer, stringIndex, 0);
		ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

		// Write the LOAD_GLOBAL instruction to a GP register that we can inspect.
		const int writeLoadGlobalInstrResult = HqBytecodeWriteLoadGlobal(hFuncSerializer, 1, stringIndex);
		ASSERT_EQ(writeLoadGlobalInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the global variable value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		const int getGlobalVarResult = HqVmGetGlobalVariable(hVm, &hValue, globalName);
		ASSERT_EQ(getGlobalVarResult, HQ_SUCCESS);

		// Validate the global variable.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);

		// Get the register value we want to inspect.
		_getGpRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), LoadParam_StoreParam)
{
	static constexpr int32_t testValueData = 12345678;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write a LOAD_IMM instruction so we have data to put in the I/O register.
		const int writeLoadImmInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
		ASSERT_EQ(writeLoadImmInstrResult, HQ_SUCCESS);

		// Write the STORE_PARAM instruction to give the global variable some data.
		const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 2, 0);
		ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

		// Write the LOAD_PARAM instruction to a GP register that we can inspect.
		const int writeLoadGlobalInstrResult = HqBytecodeWriteLoadParam(hFuncSerializer, 1, 2);
		ASSERT_EQ(writeLoadGlobalInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the GP register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		_getGpRegister(hValue, hExec, 1);

		// Validate the GP register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);

		// Get the I/O register value we want to inspect.
		_getIoRegister(hValue, hExec, 2);

		// Validate the I/O register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), InitObject_LoadObject_StoreObject)
{
	static constexpr const char* const objTypeName = "TestObject";
	static constexpr const char* const objMemberName = "foo";
	static constexpr int32_t testValueData = 12345;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		const int addObjTypeResult = HqModuleWriterAddObjectType(hModuleWriter, objTypeName);
		ASSERT_EQ(addObjTypeResult, HQ_SUCCESS);

		uint32_t memberIndex = 0;
		const int addObjMemberResult = HqModuleWriterAddObjectMember(hModuleWriter, objTypeName, objMemberName, HQ_VALUE_TYPE_INT32, &memberIndex);
		ASSERT_EQ(addObjMemberResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the INIT_OBJECT instruction to initialize an instance of an object into a GP register.
		const int writeInitObjInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 0, 0);
		ASSERT_EQ(writeInitObjInstrResult, HQ_SUCCESS);

		// Write the LOAD_IMM_I32 instruction so we have test data to assign to the object member.
		const int writeLoadI32InstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 1, testValueData);
		ASSERT_EQ(writeLoadI32InstrResult, HQ_SUCCESS);

		// Write the STORE_OBJECT instruction to set the object member to the test data.
		const int writeStoreObjInstrResult = HqBytecodeWriteStoreObject(hFuncSerializer, 0, 1, memberIndex);
		ASSERT_EQ(writeStoreObjInstrResult, HQ_SUCCESS);

		// Write the LOAD_OBJECT instruction to pull the object member data into a GP register for inspection.
		const int writeLoadObjInstrResult = HqBytecodeWriteLoadObject(hFuncSerializer, 2, 0, memberIndex);
		ASSERT_EQ(writeLoadObjInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the object instance data.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsObject(hValue));
			ASSERT_STRCASEEQ(HqValueGetObjectTypeName(hValue), objTypeName);
			ASSERT_EQ(HqValueGetObjectMemberCount(hValue), 1);

			uint8_t objMemberType = HQ_VALUE_TYPE_INT8;
			const int getObjMemberTypeResult = HqValueGetObjectMemberType(hValue, objMemberName, &objMemberType);
			ASSERT_EQ(getObjMemberTypeResult, HQ_SUCCESS);
			ASSERT_EQ(objMemberType, HQ_VALUE_TYPE_INT32);

			HqValueHandle hMemberValue = HqValueGetObjectMemberValue(hValue, objMemberName);

			// Validate the member value.
			ASSERT_NE(hMemberValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hMemberValue));
			ASSERT_EQ(HqValueGetInt32(hMemberValue), testValueData);
		}

		// Verify the value extracted from the object.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), InitArray_LoadArray_StoreArray)
{
	static constexpr int32_t testValueData = 12345;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the INIT_ARRAY instruction to initialize an instance of an array into a GP register.
		const int writeInitArrayInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 0, 1);
		ASSERT_EQ(writeInitArrayInstrResult, HQ_SUCCESS);

		// Write the LOAD_IMM_I32 instruction so we have test data to assign into the array.
		const int writeLoadI32InstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 1, testValueData);
		ASSERT_EQ(writeLoadI32InstrResult, HQ_SUCCESS);

		// Write the STORE_ARRAY instruction to set the value at the specified array index to the test data.
		const int writeStoreArrayInstrResult = HqBytecodeWriteStoreArray(hFuncSerializer, 0, 1, 0);
		ASSERT_EQ(writeStoreArrayInstrResult, HQ_SUCCESS);

		// Write the LOAD_ARRAY instruction to pull the value data from the array element into a GP register for inspection.
		const int writeLoadArrayInstrResult = HqBytecodeWriteLoadArray(hFuncSerializer, 2, 0, 0);
		ASSERT_EQ(writeLoadArrayInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the array instance data.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsArray(hValue));
			ASSERT_EQ(HqValueGetArrayLength(hValue), 1);

			HqValueHandle hIndexValue = HqValueGetArrayElement(hValue, 0);

			// Validate the array element value.
			ASSERT_NE(hIndexValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hIndexValue));
			ASSERT_EQ(HqValueGetInt32(hIndexValue), testValueData);
		}

		// Verify the value extracted from the object.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Call$Script)
{
	static constexpr const char* const functionName = "int32_t test(int32_t)";
	static constexpr int32_t testValueData = 1234578;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add the function name to the module string table.
		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, functionName, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Main function
		{
			// Set the function serializer.
			_setupFunctionSerializer(hFuncSerializer, endianness);

			// Write the instruction that we're going to test.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a STORE_PARAM instruction so we can pass the test value to the function.
			const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 0, 0);
			ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

			// Write the CALL isntruction to invoke the function.
			const int writeCallInstrResult = HqBytecodeWriteCall(hFuncSerializer, stringIndex);
			ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine the registers.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

			// Finalize the serializer and add it to the module.
			_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
		}

		// Sub-function
		{
			// Set the function serializer.
			_setupFunctionSerializer(hFuncSerializer, endianness);

			// Write a LOAD_PARAM instruction to load the input parameter to a GP register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadParam(hFuncSerializer, 0, 0);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a STORE_PARAM instruction to pass the input value back to the main function.
			// We intentionally store this on different I/O register, otherwise the check would
			// be meaningless since it would functionally be the same as not calling this function.
			const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

			// Finalize the serializer and add it to the module.
			_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, functionName);
		}
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		_getIoRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Call$Native)
{
	static constexpr const char* const functionName = "int32_t testCallNative(int32_t)";
	static constexpr int32_t testValueData = 1234578;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add the function name to the module string table.
		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, functionName, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Add the native function to the module.
		const int addNativeFunctionResult = HqModuleWriterAddNativeFunction(hModuleWriter, functionName, 1, 1);
		ASSERT_EQ(addNativeFunctionResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
		ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

		// Write a STORE_PARAM instruction so we can pass the test value to the function.
		const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 0, 0);
		ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

		// Write the CALL isntruction to invoke the function.
		const int writeCallInstrResult = HqBytecodeWriteCall(hFuncSerializer, stringIndex);
		ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the registers.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		_getIoRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Call$NativeNonExistent)
{
	static constexpr const char* const functionName = "void unboundNativeFunc()";

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add the function name to the module string table.
		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, functionName, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Add the native function to the module.
		const int addNativeFunctionResult = HqModuleWriterAddNativeFunction(hModuleWriter, functionName, 0, 0);
		ASSERT_EQ(addNativeFunctionResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the CALL isntruction to attempt invoke the function.
		const int writeCallInstrResult = HqBytecodeWriteCall(hFuncSerializer, stringIndex);
		ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_TRUE(status.exception);
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

TEST_F(_HQ_TEST_NAME(TestOpCodes), CallValue$Script)
{
	static constexpr const char* const functionName = "int32_t test(int32_t)";
	static constexpr int32_t testValueData = 1234578;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add the function name to the module string table.
		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, functionName, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Main function
		{
			// Set the function serializer.
			_setupFunctionSerializer(hFuncSerializer, endianness);

			// Write the instruction that we're going to test.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write the INIT_FUNC instruction to create a value pointing to the function we want to call.
			const int writeInitFuncInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 1, stringIndex);
			ASSERT_EQ(writeInitFuncInstrResult, HQ_SUCCESS);

			// Write a STORE_PARAM instruction so we can pass the test value to the function.
			const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 0, 0);
			ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

			// Write the CALL_VALUE isntruction to invoke the function.
			const int writeCallInstrResult = HqBytecodeWriteCallValue(hFuncSerializer, 1);
			ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine the registers.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

			// Finalize the serializer and add it to the module.
			_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
		}

		// Sub-function
		{
			// Set the function serializer.
			_setupFunctionSerializer(hFuncSerializer, endianness);

			// Write a LOAD_PARAM instruction to load the input parameter to a GP register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadParam(hFuncSerializer, 0, 0);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a STORE_PARAM instruction to pass the input value back to the main function.
			// We intentionally store this on different I/O register, otherwise the check would
			// be meaningless since it would functionally be the same as not calling this function.
			const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

			// Finalize the serializer and add it to the module.
			_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, functionName);
		}
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		_getIoRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CallValue$Native)
{
	static constexpr const char* const functionName = "int32_t testCallNative(int32_t)";
	static constexpr int32_t testValueData = 1234578;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add the function name to the module string table.
		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, functionName, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Add the native function to the module.
		const int addNativeFunctionResult = HqModuleWriterAddNativeFunction(hModuleWriter, functionName, 1, 1);
		ASSERT_EQ(addNativeFunctionResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
		ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

		// Write the INIT_FUNC instruction to create a value pointing to the function we want to call.
		const int writeInitFuncInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 1, stringIndex);
		ASSERT_EQ(writeInitFuncInstrResult, HQ_SUCCESS);

		// Write a STORE_PARAM instruction so we can pass the test value to the function.
		const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 0, 0);
		ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

		// Write the CALL_VALUE isntruction to invoke the function.
		const int writeCallInstrResult = HqBytecodeWriteCallValue(hFuncSerializer, 1);
		ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the registers.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		_getIoRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CallValue$NativeNonExistent)
{
	static constexpr const char* const functionName = "void unboundNativeFunc()";

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add the function name to the module string table.
		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, functionName, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Add the native function to the module.
		const int addNativeFunctionResult = HqModuleWriterAddNativeFunction(hModuleWriter, functionName, 0, 0);
		ASSERT_EQ(addNativeFunctionResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the INIT_FUNC instruction to create a value pointing to the function we want to call.
		const int writeInitFuncInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 0, stringIndex);
		ASSERT_EQ(writeInitFuncInstrResult, HQ_SUCCESS);

		// Write the CALL_VALUE isntruction to invoke the function.
		const int writeCallInstrResult = HqBytecodeWriteCallValue(hFuncSerializer, 0);
		ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_TRUE(status.exception);
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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Raise)
{
	static constexpr const char* const objTypeName = "TestObj";
	static constexpr int32_t testValueData = 12345;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add an object type to use for testing to the module.
		const int addObjTypeResult = HqModuleWriterAddObjectType(hModuleWriter, objTypeName);
		ASSERT_EQ(addObjTypeResult, HQ_SUCCESS);

		// Add the object type name to the module's string table.
		uint32_t objStringIndex = 0;
		const int addObjStringResult = HqModuleWriterAddString(hModuleWriter, objTypeName, &objStringIndex);
		ASSERT_EQ(addObjStringResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		const size_t guardOffsetStart = HqSerializerGetStreamPosition(hFuncSerializer);

		// Write the INIT_OBJECT instruction so we have a value to raise for the exception.
		const int writeLoadObjInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 0, objStringIndex);
		ASSERT_EQ(writeLoadObjInstrResult, HQ_SUCCESS);

		// Write the RAISE instruction to test a handled exception.
		const int writeRaiseHandledInstrResult = HqBytecodeWriteRaise(hFuncSerializer, 0);
		ASSERT_EQ(writeRaiseHandledInstrResult, HQ_SUCCESS);

		const size_t guardOffsetEnd = HqSerializerGetStreamPosition(hFuncSerializer);

		// Write an ABORT instruction that should never get called.
		const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
		ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

		const size_t handlerOffset = HqSerializerGetStreamPosition(hFuncSerializer);

		// Write a YIELD instruction so we can examine the exception value.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Write the LOAD_IMM_I32 instruction as our next exception value.
		const int writeLoadI32InstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
		ASSERT_EQ(writeLoadI32InstrResult, HQ_SUCCESS);

		// Write the RAISE instruction to test an unhandled exception.
		const int writeRaiseUnhandledInstrResult = HqBytecodeWriteRaise(hFuncSerializer, 0);
		ASSERT_EQ(writeRaiseUnhandledInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

		// Add the guarded block to the module.
		uint32_t guardBlockId = 0;
		const int addGuardedBlockResult = HqModuleWriterAddGuardedBlock(
			hModuleWriter,
			Function::main,
			guardOffsetStart,
			guardOffsetEnd - guardOffsetStart,
			&guardBlockId
		);
		ASSERT_EQ(addGuardedBlockResult, HQ_SUCCESS);

		// Add an exception handler for the guarded block.
		const int addExceptionHandlerResult = HqModuleWriterAddExceptionHandler(
			hModuleWriter,
			Function::main,
			guardBlockId,
			handlerOffset,
			HQ_VALUE_TYPE_OBJECT,
			objTypeName
		);
		ASSERT_EQ(addExceptionHandlerResult, HQ_SUCCESS);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the handled exception value.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getIoRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsObject(hValue));
			ASSERT_STREQ(HqValueGetObjectTypeName(hValue), objTypeName);
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		_getExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_TRUE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the handled exception value.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getIoRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Push_Pop)
{
	static constexpr int32_t testValueData = 12345;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write the LOAD_IMM_I32 instruction so we have test data to assign into the array.
		const int writeLoadI32InstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
		ASSERT_EQ(writeLoadI32InstrResult, HQ_SUCCESS);

		// Write a PUSH instruction so we have some data on the stack.
		const int writePushInstrResult = HqBytecodeWritePush(hFuncSerializer, 0);
		ASSERT_EQ(writePushInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the stack.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Write a POP instruction so we can pull the value we pushed into a GP register.
		const int writePopInstrResult = HqBytecodeWritePop(hFuncSerializer, 1);
		ASSERT_EQ(writePopInstrResult, HQ_SUCCESS);

		// Write one more YIELD instruction so we can examine the GP register.
		const int writeAnotherYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeAnotherYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the stack value.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getStackValue(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the value extracted from the object.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Jmp)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		JumpInstruction jump;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Begin the jump block.
		jump.Begin(hFuncSerializer, JumpInstruction::Behavior::Forward, JumpInstruction::Condition::None, 0);

		// Write an ABORT opcode just so we have an error case to check for
		// in case the JMP opcode doesn't branch correctly.
		const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
		ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

		// End the block indicating where the instruction pointer will jump to.
		jump.End();

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
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

TEST_F(_HQ_TEST_NAME(TestOpCodes), JmpIfTrue)
{
	static constexpr const char* const functionName = "void test()";
	static constexpr const char* const objTypeName = "TestObjType";

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		JumpInstruction jump;

		// Add a non-empty string to the module.
		uint32_t nonEmptyStringIndex = 0;
		const int addNonEmptyStringResult = HqModuleWriterAddString(hModuleWriter, "test", &nonEmptyStringIndex);
		ASSERT_EQ(addNonEmptyStringResult, HQ_SUCCESS);

		// Add a string for the object type.
		uint32_t objTypeStringIndex = 0;
		const int addObjTypeStringResult = HqModuleWriterAddString(hModuleWriter, objTypeName, &objTypeStringIndex);
		ASSERT_EQ(addObjTypeStringResult, HQ_SUCCESS);

		// Add a string to the function name.
		uint32_t funcStringIndex = 0;
		const int addFuncStringResult = HqModuleWriterAddString(hModuleWriter, functionName, &funcStringIndex);
		ASSERT_EQ(addFuncStringResult, HQ_SUCCESS);

		// Add the test object type.
		const int addObjTypeResult = HqModuleWriterAddObjectType(hModuleWriter, objTypeName);
		ASSERT_EQ(addObjTypeResult, HQ_SUCCESS);

		// Add the native function to the module that is not bound to anything.
		// We won't call it, we just need the runtime to *think* it exists.
		const int addDummyNativeFunctionResult = HqModuleWriterAddNativeFunction(hModuleWriter, functionName, 0, 0);
		ASSERT_EQ(addDummyNativeFunctionResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Load the immediate values that we expect evaluate to true.
		{
			const int writeLoadBoolInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 0, true);
			ASSERT_EQ(writeLoadBoolInstrResult, HQ_SUCCESS);

			const int writeLoadI8InstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 1, 123);
			ASSERT_EQ(writeLoadI8InstrResult, HQ_SUCCESS);

			const int writeLoadI16InstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 2, 12345);
			ASSERT_EQ(writeLoadI16InstrResult, HQ_SUCCESS);

			const int writeLoadI32InstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 3, 1234567);
			ASSERT_EQ(writeLoadI32InstrResult, HQ_SUCCESS);

			const int writeLoadI64InstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 4, 12345678901ll);
			ASSERT_EQ(writeLoadI64InstrResult, HQ_SUCCESS);

			const int writeLoadU8InstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 5, 234);
			ASSERT_EQ(writeLoadU8InstrResult, HQ_SUCCESS);

			const int writeLoadU16InstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 6, 23456);
			ASSERT_EQ(writeLoadU16InstrResult, HQ_SUCCESS);

			const int writeLoadU32InstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 7, 2345678u);
			ASSERT_EQ(writeLoadU32InstrResult, HQ_SUCCESS);

			const int writeLoadU64InstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 8, 23456789012ull);
			ASSERT_EQ(writeLoadU64InstrResult, HQ_SUCCESS);

			const int writeLoadF32InstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 9, 3.1415926535897932384626433832795f);
			ASSERT_EQ(writeLoadF32InstrResult, HQ_SUCCESS);

			const int writeLoadF64InstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 10, 6.283185307179586476925286766559);
			ASSERT_EQ(writeLoadF64InstrResult, HQ_SUCCESS);

			const int writeLoadStrInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 11, nonEmptyStringIndex);
			ASSERT_EQ(writeLoadStrInstrResult, HQ_SUCCESS);

			const int writeInitArrayInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 12, 1);
			ASSERT_EQ(writeInitArrayInstrResult, HQ_SUCCESS);

			const int writeInitObjInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 13, objTypeStringIndex);
			ASSERT_EQ(writeInitObjInstrResult, HQ_SUCCESS);

			const int writeInitFuncInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 14, funcStringIndex);
			ASSERT_EQ(writeInitFuncInstrResult, HQ_SUCCESS);
		}

		auto writeJumpBlock = [hFuncSerializer](const uint32_t gpRegIndex)
		{
			JumpInstruction jump;

			// Begin the jump block.
			jump.Begin(hFuncSerializer, JumpInstruction::Behavior::Forward, JumpInstruction::Condition::IfTrue, gpRegIndex);

			// Write an ABORT opcode just so we have an error case to check for
			// in case the jump operation doesn't branch correctly.
			const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
			ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

			// End the block indicating where the instruction pointer will jump to.
			jump.End();
		};

		writeJumpBlock(0); // bool
		writeJumpBlock(1); // int8
		writeJumpBlock(2); // int16
		writeJumpBlock(3); // int32
		writeJumpBlock(4); // int64
		writeJumpBlock(5); // uint8
		writeJumpBlock(6); // uint16
		writeJumpBlock(7); // uint32
		writeJumpBlock(8); // uint64
		writeJumpBlock(9); // float32
		writeJumpBlock(10); // float64
		writeJumpBlock(11); // string
		writeJumpBlock(12); // array
		writeJumpBlock(13); // object
		writeJumpBlock(14); // function

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
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

TEST_F(_HQ_TEST_NAME(TestOpCodes), JmpIfFalse)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		JumpInstruction jump;

		// Add an empty string to the module.
		uint32_t emptyStringIndex = 0;
		const int addEmptyStringResult = HqModuleWriterAddString(hModuleWriter, "", &emptyStringIndex);
		ASSERT_EQ(addEmptyStringResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Load the immediate values that we expect evaluate to true.
		{
			const int writeLoadNullInstrResult = HqBytecodeWriteLoadImmNull(hFuncSerializer, 0);
			ASSERT_EQ(writeLoadNullInstrResult, HQ_SUCCESS);

			const int writeLoadBoolInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 1, false);
			ASSERT_EQ(writeLoadBoolInstrResult, HQ_SUCCESS);

			const int writeLoadI8InstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 2, 0);
			ASSERT_EQ(writeLoadI8InstrResult, HQ_SUCCESS);

			const int writeLoadI16InstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 3, 0);
			ASSERT_EQ(writeLoadI16InstrResult, HQ_SUCCESS);

			const int writeLoadI32InstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 4, 0);
			ASSERT_EQ(writeLoadI32InstrResult, HQ_SUCCESS);

			const int writeLoadI64InstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 5, 0);
			ASSERT_EQ(writeLoadI64InstrResult, HQ_SUCCESS);

			const int writeLoadU8InstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 6, 0);
			ASSERT_EQ(writeLoadU8InstrResult, HQ_SUCCESS);

			const int writeLoadU16InstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 7, 0);
			ASSERT_EQ(writeLoadU16InstrResult, HQ_SUCCESS);

			const int writeLoadU32InstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 8, 0);
			ASSERT_EQ(writeLoadU32InstrResult, HQ_SUCCESS);

			const int writeLoadU64InstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 9, 0);
			ASSERT_EQ(writeLoadU64InstrResult, HQ_SUCCESS);

			const int writeLoadF32InstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 10, 0.0f);
			ASSERT_EQ(writeLoadF32InstrResult, HQ_SUCCESS);

			const int writeLoadF64InstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 11, 0.0);
			ASSERT_EQ(writeLoadF64InstrResult, HQ_SUCCESS);

			const int writeLoadStrInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 12, emptyStringIndex);
			ASSERT_EQ(writeLoadStrInstrResult, HQ_SUCCESS);

			const int writeInitArrayInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 13, 0);
			ASSERT_EQ(writeInitArrayInstrResult, HQ_SUCCESS);
		}

		auto writeJumpBlock = [hFuncSerializer](const uint32_t gpRegIndex)
		{
			JumpInstruction jump;

			// Begin the jump block.
			jump.Begin(hFuncSerializer, JumpInstruction::Behavior::Forward, JumpInstruction::Condition::IfFalse, gpRegIndex);

			// Write an ABORT opcode just so we have an error case to check for
			// in case the jump operation doesn't branch correctly.
			const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
			ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

			// End the block indicating where the instruction pointer will jump to.
			jump.End();
		};

		writeJumpBlock(0); // null
		writeJumpBlock(1); // bool
		writeJumpBlock(2); // int8
		writeJumpBlock(3); // int16
		writeJumpBlock(4); // int32
		writeJumpBlock(5); // int64
		writeJumpBlock(6); // uint8
		writeJumpBlock(7); // uint16
		writeJumpBlock(8); // uint32
		writeJumpBlock(9); // uint64
		writeJumpBlock(10); // float32
		writeJumpBlock(11); // float64
		writeJumpBlock(12); // string
		writeJumpBlock(13); // array

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Length)
{
	static constexpr const char* const testString = "test";
	static constexpr uint32_t arrayLength = 10;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		uint32_t stringIndex = 0;
		const int addStringResult = HqModuleWriterAddString(hModuleWriter, testString, &stringIndex);
		ASSERT_EQ(addStringResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Write an INIT_ARRAY instruction to create an array we can query.
		const int writeInitArrayInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 0, arrayLength);
		ASSERT_EQ(writeInitArrayInstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_STR instruction to create a string value we can query.
		const int writeLoadStrInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 1, stringIndex);
		ASSERT_EQ(writeLoadStrInstrResult, HQ_SUCCESS);

		// Write a LOAD_IMM_NULL instruction to demonstrate a failure when attempting to get the length of an invalid value type.
		const int writeLoadNullInstrResult = HqBytecodeWriteLoadImmNull(hFuncSerializer, 2);
		ASSERT_EQ(writeLoadNullInstrResult, HQ_SUCCESS);

		// Write a LENGTH instruction to query the length of a string.
		const int writeLengthStrInstrResult = HqBytecodeWriteLength(hFuncSerializer, 0, 0);
		ASSERT_EQ(writeLengthStrInstrResult, HQ_SUCCESS);

		// Write a LENGTH instruction to query the length of an array.
		const int writeLengthArrayInstrResult = HqBytecodeWriteLength(hFuncSerializer, 1, 1);
		ASSERT_EQ(writeLengthArrayInstrResult, HQ_SUCCESS);

		// Write a LENGTH instruction to attempt to query an invalid value type.
		const int writeLengthNullInstrResult = HqBytecodeWriteLength(hFuncSerializer, 2, 2);
		ASSERT_EQ(writeLengthNullInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_TRUE(status.exception);
		ASSERT_FALSE(status.abort);

		// Array length
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), arrayLength);
		}

		// String length
		{
			const size_t stringLength = strlen(testString);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), stringLength);
		}

		// Null
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_EQ(hValue, HQ_VALUE_HANDLE_NULL);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Add)
{
	static constexpr const char* const leftTestString = "test0";
	static constexpr const char* const rightTestString = "test1";

	static constexpr int8_t leftValueInt8 = 12;
	static constexpr int8_t rightValueInt8 = 23;

	static constexpr int16_t leftValueInt16 = 1234;
	static constexpr int16_t rightValueInt16 = 2345;

	static constexpr int32_t leftValueInt32 = 123456;
	static constexpr int32_t rightValueInt32 = 234567;

	static constexpr int64_t leftValueInt64 = 12345678901ll;
	static constexpr int64_t rightValueInt64 = 23456789012ll;

	static constexpr uint8_t leftValueUint8 = 23;
	static constexpr uint8_t rightValueUint8 = 34;

	static constexpr uint16_t leftValueUint16 = 2345;
	static constexpr uint16_t rightValueUint16 = 3456;

	static constexpr uint32_t leftValueUint32 = 234567;
	static constexpr uint32_t rightValueUint32 = 345678;

	static constexpr uint64_t leftValueUint64 = 23456789012ull;
	static constexpr uint64_t rightValueUint64 = 34567890123ull;

	static constexpr float leftValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr float rightValueFloat32 = 6.283185307179586476925286766559f;

	static constexpr double leftValueFloat64 = 2.7182818284590452353602874713527;
	static constexpr double rightValueFloat64 = 5.4365636569180904707205749427053;

	static constexpr size_t leftArraySize = 1;
	static constexpr size_t rightArraySize = 2;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		uint32_t leftStringIndex = 0;
		const int addLeftStringResult = HqModuleWriterAddString(hModuleWriter, leftTestString, &leftStringIndex);
		ASSERT_EQ(addLeftStringResult, HQ_SUCCESS);

		uint32_t rightStringIndex = 0;
		const int addRightStringResult = HqModuleWriterAddString(hModuleWriter, rightTestString, &rightStringIndex);
		ASSERT_EQ(addRightStringResult, HQ_SUCCESS);

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// string
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, rightStringIndex);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 11, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// array
		{
			const int writeInitLeftInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 20, leftArraySize);
			ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

			const int writeInitRightInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 21, rightArraySize);
			ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteAdd(hFuncSerializer, 12, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 + rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 + rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 + rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 + rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 + rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 + rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 + rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 + rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), leftValueFloat32 + rightValueFloat32);
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), leftValueFloat64 + rightValueFloat64);
		}

		// string
		{
			const size_t leftLength = strlen(leftTestString);
			const size_t rightLength = strlen(rightTestString);
			const size_t expectedLength = leftLength + rightLength;

			char* const expectedResult = reinterpret_cast<char*>(alloca(sizeof(char) * (expectedLength + 1)));
			expectedResult[0] = '\0';

			strcpy(expectedResult, leftTestString);
			strcat(expectedResult, rightTestString);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 11);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_EQ(HqValueGetStringLength(hValue), expectedLength);
			ASSERT_STREQ(HqValueGetString(hValue), expectedResult);
		}

		// array
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 12);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsArray(hValue));
			ASSERT_EQ(HqValueGetArrayLength(hValue), leftArraySize + rightArraySize);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Sub)
{
	static constexpr int8_t leftValueInt8 = 23;
	static constexpr int8_t rightValueInt8 = 12;

	static constexpr int16_t leftValueInt16 = 2345;
	static constexpr int16_t rightValueInt16 = 1234;

	static constexpr int32_t leftValueInt32 = 234567;
	static constexpr int32_t rightValueInt32 = 123456;

	static constexpr int64_t leftValueInt64 = 23456789012ll;
	static constexpr int64_t rightValueInt64 = 12345678901ll;

	static constexpr uint8_t leftValueUint8 = 34;
	static constexpr uint8_t rightValueUint8 = 23;

	static constexpr uint16_t leftValueUint16 = 3456;
	static constexpr uint16_t rightValueUint16 = 2345;

	static constexpr uint32_t leftValueUint32 = 345678;
	static constexpr uint32_t rightValueUint32 = 234567;

	static constexpr uint64_t leftValueUint64 = 34567890123ull;
	static constexpr uint64_t rightValueUint64 = 23456789012ull;

	static constexpr float leftValueFloat32 = 6.283185307179586476925286766559f;
	static constexpr float rightValueFloat32 = 3.1415926535897932384626433832795f;

	static constexpr double leftValueFloat64 = 5.4365636569180904707205749427053;
	static constexpr double rightValueFloat64 = 2.7182818284590452353602874713527;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteSub(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 - rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 - rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 - rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 - rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 - rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 - rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 - rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 - rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), leftValueFloat32 - rightValueFloat32);
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), leftValueFloat64 - rightValueFloat64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Mul)
{
	static constexpr int8_t leftValueInt8 = 12;
	static constexpr int8_t rightValueInt8 = 2;

	static constexpr int16_t leftValueInt16 = 1234;
	static constexpr int16_t rightValueInt16 = 3;

	static constexpr int32_t leftValueInt32 = 123456;
	static constexpr int32_t rightValueInt32 = 4;

	static constexpr int64_t leftValueInt64 = 12345678901ll;
	static constexpr int64_t rightValueInt64 = 5ll;

	static constexpr uint8_t leftValueUint8 = 23;
	static constexpr uint8_t rightValueUint8 = 3;

	static constexpr uint16_t leftValueUint16 = 2345;
	static constexpr uint16_t rightValueUint16 = 4;

	static constexpr uint32_t leftValueUint32 = 234567;
	static constexpr uint32_t rightValueUint32 = 5u;

	static constexpr uint64_t leftValueUint64 = 23456789012ull;
	static constexpr uint64_t rightValueUint64 = 6ull;

	static constexpr float leftValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr float rightValueFloat32 = 6.283185307179586476925286766559f;

	static constexpr double leftValueFloat64 = 2.7182818284590452353602874713527;
	static constexpr double rightValueFloat64 = 5.4365636569180904707205749427053;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMul(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), false);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 * rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 * rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 * rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 * rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 * rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 * rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 * rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 * rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), leftValueFloat32 * rightValueFloat32);
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), leftValueFloat64 * rightValueFloat64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Div)
{
	static constexpr int8_t leftValueInt8 = 12;
	static constexpr int8_t rightValueInt8 = 2;

	static constexpr int16_t leftValueInt16 = 1234;
	static constexpr int16_t rightValueInt16 = 3;

	static constexpr int32_t leftValueInt32 = 123456;
	static constexpr int32_t rightValueInt32 = 4;

	static constexpr int64_t leftValueInt64 = 12345678901ll;
	static constexpr int64_t rightValueInt64 = 5ll;

	static constexpr uint8_t leftValueUint8 = 23;
	static constexpr uint8_t rightValueUint8 = 3;

	static constexpr uint16_t leftValueUint16 = 2345;
	static constexpr uint16_t rightValueUint16 = 4;

	static constexpr uint32_t leftValueUint32 = 234567;
	static constexpr uint32_t rightValueUint32 = 5u;

	static constexpr uint64_t leftValueUint64 = 23456789012ull;
	static constexpr uint64_t rightValueUint64 = 6ull;

	static constexpr float leftValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr float rightValueFloat32 = 6.283185307179586476925286766559f;

	static constexpr double leftValueFloat64 = 2.7182818284590452353602874713527;
	static constexpr double rightValueFloat64 = 5.4365636569180904707205749427053;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), false);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 / rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 / rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 / rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 / rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 / rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 / rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 / rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 / rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), leftValueFloat32 / rightValueFloat32);
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), leftValueFloat64 / rightValueFloat64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Mod)
{
	static constexpr int8_t leftValueInt8 = 12;
	static constexpr int8_t rightValueInt8 = 2;

	static constexpr int16_t leftValueInt16 = 1234;
	static constexpr int16_t rightValueInt16 = 3;

	static constexpr int32_t leftValueInt32 = 123456;
	static constexpr int32_t rightValueInt32 = 4;

	static constexpr int64_t leftValueInt64 = 12345678901ll;
	static constexpr int64_t rightValueInt64 = 5ll;

	static constexpr uint8_t leftValueUint8 = 23;
	static constexpr uint8_t rightValueUint8 = 3;

	static constexpr uint16_t leftValueUint16 = 2345;
	static constexpr uint16_t rightValueUint16 = 4;

	static constexpr uint32_t leftValueUint32 = 234567;
	static constexpr uint32_t rightValueUint32 = 5u;

	static constexpr uint64_t leftValueUint64 = 23456789012ull;
	static constexpr uint64_t rightValueUint64 = 6ull;

	static constexpr float leftValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr float rightValueFloat32 = 6.283185307179586476925286766559f;

	static constexpr double leftValueFloat64 = 2.7182818284590452353602874713527;
	static constexpr double rightValueFloat64 = 5.4365636569180904707205749427053;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteMod(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 % rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 % rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 % rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 % rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 % rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 % rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 % rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 % rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), fmodf(leftValueFloat32, rightValueFloat32));
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), fmod(leftValueFloat64, rightValueFloat64));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Exp)
{
	static constexpr int8_t leftValueInt8 = 2;
	static constexpr int8_t rightValueInt8 = 2;

	static constexpr int16_t leftValueInt16 = 34;
	static constexpr int16_t rightValueInt16 = 3;

	static constexpr int32_t leftValueInt32 = 56;
	static constexpr int32_t rightValueInt32 = 4;

	static constexpr int64_t leftValueInt64 = 901ll;
	static constexpr int64_t rightValueInt64 = 5ll;

	static constexpr uint8_t leftValueUint8 = 3;
	static constexpr uint8_t rightValueUint8 = 3;

	static constexpr uint16_t leftValueUint16 = 45;
	static constexpr uint16_t rightValueUint16 = 4;

	static constexpr uint32_t leftValueUint32 = 67u;
	static constexpr uint32_t rightValueUint32 = 5u;

	static constexpr uint64_t leftValueUint64 = 901ull;
	static constexpr uint64_t rightValueUint64 = 6ull;

	static constexpr float leftValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr float rightValueFloat32 = 6.283185307179586476925286766559f;

	static constexpr double leftValueFloat64 = 2.7182818284590452353602874713527;
	static constexpr double rightValueFloat64 = 5.4365636569180904707205749427053;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteExp(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), false);
		}

		// int8
		{
			// Calculate the expected result.
			int8_t result = leftValueInt8;
			for(int8_t i = 0; i < rightValueInt8 - 1; ++i)
			{
				result *= leftValueInt8;
			}

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), result);
		}

		// int16
		{
			// Calculate the expected result.
			int16_t result = leftValueInt16;
			for(int16_t i = 0; i < rightValueInt16 - 1; ++i)
			{
				result *= leftValueInt16;
			}

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), result);
		}

		// int32
		{
			// Calculate the expected result.
			int32_t result = leftValueInt32;
			for(int32_t i = 0; i < rightValueInt32 - 1; ++i)
			{
				result *= leftValueInt32;
			}

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), result);
		}

		// int64
		{
			// Calculate the expected result.
			int64_t result = leftValueInt64;
			for(int64_t i = 0; i < rightValueInt64 - 1; ++i)
			{
				result *= leftValueInt64;
			}

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), result);
		}

		// uint8
		{
			// Calculate the expected result.
			uint8_t result = leftValueUint8;
			for(uint8_t i = 0; i < rightValueUint8 - 1; ++i)
			{
				result *= leftValueUint8;
			}

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), result);
		}

		// uint16
		{
			// Calculate the expected result.
			uint16_t result = leftValueUint16;
			for(uint16_t i = 0; i < rightValueUint16 - 1; ++i)
			{
				result *= leftValueUint16;
			}

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), result);
		}

		// uint32
		{
			// Calculate the expected result.
			uint32_t result = leftValueUint32;
			for(uint32_t i = 0; i < rightValueUint32 - 1; ++i)
			{
				result *= leftValueUint32;
			}

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), result);
		}

		// uint64
		{
			// Calculate the expected result.
			uint64_t result = leftValueUint64;
			for(uint64_t i = 0; i < rightValueUint64 - 1; ++i)
			{
				result *= leftValueUint64;
			}

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), result);
		}

		// float32
		{
			const float result = powf(leftValueFloat32, rightValueFloat32);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), result);
		}

		// float64
		{
			const double result = pow(leftValueFloat64, rightValueFloat64);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), result);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), BitAnd)
{
	static constexpr int8_t leftValueInt8 = 23;
	static constexpr int8_t rightValueInt8 = 12;

	static constexpr int16_t leftValueInt16 = 2345;
	static constexpr int16_t rightValueInt16 = 1234;

	static constexpr int32_t leftValueInt32 = 234567;
	static constexpr int32_t rightValueInt32 = 123456;

	static constexpr int64_t leftValueInt64 = 23456789012ll;
	static constexpr int64_t rightValueInt64 = 12345678901ll;

	static constexpr uint8_t leftValueUint8 = 34;
	static constexpr uint8_t rightValueUint8 = 23;

	static constexpr uint16_t leftValueUint16 = 3456;
	static constexpr uint16_t rightValueUint16 = 2345;

	static constexpr uint32_t leftValueUint32 = 345678;
	static constexpr uint32_t rightValueUint32 = 234567;

	static constexpr uint64_t leftValueUint64 = 34567890123ull;
	static constexpr uint64_t rightValueUint64 = 23456789012ull;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), false);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 & rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 & rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 & rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 & rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 & rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 & rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 & rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 & rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), BitOr)
{
	static constexpr int8_t leftValueInt8 = 23;
	static constexpr int8_t rightValueInt8 = 12;

	static constexpr int16_t leftValueInt16 = 2345;
	static constexpr int16_t rightValueInt16 = 1234;

	static constexpr int32_t leftValueInt32 = 234567;
	static constexpr int32_t rightValueInt32 = 123456;

	static constexpr int64_t leftValueInt64 = 23456789012ll;
	static constexpr int64_t rightValueInt64 = 12345678901ll;

	static constexpr uint8_t leftValueUint8 = 34;
	static constexpr uint8_t rightValueUint8 = 23;

	static constexpr uint16_t leftValueUint16 = 3456;
	static constexpr uint16_t rightValueUint16 = 2345;

	static constexpr uint32_t leftValueUint32 = 345678;
	static constexpr uint32_t rightValueUint32 = 234567;

	static constexpr uint64_t leftValueUint64 = 34567890123ull;
	static constexpr uint64_t rightValueUint64 = 23456789012ull;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 | rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 | rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 | rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 | rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 | rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 | rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 | rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 | rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), BitXor)
{
	static constexpr int8_t leftValueInt8 = 23;
	static constexpr int8_t rightValueInt8 = 12;

	static constexpr int16_t leftValueInt16 = 2345;
	static constexpr int16_t rightValueInt16 = 1234;

	static constexpr int32_t leftValueInt32 = 234567;
	static constexpr int32_t rightValueInt32 = 123456;

	static constexpr int64_t leftValueInt64 = 23456789012ll;
	static constexpr int64_t rightValueInt64 = 12345678901ll;

	static constexpr uint8_t leftValueUint8 = 34;
	static constexpr uint8_t rightValueUint8 = 23;

	static constexpr uint16_t leftValueUint16 = 3456;
	static constexpr uint16_t rightValueUint16 = 2345;

	static constexpr uint32_t leftValueUint32 = 345678;
	static constexpr uint32_t rightValueUint32 = 234567;

	static constexpr uint64_t leftValueUint64 = 34567890123ull;
	static constexpr uint64_t rightValueUint64 = 23456789012ull;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 ^ rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 ^ rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 ^ rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 ^ rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 ^ rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 ^ rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 ^ rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 ^ rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), LeftShift)
{
	static constexpr int8_t leftValueInt8 = 12;
	static constexpr int8_t rightValueInt8 = 2;

	static constexpr int16_t leftValueInt16 = 1234;
	static constexpr int16_t rightValueInt16 = 3;

	static constexpr int32_t leftValueInt32 = 123456;
	static constexpr int32_t rightValueInt32 = 4;

	static constexpr int64_t leftValueInt64 = 12345678901ll;
	static constexpr int64_t rightValueInt64 = 5ll;

	static constexpr uint8_t leftValueUint8 = 23;
	static constexpr uint8_t rightValueUint8 = 3;

	static constexpr uint16_t leftValueUint16 = 2345;
	static constexpr uint16_t rightValueUint16 = 4;

	static constexpr uint32_t leftValueUint32 = 234567;
	static constexpr uint32_t rightValueUint32 = 5u;

	static constexpr uint64_t leftValueUint64 = 23456789012ull;
	static constexpr uint64_t rightValueUint64 = 6ull;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 << rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 << rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 << rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 << rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 << rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 << rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 << rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 << rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), RightShift)
{
	static constexpr int8_t leftValueInt8 = 12;
	static constexpr int8_t rightValueInt8 = 2;

	static constexpr int16_t leftValueInt16 = 1234;
	static constexpr int16_t rightValueInt16 = 3;

	static constexpr int32_t leftValueInt32 = 123456;
	static constexpr int32_t rightValueInt32 = 4;

	static constexpr int64_t leftValueInt64 = 12345678901ll;
	static constexpr int64_t rightValueInt64 = 5ll;

	static constexpr uint8_t leftValueUint8 = 23;
	static constexpr uint8_t rightValueUint8 = 3;

	static constexpr uint16_t leftValueUint16 = 2345;
	static constexpr uint16_t rightValueUint16 = 4;

	static constexpr uint32_t leftValueUint32 = 234567;
	static constexpr uint32_t rightValueUint32 = 5u;

	static constexpr uint64_t leftValueUint64 = 23456789012ull;
	static constexpr uint64_t rightValueUint64 = 6ull;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 >> rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 >> rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 >> rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 >> rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 >> rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 >> rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 >> rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 >> rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), LeftRotate)
{
	static constexpr int8_t leftValueInt8 = 12;
	static constexpr int8_t rightValueInt8 = 2;

	static constexpr int16_t leftValueInt16 = 1234;
	static constexpr int16_t rightValueInt16 = 3;

	static constexpr int32_t leftValueInt32 = 123456;
	static constexpr int32_t rightValueInt32 = 4;

	static constexpr int64_t leftValueInt64 = 12345678901ll;
	static constexpr int64_t rightValueInt64 = 5ll;

	static constexpr uint8_t leftValueUint8 = 23;
	static constexpr uint8_t rightValueUint8 = 3;

	static constexpr uint16_t leftValueUint16 = 2345;
	static constexpr uint16_t rightValueUint16 = 4;

	static constexpr uint32_t leftValueUint32 = 234567;
	static constexpr uint32_t rightValueUint32 = 5u;

	static constexpr uint64_t leftValueUint64 = 23456789012ull;
	static constexpr uint64_t rightValueUint64 = 6ull;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// int8
		{
			const int8_t result = ((leftValueInt8 << (rightValueInt8 % 8)) & 0xFF) | (leftValueInt8 >> (8 - (rightValueInt8 % 8)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), result);
		}

		// int16
		{
			const int16_t result = ((leftValueInt16 << (rightValueInt16 % 16)) & 0xFFFF) | (leftValueInt16 >> (16 - (rightValueInt16 % 16)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), result);
		}

		// int32
		{
			const int32_t result = ((leftValueInt32 << (rightValueInt32 % 32)) & 0xFFFFFFFF) | (leftValueInt32 >> (32 - (rightValueInt32 % 32)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), result);
		}

		// int64
		{
			const int64_t result = (leftValueInt64 << (rightValueInt64 % 64)) | (leftValueInt64 >> (64 - (rightValueInt64 % 64)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), result);
		}

		// uint8
		{
			const uint8_t result = ((leftValueUint8 << (rightValueUint8 % 8)) & 0xFF) | (leftValueUint8 >> (8 - (rightValueUint8 % 8)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), result);
		}

		// uint16
		{
			const uint16_t result = ((leftValueUint16 << (rightValueUint16 % 16)) & 0xFFFF) | (leftValueUint16 >> (16 - (rightValueUint16 % 16)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), result);
		}

		// uint32
		{
			const uint32_t result = ((leftValueUint32 << (rightValueUint32 % 32)) & 0xFFFFFFFF) | (leftValueUint32 >> (32 - (rightValueUint32 % 32)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), result);
		}

		// uint64
		{
			const uint64_t result = (leftValueUint64 << (rightValueUint64 % 64)) | (leftValueUint64 >> (64 - (rightValueUint64 % 64)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), result);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), RightRotate)
{
	static constexpr int8_t leftValueInt8 = 12;
	static constexpr int8_t rightValueInt8 = 16;

	static constexpr int16_t leftValueInt16 = 1234;
	static constexpr int16_t rightValueInt16 = 3;

	static constexpr int32_t leftValueInt32 = 123456;
	static constexpr int32_t rightValueInt32 = 4;

	static constexpr int64_t leftValueInt64 = 12345678901ll;
	static constexpr int64_t rightValueInt64 = 5ll;

	static constexpr uint8_t leftValueUint8 = 23;
	static constexpr uint8_t rightValueUint8 = 3;

	static constexpr uint16_t leftValueUint16 = 2345;
	static constexpr uint16_t rightValueUint16 = 4;

	static constexpr uint32_t leftValueUint32 = 234567;
	static constexpr uint32_t rightValueUint32 = 5u;

	static constexpr uint64_t leftValueUint64 = 23456789012ull;
	static constexpr uint64_t rightValueUint64 = 6ull;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		_setupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeAddInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeAddInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		_finalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		_getExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// int8
		{
			const int8_t result = (leftValueInt8 >> (rightValueInt8 % 8)) | ((leftValueInt8 << (8 - (rightValueInt8 % 8))) & 0xFF);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), result);
		}

		// int16
		{
			const int16_t result = (leftValueInt16 >> (rightValueInt16 % 16)) | ((leftValueInt16 << (16 - (rightValueInt16 % 16))) & 0xFFFF);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), result);
		}

		// int32
		{
			const int32_t result = (leftValueInt32 >> (rightValueInt32 % 32)) | ((leftValueInt32 << (32 - (rightValueInt32 % 32))) & 0xFFFFFFFF);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), result);
		}

		// int64
		{
			const int64_t result = (leftValueInt64 >> (rightValueInt64 % 64)) | (leftValueInt64 << (64 - (rightValueInt64 % 64)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), result);
		}

		// uint8
		{
			const uint8_t result = (leftValueUint8 >> (rightValueUint8 % 8)) | ((leftValueUint8 << (8 - (rightValueUint8 % 8))) & 0xFF);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), result);
		}

		// uint16
		{
			const uint16_t result = (leftValueUint16 >> (rightValueUint16 % 16)) | ((leftValueUint16 << (16 - (rightValueUint16 % 16))) & 0xFFFF);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), result);
		}

		// uint32
		{
			const uint32_t result = (leftValueUint32 >> (rightValueUint32 % 32)) | ((leftValueUint32 << (32 - (rightValueUint32 % 32))) & 0xFFFFFFFF);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), result);
		}

		// uint64
		{
			const uint64_t result = (leftValueUint64 >> (rightValueUint64 % 64)) | (leftValueUint64 << (64 - (rightValueUint64 % 64)));

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			_getGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), result);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------
