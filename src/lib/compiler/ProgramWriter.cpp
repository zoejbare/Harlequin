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

#include "../common/program-format/CommonHeader.hpp"
#include "../common/program-format/VersionHeader0001.hpp"

#include <assert.h>
#include <inttypes.h>

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

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonProgramWriter::Dispose(XenonProgramWriterHandle hProgramWriter)
{
	assert(hProgramWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	// Dispose of all dependency names.
	for(auto& kv : hProgramWriter->dependencies)
	{
		XenonString::Release(kv.key);
	}

	// Dispose of all program globals.
	for(auto& kv : hProgramWriter->globals)
	{
		XenonString::Release(kv.key);
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
		XenonString::Release(funcKv.key);

		for(auto& valueKv : funcKv.value.locals)
		{
			XenonString::Release(valueKv.key);
		}
	}

	// Dispose of all object types.
	for(auto& typeKv : hProgramWriter->objectTypes)
	{
		XenonString::Release(typeKv.value.pTypeName);

		// Dispose of all members belonging to the current object type.
		for(auto& memberKv : typeKv.value.members)
		{
			XenonString::Release(memberKv.key);
		}
	}

	delete hProgramWriter;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonProgramWriter::Serialize(
	XenonProgramWriterHandle hProgramWriter,
	XenonCompilerHandle hCompiler,
	XenonSerializerHandle hSerializer,
	int endianness
)
{
	assert(hProgramWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);
	assert(hCompiler != XENON_COMPILER_HANDLE_NULL);
	assert(hSerializer != XENON_SERIALIZER_HANDLE_NULL);

	XenonReportHandle hReport = &hCompiler->report;

	XenonProgramCommonHeader commonHeader = {};
	XenonProgramHeader0001 versionHeader = {};

	commonHeader.magicNumber[0] = 'X';
	commonHeader.magicNumber[1] = 'P';
	commonHeader.magicNumber[2] = 'R';
	commonHeader.magicNumber[3] = 'G';
	commonHeader.magicNumber[4] = '_';
	commonHeader.fileVersion = XENON_PROGRAM_VERSION_0001;

	// Set the big endian flag.
	switch(endianness)
	{
		case XENON_ENDIAN_MODE_LITTLE:
			commonHeader.bigEndianFlag = 0;
			break;

		case XENON_ENDIAN_MODE_BIG:
			commonHeader.bigEndianFlag = 1;
			break;

		default:
#ifdef XENON_CPU_ENDIAN_LITTLE
			commonHeader.bigEndianFlag = 0;
#else
			commonHeader.bigEndianFlag = 1;
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

		uint32_t finalOffset;

	};

	std::deque<FunctionBinding> functionBindings;
	std::vector<uint8_t> bytecode;

	// Get the function bindings and bytecode ready to be written out.
	{
		size_t bytecodeLength = 0;

		// Build the binding data for each function while calculating total length of the bytecode.
		for(auto& kv : hProgramWriter->functions)
		{
			FunctionBinding binding;

			binding.pFunction = &kv.value;
			binding.pSignature = kv.key;
			binding.finalOffset = uint32_t(bytecodeLength);

			// Add the length of the function's bytecode, but also align the length to add padding between each function.
			bytecodeLength += binding.pFunction->bytecode.size();
			bytecodeLength = getAlignedSize(bytecodeLength);

			functionBindings.push_back(binding);
		}

		// Allocate space for the entire block of bytecode for the program.
		bytecode.resize(bytecodeLength);

		uint8_t* const pProgramBytecode = bytecode.data();
		memset(pProgramBytecode, 0, bytecode.size());

		// Fill out the full program bytecode from each function's individual bytecode.
		for(const FunctionBinding& binding : functionBindings)
		{
			memcpy(
				pProgramBytecode + binding.finalOffset,
				binding.pFunction->bytecode.data(),
				binding.pFunction->bytecode.size()
			);
		}
	}

	versionHeader.dependencyTableLength = uint32_t(hProgramWriter->dependencies.Size());
	versionHeader.objectTableLength = uint32_t(hProgramWriter->objectTypes.Size());
	versionHeader.constantTableLength = uint32_t(hProgramWriter->constants.size());
	versionHeader.globalTableLength = uint32_t(hProgramWriter->globals.Size());
	versionHeader.functionTableLength = uint32_t(functionBindings.size());
	versionHeader.bytecodeLength = uint32_t(bytecode.size());

	// Cache the current endianness of the serializer so we can restore it after writing the file.
	const int cachedEndianness = XenonSerializerGetEndianness(hSerializer);
	int result = 0;

	// Update the serializer to use the input endianness.
	result = XenonSerializerSetEndianness(hSerializer, endianness);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);
		const char* const endianString = XenonGetEndianModeString(endianness);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to set serializer endian mode: error=\"%s\", endian=\"%s\"",
			errorString,
			endianString
		);

		return false;
	}

	// Write the common header.
	result |= XenonSerializerWriteUint8(hSerializer, commonHeader.magicNumber[0]);
	result |= XenonSerializerWriteUint8(hSerializer, commonHeader.magicNumber[1]);
	result |= XenonSerializerWriteUint8(hSerializer, commonHeader.magicNumber[2]);
	result |= XenonSerializerWriteUint8(hSerializer, commonHeader.magicNumber[3]);
	result |= XenonSerializerWriteUint8(hSerializer, commonHeader.magicNumber[4]);
	result |= XenonSerializerWriteUint8(hSerializer, commonHeader.bigEndianFlag);
	result |= XenonSerializerWriteUint16(hSerializer, commonHeader.fileVersion);

	if(result != XENON_SUCCESS)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write program common header"
		);
		return false;
	}

	auto writeVersionHeader = [&hSerializer, &versionHeader]() -> int
	{
		int result = XENON_SUCCESS;

		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.dependencyTableOffset);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.dependencyTableLength);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.objectTableOffset);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.objectTableLength);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.constantTableOffset);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.constantTableLength);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.globalTableOffset);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.globalTableLength);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.functionTableOffset);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.functionTableLength);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.bytecodeOffset);
		result |= XenonSerializerWriteUint32(hSerializer, versionHeader.bytecodeLength);

		return result;
	};

	const size_t versionHeaderPosition = XenonSerializerGetStreamPosition(hSerializer);

	// Write temporary data for the version header.
	// We'll come back to fill it out at the end.
	result = writeVersionHeader();

	if(result != XENON_SUCCESS)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write program version header data"
		);
		return false;
	}

	versionHeader.dependencyTableOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the dependency table.
	for(auto& kv : hProgramWriter->dependencies)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing dependency: name=\"%s\"", kv.key->data);

		if(!SerializeString(hSerializer, hReport, kv.key->data, kv.key->length))
		{
			return false;
		}
	}

	versionHeader.objectTableOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the object type schemas.
	for(auto& typeKv : hProgramWriter->objectTypes)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing object type: name=\"%s\"", typeKv.key->data);

		if(!SerializeString(hSerializer, hReport, typeKv.key->data, typeKv.key->length))
		{
			return false;
		}

		for(size_t memberIndex = 0; memberIndex < typeKv.value.orderedMemberNames.size(); ++memberIndex)
		{
			XenonString* const pMemberName = typeKv.value.orderedMemberNames[memberIndex];
			const uint8_t memberValueType = uint8_t(typeKv.value.members.Get(pMemberName));

			XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, " - Serializing object member: name=\"%s\", type=%" PRIu8, pMemberName->data, memberValueType);

			// Write the member name string.
			if(!SerializeString(hSerializer, hReport, pMemberName->data, pMemberName->length))
			{
				return false;
			}

			// Write the member value type.
			result = XenonSerializerWriteUint8(hSerializer, uint8_t(memberValueType));
			if(result != XENON_SUCCESS)
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to serialize object member type: objectType=\"%s\", memberName=\"%s\", memberType=%" PRIu8,
					typeKv.key->data,
					pMemberName->data,
					memberValueType
				);

				return false;
			}
		}
	}

	versionHeader.constantTableOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the constant table.
	for(size_t index = 0; index < hProgramWriter->constants.size(); ++index)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing constant: index=%" PRIuPTR, index);

		if(!SerializeValue(hSerializer, hProgramWriter->constants[index], hReport))
		{
			return false;
		}
	}

	versionHeader.globalTableOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the global variable table.
	for(auto& kv : hProgramWriter->globals)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing global variable: name=\"%s\"", kv.key->data);

		// First, write the string key of the global.
		if(!SerializeString(hSerializer, hReport, kv.key->data, kv.key->length))
		{
			return false;
		}

		// Write the global's constant index.
		result = XenonSerializerWriteUint32(hSerializer, kv.value);
		if(result != XENON_SUCCESS)
		{
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize global variable value index: name=\"%s\", index=%" PRIu32,
				kv.key->data,
				kv.value
			);

			return false;
		}
	}

	versionHeader.functionTableOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

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
				"Serializing script function: signature=\"%s\", numParams=%" PRIu16 ", numReturnValues=%" PRIu16 ", offset=%" PRIu32,
				binding.pSignature->data,
				binding.pFunction->numParameters,
				binding.pFunction->numReturnValues,
				binding.finalOffset
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
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function 'isNative' flag: signature=\"%s\", native=%s",
				binding.pSignature->data,
				binding.pFunction->isNative ? "true" : "false"
			);
		}

		// Write the function's parameter count into the program file.
		result = XenonSerializerWriteUint16(hSerializer, binding.pFunction->numParameters);
		if(result != XENON_SUCCESS)
		{
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function parameter count: signature=\"%s\", count=%" PRIu16,
				binding.pSignature->data,
				binding.pFunction->numParameters
			);
		}

		// Write the function's return value count into the program file.
		result = XenonSerializerWriteUint16(hSerializer, binding.pFunction->numReturnValues);
		if(result != XENON_SUCCESS)
		{
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function return value count: signature=\"%s\", count=%" PRIu16,
				binding.pSignature->data,
				binding.pFunction->numReturnValues
			);
		}

		if(!binding.pFunction->isNative)
		{
			// Write the function's offset into the program file.
			result = XenonSerializerWriteUint32(hSerializer, binding.finalOffset);
			if(result != XENON_SUCCESS)
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to serialize function offset: signature=\"%s\", offset=%" PRIu32,
					binding.pSignature->data,
					binding.finalOffset
				);
			}

			// Write the function's local variable count.
			result = XenonSerializerWriteUint32(hSerializer, uint32_t(binding.pFunction->locals.Size()));
			if(result != XENON_SUCCESS)
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to serialize function local variable count: signature=\"%s\", count=%" PRIu32,
					binding.pSignature->data,
					uint32_t(binding.pFunction->locals.Size())
				);
			}

			// Write the function's local variable table.
			for(auto& kv : binding.pFunction->locals)
			{
				XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, " - Serializing local variable: name=\"%s\"", kv.key->data);

				// First, write the string key of the local.
				if(!SerializeString(hSerializer, hReport, kv.key->data, kv.key->length))
				{
					return false;
				}

				// Write the local's constant index.
				result = XenonSerializerWriteUint32(hSerializer, kv.value);
				if(result != XENON_SUCCESS)
				{
					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to serialize local variable value index: name=\"%s\", index=%" PRIu32,
						kv.key->data,
						kv.value
					);

					return false;
				}
			}
		}
	}

	versionHeader.bytecodeOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

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
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write program version header data"
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

	// Restore the old endianness.
	result = XenonSerializerSetEndianness(hSerializer, cachedEndianness);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);
		const char* const endianString = XenonGetEndianModeString(cachedEndianness);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to restore original serializer endian mode: error=\"%s\", endian=\"%s\"",
			errorString,
			endianString
		);

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonProgramWriter::AddConstant(XenonProgramWriterHandle hWriter, int8_t value)
{
	assert(hWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	auto kv = hWriter->indexMapInt8.find(value);
	if(kv != hWriter->indexMapInt8.end())
	{
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_INT8;
	container.as.int8 = value;

	hWriter->indexMapInt8.Insert(value, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_INT16;
	container.as.int16 = value;

	hWriter->indexMapInt16.Insert(value, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_INT32;
	container.as.int32 = value;

	hWriter->indexMapInt32.Insert(value, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_INT64;
	container.as.int64 = value;

	hWriter->indexMapInt64.Insert(value, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_UINT8;
	container.as.uint8 = value;

	hWriter->indexMapUint8.Insert(value, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_UINT16;
	container.as.uint16 = value;

	hWriter->indexMapUint16.Insert(value, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_UINT32;
	container.as.uint32 = value;

	hWriter->indexMapUint32.Insert(value, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_UINT64;
	container.as.uint64 = value;

	hWriter->indexMapUint64.Insert(value, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_FLOAT32;
	container.as.float32 = value;

	hWriter->indexMapFloat32.Insert(temp.bits, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_FLOAT64;
	container.as.float64 = value;

	hWriter->indexMapFloat64.Insert(temp.bits, output);
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
		return kv->value;
	}

	const uint32_t output = uint32_t(hWriter->constants.size());

	ValueContainer container;
	container.type = XENON_VALUE_TYPE_STRING;
	container.as.pString = pValue;

	XenonString::AddRef(pValue);

	hWriter->indexMapString.Insert(pValue, output);
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
