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
#include "../ScriptObject.hpp"
#include "../Vm.hpp"

#include "../Value.hpp"

#include <assert.h>
#include <inttypes.h>
#include <string.h>

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
			"Error reading program file dependency table offset: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file dependency table length: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
		);

		return false;
	}

	// Read the object table offset.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.objectTableOffset);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file object table offset: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
		);

		return false;
	}

	// Read the object table length.
	result = XenonSerializerReadUint32(hSerializer, &outHeader.objectTableLength);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file object table length: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file constant table offset: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file constant table length: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file global table offset: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file global table length: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file function table offset: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file function table length: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file bytecode offset: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
			"Error reading program file bytecode length: error=\"%s\", program=\"%s\"",
			errorString,
			pProgramName->data
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
	// Verify the dependency table does not start before the end of the header data.
	if(header.dependencyTableOffset < headerEndPosition)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Invalid program file dependency table offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			pProgramName->data,
			header.dependencyTableOffset,
			headerEndPosition
		);
		return false;
	}

	// Verify the dependency table does not start before the end of the header data.
	if(header.objectTableOffset < headerEndPosition)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Invalid program file object table offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			pProgramName->data,
			header.objectTableOffset,
			headerEndPosition
		);
		return false;
	}

	// Verify the constant table does not start before the end of the header data.
	if(header.constantTableOffset < headerEndPosition)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Invalid program file constant table offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
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
			"Invalid program file global table offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
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
			"Invalid program file bytecode offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
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
		// Move the stream position to the start of the dependency table.
		result = XenonSerializerSetStreamPosition(hSerializer, header.dependencyTableOffset);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the dependency table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				errorString,
				pOutProgram->pName->data,
				header.dependencyTableOffset
			);

			return false;
		}

		// Initialize the dependency table.
		XENON_MAP_FUNC_RESERVE(pOutProgram->dependencies, header.dependencyTableLength);

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
			XENON_MAP_FUNC_INSERT(pOutProgram->dependencies, pDependencyName, XENON_VALUE_HANDLE_NULL);
		}
	}

	if(header.objectTableLength > 0)
	{
		// Move the stream position to the start of the constant table.
		result = XenonSerializerSetStreamPosition(hSerializer, header.objectTableOffset);
		if(result != XENON_SUCCESS)
		{
			const char* const errorString = XenonGetErrorCodeString(result);

			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the object table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				errorString,
				pOutProgram->pName->data,
				header.objectTableOffset
			);

			return false;
		}

		// Initialize the object schema table.
		XENON_MAP_FUNC_RESERVE(pOutProgram->objectSchemas, header.objectTableLength);

		// Iterate for each object type.
		for(uint32_t objectIndex = 0; objectIndex < header.objectTableLength; ++objectIndex)
		{
			// Read the name of the global variable.
			XenonString* const pTypeName = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
			if(!pTypeName)
			{
				return false;
			}

			uint32_t memberCount;
			result = XenonSerializerReadUint32(hSerializer, &memberCount);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Error setting program file stream position to the offset of the constant table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
					errorString,
					pOutProgram->pName->data,
					header.constantTableOffset
				);

				XenonString::Release(pTypeName);
				return false;
			}

			XenonScriptObject::MemberDefinitionMap memberDefinitions;

			bool memberReadError = false;

			// Read the member definitions for this object type.
			for(uint32_t memberIndex = 0; memberIndex < memberCount; ++memberIndex)
			{
				XenonString* const pMemberName = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
				if(!pMemberName)
				{
					memberReadError = true;
					break;
				}

				uint8_t memberType;
				result = XenonSerializerReadUint8(hSerializer, &memberType);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Error reading object member type: error=\"%s\", program=\"%s\", objectType=\"%s\", memberName=\"%s\"",
						errorString,
						pOutProgram->pName->data,
						pTypeName->data,
						pMemberName->data
					);

					XenonString::Release(pMemberName);

					memberReadError = true;
					break;
				}

				XenonScriptObject::MemberDefinition def;

				def.valueType = memberType;
				def.bindingIndex = memberIndex;

				XENON_MAP_FUNC_INSERT(memberDefinitions, pMemberName, def);
			}

			if(memberReadError)
			{
				XenonString::Release(pTypeName);

				// Release all member names currently loaded for the current object type.
				for(auto& kv : memberDefinitions)
				{
					XenonString::Release(XENON_MAP_ITER_KEY(kv));
				}

				return false;
			}
			else
			{
				XenonScriptObject* const pObjectSchema = XenonScriptObject::CreateSchema(pTypeName, memberDefinitions);

				XenonString::AddRef(pTypeName);
				XENON_MAP_FUNC_INSERT(hVm->objectSchemas, pTypeName, pObjectSchema);
			}
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
				"Error setting program file stream position to the offset of the constant table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				errorString,
				pOutProgram->pName->data,
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
			XenonValueHandle hValue = XenonProgramCommonLoader::ReadValue(hSerializer, hVm, hReport);
			if(!hValue)
			{
				return false;
			}

			// Make sure all constants are set to auto-mark so they don't get garbage collected.
			XenonValue::SetAutoMark(hValue, true);

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
				"Error setting program file stream position to the offset of the global variable table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				errorString,
				pOutProgram->pName->data,
				header.globalTableOffset
			);

			return false;
		}

		// Initialize the global table.
		XENON_MAP_FUNC_RESERVE(pOutProgram->globals, header.globalTableLength);

		// Iterate for each global variable.
		for(uint32_t globalIndex = 0; globalIndex < header.globalTableLength; ++globalIndex)
		{
			// Read the name of the global variable.
			XenonString* const pVarName = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
			if(!pVarName)
			{
				return false;
			}

			if(XENON_MAP_FUNC_CONTAINS(hVm->globals, pVarName))
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
					"Failed to read global variable value index: error=\"%s\", program=\"%s\", variableName=\"%s\"",
					errorString,
					pOutProgram->pName->data,
					pVarName->data
				);

				XenonString::Release(pVarName);

				return false;
			}

			// Get the value from the constant table.
			XenonValueHandle hValue;
			if(size_t(constantIndex) < pOutProgram->constants.count)
			{
				hValue = pOutProgram->constants.pData[constantIndex];
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
			XENON_MAP_FUNC_INSERT(pOutProgram->globals, pVarName, false);
			XENON_MAP_FUNC_INSERT(hVm->globals, pVarName, hValue);
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
				"Error setting program file stream position to the offset of the global variable table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				errorString,
				pOutProgram->pName->data,
				header.globalTableOffset
			);

			return false;
		}

		auto disposeLocalVariables = [](XenonValue::StringToHandleMap& map)
		{
			for(auto& kv : map)
			{
				XenonString::Release(XENON_MAP_ITER_KEY(kv));
			}
		};

		// Iterate for each function.
		for(uint32_t funcIndex = 0; funcIndex < header.functionTableLength; ++funcIndex)
		{
			// Read the function signature.
			XenonString* const pSignature = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
			if(!pSignature)
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function signature: program=\"%s\"",
					pOutProgram->pName->data
				);
				return false;
			}

			// Read the flag indicating whether or not the function uses a native binding.
			bool isNativeFunction = false;
			result = XenonSerializerReadBool(hSerializer, &isNativeFunction);
			if(result != XENON_SUCCESS)
			{
				const char* const errorString = XenonGetErrorCodeString(result);

				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Failed to read function 'isNative' flag: error=\"%s\", program=\"%s\", function=\"%s\"",
					errorString,
					pOutProgram->pName->data,
					pSignature->data
				);

				XenonString::Release(pSignature);

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
					"Failed to read function parameter count: error=\"%s\", program=\"%s\", function=\"%s\"",
					errorString,
					pOutProgram->pName->data,
					pSignature->data
				);

				XenonString::Release(pSignature);

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
					"Failed to read function return value count: error=\"%s\", program=\"%s\", function=\"%s\"",
					errorString,
					pOutProgram->pName->data,
					pSignature->data
				);

				XenonString::Release(pSignature);

				return false;
			}

			XenonValue::StringToHandleMap locals;
			uint32_t bytecodeOffsetStart = 0;
			uint32_t bytecodeOffsetEnd = 0;

			if(!isNativeFunction)
			{
				// Read the function's bytecode offset start.
				result = XenonSerializerReadUint32(hSerializer, &bytecodeOffsetStart);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to read function bytecode offset start: error=\"%s\", program=\"%s\", function=\"%s\"",
						errorString,
						pOutProgram->pName->data,
						pSignature->data
					);

					XenonString::Release(pSignature);

					return false;
				}

				// Read the function's bytecode offset end.
				result = XenonSerializerReadUint32(hSerializer, &bytecodeOffsetEnd);
				if(result != XENON_SUCCESS)
				{
					const char* const errorString = XenonGetErrorCodeString(result);

					XenonReportMessage(
						hReport,
						XENON_MESSAGE_TYPE_ERROR,
						"Failed to read function bytecode offset end: error=\"%s\", program=\"%s\", function=\"%s\"",
						errorString,
						pOutProgram->pName->data,
						pSignature->data
					);

					XenonString::Release(pSignature);

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
						"Failed to read function local variable count: error=\"%s\", program=\"%s\", function=\"%s\"",
						errorString,
						pOutProgram->pName->data,
						pSignature->data
					);

					XenonString::Release(pSignature);

					return false;
				}

				if(numLocalVariables > 0)
				{
					XENON_MAP_FUNC_RESERVE(locals, numLocalVariables);

					bool localVarError = false;

					// Iterate for each local variable.
					for(uint32_t localIndex = 0; localIndex < numLocalVariables; ++localIndex)
					{
						// Read the name of the global variable.
						XenonString* const pVarName = XenonProgramCommonLoader::ReadString(hSerializer, hReport);
						if(!pVarName)
						{
							return false;
						}

						if(XENON_MAP_FUNC_CONTAINS(locals, pVarName))
						{
							XenonReportMessage(
								hReport,
								XENON_MESSAGE_TYPE_ERROR,
								"Local variable conflict: program=\"%s\", function=\"%s\", variableName=\"%s\"",
								pOutProgram->pName->data,
								pSignature->data,
								pVarName->data
							);

							XenonString::Release(pVarName);

							localVarError = true;
							continue;
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
								"Failed to read local variable value index: error=\"%s\", program=\"%s\", function=\"%s\", variableName=\"%s\"",
								errorString,
								pOutProgram->pName->data,
								pSignature->data,
								pVarName->data
							);

							XenonString::Release(pVarName);

							localVarError = true;

							continue;
						}

						// Get the value from the constant table.
						XenonValueHandle hValue;
						if(size_t(constantIndex) < pOutProgram->constants.count)
						{
							hValue = pOutProgram->constants.pData[constantIndex];
						}
						else
						{
							XenonReportMessage(
								hReport,
								XENON_MESSAGE_TYPE_WARNING,
								"Local variable points to invalid constant index: program=\"%s\", function=\"%s\", variableName=\"%s\", index=%" PRIu32,
								pOutProgram->pName->data,
								pSignature->data,
								pVarName->data,
								constantIndex
							);

							hValue = XenonValueCreateNull();
						}

						// Map the local variable to the function.
						XENON_MAP_FUNC_INSERT(locals, pVarName, hValue);
					}

					if(localVarError)
					{
						XenonString::Release(pSignature);
						disposeLocalVariables(locals);
						return false;
					}
				}
			}

			if(!XENON_MAP_FUNC_CONTAINS(hVm->functions, pSignature))
			{
				XenonFunctionHandle hFunction;
				if(isNativeFunction)
				{
					hFunction = XenonFunction::CreateNative(
						pOutProgram,
						pSignature,
						numParameters,
						numReturnValues
					);
				}
				else
				{
					hFunction = XenonFunction::CreateScript(
						pOutProgram,
						pSignature,
						locals,
						bytecodeOffsetStart,
						bytecodeOffsetEnd,
						numParameters,
						numReturnValues
					);
				}

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

				// Map the function signature to output program.
				// Only the name is mapped since the function itself will live in the VM.
				XenonString::AddRef(pSignature);
				XENON_MAP_FUNC_INSERT(pOutProgram->functions, pSignature, false);

				// Map the function in the load data map which will be transferred to the VM.
				XenonString::AddRef(pSignature);
				XENON_MAP_FUNC_INSERT(hVm->functions, pSignature, hFunction);
			}
			else
			{
				XenonReportMessage(
					hReport,
					XENON_MESSAGE_TYPE_ERROR,
					"Function signature conflict: program=\"%s\", function=\"%s\"",
					pOutProgram->pName->data,
					pSignature->data
				);
			}

			// The original reference to the function signature is no longer needed.
			XenonString::Release(pSignature);
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
				"Error setting program file stream position to the offset of the program bytecode: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				errorString,
				pOutProgram->pName->data,
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
