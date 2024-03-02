//
// Copyright (c) 2024, Zoe J. Bare
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

#include "TokenSpan.hpp"

#include <antlr4-runtime.h>

//----------------------------------------------------------------------------------------------------------------------

detail::TokenSpan detail::TokenSpan::Default()
{
	TokenSpan output;
	output.lineNumber = 0;
	output.startIndex = 0;
	output.stopIndex = 0;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

detail::TokenSpan detail::TokenSpan::Extract(const antlr4::Token* const pToken, const TokenSpanFlags flags)
{

	if(pToken)
	{
		const bool withSourceText = (flags & TOKEN_SPAN_FLAG_WITH_SOURCE_TEXT) > 0;

		TokenSpan output;
		output.sourceName = pToken->getTokenSource()->getSourceName();
		output.lineNumber = pToken->getLine();
		output.startIndex = withSourceText ? pToken->getStartIndex() : 0;
		output.stopIndex = withSourceText ? pToken->getStopIndex() : 0;

		return output;
	}

	return Default();
}

//----------------------------------------------------------------------------------------------------------------------
