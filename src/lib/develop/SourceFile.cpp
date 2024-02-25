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

#include "compiler/antlr/AntlrAstBuilder.hpp"
#include "compiler/antlr/AntlrErrorHandler.hpp"

#include "compiler/autogen/HarlequinLexer.h"
#include "compiler/autogen/HarlequinParser.h"

#include "DevContext.hpp"

#include "../base/Serializer.hpp"

#include <antlr4-runtime.h>

#include <assert.h>
#include <memory.h>

//----------------------------------------------------------------------------------------------------------------------

HqSourceFileHandle HqSourceFile::Load(HqDevContextHandle hCtx, const char* const filePath, int* const pErrorReason)
{
	assert(hCtx != HQ_DEV_CONTEXT_HANDLE_NULL);
	assert(filePath != nullptr);
	assert(filePath[0] != '\0');
	assert(pErrorReason != nullptr);

	HqReportHandle hReport = &hCtx->report;

	// Create a serializer for loading the source file.
	HqSerializerHandle hSerializer = HqSerializer::Create(HQ_SERIALIZER_MODE_READER);
	if(!hSerializer)
	{
		HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Failed to allocate file input serializer");
		(*pErrorReason) = HQ_ERROR_BAD_ALLOCATION;
		return nullptr;
	}

	// Attempt to load the source file.
	{
		const int result = HqSerializer::LoadFile(hSerializer, filePath);
		if(result != HQ_SUCCESS)
		{
			HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Failed to load input file: reason=%s", HqGetErrorCodeString(result));
			(*pErrorReason) = HQ_ERROR_FAILED_TO_OPEN_FILE;
			return nullptr;
		}
	}

	HqSourceFile* const pOutput = new HqSourceFile();
	if(!pOutput)
	{
		HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Failed to allocate HqSourceFile object");
		(*pErrorReason) = HQ_ERROR_BAD_ALLOCATION;
		return nullptr;
	}

	HqReference::Initialize(pOutput->ref, _onDestruct, pOutput);

	pOutput->hCtx = hCtx;
	pOutput->hSerializer = hSerializer;
	pOutput->parseResult = detail::ParseResult::Pending;

	const char* const streamData = reinterpret_cast<const char*>(pOutput->hSerializer->stream.pData);
	const size_t streamLength = pOutput->hSerializer->stream.count;

	// Initialize the source context which will let the compiler directly
	// reference the input source file when reporting messages to the user.
	pOutput->srcCtx.Initialize(hReport, filePath, streamData, streamLength);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSourceFile::Parse(HqSourceFileHandle hSrcFile)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);

	switch(hSrcFile->parseResult)
	{
		case detail::ParseResult::Success: return HQ_SUCCESS;
		case detail::ParseResult::Failure: return HQ_ERROR_FAILED_TO_PARSE_FILE;

		default:
			break;
	}

	const char* const streamData = reinterpret_cast<const char*>(hSrcFile->hSerializer->stream.pData);
	const size_t streamLength = hSrcFile->hSerializer->stream.count;

	ParseTree* pParseTree = nullptr;

	// [NOTE]
	//   Antlr utilizes a lot of static memory which *is* cleaned up on exit, but this means when using
	//   MSVC's CRT leak check API, there will be a ton of false positives reported on shutdown. The better
	//   way to check for leaky memory is to setup a custom allocator and dump any allocations that are
	//   still around after disposing of all the HQ compiler resources.

	// Setup the ANTLR lexer and parser.
	ANTLRInputStream inputStream(streamData, streamLength);
	HarlequinLexer lexer(&inputStream);
	CommonTokenStream tokenStream(&lexer);
	HarlequinParser parser(&tokenStream);

	// Set the file name for the input stream; this will be used when reporting errors and warnings while parsing.
	inputStream.name = hSrcFile->srcCtx.GetFilePath();

	// Remove the default error listeners.
	lexer.removeErrorListeners();
	parser.removeErrorListeners();

	AntlrErrorHandler errorHandler(hSrcFile->srcCtx);

	// Add our custom error listener.
	lexer.addErrorListener(reinterpret_cast<antlr4::ANTLRErrorListener*>(&errorHandler));
	parser.addErrorListener(reinterpret_cast<antlr4::ANTLRErrorListener*>(&errorHandler));

	// Parse the source file.
	pParseTree = parser.root();

	// Pre-emptively set the parse result to the 'failure' state in case any errors are detected.
	// The result will be updated to the 'success' state if we manage to get to the end of this
	// function without any errors.
	hSrcFile->parseResult = detail::ParseResult::Failure;

	// Checking for parsing/lexing errors.
	if(hSrcFile->srcCtx.EncounteredErrors())
	{
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	// Convert the ANTLR parse tree to our custom AST.
	RootNode::Ptr pRootNode = AntlrAstBuilder::Run(hSrcFile->srcCtx, pParseTree);

	// Checking for errors that occurred while building the AST.
	if(hSrcFile->srcCtx.EncounteredErrors())
	{
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	// Do the first pass of semantic analysis on the AST. This first pass will only do analysis of this
	// translation unit without regard for any imports or aliases since that information is not yet available.
	// Cross-unit analysis will happen during the linking phase later.
	// TODO: Walk the AST for analysis.

	// Check for errors that occured during semantic analysis.
	if(hSrcFile->srcCtx.EncounteredErrors())
	{
		return HQ_ERROR_FAILED_TO_PARSE_FILE;
	}

	// Parsing was successful, and the developers rejoiced!
	hSrcFile->pAstRootNode = std::move(pRootNode);
	hSrcFile->parseResult = detail::ParseResult::Success;

	return HQ_SUCCESS;
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

bool HqSourceFile::WasParsedSuccessfully(HqSourceFileHandle hSrcFile)
{
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);
	return hSrcFile->parseResult == detail::ParseResult::Success;
}

//----------------------------------------------------------------------------------------------------------------------

void HqSourceFile::_onDestruct(void* const pOpaque)
{
	using namespace antlr4;

	HqSourceFileHandle hSrcFile = reinterpret_cast<HqSourceFileHandle>(pOpaque);
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);

	HqSerializer::Dispose(hSrcFile->hSerializer);

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
