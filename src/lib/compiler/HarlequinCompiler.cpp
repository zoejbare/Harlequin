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
#include "ModuleWriter.hpp"

#include "../base/Serializer.hpp"
#include "../base/String.hpp"
#include "../common/OpCodeEnum.hpp"

#include <assert.h>
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

int HqModuleWriterCreate(HqModuleWriterHandle* phOutModuleWriter, HqCompilerHandle hCompiler)
{
	if(!phOutModuleWriter || (*phOutModuleWriter) || !hCompiler)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqModuleWriterHandle hWriter = HqModuleWriter::Create();
	if(!hWriter)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	(*phOutModuleWriter) = hWriter;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterDispose(HqModuleWriterHandle* phModuleWriter)
{
	if(!phModuleWriter || !(*phModuleWriter))
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqModuleWriterHandle hWriter = (*phModuleWriter);

	(*phModuleWriter) = HQ_MODULE_WRITER_HANDLE_NULL;

	HqModuleWriter::Dispose(hWriter);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterAddDependency(HqModuleWriterHandle hModuleWriter, const char* const moduleName)
{
	if(!hModuleWriter || !moduleName || moduleName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pModuleName = HqString::Create(moduleName);
	if(!pModuleName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	if(hModuleWriter->dependencies.count(pModuleName))
	{
		HqString::Release(pModuleName);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	hModuleWriter->dependencies.insert(pModuleName);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterAddString(HqModuleWriterHandle hModuleWriter, const char* value, uint32_t* pOutputIndex)
{
	if(!hModuleWriter || !value || !pOutputIndex)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pString = HqString::Create(value);
	if(!pString)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	(*pOutputIndex) = HqModuleWriter::AddString(hModuleWriter, pString);

	HqString::Release(pString);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterAddObjectType(HqModuleWriterHandle hModuleWriter, const char* objectTypeName)
{
	if(!hModuleWriter || !objectTypeName || objectTypeName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pTypeName = HqString::Create(objectTypeName);
	if(!pTypeName)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Verify a object type mapped to the input name doesn't already exist.
	if(hModuleWriter->objectTypes.count(pTypeName))
	{
		HqString::Release(pTypeName);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	HqObjectData objectData;
	objectData.pTypeName = pTypeName;

	hModuleWriter->objectTypes.emplace(pTypeName, objectData);

	// Automatically add the type name to the string table.
	uint32_t stringIndex = 0;
	HqModuleWriterAddString(hModuleWriter, objectTypeName, &stringIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterAddObjectMember(
	HqModuleWriterHandle hModuleWriter,
	const char* objectTypeName,
	const char* memberName,
	uint8_t memberValueType,
	uint32_t* pOutIndex
)
{
	if(!hModuleWriter
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

	auto typeKv = hModuleWriter->objectTypes.find(pTypeName);

	// Release the object type name string now that we're done with it.
	HqString::Release(pTypeName);

	if(typeKv == hModuleWriter->objectTypes.end())
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

	// Automatically add the member name to the string table.
	uint32_t stringIndex = 0;
	HqModuleWriterAddString(hModuleWriter, memberName, &stringIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterAddGlobal(HqModuleWriterHandle hModuleWriter, const char* variableName)
{
	if(!hModuleWriter || !variableName || variableName[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Create the string object to use as the map key.
	HqString* const pKey = HqString::Create(variableName);
	if(!pKey)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Verify the input name doesn't already exist.
	if(hModuleWriter->globals.count(pKey))
	{
		HqString::Release(pKey);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	// Track the name of the global variable.
	hModuleWriter->globals.insert(pKey);

	// Automatically add the variable name to the string table.
	uint32_t stringIndex = 0;
	HqModuleWriterAddString(hModuleWriter, variableName, &stringIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterSetModuleInitFunction(
	HqModuleWriterHandle hModuleWriter,
	const void* const pBytecode,
	const size_t bytecodeLength
)
{
	if(!hModuleWriter
		|| !pBytecode
		|| bytecodeLength == 0)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	// Copy the input bytecode data to the byte vector that will be set in the module writer.
	std::vector<uint8_t> bytecode(bytecodeLength);
	memcpy(bytecode.data(), pBytecode, bytecode.size());

	hModuleWriter->initBytecode = std::move(bytecode);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterAddFunction(
	HqModuleWriterHandle hModuleWriter,
	const char* const functionSignature,
	const void* const pBytecode,
	const size_t bytecodeLength,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	if(!hModuleWriter
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

	if(hModuleWriter->functions.count(pSignature))
	{
		HqString::Release(pSignature);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	// Copy the input bytecode data to the byte vector that will be mapped into the module writer.
	std::vector<uint8_t> bytecode(bytecodeLength);
	memcpy(bytecode.data(), pBytecode, bytecode.size());

	HqFunctionData function;

	function.bytecode = std::move(bytecode);
	function.numParameters = numParameters;
	function.numReturnValues = numReturnValues;
	function.isNative = false;

	hModuleWriter->functions.emplace(pSignature, function);

	// Automatically add the function signature to the string table.
	uint32_t stringIndex = 0;
	HqModuleWriterAddString(hModuleWriter, functionSignature, &stringIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterAddNativeFunction(
	HqModuleWriterHandle hModuleWriter,
	const char* const functionSignature,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	if(!hModuleWriter
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

	if(hModuleWriter->functions.count(pSignature))
	{
		HqString::Release(pSignature);
		return HQ_ERROR_KEY_ALREADY_EXISTS;
	}

	HqFunctionData function;

	function.numParameters = numParameters;
	function.numReturnValues = numReturnValues;
	function.isNative = true;

	hModuleWriter->functions.emplace(pSignature, function);

	// Automatically add the function signature to the string table.
	uint32_t stringIndex = 0;
	HqModuleWriterAddString(hModuleWriter, functionSignature, &stringIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriterAddGuardedBlock(
	HqModuleWriterHandle hModuleWriter,
	const char* const functionSignature,
	const size_t bytecodeOffset,
	const size_t bytecodeLength,
	uint32_t* const pOutBlockId
)
{
	if(!hModuleWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| bytecodeLength == 0
		|| !pOutBlockId)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqFunctionData* pFunction = nullptr;

	// Find the function in the module writer.
	const int lookupResult = HqModuleWriter::LookupFunction(hModuleWriter, functionSignature, &pFunction);
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

int HqModuleWriterAddExceptionHandler(
	HqModuleWriterHandle hModuleWriter,
	const char* const functionSignature,
	const uint32_t blockId,
	const size_t bytecodeOffset,
	const int handledType,
	const char* const className
)
{
	if(!hModuleWriter
		|| !functionSignature
		|| functionSignature[0] == '\0'
		|| handledType < 0
		|| handledType > HQ_VALUE_TYPE__MAX_VALUE
		|| (handledType == HQ_VALUE_TYPE_OBJECT && (!className || className[0] == '\0')))
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqFunctionData* pFunction = nullptr;

	// Find the function in the module writer.
	const int lookupResult = HqModuleWriter::LookupFunction(hModuleWriter, functionSignature, &pFunction);
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
			if(kv.second.pClassName && HqString::FastCompare(kv.second.pClassName, pClassName))
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

int HqModuleWriterSerialize(
	HqModuleWriterHandle hModuleWriter,
	HqReportHandle hReport,
	HqSerializerHandle hSerializer
)
{
	if(!hModuleWriter
		|| !hSerializer
		|| HqSerializerGetMode(hSerializer) != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hModuleWriter->initBytecode.size() == 0)
	{
		// Serialize bytecode for the default init function bytecode when the high-level compiler does not supply it.
		HqSerializerHandle hInitSerializer = HQ_SERIALIZER_HANDLE_NULL;
		HqSerializerCreate(&hInitSerializer, HQ_SERIALIZER_MODE_WRITER);
		HqSerializerSetEndianness(hInitSerializer, hSerializer->endianness);

		// The default init function just returns.
		HqBytecodeWriteReturn(hInitSerializer);

		const void* const pInitBytecode = HqSerializerGetRawStreamPointer(hInitSerializer);
		const size_t initBytecodeLength = HqSerializerGetStreamLength(hInitSerializer);
		assert(initBytecodeLength > 0);

		// Copy the bytecode into the module writer.
		hModuleWriter->initBytecode.resize(initBytecodeLength);
		memcpy(hModuleWriter->initBytecode.data(), pInitBytecode, initBytecodeLength);

		HqSerializerDispose(&hInitSerializer);
	}

	// Write the module to the serializer.
	if(!HqModuleWriter::Serialize(hModuleWriter, hReport, hSerializer))
	{
		return HQ_ERROR_UNSPECIFIED_FAILURE;
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

#define _HQ_WRITE_OP_BOOL(x) \
	if(HqSerializerWriteUint32(hSerializer, (x) ? 1 : 0) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}

#define _HQ_EMIT_UBYTE(x) \
	if(HqSerializerWriteUint32(hSerializer, uint32_t(x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}
#define _HQ_EMIT_SBYTE(x) \
	if(HqSerializerWriteInt32(hSerializer, int32_t(x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}

#define _HQ_EMIT_UWORD(x) \
	if(HqSerializerWriteUint32(hSerializer, uint32_t(x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}
#define _HQ_EMIT_SWORD(x) \
	if(HqSerializerWriteInt32(hSerializer, int32_t(x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}

#define _HQ_EMIT_UDWORD(x) \
	if(HqSerializerWriteUint32(hSerializer, (x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}
#define _HQ_EMIT_SDWORD(x) \
	if(HqSerializerWriteInt32(hSerializer, (x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}

#define _HQ_EMIT_UQWORD(x) \
	if(HqSerializerWriteUint64(hSerializer, (x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}
#define _HQ_EMIT_SQWORD(x) \
	if(HqSerializerWriteInt64(hSerializer, (x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}

#define _HQ_EMIT_FLOAT(x) \
	if(HqSerializerWriteFloat32(hSerializer, (x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}
#define _HQ_EMIT_DOUBLE(x) \
	if(HqSerializerWriteFloat64(hSerializer, (x)) != HQ_SUCCESS) { \
		return HQ_ERROR_NO_WRITE; \
	}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteNop(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_NOP);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteAbort(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_ABORT);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteReturn(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_RETURN);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteYield(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_YIELD);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCall(HqSerializerHandle hSerializer, const uint32_t stringIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CALL);
	_HQ_EMIT_UDWORD(stringIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCallValue(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CALL_VALUE);
	_HQ_EMIT_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteRaise(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_RAISE);
	_HQ_EMIT_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmNull(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_NULL);
	_HQ_EMIT_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmBool(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const bool value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_BOOL);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_WRITE_OP_BOOL(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmI8(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int8_t value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_I8);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_SBYTE(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmI16(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int16_t value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_I16);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_SWORD(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmI32(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int32_t value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_I32);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_SDWORD(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmI64(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int64_t value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_I64);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_SQWORD(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmU8(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint8_t value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_U8);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UBYTE(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmU16(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint16_t value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_U16);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UWORD(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmU32(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_U32);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UDWORD(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmU64(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint64_t value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_U64);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UQWORD(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmF32(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const float value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_F32);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_FLOAT(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmF64(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const double value
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_F64);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_DOUBLE(value);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadImmStr(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t stringIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_IMM_STR);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UDWORD(stringIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLoadGlobal(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t stringIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_GLOBAL);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UDWORD(stringIndex);

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

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_PARAM);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UDWORD(ioRegIndex);

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

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_OBJECT);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);
	_HQ_EMIT_UDWORD(memberIndex);

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

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LOAD_ARRAY);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);
	_HQ_EMIT_UDWORD(arrayIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteStoreGlobal(
	HqSerializerHandle hSerializer,
	const uint32_t stringIndex,
	const uint32_t gpRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_STORE_GLOBAL);
	_HQ_EMIT_UDWORD(stringIndex);
	_HQ_EMIT_UDWORD(gpRegIndex);

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

	_HQ_EMIT_UBYTE(HQ_OP_CODE_STORE_PARAM);
	_HQ_EMIT_UDWORD(ioRegIndex);
	_HQ_EMIT_UDWORD(gpRegIndex);

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

	_HQ_EMIT_UBYTE(HQ_OP_CODE_STORE_OBJECT);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);
	_HQ_EMIT_UDWORD(memberIndex);

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

	_HQ_EMIT_UBYTE(HQ_OP_CODE_STORE_ARRAY);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);
	_HQ_EMIT_UDWORD(arrayIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePush(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_PUSH);
	_HQ_EMIT_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWritePop(HqSerializerHandle hSerializer, const uint32_t gpRegIndex)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_POP);
	_HQ_EMIT_UDWORD(gpRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteInitObject(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t stringIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_INIT_OBJECT);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UDWORD(stringIndex);

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

	_HQ_EMIT_UBYTE(HQ_OP_CODE_INIT_ARRAY);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UDWORD(initialCount);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteInitFunction(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const uint32_t stringIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_INIT_FUNC);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_UDWORD(stringIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteJump(HqSerializerHandle hSerializer, const int32_t offset)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_JMP);
	_HQ_EMIT_SDWORD(offset);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteJumpIfTrue(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int32_t offset
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_JMP_TRUE);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_SDWORD(offset);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteJumpIfFalse(
	HqSerializerHandle hSerializer,
	const uint32_t gpRegIndex,
	const int32_t offset
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_JMP_FALSE);
	_HQ_EMIT_UDWORD(gpRegIndex);
	_HQ_EMIT_SDWORD(offset);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLength(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LENGTH);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteAdd(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_ADD);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteSub(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_SUB);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteMul(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_MUL);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteDiv(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_DIV);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteMod(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_MOD);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteExp(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_EXP);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteBitAnd(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_AND);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteBitOr(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_OR);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteBitXor(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_XOR);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLeftShift(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LSH);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteRightShift(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_RSH);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteLeftRotate(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_LROT);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteRightRotate(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_RROT);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastInt8(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_I8);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastInt16(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_I16);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastInt32(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_I32);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastInt64(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_I64);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastUint8(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_U8);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastUint16(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_U16);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastUint32(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_U32);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastUint64(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_U64);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastFloat32(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_F32);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastFloat64(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_F64);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastBool(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_BOOL);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCastString(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CAST_STR);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCompareEqual(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CMP_EQ);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCompareNotEqual(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CMP_NE);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCompareLess(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CMP_LT);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCompareLessEqual(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CMP_LE);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCompareGreater(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CMP_GT);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCompareGreaterEqual(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcLeftRegIndex,
	uint32_t gpSrcRightRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_CMP_GE);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcLeftRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRightRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteTest(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_TEST);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteMove(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_MOVE);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqBytecodeWriteCopy(
	HqSerializerHandle hSerializer,
	uint32_t gpDstRegIndex,
	uint32_t gpSrcRegIndex
)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	_HQ_EMIT_UBYTE(HQ_OP_CODE_COPY);
	_HQ_EMIT_UDWORD(gpDstRegIndex);
	_HQ_EMIT_UDWORD(gpSrcRegIndex);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

#undef _HQ_EMIT_UBYTE
#undef _HQ_EMIT_SBYTE

#undef _HQ_EMIT_UWORD
#undef _HQ_EMIT_SWORD

#undef _HQ_EMIT_UDWORD
#undef _HQ_EMIT_SDWORD

#undef _HQ_EMIT_UQWORD
#undef _HQ_EMIT_SQWORD

#undef _HQ_EMIT_FLOAT
#undef _HQ_EMIT_DOUBLE

//----------------------------------------------------------------------------------------------------------------------

}
