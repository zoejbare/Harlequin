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

#include "../shared/Value.hpp"

#include "../shared/program-format/CommonHeader.hpp"
#include "../shared/program-format/VersionHeader0001.hpp"

#include <assert.h>

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
	XenonValueHandle hValue,
	XenonReportHandle hReport
)
{
	assert(hSerializer != XENON_SERIALIZER_HANDLE_NULL);
	assert(hValue != XENON_VALUE_HANDLE_NULL);
	assert(hReport != XENON_REPORT_HANDLE_NULL);

	int result = 0;

	result = XenonSerializerWriteUint8(hSerializer, uint8_t(hValue->type));
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to write value type: error=\"%s\", type=%d",
			errorString,
			hValue->type
		);

		return false;
	}

	switch(hValue->type)
	{
		case XENON_VALUE_TYPE_INT8:
			result = XenonSerializerWriteInt8(hSerializer, hValue->as.int8);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as int8: error=\"%s\", data=%d",
					errorString,
					hValue->as.int8
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_INT16:
			result = XenonSerializerWriteInt16(hSerializer, hValue->as.int16);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as int16: error=\"%s\", data=%d",
					errorString,
					hValue->as.int16
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_INT32:
			result = XenonSerializerWriteInt32(hSerializer, hValue->as.int32);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as int32: error=\"%s\", data=%d",
					errorString,
					hValue->as.int32
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_INT64:
			result = XenonSerializerWriteInt64(hSerializer, hValue->as.int64);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as int64: error=\"%s\", data=%d",
					errorString,
					hValue->as.int64
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_UINT8:
			result = XenonSerializerWriteUint8(hSerializer, hValue->as.uint8);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as uint8: error=\"%s\", data=%u",
					errorString,
					hValue->as.uint8
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_UINT16:
			result = XenonSerializerWriteUint16(hSerializer, hValue->as.uint16);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as uint16: error=\"%s\", data=%u",
					errorString,
					hValue->as.uint16
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_UINT32:
			result = XenonSerializerWriteUint32(hSerializer, hValue->as.uint32);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as uint32: error=\"%s\", data=%u",
					errorString,
					hValue->as.uint32
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_UINT64:
			result = XenonSerializerWriteUint64(hSerializer, hValue->as.uint64);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as uint64: error=\"%s\", data=%u",
					errorString,
					hValue->as.uint64
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_FLOAT32:
			result = XenonSerializerWriteFloat32(hSerializer, hValue->as.float32);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as float: error=\"%s\", data=%f",
					errorString,
					hValue->as.float32
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_FLOAT64:
			result = XenonSerializerWriteFloat64(hSerializer, hValue->as.float64);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to write value data as double: error=\"%s\", data=%f",
					errorString,
					hValue->as.float64
				);

				return false;
			}
			break;

		case XENON_VALUE_TYPE_NULL:
			break;

		case XENON_VALUE_TYPE_STRING:
			// Write out the string data.
			if(!SerializeString(hSerializer, hReport, hValue->as.pString->data, hValue->as.pString->length))
			{
				return false;
			}
			break;

		case XENON_VALUE_TYPE_OBJECT:
			// TODO: Implement support for script objects.
			assert(false);
			break;

		default:
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Cannot serialize unknown value type: type=%d",
				hValue->type
			);
			break;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

XenonProgramWriterHandle XenonProgramWriter::Create()
{
	XenonProgramWriter* const pOutput = new XenonProgramWriter();

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonProgramWriter::Dispose(XenonProgramWriterHandle hProgramWriter)
{
	assert(hProgramWriter != XENON_PROGRAM_WRITER_HANDLE_NULL);

	// Dispose of all dependency names.
	for(XenonString* const pDependencyName : hProgramWriter->dependencies)
	{
		XenonString::Dispose(pDependencyName);
	}

	// Dispose of all program globals.
	for(auto& kv : hProgramWriter->globals)
	{
		XenonString::Dispose(kv.first);
	}

	// Dispose of all program constants.
	for(XenonValueHandle& value : hProgramWriter->constants)
	{
		XenonValueDispose(value);
	}

	// Dispose of all function signatures.
	for(auto& funcKv : hProgramWriter->functions)
	{
		XenonString::Dispose(funcKv.first);

		for(auto& valueKv : funcKv.second.locals)
		{
			XenonString::Dispose(valueKv.key);
			XenonValueDispose(valueKv.value);
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
		XenonString* pSignature;

		uint32_t offset;
		uint16_t numParameters;
		uint16_t numReturnValues;
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

			binding.pSignature = kv.first;
			binding.offset = uint32_t(bytecodeLength);
			binding.numParameters = kv.second.numParameters;
			binding.numReturnValues = kv.second.numReturnValues;

			// Add the length of the function's bytecode, but also align the length to add padding between each function.
			bytecodeLength += kv.second.bytecode.size();
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
			const auto kv = hProgramWriter->functions.find(binding.pSignature);
			assert(kv != hProgramWriter->functions.end());

			memcpy(pProgramBytecode + binding.offset, kv->second.bytecode.data(), kv->second.bytecode.size());
		}
	}

	versionHeader.dependencyTableLength = uint32_t(hProgramWriter->dependencies.size());
	versionHeader.constantTableLength = uint32_t(hProgramWriter->constants.size());
	versionHeader.globalTableLength = uint32_t(hProgramWriter->globals.size());
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
	for(XenonString* const pDependencyName : hProgramWriter->dependencies)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing dependency: name=\"%s\"", pDependencyName->data);

		if(!SerializeString(hSerializer, hReport, pDependencyName->data, pDependencyName->length))
		{
			return false;
		}
	}

	versionHeader.constantTableOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the constant table.
	for(size_t index = 0; index < hProgramWriter->constants.size(); ++index)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing constant: index=%d", index);

		if(!SerializeValue(hSerializer, hProgramWriter->constants[index], hReport))
		{
			return false;
		}
	}

	versionHeader.globalTableOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the global table.
	for(auto& kv : hProgramWriter->globals)
	{
		XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Serializing global variable: name=\"%s\"", kv.first->data);

		// First, write the string key of the global.
		if(!SerializeString(hSerializer, hReport, kv.first->data, kv.first->length))
		{
			return false;
		}

		// Get the constant this global is mapped to.
		XenonValueHandle hValue = hProgramWriter->constants[kv.second];

		// Next, write the value of the global.
		if(!SerializeValue(hSerializer, hValue, hReport))
		{
			return false;
		}
	}

	versionHeader.functionTableOffset = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	// Write the function table.
	for(const FunctionBinding& binding : functionBindings)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_VERBOSE,
			"Serializing function: signature=\"%s\", offset=%u",
			binding.pSignature->data,
			binding.offset
		);

		// Write the function signature.
		if(!SerializeString(hSerializer, hReport, binding.pSignature->data, binding.pSignature->length))
		{
			return false;
		}

		// Write the function's offset into the program file.
		result = XenonSerializerWriteUint32(hSerializer, binding.offset);
		if(result != XENON_SUCCESS)
		{
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function offset: signature=\"%s\", offset=%u",
				binding.pSignature->data,
				binding.offset
			);
		}

		// Write the function's parameter count into the program file.
		result = XenonSerializerWriteUint16(hSerializer, binding.numParameters);
		if(result != XENON_SUCCESS)
		{
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function parameter count: signature=\"%s\", count=%u",
				binding.pSignature->data,
				binding.numParameters
			);
		}

		// Write the function's return value count into the program file.
		result = XenonSerializerWriteUint16(hSerializer, binding.numReturnValues);
		if(result != XENON_SUCCESS)
		{
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Failed to serialize function return value count: signature=\"%s\", count=%u",
				binding.pSignature->data,
				binding.numReturnValues
			);
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
			"Failed to move serializer position to the start of the file version header: error=\"%s\", position=%zu",
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
			"Failed to move serializer position to the end of the file stream: error=\"%s\", position=%zu",
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
