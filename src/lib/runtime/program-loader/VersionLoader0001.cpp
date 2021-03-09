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

#include "VersionLoader0001.hpp"
#include "CommonLoader.hpp"

#include "../Function.hpp"
#include "../Program.hpp"
#include "../Vm.hpp"

#include "../../shared/Value.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

static bool ReadHeader(
	XenonProgramHeader0001& outHeader,
	XenonSerializerHandle hSerializer,
	XenonReportHandle hReport
)
{
	int result = XENON_SUCCESS;

	// Read the dependency table offset.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.dependencyTableOffset);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file dependency table offset: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the dependency table length.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.dependencyTableLength);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file dependency table length: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the constant table offset.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.constantTableOffset);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file constant table offset: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the constant table length.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.constantTableLength);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file constant table length: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the global table offset.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.globalTableOffset);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file global table offset: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the global table length.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.globalTableLength);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file global table length: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the function table offset.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.functionTableOffset);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file function table offset: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the function table length.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.functionTableLength);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file function table length: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the bytecode offset.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.bytecodeOffset);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file bytecode offset: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Read the bytecode length.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.bytecodeLength);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file bytecode length: error=\"%s\"",
			errorString
		);

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

static bool ValidateHeader(
	const XenonProgramHeader0001& header,
	XenonReportHandle hReport,
	const uint32_t headerEndPosition
)
{
	// Verify the constant table does not start before the end of the header data.
	if(header.constantTableOffset < headerEndPosition)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Invalid program file constant table offset: offset=%u, expected=%u",
			header.constantTableOffset,
			headerEndPosition
		);
		return false;
	}

	// Verify the global table does not start before the end of the header data.
	if(header.globalTableOffset < headerEndPosition)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Invalid program file global table offset: offset=%u, expected=%u",
			header.globalTableOffset,
			headerEndPosition
		);
		return false;
	}

	// Verify the bytecode does not start before the end of the header data.
	if(header.bytecodeOffset < headerEndPosition)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Invalid program file bytecode offset: offset=%u, expected=%u",
			header.bytecodeOffset,
			headerEndPosition
		);
		return false;
	}

	// TODO: There's probably a lot more we could do here.

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonProgramVersion0001::Load(
	XenonProgram* const pOutProgram,
	XenonVmHandle hVm,
	XenonSerializerHandle hSerializer
)
{
	assert(pOutProgram != nullptr);
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(hSerializer != XENON_SERIALIZER_HANDLE_NULL);

	XenonReportHandle hReport = &hVm->report;

	int result = XENON_SUCCESS;

	XenonProgramHeader0001 header;
	if(!ReadHeader(header, hSerializer, hReport))
	{
		// Failed to read the version header.
		return false;
	}

	const uint32_t headerEndPosition = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	if(!ValidateHeader(header, hReport, headerEndPosition))
	{
		// The header data is invalid.
		return false;
	}

	if(header.dependencyTableLength > 0)
	{
		// Move the stream position to the start of the constant table.
		result = XenonSerializerSetStreamPosition(hSerializer, header.dependencyTableOffset);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the dependency table: error=\"%s\", offset=%u",
				errorString,
				header.dependencyTableOffset
			);

			return false;
		}

		// Initialize the dependency table.
		pOutProgram->dependencies.Reserve(header.dependencyTableLength);

		for(uint32_t index = 0; index < header.dependencyTableLength; ++index)
		{
			// Read the name of the dependency.
			XenonString* const pDependencyName = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
			if(!pDependencyName)
			{
				return false;
			}

			// We stuff the dependency name into a map, but that's just for convenience of storing it.
			// The value it's mapped to isn't used for anything, so it can be null.
			pOutProgram->dependencies.Insert(pDependencyName, XENON_VALUE_HANDLE_NULL);
		}
	}

	if(header.constantTableLength > 0)
	{
		// Move the stream position to the start of the constant table.
		result = XenonSerializerSetStreamPosition(hSerializer, header.constantTableOffset);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the constant table: error=\"%s\", offset=%u",
				errorString,
				header.constantTableOffset
			);

			return false;
		}

		// Make space in the constant table.
		XenonValue::HandleArray::Reserve(pOutProgram->constants, header.constantTableLength);
		pOutProgram->constants.count = header.constantTableLength;

		for(uint32_t index = 0; index < header.constantTableLength; ++index)
		{
			XenonValue* const pValue = XenonProgramCommonLoader::ReadValue(hSerializer, hReport);
			if(!pValue)
			{
				return false;
			}

			pOutProgram->constants.pData[index] = pValue;
		}
	}

	if(header.globalTableLength > 0)
	{
		// Move the stream position to the start of the global table.
		result = XenonSerializerSetStreamPosition(hSerializer, header.globalTableOffset);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the global variable table: error=\"%s\", offset=%u",
				errorString,
				header.globalTableOffset
			);

			return false;
		}

		// Initialize the global table.
		pOutProgram->globals.Reserve(header.globalTableLength);

		for(uint32_t index = 0; index < header.globalTableLength; ++index)
		{
			// Read the name of the global variable.
			XenonString* const pVarName = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
			if(!pVarName)
			{
				return false;
			}

			// Read the global variable value data.
			XenonValue* pGlobalVar = XenonProgramCommonLoader::ReadValue(hSerializer, hReport);
			if(!pGlobalVar)
			{
				XenonString::Dispose(pVarName);
				return false;
			}

			pOutProgram->globals.Insert(pVarName, pGlobalVar);
		}
	}

	if(header.functionTableLength > 0)
	{
		// Move the stream position to the start of the function table.
		result = XenonSerializerSetStreamPosition(hSerializer, header.functionTableOffset);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the global variable table: error=\"%s\", offset=%u",
				errorString,
				header.globalTableOffset
			);

			return false;
		}

		for(uint32_t index = 0; index < header.functionTableLength; ++index)
		{
			// Read the function signature.
			XenonString* const pSignature = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
			if(!pSignature)
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function signature"
				);

				return false;
			}

			// Read the function offset.
			uint32_t offset = 0;
			result = XenonSerializerReadUint32(hSerializer, &offset);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function bytecode offset: error=\"%s\", function=\"%s\"",
					errorString,
					pSignature->data
				);

				XenonString::Dispose(pSignature);

				return false;
			}

			// Read the function parameter count.
			uint16_t numParameters = 0;
			result = XenonSerializerReadUint16(hSerializer, &numParameters);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function parameter count: error=\"%s\", function=\"%s\"",
					errorString,
					pSignature->data
				);

				XenonString::Dispose(pSignature);

				return false;
			}

			// Read the function return value count.
			uint16_t numReturnValues = 0;
			result = XenonSerializerReadUint16(hSerializer, &numReturnValues);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function return value count: error=\"%s\", function=\"%s\"",
					errorString,
					pSignature->data
				);

				XenonString::Dispose(pSignature);

				return false;
			}

			if(!hVm->functions.Contains(pSignature))
			{
				XenonFunctionHandle hFunction = XenonFunction::Create(
					pOutProgram,
					pSignature,
					offset,
					numParameters,
					numReturnValues
				);
				if(!hFunction)
				{
					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to create function handle"
					);

					XenonString::Dispose(pSignature);

					return false;
				}

				// Map the function to the signature.
				hVm->functions.Insert(pSignature, hFunction);
			}
			else
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Function signature conflict: signature=\"%s\", program=\"%s\"",
					pSignature->data
				);
			}
		}
	}

	if(header.bytecodeLength > 0)
	{
		// Move the stream position to the start of the bytecode.
		result = XenonSerializerSetStreamPosition(hSerializer, header.bytecodeOffset);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the program bytecode: error=\"%s\", offset=%u",
				errorString,
				header.bytecodeOffset
			);

			return false;
		}

		// Reserve space in the program's bytecode array.
		XenonByteHelper::Array::Reserve(pOutProgram->code, header.bytecodeLength);
		pOutProgram->code.count = header.bytecodeLength;

		// Get the stream pointer directly to avoid having to create a staging buffer.
		const uint8_t* const pBytecode = reinterpret_cast<const uint8_t*>(XenonSerializerGetRawStreamPointer(hSerializer));

		// Copy the bytecode into the program.
		memcpy(pOutProgram->code.pData, pBytecode + header.bytecodeOffset, header.bytecodeLength);
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
