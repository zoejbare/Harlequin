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

#include "AliasNode.hpp"
#include "NamespaceNode.hpp"
#include "UsingNode.hpp"

//----------------------------------------------------------------------------------------------------------------------

class RootNode final
	: public AstBaseNode
{
public:

	typedef std::shared_ptr<RootNode> Ptr;

	inline static Ptr New()
	{
		return std::make_shared<RootNode>();
	}

	virtual void Walk(SourceContext& srcCtx, AstBaseVisitor* const pVisitor, const bool visit = true) const override
	{
		_HQ_AST_NODE_WALK_PREAMBLE(AstBaseNode, srcCtx, pVisitor, visit);

		// Visit all 'using' nodes.
		for(const auto& pNode : imports)
		{
			pNode->Walk(srcCtx, pVisitor);
		}

		// Visit all 'using alias' nodes.
		for(const auto& pNode : aliases)
		{
			pNode->Walk(srcCtx, pVisitor);
		}

		// Visit all 'namespace' nodes.
		for(const auto& pNode : namespaces)
		{
			pNode->Walk(srcCtx, pVisitor);
		}
	}

	UsingNode::PtrDeque imports;
	AliasNode::PtrDeque aliases;
	NamespaceNode::PtrDeque namespaces;
};

//----------------------------------------------------------------------------------------------------------------------
