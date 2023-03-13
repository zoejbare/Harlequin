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

#include <compiler/WriteJumpInstr.hpp>
#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------------------------------

namespace Function
{
	static constexpr const char* const main = "void main()";
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

inline void SetupFunctionSerializer(HqSerializerHandle& output, const int endianness)
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

inline void FinalizeFunctionSerializer(
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

inline void GetExecutionStatus(ExecStatus& output, HqExecutionHandle hExec)
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

inline void GetCurrentFrame(HqFrameHandle& output, HqExecutionHandle hExec)
{
	// Get the current frame in the callstack.
	HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
	const int getFrameResult = HqExecutionGetCurrentFrame(hExec, &hFrame);
	ASSERT_EQ(getFrameResult, HQ_SUCCESS);
	ASSERT_NE(hFrame, HQ_FRAME_HANDLE_NULL);

	output = hFrame;
}

//----------------------------------------------------------------------------------------------------------------------

inline void GetGpRegister(HqValueHandle& output, HqExecutionHandle hExec, const int gpRegIndex)
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

inline void GetIoRegister(HqValueHandle& output, HqExecutionHandle hExec, const int ioRegIndex)
{
	// Get the general-purpose register that has the value we want to inspect.
	HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
	const int getIoRegisterResult = HqExecutionGetIoRegister(hExec, &hValue, ioRegIndex);
	ASSERT_EQ(getIoRegisterResult, HQ_SUCCESS);

	output = hValue;
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
		SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeNopInstrResult = HqBytecodeWriteNop(hFuncSerializer);
		ASSERT_EQ(writeNopInstrResult, HQ_SUCCESS);

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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Abort)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
		ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);

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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Yield)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the instruction that we're going to test.
		const int writeYieldInstrResult = HqBytecodeWriteYield(hFuncSerializer);
		ASSERT_EQ(writeYieldInstrResult, HQ_SUCCESS);

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
		SetupFunctionSerializer(hFuncSerializer, endianness);

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

		// Verify the null value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 0);

			// Validate the register value.
			ASSERT_EQ(hValue, HQ_VALUE_HANDLE_NULL);
		}

		// Verify the boolean value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 1);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsBool(hValue));
			ASSERT_TRUE(HqValueGetBool(hValue));
		}

		// Verify the int8 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 2);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt8(hValue));
			ASSERT_EQ(HqValueGetInt8(hValue), testValueDataI8);
		}

		// Verify the int16 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 3);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt16(hValue));
			ASSERT_EQ(HqValueGetInt16(hValue), testValueDataI16);
		}

		// Verify the int32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 4);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt32(hValue));
			ASSERT_EQ(HqValueGetInt32(hValue), testValueDataI32);
		}

		// Verify the int64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 5);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsInt64(hValue));
			ASSERT_EQ(HqValueGetInt64(hValue), testValueDataI64);
		}

		// Verify the uint8 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 6);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint8(hValue));
			ASSERT_EQ(HqValueGetUint8(hValue), testValueDataU8);
		}

		// Verify the uint16 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 7);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint16(hValue));
			ASSERT_EQ(HqValueGetUint16(hValue), testValueDataU16);
		}

		// Verify the uint32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 8);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint32(hValue));
			ASSERT_EQ(HqValueGetUint32(hValue), testValueDataU32);
		}

		// Verify the uint64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 9);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsUint64(hValue));
			ASSERT_EQ(HqValueGetUint64(hValue), testValueDataU64);
		}

		// Verify the float32 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 10);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat32(hValue));
			ASSERT_EQ(HqValueGetFloat32(hValue), testValueDataF32);
		}

		// Verify the float64 value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 11);

			// Validate the register value.
			ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
			ASSERT_TRUE(HqValueIsFloat64(hValue));
			ASSERT_EQ(HqValueGetFloat64(hValue), testValueDataF64);
		}

		// Verify the string value.
		{
			// Get the register value.
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
			GetGpRegister(hValue, hExec, 12);

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
		SetupFunctionSerializer(hFuncSerializer, endianness);

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

		// Get the global variable value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		const int getGlobalVarResult = HqVmGetGlobalVariable(hVm, &hValue, globalName);
		ASSERT_EQ(getGlobalVarResult, HQ_SUCCESS);

		// Validate the global variable.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);

		// Get the register value we want to inspect.
		GetGpRegister(hValue, hExec, 1);

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

		SetupFunctionSerializer(hFuncSerializer, endianness);

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

		// Get the GP register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		GetGpRegister(hValue, hExec, 1);

		// Validate the GP register value.
		ASSERT_NE(hValue, HQ_VALUE_HANDLE_NULL);
		ASSERT_TRUE(HqValueIsInt32(hValue));
		ASSERT_EQ(HqValueGetInt32(hValue), testValueData);

		// Get the I/O register value we want to inspect.
		GetIoRegister(hValue, hExec, 2);

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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Call_Script)
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
			SetupFunctionSerializer(hFuncSerializer, endianness);

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
			FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, Function::main);
		}

		// Sub-function
		{
			// Set the function serializer.
			SetupFunctionSerializer(hFuncSerializer, endianness);

			// Write a LOAD_PARAM instruction to load the input parameter to a GP register.
			const int writeLoadInstrResult = HqBytecodeWriteLoadParam(hFuncSerializer, 0, 0);
			ASSERT_EQ(writeLoadInstrResult, HQ_SUCCESS);

			// Write a STORE_PARAM instruction to pass the input value back to the main function.
			// We intentionally store this on different I/O register, otherwise the check would
			// be meaningless since it would functionally be the same as not calling this function.
			const int writeStoreInstrResult = HqBytecodeWriteStoreParam(hFuncSerializer, 1, 0);
			ASSERT_EQ(writeStoreInstrResult, HQ_SUCCESS);

			// Finalize the serializer and add it to the module.
			FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, functionName);
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
		GetExecutionStatus(status, hExec);
		ASSERT_TRUE(status.yield);
		ASSERT_TRUE(status.running);
		ASSERT_FALSE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		GetIoRegister(hValue, hExec, 1);

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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Call_Native)
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
		SetupFunctionSerializer(hFuncSerializer, endianness);

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

		// Get the register value we want to inspect.
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;
		GetIoRegister(hValue, hExec, 1);

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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Call_NativeNonExistent)
{
	static constexpr const char* const functionName = "void nativeFuncWithoutImpl()";

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
		SetupFunctionSerializer(hFuncSerializer, endianness);

		// Write the CALL isntruction to attempt invoke the function.
		const int writeCallInstrResult = HqBytecodeWriteCall(hFuncSerializer, stringIndex);
		ASSERT_EQ(writeCallInstrResult, HQ_SUCCESS);

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

TEST_F(_HQ_TEST_NAME(TestOpCodes), Jmp)
{
	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// Set the function serializer.
		SetupFunctionSerializer(hFuncSerializer, endianness);

		// Scoped to write the JMP opcode.
		{
			WriteJumpInstr jump(hFuncSerializer, JumpBehavior::Forward, JumpCondition::None, 0);

			// Write an ABORT opcode just so we have an error case to check for
			// in case the JMP opcode doesn't branch correctly.
			const int writeAbortInstrResult = HqBytecodeWriteAbort(hFuncSerializer);
			ASSERT_EQ(writeAbortInstrResult, HQ_SUCCESS);
		}

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
