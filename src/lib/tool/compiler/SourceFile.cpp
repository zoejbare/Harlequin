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

#include "util/ParserErrorListener.hpp"
#include "util/SymbolVisitor.hpp"

#include "generated/HarlequinLexer.h"
#include "generated/HarlequinParser.h"

#include "../ToolContext.hpp"

#include "../../base/Serializer.hpp"

#include <antlr4-runtime.h>

#include <assert.h>
#include <memory.h>

//----------------------------------------------------------------------------------------------------------------------

HqSourceFileHandle HqSourceFile::Load(HqToolContextHandle hToolCtx, const char* const filePath, int* const pErrorReason)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	assert(hToolCtx != HQ_TOOL_CONTEXT_HANDLE_NULL);
	assert(filePath != nullptr);
	assert(filePath[0] != '\0');
	assert(pErrorReason != nullptr);

	// Create a serializer for loading the source file.
	HqSerializerHandle hSerializer = HqSerializer::Create(HQ_SERIALIZER_MODE_READER);
	if(!hSerializer)
	{
		(*pErrorReason) = HQ_ERROR_BAD_ALLOCATION;
		return nullptr;
	}

	// Attempt to load the source file.
	if(HqSerializer::LoadFile(hSerializer, filePath) != HQ_SUCCESS)
	{
		(*pErrorReason) = HQ_ERROR_FAILED_TO_OPEN_FILE;
		return nullptr;
	}

	HqSourceFile* const pOutput = new HqSourceFile();
	assert(pOutput != nullptr);

	HqReference::Initialize(pOutput->ref, prv_onDestruct, pOutput);

	pOutput->hToolCtx = hToolCtx;
	pOutput->hSerializer = hSerializer;

	// Initialize the source data structures.
	NamespaceMap::Allocate(pOutput->namespaces);
	ClassAliasMap::Allocate(pOutput->classAliases);
	ClassMap::Allocate(pOutput->classes);
	ClassVarMap::Allocate(pOutput->classVars);

	// NOTE: Antlr utilizes a lot of static memory which *is* cleaned up on exit, but this means when using
	//       MSVC's CRT leak check API, there will be a ton of false positives reported on shutdown.
	pOutput->pInputStream = new ANTLRInputStream(
		reinterpret_cast<const char*>(pOutput->hSerializer->stream.pData), 
		pOutput->hSerializer->stream.count
	);
	pOutput->pLexer = new HarlequinLexer(pOutput->pInputStream);
	pOutput->pTokenStream = new CommonTokenStream(pOutput->pLexer);
	pOutput->pParser = new HarlequinParser(pOutput->pTokenStream);
	pOutput->pAstRoot = nullptr;

	// Set the file name for the input stream; this will be used when reporting errors and warnings.
	pOutput->pInputStream->name = filePath;

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
	if(errorListener.EncounteredError())
	{
		hSrcFile->pAstRoot = nullptr;
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	// Walk the AST to discover the source file symbols.
	SymbolVisitor symbolResolver(hSrcFile, &errorListener);
	symbolResolver.visit(hSrcFile->pAstRoot);

	// Check for post-parsing errors.
	if(errorListener.EncounteredError())
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

	// Free all class metadata.
	{
		ClassMap::Iterator iter;
		while(ClassMap::IterateNext(hSrcFile->classes, iter))
		{
			// Release all the implements strings.
			for(size_t i = 0; i < iter.pData->value->implements.count; ++i)
			{
				HqString::Release(iter.pData->value->implements.pData[i]);
			}

			// Release all the extends strings.
			for(size_t i = 0; i < iter.pData->value->extends.count; ++i)
			{
				HqString::Release(iter.pData->value->extends.pData[i]);
			}

			ClassMetaData::StringArray::Dispose(iter.pData->value->implements);
			ClassMetaData::StringArray::Dispose(iter.pData->value->extends);

			HqString::Release(iter.pData->key);
			HqString::Release(iter.pData->value->pQualifiedName);
			HqString::Release(iter.pData->value->pShortName);
			HqString::Release(iter.pData->value->pNamespace);

			delete iter.pData->value;
		}
	}

	// Free all class variable metadata.
	{
		ClassVarMap::Iterator iter;
		while(ClassVarMap::IterateNext(hSrcFile->classVars, iter))
		{
			HqString::Release(iter.pData->value->var.pName);
			HqString::Release(iter.pData->value->var.pTypeName);

			delete iter.pData->value;
		}
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
	ClassMap::Dispose(hSrcFile->classes);
	ClassVarMap::Dispose(hSrcFile->classVars);
	LineArray::Dispose(hSrcFile->fileLines);

	HqSerializer::Dispose(hSrcFile->hSerializer);

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
