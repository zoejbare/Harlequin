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

#include "ReferenceModule.hpp"

#include "../ToolContext.hpp"

#include <assert.h>
#include <inttypes.h>
#include <memory.h>

//----------------------------------------------------------------------------------------------------------------------

HqReferenceModuleHandle HqReferenceModule::Load(
	HqToolContextHandle hToolCtx,
	const void* pFileData,
	size_t fileSize,
	int* pErrorReason)
{
	assert(hToolCtx != HQ_TOOL_CONTEXT_HANDLE_NULL);
	assert(pFileData != nullptr);
	assert(fileSize > 0);
	assert(pErrorReason != nullptr);

	HqReferenceModule* const pOutput = new HqReferenceModule();
	assert(pOutput != nullptr);

	// Attempt to load the module metadata, disregarding the bytecode.
	if(!HqModuleLoader::Load(pOutput->data, &hToolCtx->report, pFileData, fileSize, HqModuleLoader::DISCARD_BYTECODE))
	{
		(*pErrorReason) = HQ_ERROR_FAILED_TO_OPEN_FILE;
		delete pOutput;
		return nullptr;
	}

	pOutput->hToolCtx = hToolCtx;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqReferenceModule::Dispose(HqReferenceModuleHandle hRefModule)
{
	assert(hRefModule != HQ_REFERENCE_MODULE_HANDLE_NULL);

	HqModuleLoader::Dispose(hRefModule->data);

	delete hRefModule;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqReferenceModule::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqReferenceModule::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
