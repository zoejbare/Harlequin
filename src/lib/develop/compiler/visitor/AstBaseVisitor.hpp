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

#include "../detail/enum/VisitorAction.hpp"

//----------------------------------------------------------------------------------------------------------------------

class AstBaseNode;

class AliasNode;
class ClassNode;
class NamespaceNode;
class RootNode;
class UsingNode;

// TODO: Replace this with the real source file context.
class SourceContext;

//----------------------------------------------------------------------------------------------------------------------

class AstBaseVisitor
{
public:

	void* operator new(const size_t sizeInBytes)
	{
		return HqMemAlloc(sizeInBytes);
	}

	void operator delete(void* const pObject)
	{
		HqMemFree(pObject);
	}

	virtual detail::VisitorAction Visit(SourceContext&, const AstBaseNode*) { return detail::VisitorAction::Continue; }

	virtual detail::VisitorAction Visit(SourceContext& srcCtx, const AliasNode* const pNode) { return Visit(srcCtx, reinterpret_cast<const AstBaseNode*>(pNode)); }
	virtual detail::VisitorAction Visit(SourceContext& srcCtx, const ClassNode* const pNode) { return Visit(srcCtx, reinterpret_cast<const AstBaseNode*>(pNode)); }
	virtual detail::VisitorAction Visit(SourceContext& srcCtx, const NamespaceNode* const pNode) { return Visit(srcCtx, reinterpret_cast<const AstBaseNode*>(pNode)); }
	virtual detail::VisitorAction Visit(SourceContext& srcCtx, const RootNode* const pNode) { return Visit(srcCtx, reinterpret_cast<const AstBaseNode*>(pNode)); }
	virtual detail::VisitorAction Visit(SourceContext& srcCtx, const UsingNode* const pNode) { return Visit(srcCtx, reinterpret_cast<const AstBaseNode*>(pNode)); }
};

//----------------------------------------------------------------------------------------------------------------------
