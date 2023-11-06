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

#include "IntermediateCode.hpp"
#include "ParserErrorHandler.hpp"

#include "../SourceFile.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

IntermediateCode::IntermediateCode(ParserErrorHandler* const pErrorHandler)
	: m_pErrorHandler(pErrorHandler)
	, m_symbolTable()
	, m_namespaceStack()
	, m_classStack()
{
}

//----------------------------------------------------------------------------------------------------------------------

IntermediateCode* IntermediateCode::Resolve(
	antlr4::tree::ParseTree* const pParseTree, 
	ParserErrorHandler* const pErrorHandler)
{
	IntermediateCode* pOutput = new IntermediateCode(pErrorHandler);

	// Walk the parse tree to discover the source file symbols and generate
	// the source code's intermediate representation.
	pOutput->visit(pParseTree);

	// Dispose of the code IR object if there were errors resolving the parse tree.
	if(pErrorHandler->EncounteredError())
	{
		delete pOutput;
		pOutput = nullptr;
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

bool IntermediateCode::shouldVisitNextChild(antlr4::tree::ParseTree* const pNode, const std::any& currentResult)
{
	(void) pNode;
	(void) currentResult;

	return !m_pErrorHandler->EncounteredError();
}

//----------------------------------------------------------------------------------------------------------------------

std::any IntermediateCode::visitUsingStmt(HarlequinParser::UsingStmtContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const std::string qualifiedNamespace = pCtx->qualifiedId()->getText();

	// Check to see if this namespace is already being used (we can ignore duplicate namespaces).
	if(!m_symbolTable.namespaceImports.count(qualifiedNamespace))
	{
		m_symbolTable.namespaceImports.insert(qualifiedNamespace);
	}

	return visitChildren(pCtx);
}

//----------------------------------------------------------------------------------------------------------------------

std::any IntermediateCode::visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const std::string qualifiedClassName = pCtx->qualifiedId()->getText();
	const auto aliasNames = pCtx->Id();

	// Map each alias listed in the statement.
	for(auto& alias : aliasNames)
	{
		const std::string aliasedClassName = alias->getText();

		// Check if the class alias has not already been declared.
		// If it has, it means there is an ambiguous reference to
		// that class name.
		if(!m_symbolTable.classAliases.count(aliasedClassName))\
		{
			m_symbolTable.classAliases.emplace(aliasedClassName, qualifiedClassName);
		}
		else
		{
			// Report the duplicate alias name as an error.
			m_pErrorHandler->Report(
				MessageCode::ErrorDuplicateAlias,
				alias->getSymbol(),
				"duplicate class alias '%s'",
				aliasedClassName.c_str()
			);
		}
	}

	return visitChildren(pCtx);
}

//----------------------------------------------------------------------------------------------------------------------

std::any IntermediateCode::visitNamespaceDecl(HarlequinParser::NamespaceDeclContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const bool isChildOfNamespace = (m_namespaceStack.size() > 0);

	const std::string shortName = pCtx->qualifiedId()->getText();
	const std::string qualifiedName = isChildOfNamespace
		? m_namespaceStack.back() + "." + shortName
		: shortName;

	m_namespaceStack.push_back(qualifiedName);

	std::any result = visitChildren(pCtx);

	m_namespaceStack.pop_back();

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

std::any IntermediateCode::visitClassDecl(HarlequinParser::ClassDeclContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const bool isChildOfNamespace = (m_namespaceStack.size() > 0);

	const std::string currentNamespace = isChildOfNamespace
		? m_namespaceStack.back()
		: std::string();

	const std::string shortName = pCtx->Id()->getText();
	const std::string qualifiedName = isChildOfNamespace
		? currentNamespace + "." + shortName
		: shortName;

	// Check to see if this class has already been defined.
	if(!m_symbolTable.classSymbols.count(qualifiedName))
	{
		const bool isStatic = (pCtx->StaticKw() != nullptr);

		const ClassType classType = prv_resolveClassType(pCtx->classType()->getText());
		const ScopeType scopeType = prv_resolveScopeType(pCtx->scopeModifier()->getText());

		if(classType == ClassType::Class || (classType == ClassType::Interface && !isStatic))
		{
			auto* const pClassInheritanceCtx = pCtx->classInheritance();

			// Create new class symbol.
			ClassSymbol* const pClass = new ClassSymbol();

			// Handle the class inheritance.
			if(pClassInheritanceCtx)
			{
				auto* const pClassImplsCtx = pClassInheritanceCtx->classImpls();
				auto* const pClassExtendsCtx = pClassInheritanceCtx->classExtends();

				if(pClassImplsCtx)
				{
					const size_t length = pClassImplsCtx->qualifiedId().size();

					pClass->implements.resize(length);

					// Extract each interface name to implement.
					for(size_t i = 0; i < length; ++i)
					{
						pClass->implements[i] = pClassImplsCtx->qualifiedId(i)->getText();
					}
				}

				if(pClassExtendsCtx)
				{
					pClass->extends.resize(1);

					// Extract the class name to extend.
					pClass->extends[0] = pClassExtendsCtx->qualifiedId()->getText();
				}
			}

			// Fill in the class properties.
			pClass->qualifiedName = qualifiedName;
			pClass->shortName = shortName;
			pClass->namespaceName = currentNamespace;
			pClass->type = classType;
			pClass->scope = scopeType;
			pClass->isStatic = isStatic;

			// Add the class to the symbol table.
			m_symbolTable.classSymbols.emplace(qualifiedName, pClass);

			m_classStack.push_back(pClass);
		}
		else
		{
			// Report the invalid scenario of a static interface declaration.
			m_pErrorHandler->Report(
				MessageCode::ErrorStaticInterface,
				pCtx->StaticKw()->getSymbol(),
				"illegal static interface declaration '%s'",
				qualifiedName.c_str()
			);
		}
	}
	else
	{
		// Report the duplicate class declaration as an error.
		m_pErrorHandler->Report(
			MessageCode::ErrorDuplicateClass,
			pCtx->Id()->getSymbol(),
			"duplicate class declaration '%s'",
			qualifiedName.c_str()
		);
	}

	std::any result = defaultResult();

	if(!m_pErrorHandler->EncounteredError())
	{
		result = visitChildren(pCtx);

		m_classStack.pop_back();
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

std::any IntermediateCode::visitClassVarDecl(HarlequinParser::ClassVarDeclContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const std::string& className = m_classStack.back()->qualifiedName;

	auto* const pScopeModCtx = pCtx->scopeModifier();
	auto* const pVarDeclCtx = pCtx->varDecl();
	auto* const pVarModCtx = pVarDeclCtx->varModifier();
	auto* const pTypeNameCtx = pVarDeclCtx->typeNameDecl();
	const auto varDefs = pVarDeclCtx->varDef();

	const bool isStatic = (pVarModCtx && (pVarModCtx->StaticKw() != nullptr));
	const bool isConst = (pVarModCtx && (pVarModCtx->ConstKw() != nullptr));

	// When no explicit scope is present, the default is private.
	const ScopeType scopeType = pScopeModCtx
		? prv_resolveScopeType(pScopeModCtx->getText())
		: ScopeType::Private;

	const std::string typeName = pTypeNameCtx->getText();
	int valueType = HQ_VALUE_TYPE_OBJECT;

	if(typeName == "int8")
	{
		valueType = HQ_VALUE_TYPE_INT8;
	}
	else if(typeName == "int16")
	{
		valueType = HQ_VALUE_TYPE_INT16;
	}
	else if(typeName == "int32")
	{
		valueType = HQ_VALUE_TYPE_INT32;
	}
	else if(typeName == "int64")
	{
		valueType = HQ_VALUE_TYPE_INT64;
	}
	else if(typeName == "uint8")
	{
		valueType = HQ_VALUE_TYPE_UINT8;
	}
	else if(typeName == "uint16")
	{
		valueType = HQ_VALUE_TYPE_UINT16;
	}
	else if(typeName == "uint32")
	{
		valueType = HQ_VALUE_TYPE_UINT32;
	}
	else if(typeName == "uint64")
	{
		valueType = HQ_VALUE_TYPE_UINT64;
	}
	else if(typeName == "float32")
	{
		valueType = HQ_VALUE_TYPE_FLOAT32;
	}
	else if(typeName == "float64")
	{
		valueType = HQ_VALUE_TYPE_FLOAT64;
	}
	else if(typeName == "bool")
	{
		valueType = HQ_VALUE_TYPE_BOOL;
	}
	else if(typeName == "string")
	{
		valueType = HQ_VALUE_TYPE_STRING;
	}
	else if(typeName == "native")
	{
		valueType = HQ_VALUE_TYPE_NATIVE;
	}
	else
	{
		// TODO: Handle arrays
		// TODO: Handle templates?
	}

	// TODO: Make this generic for all types of variables, not just class variables.
	for(auto* const pVarDefCtx : varDefs)
	{
		const std::string shortName = pVarDefCtx->Id()->getText();
		const std::string qualifiedName = className + "." + shortName;

		if(!m_symbolTable.classVarSymbols.count(qualifiedName))
		{
			ClassVarSymbol* const pClassVar = new ClassVarSymbol();

			pClassVar->scope = scopeType;
			pClassVar->var.type = valueType;
			pClassVar->var.isStatic = isStatic;
			pClassVar->var.isConst = isConst;

			// Cache the fully qualified name of the variable.
			pClassVar->var.name = qualifiedName;

			// Store the type name of the variable only if it's an object.
			if(valueType == HQ_VALUE_TYPE_OBJECT)
			{
				pClassVar->var.className = typeName;
			}

			m_symbolTable.classVarSymbols.emplace(qualifiedName, pClassVar);
		}
		else
		{
			// Report the duplicate variable name as an error.
			m_pErrorHandler->Report(
				MessageCode::ErrorDuplicateVar,
				pVarDefCtx->Id()->getSymbol(),
				"duplicate class variable '%s'",
				qualifiedName.c_str()
			);
		}
	}

	return defaultResult();
}

//----------------------------------------------------------------------------------------------------------------------

std::any IntermediateCode::visitCtorDecl(HarlequinParser::CtorDeclContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	// TODO: Implement me
	(void) pCtx;

	return defaultResult();
}

//----------------------------------------------------------------------------------------------------------------------

std::any IntermediateCode::visitMethodDecl(HarlequinParser::MethodDeclContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	// TODO: Implement me
	(void) pCtx;

	return defaultResult();
}

//----------------------------------------------------------------------------------------------------------------------

inline ClassType IntermediateCode::prv_resolveClassType(const std::string& text) const
{
	return (text == "interface")
		? ClassType::Interface
		: ClassType::Class;
}

//----------------------------------------------------------------------------------------------------------------------

inline ScopeType IntermediateCode::prv_resolveScopeType(const std::string& text) const
{
	return (text == "public")
		? ScopeType::Public
		: (text == "protected")
			? ScopeType::Protected
			: ScopeType::Private;
}

//----------------------------------------------------------------------------------------------------------------------
