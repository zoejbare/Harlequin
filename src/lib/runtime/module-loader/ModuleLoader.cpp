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

#include "ModuleLoader.hpp"
#include "CommonLoader.hpp"

#include "../Decoder.hpp"
#include "../Function.hpp"
#include "../Module.hpp"
#include "../ScriptObject.hpp"
#include "../Value.hpp"
#include "../Vm.hpp"

#include "../../common/OpCodeEnum.hpp"

#include <assert.h>
#include <inttypes.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

HqModuleLoader::HqModuleLoader(
	HqModuleHandle hModule,
	HqVmHandle hVm,
	HqSerializerHandle hSerializer
)
	: m_hModule(hModule)
	, m_hVm(hVm)
	, m_hSerializer(hSerializer)
	, m_hReport(HQ_REPORT_HANDLE_NULL)
	, m_moduleHeader()
	, m_strings()
	, m_objectSchemas()
	, m_globalValues()
	, m_functions()
{
	assert(m_hModule != HQ_MODULE_HANDLE_NULL);
	assert(m_hVm != HQ_VM_HANDLE_NULL);
	assert(m_hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	StringResources::Allocate(m_strings);
	HqScriptObject::StringToPtrMap::Allocate(m_objectSchemas);
	HqValue::StringToHandleMap::Allocate(m_globalValues);
	HqFunction::StringToHandleMap::Allocate(m_functions);

	m_hReport = &m_hVm->report;
}

//----------------------------------------------------------------------------------------------------------------------

HqModuleLoader::~HqModuleLoader()
{
	// Release all tracked strings.
	{
		StringResources::Iterator iter;
		while(StringResources::IterateNext(m_strings, iter))
		{
			HqString::Release(iter.pData->key);
		}

		StringResources::Dispose(m_strings);
	}

	// Dispose of the loaded object schema data.
	{
		HqScriptObject::StringToPtrMap::Iterator iter;
		while(HqScriptObject::StringToPtrMap::IterateNext(m_objectSchemas, iter))
		{
			HqString::Release(iter.pData->key);

			if(iter.pData->value)
			{
				HqScriptObject::Dispose(iter.pData->value);
			}
		}

		HqScriptObject::StringToPtrMap::Dispose(m_objectSchemas);
	}

	// Release all global variable names.
	{
		HqValue::StringToHandleMap::Iterator iter;
		while(HqValue::StringToHandleMap::IterateNext(m_globalValues, iter))
		{
			HqString::Release(iter.pData->key);
		}

		HqValue::StringToHandleMap::Dispose(m_globalValues);
	}

	// Dispose of the loaded function data
	{
		HqFunction::StringToHandleMap::Iterator iter;
		while(HqFunction::StringToHandleMap::IterateNext(m_functions, iter))
		{
			HqString::Release(iter.pData->key);

			if(iter.pData->value)
			{
				HqFunction::Dispose(iter.pData->value);
			}
		}

		HqFunction::StringToHandleMap::Dispose(m_functions);
	}
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::Load(
	HqModule* const pOutModule,
	HqVmHandle hVm,
	HqSerializerHandle hSerializer
)
{
	HqModuleLoader loader(pOutModule, hVm, hSerializer);

	return loader.prv_loadFile();
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_loadFile()
{
	// Attempt to read the module header.
	if(!prv_readModuleHeader())
	{
		return false;
	}

	// Validated the data read into the module header.
	if(!prv_validateModuleHeader())
	{
		return false;
	}

	// Read the module dependency table.
	if(!prv_readDependencyTable())
	{
		return false;
	}

	// Read the object schemas defined in this module.
	if(!prv_readObjectTable())
	{
		return false;
	}

	// Read the module strings.
	if(!prv_readStringTable())
	{
		return false;
	}

	// Read the module globals variables.
	if(!prv_readGlobalTable())
	{
		return false;
	}

	// Read the module functions.
	if(!prv_readFunctions())
	{
		return false;
	}

	// Read the module bytecode.
	if(!prv_readBytecode())
	{
		return false;
	}

	// Finalize the loaded module data. No load failure past this point or things are likely to fall apart!
	prv_finalize();

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

void HqModuleLoader::prv_finalize()
{
	auto endianSwapFunctionBytecode = [](HqFunctionHandle hFunc)
	{
		assert(hFunc != nullptr);
		assert(!hFunc->isNative);

		HqDecoder decoder;
		HqDecoder::Initialize(decoder, hFunc->hModule, hFunc->bytecodeOffsetStart);

		// Iterate through each instruction.
		for(;;)
		{
			const uint8_t opCode = HqDecoder::EndianSwapUint8(decoder);

			HqVm::EndianSwapOpCode(hFunc->hModule->hVm, decoder, opCode);

			if(opCode == HQ_OP_CODE_RETURN)
			{
				// The RETURN opcode indicates the end of the function.
				break;
			}
		}
	};

	const bool needEndianSwap = HqSerializerGetEndianness(m_hSerializer) != HqGetPlatformEndianness();

	if(m_moduleHeader.initFunctionLength > 0)
	{
		// Create the module's initializer function.
		m_hModule->hInitFunction = HqFunction::CreateInit(m_hModule, m_moduleHeader.initFunctionLength);

		if(needEndianSwap)
		{
			endianSwapFunctionBytecode(m_hModule->hInitFunction);
		}
	}

	// Link the object schemas into the module and VM.
	{
		// Distribute the object schemas.
		HqScriptObject::StringToPtrMap::Iterator iter;
		while(HqScriptObject::StringToPtrMap::IterateNext(m_objectSchemas, iter))
		{
			HqString* const pTypeName = iter.pData->key;
			HqScriptObject* const pObjectSchema = iter.pData->value;

			if(HqScriptObject::StringToPtrMap::Contains(m_hVm->objectSchemas, pTypeName))
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_WARNING,
					"Class type conflict: module=\"%s\", className=\"%s\"",
					m_hModule->pName->data,
					pTypeName->data
				);
				continue;
			}

			// Track the name of the schema in the module.
			HqString::AddRef(pTypeName);
			HqValue::StringToBoolMap::Insert(m_hModule->objectSchemas, pTypeName, false);

			// Track the schema itself in the VM.
			HqString::AddRef(pTypeName);
			HqScriptObject::StringToPtrMap::Insert(m_hVm->objectSchemas, pTypeName, pObjectSchema);

			// Clear the object schema so it doesn't get disposed after loading has completed.
			iter.pData->value = nullptr;
		}
	}

	// Link the global variables to the VM
	{
		// Map the global variables.
		HqValue::StringToHandleMap::Iterator iter;
		while(HqValue::StringToHandleMap::IterateNext(m_globalValues, iter))
		{
			HqString* const pVarName = iter.pData->key;
			HqValueHandle hValue = iter.pData->value;

			if(HqValue::StringToHandleMap::Contains(m_hVm->globals, pVarName))
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_WARNING,
					"Global variable conflict: module=\"%s\", variableName=\"%s\"",
					m_hModule->pName->data,
					pVarName->data
				);
				HqValue::SetAutoMark(hValue, false);
				continue;
			}

			// Track the name of the global in the module.
			HqString::AddRef(pVarName);
			HqValue::StringToBoolMap::Insert(m_hModule->globals, pVarName, false);

			// Add the global to the VM.
			HqString::AddRef(pVarName);
			HqValue::StringToHandleMap::Insert(m_hVm->globals, pVarName, hValue);
		}
	}

	// Link the functions to the VM.
	{
		HqFunction::StringToHandleMap::Iterator iter;
		while(HqFunction::StringToHandleMap::IterateNext(m_functions, iter))
		{
			HqString* const pSignature = iter.pData->key;
			HqFunctionHandle hFunction = iter.pData->value;

			// Clear the function handle in the map so it doesn't get destroyed after the script load has completed.
			iter.pData->value = HQ_FUNCTION_HANDLE_NULL;

			// Check if a function with this signature has already been loaded.
			if(HqFunction::StringToHandleMap::Contains(m_hVm->functions, pSignature))
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Function signature conflict: module=\"%s\", function=\"%s\"",
					m_hModule->pName->data,
					pSignature->data
				);
				HqFunction::Dispose(hFunction);
				continue;
			}

			// Endian swap all script function bytecode when the endianness of
			// the module file does not match the endianness of the platform.
			if(!hFunction->isNative && needEndianSwap)
			{
				endianSwapFunctionBytecode(hFunction);
			}

			// Map the function signature to output module.
			// Only the name is mapped since the function itself will live in the VM.
			HqString::AddRef(pSignature);
			HqFunction::StringToBoolMap::Insert(m_hModule->functions, pSignature, false);

			// Map the function in the load data map which will be transferred to the VM.
			HqString::AddRef(pSignature);
			HqFunction::StringToHandleMap::Insert(m_hVm->functions, pSignature, hFunction);
		}
	}

}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readModuleHeader()
{
	int result = HQ_SUCCESS;

	// Read the dependency table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.dependencyTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file dependency table offset: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the dependency table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.dependencyTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file dependency table length: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the object table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.objectTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file object table offset: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the object table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.objectTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file object table length: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the string table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.stringTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file string table offset: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the string table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.stringTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file string table length: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the global table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.globalTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file global table offset: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the global table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.globalTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file global table length: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the function table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.functionTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file function table offset: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the function table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.functionTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file function table length: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the extension table offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.extensionTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file extension table offset: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the extension table length.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.extensionTable.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file extension table length: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the bytecode offset.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.bytecode.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file bytecode offset: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);

		return false;
	}

	// Read the bytecode length.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.bytecode.length);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file bytecode length: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Read the initializer function bytecode length.
	result = HqSerializerReadUint32(m_hSerializer, &m_moduleHeader.initFunctionLength);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file initializer function bytecode length: error=\"%s\", module=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data
		);
		return false;
	}

	// Get the offset that indicates the end of the file header.
	m_moduleHeader.headerEndPosition = uint32_t(HqSerializerGetStreamPosition(m_hSerializer));

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_validateModuleHeader()
{
	// Verify the dependency table does not start before the end of the header data.
	if(m_moduleHeader.dependencyTable.offset < m_moduleHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid module file dependency table offset: module=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hModule->pName->data,
			m_moduleHeader.dependencyTable.offset,
			m_moduleHeader.headerEndPosition
		);
		return false;
	}

	// Verify the dependency table does not start before the end of the header data.
	if(m_moduleHeader.objectTable.offset < m_moduleHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid module file object table offset: module=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hModule->pName->data,
			m_moduleHeader.objectTable.offset,
			m_moduleHeader.headerEndPosition
		);
		return false;
	}

	// Verify the constant table does not start before the end of the header data.
	if(m_moduleHeader.stringTable.offset < m_moduleHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid module file string table offset: module=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hModule->pName->data,
			m_moduleHeader.stringTable.offset,
			m_moduleHeader.headerEndPosition
		);
		return false;
	}

	// Verify the global table does not start before the end of the header data.
	if(m_moduleHeader.globalTable.offset < m_moduleHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid module file global table offset: module=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hModule->pName->data,
			m_moduleHeader.globalTable.offset,
			m_moduleHeader.headerEndPosition
		);
		return false;
	}

	// Verify the bytecode does not start before the end of the header data.
	if(m_moduleHeader.bytecode.offset < m_moduleHeader.headerEndPosition)
	{
		HqReportMessage(
			m_hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid module file bytecode offset: module=\"%s\", offset=%" PRIu32 ", expectedMinimum=%" PRIu32,
			m_hModule->pName->data,
			m_moduleHeader.bytecode.offset,
			m_moduleHeader.headerEndPosition
		);
		return false;
	}

	// TODO: There's probably a lot more we could check here.

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readDependencyTable()
{
	if(m_moduleHeader.dependencyTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the dependency table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_moduleHeader.dependencyTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting module file stream position to the offset of the dependency table: error=\"%s\", module=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hModule->pName->data,
				m_moduleHeader.dependencyTable.offset
			);
			return false;
		}

		// Iterate for each dependency.
		for(uint32_t index = 0; index < m_moduleHeader.dependencyTable.length; ++index)
		{
			// Read the name of the dependency.
			HqString* const pDependencyName = HqModuleCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pDependencyName)
			{
				return false;
			}

			prv_trackString(pDependencyName);

			// Add the dependency to the module.
			HqString::AddRef(pDependencyName);
			HqValue::StringToBoolMap::Insert(m_hModule->dependencies, pDependencyName, false);

			// Release the extra string reference.
			HqString::Release(pDependencyName);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readObjectTable()
{
	if(m_moduleHeader.objectTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the constant table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_moduleHeader.objectTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting module file stream position to the offset of the object table: error=\"%s\", module=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hModule->pName->data,
				m_moduleHeader.objectTable.offset
			);
			return false;
		}

		// Iterate for each object type.
		for(uint32_t objectIndex = 0; objectIndex < m_moduleHeader.objectTable.length; ++objectIndex)
		{
			// Read the name of the global variable.
			HqString* const pTypeName = HqModuleCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pTypeName)
			{
				return false;
			}

			prv_trackString(pTypeName);

			// Release the extra string reference.
			HqString::Release(pTypeName);

			uint32_t memberCount;
			result = HqSerializerReadUint32(m_hSerializer, &memberCount);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Error setting module file stream position to the offset of the string table: error=\"%s\", module=\"%s\", offset=%" PRIu32,
					HqGetErrorCodeString(result),
					m_hModule->pName->data,
					m_moduleHeader.stringTable.offset
				);
				return false;
			}

			HqScriptObject::MemberDefinitionMap memberDefinitions;
			HqScriptObject::MemberDefinitionMap::Allocate(memberDefinitions);

			// Read the member definitions for this object type.
			for(uint32_t memberIndex = 0; memberIndex < memberCount; ++memberIndex)
			{
				HqString* const pMemberName = HqModuleCommonLoader::ReadString(m_hSerializer, m_hReport);
				if(!pMemberName)
				{
					HqScriptObject::MemberDefinitionMap::Dispose(memberDefinitions);
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Error reading object member name: error=\"%s\", module=\"%s\", objectType=\"%s\", memberIndex=\"%s\"",
						HqGetErrorCodeString(result),
						m_hModule->pName->data,
						pTypeName->data,
						memberIndex
					);
					return false;
				}

				prv_trackString(pMemberName);

				// Release the extra string reference.
				HqString::Release(pMemberName);

				uint8_t memberType;
				result = HqSerializerReadUint8(m_hSerializer, &memberType);
				if(result != HQ_SUCCESS)
				{
					HqScriptObject::MemberDefinitionMap::Dispose(memberDefinitions);
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Error reading object member type: error=\"%s\", module=\"%s\", objectType=\"%s\", memberName=\"%s\"",
						HqGetErrorCodeString(result),
						m_hModule->pName->data,
						pTypeName->data,
						pMemberName->data
					);
					return false;
				}

				HqScriptObject::MemberDefinition def;

				def.valueType = memberType;
				def.bindingIndex = memberIndex;
				
				// We don't need to add a reference on the member name here since creating the schema will do that for us.
				HqScriptObject::MemberDefinitionMap::Insert(memberDefinitions, pMemberName, def);
			}

			// Create the object schema from the type name and member definitions.
			HqScriptObject* const pObjectSchema = HqScriptObject::CreateSchema(pTypeName, memberDefinitions);

			prv_trackObjectSchema(pTypeName, pObjectSchema);

			// Dispose of the member definitions.
			{
				HqScriptObject::MemberDefinitionMap::Iterator iter;
				while(HqScriptObject::MemberDefinitionMap::IterateNext(memberDefinitions, iter))
				{
					HqString::Release(iter.pData->key);
				}

				HqScriptObject::MemberDefinitionMap::Dispose(memberDefinitions);
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readStringTable()
{
	if(m_moduleHeader.stringTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the string table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_moduleHeader.stringTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting module file stream position to the offset of the string table: error=\"%s\", module=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hModule->pName->data,
				m_moduleHeader.stringTable.offset
			);
			return false;
		}

		// Make space in the string table.
		HqModule::StringArray::Reserve(m_hModule->strings, m_moduleHeader.stringTable.length);
		m_hModule->strings.count = m_moduleHeader.stringTable.length;

		// Iterate for each string.
		for(uint32_t index = 0; index < m_moduleHeader.stringTable.length; ++index)
		{
			HqString* const pString = HqModuleCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pString)
			{
				m_hModule->strings.count = index;
				return false;
			}

			m_hModule->strings.pData[index] = pString;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readGlobalTable()
{
	if(m_moduleHeader.globalTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the global table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_moduleHeader.globalTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting module file stream position to the offset of the global variable table: error=\"%s\", module=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hModule->pName->data,
				m_moduleHeader.globalTable.offset
			);
			return false;
		}

		// Iterate for each global variable.
		for(uint32_t globalIndex = 0; globalIndex < m_moduleHeader.globalTable.length; ++globalIndex)
		{
			// Read the name of the global variable.
			HqString* const pVarName = HqModuleCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pVarName)
			{
				return false;
			}

			prv_trackString(pVarName);
			prv_trackGlobalValue(pVarName, HQ_VALUE_HANDLE_NULL);

			// Release the extra string reference.
			HqString::Release(pVarName);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readFunctions()
{
	if(m_moduleHeader.functionTable.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the function table.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_moduleHeader.functionTable.offset);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting module file stream position to the offset of the global variable table: error=\"%s\", module=\"%s\", offset=%" PRIu32,
				HqGetErrorCodeString(result),
				m_hModule->pName->data,
				m_moduleHeader.globalTable.offset
			);
			return false;
		}

		// Iterate for each function.
		for(uint32_t funcIndex = 0; funcIndex < m_moduleHeader.functionTable.length; ++funcIndex)
		{
			// Read the function signature.
			HqString* const pSignature = HqModuleCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pSignature)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read function signature: module=\"%s\"",
					m_hModule->pName->data
				);
				return false;
			}

			prv_trackString(pSignature);

			// Release the extra string reference.
			HqString::Release(pSignature);

			// Read the flag indicating whether or not the function uses a native binding.
			bool isNativeFunction = false;
			result = HqSerializerReadBool(m_hSerializer, &isNativeFunction);
			if(result != HQ_SUCCESS)
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read function 'isNative' flag: error=\"%s\", module=\"%s\", function=\"%s\"",
					HqGetErrorCodeString(result),
					m_hModule->pName->data,
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
					"Failed to read function parameter count: error=\"%s\", module=\"%s\", function=\"%s\"",
					HqGetErrorCodeString(result),
					m_hModule->pName->data,
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
					"Failed to read function return value count: error=\"%s\", module=\"%s\", function=\"%s\"",
					HqGetErrorCodeString(result),
					m_hModule->pName->data,
					pSignature->data
				);
				return false;
			}

			HqFunctionHandle hFunction;

			if(!isNativeFunction)
			{
				HqFunction::StringToBoolMap locals;
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
						"Failed to read function bytecode offset start: error=\"%s\", module=\"%s\", function=\"%s\"",
						HqGetErrorCodeString(result),
						m_hModule->pName->data,
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
						"Failed to read function bytecode offset end: error=\"%s\", module=\"%s\", function=\"%s\"",
						HqGetErrorCodeString(result),
						m_hModule->pName->data,
						pSignature->data
					);
					return false;
				}

				HqFunction::StringToBoolMap::Allocate(locals);
				auto freeFunctionLocals = [&locals]()
				{
					// Dispose of the temporary local variable map.
					{
						HqFunction::StringToBoolMap::Iterator iter;
						while(HqFunction::StringToBoolMap::IterateNext(locals, iter))
						{
							HqString::Release(iter.pData->key);
						}

						HqFunction::StringToBoolMap::Dispose(locals);
					}
				};

				// Read the function's local variables.
				if(!prv_readLocalVariables(pSignature, locals))
				{
					freeFunctionLocals();
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
					freeFunctionLocals();

					return false;
				}

				// Create a script function.
				hFunction = HqFunction::CreateScript(
					m_hModule,
					pSignature,
					locals,
					guardedBlocks,
					bytecodeOffset,
					bytecodeLength,
					numParameters,
					numReturnValues
				);

				freeFunctionLocals();
			}
			else
			{
				// Create a native function.
				hFunction = HqFunction::CreateNative(
					m_hModule,
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
					"Failed to create function handle: module=\"%s\"",
					m_hModule->pName->data
				);
				return false;
			}

			prv_trackFunction(pSignature, hFunction);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readBytecode()
{
	if(m_moduleHeader.bytecode.length > 0)
	{
		int result = HQ_SUCCESS;

		// Move the stream position to the start of the bytecode.
		result = HqSerializerSetStreamPosition(m_hSerializer, m_moduleHeader.bytecode.offset);
		if(result != HQ_SUCCESS)
		{
			const char* const errorString = HqGetErrorCodeString(result);

			HqReportMessage(
				m_hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Error setting module file stream position to the offset of the module bytecode: error=\"%s\", module=\"%s\", offset=%" PRIu32,
				errorString,
				m_hModule->pName->data,
				m_moduleHeader.bytecode.offset
			);

			return false;
		}

		// Reserve space in the module's bytecode array.
		HqByteHelper::Array::Reserve(m_hModule->code, m_moduleHeader.bytecode.length);
		m_hModule->code.count = m_moduleHeader.bytecode.length;

		// Get the stream pointer directly to avoid having to create a staging buffer.
		const uint8_t* const pBytecode = reinterpret_cast<const uint8_t*>(HqSerializerGetRawStreamPointer(m_hSerializer));

		// Copy the bytecode into the module.
		memcpy(m_hModule->code.pData, pBytecode + m_moduleHeader.bytecode.offset, m_moduleHeader.bytecode.length);
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readLocalVariables(HqString* const pSignature, HqFunction::StringToBoolMap& outLocals)
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
			"Failed to read function local variable count: error=\"%s\", module=\"%s\", function=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data,
			pSignature->data
		);
		return false;
	}

	if(numLocalVariables > 0)
	{
		// Iterate for each local variable.
		for(uint32_t localIndex = 0; localIndex < numLocalVariables; ++localIndex)
		{
			// Read the name of the global variable.
			HqString* const pVarName = HqModuleCommonLoader::ReadString(m_hSerializer, m_hReport);
			if(!pVarName)
			{
				return false;
			}

			prv_trackString(pVarName);

			// Release the extra string reference.
			HqString::Release(pVarName);

			if(HqFunction::StringToBoolMap::Contains(outLocals, pVarName))
			{
				HqReportMessage(
					m_hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Local variable conflict: module=\"%s\", function=\"%s\", variableName=\"%s\"",
					m_hModule->pName->data,
					pSignature->data,
					pVarName->data
				);
				return false;
			}

			// Map the local variable to the function.
			HqString::AddRef(pVarName);
			HqFunction::StringToBoolMap::Insert(outLocals, pVarName, false);
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::prv_readGuardedBlocks(HqString* const pSignature, HqGuardedBlock::Array& outBlocks)
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
			"Failed to read function guarded block count: error=\"%s\", module=\"%s\", function=\"%s\"",
			HqGetErrorCodeString(result),
			m_hModule->pName->data,
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
					"Failed to read guarded block's offset: error=\"%s\", module=\"%s\", function=\"%s\", block=%" PRIu32,
					HqGetErrorCodeString(result),
					m_hModule->pName->data,
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
					"Failed to read guarded block's length: error=\"%s\", module=\"%s\", function=\"%s\", block=%" PRIu32,
					HqGetErrorCodeString(result),
					m_hModule->pName->data,
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
					"Failed to read guarded block's exception handler count: error=\"%s\", module=\"%s\", function=\"%s\", block=%" PRIu32,
					HqGetErrorCodeString(result),
					m_hModule->pName->data,
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
					"Failed to allocate new guarded block: module=\"%s\", function=\"%s\", block=%" PRIu32,
					m_hModule->pName->data,
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
						"Failed to read exception handler's type: error=\"%s\", module=\"%s\", function=\"%s\", block=%" PRIu32 ", handler=%" PRIu32,
						HqGetErrorCodeString(result),
						m_hModule->pName->data,
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
						"Failed to read exception handler's offset: error=\"%s\", module=\"%s\", function=\"%s\", block=%" PRIu32 ", handler=%" PRIu32,
						HqGetErrorCodeString(result),
						m_hModule->pName->data,
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
					pClassName = HqModuleCommonLoader::ReadString(m_hSerializer, m_hReport);
					if(!pClassName)
					{
						if(result != HQ_SUCCESS)
						{
							HqReportMessage(
								m_hReport,
								HQ_MESSAGE_TYPE_ERROR,
								"Failed to read exception handler type class name: module=\"%s\", function=\"%s\", block=%" PRIu32 ", handler=%" PRIu32,
								HqGetErrorCodeString(result),
								m_hModule->pName->data,
								pSignature->data,
								blockIndex,
								handlerIndex
							);
							return false;
						}
					}

					prv_trackString(pClassName);

					// Release the extra string reference.
					HqString::Release(pClassName);
				}

				HqExceptionHandler* const pExceptionHandler = HqExceptionHandler::Create(handledType, handlerOffset, pClassName);
				if(!pExceptionHandler)
				{
					HqReportMessage(
						m_hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to allocate new exception handler: module=\"%s\", function=\"%s\", block=%" PRIu32 ", handler=%" PRIu32,
						m_hModule->pName->data,
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
