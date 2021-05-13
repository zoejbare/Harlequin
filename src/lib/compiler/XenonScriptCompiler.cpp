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
#include "FunctionData.hpp"
#include "ProgramWriter.hpp"

#include "../base/String.hpp"

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
		XenonString::Release(pProgramName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	hProgramWriter->dependencies.Insert(pProgramName, false);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantNull(XenonProgramWriterHandle hProgramWriter, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = hProgramWriter->nullIndex;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantBool(XenonProgramWriterHandle hProgramWriter, bool value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = value ? hProgramWriter->boolTrueIndex : hProgramWriter->boolFalseIndex;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantInt8(XenonProgramWriterHandle hProgramWriter, int8_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantInt16(XenonProgramWriterHandle hProgramWriter, int16_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantInt32(XenonProgramWriterHandle hProgramWriter, int32_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantInt64(XenonProgramWriterHandle hProgramWriter, int64_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantUint8(XenonProgramWriterHandle hProgramWriter, uint8_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantUint16(XenonProgramWriterHandle hProgramWriter, uint16_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantUint32(XenonProgramWriterHandle hProgramWriter, uint32_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantUint64(XenonProgramWriterHandle hProgramWriter, uint64_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantFloat32(XenonProgramWriterHandle hProgramWriter, float value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantFloat64(XenonProgramWriterHandle hProgramWriter, double value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, value);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddConstantString(XenonProgramWriterHandle hProgramWriter, const char* value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !value || !pOutputIndex)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonString* const pString = XenonString::Create(value);
	if(!pString)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	(*pOutputIndex) = XenonProgramWriter::AddConstant(hProgramWriter, pString);

	XenonString::Release(pString);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddObjectType(XenonProgramWriterHandle hProgramWriter, const char* objectTypeName)
{
	if(!hProgramWriter || !objectTypeName || objectTypeName[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonString* const pTypeName = XenonString::Create(objectTypeName);
	if(!pTypeName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Verify a object type mapped to the input name doesn't already exist.
	if(hProgramWriter->objectTypes.Contains(pTypeName))
	{
		XenonString::Release(pTypeName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	XenonObjectData objectData;
	objectData.pTypeName = pTypeName;

	hProgramWriter->objectTypes.Insert(pTypeName, objectData);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddObjectMember(
	XenonProgramWriterHandle hProgramWriter,
	const char* objectTypeName,
	const char* memberName,
	int memberValueType
)
{
	if(!hProgramWriter
		|| !objectTypeName
		|| objectTypeName[0] == '\0'
		|| !memberName
		|| memberName[0] == '\0'
		|| memberValueType < 0
		|| memberValueType > XENON_VALUE_TYPE__MAX_VALUE)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonString* const pTypeName = XenonString::Create(objectTypeName);
	if(!pTypeName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	auto typeKv = hProgramWriter->objectTypes.find(pTypeName);

	// Release the object type name string now that we're done with it.
	XenonString::Release(pTypeName);

	if(typeKv == hProgramWriter->objectTypes.end())
	{
		// Cannot add member to an object type that does not exist.
		return XENON_ERROR_KEY_DOES_NOT_EXIST;
	}

	XenonString* const pMemberName = XenonString::Create(memberName);
	if(!pMemberName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Find the member in the object type.
	auto memberKv = typeKv->value.members.find(pMemberName);
	if(memberKv != typeKv->value.members.end())
	{
		XenonString::Release(pMemberName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	// Add the member to the object type.
	typeKv->value.members.Insert(pMemberName, memberValueType);
	typeKv->value.orderedMemberNames.push_back(pMemberName);

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
		XenonString::Release(pKey);
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

	XenonFunctionData function;

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

	XenonFunctionData function;

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
		XenonString::Release(pSignature);
		return XENON_ERROR_KEY_DOES_NOT_EXIST;
	}

	// The string object for the function signature is no longer needed.
	XenonString::Release(pSignature);

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
