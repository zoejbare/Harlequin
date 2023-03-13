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

#include "RuntimeUtil.hpp"
#include "Memory.hpp"

#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" void CompileBytecode(std::vector<uint8_t>& outBytecode, CompilerCallback callback)
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

extern "C" void ProcessBytecode(
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
