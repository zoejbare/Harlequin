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

#include "../FuncTestUtil.hpp"
#include "../Memory.hpp"

#include <tool/compiler/ControlFlow.hpp>
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
};

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Return)
{
	std::vector<uint8_t> bytecode;

	// Compiling with just an empty init function is enough to test the RETURN opcode since
	// that gets added to the module by default if no other bytecode is supplied for it.
	Util::CompileBytecode(bytecode, nullptr);
	ASSERT_GT(bytecode.size(), 0u);

	Util::ProcessBytecode("TestOpCodes", nullptr, nullptr, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Nop)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeNopInstrResult = HqBytecodeWriteNop(hFuncSerializer);
		ASSERT_EQ(writeNopInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_TRUE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Abort)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
		ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_TRUE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Yield)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the null value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_EQ(hValue, HQ_VALUE_HANDLE_NULL);
		}

		// Verify the boolean value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Verify the int8 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), testValueDataI8);
		}

		// Verify the int16 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), testValueDataI16);
		}

		// Verify the int32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueDataI32);
		}

		// Verify the int64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), testValueDataI64);
		}

		// Verify the uint8 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), testValueDataU8);
		}

		// Verify the uint16 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), testValueDataU16);
		}

		// Verify the uint32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), testValueDataU32);
		}

		// Verify the uint64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), testValueDataU64);
		}

		// Verify the float32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), testValueDataF32);
		}

		// Verify the float64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 11);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), testValueDataF64);
		}

		// Verify the string value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 12);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STRCASEEQ(HqValueGetString(hValue), testValueDataStr);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
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
		Util::GetGpRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), LoadParam_StoreParam)
{
	static constexpr int32_t testValueData = 12345678;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write a LOAD_IMM instruction so we have data to put in the I/O register.
		const int writeLoadImmInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData);
		ASSERT_EQ(writeLoadImmInstrResult, HQ_SUCCESS);

		// Write the STORE_PARAM instruction to have some data to work with.
		const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 2, 0);
		ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

		// Write the LOAD_PARAM instruction to a GP register that we can inspect.
		const int writeLoadGlobalInstrResult = HqBytecodeWriteLoadParam(hFuncSerializer, 1, 2);
		ASSERT_EQ(writeLoadGlobalInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the GP register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		Util::GetGpRegister(hValue, hExec, 1);

		// Validate the GP register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);

		// Get the I/O register value we want to inspect.
		Util::GetIoRegister(hValue, hExec, 2);

		// Validate the I/O register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), LoadVariable_StoreVariable)
{
	static constexpr int32_t testValueData = 12345678;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write a LOAD_IMM instruction so we have data to put in the I/O register.
		ASSERT_EQ(HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueData), HQ_SUCCESS);

		// Write the STORE_VAR instruction to give have some data to work with.
		ASSERT_EQ(HqBytecodeWriteStoreVariable(hFuncSerializer, 2, 0), HQ_SUCCESS);

		// Write the LOAD_VAR instruction to a GP register that we can inspect.
		ASSERT_EQ(HqBytecodeWriteLoadVariable(hFuncSerializer, 1, 2), HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the values.
		ASSERT_EQ(HqBytecodeWriteYield(hFuncSerializer), HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the GP register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		Util::GetGpRegister(hValue, hExec, 1);

		// Validate the GP register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);

		// Get the variable register value we want to inspect.
		Util::GetVrRegister(hValue, hExec, 2);

		// Validate the variable register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the object instance data.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

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
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), InitArray_LoadArray_StoreArray)
{
	static constexpr int32_t testValueData = 12345;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the INIT_ARRAY instruction to initialize an instance of an array into a GP register.
		ASSERT_EQ(HqBytecodeWriteInitArray(hFuncSerializer, 0, 1), HQ_SUCCESS);

		// Write the LOAD_IMM_I32 instruction so we have test data to assign into the array.
		ASSERT_EQ(HqBytecodeWriteLoadImmI32(hFuncSerializer, 1, testValueData), HQ_SUCCESS);

		// Write the LOAD_IMM_I8 instruction with the array index.
		ASSERT_EQ(HqBytecodeWriteLoadImmI8(hFuncSerializer, 2, 0), HQ_SUCCESS);

		// Write the STORE_ARRAY instruction to set the value at the specified array index to the test data.
		ASSERT_EQ(HqBytecodeWriteStoreArray(hFuncSerializer, 0, 1, 2), HQ_SUCCESS);

		// Write the LOAD_ARRAY instruction to pull the value data from the array element into a GP register for inspection.
		ASSERT_EQ(HqBytecodeWriteLoadArray(hFuncSerializer, 2, 0, 2), HQ_SUCCESS);

		// Write a YIELD instruction so we can examine the values.
		ASSERT_EQ(HqBytecodeWriteYield(hFuncSerializer), HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the array instance data.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

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
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
			Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
			Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
		}

		// Sub-function
		{
			// Set the function serializer.
			Util::SetupFunctionSerializer(hFuncSerializer, endianness);

			// Write a LOAD_PARAM instruction to load the input parameter to a GP register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadParam(hFuncSerializer, 0, 0);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a STORE_PARAM instruction to pass the input value back to the main function.
			// We intentionally store this on different I/O register, otherwise the check would
			// be meaningless since it would functionally be the same as not calling this function.
			const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

			// Finalize the serializer and add it to the module.
			Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, functionName);
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
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		Util::GetIoRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		Util::GetIoRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the CALL isntruction to attempt invoke the function.
		const int writeCallInstrResult = HqBytecodeWriteCall(hFuncSerializer, stringIndex);
		ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_TRUE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
			Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
			Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
		}

		// Sub-function
		{
			// Set the function serializer.
			Util::SetupFunctionSerializer(hFuncSerializer, endianness);

			// Write a LOAD_PARAM instruction to load the input parameter to a GP register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadParam(hFuncSerializer, 0, 0);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a STORE_PARAM instruction to pass the input value back to the main function.
			// We intentionally store this on different I/O register, otherwise the check would
			// be meaningless since it would functionally be the same as not calling this function.
			const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

			// Finalize the serializer and add it to the module.
			Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, functionName);
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
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		Util::GetIoRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		Util::GetIoRegister(hValue, hExec, 1);

		// Validate the register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the INIT_FUNC instruction to create a value pointing to the function we want to call.
		const int writeInitFuncInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 0, stringIndex);
		ASSERT_EQ(writeInitFuncInstrResult, HQ_SUCCESS);

		// Write the CALL_VALUE isntruction to invoke the function.
		const int writeCallInstrResult = HqBytecodeWriteCallValue(hFuncSerializer, 0);
		ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_TRUE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);

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
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the handled exception value.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetIoRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsObject(hValue));
			ASSERT_STREQ(HqValueGetObjectTypeName(hValue), objTypeName);
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_TRUE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the handled exception value.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetIoRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Push_Pop)
{
	static constexpr int32_t testValueData = 12345;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the stack value.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetStackValue(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Verify the value extracted from the object.
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueData);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Jmp)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		ControlFlow ctrl;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Begin the jump block.
		ctrl.Begin(hFuncSerializer, ControlFlow::Behavior::If, ControlFlow::Condition::None, 0);

		// Write an ABORT opcode just so we have an error case to check for
		// in case the JMP opcode doesn't branch correctly.
		const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
		ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

		// End the block indicating where the instruction pointer will jump to.
		ctrl.End();

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_TRUE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), JmpIfTrue)
{
	static constexpr const char* const functionName = "void test()";
	static constexpr const char* const objTypeName = "TestObjType";

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
			ControlFlow ctrl;

			// Begin the jump block.
			ctrl.Begin(hFuncSerializer, ControlFlow::Behavior::If, ControlFlow::Condition::True, gpRegIndex);

			// Write an ABORT opcode just so we have an error case to check for
			// in case the jump operation doesn't branch correctly.
			const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
			ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

			// End the block indicating where the instruction pointer will jump to.
			ctrl.End();
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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_TRUE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), JmpIfFalse)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Add an empty string to the module.
		uint32_t emptyStringIndex = 0;
		const int addEmptyStringResult = HqModuleWriterAddString(hModuleWriter, "", &emptyStringIndex);
		ASSERT_EQ(addEmptyStringResult, HQ_SUCCESS);

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
			ControlFlow ctrl;

			// Begin the jump block.
			ctrl.Begin(hFuncSerializer, ControlFlow::Behavior::If, ControlFlow::Condition::False, gpRegIndex);

			// Write an ABORT opcode just so we have an error case to check for
			// in case the jump operation doesn't branch correctly.
			const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
			ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

			// End the block indicating where the instruction pointer will jump to.
			ctrl.End();
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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_TRUE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_TRUE(status.exception);
		ASSERT_FALSE(status.abort);

		// Array length
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), arrayLength);
		}

		// String length
		{
			const size_t stringLength = strlen(testString);

			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), stringLength);
		}

		// Null
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_EQ(hValue, HQ_VALUE_HANDLE_NULL);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

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
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 + rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 + rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 + rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 + rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 + rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 + rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 + rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 + rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), leftValueFloat32 + rightValueFloat32);
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

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
			Util::GetGpRegister(hValue, hExec, 11);

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
			Util::GetGpRegister(hValue, hExec, 12);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsArray(hValue));
			ASSERT_EQ(HqValueGetArrayLength(hValue), leftArraySize + rightArraySize);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeSubInstrResult = HqBytecodeWriteSub(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeSubInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 - rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 - rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 - rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 - rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 - rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 - rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 - rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 - rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), leftValueFloat32 - rightValueFloat32);
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), leftValueFloat64 - rightValueFloat64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeMulInstrResult = HqBytecodeWriteMul(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeMulInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), false);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 * rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 * rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 * rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 * rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 * rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 * rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 * rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 * rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), leftValueFloat32 * rightValueFloat32);
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), leftValueFloat64 * rightValueFloat64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeDivInstrResult = HqBytecodeWriteDiv(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeDivInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), false);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 / rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 / rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 / rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 / rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 / rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 / rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 / rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 / rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), leftValueFloat32 / rightValueFloat32);
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), leftValueFloat64 / rightValueFloat64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeModInstrResult = HqBytecodeWriteMod(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeModInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 % rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 % rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 % rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 % rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 % rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 % rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 % rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 % rightValueUint64);
		}

		// float32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), fmodf(leftValueFloat32, rightValueFloat32));
		}

		// float64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), fmod(leftValueFloat64, rightValueFloat64));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 9, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeExpInstrResult = HqBytecodeWriteExp(hFuncSerializer, 10, 20, 21);
			ASSERT_EQ(writeExpInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

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
			Util::GetGpRegister(hValue, hExec, 1);

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
			Util::GetGpRegister(hValue, hExec, 2);

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
			Util::GetGpRegister(hValue, hExec, 3);

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
			Util::GetGpRegister(hValue, hExec, 4);

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
			Util::GetGpRegister(hValue, hExec, 5);

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
			Util::GetGpRegister(hValue, hExec, 6);

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
			Util::GetGpRegister(hValue, hExec, 7);

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
			Util::GetGpRegister(hValue, hExec, 8);

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
			Util::GetGpRegister(hValue, hExec, 9);

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
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), result);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitAndInstrResult = HqBytecodeWriteBitAnd(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeBitAndInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), false);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 & rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 & rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 & rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 & rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 & rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 & rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 & rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 & rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitOrInstrResult = HqBytecodeWriteBitOr(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeBitOrInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 | rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 | rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 | rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 | rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 | rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 | rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 | rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 | rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 0, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitXor(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 ^ rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 ^ rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 ^ rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 ^ rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 ^ rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 ^ rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 ^ rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 ^ rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), BitNot)
{
	static constexpr int8_t testValueInt8 = 23;
	static constexpr int16_t testValueInt16 = 2345;
	static constexpr int32_t testValueInt32 = 234567;
	static constexpr int64_t testValueInt64 = 23456789012ll;
	static constexpr uint8_t testValueUint8 = 34;
	static constexpr uint16_t testValueUint16 = 3456;
	static constexpr uint32_t testValueUint32 = 345678;
	static constexpr uint64_t testValueUint64 = 34567890123ull;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Bool
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 0, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, testValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 1, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, testValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 2, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, testValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 3, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, testValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 4, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, testValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 5, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, testValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 6, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, testValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 7, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, testValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeBitXorInstrResult = HqBytecodeWriteBitNot(hFuncSerializer, 8, 20);
			ASSERT_EQ(writeBitXorInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_EQ(HqValueGetBool(hValue), true);
		}

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), int8_t(~testValueInt8));
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), int16_t(~testValueInt16));
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), int32_t(~testValueInt32));
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), int64_t(~testValueInt64));
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), uint8_t(~testValueUint8));
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), uint16_t(~testValueUint16));
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), uint32_t(~testValueUint32));
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), uint64_t(~testValueUint64));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftShiftInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeLeftShiftInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftShiftInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeLeftShiftInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftShiftInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeLeftShiftInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftShiftInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeLeftShiftInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftShiftInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeLeftShiftInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftShiftInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeLeftShiftInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftShiftInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeLeftShiftInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftShiftInstrResult = HqBytecodeWriteLeftShift(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeLeftShiftInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 << rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 << rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 << rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 << rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 << rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 << rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 << rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 << rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightShiftInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeRightShiftInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightShiftInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeRightShiftInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightShiftInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeRightShiftInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightShiftInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeRightShiftInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightShiftInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeRightShiftInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightShiftInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeRightShiftInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightShiftInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeRightShiftInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightShiftInstrResult = HqBytecodeWriteRightShift(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeRightShiftInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// int8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), leftValueInt8 >> rightValueInt8);
		}

		// int16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), leftValueInt16 >> rightValueInt16);
		}

		// int32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), leftValueInt32 >> rightValueInt32);
		}

		// int64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), leftValueInt64 >> rightValueInt64);
		}

		// uint8
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), leftValueUint8 >> rightValueUint8);
		}

		// uint16
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), leftValueUint16 >> rightValueUint16);
		}

		// uint32
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), leftValueUint32 >> rightValueUint32);
		}

		// uint64
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), leftValueUint64 >> rightValueUint64);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftRotInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeLeftRotInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftRotInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeLeftRotInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftRotInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeLeftRotInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftRotInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeLeftRotInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftRotInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeLeftRotInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftRotInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeLeftRotInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftRotInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeLeftRotInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeLeftRotInstrResult = HqBytecodeWriteLeftRotate(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeLeftRotInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
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
			Util::GetGpRegister(hValue, hExec, 1);

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
			Util::GetGpRegister(hValue, hExec, 2);

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
			Util::GetGpRegister(hValue, hExec, 3);

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
			Util::GetGpRegister(hValue, hExec, 4);

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
			Util::GetGpRegister(hValue, hExec, 5);

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
			Util::GetGpRegister(hValue, hExec, 6);

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
			Util::GetGpRegister(hValue, hExec, 7);

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
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), result);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// int8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightRotInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 1, 20, 21);
			ASSERT_EQ(writeRightRotInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightRotInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 2, 20, 21);
			ASSERT_EQ(writeRightRotInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightRotInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 3, 20, 21);
			ASSERT_EQ(writeRightRotInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightRotInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 4, 20, 21);
			ASSERT_EQ(writeRightRotInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightRotInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 5, 20, 21);
			ASSERT_EQ(writeRightRotInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightRotInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 6, 20, 21);
			ASSERT_EQ(writeRightRotInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightRotInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 7, 20, 21);
			ASSERT_EQ(writeRightRotInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
			ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

			const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
			ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

			const int writeRightRotInstrResult = HqBytecodeWriteRightRotate(hFuncSerializer, 8, 20, 21);
			ASSERT_EQ(writeRightRotInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine the values.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
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
			Util::GetGpRegister(hValue, hExec, 1);

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
			Util::GetGpRegister(hValue, hExec, 2);

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
			Util::GetGpRegister(hValue, hExec, 3);

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
			Util::GetGpRegister(hValue, hExec, 4);

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
			Util::GetGpRegister(hValue, hExec, 5);

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
			Util::GetGpRegister(hValue, hExec, 6);

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
			Util::GetGpRegister(hValue, hExec, 7);

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
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), result);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastBool)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastBool(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// int8
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// int16
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// int32
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// int64
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// uint8
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// uint16
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// uint32
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// uint64
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// float32
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// float64
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastInt8)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt8(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), 1);
		}

		// int8
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// int16
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// int32
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// int64
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// uint8
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// uint16
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// uint32
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// uint64
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// float32
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}

		// float64
		{
			const int8_t expectedValue = static_cast<int8_t>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastInt16)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt16(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), 1);
		}

		// int8
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// int16
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// int32
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// int64
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// uint8
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// uint16
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// uint32
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// uint64
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// float32
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}

		// float64
		{
			const int16_t expectedValue = static_cast<int16_t>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastInt32)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt32(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), 1);
		}

		// int8
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// int16
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// int32
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// int64
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// uint8
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// uint16
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// uint32
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// uint64
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// float32
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}

		// float64
		{
			const int32_t expectedValue = static_cast<int32_t>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastInt64)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastInt64(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), 1);
		}

		// int8
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// int16
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// int32
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// int64
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// uint8
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// uint16
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// uint32
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// uint64
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// float32
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}

		// float64
		{
			const int64_t expectedValue = static_cast<int64_t>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastUint8)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint8(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), 1);
		}

		// int8
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// int16
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// int32
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// int64
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// uint8
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// uint16
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// uint32
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// uint64
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// float32
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}

		// float64
		{
			const uint8_t expectedValue = static_cast<uint8_t>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastUint16)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint16(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), 1);
		}

		// int8
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// int16
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// int32
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// int64
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// uint8
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// uint16
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// uint32
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// uint64
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// float32
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}

		// float64
		{
			const uint16_t expectedValue = static_cast<uint16_t>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastUint32)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint32(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), 1);
		}

		// int8
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// int16
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// int32
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// int64
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// uint8
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// uint16
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// uint32
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// uint64
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// float32
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}

		// float64
		{
			const uint32_t expectedValue = static_cast<uint32_t>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastUint64)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastUint64(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), 1);
		}

		// int8
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// int16
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// int32
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// int64
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// uint8
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// uint16
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// uint32
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// uint64
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// float32
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}

		// float64
		{
			const uint64_t expectedValue = static_cast<uint64_t>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastFloat32)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat32(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), 1.0f);
		}

		// int8
		{
			const float expectedValue = static_cast<float>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// int16
		{
			const float expectedValue = static_cast<float>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// int32
		{
			const float expectedValue = static_cast<float>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// int64
		{
			const float expectedValue = static_cast<float>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// uint8
		{
			const float expectedValue = static_cast<float>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// uint16
		{
			const float expectedValue = static_cast<float>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// uint32
		{
			const float expectedValue = static_cast<float>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// uint64
		{
			const float expectedValue = static_cast<float>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// float32
		{
			const float expectedValue = static_cast<float>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}

		// float64
		{
			const float expectedValue = static_cast<float>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_FLOAT_EQ(HqValueGetFloat32(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastFloat64)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastFloat64(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), 1.0);
		}

		// int8
		{
			const double expectedValue = static_cast<double>(testValueInt8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// int16
		{
			const double expectedValue = static_cast<double>(testValueInt16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// int32
		{
			const double expectedValue = static_cast<double>(testValueInt32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// int64
		{
			const double expectedValue = static_cast<double>(testValueInt64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// uint8
		{
			const double expectedValue = static_cast<double>(testValueUint8);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// uint16
		{
			const double expectedValue = static_cast<double>(testValueUint16);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// uint32
		{
			const double expectedValue = static_cast<double>(testValueUint32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// uint64
		{
			const double expectedValue = static_cast<double>(testValueUint64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// float32
		{
			const double expectedValue = static_cast<double>(testValueFloat32);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}

		// float64
		{
			const double expectedValue = static_cast<double>(testValueFloat64);

			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_DOUBLE_EQ(HqValueGetFloat64(hValue), expectedValue);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CastString)
{
	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static constexpr uint32_t tempRegIndex = HQ_VM_GP_REGISTER_COUNT - 1;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, tempRegIndex, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 0, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, tempRegIndex, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 1, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, tempRegIndex, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 2, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, tempRegIndex, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 3, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, tempRegIndex, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 4, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, tempRegIndex, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 5, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, tempRegIndex, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 6, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, tempRegIndex, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 7, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, tempRegIndex, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 8, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, tempRegIndex, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 9, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, tempRegIndex, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a CAST instruction to create a new value from the source register, casting to the destination type.
			const int writeCopyInstrResult = HqBytecodeWriteCastString(hFuncSerializer, 10, tempRegIndex);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);
		}

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// bool
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 0);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "true");
		}

		// int8
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "12");
		}

		// int16
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 2);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "1234");
		}

		// int32
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 3);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "123456");
		}

		// int64
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 4);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "12345678901");
		}

		// uint8
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 5);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "23");
		}

		// uint16
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 6);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "2345");
		}

		// uint32
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 7);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "234567");
		}

		// uint64
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 8);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "23456789012");
		}

		// float32
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 9);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "3.141593");
		}

		// float64
		{
			// Get the register values we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, 10);

			// Validate the register values.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsString(hValue));
			ASSERT_STREQ(HqValueGetString(hValue), "2.718282");
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CompareEqual)
{
	static constexpr const char* const objTypeName = "TestObj";
	static constexpr const char* const dummyFuncName = "void dummyFunc()";

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
	static constexpr size_t rightArraySize = 1;

	static uint32_t trueRegCount = 0;
	static uint32_t falseRegCount = 0;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		uint32_t objTypeStringIndex = 0;
		const int addObjStringResult = HqModuleWriterAddString(hModuleWriter, objTypeName, &objTypeStringIndex);
		ASSERT_EQ(addObjStringResult, HQ_SUCCESS);

		uint32_t mainFuncStringIndex = 0;
		const int addMainFuncStringResult = HqModuleWriterAddString(hModuleWriter, Function::main, &mainFuncStringIndex);
		ASSERT_EQ(addMainFuncStringResult, HQ_SUCCESS);

		uint32_t dummyFuncStringIndex = 0;
		const int addDummyFuncStringResult = HqModuleWriterAddString(hModuleWriter, dummyFuncName, &dummyFuncStringIndex);
		ASSERT_EQ(addDummyFuncStringResult, HQ_SUCCESS);

		uint32_t leftStringIndex = 0;
		const int addLeftStringResult = HqModuleWriterAddString(hModuleWriter, leftTestString, &leftStringIndex);
		ASSERT_EQ(addLeftStringResult, HQ_SUCCESS);

		uint32_t rightStringIndex = 0;
		const int addRightStringResult = HqModuleWriterAddString(hModuleWriter, rightTestString, &rightStringIndex);
		ASSERT_EQ(addRightStringResult, HQ_SUCCESS);

		const int addObjTypeResult = HqModuleWriterAddObjectType(hModuleWriter, objTypeName);
		ASSERT_EQ(addObjTypeResult, HQ_SUCCESS);

		const int addDummyFuncResult = HqModuleWriterAddNativeFunction(hModuleWriter, dummyFuncName, 0, 0);
		ASSERT_EQ(addDummyFuncResult, HQ_SUCCESS);

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// True comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, leftValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, leftValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, leftValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, leftValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, leftValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, leftValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, leftValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, leftValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, leftValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, leftValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, leftStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// function
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 20, mainFuncStringIndex);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeInitRightInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 21, mainFuncStringIndex);
				ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Same value reference
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, trueRegCount++, 20, 20);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// False comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, rightStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// array
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 20, leftArraySize);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeInitRightInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 21, rightArraySize);
				ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// object
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 20, objTypeStringIndex);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeInitRightInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 21, objTypeStringIndex);
				ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// function
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 20, mainFuncStringIndex);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeInitRightInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 21, dummyFuncStringIndex);
				ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all true comparisons.
		for(uint32_t regIndex = 0; regIndex < trueRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all false comparisons.
		for(uint32_t regIndex = 0; regIndex < falseRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_FALSE(HqValueGetBool(hValue));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CompareNotEqual)
{
	static constexpr const char* const objTypeName = "TestObj";
	static constexpr const char* const dummyFuncName = "void dummyFunc()";

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
	static constexpr size_t rightArraySize = 1;

	static uint32_t trueRegCount = 0;
	static uint32_t falseRegCount = 0;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		uint32_t objTypeStringIndex = 0;
		const int addObjStringResult = HqModuleWriterAddString(hModuleWriter, objTypeName, &objTypeStringIndex);
		ASSERT_EQ(addObjStringResult, HQ_SUCCESS);

		uint32_t mainFuncStringIndex = 0;
		const int addMainFuncStringResult = HqModuleWriterAddString(hModuleWriter, Function::main, &mainFuncStringIndex);
		ASSERT_EQ(addMainFuncStringResult, HQ_SUCCESS);

		uint32_t dummyFuncStringIndex = 0;
		const int addDummyFuncStringResult = HqModuleWriterAddString(hModuleWriter, dummyFuncName, &dummyFuncStringIndex);
		ASSERT_EQ(addDummyFuncStringResult, HQ_SUCCESS);

		uint32_t leftStringIndex = 0;
		const int addLeftStringResult = HqModuleWriterAddString(hModuleWriter, leftTestString, &leftStringIndex);
		ASSERT_EQ(addLeftStringResult, HQ_SUCCESS);

		uint32_t rightStringIndex = 0;
		const int addRightStringResult = HqModuleWriterAddString(hModuleWriter, rightTestString, &rightStringIndex);
		ASSERT_EQ(addRightStringResult, HQ_SUCCESS);

		const int addObjTypeResult = HqModuleWriterAddObjectType(hModuleWriter, objTypeName);
		ASSERT_EQ(addObjTypeResult, HQ_SUCCESS);

		const int addDummyFuncResult = HqModuleWriterAddNativeFunction(hModuleWriter, dummyFuncName, 0, 0);
		ASSERT_EQ(addDummyFuncResult, HQ_SUCCESS);

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// False comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, leftValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, leftValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, leftValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, leftValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, leftValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, leftValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, leftValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, leftValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, leftValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, leftValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, leftStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// function
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 20, mainFuncStringIndex);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeInitRightInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 21, mainFuncStringIndex);
				ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Same value reference
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, trueRegCount++, 20, 20);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// False comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, rightStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// array
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 20, leftArraySize);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeInitRightInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 21, rightArraySize);
				ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// object
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 20, objTypeStringIndex);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeInitRightInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 21, objTypeStringIndex);
				ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// function
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 20, mainFuncStringIndex);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeInitRightInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 21, dummyFuncStringIndex);
				ASSERT_EQ(writeInitRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareNotEqual(hFuncSerializer, falseRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all true comparisons.
		for(uint32_t regIndex = 0; regIndex < trueRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_FALSE(HqValueGetBool(hValue));
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all false comparisons.
		for(uint32_t regIndex = 0; regIndex < falseRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CompareLess)
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

	static uint32_t lessThanRegCount = 0;
	static uint32_t equalToRegCount = 0;

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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Less-than comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, rightStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Equal-to comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, leftValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, leftValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, leftValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, leftValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, leftValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, leftValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, leftValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, leftValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, leftValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, leftValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, leftStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Same value reference
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLess(hFuncSerializer, equalToRegCount++, 20, 20);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all less-than comparisons.
		for(uint32_t regIndex = 0; regIndex < lessThanRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all equal-to comparisons.
		for(uint32_t regIndex = 0; regIndex < equalToRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_FALSE(HqValueGetBool(hValue));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CompareLessEqual)
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

	static uint32_t lessThanRegCount = 0;
	static uint32_t equalToRegCount = 0;

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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Less-than comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, rightStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, lessThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Equal-to comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, leftValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, leftValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, leftValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, leftValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, leftValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, leftValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, leftValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, leftValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, leftValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, leftValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, leftStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Same value reference
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 20);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all less-than comparisons.
		for(uint32_t regIndex = 0; regIndex < lessThanRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all equal-to comparisons.
		for(uint32_t regIndex = 0; regIndex < equalToRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CompareGreater)
{
	static constexpr const char* const leftTestString = "test1";
	static constexpr const char* const rightTestString = "test0";

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

	static uint32_t greaterThanRegCount = 0;
	static uint32_t equalToRegCount = 0;

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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Greater-than comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, false);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, rightStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Equal-to comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, leftValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, leftValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, leftValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, leftValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, leftValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, leftValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, leftValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, leftValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, leftValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, leftValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, leftStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Same value reference
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreater(hFuncSerializer, equalToRegCount++, 20, 20);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all greater-than comparisons.
		for(uint32_t regIndex = 0; regIndex < greaterThanRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all equal-to comparisons.
		for(uint32_t regIndex = 0; regIndex < equalToRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_FALSE(HqValueGetBool(hValue));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), CompareGreaterEqual)
{
	static constexpr const char* const leftTestString = "test1";
	static constexpr const char* const rightTestString = "test0";

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

	static uint32_t greaterThanRegCount = 0;
	static uint32_t equalToRegCount = 0;

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
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Greater-than comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, false);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, rightValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, rightValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, rightValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, rightValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, rightValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, rightValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, rightValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, rightValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, rightValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, rightValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, rightStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareGreaterEqual(hFuncSerializer, greaterThanRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Equal-to comparisons
		{
			// Bool
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 21, true);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, leftValueInt8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 21, leftValueInt8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, leftValueInt16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 21, leftValueInt16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, leftValueInt32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 21, leftValueInt32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, leftValueInt64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 21, leftValueInt64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, leftValueUint8);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 21, leftValueUint8);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, leftValueUint16);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 21, leftValueUint16);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, leftValueUint32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 21, leftValueUint32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, leftValueUint64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 21, leftValueUint64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, leftValueFloat32);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 21, leftValueFloat32);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, leftValueFloat64);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 21, leftValueFloat64);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadLeftInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, leftStringIndex);
				ASSERT_EQ(writeLoadLeftInstrResult, HQ_SUCCESS);

				const int writeLoadRightInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 21, leftStringIndex);
				ASSERT_EQ(writeLoadRightInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 21);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Same value reference
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeCompareInstrResult = HqBytecodeWriteCompareLessEqual(hFuncSerializer, equalToRegCount++, 20, 20);
				ASSERT_EQ(writeCompareInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all less-than comparisons.
		for(uint32_t regIndex = 0; regIndex < greaterThanRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all equal-to comparisons.
		for(uint32_t regIndex = 0; regIndex < equalToRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Test)
{
	static constexpr const char* const objTypeName = "TestObj";

	static constexpr const char* const emptyTestString = "";
	static constexpr const char* const nonEmptyTestString = "test";

	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	static uint32_t trueRegCount = 0;
	static uint32_t falseRegCount = 0;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		uint32_t objTypeStringIndex = 0;
		const int addObjStringResult = HqModuleWriterAddString(hModuleWriter, objTypeName, &objTypeStringIndex);
		ASSERT_EQ(addObjStringResult, HQ_SUCCESS);

		uint32_t mainFuncStringIndex = 0;
		const int addMainFuncStringResult = HqModuleWriterAddString(hModuleWriter, Function::main, &mainFuncStringIndex);
		ASSERT_EQ(addMainFuncStringResult, HQ_SUCCESS);

		uint32_t emptyStringIndex = 0;
		const int addEmptyStringResult = HqModuleWriterAddString(hModuleWriter, emptyTestString, &emptyStringIndex);
		ASSERT_EQ(addEmptyStringResult, HQ_SUCCESS);

		uint32_t nonEmptyStringIndex = 0;
		const int addNonEmptyStringResult = HqModuleWriterAddString(hModuleWriter, nonEmptyTestString, &nonEmptyStringIndex);
		ASSERT_EQ(addNonEmptyStringResult, HQ_SUCCESS);

		const int addObjTypeResult = HqModuleWriterAddObjectType(hModuleWriter, objTypeName);
		ASSERT_EQ(addObjTypeResult, HQ_SUCCESS);

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// True comparisons
		{
			// Bool
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, true);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, testValueInt8);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, testValueInt16);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, testValueInt32);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, testValueInt64);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, testValueUint8);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, testValueUint16);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, testValueUint32);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, testValueUint64);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, testValueFloat32);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, testValueFloat64);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, nonEmptyStringIndex);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// array
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 20, 1);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// object
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 20, objTypeStringIndex);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// function
			{
				const int writeInitLeftInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 20, mainFuncStringIndex);
				ASSERT_EQ(writeInitLeftInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, trueRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// False comparisons
		{
			// Bool
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 20, false);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// int8
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// int16
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// int32
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// int64
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// uint8
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// uint16
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// uint32
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// uint64
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// float32
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 20, 0.0f);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// float64
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 20, 0.0);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// string
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 20, emptyStringIndex);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// array
			{
				const int writeInitArrayInstrResult = HqBytecodeWriteInitArray(hFuncSerializer, 20, 0);
				ASSERT_EQ(writeInitArrayInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// null
			{
				const int writeLoadInstrResult = HqBytecodeWriteLoadImmNull(hFuncSerializer, 20);
				ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

				const int writeTestInstrResult = HqBytecodeWriteTest(hFuncSerializer, falseRegCount++, 20);
				ASSERT_EQ(writeTestInstrResult, HQ_SUCCESS);
			}

			// Write a YIELD instruction so we can examine the values.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all true comparisons.
		for(uint32_t regIndex = 0; regIndex < trueRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Run the execution context again.
		const int execRunAgainResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunAgainResult, HQ_SUCCESS);

		// Get the status of the execution context.
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Check for all false comparisons.
		for(uint32_t regIndex = 0; regIndex < falseRegCount; ++regIndex)
		{
			// Get the register value we want to inspect.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			Util::GetGpRegister(hValue, hExec, regIndex);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_FALSE(HqValueGetBool(hValue));
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Move)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write a LOAD_IMM_I8 instruction just to have a value to move.
		const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 0, 123);
		ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

		// Write a MOV instruction to copy the value reference to another register.
		const int writeMovInstrResult = HqBytecodeWriteMove(hFuncSerializer, 1, 0);
		ASSERT_EQ(writeMovInstrResult, HQ_SUCCESS);

		// Write a YIELD instruction so we can examine register data.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
		HqValueHandle hMovedValue = HQ_VALUE_HANDLE_NULL;

		// Get the register values we want to inspect.
		Util::GetGpRegister(hOriginalValue, hExec, 0);
		Util::GetGpRegister(hMovedValue, hExec, 1);

		// Validate the register values.
		ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_NE(hMovedValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_EQ(hOriginalValue, hMovedValue);
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestOpCodes), Copy)
{
	static constexpr const char* const objTypeName = "TestObj";
	static constexpr const char* const objMemberName = "member";
	static constexpr const char* const testString = "test";

	static constexpr int8_t testValueInt8 = 12;
	static constexpr int16_t testValueInt16 = 1234;
	static constexpr int32_t testValueInt32 = 123456;
	static constexpr int64_t testValueInt64 = 12345678901ll;
	static constexpr uint8_t testValueUint8 = 23;
	static constexpr uint16_t testValueUint16 = 2345;
	static constexpr uint32_t testValueUint32 = 234567;
	static constexpr uint64_t testValueUint64 = 23456789012ull;
	static constexpr float testValueFloat32 = 3.1415926535897932384626433832795f;
	static constexpr double testValueFloat64 = 2.7182818284590452353602874713527;

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		uint32_t testStringIndex = 0;
		const int addTestStringResult = HqModuleWriterAddString(hModuleWriter, testString, &testStringIndex);
		ASSERT_EQ(addTestStringResult, HQ_SUCCESS);

		uint32_t funcStringIndex = 0;
		const int addFuncStringResult = HqModuleWriterAddString(hModuleWriter, Function::main, &funcStringIndex);
		ASSERT_EQ(addFuncStringResult, HQ_SUCCESS);

		uint32_t objTypeStringIndex = 0;
		const int addObjTypeStringResult = HqModuleWriterAddString(hModuleWriter, objTypeName, &objTypeStringIndex);
		ASSERT_EQ(addObjTypeStringResult, HQ_SUCCESS);

		const int addObjTypeResult = HqModuleWriterAddObjectType(hModuleWriter, objTypeName);
		ASSERT_EQ(addObjTypeResult, HQ_SUCCESS);

		uint32_t objMemberIndex = 0;
		const int addObjMemberResult = HqModuleWriterAddObjectMember(hModuleWriter, objTypeName, objMemberName, HQ_VALUE_TYPE_INT8, &objMemberIndex);
		ASSERT_EQ(addObjMemberResult, HQ_SUCCESS);

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);

		// null
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmNull(hFuncSerializer, 0);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// bool
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmBool(hFuncSerializer, 0, true);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// int8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 0, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// int16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI16(hFuncSerializer, 0, testValueInt16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// int32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI32(hFuncSerializer, 0, testValueInt32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// int64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI64(hFuncSerializer, 0, testValueInt64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// uint8
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU8(hFuncSerializer, 0, testValueUint8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// uint16
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU16(hFuncSerializer, 0, testValueUint16);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// uint32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU32(hFuncSerializer, 0, testValueUint32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// uint64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmU64(hFuncSerializer, 0, testValueUint64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// float32
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF32(hFuncSerializer, 0, testValueFloat32);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// float64
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmF64(hFuncSerializer, 0, testValueFloat64);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// string
		{
			// Write an instruction to load the initial value into a register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmStr(hFuncSerializer, 0, testStringIndex);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// function
		{
			// Write an INIT_FUNC instruction to create a function value.
			const int writeLoadInstrResult = HqBytecodeWriteInitFunction(hFuncSerializer, 0, funcStringIndex);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// object
		{
			// Write an INIT_OBJECT instruction to create an array value.
			const int writeInitInstrResult = HqBytecodeWriteInitObject(hFuncSerializer, 0, objTypeStringIndex);
			ASSERT_EQ(writeInitInstrResult, HQ_SUCCESS);

			// Write an instruction to load some data into a register that will live in the array.
			const int writeLoadInstrResult = HqBytecodeWriteLoadImmI8(hFuncSerializer, 1, testValueInt8);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a STORE_OBJECT instruction to place the loaded value in the object.
			const int writeStoreInstrResult = HqBytecodeWriteStoreObject(hFuncSerializer, 0, 1, objMemberIndex);
			ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			const int writeCopyInstrResult = HqBytecodeWriteCopy(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeCopyInstrResult, HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
			ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);
		}

		// array
		{
			// Write an INIT_ARRAY instruction to create an array value.
			ASSERT_EQ(HqBytecodeWriteInitArray(hFuncSerializer, 0, 1), HQ_SUCCESS);

			// Write an instruction to load some data into a register that will live in the array.
			ASSERT_EQ(HqBytecodeWriteLoadImmI8(hFuncSerializer, 1, testValueInt8), HQ_SUCCESS);

			// Write an instruction to store the array index.
			ASSERT_EQ(HqBytecodeWriteLoadImmI16(hFuncSerializer, 2, 0), HQ_SUCCESS);

			// Write a STORE_ARRAY instruction to place the loaded value in the array.
			ASSERT_EQ(HqBytecodeWriteStoreArray(hFuncSerializer, 0, 1, 2), HQ_SUCCESS);

			// Write a COPY instruction to create a new value from the source register.
			ASSERT_EQ(HqBytecodeWriteCopy(hFuncSerializer, 1, 0), HQ_SUCCESS);

			// Write a YIELD instruction so we can examine register data.
			ASSERT_EQ(HqBytecodeWriteYield(hFuncSerializer), HQ_SUCCESS);
		}

		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		(void) hVm;

		// null
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_EQ(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_EQ(hCopiedValue, HQ_VALUE_HANDLE_NULL);
		}

		// bool
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsBool(hOriginalValue));
			ASSERT_TRUE(HqValueIsBool(hCopiedValue));
			ASSERT_EQ(HqValueGetBool(hOriginalValue), HqValueGetBool(hCopiedValue));
		}

		// int8
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsInt8(hOriginalValue));
			ASSERT_TRUE(HqValueIsInt8(hCopiedValue));
			ASSERT_EQ(HqValueGetInt8(hOriginalValue), HqValueGetInt8(hCopiedValue));
		}

		// int16
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsInt16(hOriginalValue));
			ASSERT_TRUE(HqValueIsInt16(hCopiedValue));
			ASSERT_EQ(HqValueGetInt16(hOriginalValue), HqValueGetInt16(hCopiedValue));
		}

		// int32
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsInt32(hOriginalValue));
			ASSERT_TRUE(HqValueIsInt32(hCopiedValue));
			ASSERT_EQ(HqValueGetInt32(hOriginalValue), HqValueGetInt32(hCopiedValue));
		}

		// int64
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsInt64(hOriginalValue));
			ASSERT_TRUE(HqValueIsInt64(hCopiedValue));
			ASSERT_EQ(HqValueGetInt64(hOriginalValue), HqValueGetInt64(hCopiedValue));
		}

		// uint8
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsUint8(hOriginalValue));
			ASSERT_TRUE(HqValueIsUint8(hCopiedValue));
			ASSERT_EQ(HqValueGetUint8(hOriginalValue), HqValueGetUint8(hCopiedValue));
		}

		// uint16
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsUint16(hOriginalValue));
			ASSERT_TRUE(HqValueIsUint16(hCopiedValue));
			ASSERT_EQ(HqValueGetUint16(hOriginalValue), HqValueGetUint16(hCopiedValue));
		}

		// uint32
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsUint32(hOriginalValue));
			ASSERT_TRUE(HqValueIsUint32(hCopiedValue));
			ASSERT_EQ(HqValueGetUint32(hOriginalValue), HqValueGetUint32(hCopiedValue));
		}

		// uint64
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsUint64(hOriginalValue));
			ASSERT_TRUE(HqValueIsUint64(hCopiedValue));
			ASSERT_EQ(HqValueGetUint64(hOriginalValue), HqValueGetUint64(hCopiedValue));
		}

		// float32
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsFloat32(hOriginalValue));
			ASSERT_TRUE(HqValueIsFloat32(hCopiedValue));
			ASSERT_EQ(HqValueGetFloat32(hOriginalValue), HqValueGetFloat32(hCopiedValue));
		}

		// float64
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsFloat64(hOriginalValue));
			ASSERT_TRUE(HqValueIsFloat64(hCopiedValue));
			ASSERT_EQ(HqValueGetFloat64(hOriginalValue), HqValueGetFloat64(hCopiedValue));
		}

		// string
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsString(hOriginalValue));
			ASSERT_TRUE(HqValueIsString(hCopiedValue));
			ASSERT_STREQ(HqValueGetString(hOriginalValue), HqValueGetString(hCopiedValue));
		}

		// function
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsFunction(hOriginalValue));
			ASSERT_TRUE(HqValueIsFunction(hCopiedValue));
			ASSERT_EQ(HqValueGetFunction(hOriginalValue), HqValueGetFunction(hCopiedValue));
		}

		// object
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsObject(hOriginalValue));
			ASSERT_TRUE(HqValueIsObject(hCopiedValue));

			HqValueHandle hOriginalMember = HqValueGetObjectMemberValue(hOriginalValue, objMemberName);
			HqValueHandle hCopiedMember = HqValueGetObjectMemberValue(hCopiedValue, objMemberName);

			// Validate the member values.
			ASSERT_NE(hOriginalMember, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedMember, HQ_VALUE_HANDLE_NULL);
			ASSERT_EQ(hOriginalMember, hCopiedMember);
			ASSERT_TRUE(HqValueIsInt8(hOriginalMember));
			ASSERT_EQ(HqValueGetInt8(hOriginalMember), testValueInt8);
		}

		// array
		{
			// Run the execution context.
			const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
			ASSERT_EQ(execRunResult, HQ_SUCCESS);

			// Get the status of the execution context.
			ExecStatus status;
			Util::GetExecutionStatus(status, hExec);
			ASSERT_TRUE(status.yield);
			ASSERT_TRUE(status.running);
			ASSERT_FALSE(status.complete);
			ASSERT_FALSE(status.exception);
			ASSERT_FALSE(status.abort);

			HqValueHandle hOriginalValue = HQ_VALUE_HANDLE_NULL;
			HqValueHandle hCopiedValue = HQ_VALUE_HANDLE_NULL;

			// Get the register values we want to inspect.
			Util::GetGpRegister(hOriginalValue, hExec, 0);
			Util::GetGpRegister(hCopiedValue, hExec, 1);

			// Validate the register values.
			ASSERT_NE(hOriginalValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hOriginalValue, hCopiedValue);
			ASSERT_TRUE(HqValueIsArray(hOriginalValue));
			ASSERT_TRUE(HqValueIsArray(hCopiedValue));

			HqValueHandle hOriginalElement = HqValueGetArrayElement(hOriginalValue, 0);
			HqValueHandle hCopiedElement = HqValueGetArrayElement(hCopiedValue, 0);

			// Validate the array element values.
			ASSERT_NE(hOriginalElement, HQ_VALUE_HANDLE_NULL);
			ASSERT_NE(hCopiedElement, HQ_VALUE_HANDLE_NULL);
			ASSERT_EQ(hOriginalElement, hCopiedElement);
			ASSERT_TRUE(HqValueIsInt8(hOriginalElement));
			ASSERT_EQ(HqValueGetInt8(hOriginalElement), testValueInt8);
		}
	};

	std::vector<uint8_t> bytecode;

	// Construct the module bytecode for the test.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	// Run the module bytecode.
	Util::ProcessBytecode("TestOpCodes", Function::main, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------
