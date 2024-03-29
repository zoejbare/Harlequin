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

#include "AntlrErrorHandler.hpp"

#include "../SourceContext.hpp"

#include <antlr4-runtime.h>

#include <assert.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------

AntlrErrorHandler::AntlrErrorHandler(SourceContext& srcCtx)
	: m_pSrcCtx(&srcCtx)
{
	assert(m_pSrcCtx != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

void AntlrErrorHandler::syntaxError(
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
	m_pSrcCtx->Report(detail::MessageCode::ErrorSyntax, detail::TokenSpan::Extract(pOffendingSymbol), msg);
}

//----------------------------------------------------------------------------------------------------------------------
