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
	const char* const name,
	const void* const pFileData,
	const size_t fileSize,
	int* const pErrorReason)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	assert(hToolCtx != HQ_TOOL_CONTEXT_HANDLE_NULL);
	assert(name != nullptr);
	assert(name[0] != '\0');
	assert(pFileData != nullptr);
	assert(fileSize > 0);
	assert(pErrorReason != nullptr);

	HqSourceFile* const pOutput = new HqSourceFile();
	assert(pOutput != nullptr);

	HqReference::Initialize(pOutput->ref, prv_onDestruct, pOutput);

	pOutput->hToolCtx = hToolCtx;

	// Initialize the file data array and reserve space for the input data.
	FileData::Initialize(pOutput->fileData);
	FileData::Reserve(pOutput->fileData, fileSize);

	pOutput->fileData.count = fileSize;

	// Copy the input data to the file data array.
	memcpy(pOutput->fileData.pData, pFileData, fileSize);

	// Initialize the source data structures.
	NamespaceMap::Allocate(pOutput->namespaces);
	ClassAliasMap::Allocate(pOutput->classAliases);

	// NOTE: Antlr utilizes a lot of static memory which *is* cleaned up on exit, but this means when using
	//       MSVC's CRT leak check API, there will be a ton of false positives reported on shutdown.
	pOutput->pInputStream = new ANTLRInputStream(reinterpret_cast<const char*>(pOutput->fileData.pData), pOutput->fileData.count);
	pOutput->pLexer = new HarlequinLexer(pOutput->pInputStream);
	pOutput->pTokenStream = new CommonTokenStream(pOutput->pLexer);
	pOutput->pParser = new HarlequinParser(pOutput->pTokenStream);
	pOutput->pAstRoot = nullptr;

	// Set the file name for the input stream; this will be used when reporting errors and warnings.
	pOutput->pInputStream->name = name;

	pOutput->parsed = false;

	// Remove the default error listeners.
	pOutput->pLexer->removeErrorListeners();
	pOutput->pParser->removeErrorListeners();

	// Resolve the file data stream to an array of lines that will be used when displaying error/warning messages.
	{
		std::string fileText = pOutput->pInputStream->toString();

		// Calculate the number of lines there are in the file text.
		const size_t lineCount = std::count(fileText.begin(), fileText.end(), '\n') + 1;

		// Reserve enough space in the array for each line.
		LineArray::Initialize(pOutput->fileLines);
		LineArray::Reserve(pOutput->fileLines, lineCount);

		memset(pOutput->fileLines.pData, 0, sizeof(HqString*) * lineCount);

		// Replace all carriage returns in the line with whitespace.
		std::replace(fileText.begin(), fileText.end(), '\r', ' ');

		// Extract each line from the text.
		std::stringstream ss(fileText);
		for(std::string line; std::getline(ss, line, '\n');)
		{
			// Strip the whitespace from the end of the line.
			line.erase(
				std::find_if(
					line.rbegin(), 
					line.rend(), 
					[](const char ch)
					{ 
						return !std::isspace(ch);
					}
				).base(), 
				line.end()
			);

			HqString* const pLine = HqString::Create(line.c_str());

			// Add the line to the array.
			pOutput->fileLines.pData[pOutput->fileLines.count] = pLine;
			++pOutput->fileLines.count;
		}
	}

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
	using namespace antlr4::tree;

	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);

	if(hSrcFile->parsed)
	{
		return hSrcFile->pAstRoot ? HQ_SUCCESS : HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	ParserErrorListener errorListener(&hSrcFile->hToolCtx->report, &hSrcFile->fileLines);

	// Add our custom error listener.
	hSrcFile->pLexer->addErrorListener(&errorListener);
	hSrcFile->pParser->addErrorListener(&errorListener);

	// Parse the source file.
	hSrcFile->pAstRoot = hSrcFile->pParser->root();
	hSrcFile->parsed = true;

	// Remove the error listener.
	hSrcFile->pLexer->removeErrorListeners();
	hSrcFile->pParser->removeErrorListeners();

	// Checking for parsing+lexing error
	if(errorListener.GetErrorCount() > 0)
	{
		hSrcFile->pAstRoot = nullptr;
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	// Walk the source file's AST.
	FileDataListener astListener(hSrcFile, &errorListener);
	ParseTreeWalker::DEFAULT.walk(&astListener, hSrcFile->pAstRoot);

	// Check for post-parsing errors.
	if(errorListener.GetErrorCount() > 0)
	{
		hSrcFile->pAstRoot = nullptr;
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqSourceFile::WasParsedSuccessfully(HqSourceFileHandle hSrcFile)
{
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);
	return hSrcFile->parsed && (hSrcFile->pAstRoot != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

void HqSourceFile::prv_onDestruct(void* const pOpaque)
{
	HqSourceFileHandle hSrcFile = reinterpret_cast<HqSourceFileHandle>(pOpaque);
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);

	// Release all file line strings.
	for(size_t i = 0; i < hSrcFile->fileLines.count; ++i)
	{
		HqString::Release(hSrcFile->fileLines.pData[i]);
	}

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
	LineArray::Dispose(hSrcFile->fileLines);

	delete hSrcFile->pInputStream;
	delete hSrcFile->pTokenStream;
	delete hSrcFile->pLexer;
	delete hSrcFile->pParser;

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
