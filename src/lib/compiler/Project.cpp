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

#include <rapidxml_ns.hpp>

#include <assert.h>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------

HqProjectHandle HqProject::Create(HqCompilerHandle hCompiler)
{
	HqProject* const pOutput = new HqProject();
	assert(pOutput != nullptr);

	pOutput->hCompiler = hCompiler;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqProject::Dispose(HqProjectHandle hProject)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);
	delete hProject;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProject::Load(HqProjectHandle hProject, HqSerializerHandle hSerializer)
{
	using namespace rapidxml_ns;

	assert(hProject != HQ_PROJECT_HANDLE_NULL);
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	HqReportHandle hReport = &hProject->hCompiler->report;

	// We can read the XML file directly from the serializer stream, so we need the stream pointer and
	// the positional offset of the serializer so we can parse from the correct starting location.
	char* const streamData = reinterpret_cast<char*>(HqSerializerGetRawStreamPointer(hSerializer));
	const size_t streamOffset = HqSerializerGetStreamPosition(hSerializer);

	xml_document<> doc;

	try
	{
		// Parse the XML file in-place at the necessary offset.
		doc.parse<0>(streamData + streamOffset);
	}
	catch(const parse_error& error)
	{
		// An error occurred while parsing.
		HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Failed to parse Harlequin project file: %s\n\t%s", error.what(), error.where<char>());
		return HQ_ERROR_FAILED_TO_OPEN_FILE;
	}

	return HQ_SUCCESS;
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
