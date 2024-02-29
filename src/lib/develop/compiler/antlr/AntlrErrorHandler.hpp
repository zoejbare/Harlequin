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

#include "../detail/MessageCode.hpp"
#include "../detail/TokenSpan.hpp"

#include <BaseErrorListener.h>

#include <stdarg.h>

//----------------------------------------------------------------------------------------------------------------------

class SourceContext;

class AntlrErrorHandler final
	: protected antlr4::BaseErrorListener
{
public:

	AntlrErrorHandler() = delete;
	explicit AntlrErrorHandler(SourceContext& srcCtx);

private:

	virtual void syntaxError(
		antlr4::Recognizer* pRecognizer,
		antlr4::Token* pOffendingSymbol,
		size_t line,
		size_t charPositionInLine,
		const std::string& msg,
		std::exception_ptr e
	) override;

	SourceContext* m_pSrcCtx;
};

//----------------------------------------------------------------------------------------------------------------------