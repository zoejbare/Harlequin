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
#include "Vm.hpp"

#include "program-loader/CommonLoader.hpp"
#include "program-loader/VersionLoader0001.hpp"

#include <assert.h>
#include <stdio.h>

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
	result = XenonSerializerSetEndianness(hSerializer, XENON_ENDIAN_MODE_NATIVE);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);
		const char* const endianString = XenonGetEndianModeString(XENON_ENDIAN_MODE_NATIVE);

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
	pOutProgram->endianness = (commonHeader.bigEndianFlag > 0) ? XENON_ENDIAN_MODE_BIG : XENON_ENDIAN_MODE_LITTLE;

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

XenonProgramHandle XenonProgram::Create(XenonVmHandle hVm, const char* const filePath)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(filePath != nullptr);

	XenonReportHandle hReport = &hVm->report;
	XenonSerializerHandle hSerializer = XENON_SERIALIZER_HANDLE_NULL;

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

		return nullptr;
	}

	XenonProgram* pOutput = new XenonProgram();

	// Initialize the program data.
	XenonValue::HandleArray::Initialize(pOutput->constants);
	XenonByteHelper::Array::Initialize(pOutput->code);

	XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Loading program from file: file=\"%s\"", filePath);

	// Attempt to load the program.
	if(!ProgramLoad(pOutput, hVm, hSerializer))
	{
		// Failed to load the program from the file.
		delete pOutput;
		pOutput = nullptr;
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
	const char* const programName,
	const void* const pFileData,
	const size_t fileLength
)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(programName != nullptr);
	assert(pFileData != nullptr);
	assert(fileLength > 0);

	XenonReportHandle hReport = &hVm->report;
	XenonSerializerHandle hSerializer = XENON_SERIALIZER_HANDLE_NULL;

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

	XenonProgram* pOutput = new XenonProgram();

	// Initialize the program data.
	XenonValue::HandleArray::Initialize(pOutput->constants);
	XenonByteHelper::Array::Initialize(pOutput->code);

	XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Loading program: name=\"%s\"", programName);

	// Attempt to load the program.
	if(!ProgramLoad(pOutput, hVm, hSerializer))
	{
		// Failed to load the program from the file.
		delete pOutput;
		pOutput = nullptr;
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

	// Dispose of all program functions.
	for(auto& kv : hProgram->functions)
	{
		XenonString::Dispose(kv.key);
		XenonFunction::Dispose(kv.value);
	}

	// Dispose of all program dependencies.
	for(auto& kv : hProgram->dependencies)
	{
		XenonString::Dispose(kv.key);

		// The mapped value doesn't contain anything, so it doesn't need to be released.
	}

	// Dispose of all program globals.
	for(auto& kv : hProgram->globals)
	{
		XenonString::Dispose(kv.key);
		XenonValueDispose(kv.value);
	}

	// Dispose of all program constants.
	for(size_t i = 0; i < hProgram->constants.count; ++i)
	{
		XenonValueDispose(hProgram->constants.pData[i]);
	}

	// Clean up the data structures.
	XenonValue::HandleArray::Dispose(hProgram->constants);
	XenonByteHelper::Array::Dispose(hProgram->code);

	delete hProgram;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonProgram::GetConstant(XenonProgramHandle hProgram, const uint32_t index)
{
	assert(hProgram != nullptr);

	return (index < hProgram->constants.count)
		? XenonValueReference(hProgram->constants.pData[index])
		: XENON_VALUE_HANDLE_NULL;
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
