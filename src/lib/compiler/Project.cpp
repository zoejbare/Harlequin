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

HqProjectHandle HqProject::Create(HqCompilerHandle hCompiler)
{
	assert(hCompiler != HQ_COMPILER_HANDLE_NULL);

	HqProject* const pOutput = new HqProject();
	assert(pOutput != nullptr);

	pOutput->hCompiler = hCompiler;
	pOutput->pOutput = nullptr;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqProject::Dispose(HqProjectHandle hProject)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);

	// Release the output path.
	HqString::Release(hProject->pOutput);

	// Release all defines.
	for(HqString* const pDefine : hProject->defines)
	{
		HqString::Release(pDefine);
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
		for(HqString* const pDefine : file.defines)
		{
			HqString::Release(pDefine);
		}

		// Release the file path.
		HqString::Release(file.pPath);
	}

	delete hProject;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProject::SetOutput(HqProjectHandle hProject, HqString* const pPath)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);
	assert(pPath != nullptr);
	assert(pPath->length > 0);

	if(hProject->pOutput)
	{
		HqReportMessage(
			&hProject->hCompiler->report, 
			HQ_MESSAGE_TYPE_WARNING, 
			"Overwriting project output path: %s", 
			hProject->pOutput->data
		);
		HqString::Release(hProject->pOutput);
	}

	hProject->pOutput = pPath;

	HqString::AddRef(hProject->pOutput);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProject::AddReference(HqProjectHandle hProject, HqString* const pPath, HqString* const pName)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);
	assert(pPath != nullptr);
	assert(pPath->length > 0);

	if(pName)
	{
		assert(pName->length > 0);
	}

	// Check for any entry matching the input path or name.
	for(auto& kv : hProject->references)
	{
		if(HqString::FastCompare(pPath, kv.second.pPath))
		{
			HqReportMessage(
				&hProject->hCompiler->report, 
				HQ_MESSAGE_TYPE_ERROR, 
				"Duplicate reference path: %s", 
				pPath->data
			);
			return HQ_ERROR_DUPLICATE;
		}

		if(pName && HqString::FastCompare(pName, kv.second.pName))
		{
			HqReportMessage(
				&hProject->hCompiler->report, 
				HQ_MESSAGE_TYPE_ERROR, 
				"Duplicate reference name: %s", 
				pName->data
			);
			return HQ_ERROR_DUPLICATE;
		}
	}

	Reference ref;
	ref.pPath = pPath;
	ref.pName = pName;

	hProject->references.emplace(pPath, ref);
	HqString::AddRef(pPath);
	HqString::AddRef(pName);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProject::AddFile(HqProjectHandle hProject, HqString* const pPath)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);
	assert(pPath != nullptr);
	assert(pPath->length > 0);

	if(hProject->files.count(pPath))
	{
		HqReportMessage(
			&hProject->hCompiler->report, 
			HQ_MESSAGE_TYPE_ERROR, 
			"Duplicate source file: %s", 
			pPath->data
		);
		return HQ_ERROR_DUPLICATE;
	}

	File file;
	file.pPath = pPath;

	hProject->files.emplace(pPath, file);
	HqString::AddRef(pPath);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProject::AddDefine(HqProjectHandle hProject, HqString* const pDefine)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);
	assert(pDefine != nullptr);
	assert(pDefine->length > 0);

	hProject->defines.push_back(pDefine);
	HqString::AddRef(pDefine);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqProject::AddFileDefine(HqProjectHandle hProject, HqString* const pFilePath, HqString* const pDefine)
{
	assert(hProject != HQ_PROJECT_HANDLE_NULL);
	assert(pFilePath != nullptr);
	assert(pFilePath->length > 0);
	assert(pDefine != nullptr);
	assert(pDefine->length > 0);

	auto kv = hProject->files.find(pFilePath);
	if(kv == hProject->files.end())
	{
		HqReportMessage(
			&hProject->hCompiler->report, 
			HQ_MESSAGE_TYPE_ERROR, 
			"Source file not found for define"
				": file='%s'"
				", define='%s'", 
			pFilePath->data, 
			pDefine->data
		);
		return HQ_ERROR_NON_EXISTENT;
	}

	kv->second.defines.push_back(pDefine);
	HqString::AddRef(pDefine);

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
