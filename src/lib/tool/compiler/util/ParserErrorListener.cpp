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

#include "ParserErrorListener.hpp"

#include <antlr4-runtime.h>

#include <assert.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------

ParserErrorListener::ParserErrorListener(HqReportHandle hReport, const HqArray<HqString*>* const pSourceLines)
	: m_hReport(hReport)
	, m_pSourceLines(pSourceLines)
	, m_errorCount(0)
	, m_warningCount(0)
{
	assert(hReport != HQ_REPORT_HANDLE_NULL);
	assert(pSourceLines != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

void ParserErrorListener::syntaxError(
	antlr4::Recognizer* const pRecognizer,
	antlr4::Token* const pOffendingSymbol,
	const size_t line,
	const size_t charPositionInLine,
	const std::string& msg,
	std::exception_ptr e
)
{
	(void) pRecognizer;
	(void) line;
	(void) charPositionInLine;
	(void) e;
	prv_report(HQ_MESSAGE_TYPE_ERROR, MessageCode::ErrorSyntax, pOffendingSymbol, msg.c_str());
}

//----------------------------------------------------------------------------------------------------------------------

void ParserErrorListener::Report(const MessageCode code, const antlr4::Token* const pOffendingSymbol, const char* const fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);
	const char* const message = HqString::RawFormatVarArgs(fmt, vl);
	va_end(vl);

	prv_report(
		int(code) < int(MessageCode::_WarningStart_)
			? HQ_MESSAGE_TYPE_ERROR
			: HQ_MESSAGE_TYPE_WARNING,
		code,
		pOffendingSymbol,
		message
	);
	HqMemFree((void*) message);
}

//----------------------------------------------------------------------------------------------------------------------

void ParserErrorListener::prv_report(
	const int type,
	const MessageCode code,
	const antlr4::Token* const pOffendingSymbol,
	const char* const msg
)
{
	const size_t startIndex = pOffendingSymbol->getStartIndex();
	const size_t stopIndex = pOffendingSymbol->getStopIndex();
	const size_t lineNumber = pOffendingSymbol->getLine();
	const size_t charPositionInLine = pOffendingSymbol->getCharPositionInLine();
	const std::string sourceName = pOffendingSymbol->getTokenSource()->getSourceName();

	const HqString* const pLine = m_pSourceLines->pData[lineNumber - 1];

	const char* typeStr = nullptr;
	switch(type)
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
	snprintf(codeStr, sizeof(codeStr) / sizeof(char), "C%04" PRId32, int(code));

	std::string finalMsg 
		= sourceName 
		+ "(" + std::to_string(lineNumber) + "): " 
		+ typeStr + codeStr + ": " 
		+ msg + "\n" 
		+ pLine->data + "\n";

	// Add the whitespace leading up to the underlining.
	for(size_t i = 0; i < charPositionInLine; ++i)
	{
		const char charInLine = pLine->data[i];

		finalMsg += (isspace(charInLine) > 0) 
			? charInLine 
			: ' ';
	}

	// Add the underline that is as long as the offending token.
	for(size_t i = startIndex; i <= stopIndex; ++i)
	{
		finalMsg += "~";
	}

	HqReportMessage(m_hReport, type, "%s", finalMsg.c_str());

	if(type == HQ_MESSAGE_TYPE_ERROR)
	{
		++m_errorCount;
	}
}

//----------------------------------------------------------------------------------------------------------------------
