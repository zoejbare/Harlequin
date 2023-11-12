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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "MessageCode.hpp"
#include "SourceData.hpp"

#include "../../../base/String.hpp"

#include <BaseErrorListener.h>

#include <stdarg.h>

//----------------------------------------------------------------------------------------------------------------------

class ParserErrorHandler final
	: protected antlr4::BaseErrorListener
{
public:

	ParserErrorHandler() = delete;
	ParserErrorHandler(HqReportHandle hReport, const SourceData& srcData);

	size_t GetErrorCount() const;
	size_t GetWarningCount() const;

	bool EncounteredError() const;

	void Report(MessageCode code, const antlr4::Token* pOffendingSymbol, const char* fmt, ...);


private:

	enum class MsgType
	{
		Error,
		Warning,
	};

	virtual void syntaxError(
		antlr4::Recognizer* pRecognizer,
		antlr4::Token* pOffendingSymbol,
		size_t line,
		size_t charPositionInLine,
		const std::string& msg,
		std::exception_ptr e
	) override;

	void prv_report(int, MessageCode, const antlr4::Token*, const char*);

	HqReportHandle m_hReport;

	const SourceData* m_pSrcData;

	size_t m_errorCount;
	size_t m_warningCount;
};

//----------------------------------------------------------------------------------------------------------------------

inline size_t ParserErrorHandler::GetErrorCount() const
{
	return m_errorCount;
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t ParserErrorHandler::GetWarningCount() const
{
	return m_warningCount;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool ParserErrorHandler::EncounteredError() const
{
	return m_errorCount > 0;
}

//----------------------------------------------------------------------------------------------------------------------

inline void ParserErrorHandler::Report(
	const MessageCode code, 
	const antlr4::Token* const pOffendingSymbol, 
	const char* const fmt, 
	...)
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
