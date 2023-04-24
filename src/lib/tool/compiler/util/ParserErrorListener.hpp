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

#include "ErrorNotifier.hpp"

#include "../../../base/String.hpp"
#include "../../../common/Array.hpp"

#include <BaseErrorListener.h>

//----------------------------------------------------------------------------------------------------------------------

class ParserErrorListener
	: public antlr4::BaseErrorListener
	, public IErrorNotifier
{
public:

	ParserErrorListener() = delete;
	ParserErrorListener(HqReportHandle hReport, const HqArray<HqString*>* pSourcLines);

	size_t GetErrorCount() const;
	size_t GetWarningCount() const;

	virtual void syntaxError(
		antlr4::Recognizer* pRecognizer,
		antlr4::Token* pOffendingSymbol,
		size_t line,
		size_t charPositionInLine,
		const std::string& msg,
		std::exception_ptr e
	) override;

	virtual bool EncounteredError() const override;
	virtual void Report(MessageCode code, const antlr4::Token* pOffendingSymbol, const char* fmt, ...) override;


private:

	enum class MsgType
	{
		Error,
		Warning,
	};

	void prv_report(int, MessageCode, const antlr4::Token*, const char*);

	HqReportHandle m_hReport;

	const HqArray<HqString*>* m_pSourceLines;

	size_t m_errorCount;
	size_t m_warningCount;
};

//----------------------------------------------------------------------------------------------------------------------

inline size_t ParserErrorListener::GetErrorCount() const
{
	return m_errorCount;
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t ParserErrorListener::GetWarningCount() const
{
	return m_warningCount;
}

//----------------------------------------------------------------------------------------------------------------------
