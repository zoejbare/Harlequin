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
#include <string_view>

//----------------------------------------------------------------------------------------------------------------------

class AntlrAstBuilder final
	: protected HarlequinBaseVisitor
{
public:

	static RootNode::Ptr Run(SourceContext& srcCtx, antlr4::tree::ParseTree* pParseTree);


private:

	typedef std::list<std::string_view> StringStack;

	AntlrAstBuilder() = delete;
	explicit AntlrAstBuilder(SourceContext&);

	virtual std::any visitRoot(HarlequinParser::RootContext*) override;

	virtual std::any visitUsingStmt(HarlequinParser::UsingStmtContext*) override;
	virtual std::any visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext*) override;

	virtual std::any visitNamespaceDecl(HarlequinParser::NamespaceDeclContext*) override;
	virtual std::any visitClassDecl(HarlequinParser::ClassDeclContext*) override;

	virtual std::any visitClassVarDeclStmt(HarlequinParser::ClassVarDeclStmtContext*) override;

	void _getQualifiedName(HarlequinParser::QualifiedIdContext*, detail::QualifiedTypeName&) const;
	void _getQualifiedName(antlr4::tree::TerminalNode*, detail::QualifiedTypeName&) const;

	void _getAccessDecl(HarlequinParser::AccessBaseSpecifierContext*, detail::AccessDecl&) const;
	void _getAccessDecl(HarlequinParser::AccessSpecifierContext*, detail::AccessDecl&, detail::TypeName::Deque&) const;
	void _getClassDecl(HarlequinParser::ClassTypeContext*, detail::ClassDecl&) const;
	void _getStorageDecl(HarlequinParser::StorageSpecifierContext*, detail::StorageDecl&) const;
	void _getConstDecl(HarlequinParser::ConstQualifierContext*, detail::ConstDecl&) const;
	void _getVarDecl(HarlequinParser::TypeNameDeclContext*, detail::VarDecl&, detail::ArrayDecl&) const;

	static antlr4::Token* _getClassTypeToken(HarlequinParser::ClassTypeContext*);
	static antlr4::Token* _getAccessSpecToken(HarlequinParser::AccessBaseSpecifierContext*);
	static antlr4::Token* _getStorageSpecToken(HarlequinParser::StorageSpecifierContext*);

	SourceContext* m_pSrcCtx;

	StringStack m_prefixStack;
};

//----------------------------------------------------------------------------------------------------------------------