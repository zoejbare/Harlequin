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

#include "generator/ParserErrorHandler.hpp"

#include "parser/HarlequinLexer.h"
#include "parser/HarlequinParser.h"

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

	// NOTE: Antlr utilizes a lot of static memory which *is* cleaned up on exit, but this means when using
	//       MSVC's CRT leak check API, there will be a ton of false positives reported on shutdown.
	pOutput->pInputStream = new ANTLRInputStream(
		reinterpret_cast<const char*>(pOutput->hSerializer->stream.pData),
		pOutput->hSerializer->stream.count);
	pOutput->pLexer = new HarlequinLexer(pOutput->pInputStream);
	pOutput->pTokenStream = new CommonTokenStream(pOutput->pLexer);
	pOutput->pParser = new HarlequinParser(pOutput->pTokenStream);
	pOutput->pParseTree = nullptr;

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
		pOutput->srcData.lines.reserve(lineCount);

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

			// Add the line to the array.
			pOutput->srcData.lines.push_back(line);
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
		return hSrcFile->pParseTree ? HQ_SUCCESS : HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	ParserErrorHandler errorHandler(&hSrcFile->hToolCtx->report, hSrcFile->srcData);

	// Add our custom error listener.
	hSrcFile->pLexer->addErrorListener(reinterpret_cast<antlr4::ANTLRErrorListener*>(&errorHandler));
	hSrcFile->pParser->addErrorListener(reinterpret_cast<antlr4::ANTLRErrorListener*>(&errorHandler));

	// Parse the source file.
	hSrcFile->pParseTree = hSrcFile->pParser->root();
	hSrcFile->parsed = true;

	// Remove the error listener.
	hSrcFile->pLexer->removeErrorListeners();
	hSrcFile->pParser->removeErrorListeners();

	// Checking for parsing+lexing errors.
	if(errorHandler.EncounteredError())
	{
		hSrcFile->pParseTree = nullptr;
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	// Generate the symbol table from the parse tree.
	SymbolTableGenerator::Run(hSrcFile->pParseTree, &errorHandler, &hSrcFile->symbolTable);

	// Check for errors that occured during symbol resolution.
	if(errorHandler.EncounteredError())
	{
		hSrcFile->pParseTree = nullptr;
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqSourceFile::WasParsedSuccessfully(HqSourceFileHandle hSrcFile)
{
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);
	return hSrcFile->parsed && (hSrcFile->pParseTree != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

void HqSourceFile::prv_onDestruct(void* const pOpaque)
{
	HqSourceFileHandle hSrcFile = reinterpret_cast<HqSourceFileHandle>(pOpaque);
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);

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
