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

#include "Module.hpp"

#include "BuiltInDecl.hpp"
#include "Vm.hpp"

#include "../base/ModuleLoader.hpp"
#include "../base/Mutex.hpp"

#include "../common/OpCodeEnum.hpp"

#include <assert.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

HqModuleHandle HqModule::Create(HqVmHandle hVm, HqString* const pModuleName, const char* const filePath)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pModuleName != nullptr);
	assert(filePath != nullptr);

	HqReportHandle hReport = &hVm->report;
	HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Loading module \"%s\" from file: \"%s\"", pModuleName->data, filePath);

	HqModule* pOutput = nullptr;

	// Attempt to load the module data.
	HqModuleLoader loader;
	if(HqModuleLoader::Load(loader, hReport, filePath, HqModuleLoader::NO_FLAGS))
	{
		pOutput = new HqModule();
		assert(pOutput != nullptr);

		// Add the loaded module data to the runtime.
		if(!prv_init(hVm, hReport, pOutput, loader, pModuleName))
		{
			// The module data failed verification, so we abandon the load.
			Dispose(pOutput);
			pOutput = nullptr;
		}
	}

	HqModuleLoader::Dispose(loader);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqModuleHandle HqModule::Create(
	HqVmHandle hVm,
	HqString* const pModuleName,
	const void* const pFileData,
	const size_t fileLength
)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pModuleName != nullptr);
	assert(pFileData != nullptr);
	assert(fileLength > 0);

	HqReportHandle hReport = &hVm->report;
	HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Loading module \"%s\" from data buffer", pModuleName->data);

	HqModule* pOutput = nullptr;

	// Attempt to load the module data.
	HqModuleLoader loader;
	if(HqModuleLoader::Load(loader, hReport, pFileData, fileLength, HqModuleLoader::NO_FLAGS))
	{
		pOutput = new HqModule();
		assert(pOutput != nullptr);

		// Add the loaded module data to the runtime.
		if(!prv_init(hVm, hReport, pOutput, loader, pModuleName))
		{
			// The module data failed verification, so we abandon the load.
			Dispose(pOutput);
			pOutput = nullptr;
		}
	}

	HqModuleLoader::Dispose(loader);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqModule::Dispose(HqModuleHandle hModule)
{
	assert(hModule != HQ_MODULE_HANDLE_NULL);

	// Release all strings values.
	{
		for(size_t i = 0; i < hModule->strings.count; ++i)
		{
			HqString::Release(hModule->strings.pData[i]);
		}

		StringArray::Dispose(hModule->strings);
	}

	// Dispose of all module dependencies.
	{
		HqValue::StringToBoolMap::Iterator iter;
		while(HqValue::StringToBoolMap::IterateNext(hModule->dependencies, iter))
		{
			HqString::Release(iter.pData->key);
		}

		HqValue::StringToBoolMap::Dispose(hModule->dependencies);
	}

	// Release the global variable names.
	{
		HqValue::StringToBoolMap::Iterator iter;
		while(HqValue::StringToBoolMap::IterateNext(hModule->globals, iter))
		{
			HqString::Release(iter.pData->key);
		}

		HqValue::StringToBoolMap::Dispose(hModule->globals);
	}

	// Release the object schema type names.
	{
		HqScriptObject::StringToPtrMap::Iterator iter;
		while(HqScriptObject::StringToPtrMap::IterateNext(hModule->objectSchemas, iter))
		{
			HqString::Release(iter.pData->key);
		}

		HqScriptObject::StringToPtrMap::Dispose(hModule->objectSchemas);
	}

	// Release the function signature keys.
	{
		HqFunction::StringToHandleMap::Iterator iter;
		while(HqFunction::StringToHandleMap::IterateNext(hModule->functions, iter))
		{
			HqString::Release(iter.pData->key);
		}

		HqFunction::StringToHandleMap::Dispose(hModule->functions);
	}

	// Clean up the bytecode.
	HqByteHelper::Array::Dispose(hModule->code);

	if(hModule->hInitFunction)
	{
		// Dispose of the initializer function.
		HqFunction::Dispose(hModule->hInitFunction);
		hModule->hInitFunction = HQ_FUNCTION_HANDLE_NULL;
	}

	HqString::Release(hModule->pName);

	if(hModule->hDll)
	{
		HqSysCloseLibrary(&hModule->hDll);
	}

	delete hModule;
}

//----------------------------------------------------------------------------------------------------------------------

HqString* HqModule::GetString(HqModuleHandle hModule, const uint32_t index, int* const pOutResult)
{
	assert(hModule != HQ_MODULE_HANDLE_NULL);
	assert(pOutResult != nullptr);

	if(index >= hModule->strings.count)
	{
		(*pOutResult) = HQ_ERROR_INDEX_OUT_OF_RANGE;
		return nullptr;
	}

	(*pOutResult) = HQ_SUCCESS;

	return hModule->strings.pData[index];
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModule::prv_init(HqVmHandle hVm, HqReportHandle hReport, HqModuleHandle hModule, HqModuleLoader& loader, HqString* const pModuleName)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hModule != HQ_MODULE_HANDLE_NULL);
	assert(pModuleName != nullptr);

	auto endianSwapBytecode = [&hVm](uint8_t* const pBytecode, const uint32_t offset)
	{
		assert(pBytecode != nullptr);

		HqDecoder decoder;
		HqDecoder::Initialize(decoder, pBytecode, offset);

		// Iterate through each instruction in the bytecode.
		for(;;)
		{
			const uint8_t opCode = HqDecoder::EndianSwapUint8(decoder);

			HqVm::EndianSwapOpCode(hVm, decoder, opCode);

			if(opCode == HQ_OP_CODE_RETURN)
			{
				// The RETURN opcode indicates the end of the function,
				// so we don't care about any bytecode after this.
				break;
			}
		}
	};

	const bool needEndianSwap = (loader.endianness != HqGetPlatformEndianness());

	hModule->hVm = hVm;
	hModule->hInitFunction = HQ_FUNCTION_HANDLE_NULL;
	hModule->hDll = HQ_DLL_HANDLE_NULL;
	hModule->pDllEntryPoint = nullptr;
	hModule->pName = pModuleName;

	// Initialize the module data maps.
	HqValue::StringToBoolMap::Allocate(hModule->dependencies);
	HqValue::StringToBoolMap::Allocate(hModule->globals);
	HqScriptObject::StringToPtrMap::Allocate(hModule->objectSchemas);
	HqFunction::StringToHandleMap::Allocate(hModule->functions);

	// Initialize the module arrays.
	StringArray::Initialize(hModule->strings);
	HqByteHelper::Array::Initialize(hModule->code);

	// This block needs to lock the garbage collector since we'll be manipulating
	// the VM and adding garbage collected resources.
	{
		HqScopedMutex vmLock(hVm->lock);
		HqScopedReadLock gcLock(hVm->gc.rwLock, hVm->isGcThreadEnabled);

		// Verify the module can initialized in the VM.
		if(!prv_verify(hVm, hReport, loader, pModuleName))
		{
			return false;
		}

		// Map the module to the VM.
		HqModule::StringToHandleMap::Insert(hVm->modules, pModuleName, hModule);
		HqString::AddRef(pModuleName);

		// Create the init function.
		hModule->hInitFunction = HqFunction::CreateInit(
			hModule, 
			uint32_t(loader.bytecode.count), 
			uint32_t(loader.initBytecode.count)
		);

		// Reserve space for the string pool.
		StringArray::Reserve(hModule->strings, loader.strings.count);

		// Set the total number of strings that will exist in the string pool.
		hModule->strings.count = loader.strings.count;

		// Add the module's strings.
		for(size_t stringIndex = 0; stringIndex < loader.strings.count; ++stringIndex)
		{
			HqString* const pString = loader.strings.pData[stringIndex];

			hModule->strings.pData[stringIndex] = pString;

			// Add a reference to the string.
			HqString::AddRef(pString);
		}

		// Add the module's dependencies.
		for(size_t depIndex = 0; depIndex < loader.dependencies.count; ++depIndex)
		{
			HqString* const pDepName = loader.dependencies.pData[depIndex];

			if(!HqValue::StringToBoolMap::Contains(hModule->dependencies, pDepName))
			{
				HqValue::StringToBoolMap::Insert(hModule->dependencies, pDepName, false);
				HqString::AddRef(pDepName);
			}
		}

		// Add the module's global variables
		for(size_t varIndex = 0; varIndex < loader.globals.count; ++varIndex)
		{
			HqString* const pVarName = loader.globals.pData[varIndex];

			if(!HqValue::StringToBoolMap::Contains(hModule->globals, pVarName))
			{
				// Add the global variable to the module.
				HqValue::StringToBoolMap::Insert(hModule->globals, pVarName, false);
				HqString::AddRef(pVarName);

				// Add the global variable to the VM.
				HqValue::StringToHandleMap::Insert(hVm->globals, pVarName, HQ_VALUE_HANDLE_NULL);
				HqString::AddRef(pVarName);
			}
		}

		// Add the module's object type schemas.
		for(size_t objTypeIndex = 0; objTypeIndex < loader.objectTypes.count; ++objTypeIndex)
		{
			const HqModuleLoader::ObjectType& type = loader.objectTypes.pData[objTypeIndex];

			HqScriptObject::MemberDefinitionMap objMemberDefs;
			HqScriptObject::MemberDefinitionMap::Allocate(objMemberDefs);

			// Map each object member.
			for(size_t memberIndex = 0; memberIndex < type.memberDefinitions.count; ++memberIndex)
			{
				const HqModuleLoader::ObjectType::MemberDefinition& inputDef = type.memberDefinitions.pData[memberIndex];
				HqScriptObject::MemberDefinition outputDef;

				outputDef.bindingIndex = uint32_t(memberIndex);
				outputDef.valueType = inputDef.type;

				HqScriptObject::MemberDefinitionMap::Insert(objMemberDefs, inputDef.pName, outputDef);
			}

			// Create the new object schema. This call will internally add a reference to the object type name
			// and each member name. This means we don't need to explicitly add references to those strings here.
			HqScriptObject* const pObjSchema = HqScriptObject::CreateSchema(type.pName, objMemberDefs);

			// Dispose of the object member map since it's no longer needed.
			HqScriptObject::MemberDefinitionMap::Dispose(objMemberDefs);

			// Add the object schema to the module.
			HqScriptObject::StringToPtrMap::Insert(hModule->objectSchemas, type.pName, pObjSchema);
			HqString::AddRef(type.pName);

			// Add the object schema to the VM.
			HqScriptObject::StringToPtrMap::Insert(hVm->objectSchemas, type.pName, pObjSchema);
			HqString::AddRef(type.pName);
		}

		// Add the module's functions.
		for(size_t funcIndex = 0; funcIndex < loader.functions.count; ++funcIndex)
		{
			const HqModuleLoader::Function& func = loader.functions.pData[funcIndex];

			HqFunctionHandle hFunc = HQ_FUNCTION_HANDLE_NULL;

			if(func.isNative)
			{
				// Create a native function handle. This will implicitly add a reference to the function signature.
				hFunc = HqFunction::CreateNative(hModule, func.pSignature, func.numInputs, func.numOutputs);
			}
			else
			{
				HqGuardedBlock::PtrArray guardedBlocks;
				HqGuardedBlock::PtrArray::Initialize(guardedBlocks);
				HqGuardedBlock::PtrArray::Reserve(guardedBlocks, func.guardedBlocks.count);

				guardedBlocks.count = func.guardedBlocks.count;

				// Construct the guarded blocks for the function.
				for(size_t blockIndex = 0; blockIndex < func.guardedBlocks.count; ++blockIndex)
				{
					const HqModuleLoader::Function::GuardedBlock& block = func.guardedBlocks.pData[blockIndex];

					HqGuardedBlock::ExceptionHandlerArray exceptionHandlers;
					HqGuardedBlock::ExceptionHandlerArray::Initialize(exceptionHandlers);
					HqGuardedBlock::ExceptionHandlerArray::Reserve(exceptionHandlers, block.exceptionHandlers.count);

					exceptionHandlers.count = block.exceptionHandlers.count;

					// Setup the exception handlers for the current guarded block.
					for(size_t handlerIndex = 0; handlerIndex < block.exceptionHandlers.count; ++handlerIndex)
					{
						const HqModuleLoader::Function::GuardedBlock::ExceptionHandler& inputHandler = block.exceptionHandlers.pData[handlerIndex];
						HqGuardedBlock::ExceptionHandler& outputHandler = exceptionHandlers.pData[handlerIndex];

						outputHandler.pClassName = inputHandler.pClassName;
						outputHandler.offset = inputHandler.offset;
						outputHandler.type = inputHandler.type;
					}

					// Create the new guarded block. This will implicitly add a reference to each exception handler class name.
					guardedBlocks.pData[blockIndex] = HqGuardedBlock::Create(exceptionHandlers, block.offset, block.length);

					HqGuardedBlock::ExceptionHandlerArray::Dispose(exceptionHandlers);
				}

				// Create the new script function. This will implicitly add a reference to
				// the signature and copy all guarded blocks to its own internal array.
				hFunc = HqFunction::CreateScript(
					hModule, 
					func.pSignature, 
					guardedBlocks, 
					func.offset, 
					func.offset + func.length, 
					func.numInputs, 
					func.numOutputs
				);

				HqGuardedBlock::PtrArray::Dispose(guardedBlocks);
			}

			// Add the function to the module.
			HqFunction::StringToHandleMap::Insert(hModule->functions, func.pSignature, hFunc);
			HqString::AddRef(func.pSignature);

			// Add the function to the VM.
			HqFunction::StringToHandleMap::Insert(hVm->functions, func.pSignature, hFunc);
			HqString::AddRef(func.pSignature);
		}
	}

	// Reserve enough space for both the general bytecode and init bytecode.
	// This is so we can concatenate them together for the runtime code.
	HqByteHelper::Array::Reserve(hModule->code, loader.bytecode.count + loader.initBytecode.count);

	// Copy all bytecode into the module.
	memcpy(hModule->code.pData, loader.bytecode.pData, loader.bytecode.count);
	memcpy(hModule->code.pData + loader.bytecode.count, loader.initBytecode.pData, loader.initBytecode.count);

	if(needEndianSwap)
	{
		uint8_t* const pBytecode = hModule->code.pData;

		// Endian swap the init function.
		endianSwapBytecode(pBytecode, hModule->hInitFunction->bytecodeOffsetStart);

		// Endian swap each non-native function.
		HqFunction::StringToHandleMap::Iterator iter;
		while(HqFunction::StringToHandleMap::IterateNext(hModule->functions, iter))
		{
			HqFunctionHandle hFunc = iter.pData->value;

			if(!hFunc->isNative)
			{
				endianSwapBytecode(pBytecode, hFunc->bytecodeOffsetStart);
			}
		}
	}

#if !defined(HQ_BUILD_STATIC_LIB)
	// Attempt to load a native DLL with the same name as this module.
	// This module will presumeably have special logic for binding any
	// native functions specific to this module into the script VM.
	{
		HqSysPlatformInfo info;
		HqSysGetInfo(&info);

		const size_t dllExtLength = strlen(info.dllExt);
		const size_t dllPathMaxLength = pModuleName->length + dllExtLength + 1;

		// If the current platform has no DLL file extension defined, we skip loading the DLL.
		if(dllExtLength > 0)
		{
			// Allocate a new path at the maximum length.
			char* const dllPath = reinterpret_cast<char*>(HqMemAlloc(dllPathMaxLength * sizeof(char)));

			// Copy the module name and the native DLL extension into the DLL path string.
			// TODO: We should probably re-evaluate how we construct the DLL file path at
			//       some point so we have more control over where they're loaded from.
			strcpy(dllPath, pModuleName->data);
			strcat(dllPath, info.dllExt);

			// Attempt to open the DLL.
			HqDllHandle hDll = HQ_DLL_HANDLE_NULL;
			const int dllResult = HqSysOpenLibrary(&hDll, dllPath);

			// Free the DLL file path now that we no longer need it.
			HqMemFree(dllPath);

			if(dllResult == HQ_SUCCESS)
			{
				HqDllEntryPoint pEntryPoint = reinterpret_cast<HqDllEntryPoint>(HqSysGetSymbol(hDll, "HqDllInit"));

				if(pEntryPoint)
				{
					hModule->hDll = hDll;
					hModule->pDllEntryPoint = pEntryPoint;
				}
				else
				{
					HqReportMessage(
						hReport, 
						HQ_MESSAGE_TYPE_WARNING, 
						"Found native DLL for module '%s', but did not contain the required entry point function", 
						pModuleName->data
					);
					HqSysCloseLibrary(&hDll);
				}
			}
		}
	}
#endif

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModule::prv_verify(HqVmHandle hVm, HqReportHandle hReport, HqModuleLoader& loader, HqString* const pModuleName)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pModuleName != nullptr);

	// Check if a module with this name has already been loaded.
	if(HqModule::StringToHandleMap::Contains(hVm->modules, pModuleName))
	{
		// If a module with this name already exists, we can assume it's the same module,
		// so to avoid spamming useless conflict errors, we bail out now.
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Module already loaded: module='%s'",
			pModuleName->data
		);
		return false;
	}

	bool success = true;

	// Check for global variable name conflicts.
	for(size_t varIndex = 0; varIndex < loader.globals.count; ++varIndex)
	{
		HqString* const pVarName = loader.globals.pData[varIndex];

		if(HqValue::StringToHandleMap::Contains(hVm->globals, pVarName))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Global variable conflict: module='%s', variableName='%s'",
				pModuleName->data,
				pVarName->data
			);
			success = false;
		}
	}

	// Check for object schema conflicts.
	for(size_t objTypeIndex = 0; objTypeIndex < loader.objectTypes.count; ++objTypeIndex)
	{
		const HqModuleLoader::ObjectType& type = loader.objectTypes.pData[objTypeIndex];

		if(HqScriptObject::StringToPtrMap::Contains(hVm->objectSchemas, type.pName))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Class type conflict: module='%s', className='%s'",
				pModuleName->data,
				type.pName->data
			);
			success = false;
		}
	}

	// Check for function conflicts.
	for(size_t funcIndex = 0; funcIndex < loader.functions.count; ++funcIndex)
	{
		const HqModuleLoader::Function& func = loader.functions.pData[funcIndex];

		if(HqFunction::StringToHandleMap::Contains(hVm->functions, func.pSignature))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Function signature conflict: module='%s', className='%s'",
				pModuleName->data,
				func.pSignature->data
			);
			success = false;
		}
	}

	return success;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqModule::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqModule::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
