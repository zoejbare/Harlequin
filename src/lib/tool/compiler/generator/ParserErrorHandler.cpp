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

#include "ParserErrorHandler.hpp"

#include <antlr4-runtime.h>

#include <assert.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------

ParserErrorHandler::ParserErrorHandler(HqReportHandle hReport, const SourceData& srcData)
	: m_hReport(hReport)
	, m_pSrcData(&srcData)
	, m_errorCount(0)
	, m_warningCount(0)
{
	assert(hReport != HQ_REPORT_HANDLE_NULL);
	assert(m_pSrcData != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

void ParserErrorHandler::syntaxError(
	antlr4::Recognizer* const pRecognizer,
	antlr4::Token* const pOffendingSymbol,
	const size_t line,
	const size_t charPositionInLine,
	const std::string& msg,
	std::exception_ptr e)
{
	(void) pRecognizer;
	(void) line;
	(void) charPositionInLine;
	(void) e;
	Report(MessageCode::ErrorSyntax, TokenSpan::WithSourceText(pOffendingSymbol), msg);
}

//----------------------------------------------------------------------------------------------------------------------

void ParserErrorHandler::Report(
	const MessageCode code,
	const TokenSpan& span,
	const std::string& primaryMsg,
	const std::string& secondaryMsg)
{
	const int msgType = _getMsgType(code);

	const char* typeStr = nullptr;
	switch(msgType)
	{
		case HQ_MESSAGE_TYPE_ERROR:
			typeStr = "error ";
			break;

		case HQ_MESSAGE_TYPE_WARNING:
			typeStr = "warning ";
			break;

		default:
			// This should never happen.
			assert(false);
			break;
	}

	char codeStr[6];
	snprintf(codeStr, sizeof(codeStr) / sizeof(char), "C%04" PRIu16, uint16_t(code));

	std::stringstream msgStream;
	msgStream
		<< span.sourceName
		<< "(" << span.lineNumber << "): "
		<< typeStr << codeStr << ": "
		<< primaryMsg;

	if(span.startIndex < span.stopIndex)
	{
		const std::string sourceCode = m_pSrcData->lines[span.lineNumber - 1];

		// Add the source code text to the message.
		msgStream << std::endl << sourceCode << std::endl;

		// Add the whitespace leading up to the underlining.
		for(size_t i = 0; i < span.positionInLine; ++i)
		{
			const char charInLine = sourceCode[i];
			const char charToWrite = (isspace(charInLine) > 0)
				? charInLine
				: ' ';

			msgStream << charToWrite;
		}

		// Add the underline that is as long as the offending token.
		for(size_t i = span.startIndex; i <= span.stopIndex; ++i)
		{
			msgStream << '~';
		}
	}

	if(!secondaryMsg.empty())
	{
		// Append the secondary message to the end of the stream.
		msgStream << std::endl << primaryMsg;
	}

	// Pass the final message string to the API's reporting mechanism.
	HqReportMessage(m_hReport, msgType, "%s", msgStream.str().c_str());

	if(msgType == HQ_MESSAGE_TYPE_ERROR)
	{
		++m_errorCount;
	}
}

//----------------------------------------------------------------------------------------------------------------------
