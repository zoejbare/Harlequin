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

#include "../XenonScript.h"

#include "Compiler.hpp"
#include "Function.hpp"
#include "ProgramWriter.hpp"

#include "../shared/String.hpp"
#include "../shared/Value.hpp"

//----------------------------------------------------------------------------------------------------------------------

extern "C" {

//----------------------------------------------------------------------------------------------------------------------

int XenonCompilerCreate(XenonCompilerHandle* phOutCompiler, XenonCompilerInit init)
{
	if(!phOutCompiler
		|| (*phOutCompiler)
		|| init.common.report.reportLevel < XENON_MESSAGE_TYPE_VERBOSE
		|| init.common.report.reportLevel > XENON_MESSAGE_TYPE_FATAL)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonCompilerHandle hCompiler = XenonCompiler::Create(init);

	// The message has to be printed *after* creating the VM state since the state is used in this function.
	XenonReportMessage(&hCompiler->report, XENON_MESSAGE_TYPE_VERBOSE, "Initializing Xenon compiler");

	(*phOutCompiler) = hCompiler;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonCompilerDispose(XenonCompilerHandle* phCompiler)
{
	if(!phCompiler || !(*phCompiler))
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonCompilerHandle hCompiler = (*phCompiler);

	(*phCompiler) = XENON_COMPILER_HANDLE_NULL;

	XenonReportMessage(&hCompiler->report, XENON_MESSAGE_TYPE_VERBOSE, "Releasing Xenon compiler");
	XenonCompiler::Dispose(hCompiler);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonCompilerGetReportHandle(XenonCompilerHandle hCompiler, XenonReportHandle* phOutReport)
{
	if(!hCompiler || !phOutReport)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*phOutReport) = &hCompiler->report;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterCreate(XenonProgramWriterHandle* phOutProgramWriter, XenonCompilerHandle hCompiler)
{
	if(!phOutProgramWriter || (*phOutProgramWriter) || !hCompiler)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonProgramWriterHandle hWriter = XenonProgramWriter::Create();
	if(!hWriter)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	(*phOutProgramWriter) = hWriter;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterDispose(XenonProgramWriterHandle* phProgramWriter)
{
	if(!phProgramWriter || !(*phProgramWriter))
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonProgramWriterHandle hWriter = (*phProgramWriter);

	(*phProgramWriter) = XENON_PROGRAM_WRITER_HANDLE_NULL;

	XenonProgramWriter::Dispose(hWriter);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddDependency(XenonProgramWriterHandle hProgramWriter, const char* const programName)
{
	if(!hProgramWriter || !programName || programName[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonString* const pProgramName = XenonString::Create(programName);
	if(!pProgramName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	if(hProgramWriter->dependencies.Contains(pProgramName))
	{
		XenonString::Dispose(pProgramName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	hProgramWriter->dependencies.Insert(pProgramName, false);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstant(XenonProgramWriterHandle hProgramWriter, XenonValueHandle hValue, uint32_t* pOutConstantIndex)
{
	if(!hProgramWriter || !hValue || !pOutConstantIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	const uint32_t constIndex = uint32_t(hProgramWriter->constants.size());

	XenonValueHandle hValueRef = XenonValueReference(hValue);

	// Add the input constant.
	hProgramWriter->constants.push_back(hValueRef);

	// Store the constant index.
	(*pOutConstantIndex) = constIndex;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddGlobal(XenonProgramWriterHandle hProgramWriter, const char* variableName, uint32_t constantIndex)
{
	if(!hProgramWriter || !variableName || variableName[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Verify the input constant table index is within the correct range.
	if(uint32_t(hProgramWriter->constants.size()) <= constantIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create the string object to use as the map key.
	XenonString* const pKey = XenonString::Create(variableName);
	if(!pKey)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Verify a value mapped to the input name doesn't already exist.
	if(hProgramWriter->globals.Contains(pKey))
	{
		XenonString::Dispose(pKey);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	// Map the global variable name to the constant table index.
	hProgramWriter->globals.Insert(pKey, constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddFunction(
	XenonProgramWriterHandle hProgramWriter,
	const char* const functionSignature,
	const void* const pFunctionBytecode,
	const size_t functionBytecodeLength,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	if(!hProgramWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| !pFunctionBytecode
		|| functionBytecodeLength == 0
		|| numParameters > XENON_VM_IO_REGISTER_COUNT
		|| numReturnValues > XENON_VM_IO_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonString* pSignature = XenonString::Create(functionSignature);
	if(!pSignature)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Copy the input bytecode data to the byte vector that will be mapped into the program writer.
	std::vector<uint8_t> bytecode(functionBytecodeLength);
	memcpy(bytecode.data(), pFunctionBytecode, bytecode.size());

	XenonFunction function;

	function.bytecode = std::move(bytecode);
	function.numParameters = numParameters;
	function.numReturnValues = numReturnValues;
	function.isNative = false;

	hProgramWriter->functions.Insert(pSignature, function);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddNativeFunction(
	XenonProgramWriterHandle hProgramWriter,
	const char* const functionSignature,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	if(!hProgramWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| numParameters > XENON_VM_IO_REGISTER_COUNT
		|| numReturnValues > XENON_VM_IO_REGISTER_COUNT)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonString* pSignature = XenonString::Create(functionSignature);
	if(!pSignature)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	XenonFunction function;

	function.numParameters = numParameters;
	function.numReturnValues = numReturnValues;
	function.isNative = true;

	hProgramWriter->functions.Insert(pSignature, function);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddLocalVariable(
	XenonProgramWriterHandle hProgramWriter,
	const char* functionSignature,
	const char* variableName,
	uint32_t constantIndex
)
{
	if(!hProgramWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| !variableName
		|| variableName[0] == '\0'
		|| constantIndex >= hProgramWriter->constants.size())
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonString* const pSignature = XenonString::Create(functionSignature);
	if(!pSignature)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Find the desired function by checking what we currently have mapped.
	auto kv = hProgramWriter->functions.find(pSignature);
	if(kv == hProgramWriter->functions.end())
	{
		XenonString::Dispose(pSignature);
		return XENON_ERROR_KEY_DOES_NOT_EXIST;
	}

	// The string object for the function signature is no longer needed.
	XenonString::Dispose(pSignature);

	// Script local variables cannot be added to native functions.
	if(kv->value.isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	XenonString* const pVariableName = XenonString::Create(variableName);
	if(!pVariableName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	kv->value.locals.Insert(pVariableName, constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterSerialize(
	XenonProgramWriterHandle hProgramWriter,
	XenonCompilerHandle hCompiler,
	XenonSerializerHandle hSerializer,
	int endianness
)
{
	if(!hProgramWriter
		|| !hCompiler
		|| !hSerializer
		|| XenonSerializerGetMode(hSerializer) != XENON_SERIALIZER_MODE_WRITER
		|| XenonGetEndianModeString(endianness) == nullptr)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Write the program to the serializer.
	if(!XenonProgramWriter::Serialize(hProgramWriter, hCompiler, hSerializer, endianness))
	{
		return XENON_ERROR_UNSPECIFIED_FAILURE;
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

}
