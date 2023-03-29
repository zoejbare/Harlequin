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

#include <assert.h>
#include <inttypes.h>
#include <memory.h>

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
	pOutput->pOutput = nullptr;

	typedef std::vector<char> XmlBuffer;

	// Rapidxml expects there to be a null terminator at the end of XML files, so we'll need
	// to copy the file data into a buffer, then manually add the null terminator to the end.
	XmlBuffer fileData;
	fileData.resize(projectFileSize + 1);

	// Copy the file data into the buffer.
	memcpy(fileData.data(), pProjectFileData, projectFileSize);
	fileData[projectFileSize] = '\0';

	HqReportHandle hReport = &pOutput->hCompiler->report;

	xml_document<> doc;

	try
	{
		// Parse the XML file in-place.
		doc.parse<0>(fileData.data());
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
		return nullptr;
	}

	// Attempt to load the data in the XML file.
	if(!prv_loadXmlDoc(pOutput, hReport, doc))
	{
		// An error occurred while loading the XML file data.
		HqReportMessage(
			hReport, 
			HQ_MESSAGE_TYPE_ERROR, 
			"Failed to load Harlequin project file data"
		);
		delete pOutput;
		return nullptr;
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqProject::Dispose(HqProjectHandle hProject)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);

	// Release the output path.
	HqString::Release(hProject->pOutput);

	// Release all define names and values.
	for(const Define& def : hProject->defines)
	{
		HqString::Release(def.pName);
		HqString::Release(def.pValue);
	}

	// Release all module reference paths.
	for(auto& kv : hProject->references)
	{
		HqString::Release(kv.second.pPath);
		HqString::Release(kv.second.pName);
	}

	// Iterate over each source file.
	for(auto& kv : hProject->files)
	{
		const File& file = kv.second;

		// Release all file-specific defines.
		for(const Define& def : file.defines)
		{
			HqString::Release(def.pName);
			HqString::Release(def.pValue);
		}

		// Release the file path.
		HqString::Release(file.pPath);
	}

	delete hProject;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqProject::prv_loadXmlDoc(HqProjectHandle hProject, HqReportHandle hReport, const rapidxml_ns::xml_document<>& root)
{
	using namespace rapidxml_ns;

	// Get the project node which will contain all project data.
	const xml_node<>* const pProjectNode = root.first_node("project");
	if(!pProjectNode)
	{
		HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Missing <project> node");
		return false;
	}

	// Get the project output.
	{
		const xml_node<>* const pOutputNode = pProjectNode->first_node("output");
		if(!pOutputNode)
		{
			HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Missing <output> node");
			return false;
		}

		const xml_attribute<>* const pPathAttr = pOutputNode->first_attribute("path");
		if(!pPathAttr)
		{
			HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Output is missing \"path\" attribute");
			return false;
		}

		hProject->pOutput = prv_createString(pPathAttr->value(), pPathAttr->value_size());
	}

	// Get each project reference.
	for(const xml_node<>* pRefNode = pProjectNode->first_node("reference")
		; pRefNode
		; pRefNode = pRefNode->next_sibling("reference"))
	{
		const xml_attribute<>* const pPathAttr = pRefNode->first_attribute("path");
		if(!pPathAttr)
		{
			HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Reference is missing \"path\" attribute");
			return false;
		}

		const xml_attribute<>* const pNameAttr = pRefNode->first_attribute("name");
		if(!pNameAttr)
		{
			HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Reference is missing \"name\" attribute");
			return false;
		}

		Reference ref;
		ref.pPath = prv_createString(pPathAttr->value(), pPathAttr->value_size());
		ref.pName = prv_createString(pNameAttr->value(), pNameAttr->value_size());

		// Check if this path has already been added.
		for(auto& kv : hProject->references)
		{
			if(HqString::FastCompare(ref.pPath, kv.second.pPath))
			{
				HqReportMessage(hReport, HQ_MESSAGE_TYPE_WARNING, "Module reference path duplicate: '%s'", ref.pPath->data);

				HqString::Release(ref.pPath);
				HqString::Release(ref.pName);

				ref.pPath = nullptr;
				ref.pName = nullptr;

				break;
			}

			if(HqString::FastCompare(ref.pName, kv.second.pName))
			{
				HqReportMessage(hReport, HQ_MESSAGE_TYPE_WARNING, "Module reference name duplicate: '%s'", ref.pName->data);

				HqString::Release(ref.pPath);
				HqString::Release(ref.pName);

				ref.pPath = nullptr;
				ref.pName = nullptr;

				break;
			}
		}

		if(ref.pPath)
		{
			hProject->references.emplace(ref.pPath, ref);
		}
	}

	// Get each project define.
	for(const xml_node<>* pDefineNode = pProjectNode->first_node("define")
		; pDefineNode
		; pDefineNode = pDefineNode->next_sibling("define"))
	{
		const xml_attribute<>* const pNameAttr = pDefineNode->first_attribute("name");
		if(!pNameAttr)
		{
			HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Project define is missing \"name\" attribute");
			return false;
		}

		const xml_attribute<>* const pValueAttr = pDefineNode->first_attribute("value");

		Define def;
		def.pName = prv_createString(pNameAttr->value(), pNameAttr->value_size());
		def.pValue = (pValueAttr) ? prv_createString(pValueAttr->value(), pValueAttr->value_size()) : nullptr;

		hProject->defines.push_back(def);
	}

	// Get each project source file.
	for(const xml_node<>* pFileNode = pProjectNode->first_node("file")
		; pFileNode
		; pFileNode = pFileNode->next_sibling("file"))
	{
		const xml_attribute<>* const pPathAttr = pFileNode->first_attribute("path");
		if(!pPathAttr)
		{
			HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "File is missing \"path\" attribute");
			return false;
		}

		File file;
		file.pPath = prv_createString(pPathAttr->value(), pPathAttr->value_size());

		// Check if this path has already been added.
		for(auto& kv : hProject->references)
		{
			if(HqString::FastCompare(file.pPath, kv.second.pPath))
			{
				HqReportMessage(hReport, HQ_MESSAGE_TYPE_WARNING, "Source file path duplicate: '%s'", file.pPath->data);
				HqString::Release(file.pPath);

				file.pPath = nullptr;
				break;
			}
		}

		if(file.pPath)
		{
			// Need the use the returned pair in order to access the mapped file object.
			auto kv = hProject->files.emplace(file.pPath, file).first;

			// Get each define specific to the current file.
			for(const xml_node<>* pDefineNode = pFileNode->first_node("define")
				; pDefineNode
				; pDefineNode = pDefineNode->next_sibling("define"))
			{
				const xml_attribute<>* const pNameAttr = pDefineNode->first_attribute("name");
				if(!pNameAttr)
				{
					HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "File define is missing \"name\" attribute");
					return false;
				}

				const xml_attribute<>* const pValueAttr = pDefineNode->first_attribute("value");

				Define def;
				def.pName = prv_createString(pNameAttr->value(), pNameAttr->value_size());
				def.pValue = (pValueAttr) ? prv_createString(pValueAttr->value(), pValueAttr->value_size()) : nullptr;

				kv->second.defines.push_back(def);
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

HqString* HqProject::prv_createString(const char* const data, const size_t length)
{
	// Allocate a temporary string.
	char* const temp = reinterpret_cast<char*>(alloca(sizeof(char) * (length + 1)));

	// Copy the attribute value into the string and add a null terminator.
	memcpy(temp, data, length);
	temp[length] = '\0';

	return HqString::Create(temp);
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
