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

#include "ParserErrorHandler.hpp"
#include "SymbolTable.hpp"

#include "../parser/HarlequinBaseVisitor.h"

#include <list>

//----------------------------------------------------------------------------------------------------------------------

class SymbolTableGenerator final
	: protected HarlequinBaseVisitor
{
public:

	static void Run(
		antlr4::tree::ParseTree* pParseTree,
		ParserErrorHandler* pErrorHandler,
		SymbolTable* pSymbolTable);


private:

	SymbolTableGenerator(ParserErrorHandler*, SymbolTable*);

	virtual bool shouldVisitNextChild(antlr4::tree::ParseTree*, const std::any&) override;

	virtual std::any visitUsingStmt(HarlequinParser::UsingStmtContext*) override;
	virtual std::any visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext*) override;

	virtual std::any visitNamespaceDecl(HarlequinParser::NamespaceDeclContext*) override;
	virtual std::any visitClassDecl(HarlequinParser::ClassDeclContext*) override;

	virtual std::any visitClassVarDeclStmt(HarlequinParser::ClassVarDeclStmtContext*) override;
	virtual std::any visitCtorDecl(HarlequinParser::CtorDeclContext*) override;
	virtual std::any visitMethodDecl(HarlequinParser::MethodDeclContext*) override;

	void _resolveAccessSpecifier(
		detail::AccessType&,
		detail::StringArray&,
		HarlequinParser::AccessSpecifierContext*);
	void _resolveVariableCommonData(
		detail::VariableBase&, 
		HarlequinParser::VarDeclContext*, 
		HarlequinParser::ConstQualifierContext*);
	void _resolveQualifiedIdArray(detail::StringArray&, const std::vector<HarlequinParser::QualifiedIdContext*>&);

	ParserErrorHandler* m_pErrorHandler;
	SymbolTable* m_pSymbolTable;

	std::list<NamespaceSymbol*> m_namespaceStack;
	std::list<ClassSymbol*> m_classStack;
	std::list<ClassVarSymbol*> m_classVarStack;
};

//----------------------------------------------------------------------------------------------------------------------
