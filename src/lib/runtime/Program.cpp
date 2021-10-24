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

#include "Program.hpp"
#include "BuiltInDecl.hpp"
#include "Vm.hpp"

#include "program-loader/CommonLoader.hpp"
#include "program-loader/VersionLoader0001.hpp"

#include "../base/Mutex.hpp"

#include <assert.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

static bool ProgramLoad(
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

	// The first few bytes should always be read natively. We'll switch
	// the endianness after reading the 'isBigEndian' flag.
	result = XenonSerializerSetEndianness(hSerializer, XENON_ENDIAN_ORDER_NATIVE);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);
		const char* const endianString = XenonGetEndianModeString(XENON_ENDIAN_ORDER_NATIVE);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error setting endian mode on the program file serializer: error=\"%s\", endiaMode=\"%s\"",
			errorString,
			endianString
		);

		return false;
	}

	XenonProgramCommonHeader commonHeader;
	memset(&commonHeader, 0, sizeof(commonHeader));

	// Read the magic number.
	result = XenonSerializerReadBuffer(hSerializer, sizeof(commonHeader.magicNumber), commonHeader.magicNumber);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file magic number: error=\"%s\"",
			errorString
		);

		return false;
	}

	// Validate the magic number.
	if(!XenonProgramCommonLoader::CheckMagicNumber(commonHeader))
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Invalid program file magic number: magicNumber=\"%c%c%c%c%c\", expected=\"XPRG_\"",
			commonHeader.magicNumber[0],
			commonHeader.magicNumber[1],
			commonHeader.magicNumber[2],
			commonHeader.magicNumber[3],
			commonHeader.magicNumber[4]
		);

		return false;
	}

	// Read the 'isBigEndian' flag.
	result = XenonSerializerReadUint8(hSerializer, &commonHeader.bigEndianFlag);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file big endian flag: error=\"%s\"",
			errorString
		);

		return false;
	}

	XenonReportMessage(
		hReport,
		XENON_MESSAGE_TYPE_VERBOSE,
		"Detected program file endianness: bigEndian=%d",
		commonHeader.bigEndianFlag
	);

	// Save the endianness value to the output program since we'll need
	// that when dispatching bytecode data.
	pOutProgram->endianness = (commonHeader.bigEndianFlag > 0)
		? XENON_ENDIAN_ORDER_BIG
		: XENON_ENDIAN_ORDER_LITTLE;

	// Now that we know the endianness, we can set it on the serializer.
	result = XenonSerializerSetEndianness(hSerializer, pOutProgram->endianness);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);
		const char* const endianString = XenonGetEndianModeString(pOutProgram->endianness);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error setting endian mode on the program file serializer: error=\"%s\", endianMode=\"%s\"",
			errorString,
			endianString
		);

		return false;
	}

	// Read the program file version.
	result = XenonSerializerReadUint16(hSerializer, &commonHeader.fileVersion);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Error reading program file version number: error=\"%s\"",
			errorString
		);
		return false;
	}

	XenonReportMessage(
		hReport,
		XENON_MESSAGE_TYPE_VERBOSE,
		"Detected program file version: version=0x%04X",
		commonHeader.fileVersion
	);

	// Continue loading for the correct file version.
	switch(commonHeader.fileVersion)
	{
		case XENON_PROGRAM_VERSION_0001:
			return XenonProgramVersion0001::Load(pOutProgram, hVm, hSerializer);

		default:
			break;
	}

	// Getting here means the file we're loading has file version we don't support.
	XenonReportMessage(
		hReport,
		XENON_MESSAGE_TYPE_ERROR,
		"No loader available for program file version: version=0x%04X",
		commonHeader.fileVersion
	);

	return false;
}

//----------------------------------------------------------------------------------------------------------------------

XenonProgramHandle XenonProgram::Create(XenonVmHandle hVm, XenonString* const pProgramName, const char* const filePath)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pProgramName != nullptr);
	assert(filePath != nullptr);

	XenonReportHandle hReport = &hVm->report;
	XenonSerializerHandle hSerializer = XENON_SERIALIZER_HANDLE_NULL;

	XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Loading program \"%s\" from file: \"%s\"", pProgramName->data, filePath);

	int result;

	// Create the serializer for stream reading.
	result = XenonSerializerCreate(&hSerializer, XENON_SERIALIZER_MODE_READER);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		// Failed to the create the serializer.
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to create program serializer: error=\"%s\"",
			errorString
		);

		return nullptr;
	}

	// Open the file into a serializer.
	result = XenonSerializerLoadStreamFromFile(hSerializer, filePath);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		// The file could not be opened.
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to load program stream: error=\"%s\"",
			errorString
		);

		XenonSerializerDispose(&hSerializer);
		return nullptr;
	}

	XenonString::AddRef(pProgramName);

	XenonProgram* pOutput = new XenonProgram();
	assert(pOutput != XENON_PROGRAM_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->pName = pProgramName;

	// Initialize the program data.
	XenonValue::HandleArray::Initialize(pOutput->constants);
	XenonByteHelper::Array::Initialize(pOutput->code);

	// This load block needs to lock the garbage collector since we'll be manipulating
	// the VM and adding garbage collected resources.
	{
		XenonScopedWriteLock gcLock(hVm->gcRwLock);

		// Attempt to load the program.
		if(ProgramLoad(pOutput, hVm, hSerializer))
		{
			// Map the program to the VM.
			XENON_MAP_FUNC_INSERT(hVm->programs, pProgramName, pOutput);
		}
		else
		{
			// The program failed to load, so any stale load data needs to be freed.
			prv_freeLoadedData(pOutput, hVm);

			XenonString::Release(pProgramName);
			XenonProgram::Dispose(pOutput);

			pOutput = nullptr;
		}
	}

	result = XenonSerializerDispose(&hSerializer);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		// Failed disposing of the serializer.
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_WARNING,
			"Failed to dispose of program serializer: error=\"%s\"",
			errorString
		);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonProgramHandle XenonProgram::Create(
	XenonVmHandle hVm,
	XenonString* const pProgramName,
	const void* const pFileData,
	const size_t fileLength
)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pProgramName != nullptr);
	assert(pFileData != nullptr);
	assert(fileLength > 0);

	XenonReportHandle hReport = &hVm->report;
	XenonSerializerHandle hSerializer = XENON_SERIALIZER_HANDLE_NULL;

	XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Loading program \"%s\" from data buffer", pProgramName->data);

	int result;

	// Create the serializer for stream reading.
	result = XenonSerializerCreate(&hSerializer, XENON_SERIALIZER_MODE_READER);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		// Failed to the create the serializer.
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to create program serializer: error=\"%s\"",
			errorString
		);

		return nullptr;
	}

	// Open the file stream into a serializer.
	result = XenonSerializerLoadStreamFromBuffer(hSerializer, pFileData, fileLength);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		// The stream could not be opened.
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"Failed to load program stream: error=\"%s\"",
			errorString
		);

		return nullptr;
	}

	XenonString::AddRef(pProgramName);

	XenonProgram* pOutput = new XenonProgram();
	assert(pOutput != XENON_PROGRAM_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->pName = pProgramName;

	// Initialize the program data.
	XenonValue::HandleArray::Initialize(pOutput->constants);
	XenonByteHelper::Array::Initialize(pOutput->code);

	// This load block needs to lock the garbage collector since we'll be manipulating
	// the VM and adding garbage collected resources.
	{
		XenonScopedWriteLock gcLock(hVm->gcRwLock);

		// Attempt to load the program.
		if(ProgramLoad(pOutput, hVm, hSerializer))
		{
			// Map the program to the VM.
			XENON_MAP_FUNC_INSERT(hVm->programs, pProgramName, pOutput);
		}
		else
		{
			// The program failed to load, so any stale load data needs to be freed.
			prv_freeLoadedData(pOutput, hVm);

			XenonString::Release(pProgramName);
			XenonProgram::Dispose(pOutput);

			pOutput = nullptr;
		}
	}

	result = XenonSerializerDispose(&hSerializer);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		// Failed disposing of the serializer.
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_WARNING,
			"Failed to dispose of program serializer: error=\"%s\"",
			errorString
		);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonProgram::Dispose(XenonProgramHandle hProgram)
{
	assert(hProgram != XENON_PROGRAM_HANDLE_NULL);

	// Dispose of all program dependencies.
	for(auto& kv : hProgram->dependencies)
	{
		XenonString::Release(XENON_MAP_ITER_KEY(kv));
	}

	// Release the function signature keys.
	for(auto& kv : hProgram->functions)
	{
		XenonString::Release(XENON_MAP_ITER_KEY(kv));
	}

	// Release the global variable name keys.
	for(auto& kv : hProgram->globals)
	{
		XenonString::Release(XENON_MAP_ITER_KEY(kv));
	}

	// Clean up the data structures.
	XenonValue::HandleArray::Dispose(hProgram->constants);
	XenonByteHelper::Array::Dispose(hProgram->code);

	XenonString::Release(hProgram->pName);

	delete hProgram;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonProgram::GetConstant(XenonProgramHandle hProgram, const uint32_t index, int* const pOutResult)
{
	assert(hProgram != XENON_PROGRAM_HANDLE_NULL);
	assert(pOutResult != nullptr);

	if(index >= hProgram->constants.count)
	{
		(*pOutResult) = XENON_ERROR_INDEX_OUT_OF_RANGE;
		return XENON_VALUE_HANDLE_NULL;
	}

	(*pOutResult) = XENON_SUCCESS;

	return hProgram->constants.pData[index];
}

//----------------------------------------------------------------------------------------------------------------------

void XenonProgram::prv_freeLoadedData(XenonProgramHandle hProgram, XenonVmHandle hVm)
{
	assert(hProgram != XENON_PROGRAM_HANDLE_NULL);
	assert(hVm != XENON_VM_HANDLE_NULL);

	// Remove the loaded globals from the VM.
	for(auto& kv : hProgram->globals)
	{
		XenonString* const pKey = XENON_MAP_ITER_KEY(kv);

		XENON_MAP_FUNC_REMOVE(hVm->globals, pKey);

		XenonString::Release(pKey);
	}

	// Remove the loaded functions from the VM.
	for(auto& kv : hProgram->functions)
	{
		XenonString* const pKey = XENON_MAP_ITER_KEY(kv);
		XenonFunctionHandle hFunction = XENON_MAP_FUNC_GET(hVm->functions, pKey);

		XENON_MAP_FUNC_REMOVE(hVm->functions, pKey);

		XenonString::Release(pKey);
		XenonFunction::Dispose(hFunction);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonProgram::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonProgram::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
