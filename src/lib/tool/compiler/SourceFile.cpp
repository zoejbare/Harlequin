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

#include "SourceFile.hpp"

#include "util/FileDataListener.hpp"
#include "util/ParserErrorListener.hpp"

#include "generated/HarlequinLexer.h"
#include "generated/HarlequinParser.h"

#include "../ToolContext.hpp"

#include <antlr4-runtime.h>

#include <assert.h>
#include <memory.h>

//----------------------------------------------------------------------------------------------------------------------

HqSourceFileHandle HqSourceFile::Load(
	HqToolContextHandle hToolCtx,
	const void* pFileData,
	size_t fileSize,
	int* pErrorReason)
{
	assert(hToolCtx != HQ_TOOL_CONTEXT_HANDLE_NULL);
	assert(pFileData != nullptr);
	assert(fileSize > 0);
	assert(pErrorReason != nullptr);

	HqSourceFile* const pOutput = new HqSourceFile();
	assert(pOutput != nullptr);

	HqReference::Initialize(pOutput->ref, prv_onDestruct, pOutput);

	pOutput->hToolCtx = hToolCtx;
	pOutput->parsed = false;

	// Initialize the file data array and reserve space for the input data.
	FileData::Initialize(pOutput->fileData);
	FileData::Reserve(pOutput->fileData, fileSize);

	pOutput->fileData.count = fileSize;

	// Initialize the source data structures.
	NamespaceMap::Allocate(pOutput->namespaces);
	ClassAliasMap::Allocate(pOutput->classAliases);

	// Copy the input data to the file data array.
	memcpy(pOutput->fileData.pData, pFileData, fileSize);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqSourceFile::AddRef(HqSourceFileHandle hSrcFile)
{
	return (hSrcFile)
		? HqReference::AddRef(hSrcFile->ref)
		: -1;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqSourceFile::Release(HqSourceFileHandle hSrcFile)
{
	return (hSrcFile)
		? HqReference::Release(hSrcFile->ref)
		: -1;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSourceFile::Parse(HqSourceFileHandle hSrcFile)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);

	if(hSrcFile->parsed)
	{
		return HQ_SUCCESS;
	}

	// NOTE: Antlr utilizes a lot of static memory which *is* cleaned up on exit, but this means when using
	//       MSVC's CRT leak check API, there will be a ton of false positives reported on shutdown.
	ANTLRInputStream inputStream(reinterpret_cast<const char*>(hSrcFile->fileData.pData), hSrcFile->fileData.count);
	HarlequinLexer lexer(&inputStream);
	CommonTokenStream tokenStream(&lexer);
	HarlequinParser parser(&tokenStream);

	// Remove the default error listeners.
	lexer.removeErrorListeners();
	parser.removeErrorListeners();

	// Add our custom error listener.
	ParserErrorListener errorListener;
	lexer.addErrorListener(&errorListener);
	parser.addErrorListener(&errorListener);

	// Parse the source file.
	ParseTree* pAstRoot = parser.root();

	// Checking for parsing+lexing error
	if(errorListener.GetErrorCount() > 0)
	{
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	// Walk the source file's AST.
	FileDataListener astListener(hSrcFile, errorListener);
	ParseTreeWalker::DEFAULT.walk(&astListener, pAstRoot);

	// Check for post-parsing errors.
	if(errorListener.GetErrorCount() > 0)
	{
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	// TODO: Fill data on the source file from the AST listener.

	hSrcFile->parsed = true;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

void HqSourceFile::prv_onDestruct(void* const pOpaque)
{
	HqSourceFileHandle hSrcFile = reinterpret_cast<HqSourceFileHandle>(pOpaque);
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);

	// Release all mapped class alias strings.
	{
		ClassAliasMap::Iterator iter;
		while(ClassAliasMap::IterateNext(hSrcFile->classAliases, iter))
		{
			HqString::Release(iter.pData->key);
			HqString::Release(iter.pData->value);
		}
	}

	// Release all used namespace strings
	{
		NamespaceMap::Iterator iter;
		while(NamespaceMap::IterateNext(hSrcFile->namespaces, iter))
		{
			HqString::Release(iter.pData->key);
		}
	}

	// Dispose of the source data structures.
	NamespaceMap::Dispose(hSrcFile->namespaces);
	ClassAliasMap::Dispose(hSrcFile->classAliases);

	// Dispose of the raw file data.
	FileData::Dispose(hSrcFile->fileData);

	delete hSrcFile;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqSourceFile::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqSourceFile::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
