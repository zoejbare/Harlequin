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
#include "SymbolTable.hpp"

#include "../parser/HarlequinBaseVisitor.h"

#include <list>

//----------------------------------------------------------------------------------------------------------------------

class SymbolVisitor
	: public HarlequinBaseVisitor
{
public:

	SymbolVisitor() = delete;
	SymbolVisitor(SymbolTable& symbolTable, IErrorNotifier* pErrorNotifier);

	virtual bool shouldVisitNextChild(antlr4::tree::ParseTree* pNode, const std::any& currentResult) override;

	virtual std::any visitUsingStmt(HarlequinParser::UsingStmtContext*) override;
	virtual std::any visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext*) override;

	virtual std::any visitNamespaceDecl(HarlequinParser::NamespaceDeclContext*) override;
	virtual std::any visitClassDecl(HarlequinParser::ClassDeclContext*) override;

	virtual std::any visitClassVarDecl(HarlequinParser::ClassVarDeclContext*) override;
	virtual std::any visitCtorDecl(HarlequinParser::CtorDeclContext *context) override;
	virtual std::any visitMethodDecl(HarlequinParser::MethodDeclContext *context) override;


private:

	ClassType prv_resolveClassType(const std::string&) const;
	ScopeType prv_resolveScopeType(const std::string&) const;

	SymbolTable* m_pSymbolTable;

	IErrorNotifier* m_pErrorNotifier;

	std::list<std::string> m_namespaceStack;
	std::list<ClassSymbol*> m_classStack;
};

//----------------------------------------------------------------------------------------------------------------------
