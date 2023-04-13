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

#include "FileParseListener.hpp"

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

	pOutput->hToolCtx = hToolCtx;

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

void HqSourceFile::Dispose(HqSourceFileHandle hSrcFile)
{
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

int HqSourceFile::Parse(HqSourceFileHandle hSrcFile)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);

	// NOTE: Antlr utilizes a lot of static memory which *is* cleaned up on exit, but this means when using
	//       MSVC's CRT leak check API, there will be a ton of false positives reported on shutdown.
	ANTLRInputStream inputStream(reinterpret_cast<const char*>(hSrcFile->fileData.pData), hSrcFile->fileData.count);
	HarlequinLexer lexer(&inputStream);
	CommonTokenStream tokenStream(&lexer);
	HarlequinParser parser(&tokenStream);

	// TODO: Setup the parser error handler

	// Parse the source file.
	ParseTree* pAstRoot = parser.root();

	// Walk the source file's AST.
	HqFileParseListener listener(hSrcFile);
	ParseTreeWalker::DEFAULT.walk(&listener, pAstRoot);

	return HQ_ERROR_NOT_IMPLEMENTED;
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
