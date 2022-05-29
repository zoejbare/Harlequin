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

#include "ProgramWriter.hpp"
#include "Compiler.hpp"

#include "../common/program-format/FileHeader.hpp"
#include "../common/program-format/ProgramHeader.hpp"

#include <algorithm>
#include <assert.h>
#include <inttypes.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

static bool SerializeString(
	XenonSerializerHandle hSerializer,
	XenonReportHandle hReport,
	const char* const stringData,
	const size_t stringLength
)
{
	assert(hSerializer != XENON_SERIALIZER_HANDLE_NULL);
	assert(hReport != XENON_REPORT_HANDLE_NULL);

	int result = 0;

	if(stringLength > 0)
	{
		assert(stringData != nullptr);

		result = XenonSerializerWriteBuffer(hSerializer, stringLength, stringData);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to write string data: error=\"%s\", data=\"%s\"",
				errorString,
				stringData
			);

			return false;
		}
	}

	// Always write the null-terminator for strings.
	result = XenonSerializerWriteUint8(hSerializer, 0);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write string null-terminator: error=\"%s\"",
			errorString
		);

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

static bool SerializeValue(
	XenonSerializerHandle hSerializer,
	XenonProgramWriter::ValueContainer& value,
	XenonReportHandle hReport
)
{
	assert(hSerializer != XENON_SERIALIZER_HANDLE_NULL);
	assert(hReport != XENON_REPORT_HANDLE_NULL);

	int result = 0;

	result = XenonSerializerWriteUint8(hSerializer, uint8_t(value.type));
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write value type: error=\"%s\", type=%" PRIu32,
			errorString,
			value.type
		);

		return false;
	}

	switch(value.type)
	{
		case XENON_VALUE_TYPE_NULL:
			break;

		case XENON_VALUE_TYPE_INT8:
			result = XenonSerializerWriteInt8(hSerializer, value.as.int8);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as int8: error=\"%s\", data=%" PRId8,
					errorString,
					value.as.int8
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_INT16:
			result = XenonSerializerWriteInt16(hSerializer, value.as.int16);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as int16: error=\"%s\", data=%" PRId16,
					errorString,
					value.as.int16
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_INT32:
			result = XenonSerializerWriteInt32(hSerializer, value.as.int32);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as int32: error=\"%s\", data=%" PRId32,
					errorString,
					value.as.int32
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_INT64:
			result = XenonSerializerWriteInt64(hSerializer, value.as.int64);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as int64: error=\"%s\", data=%" PRId64,
					errorString,
					value.as.int64
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_UINT8:
			result = XenonSerializerWriteUint8(hSerializer, value.as.uint8);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as uint8: error=\"%s\", data=%" PRIu8,
					errorString,
					value.as.uint8
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_UINT16:
			result = XenonSerializerWriteUint16(hSerializer, value.as.uint16);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as uint16: error=\"%s\", data=%" PRIu16,
					errorString,
					value.as.uint16
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_UINT32:
			result = XenonSerializerWriteUint32(hSerializer, value.as.uint32);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as uint32: error=\"%s\", data=%" PRIu32,
					errorString,
					value.as.uint32
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_UINT64:
			result = XenonSerializerWriteUint64(hSerializer, value.as.uint64);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as uint64: error=\"%s\", data=%" PRIu64,
					errorString,
					value.as.uint64
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_FLOAT32:
			result = XenonSerializerWriteFloat32(hSerializer, value.as.float32);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as float: error=\"%s\", data=%f",
					errorString,
					value.as.float32
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_FLOAT64:
			result = XenonSerializerWriteFloat64(hSerializer, value.as.float64);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as double: error=\"%s\", data=%f",
					errorString,
					value.as.float64
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_BOOL:
			result = XenonSerializerWriteBool(hSerializer, value.as.boolean);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as bool: error=\"%s\", data=%s",
					errorString,
					value.as.boolean ? "true" : "false"
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_STRING:
			// Write out the string data.
			if(!SerializeString(hSerializer, hReport, value.as.pString->data, value.as.pString->length))
			{
				return false;
			}
			break;

		case XENON_VALUE_TYPE_OBJECT:
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Cannot serializer object value type"
			);
			return false;

		default:
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Cannot serialize unknown value type: type=%" PRId32,
				value.type
			);
			return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

XenonProgramWriterHandle XenonProgramWriter::Create()
{
	XenonProgramWriter* const pOutput = new XenonProgramWriter();

	ValueContainer value;

	// Add the default null value constant.
	value.type = XENON_VALUE_TYPE_NULL;
	value.as.pString = nullptr;
	pOutput->nullIndex = uint32_t(pOutput->constants.size());
	pOutput->constants.push_back(value);

	// Add the default boolean (false) constant.
	value.type = XENON_VALUE_TYPE_BOOL;
	value.as.boolean = false;
	pOutput->boolFalseIndex = uint32_t(pOutput->constants.size());
	pOutput->constants.push_back(value);

	// Add the default boolean (true) constant.
	value.as.boolean = true;
	pOutput->boolTrueIndex = uint32_t(pOutput->constants.size());
	pOutput->constants.push_back(value);

	// Serialize bytecode for the default init function bytecode just in case the high-level compiler does not supply it.
	XenonSerializerHandle hInitSerializer = XENON_SERIALIZER_HANDLE_NULL;
	XenonSerializerCreate(&hInitSerializer, XENON_SERIALIZER_MODE_WRITER);
	XenonBytecodeWriteReturn(hInitSerializer);

	const void* const pInitBytecode = XenonSerializerGetRawStreamPointer(hInitSerializer);
	const size_t initBytecodeLength = XenonSerializerGetStreamLength(hInitSerializer);

	if(initBytecodeLength > 0)
	{
		// Copy the default init bytecode into the program writer.
		pOutput->initBytecode.resize(initBytecodeLength);
		memcpy(pOutput->initBytecode.data(), pInitBytecode, initBytecodeLength);
	}

	XenonSerializerDispose(&hInitSerializer);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonProgramWriter::Dispose(XenonProgramWriterHandle hProgramWriter)
{
	assert(hProgramWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	// Dispose of all dependency names.
	for(auto& kv : hProgramWriter->dependencies)
	{
		XenonString::Release(kv.first);
	}

	// Dispose of all program globals.
	for(auto& kv : hProgramWriter->globals)
	{
		XenonString::Release(kv.first);
	}

	// Dispose of all program constants.
	for(ValueContainer& value : hProgramWriter->constants)
	{
		if(value.type == XENON_VALUE_TYPE_STRING)
		{
			XenonString::Release(value.as.pString);
		}
	}

	// Dispose of all functions.
	for(auto& funcKv : hProgramWriter->functions)
	{
		XenonString::Release(funcKv.first);

		for(auto& valueKv : funcKv.second.locals)
		{
			XenonString::Release(valueKv.first);
		}

		// Dispose of each guarded block.
		for(XenonFunctionData::GuardedBlock& guardedBlock : funcKv.second.guardedBlocks)
		{
			// Dispose of each exception handler registered under this guarded block.
			for(auto& handlerKv : guardedBlock.handlers)
			{
				if(handlerKv.second.pClassName)
				{
					XenonString::Release(handlerKv.second.pClassName);
				}
			}
		}
	}

	// Dispose of all object types.
	for(auto& typeKv : hProgramWriter->objectTypes)
	{
		XenonString::Release(typeKv.second.pTypeName);

		// Dispose of all members belonging to the current object type.
		for(auto& memberKv : typeKv.second.members)
		{
			XenonString::Release(memberKv.first);
		}
	}

	delete hProgramWriter;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonProgramWriter::Serialize(
	XenonProgramWriterHandle hProgramWriter,
	XenonCompilerHandle hCompiler,
	XenonSerializerHandle hSerializer
)
{
	assert(hProgramWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);
	assert(hCompiler != XENON_COMPILER_HANDLE_NULL);
	assert(hSerializer != XENON_SERIALIZER_HANDLE_NULL);

	XenonReportHandle hReport = &hCompiler->report;

	XenonFileHeader fileHeader = {};
	XenonProgramHeader programHeader = {};

	fileHeader.magicNumber[0] = 'X';
	fileHeader.magicNumber[1] = 'P';
	fileHeader.magicNumber[2] = 'R';
	fileHeader.magicNumber[3] = 'G';
	fileHeader.magicNumber[4] = '_';

	const int endianness = XenonSerializerGetEndianness(hSerializer);

	// Set the big endian flag.
	switch(endianness)
	{
		case XENON_ENDIAN_ORDER_LITTLE:
			fileHeader.bigEndianFlag = 0;
			break;

		case XENON_ENDIAN_ORDER_BIG:
			fileHeader.bigEndianFlag = 1;
			break;

		default:
#ifdef XENON_CPU_ENDIAN_LITTLE
			fileHeader.bigEndianFlag = 0;
#else
			fileHeader.bigEndianFlag = 1;
#endif
			break;
	}

	auto getAlignedSize = [](const size_t size) -> size_t
	{
		// (size + (alignment - 1)) & ~(alignment - 1)
		return (size + 63) & ~63;
	};

	struct FunctionBinding
	{
		XenonFunctionData* pFunction;
		XenonString* pSignature;

		uint32_t offset;
		uint32_t length;
	};

	std::deque<FunctionBinding> functionBindings;
	XenonFunctionData::Bytecode bytecode;

	// Get the function bindings and bytecode ready to be written out.
	{
		// The program's init function will always be at the start of the bytecode.
		// All other functions will be defined after it.
		uint32_t bytecodeLength = uint32_t(getAlignedSize(hProgramWriter->initBytecode.size()));

		// Build the binding data for each function while calculating total length of the bytecode.
		for(auto& kv : hProgramWriter->functions)
		{
			FunctionBinding binding;

			binding.pFunction = &kv.second;
			binding.pSignature = kv.first;
			binding.offset = uint32_t(bytecodeLength);
			binding.length = uint32_t(binding.pFunction->bytecode.size());

			// Align the bytecode length to add padding between each function.
			bytecodeLength += uint32_t(getAlignedSize(binding.length));

			functionBindings.push_back(binding);
		}

		// Allocate space for the entire block of bytecode for the program.
		bytecode.resize(bytecodeLength);

		// Clear the bytecode buffer.
		uint8_t* const pProgramBytecode = bytecode.data();
		memset(pProgramBytecode, 0, bytecode.size());

		// Copy the program's init function to the start of the bytecode.
		if(hProgramWriter->initBytecode.size() > 0)
		{
			memcpy(bytecode.data(), hProgramWriter->initBytecode.data(), hProgramWriter->initBytecode.size());
		}

		// Fill out the full program bytecode from each function's individual bytecode.
		for(const FunctionBinding& binding : functionBindings)
		{
			memcpy(
				pProgramBytecode + binding.offset,
				binding.pFunction->bytecode.data(),
				binding.pFunction->bytecode.size()
			);
		}
	}

	programHeader.dependencyTable.length = uint32_t(hProgramWriter->dependencies.size());
	programHeader.objectTable.length = uint32_t(hProgramWriter->objectTypes.size());
	programHeader.constantTable.length = uint32_t(hProgramWriter->constants.size());
	programHeader.globalTable.length = uint32_t(hProgramWriter->globals.size());
	programHeader.bytecode.length = uint32_t(bytecode.size());
	programHeader.functionTable.length = uint32_t(functionBindings.size());
	programHeader.initFunctionLength = uint32_t(hProgramWriter->initBytecode.size());

	// TODO: Add support for program extensions.
	programHeader.extensionTable.offset = 0;
	programHeader.extensionTable.length = 0;

	int result = XENON_SUCCESS;

	// Write the common header.
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.magicNumber[0]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.magicNumber[1]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.magicNumber[2]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.magicNumber[3]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.magicNumber[4]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[0]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[1]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[2]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[3]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[4]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[5]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[6]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[7]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[8]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.reserved[9]); }
	if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint8(hSerializer, fileHeader.bigEndianFlag); }

	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write program common header: error=\"%s\"",
			errorString
		);

		return false;
	}

	auto writeVersionHeader = [&hSerializer, &programHeader]() -> int
	{
		int result = XENON_SUCCESS;

		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.dependencyTable.offset); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.dependencyTable.length); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.objectTable.offset); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.objectTable.length); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.constantTable.offset); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.constantTable.length); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.globalTable.offset); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.globalTable.length); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.functionTable.offset); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.functionTable.length); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.extensionTable.offset); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.extensionTable.length); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.bytecode.offset); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.bytecode.length); }
		if(result == XENON_SUCCESS) { result = XenonSerializerWriteUint32(hSerializer, programHeader.initFunctionLength); }

		return result;
	};

	const size_t versionHeaderPosition = XenonSerializerGetStreamPosition(hSerializer);

	// Write temporary data for the version header.
	// We'll come back to fill it out at the end.
	result = writeVersionHeader();

	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write program version header data: error=\"%s\"",
			errorString
		);

		return false;
	}

	programHeader.dependencyTable.offset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the dependency table.
	for(auto& kv : hProgramWriter->dependencies)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing dependency: name=\"%s\"", kv.first->data);

		if(!SerializeString(hSerializer, hReport, kv.first->data, kv.first->length))
		{
			return false;
		}
	}

	programHeader.objectTable.offset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the object type schemas.
	for(auto& typeKv : hProgramWriter->objectTypes)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing object type: name=\"%s\"", typeKv.first->data);

		if(!SerializeString(hSerializer, hReport, typeKv.first->data, typeKv.first->length))
		{
			return false;
		}

		const uint32_t memberCount = uint32_t(typeKv.second.orderedMemberNames.size());

		// Write the number of members belonging to this object.
		result = XenonSerializerWriteUint32(hSerializer, memberCount);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize object member count: error=\"%s\", objectType=\"%s\", memberCount=%" PRIu32,
				errorString,
				typeKv.first->data,
				memberCount
			);

			return false;
		}

		for(size_t memberIndex = 0; memberIndex < typeKv.second.orderedMemberNames.size(); ++memberIndex)
		{
			XenonString* const pMemberName = typeKv.second.orderedMemberNames[memberIndex];
			const uint8_t memberValueType = uint8_t(typeKv.second.members[pMemberName]);

			const char* const memberTypeString = XenonGetValueTypeString(memberValueType);

			XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, " - Serializing object member: name=\"%s\", type=%s" , pMemberName->data, memberTypeString);

			// Write the member name string.
			if(!SerializeString(hSerializer, hReport, pMemberName->data, pMemberName->length))
			{
				return false;
			}

			// Write the member value type.
			result = XenonSerializerWriteUint8(hSerializer, uint8_t(memberValueType));
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to serialize object member type: error=\"%s\", objectType=\"%s\", memberName=\"%s\", memberType=%s",
					errorString,
					typeKv.first->data,
					pMemberName->data,
					memberTypeString
				);

				return false;
			}
		}
	}

	programHeader.constantTable.offset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the constant table.
	for(size_t index = 0; index < hProgramWriter->constants.size(); ++index)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing constant: index=%" PRIuPTR, index);

		if(!SerializeValue(hSerializer, hProgramWriter->constants[index], hReport))
		{
			return false;
		}
	}

	programHeader.globalTable.offset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the global variable table.
	for(auto& kv : hProgramWriter->globals)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing global variable: name=\"%s\"", kv.first->data);

		// First, write the string key of the global.
		if(!SerializeString(hSerializer, hReport, kv.first->data, kv.first->length))
		{
			return false;
		}

		// Write the global's constant index.
		result = XenonSerializerWriteUint32(hSerializer, kv.second);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize global variable value index: error=\"%s\", name=\"%s\", index=%" PRIu32,
				errorString,
				kv.first->data,
				kv.second
			);

			return false;
		}
	}

	programHeader.functionTable.offset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the function table.
	for(const FunctionBinding& binding : functionBindings)
	{
		if(binding.pFunction->isNative)
		{
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_VERBOSE,
				"Serializing native function: signature=\"%s\", numParams=%" PRIu16 ", numReturnValues=%" PRIu16,
				binding.pSignature->data,
				binding.pFunction->numParameters,
				binding.pFunction->numReturnValues
			);
		}
		else
		{
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_VERBOSE,
				"Serializing script function: signature=\"%s\", numParams=%" PRIu16
				", numReturnValues=%" PRIu16 ", offset=0x%" PRIX32 ", length=%" PRIu32,
				binding.pSignature->data,
				binding.pFunction->numParameters,
				binding.pFunction->numReturnValues,
				binding.offset,
				binding.length
			);
		}

		// Write the function signature.
		if(!SerializeString(hSerializer, hReport, binding.pSignature->data, binding.pSignature->length))
		{
			return false;
		}

		// Write the function's native switch into the program file.
		result = XenonSerializerWriteBool(hSerializer, binding.pFunction->isNative);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function 'isNative' flag: error=\"%s\", signature=\"%s\", native=%s",
				errorString,
				binding.pSignature->data,
				binding.pFunction->isNative ? "true" : "false"
			);

			return false;
		}

		// Write the function's parameter count into the program file.
		result = XenonSerializerWriteUint16(hSerializer, binding.pFunction->numParameters);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function parameter count: error=\"%s\", signature=\"%s\", count=%" PRIu16,
				errorString,
				binding.pSignature->data,
				binding.pFunction->numParameters
			);

			return false;
		}

		// Write the function's return value count into the program file.
		result = XenonSerializerWriteUint16(hSerializer, binding.pFunction->numReturnValues);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function return value count: error=\"%s\", signature=\"%s\", count=%" PRIu16,
				errorString,
				binding.pSignature->data,
				binding.pFunction->numReturnValues
			);

			return false;
		}

		if(!binding.pFunction->isNative)
		{
			// Write the function's offset into the program file.
			result = XenonSerializerWriteUint32(hSerializer, binding.offset);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to serialize function offset: error=\"%s\", signature=\"%s\", offset=0x%" PRIX32,
					errorString,
					binding.pSignature->data,
					binding.offset
				);

				return false;
			}

			// Write the function's length in bytes into the program file.
			result = XenonSerializerWriteUint32(hSerializer, binding.length);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to serialize function offset: error=\"%s\", signature=\"%s\", length=%" PRIu32,
					errorString,
					binding.pSignature->data,
					binding.length
				);

				return false;
			}

			// Write the function's local variable count.
			result = XenonSerializerWriteUint32(hSerializer, uint32_t(binding.pFunction->locals.size()));
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to serialize function local variable count: error=\"%s\", signature=\"%s\", count=%" PRIu32,
					errorString,
					binding.pSignature->data,
					uint32_t(binding.pFunction->locals.size())
				);

				return false;
			}

			// Write the function's local variable table.
			for(auto& kv : binding.pFunction->locals)
			{
				XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, " - Serializing local variable: name=\"%s\"", kv.first->data);

				// First, write the string key of the local.
				if(!SerializeString(hSerializer, hReport, kv.first->data, kv.first->length))
				{
					return false;
				}

				// Write the local's constant index.
				result = XenonSerializerWriteUint32(hSerializer, kv.second);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to serialize local variable value index: error=\"%s\", name=\"%s\", index=%" PRIu32,
						errorString,
						kv.first->data,
						kv.second
					);

					return false;
				}
			}

			const size_t guardedBlockCount = uint32_t(binding.pFunction->guardedBlocks.size());

			// Write the function's guarded block count into the program file.
			result = XenonSerializerWriteUint32(hSerializer, uint32_t(guardedBlockCount));
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to serialize function guarded block count: error=\"%s\", signature=\"%s\", count=%zu",
					errorString,
					binding.pSignature->data,
					guardedBlockCount
				);

				return false;
			}

			auto guardedBlockSortFunc = [](
				const XenonFunctionData::GuardedBlock& left,
				const XenonFunctionData::GuardedBlock& right
			) -> bool
			{
				if(left.offset < right.offset)
				{
					// Earlier guarded blocks are sorted before the blocks that come after them.
					return true;
				}
				else if(left.offset == right.offset)
				{
					// Nested blocks are sorted *after* the blocks that contain them.
					if(left.length > right.length)
					{
						return true;
					}
				}

				return false;
			};

			auto exceptionHandlerSortFunc = [](
				const XenonFunctionData::ExceptionHandler& left,
				const XenonFunctionData::ExceptionHandler& right
			) -> bool
			{
				return left.offset < right.offset;
			};

			// Sort the guarded blocks for this function.
			std::sort(
				binding.pFunction->guardedBlocks.begin(),
				binding.pFunction->guardedBlocks.end(),
				guardedBlockSortFunc
			);

			// Serialize the guarded blocks with their exception handlers.
			for(const XenonFunctionData::GuardedBlock& block : binding.pFunction->guardedBlocks)
			{
				XenonFunctionData::ExceptionHandler::Vector exceptionHandlers;
				exceptionHandlers.reserve(block.handlers.size());

				// Build a flat array of exception handlers for this guarded block.
				for(auto& kv : block.handlers)
				{
					exceptionHandlers.push_back(kv.second);
				}

				// Sort the array of exception handlers.
				std::sort(exceptionHandlers.begin(), exceptionHandlers.end(), exceptionHandlerSortFunc);

				// Calculate the offset from the start of the bytecode for this guarded block.
				const uint32_t blockOffset = binding.offset + block.offset;

				// Write the guarded block bytecode offset.
				result = XenonSerializerWriteUint32(hSerializer, blockOffset);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to write guarded block bytecode offset: error=\"%s\", signature=\"%s\", offset=%" PRIu32,
						errorString,
						binding.pSignature->data,
						block.offset
					);

					return false;
				}

				// Write the guarded block bytecode length.
				result = XenonSerializerWriteUint32(hSerializer, block.length);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to write guarded block bytecode length: error=\"%s\", signature=\"%s\", length=%" PRIu32,
						errorString,
						binding.pSignature->data,
						block.length
					);

					return false;
				}

				const uint32_t exceptionHandlerCount = uint32_t(exceptionHandlers.size());

				// Write the number of exception handlers contained in this guarded block.
				result = XenonSerializerWriteUint32(hSerializer, exceptionHandlerCount);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to write guarded block exception handler count: error=\"%s\", signature=\"%s\", count=%" PRIu32,
						errorString,
						binding.pSignature->data,
						exceptionHandlerCount
					);

					return false;
				}

				// Serialize each exception handler contained by this guarded block.
				for(uint32_t handlerIndex = 0; handlerIndex < exceptionHandlerCount; ++handlerIndex)
				{
					const XenonFunctionData::ExceptionHandler& handler = exceptionHandlers[handlerIndex];

					// Write the value type for this exception handler.
					result = XenonSerializerWriteUint8(hSerializer, uint8_t(handler.type));
					if(result != XENON_SUCCESS)
					{
						const char* const errorString = XenonGetErrorCodeString(result);
						const char* const valueTypeString = XenonGetValueTypeString(handler.type);

						XenonReportMessage(
							hReport,
							XENON_MESSAGE_TYPE_ERROR,
							"Failed to write exception handler type: error=\"%s\", signature=\"%s\", type=\"%s\"",
							errorString,
							binding.pSignature->data,
							valueTypeString
						);

						return false;
					}

					// Calculate the offset from the start of the bytecode for this exception handler.
					const uint32_t handlerOffset = binding.offset + handler.offset;

					// Write the offset where this exception handler is located.
					result = XenonSerializerWriteUint32(hSerializer, handlerOffset);
					if(result != XENON_SUCCESS)
					{
						const char* const errorString = XenonGetErrorCodeString(result);

						XenonReportMessage(
							hReport,
							XENON_MESSAGE_TYPE_ERROR,
							"Failed to write exception handler offset: error=\"%s\", signature=\"%s\", offset=%" PRIu32,
							errorString,
							binding.pSignature->data,
							handler.offset
						);

						return false;
					}

					if(handler.type == XENON_VALUE_TYPE_OBJECT)
					{
						// Write the class name if this exception handler references an object type.
						if(!SerializeString(hSerializer, hReport, handler.pClassName->data, handler.pClassName->length))
						{
							return false;
						}
					}
				}
			}
		}
	}

	programHeader.bytecode.offset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	if(bytecode.size() > 0)
	{
		// Write the bytecode.
		result = XenonSerializerWriteBuffer(hSerializer, bytecode.size(), bytecode.data());
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to write program bytecode buffer: error=\"%s\"",
				errorString
			);

			return false;
		}
	}

	const size_t fileEndPosition = XenonSerializerGetStreamPosition(hSerializer);

	// Move back to the version header so we can write the real data for it.
	result = XenonSerializerSetStreamPosition(hSerializer, versionHeaderPosition);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to move serializer position to the start of the file version header: error=\"%s\", position=%" PRIuPTR,
			errorString,
			versionHeaderPosition
		);

		return false;
	}

	// Write the real version header now.
	result = writeVersionHeader();

	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write program version header data (2nd pass): error=\"%s\"",
			errorString
		);

		return false;
	}

	result = XenonSerializerSetStreamPosition(hSerializer, fileEndPosition);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to move serializer position to the end of the file stream: error=\"%s\", position=%" PRIuPTR,
			errorString,
			fileEndPosition
		);

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonProgramWriter::LookupFunction(
	XenonProgramWriterHandle hWriter,
	const char* const functionSignature,
	XenonFunctionData** const ppOutFunction
)
{
	if(!hWriter || !functionSignature || functionSignature[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonString* const pSignature = XenonString::Create(functionSignature);
	if(!pSignature)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Find the desired function by checking what we currently have mapped.
	auto kv = hWriter->functions.find(pSignature);
	if(kv == hWriter->functions.end())
	{
		XenonString::Release(pSignature);
		return XENON_ERROR_KEY_DOES_NOT_EXIST;
	}

	// The string object for the function signature is no longer needed.
	XenonString::Release(pSignature);

	(*ppOutFunction) = &kv->second;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, int8_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapInt8.find(value);
	if(kv != hWriter->indexMapInt8.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_INT8;
	container.as.int8 = value;

	hWriter->indexMapInt8.emplace(value, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, int16_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapInt16.find(value);
	if(kv != hWriter->indexMapInt16.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_INT16;
	container.as.int16 = value;

	hWriter->indexMapInt16.emplace(value, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, int32_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapInt32.find(value);
	if(kv != hWriter->indexMapInt32.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_INT32;
	container.as.int32 = value;

	hWriter->indexMapInt32.emplace(value, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, int64_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapInt64.find(value);
	if(kv != hWriter->indexMapInt64.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_INT64;
	container.as.int64 = value;

	hWriter->indexMapInt64.emplace(value, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, uint8_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapUint8.find(value);
	if(kv != hWriter->indexMapUint8.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_UINT8;
	container.as.uint8 = value;

	hWriter->indexMapUint8.emplace(value, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, uint16_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapUint16.find(value);
	if(kv != hWriter->indexMapUint16.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_UINT16;
	container.as.uint16 = value;

	hWriter->indexMapUint16.emplace(value, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, uint32_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapUint32.find(value);
	if(kv != hWriter->indexMapUint32.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_UINT32;
	container.as.uint32 = value;

	hWriter->indexMapUint32.emplace(value, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, uint64_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapUint64.find(value);
	if(kv != hWriter->indexMapUint64.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_UINT64;
	container.as.uint64 = value;

	hWriter->indexMapUint64.emplace(value, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, float value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	union
	{
		float original;
		uint32_t bits;
	} temp;

	temp.original = value;

	auto kv = hWriter->indexMapFloat32.find(temp.bits);
	if(kv != hWriter->indexMapFloat32.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_FLOAT32;
	container.as.float32 = value;

	hWriter->indexMapFloat32.emplace(temp.bits, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, double value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	union
	{
		double original;
		uint64_t bits;
	} temp;

	temp.original = value;

	auto kv = hWriter->indexMapFloat64.find(temp.bits);
	if(kv != hWriter->indexMapFloat64.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_FLOAT64;
	container.as.float64 = value;

	hWriter->indexMapFloat64.emplace(temp.bits, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, XenonString* const pValue)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);
	assert(pValue != nullptr);

	auto kv = hWriter->indexMapString.find(pValue);
	if(kv != hWriter->indexMapString.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_STRING;
	container.as.pString = pValue;

	XenonString::AddRef(pValue);

	hWriter->indexMapString.emplace(pValue, output);
	hWriter->constants.push_back(container);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonProgramWriter::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonProgramWriter::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
