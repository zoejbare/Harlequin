//
// Copyright (c) 2023, Zoe J. Bare
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

#include "Project.hpp"
#include "Compiler.hpp"

#include "../common/Array.hpp"

#include <rapidxml_ns.hpp>

#include <assert.h>
#include <inttypes.h>

#include <vector>

//----------------------------------------------------------------------------------------------------------------------

HqProjectHandle HqProject::Load(HqCompilerHandle hCompiler, const void* const pProjectFileData, const size_t projectFileSize)
{
	using namespace rapidxml_ns;

	assert(hCompiler != HQ_COMPILER_HANDLE_NULL);
	assert(pProjectFileData != HQ_SERIALIZER_HANDLE_NULL);
	assert(projectFileSize > 0);

	HqProject* const pOutput = new HqProject();
	assert(pOutput != nullptr);

	pOutput->hCompiler = hCompiler;

	typedef HqArray<char> XmlBuffer;

	// Rapidxml expects there to be a null terminator at the end of XML files, so we'll need
	// to copy the file data into a buffer, then manually add the null terminator to the end.
	XmlBuffer fileData;
	XmlBuffer::Initialize(fileData);
	XmlBuffer::Reserve(fileData, projectFileSize + 1);

	// Copy the file data into the buffer.
	memcpy(fileData.pData, pProjectFileData, projectFileSize);
	fileData.pData[projectFileSize] = '\0';

	HqReportHandle hReport = &pOutput->hCompiler->report;

	xml_document<> doc;

	try
	{
		// Parse the XML file in-place at the necessary offset.
		doc.parse<0>(fileData.pData);
	}
	catch(const parse_error& error)
	{
		// An error occurred while parsing.
		HqReportMessage(
			hReport, 
			HQ_MESSAGE_TYPE_ERROR, 
			"Failed to parse Harlequin project file: %s", 
			error.what()
		);
		delete pOutput;
	}

	// Dispose of the temporary file data buffer.
	XmlBuffer::Dispose(fileData);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqProject::Dispose(HqProjectHandle hProject)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);
	delete hProject;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqProject::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqProject::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
