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
#include "../common/OpCodeEnum.hpp"

#include <string.h>

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

	if(hProgramWriter->dependencies.count(pProgramName))
	{
		XenonString::Release(pProgramName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	hProgramWriter->dependencies.emplace(pProgramName, false);

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
	if(hProgramWriter->objectTypes.count(pTypeName))
	{
		XenonString::Release(pTypeName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	XenonObjectData objectData;
	objectData.pTypeName = pTypeName;

	hProgramWriter->objectTypes.emplace(pTypeName, objectData);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddObjectMember(
	XenonProgramWriterHandle hProgramWriter,
	const char* objectTypeName,
	const char* memberName,
	uint8_t memberValueType,
	uint32_t* pOutIndex
)
{
	if(!hProgramWriter
		|| !objectTypeName
		|| objectTypeName[0] == '\0'
		|| !memberName
		|| memberName[0] == '\0'
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
	auto memberKv = typeKv->second.members.find(pMemberName);
	if(memberKv != typeKv->second.members.end())
	{
		XenonString::Release(pMemberName);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	if(pOutIndex)
	{
		(*pOutIndex) = uint32_t(typeKv->second.orderedMemberNames.size());
	}

	// Add the member to the object type.
	typeKv->second.members.emplace(pMemberName, memberValueType);
	typeKv->second.orderedMemberNames.push_back(pMemberName);

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
	if(hProgramWriter->globals.count(pKey))
	{
		XenonString::Release(pKey);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	// Map the global variable name to the constant table index.
	hProgramWriter->globals.emplace(pKey, constantIndex);

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

	hProgramWriter->functions.emplace(pSignature, function);

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

	hProgramWriter->functions.emplace(pSignature, function);

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
	if(kv->second.isNative)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	XenonString* const pVariableName = XenonString::Create(variableName);
	if(!pVariableName)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	kv->second.locals.emplace(pVariableName, constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddGuardedBlock(
	XenonProgramWriterHandle hProgramWriter,
	const char* const functionSignature,
	const size_t bytecodeOffset,
	const size_t bytecodeLength,
	uint32_t* const pOutBlockId
)
{
	if(!hProgramWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| bytecodeLength == 0
		|| !pOutBlockId)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonFunctionData* pFunction = nullptr;

	// Find the function in the program writer.
	const int lookupResult = XenonProgramWriter::LookupFunction(hProgramWriter, functionSignature, &pFunction);
	if(lookupResult != XENON_SUCCESS)
	{
		return lookupResult;
	}

	if(pFunction->isNative)
	{
		// Cannot add guarded blocks to native functions.
		return XENON_ERROR_INVALID_TYPE;
	}

	// Verify this block isn't already being guarded in this function.
	for(XenonFunctionData::GuardedBlock& block : pFunction->guardedBlocks)
	{
		if(block.offset == bytecodeOffset && block.length == bytecodeLength)
		{
			return XENON_ERROR_INVALID_RANGE;
		}
	}

	const uint32_t blockId = uint32_t(pFunction->guardedBlocks.size());

	XenonFunctionData::GuardedBlock newBlock;

	newBlock.id = blockId;
	newBlock.offset = uint32_t(bytecodeOffset);
	newBlock.length = uint32_t(bytecodeLength);

	// Add the guarded block to the function.
	pFunction->guardedBlocks.push_back(newBlock);

	(*pOutBlockId) = blockId;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterAddExceptionHandler(
	XenonProgramWriterHandle hProgramWriter,
	const char* const functionSignature,
	const uint32_t blockId,
	const size_t bytecodeOffset,
	const int handledType,
	const char* const className
)
{
	if(!hProgramWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| handledType < 0
		|| handledType > XENON_VALUE_TYPE__MAX_VALUE
		|| (handledType == XENON_VALUE_TYPE_OBJECT && (!className || className[0] == '\0')))
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonFunctionData* pFunction = nullptr;

	// Find the function in the program writer.
	const int lookupResult = XenonProgramWriter::LookupFunction(hProgramWriter, functionSignature, &pFunction);
	if(lookupResult != XENON_SUCCESS)
	{
		return lookupResult;
	}

	// Make sure the block ID is valid.
	if(blockId >= uint32_t(pFunction->guardedBlocks.size()))
	{
		return XENON_ERROR_UNKNOWN_ID;
	}

	XenonFunctionData::GuardedBlock& guardedBlock = pFunction->guardedBlocks[blockId];

	// Verify a handler at this offset hasn't already been added to this guarded block.
	if(guardedBlock.handlers.count(uint32_t(bytecodeOffset)))
	{
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	XenonString* const pClassName = (handledType == XENON_VALUE_TYPE_OBJECT)
		? XenonString::Create(className)
		: nullptr;

	if (pClassName)
	{
		// Check if there is already a handler registered for this class type.
		for(auto& kv : guardedBlock.handlers)
		{
			if(kv.second.pClassName && XenonString::Compare(kv.second.pClassName, pClassName))
			{
				XenonString::Release(pClassName);
				return XENON_ERROR_KEY_ALREADY_EXISTS;
			}
		}
	}

	XenonFunctionData::ExceptionHandler newHandler;

	newHandler.pClassName = pClassName;
	newHandler.offset = uint32_t(bytecodeOffset);
	newHandler.type = handledType;

	// Map the new handler to the guarded block.
	guardedBlock.handlers.emplace(newHandler.offset, newHandler);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriterSerialize(
	XenonProgramWriterHandle hProgramWriter,
	XenonCompilerHandle hCompiler,
	XenonSerializerHandle hSerializer
)
{
	if(!hProgramWriter
		|| !hCompiler
		|| !hSerializer
		|| XenonSerializerGetMode(hSerializer) != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Write the program to the serializer.
	if(!XenonProgramWriter::Serialize(hProgramWriter, hCompiler, hSerializer))
	{
		return XENON_ERROR_UNSPECIFIED_FAILURE;
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

#define _XENON_WRITE_OP_BYTE(x) \
	if(XenonSerializerWriteUint8(hSerializer, x) != XENON_SUCCESS) { \
		return XENON_ERROR_NO_WRITE; \
	}
#define _XENON_WRITE_OP_UDWORD(x) \
	if(XenonSerializerWriteUint32(hSerializer, x) != XENON_SUCCESS) { \
		return XENON_ERROR_NO_WRITE; \
	}
#define _XENON_WRITE_OP_SDWORD(x) \
	if(XenonSerializerWriteInt32(hSerializer, x) != XENON_SUCCESS) { \
		return XENON_ERROR_NO_WRITE; \
	}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteNop(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_NOP);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteAbort(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_ABORT);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteReturn(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_RETURN);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteYield(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_YIELD);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteCall(XenonSerializerHandle hSerializer, const uint32_t constantIndex)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_CALL);
	_XENON_WRITE_OP_UDWORD(constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteRaise(XenonSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_RAISE);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteLoadConstant(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_LOAD_CONSTANT);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteLoadGlobal(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_LOAD_GLOBAL);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteLoadLocal(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_LOAD_LOCAL);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteLoadParam(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t ioRegIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_LOAD_PARAM);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(ioRegIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteLoadObject(
	XenonSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t memberIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_LOAD_OBJECT);
	_XENON_WRITE_OP_UDWORD(gpDstRegIndex);
	_XENON_WRITE_OP_UDWORD(gpSrcRegIndex);
	_XENON_WRITE_OP_UDWORD(memberIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteStoreGlobal(
	XenonSerializerHandle hSerializer,
	const uint32_t constantIndex,
	const uint32_t gpRegIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_STORE_GLOBAL);
	_XENON_WRITE_OP_UDWORD(constantIndex);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteStoreLocal(
	XenonSerializerHandle hSerializer,
	const uint32_t constantIndex,
	const uint32_t gpRegIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_STORE_LOCAL);
	_XENON_WRITE_OP_UDWORD(constantIndex);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteStoreParam(
	XenonSerializerHandle hSerializer,
	const uint32_t ioRegIndex,
	const uint32_t gpRegIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_STORE_PARAM);
	_XENON_WRITE_OP_UDWORD(ioRegIndex);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteStoreObject(
	XenonSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t memberIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_STORE_OBJECT);
	_XENON_WRITE_OP_UDWORD(gpDstRegIndex);
	_XENON_WRITE_OP_UDWORD(gpSrcRegIndex);
	_XENON_WRITE_OP_UDWORD(memberIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWritePullGlobal(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_PULL_GLOBAL);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWritePullLocal(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_PULL_LOCAL);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWritePullParam(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t ioRegIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_PULL_PARAM);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(ioRegIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWritePullObject(
	XenonSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t memberIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_PULL_OBJECT);
	_XENON_WRITE_OP_UDWORD(gpDstRegIndex);
	_XENON_WRITE_OP_UDWORD(gpSrcRegIndex);
	_XENON_WRITE_OP_UDWORD(memberIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWritePush(XenonSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_PUSH);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWritePop(XenonSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_POP);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteInitObject(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_INIT_OBJECT);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(constantIndex);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteInitArray(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t initialCount
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_INIT_ARRAY);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_UDWORD(initialCount);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteBranch(XenonSerializerHandle hSerializer, const int32_t offset)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_BRANCH);
	_XENON_WRITE_OP_SDWORD(offset);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteBranchIfTrue(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int32_t offset
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_BRANCH_IF_TRUE);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_SDWORD(offset);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonBytecodeWriteBranchIfFalse(
	XenonSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int32_t offset
)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	_XENON_WRITE_OP_BYTE(XENON_OP_CODE_BRANCH_IF_FALSE);
	_XENON_WRITE_OP_UDWORD(gpRegIndex);
	_XENON_WRITE_OP_SDWORD(offset);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

#undef _XENON_WRITE_OP_BYTE
#undef _XENON_WRITE_OP_UDWORD
#undef _XENON_WRITE_OP_SDWORD

//----------------------------------------------------------------------------------------------------------------------

}
