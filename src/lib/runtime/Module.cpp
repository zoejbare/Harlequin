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

#include "module-loader/CommonLoader.hpp"
#include "module-loader/ModuleLoader.hpp"

#include "../base/Mutex.hpp"

#include <assert.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

static bool LoadModule(
	HqModule* const pOutModule,
	HqVmHandle hVm,
	HqSerializerHandle hSerializer
)
{
	assert(pOutModule != nullptr);
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	HqReportHandle hReport = &hVm->report;

	int result = HQ_SUCCESS;

	// The first few bytes should always be read natively. We'll switch
	// the endianness after reading the 'isBigEndian' flag.
	result = HqSerializerSetEndianness(hSerializer, HQ_ENDIAN_ORDER_NATIVE);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);
		const char* const endianString = HqGetEndiannessString(HQ_ENDIAN_ORDER_NATIVE);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error setting endian mode on the module file serializer: error=\"%s\", endiaMode=\"%s\"",
			errorString,
			endianString
		);

		return false;
	}

	HqFileHeader fileHeader;
	memset(&fileHeader, 0, sizeof(fileHeader));

	// Read the magic number.
	result = HqSerializerReadBuffer(hSerializer, sizeof(fileHeader.magicNumber), fileHeader.magicNumber);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file magic number: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Validate the magic number.
	if(!HqModuleCommonLoader::CheckMagicNumber(fileHeader))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid module file magic number: magicNumber=\"%c%c%c%c%c\", expected=\"HQPRG\"",
			fileHeader.magicNumber[0],
			fileHeader.magicNumber[1],
			fileHeader.magicNumber[2],
			fileHeader.magicNumber[3],
			fileHeader.magicNumber[4]
		);

		return false;
	}

	// Read the reserved section of the file header.
	result = HqSerializerReadBuffer(hSerializer, sizeof(fileHeader.reserved), fileHeader.reserved);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file reserved section: error=\"%s\"",
			HqGetErrorCodeString(result)
		);
		return false;
	}

	// Read the 'isBigEndian' flag.
	result = HqSerializerReadUint8(hSerializer, &fileHeader.bigEndianFlag);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error reading module file big endian flag: error=\"%s\"",
			HqGetErrorCodeString(result)
		);
		return false;
	}

	HqReportMessage(
		hReport,
		HQ_MESSAGE_TYPE_VERBOSE,
		"Detected module file endianness: bigEndian=%d",
		fileHeader.bigEndianFlag
	);

	// Save the endianness value to the output module since we'll need
	// that when dispatching bytecode data.
	const int endianness = (fileHeader.bigEndianFlag > 0)
		? HQ_ENDIAN_ORDER_BIG
		: HQ_ENDIAN_ORDER_LITTLE;

	// Now that we know the endianness, we can set it on the serializer.
	result = HqSerializerSetEndianness(hSerializer, endianness);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Error setting endian mode on the module file serializer: error=\"%s\", endianness=\"%s\"",
			HqGetErrorCodeString(result),
			HqGetEndiannessString(endianness)
		);
		return false;
	}

	return HqModuleLoader::Load(pOutModule, hVm, hSerializer);
}

//----------------------------------------------------------------------------------------------------------------------

HqModuleHandle HqModule::Create(HqVmHandle hVm, HqString* const pModuleName, const char* const filePath)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pModuleName != nullptr);
	assert(filePath != nullptr);

	HqReportHandle hReport = &hVm->report;
	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Loading module \"%s\" from file: \"%s\"", pModuleName->data, filePath);

	int result;

	// Create the serializer for stream reading.
	result = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_READER);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		// Failed to the create the serializer.
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to create module serializer: error=\"%s\"",
			errorString
		);

		return nullptr;
	}

	// Open the file into a serializer.
	result = HqSerializerLoadStreamFromFile(hSerializer, filePath);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		// The file could not be opened.
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to load module stream: error=\"%s\"",
			errorString
		);

		HqSerializerDispose(&hSerializer);
		return nullptr;
	}

	HqString::AddRef(pModuleName);

	HqModule* pOutput = new HqModule();
	assert(pOutput != HQ_MODULE_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->hInitFunction = HQ_FUNCTION_HANDLE_NULL;
	pOutput->pName = pModuleName;

	// Initialize the module data.
	HqValue::StringToBoolMap::Allocate(pOutput->dependencies);
	HqFunction::StringToBoolMap::Allocate(pOutput->functions);
	HqValue::StringToBoolMap::Allocate(pOutput->objectSchemas);
	HqValue::StringToBoolMap::Allocate(pOutput->globals);

	StringArray::Initialize(pOutput->strings);
	HqByteHelper::Array::Initialize(pOutput->code);

	// This load block needs to lock the garbage collector since we'll be manipulating
	// the VM and adding garbage collected resources.
	{
		HqScopedMutex vmLock(hVm->lock);
		HqScopedReadLock gcLock(hVm->gc.rwLock, hVm->isGcThreadEnabled);

		// Attempt to load the module.
		if(LoadModule(pOutput, hVm, hSerializer))
		{
			// Map the module to the VM.
			HqModule::StringToHandleMap::Insert(hVm->modules, pModuleName, pOutput);
		}
		else
		{
			HqString::Release(pModuleName);
			HqModule::Dispose(pOutput);

			pOutput = nullptr;
		}
	}

	result = HqSerializerDispose(&hSerializer);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		// Failed disposing of the serializer.
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_WARNING,
			"Failed to dispose of module serializer: error=\"%s\"",
			errorString
		);
	}

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
	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Loading module \"%s\" from data buffer", pModuleName->data);

	int result;

	// Create the serializer for stream reading.
	result = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_READER);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		// Failed to the create the serializer.
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to create module serializer: error=\"%s\"",
			errorString
		);

		return nullptr;
	}

	// Open the file stream into a serializer.
	result = HqSerializerLoadStreamFromBuffer(hSerializer, pFileData, fileLength);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		// The stream could not be opened.
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to load module stream: error=\"%s\"",
			errorString
		);

		return nullptr;
	}

	HqString::AddRef(pModuleName);

	HqModule* pOutput = new HqModule();
	assert(pOutput != HQ_MODULE_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->pName = pModuleName;

	// Initialize the module data.
	HqValue::StringToBoolMap::Allocate(pOutput->dependencies);
	HqFunction::StringToBoolMap::Allocate(pOutput->functions);
	HqValue::StringToBoolMap::Allocate(pOutput->objectSchemas);
	HqValue::StringToBoolMap::Allocate(pOutput->globals);

	StringArray::Initialize(pOutput->strings);
	HqByteHelper::Array::Initialize(pOutput->code);

	// This load block needs to lock the garbage collector since we'll be manipulating
	// the VM and adding garbage collected resources.
	{
		HqScopedMutex vmLock(hVm->lock);
		HqScopedReadLock gcLock(hVm->gc.rwLock, hVm->isGcThreadEnabled);

		// Attempt to load the module.
		if(LoadModule(pOutput, hVm, hSerializer))
		{
			// Map the module to the VM.
			HqModule::StringToHandleMap::Insert(hVm->modules, pModuleName, pOutput);
		}
		else
		{
			HqString::Release(pModuleName);
			HqModule::Dispose(pOutput);

			pOutput = nullptr;
		}
	}

	result = HqSerializerDispose(&hSerializer);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		// Failed disposing of the serializer.
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_WARNING,
			"Failed to dispose of module serializer: error=\"%s\"",
			errorString
		);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqModule::Dispose(HqModuleHandle hModule)
{
	assert(hModule != HQ_MODULE_HANDLE_NULL);

	// Dispose of all module dependencies.
	{
		HqValue::StringToBoolMap::Iterator iter;
		while(HqValue::StringToBoolMap::IterateNext(hModule->dependencies, iter))
		{
			HqString::Release(iter.pData->key);
		}

		HqValue::StringToBoolMap::Dispose(hModule->dependencies);
	}

	// Release the function signature keys.
	{
		HqFunction::StringToBoolMap::Iterator iter;
		while(HqFunction::StringToBoolMap::IterateNext(hModule->functions, iter))
		{
			HqString::Release(iter.pData->key);
		}

		HqFunction::StringToBoolMap::Dispose(hModule->functions);
	}

	// Release the object schema type names.
	{
		HqValue::StringToBoolMap::Iterator iter;
		while(HqValue::StringToBoolMap::IterateNext(hModule->objectSchemas, iter))
		{
			HqString::Release(iter.pData->key);
		}

		HqValue::StringToBoolMap::Dispose(hModule->objectSchemas);
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

	// Release all strings values.
	for(size_t i = 0; i < hModule->strings.count; ++i)
	{
		HqString::Release(hModule->strings.pData[i]);
	}

	// Clean up the data structures.
	StringArray::Dispose(hModule->strings);
	HqByteHelper::Array::Dispose(hModule->code);

	if(hModule->hInitFunction)
	{
		// Dispose of the initializer function.
		HqFunction::Dispose(hModule->hInitFunction);
		hModule->hInitFunction = HQ_FUNCTION_HANDLE_NULL;
	}

	HqString::Release(hModule->pName);

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
