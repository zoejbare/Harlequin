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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "AstBaseNode.hpp"

#include "../detail/TokenSpan.hpp"

#include <memory>
#include <deque>
#include <string>

//----------------------------------------------------------------------------------------------------------------------

class AliasNode final
	: public AstBaseNode
{
public:

	typedef std::shared_ptr<AliasNode> Ptr;
	typedef std::deque<Ptr>            PtrDeque;

	inline static Ptr New()
	{
		return std::make_shared<AliasNode>();
	}

	virtual void Walk(SourceContext& srcCtx, AstBaseVisitor* pVisitor, bool visit = true) const override
	{
		_HQ_AST_NODE_WALK_PREAMBLE(AstBaseNode, srcCtx, pVisitor, visit);
	}

	std::string originalName;
	std::string aliasedName;

	detail::TokenSpan originalNameTokenSpan;
	detail::TokenSpan aliasedNameTokenSpan;
};

//----------------------------------------------------------------------------------------------------------------------
