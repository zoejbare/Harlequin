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

#include "../SourceContext.hpp"

#include "../ast/RootNode.hpp"
#include "../autogen/HarlequinBaseVisitor.h"

#include <list>

//----------------------------------------------------------------------------------------------------------------------

class AntlrAstBuilder final
	: protected HarlequinBaseVisitor
{
public:

	static RootNode::Ptr Run(SourceContext& srcCtx, antlr4::tree::ParseTree* pParseTree);


private:

	AntlrAstBuilder() = delete;
	explicit AntlrAstBuilder(SourceContext&);

	virtual std::any visitRoot(HarlequinParser::RootContext*) override;

	virtual std::any visitUsingStmt(HarlequinParser::UsingStmtContext*) override;
	virtual std::any visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext*) override;

	virtual std::any visitNamespaceDecl(HarlequinParser::NamespaceDeclContext*) override;
	virtual std::any visitClassDecl(HarlequinParser::ClassDeclContext*) override;

	virtual std::any visitClassVarDeclStmt(HarlequinParser::ClassVarDeclStmtContext*) override;

	void _getClassType(HarlequinParser::ClassTypeContext*, detail::ClassType&, detail::TokenSpan&);
	void _getAccessSpec(HarlequinParser::AccessBaseSpecifierContext*, detail::AccessType&, detail::TokenSpan&);
	void _getAccessSpec(HarlequinParser::AccessSpecifierContext*, detail::AccessType&, detail::TokenSpan&, detail::TypeName::Deque&);
	void _getStorageSpec(HarlequinParser::StorageSpecifierContext*, detail::StorageType&, detail::TokenSpan&);
	void _getConstQualifier(HarlequinParser::ConstQualifierContext*, detail::ConstType&, detail::TokenSpan&);
	void _getVarType(HarlequinParser::TypeNameDeclContext*, detail::VarType&, detail::TokenSpan&, detail::ArrayType&, detail::TokenSpan&);

	static antlr4::Token* _getClassTypeToken(HarlequinParser::ClassTypeContext*);
	static antlr4::Token* _getAccessSpecToken(HarlequinParser::AccessBaseSpecifierContext*);
	static antlr4::Token* _getStorageSpecToken(HarlequinParser::StorageSpecifierContext*);

	SourceContext* m_pSrcCtx;

	std::list<std::string> m_namespaceStack;
	std::list<std::string> m_classStack;
};

//----------------------------------------------------------------------------------------------------------------------
