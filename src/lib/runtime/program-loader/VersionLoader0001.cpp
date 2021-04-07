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

#include "../Value.hpp"

#include <assert.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------

static bool ReadHeader(
	XenonProgramHeader0001& outHeader,
	XenonString* const pProgramName,
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
			"Error reading program file dependency table offset: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file dependency table length: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file constant table offset: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file constant table length: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file global table offset: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file global table length: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file function table offset: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file function table length: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file bytecode offset: program=\"%s\", error=\"%s\"",
			pProgramName->data,
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
			"Error reading program file bytecode length: program=\"%s\", error=\"%s\"",
			pProgramName->data,
			errorString
		);

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

static bool ValidateHeader(
	const XenonProgramHeader0001& header,
	XenonString* const pProgramName,
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
			"Invalid program file constant table offset: program=\"%s\", offset=%" PRIu32 ", expected=%" PRIu32,
			pProgramName->data,
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
			"Invalid program file global table offset: program=\"%s\", offset=%" PRIu32 ", expected=%" PRIu32,
			pProgramName->data,
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
			"Invalid program file bytecode offset: program=\"%s\", offset=%" PRIu32 ", expected=%" PRIu32,
			pProgramName->data,
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
	XenonProgram::LoadData& outLoadData,
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
	if(!ReadHeader(header, pOutProgram->pName, hSerializer, hReport))
	{
		// Failed to read the version header.
		return false;
	}

	const uint32_t headerEndPosition = uint32_t(XenonSerializerGetStreamPosition(hSerializer));

	if(!ValidateHeader(header, pOutProgram->pName, hReport, headerEndPosition))
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
				"Error setting program file stream position to the offset of the dependency table: program=\"%s\", error=\"%s\", offset=%" PRIu32,
				pOutProgram->pName->data,
				errorString,
				header.dependencyTableOffset
			);

			return false;
		}

		// Initialize the dependency table.
		pOutProgram->dependencies.Reserve(header.dependencyTableLength);

		// Iterate for each dependency.
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
				"Error setting program file stream position to the offset of the constant table: program=\"%s\", error=\"%s\", offset=%" PRIu32,
				pOutProgram->pName->data,
				errorString,
				header.constantTableOffset
			);

			return false;
		}

		// Make space in the constant table.
		XenonValue::HandleArray::Reserve(pOutProgram->constants, header.constantTableLength);
		pOutProgram->constants.count = header.constantTableLength;

		// Iterate for each constant.
		for(uint32_t index = 0; index < header.constantTableLength; ++index)
		{
			XenonValueHandle hValue = XenonProgramCommonLoader::ReadValue(hSerializer, hReport);
			if(!hValue)
			{
				return false;
			}

			pOutProgram->constants.pData[index] = hValue;
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
				"Error setting program file stream position to the offset of the global variable table: program=\"%s\", error=\"%s\", offset=%" PRIu32,
				pOutProgram->pName->data,
				errorString,
				header.globalTableOffset
			);

			return false;
		}

		// Initialize the global table.
		pOutProgram->globals.Reserve(header.globalTableLength);

		// Iterate for each global variable.
		for(uint32_t globalIndex = 0; globalIndex < header.globalTableLength; ++globalIndex)
		{
			// Read the name of the global variable.
			XenonString* const pVarName = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
			if(!pVarName)
			{
				return false;
			}

			if(hVm->globals.Contains(pVarName))
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Global variable conflict: program=\"%s\", variableName=\"%s\"",
					pOutProgram->pName->data,
					pVarName->data
				);

				XenonString::Release(pVarName);

				return false;
			}

			// Read the global variable value index.
			uint32_t constantIndex = 0;
			result = XenonSerializerReadUint32(hSerializer, &constantIndex);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read global variable value index: program=\"%s\", variableName=\"%s\", error=\"%s\"",
					pOutProgram->pName->data,
					pVarName->data,
					errorString
				);

				XenonString::Release(pVarName);

				return false;
			}

			// Get the value from the constant table.
			XenonValueHandle hValue;
			if(size_t(constantIndex) < pOutProgram->constants.count)
			{
				hValue = XenonValueReference(pOutProgram->constants.pData[constantIndex]);
			}
			else
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_WARNING,
					"Global variable points to invalid constant index: program=\"%s\", variableName=\"%s\", index=%" PRIu32,
					pOutProgram->pName->data,
					pVarName->data,
					constantIndex
				);

				hValue = XenonValueCreateNull();
			}

			XenonString::AddRef(pVarName);

			// Map the global variable.
			pOutProgram->globals.Insert(pVarName, false);
			outLoadData.globals.Insert(pVarName, hValue);
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
				"Error setting program file stream position to the offset of the global variable table: program=\"%s\", error=\"%s\", offset=%" PRIu32,
				pOutProgram->pName->data,
				errorString,
				header.globalTableOffset
			);

			return false;
		}

		// Iterate for each function.
		for(uint32_t funcIndex = 0; funcIndex < header.functionTableLength; ++funcIndex)
		{
			XenonFunctionHandle hFunction = XenonFunction::Create(pOutProgram);
			if(!hFunction)
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to create function handle: program=\"%s\"",
					pOutProgram->pName->data
				);

				return false;
			}

			// Read the function signature.
			hFunction->pSignature = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
			if(!hFunction->pSignature)
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function signature: program=\"%s\"",
					pOutProgram->pName->data
				);

				XenonFunction::Dispose(hFunction);

				return false;
			}

			// Read the function offset.
			result = XenonSerializerReadBool(hSerializer, &hFunction->isNative);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function 'isNative' flag: program=\"%s\", function=\"%s\", error=\"%s\"",
					pOutProgram->pName->data,
					hFunction->pSignature->data,
					errorString
				);

				XenonFunction::Dispose(hFunction);

				return false;
			}

			// Read the function parameter count.
			result = XenonSerializerReadUint16(hSerializer, &hFunction->numParameters);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function parameter count: program=\"%s\", function=\"%s\", error=\"%s\"",
					pOutProgram->pName->data,
					hFunction->pSignature->data,
					errorString
				);

				XenonFunction::Dispose(hFunction);

				return false;
			}

			// Read the function return value count.
			result = XenonSerializerReadUint16(hSerializer, &hFunction->numReturnValues);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function return value count: program=\"%s\", function=\"%s\", error=\"%s\"",
					pOutProgram->pName->data,
					hFunction->pSignature->data,
					errorString
				);

				XenonFunction::Dispose(hFunction);

				return false;
			}

			if(!hFunction->isNative)
			{
				// Read the function offset.
				result = XenonSerializerReadUint32(hSerializer, &hFunction->offset);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to read function bytecode offset: program=\"%s\", function=\"%s\", error=\"%s\"",
						pOutProgram->pName->data,
						hFunction->pSignature->data,
						errorString
					);

					XenonFunction::Dispose(hFunction);

					return false;
				}

				// Read the function local variable count.
				uint32_t numLocalVariables = 0;
				result = XenonSerializerReadUint32(hSerializer, &numLocalVariables);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to read function local variable count: program=\"%s\", function=\"%s\", error=\"%s\"",
						pOutProgram->pName->data,
						hFunction->pSignature->data,
						errorString
					);

					XenonFunction::Dispose(hFunction);

					return false;
				}

				if(numLocalVariables > 0)
				{
					hFunction->locals.Reserve(numLocalVariables);

					// Iterate for each local variable.
					for(uint32_t localIndex = 0; localIndex < numLocalVariables; ++localIndex)
					{
						// Read the name of the global variable.
						XenonString* const pVarName = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
						if(!pVarName)
						{
							return false;
						}

						if(hFunction->locals.Contains(pVarName))
						{
							XenonReportMessage(
								hReport,
								XENON_MESSAGE_TYPE_ERROR,
								"Local variable conflict: program=\"%s\", function=\"%s\", variableName=\"%s\"",
								pOutProgram->pName->data,
								hFunction->pSignature->data,
								pVarName->data
							);

							XenonString::Release(pVarName);

							return false;
						}

						// Read the local variable value index.
						uint32_t constantIndex = 0;
						result = XenonSerializerReadUint32(hSerializer, &constantIndex);
						if(result != XENON_SUCCESS)
						{
							const char* const errorString = XenonGetErrorCodeString(result);

							XenonReportMessage(
								hReport,
								XENON_MESSAGE_TYPE_ERROR,
								"Failed to read local variable value index: program=\"%s\", function=\"%s\", variableName=\"%s\", error=\"%s\"",
								pOutProgram->pName->data,
								hFunction->pSignature->data,
								pVarName->data,
								errorString
							);

							XenonString::Release(pVarName);

							return false;
						}

						// Get the value from the constant table.
						XenonValueHandle hValue;
						if(size_t(constantIndex) < pOutProgram->constants.count)
						{
							hValue = XenonValueReference(pOutProgram->constants.pData[constantIndex]);
						}
						else
						{
							XenonReportMessage(
								hReport,
								XENON_MESSAGE_TYPE_WARNING,
								"Local variable points to invalid constant index: program=\"%s\", function=\"%s\", variableName=\"%s\", index=%" PRIu32,
								pOutProgram->pName->data,
								hFunction->pSignature->data,
								pVarName->data,
								constantIndex
							);

							hValue = XenonValueCreateNull();
						}

						// Map the local variable to the function.
						hFunction->locals.Insert(pVarName, hValue);
					}
				}
			}

			if(!hVm->functions.Contains(hFunction->pSignature))
			{
				XenonString::AddRef(hFunction->pSignature);

				// Map the function to the signature.
				pOutProgram->functions.Insert(hFunction->pSignature, false);
				outLoadData.functions.Insert(hFunction->pSignature, hFunction);
			}
			else
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Function signature conflict: program=\"%s\", function=\"%s\"",
					pOutProgram->pName->data,
					hFunction->pSignature->data
				);

				XenonFunction::Dispose(hFunction);
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
				"Error setting program file stream position to the offset of the program bytecode: program=\"%s\", error=\"%s\", offset=%" PRIu32,
				pOutProgram->pName->data,
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
