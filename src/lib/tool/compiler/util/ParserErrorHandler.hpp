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

#include "../../../Harlequin.h"

#include <ANTLRErrorListener.h>

#include <list>
#include <string>

//----------------------------------------------------------------------------------------------------------------------

class ParserErrorHandler
	: public antlr4::ANTLRErrorListener
{
public:

	ParserErrorHandler();

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

	virtual void reportAmbiguity(
		antlr4::Parser* pRecognizer,
		const antlr4::dfa::DFA& dfa,
		size_t startIndex,
		size_t stopIndex,
		bool exact,
		const antlrcpp::BitSet& ambigAlts,
		antlr4::atn::ATNConfigSet* pConfigs
	) override;

	virtual void reportAttemptingFullContext(
		antlr4::Parser* pRecognizer, 
		const antlr4::dfa::DFA& dfa, 
		size_t startIndex, 
		size_t stopIndex,
		const antlrcpp::BitSet& conflictingAlts,
		antlr4::atn::ATNConfigSet* pConfigs
	) override;

	virtual void reportContextSensitivity(
		antlr4::Parser* pRecognizer,
		const antlr4::dfa::DFA& dfa,
		size_t startIndex,
		size_t stopIndex,
		size_t prediction,
		antlr4::atn::ATNConfigSet* pConfigs
	) override;


private:

	size_t m_errorCount;
	size_t m_warningCount;
};

//----------------------------------------------------------------------------------------------------------------------

inline ParserErrorHandler::ParserErrorHandler()
	: m_errorCount(0)
	, m_warningCount(0)
{
}

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
