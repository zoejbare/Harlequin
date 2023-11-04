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

#include "SymbolVisitor.hpp"
#include "ParserErrorListener.hpp"

#include "../SourceFile.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

SymbolVisitor::SymbolVisitor(SymbolTable& symbolTable, IErrorNotifier* const pErrorNotifier)
	: m_pSymbolTable(&symbolTable)
	, m_pErrorNotifier(pErrorNotifier)
{
	assert(pErrorNotifier != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

bool SymbolVisitor::shouldVisitNextChild(antlr4::tree::ParseTree* const pNode, const std::any& currentResult)
{
	(void) pNode;
	(void) currentResult;

	return !m_pErrorNotifier->EncounteredError();
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolVisitor::visitUsingStmt(HarlequinParser::UsingStmtContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const std::string qualifiedNamespace = pCtx->qualifiedId()->getText();

	// Check to see if this namespace is already being used (we can ignore duplicate namespaces).
	if(!m_pSymbolTable->namespaceImports.count(qualifiedNamespace))
	{
		m_pSymbolTable->namespaceImports.insert(qualifiedNamespace);
	}

	return visitChildren(pCtx);
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolVisitor::visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
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
		if(!m_pSymbolTable->classAliases.count(aliasedClassName))\
		{
			m_pSymbolTable->classAliases.emplace(aliasedClassName, qualifiedClassName);
		}
		else
		{
			// Report the duplicate alias name as an error.
			m_pErrorNotifier->Report(
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

std::any SymbolVisitor::visitNamespaceDecl(HarlequinParser::NamespaceDeclContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
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

std::any SymbolVisitor::visitClassDecl(HarlequinParser::ClassDeclContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
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
	if(!m_pSymbolTable->classSymbols.count(qualifiedName))
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
			m_pSymbolTable->classSymbols.emplace(qualifiedName, pClass);

			m_classStack.push_back(pClass);
		}
		else
		{
			// Report the invalid scenario of a static interface declaration.
			m_pErrorNotifier->Report(
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
		m_pErrorNotifier->Report(
			MessageCode::ErrorDuplicateClass,
			pCtx->Id()->getSymbol(),
			"duplicate class declaration '%s'",
			qualifiedName.c_str()
		);
	}

	std::any result = defaultResult();

	if(!m_pErrorNotifier->EncounteredError())
	{
		result = visitChildren(pCtx);

		m_classStack.pop_back();
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolVisitor::visitClassVarDecl(HarlequinParser::ClassVarDeclContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const std::string& className = m_classStack.back()->qualifiedName;

	auto* const pScopeModCtx = pCtx->scopeModifier();
	auto* const pVarDeclCtx = pCtx->varDecl();
	auto* const pVarModCtx = pVarDeclCtx->varModifier();
	auto* const pTypeNameCtx = pVarDeclCtx->typeName();
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

		if(!m_pSymbolTable->classVarSymbols.count(qualifiedName))
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

			m_pSymbolTable->classVarSymbols.emplace(qualifiedName, pClassVar);
		}
		else
		{
			// Report the duplicate variable name as an error.
			m_pErrorNotifier->Report(
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

std::any SymbolVisitor::visitCtorDecl(HarlequinParser::CtorDeclContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	// TODO: Implement me
	(void) pCtx;

	return defaultResult();
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolVisitor::visitMethodDecl(HarlequinParser::MethodDeclContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	// TODO: Implement me
	(void) pCtx;

	return defaultResult();
}

//----------------------------------------------------------------------------------------------------------------------

inline ClassType SymbolVisitor::prv_resolveClassType(const std::string& text) const
{
	return (text == "interface")
		? ClassType::Interface
		: ClassType::Class;
}

//----------------------------------------------------------------------------------------------------------------------

inline ScopeType SymbolVisitor::prv_resolveScopeType(const std::string& text) const
{
	return (text == "public")
		? ScopeType::Public
		: (text == "protected")
			? ScopeType::Protected
			: ScopeType::Private;
}

//----------------------------------------------------------------------------------------------------------------------
