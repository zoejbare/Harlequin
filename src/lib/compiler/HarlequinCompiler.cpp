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

#include "../Harlequin.h"

#include "Compiler.hpp"
#include "FunctionData.hpp"
#include "ProgramWriter.hpp"

#include "../base/String.hpp"
#include "../common/OpCodeEnum.hpp"

#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" {

//----------------------------------------------------------------------------------------------------------------------

int HqCompilerCreate(HqCompilerHandle* phOutCompiler, HqCompilerInit init)
{
	if(!phOutCompiler
		|| (*phOutCompiler)
		|| init.common.report.reportLevel < HQ_MESSAGE_TYPE_VERBOSE
		|| init.common.report.reportLevel > HQ_MESSAGE_TYPE_FATAL)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqCompilerHandle hCompiler = HqCompiler::Create(init);

	// The message has to be printed *after* creating the VM state since the state is used in this function.
	HqReportMessage(&hCompiler->report, HQ_MESSAGE_TYPE_VERBOSE, "Initializing Harlequin compiler");

	(*phOutCompiler) = hCompiler;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqCompilerDispose(HqCompilerHandle* phCompiler)
{
	if(!phCompiler || !(*phCompiler))
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqCompilerHandle hCompiler = (*phCompiler);

	(*phCompiler) = HQ_COMPILER_HANDLE_NULL;

	HqReportMessage(&hCompiler->report, HQ_MESSAGE_TYPE_VERBOSE, "Releasing Harlequin compiler");
	HqCompiler::Dispose(hCompiler);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqCompilerGetReportHandle(HqCompilerHandle hCompiler, HqReportHandle* phOutReport)
{
	if(!hCompiler || !phOutReport)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*phOutReport) = &hCompiler->report;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterCreate(HqProgramWriterHandle* phOutProgramWriter, HqCompilerHandle hCompiler)
{
	if(!phOutProgramWriter || (*phOutProgramWriter) || !hCompiler)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqProgramWriterHandle hWriter = HqProgramWriter::Create();
	if(!hWriter)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	(*phOutProgramWriter) = hWriter;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterDispose(HqProgramWriterHandle* phProgramWriter)
{
	if(!phProgramWriter || !(*phProgramWriter))
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqProgramWriterHandle hWriter = (*phProgramWriter);

	(*phProgramWriter) = HQ_PROGRAM_WRITER_HANDLE_NULL;

	HqProgramWriter::Dispose(hWriter);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddDependency(HqProgramWriterHandle hProgramWriter, const char* const programName)
{
	if(!hProgramWriter || !programName || programName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pProgramName = HqString::Create(programName);
	if(!pProgramName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	if(hProgramWriter->dependencies.count(pProgramName))
	{
		HqString::Release(pProgramName);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	hProgramWriter->dependencies.emplace(pProgramName, false);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantNull(HqProgramWriterHandle hProgramWriter, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = hProgramWriter->nullIndex;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantBool(HqProgramWriterHandle hProgramWriter, bool value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = value ? hProgramWriter->boolTrueIndex : hProgramWriter->boolFalseIndex;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantInt8(HqProgramWriterHandle hProgramWriter, int8_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantInt16(HqProgramWriterHandle hProgramWriter, int16_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantInt32(HqProgramWriterHandle hProgramWriter, int32_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantInt64(HqProgramWriterHandle hProgramWriter, int64_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantUint8(HqProgramWriterHandle hProgramWriter, uint8_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantUint16(HqProgramWriterHandle hProgramWriter, uint16_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantUint32(HqProgramWriterHandle hProgramWriter, uint32_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantUint64(HqProgramWriterHandle hProgramWriter, uint64_t value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantFloat32(HqProgramWriterHandle hProgramWriter, float value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantFloat64(HqProgramWriterHandle hProgramWriter, double value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddConstantString(HqProgramWriterHandle hProgramWriter, const char* value, uint32_t* pOutputIndex)
{
	if(!hProgramWriter || !value || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pString = HqString::Create(value);
	if(!pString)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	(*pOutputIndex) = HqProgramWriter::AddConstant(hProgramWriter, pString);

	HqString::Release(pString);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddObjectType(HqProgramWriterHandle hProgramWriter, const char* objectTypeName)
{
	if(!hProgramWriter || !objectTypeName || objectTypeName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pTypeName = HqString::Create(objectTypeName);
	if(!pTypeName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Verify a object type mapped to the input name doesn't already exist.
	if(hProgramWriter->objectTypes.count(pTypeName))
	{
		HqString::Release(pTypeName);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	HqObjectData objectData;
	objectData.pTypeName = pTypeName;

	hProgramWriter->objectTypes.emplace(pTypeName, objectData);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddObjectMember(
	HqProgramWriterHandle hProgramWriter,
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
		|| memberValueType > HQ_VALUE_TYPE__MAX_VALUE)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pTypeName = HqString::Create(objectTypeName);
	if(!pTypeName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	auto typeKv = hProgramWriter->objectTypes.find(pTypeName);

	// Release the object type name string now that we're done with it.
	HqString::Release(pTypeName);

	if(typeKv == hProgramWriter->objectTypes.end())
	{
		// Cannot add member to an object type that does not exist.
		return HQ_ERROR_KEY_DOES_NOT_EXIST;
	}

	HqString* const pMemberName = HqString::Create(memberName);
	if(!pMemberName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Find the member in the object type.
	auto memberKv = typeKv->second.members.find(pMemberName);
	if(memberKv != typeKv->second.members.end())
	{
		HqString::Release(pMemberName);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	if(pOutIndex)
	{
		(*pOutIndex) = uint32_t(typeKv->second.orderedMemberNames.size());
	}

	// Add the member to the object type.
	typeKv->second.members.emplace(pMemberName, memberValueType);
	typeKv->second.orderedMemberNames.push_back(pMemberName);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddGlobal(HqProgramWriterHandle hProgramWriter, const char* variableName, uint32_t constantIndex)
{
	if(!hProgramWriter || !variableName || variableName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Verify the input constant table index is within the correct range.
	if(uint32_t(hProgramWriter->constants.size()) <= constantIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Create the string object to use as the map key.
	HqString* const pKey = HqString::Create(variableName);
	if(!pKey)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Verify a value mapped to the input name doesn't already exist.
	if(hProgramWriter->globals.count(pKey))
	{
		HqString::Release(pKey);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	// Map the global variable name to the constant table index.
	hProgramWriter->globals.emplace(pKey, constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterSetProgramInitFunction(
	HqProgramWriterHandle hProgramWriter,
	const void* const pBytecode,
	const size_t bytecodeLength
)
{
	if(!hProgramWriter
		|| !pBytecode
		|| bytecodeLength == 0)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Copy the input bytecode data to the byte vector that will be set in the program writer.
	std::vector<uint8_t> bytecode(bytecodeLength);
	memcpy(bytecode.data(), pBytecode, bytecode.size());

	hProgramWriter->initBytecode = std::move(bytecode);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddFunction(
	HqProgramWriterHandle hProgramWriter,
	const char* const functionSignature,
	const void* const pBytecode,
	const size_t bytecodeLength,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	if(!hProgramWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| !pBytecode
		|| bytecodeLength == 0
		|| numParameters > HQ_VM_IO_REGISTER_COUNT
		|| numReturnValues > HQ_VM_IO_REGISTER_COUNT)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* pSignature = HqString::Create(functionSignature);
	if(!pSignature)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Copy the input bytecode data to the byte vector that will be mapped into the program writer.
	std::vector<uint8_t> bytecode(bytecodeLength);
	memcpy(bytecode.data(), pBytecode, bytecode.size());

	HqFunctionData function;

	function.bytecode = std::move(bytecode);
	function.numParameters = numParameters;
	function.numReturnValues = numReturnValues;
	function.isNative = false;

	hProgramWriter->functions.emplace(pSignature, function);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddNativeFunction(
	HqProgramWriterHandle hProgramWriter,
	const char* const functionSignature,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	if(!hProgramWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| numParameters > HQ_VM_IO_REGISTER_COUNT
		|| numReturnValues > HQ_VM_IO_REGISTER_COUNT)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* pSignature = HqString::Create(functionSignature);
	if(!pSignature)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	HqFunctionData function;

	function.numParameters = numParameters;
	function.numReturnValues = numReturnValues;
	function.isNative = true;

	hProgramWriter->functions.emplace(pSignature, function);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddLocalVariable(
	HqProgramWriterHandle hProgramWriter,
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
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pSignature = HqString::Create(functionSignature);
	if(!pSignature)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Find the desired function by checking what we currently have mapped.
	auto kv = hProgramWriter->functions.find(pSignature);
	if(kv == hProgramWriter->functions.end())
	{
		HqString::Release(pSignature);
		return HQ_ERROR_KEY_DOES_NOT_EXIST;
	}

	// The string object for the function signature is no longer needed.
	HqString::Release(pSignature);

	// Script local variables cannot be added to native functions.
	if(kv->second.isNative)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	HqString* const pVariableName = HqString::Create(variableName);
	if(!pVariableName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	kv->second.locals.emplace(pVariableName, constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddGuardedBlock(
	HqProgramWriterHandle hProgramWriter,
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
		return HQ_ERROR_INVALID_ARG;
	}

	HqFunctionData* pFunction = nullptr;

	// Find the function in the program writer.
	const int lookupResult = HqProgramWriter::LookupFunction(hProgramWriter, functionSignature, &pFunction);
	if(lookupResult != HQ_SUCCESS)
	{
		return lookupResult;
	}

	if(pFunction->isNative)
	{
		// Cannot add guarded blocks to native functions.
		return HQ_ERROR_INVALID_TYPE;
	}

	// Verify this block isn't already being guarded in this function.
	for(HqFunctionData::GuardedBlock& block : pFunction->guardedBlocks)
	{
		if(block.offset == bytecodeOffset && block.length == bytecodeLength)
		{
			return HQ_ERROR_INVALID_RANGE;
		}
	}

	const uint32_t blockId = uint32_t(pFunction->guardedBlocks.size());

	HqFunctionData::GuardedBlock newBlock;

	newBlock.id = blockId;
	newBlock.offset = uint32_t(bytecodeOffset);
	newBlock.length = uint32_t(bytecodeLength);

	// Add the guarded block to the function.
	pFunction->guardedBlocks.push_back(newBlock);

	(*pOutBlockId) = blockId;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterAddExceptionHandler(
	HqProgramWriterHandle hProgramWriter,
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
		|| handledType > HQ_VALUE_TYPE__MAX_VALUE
		|| (handledType == HQ_VALUE_TYPE_OBJECT && (!className || className[0] == '\0')))
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqFunctionData* pFunction = nullptr;

	// Find the function in the program writer.
	const int lookupResult = HqProgramWriter::LookupFunction(hProgramWriter, functionSignature, &pFunction);
	if(lookupResult != HQ_SUCCESS)
	{
		return lookupResult;
	}

	// Make sure the block ID is valid.
	if(blockId >= uint32_t(pFunction->guardedBlocks.size()))
	{
		return HQ_ERROR_UNKNOWN_ID;
	}

	HqFunctionData::GuardedBlock& guardedBlock = pFunction->guardedBlocks[blockId];

	// Verify a handler at this offset hasn't already been added to this guarded block.
	if(guardedBlock.handlers.count(uint32_t(bytecodeOffset)))
	{
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	HqString* const pClassName = (handledType == HQ_VALUE_TYPE_OBJECT)
		? HqString::Create(className)
		: nullptr;

	if (pClassName)
	{
		// Check if there is already a handler registered for this class type.
		for(auto& kv : guardedBlock.handlers)
		{
			if(kv.second.pClassName && HqString::Compare(kv.second.pClassName, pClassName))
			{
				HqString::Release(pClassName);
				return HQ_ERROR_KEY_ALREADY_EXISTS;
			}
		}
	}

	HqFunctionData::ExceptionHandler newHandler;

	newHandler.pClassName = pClassName;
	newHandler.offset = uint32_t(bytecodeOffset);
	newHandler.type = handledType;

	// Map the new handler to the guarded block.
	guardedBlock.handlers.emplace(newHandler.offset, newHandler);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProgramWriterSerialize(
	HqProgramWriterHandle hProgramWriter,
	HqCompilerHandle hCompiler,
	HqSerializerHandle hSerializer
)
{
	if(!hProgramWriter
		|| !hCompiler
		|| !hSerializer
		|| HqSerializerGetMode(hSerializer) != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Write the program to the serializer.
	if(!HqProgramWriter::Serialize(hProgramWriter, hCompiler, hSerializer))
	{
		return HQ_ERROR_UNSPECIFIED_FAILURE;
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

#define _HQ_WRITE_OP_BYTE(x) \
	if(HqSerializerWriteUint8(hSerializer, x) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}
#define _HQ_WRITE_OP_UDWORD(x) \
	if(HqSerializerWriteUint32(hSerializer, x) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}
#define _HQ_WRITE_OP_SDWORD(x) \
	if(HqSerializerWriteInt32(hSerializer, x) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteNop(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_NOP);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteAbort(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_ABORT);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteReturn(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_RETURN);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteYield(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_YIELD);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCall(HqSerializerHandle hSerializer, const uint32_t constantIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_CALL);
	_HQ_WRITE_OP_UDWORD(constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCallValue(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_CALL_VALUE);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteRaise(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_RAISE);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadConstant(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_LOAD_CONSTANT);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadGlobal(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_LOAD_GLOBAL);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadLocal(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_LOAD_LOCAL);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadParam(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t ioRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_LOAD_PARAM);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(ioRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadObject(
	HqSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t memberIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_LOAD_OBJECT);
	_HQ_WRITE_OP_UDWORD(gpDstRegIndex);
	_HQ_WRITE_OP_UDWORD(gpSrcRegIndex);
	_HQ_WRITE_OP_UDWORD(memberIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadArray(
	HqSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t arrayIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_LOAD_ARRAY);
	_HQ_WRITE_OP_UDWORD(gpDstRegIndex);
	_HQ_WRITE_OP_UDWORD(gpSrcRegIndex);
	_HQ_WRITE_OP_UDWORD(arrayIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteStoreGlobal(
	HqSerializerHandle hSerializer,
	const uint32_t constantIndex,
	const uint32_t gpRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_STORE_GLOBAL);
	_HQ_WRITE_OP_UDWORD(constantIndex);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteStoreLocal(
	HqSerializerHandle hSerializer,
	const uint32_t constantIndex,
	const uint32_t gpRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_STORE_LOCAL);
	_HQ_WRITE_OP_UDWORD(constantIndex);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteStoreParam(
	HqSerializerHandle hSerializer,
	const uint32_t ioRegIndex,
	const uint32_t gpRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_STORE_PARAM);
	_HQ_WRITE_OP_UDWORD(ioRegIndex);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteStoreObject(
	HqSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t memberIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_STORE_OBJECT);
	_HQ_WRITE_OP_UDWORD(gpDstRegIndex);
	_HQ_WRITE_OP_UDWORD(gpSrcRegIndex);
	_HQ_WRITE_OP_UDWORD(memberIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteStoreArray(
	HqSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t arrayIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_STORE_ARRAY);
	_HQ_WRITE_OP_UDWORD(gpDstRegIndex);
	_HQ_WRITE_OP_UDWORD(gpSrcRegIndex);
	_HQ_WRITE_OP_UDWORD(arrayIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePullGlobal(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_PULL_GLOBAL);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePullLocal(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_PULL_LOCAL);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePullParam(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t ioRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_PULL_PARAM);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(ioRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePullObject(
	HqSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t memberIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_PULL_OBJECT);
	_HQ_WRITE_OP_UDWORD(gpDstRegIndex);
	_HQ_WRITE_OP_UDWORD(gpSrcRegIndex);
	_HQ_WRITE_OP_UDWORD(memberIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePullArray(
	HqSerializerHandle hSerializer,
	const uint32_t gpDstRegIndex,
	const uint32_t gpSrcRegIndex,
	const uint32_t arrayIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_PULL_ARRAY);
	_HQ_WRITE_OP_UDWORD(gpDstRegIndex);
	_HQ_WRITE_OP_UDWORD(gpSrcRegIndex);
	_HQ_WRITE_OP_UDWORD(arrayIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePush(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_PUSH);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePop(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_POP);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteInitObject(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_INIT_OBJECT);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteInitArray(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t initialCount
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_INIT_ARRAY);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(initialCount);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteInitFunction(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t constantIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_INIT_FUNCTION);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_UDWORD(constantIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteBranch(HqSerializerHandle hSerializer, const int32_t offset)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_BRANCH);
	_HQ_WRITE_OP_SDWORD(offset);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteBranchIfTrue(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int32_t offset
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_BRANCH_IF_TRUE);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_SDWORD(offset);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteBranchIfFalse(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int32_t offset
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_WRITE_OP_BYTE(HQ_OP_CODE_BRANCH_IF_FALSE);
	_HQ_WRITE_OP_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_SDWORD(offset);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

#undef _HQ_WRITE_OP_BYTE
#undef _HQ_WRITE_OP_UDWORD
#undef _HQ_WRITE_OP_SDWORD

//----------------------------------------------------------------------------------------------------------------------

}
