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

#include "ProgramLoader.hpp"
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

HqProgramLoader::HqProgramLoader(
	HqProgramHandle hProgram,
	HqVmHandle hVm,
	HqSerializerHandle hSerializer
)
	: m_hProgram(hProgram)
	, m_hVm(hVm)
	, m_hSerializer(hSerializer)
	, m_hReport(HQ_REPORT_HANDLE_NULL)
	, m_programHeader()
	, m_strings()
	, m_objectSchemas()
	, m_globalValues()
	, m_functions()
{
	assert(m_hProgram != HQ_PROGRAM_HANDLE_NULL);
	assert(m_hVm != HQ_VM_HANDLE_NULL);
	assert(m_hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	m_hReport = &m_hVm->report;
}

//----------------------------------------------------------------------------------------------------------------------

HqProgramLoader::~HqProgramLoader()
{
	for(auto& kv : m_strings)
	{
		HqString::Release(HQ_MAP_ITER_KEY(kv));
	}

	for(auto& kv : m_objectSchemas)
	{
		HqScriptObject::Dispose(HQ_MAP_ITER_VALUE(kv));
	}

	for(auto& kv : m_globalValues)
	{
		HqValue::SetAutoMark(HQ_MAP_ITER_VALUE(kv), false);
	}

	for(auto& kv : m_functions)
	{
		HqFunction::Dispose(HQ_MAP_ITER_VALUE(kv));
	}
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::Load(
	HqProgram* const pOutProgram,
	HqVmHandle hVm,
	HqSerializerHandle hSerializer
)
{
	HqProgramLoader loader(pOutProgram, hVm, hSerializer);

	return loader.prv_loadFile();
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_loadFile()
{
	// Attempt to read the program header.
	if(!prv_readProgramHeader())
	{
		return false;
	}

	// Validated the data read into the program header.
	if(!prv_validateProgramHeader())
	{
		return false;
	}

	// Read the program dependency table.
	if(!prv_readDependencyTable())
	{
		return false;
	}

	// Read the object schemas defined in this program.
	if(!prv_readObjectTable())
	{
		return false;
	}

	// Read the program constants.
	if(!prv_readConstantTable())
	{
		return false;
	}

	// Read the program globals variables.
	if(!prv_readGlobalTable())
	{
		return false;
	}

	// Read the program functions.
	if(!prv_readFunctions())
	{
		return false;
	}

	// Read the program bytecode.
	if(!prv_readBytecode())
	{
		return false;
	}

	// Finalize the loaded program data. No load failure past this point or things are likely to fall apart!
	prv_finalize();

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

void HqProgramLoader::prv_finalize()
{
	if(m_programHeader.initFunctionLength > 0)
	{
		// Create the program's initializer function.
		m_hProgram->hInitFunction = HqFunction::CreateInit(m_hProgram, m_programHeader.initFunctionLength);
	}

	// Link the object schemas into the program and VM.
	{
		// Initialize the program's object table and reserve extra space in the VM's object table.
		HQ_MAP_FUNC_RESERVE(m_hProgram->objectSchemas, m_programHeader.objectTable.length);
		HQ_MAP_FUNC_RESERVE(m_hVm->objectSchemas, HQ_MAP_FUNC_SIZE(m_hVm->objectSchemas) + m_programHeader.objectTable.length);

		// Distribute the object schemas.
		for(auto& kv : m_objectSchemas)
		{
			HqString* const pTypeName = HQ_MAP_ITER_KEY(kv);
			HqScriptObject* const pObjectSchema = HQ_MAP_ITER_VALUE(kv);

			if(HQ_MAP_FUNC_CONTAINS(m_hVm->objectSchemas, HQ_MAP_ITER_KEY(kv)))
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_WARNING,
					"Class type conflict: program=\"%s\", className=\"%s\"",
					m_hProgram->pName->data,
					pTypeName->data
				);
				HqScriptObject::Dispose(pObjectSchema);
				continue;
			}

			// Track the name of the schema in the program.
			HqString::AddRef(pTypeName);
			HQ_MAP_FUNC_INSERT(m_hProgram->objectSchemas, pTypeName, false);

			// Track the schema itself in the VM.
			HqString::AddRef(pTypeName);
			HQ_MAP_FUNC_INSERT(m_hVm->objectSchemas, pTypeName, pObjectSchema);
		}

		HQ_MAP_FUNC_CLEAR(m_objectSchemas);
	}

	// Link the global variables to the VM
	{
		// Initialize the program's global table and reserve extra space in the VM's global table.
		HQ_MAP_FUNC_RESERVE(m_hProgram->globals, m_programHeader.globalTable.length);
		HQ_MAP_FUNC_RESERVE(m_hVm->globals, HQ_MAP_FUNC_SIZE(m_hVm->globals) + m_programHeader.globalTable.length);

		// Map the global variables.
		for(auto& kv : m_globalValues)
		{
			HqString* const pVarName = HQ_MAP_ITER_KEY(kv);
			HqValueHandle hValue = HQ_MAP_ITER_VALUE(kv);

			if(HQ_MAP_FUNC_CONTAINS(m_hVm->globals, pVarName))
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_WARNING,
					"Global variable conflict: program=\"%s\", variableName=\"%s\"",
					m_hProgram->pName->data,
					pVarName->data
				);
				HqValue::SetAutoMark(hValue, false);
				continue;
			}

			// Track the name of the global in the program.
			HqString::AddRef(pVarName);
			HQ_MAP_FUNC_INSERT(m_hProgram->globals, pVarName, false);

			// Add the global to the VM.
			HqString::AddRef(pVarName);
			HQ_MAP_FUNC_INSERT(m_hVm->globals, pVarName, hValue);
		}

		HQ_MAP_FUNC_CLEAR(m_globalValues);
	}

	// Link the functions to the VM.
	{
		// Initialize the program's function table and reserve extra space in the VM's function table.
		HQ_MAP_FUNC_RESERVE(m_hProgram->functions, m_programHeader.functionTable.length);
		HQ_MAP_FUNC_RESERVE(m_hVm->functions, HQ_MAP_FUNC_SIZE(m_hVm->functions) + m_programHeader.functionTable.length);

		for(auto& kv : m_functions)
		{
			HqString* const pSignature = HQ_MAP_ITER_KEY(kv);
			HqFunctionHandle hFunction = HQ_MAP_ITER_VALUE(kv);

			// Check if a function with this signature has already been loaded.
			if(HQ_MAP_FUNC_CONTAINS(m_hVm->functions, pSignature))
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Function signature conflict: program=\"%s\", function=\"%s\"",
					m_hProgram->pName->data,
					pSignature->data
				);
				HqFunction::Dispose(hFunction);
				continue;
			}

			// Map the function signature to output program.
			// Only the name is mapped since the function itself will live in the VM.
			HqString::AddRef(pSignature);
			HQ_MAP_FUNC_INSERT(m_hProgram->functions, pSignature, false);

			// Map the function in the load data map which will be transferred to the VM.
			HqString::AddRef(pSignature);
			HQ_MAP_FUNC_INSERT(m_hVm->functions, pSignature, hFunction);
		}

		HQ_MAP_FUNC_CLEAR(m_functions);
	}
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readProgramHeader()
{
	int result = HQ_SUCCESS;

	// Read the dependency table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.dependencyTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file dependency table offset: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the dependency table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.dependencyTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file dependency table length: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the object table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.objectTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file object table offset: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the object table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.objectTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file object table length: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the constant table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.constantTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file constant table offset: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the constant table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.constantTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file constant table length: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the global table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.globalTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file global table offset: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the global table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.globalTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file global table length: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the function table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.functionTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file function table offset: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the function table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.functionTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file function table length: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the extension table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.extensionTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file extension table offset: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the extension table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.extensionTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file extension table length: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the bytecode offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.bytecode.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file bytecode offset: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);

		return false;
	}

	// Read the bytecode length.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.bytecode.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file bytecode length: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Read the initializer function bytecode length.
	result = HqSerializerReadUint32(m_hSerializer, &m_programHeader.initFunctionLength);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading program file initializer function bytecode length: error=\"%s\", program=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data
		);
		return false;
	}

	// Get the offset that indicates the end of the file header.
	m_programHeader.headerEndPosition = uint32_t(HqSerializerGetStreamPosition(m_hSerializer));

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_validateProgramHeader()
{
	// Verify the dependency table does not start before the end of the header data.
	if(m_programHeader.dependencyTable.offset < m_programHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid program file dependency table offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hProgram->pName->data,
			m_programHeader.dependencyTable.offset,
			m_programHeader.headerEndPosition
		);
		return false;
	}

	// Verify the dependency table does not start before the end of the header data.
	if(m_programHeader.objectTable.offset < m_programHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid program file object table offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hProgram->pName->data,
			m_programHeader.objectTable.offset,
			m_programHeader.headerEndPosition
		);
		return false;
	}

	// Verify the constant table does not start before the end of the header data.
	if(m_programHeader.constantTable.offset < m_programHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid program file constant table offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hProgram->pName->data,
			m_programHeader.constantTable.offset,
			m_programHeader.headerEndPosition
		);
		return false;
	}

	// Verify the global table does not start before the end of the header data.
	if(m_programHeader.globalTable.offset < m_programHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid program file global table offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hProgram->pName->data,
			m_programHeader.globalTable.offset,
			m_programHeader.headerEndPosition
		);
		return false;
	}

	// Verify the bytecode does not start before the end of the header data.
	if(m_programHeader.bytecode.offset < m_programHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid program file bytecode offset: program=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hProgram->pName->data,
			m_programHeader.bytecode.offset,
			m_programHeader.headerEndPosition
		);
		return false;
	}

	// TODO: There's probably a lot more we could check here.

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readDependencyTable()
{
	if(m_programHeader.dependencyTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the dependency table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_programHeader.dependencyTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the dependency table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hProgram->pName->data,
				m_programHeader.dependencyTable.offset
			);
			return false;
		}

		// Initialize the dependency table.
		HQ_MAP_FUNC_RESERVE(m_hProgram->dependencies, m_programHeader.dependencyTable.length);

		// Iterate for each dependency.
		for(uint32_t index = 0; index < m_programHeader.dependencyTable.length; ++index)
		{
			// Read the name of the dependency.
			HqString* const pDependencyName = HqProgramCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pDependencyName)
			{
				return false;
			}

			prv_trackString(pDependencyName);

			// We stuff the dependency name into a map, but that's just for convenience of storing it.
			// The value it's mapped to isn't used for anything, so it can be null.
			HqString::AddRef(pDependencyName);
			HQ_MAP_FUNC_INSERT(m_hProgram->dependencies, pDependencyName, HQ_VALUE_HANDLE_NULL);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readObjectTable()
{
	if(m_programHeader.objectTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the constant table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_programHeader.objectTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the object table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hProgram->pName->data,
				m_programHeader.objectTable.offset
			);
			return false;
		}

		// Initialize the object schema table.
		HQ_MAP_FUNC_RESERVE(m_hProgram->objectSchemas, m_programHeader.objectTable.length);

		// Iterate for each object type.
		for(uint32_t objectIndex = 0; objectIndex < m_programHeader.objectTable.length; ++objectIndex)
		{
			// Read the name of the global variable.
			HqString* const pTypeName = HqProgramCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pTypeName)
			{
				return false;
			}

			prv_trackString(pTypeName);

			uint32_t memberCount;
			result = HqSerializerReadUint32(m_hSerializer, &memberCount);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Error setting program file stream position to the offset of the constant table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					m_programHeader.constantTable.offset
				);
				return false;
			}

			HqScriptObject::MemberDefinitionMap memberDefinitions;

			// Read the member definitions for this object type.
			for(uint32_t memberIndex = 0; memberIndex < memberCount; ++memberIndex)
			{
				HqString* const pMemberName = HqProgramCommonLoader::ReadString(m_hSerializer, m_hReport);
				if(!pMemberName)
				{
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Error reading object member name: error=\"%s\", program=\"%s\", objectType=\"%s\", memberIndex=\"%s\"",
						HqGetErrorCodeString(result),
						m_hProgram->pName->data,
						pTypeName->data,
						memberIndex
					);
					return false;
				}

				prv_trackString(pMemberName);

				uint8_t memberType;
				result = HqSerializerReadUint8(m_hSerializer, &memberType);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Error reading object member type: error=\"%s\", program=\"%s\", objectType=\"%s\", memberName=\"%s\"",
						HqGetErrorCodeString(result),
						m_hProgram->pName->data,
						pTypeName->data,
						pMemberName->data
					);
					return false;
				}

				HqScriptObject::MemberDefinition def;

				def.valueType = memberType;
				def.bindingIndex = memberIndex;

				// We don't need to add a reference on the member name here since
				// creating the schema will do that for us.
				HQ_MAP_FUNC_INSERT(memberDefinitions, pMemberName, def);
			}

			// Create the object schema from the type name and member definitions.
			HqScriptObject* const pObjectSchema = HqScriptObject::CreateSchema(pTypeName, memberDefinitions);

			prv_trackObjectSchema(pTypeName, pObjectSchema);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readConstantTable()
{
	if(m_programHeader.constantTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the constant table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_programHeader.constantTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the constant table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hProgram->pName->data,
				m_programHeader.constantTable.offset
			);
			return false;
		}

		// Make space in the constant table.
		HqValue::HandleArray::Reserve(m_hProgram->constants, m_programHeader.constantTable.length);
		m_hProgram->constants.count = m_programHeader.constantTable.length;

		// Iterate for each constant.
		for(uint32_t index = 0; index < m_programHeader.constantTable.length; ++index)
		{
			HqValueHandle hValue = HqProgramCommonLoader::ReadValue(m_hSerializer, m_hVm, m_hReport);
			if(!hValue)
			{
				m_hProgram->constants.count = index;
				return false;
			}

			m_hProgram->constants.pData[index] = hValue;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readGlobalTable()
{
	if(m_programHeader.globalTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the global table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_programHeader.globalTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the global variable table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hProgram->pName->data,
				m_programHeader.globalTable.offset
			);
			return false;
		}

		// Iterate for each global variable.
		for(uint32_t globalIndex = 0; globalIndex < m_programHeader.globalTable.length; ++globalIndex)
		{
			// Read the name of the global variable.
			HqString* const pVarName = HqProgramCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pVarName)
			{
				return false;
			}

			prv_trackString(pVarName);

			// Read the global variable value index.
			uint32_t constantIndex = 0;
			result = HqSerializerReadUint32(m_hSerializer, &constantIndex);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read global variable value index: error=\"%s\", program=\"%s\", variableName=\"%s\"",
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					pVarName->data
				);
				return false;
			}

			// Get the value from the constant table.
			HqValueHandle hValue;
			if(size_t(constantIndex) < m_hProgram->constants.count)
			{
				hValue = m_hProgram->constants.pData[constantIndex];
			}
			else
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_WARNING,
					"Global variable points to invalid constant index: program=\"%s\", variableName=\"%s\", index=%" PRIu32,
					m_hProgram->pName->data,
					pVarName->data,
					constantIndex
				);

				hValue = HqValueCreateNull();
			}

			prv_trackGlobalValue(pVarName, hValue);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readFunctions()
{
	if(m_programHeader.functionTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the function table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_programHeader.functionTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the global variable table: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hProgram->pName->data,
				m_programHeader.globalTable.offset
			);
			return false;
		}

		// Iterate for each function.
		for(uint32_t funcIndex = 0; funcIndex < m_programHeader.functionTable.length; ++funcIndex)
		{
			// Read the function signature.
			HqString* const pSignature = HqProgramCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pSignature)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read function signature: program=\"%s\"",
					m_hProgram->pName->data
				);
				return false;
			}

			prv_trackString(pSignature);

			// Read the flag indicating whether or not the function uses a native binding.
			bool isNativeFunction = false;
			result = HqSerializerReadBool(m_hSerializer, &isNativeFunction);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read function 'isNative' flag: error=\"%s\", program=\"%s\", function=\"%s\"",
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					pSignature->data
				);
				return false;
			}

			// Read the function parameter count.
			uint16_t numParameters = 0;
			result = HqSerializerReadUint16(m_hSerializer, &numParameters);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read function parameter count: error=\"%s\", program=\"%s\", function=\"%s\"",
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					pSignature->data
				);
				return false;
			}

			// Read the function return value count.
			uint16_t numReturnValues = 0;
			result = HqSerializerReadUint16(m_hSerializer, &numReturnValues);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read function return value count: error=\"%s\", program=\"%s\", function=\"%s\"",
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					pSignature->data
				);
				return false;
			}

			HqFunctionHandle hFunction;

			if(!isNativeFunction)
			{
				HqValue::StringToHandleMap locals;
				HqGuardedBlock::Array guardedBlocks;

				uint32_t bytecodeOffset = 0;
				uint32_t bytecodeLength = 0;

				// Read the function's bytecode offset.
				result = HqSerializerReadUint32(m_hSerializer, &bytecodeOffset);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to read function bytecode offset start: error=\"%s\", program=\"%s\", function=\"%s\"",
						HqGetErrorCodeString(result),
						m_hProgram->pName->data,
						pSignature->data
					);
					return false;
				}

				// Read the function's bytecode length.
				result = HqSerializerReadUint32(m_hSerializer, &bytecodeLength);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to read function bytecode offset end: error=\"%s\", program=\"%s\", function=\"%s\"",
						HqGetErrorCodeString(result),
						m_hProgram->pName->data,
						pSignature->data
					);
					return false;
				}

				// Read the function's local variables.
				if(!prv_readLocalVariables(pSignature, locals))
				{
					// Release all the loaded local variables.
					for(auto& kv : locals)
					{
						HqValue::SetAutoMark(HQ_MAP_ITER_VALUE(kv), false);
					}
					return false;
				}

				// Read the function's guarded blocks.
				if(!prv_readGuardedBlocks(pSignature, guardedBlocks))
				{
					for(size_t blockIndex = 0; blockIndex < guardedBlocks.count; ++blockIndex)
					{
						HqGuardedBlock::Dispose(guardedBlocks.pData[blockIndex]);
					}

					HqGuardedBlock::Array::Dispose(guardedBlocks);
					return false;
				}

				hFunction = HqFunction::CreateScript(
					m_hProgram,
					pSignature,
					locals,
					guardedBlocks,
					bytecodeOffset,
					bytecodeLength,
					numParameters,
					numReturnValues
				);
			}
			else
			{
				hFunction = HqFunction::CreateNative(
					m_hProgram,
					pSignature,
					numParameters,
					numReturnValues
				);
			}

			if(!hFunction)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to create function handle: program=\"%s\"",
					m_hProgram->pName->data
				);
				return false;
			}

			prv_trackFunction(pSignature, hFunction);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readBytecode()
{
	if(m_programHeader.bytecode.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the bytecode.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_programHeader.bytecode.offset);
		if(result != HQ_SUCCESS)
		{
			const char* const errorString = HqGetErrorCodeString(result);

			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting program file stream position to the offset of the program bytecode: error=\"%s\", program=\"%s\", offset=%" PRIu32,
				errorString,
				m_hProgram->pName->data,
				m_programHeader.bytecode.offset
			);

			return false;
		}

		// Reserve space in the program's bytecode array.
		HqByteHelper::Array::Reserve(m_hProgram->code, m_programHeader.bytecode.length);
		m_hProgram->code.count = m_programHeader.bytecode.length;

		// Get the stream pointer directly to avoid having to create a staging buffer.
		const uint8_t* const pBytecode = reinterpret_cast<const uint8_t*>(HqSerializerGetRawStreamPointer(m_hSerializer));

		// Copy the bytecode into the program.
		memcpy(m_hProgram->code.pData, pBytecode + m_programHeader.bytecode.offset, m_programHeader.bytecode.length);
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readLocalVariables(HqString* const pSignature, HqValue::StringToHandleMap& outLocals)
{
	int result = HQ_SUCCESS;

	// Read the local variable count.
	uint32_t numLocalVariables = 0;
	result = HqSerializerReadUint32(m_hSerializer, &numLocalVariables);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to read function local variable count: error=\"%s\", program=\"%s\", function=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data,
			pSignature->data
		);
		return false;
	}

	if(numLocalVariables > 0)
	{
		HQ_MAP_FUNC_RESERVE(outLocals, numLocalVariables);

		// Iterate for each local variable.
		for(uint32_t localIndex = 0; localIndex < numLocalVariables; ++localIndex)
		{
			// Read the name of the global variable.
			HqString* const pVarName = HqProgramCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pVarName)
			{
				return false;
			}

			prv_trackString(pVarName);

			if(HQ_MAP_FUNC_CONTAINS(outLocals, pVarName))
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Local variable conflict: program=\"%s\", function=\"%s\", variableName=\"%s\"",
					m_hProgram->pName->data,
					pSignature->data,
					pVarName->data
				);
				return false;
			}

			// Read the local variable value index.
			uint32_t constantIndex = 0;
			result = HqSerializerReadUint32(m_hSerializer, &constantIndex);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read local variable value index: error=\"%s\", program=\"%s\", function=\"%s\", variableName=\"%s\"",
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					pSignature->data,
					pVarName->data
				);
				return false;
			}

			// Get the value from the constant table.
			HqValueHandle hValue;
			if(size_t(constantIndex) < m_hProgram->constants.count)
			{
				hValue = m_hProgram->constants.pData[constantIndex];
			}
			else
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_WARNING,
					"Local variable points to invalid constant index: program=\"%s\", function=\"%s\", variableName=\"%s\", index=%" PRIu32,
					m_hProgram->pName->data,
					pSignature->data,
					pVarName->data,
					constantIndex
				);

				hValue = HqValueCreateNull();
			}

			// Map the local variable to the function.
			HqString::AddRef(pVarName);
			HQ_MAP_FUNC_INSERT(outLocals, pVarName, hValue);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramLoader::prv_readGuardedBlocks(HqString* const pSignature, HqGuardedBlock::Array& outBlocks)
{
	int result = HQ_SUCCESS;

	// Read the guarded block count.
	uint32_t numGuardedBlocks = 0;
	result = HqSerializerReadUint32(m_hSerializer, &numGuardedBlocks);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to read function guarded block count: error=\"%s\", program=\"%s\", function=\"%s\"",
			HqGetErrorCodeString(result),
			m_hProgram->pName->data,
			pSignature->data
		);
		return false;
	}

	// Initialize the output array of guarded blocks.
	HqGuardedBlock::Array::Initialize(outBlocks);
	HqGuardedBlock::Array::Reserve(outBlocks, numGuardedBlocks);

	if(numGuardedBlocks > 0)
	{
		for(uint32_t blockIndex = 0; blockIndex < numGuardedBlocks; ++blockIndex)
		{
			// Read the bytecode offset of the guarded block.
			uint32_t offset = 0;
			result = HqSerializerReadUint32(m_hSerializer, &offset);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read guarded block's offset: error=\"%s\", program=\"%s\", function=\"%s\", block=%" PRIu32,
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					pSignature->data,
					blockIndex
				);
				return false;
			}

			// Read the bytecode length of the guarded block.
			uint32_t length = 0;
			result = HqSerializerReadUint32(m_hSerializer, &length);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read guarded block's length: error=\"%s\", program=\"%s\", function=\"%s\", block=%" PRIu32,
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					pSignature->data,
					blockIndex
				);
				return false;
			}

			// Read the number of exception handlers that belong to this guarded block.
			uint32_t numExceptionHandlers = 0;
			result = HqSerializerReadUint32(m_hSerializer, &numExceptionHandlers);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read guarded block's exception handler count: error=\"%s\", program=\"%s\", function=\"%s\", block=%" PRIu32,
					HqGetErrorCodeString(result),
					m_hProgram->pName->data,
					pSignature->data,
					blockIndex
				);
				return false;
			}

			HqGuardedBlock* const pGuardedBlock = HqGuardedBlock::Create(offset, length, numExceptionHandlers);
			if(!pGuardedBlock)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to allocate new guarded block: program=\"%s\", function=\"%s\", block=%" PRIu32,
					m_hProgram->pName->data,
					pSignature->data,
					blockIndex
				);
				return false;
			}

			// Add the new block to the output array.
			outBlocks.pData[blockIndex] = pGuardedBlock;
			++outBlocks.count;

			// Load the exception handlers for this block.
			for(uint32_t handlerIndex = 0; handlerIndex < numExceptionHandlers; ++handlerIndex)
			{
				// Read the data type this handler will handle.
				uint8_t handledType = 0;
				result = HqSerializerReadUint8(m_hSerializer, &handledType);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to read exception handler's type: error=\"%s\", program=\"%s\", function=\"%s\", block=%" PRIu32 ", handler=%" PRIu32,
						HqGetErrorCodeString(result),
						m_hProgram->pName->data,
						pSignature->data,
						blockIndex,
						handlerIndex
					);
					return false;
				}

				// Read the bytecode offset where this handler is located.
				uint32_t handlerOffset = 0;
				result = HqSerializerReadUint32(m_hSerializer, &handlerOffset);
				if(result != HQ_SUCCESS)
				{
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to read exception handler's offset: error=\"%s\", program=\"%s\", function=\"%s\", block=%" PRIu32 ", handler=%" PRIu32,
						HqGetErrorCodeString(result),
						m_hProgram->pName->data,
						pSignature->data,
						blockIndex,
						handlerIndex
					);
					return false;
				}

				HqString* pClassName = nullptr;

				if(handledType == HQ_VALUE_TYPE_OBJECT)
				{
					// When an object type is used for the handler, read the class name that is handles.
					pClassName = HqProgramCommonLoader::ReadString(m_hSerializer, m_hReport);
					if(!pClassName)
					{
						if(result != HQ_SUCCESS)
						{
							HqReportMessage(
								m_hReport,
								HQ_MESSAGE_TYPE_ERROR,
								"Failed to read exception handler type class name: program=\"%s\", function=\"%s\", block=%" PRIu32 ", handler=%" PRIu32,
								HqGetErrorCodeString(result),
								m_hProgram->pName->data,
								pSignature->data,
								blockIndex,
								handlerIndex
							);
							return false;
						}
					}

					prv_trackString(pClassName);
				}

				HqExceptionHandler* const pExceptionHandler = HqExceptionHandler::Create(handledType, handlerOffset, pClassName);
				if(!pExceptionHandler)
				{
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to allocate new exception handler: program=\"%s\", function=\"%s\", block=%" PRIu32 ", handler=%" PRIu32,
						m_hProgram->pName->data,
						pSignature->data,
						blockIndex,
						handlerIndex
					);
					return false;
				}

				// Add the new handler to the guarded block.
				pGuardedBlock->handlers.pData[handlerIndex] = pExceptionHandler;
				++pGuardedBlock->handlers.count;
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
