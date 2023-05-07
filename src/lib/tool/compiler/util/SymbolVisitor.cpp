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

SymbolVisitor::SymbolVisitor(HqSourceFileHandle hSrcFile, IErrorNotifier* const pErrorNotifier)
	: m_hSrcFile(hSrcFile)
	, m_pErrorNotifier(pErrorNotifier)
{
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);
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

	HqString* const pNamespace = HqString::Create(qualifiedNamespace.c_str());

	// Check to see if this namespace is already being used (we can ignore duplicate namespaces).
	if(!HqSourceFile::NamespaceMap::Contains(m_hSrcFile->namespaces, pNamespace))
	{
		// Add a reference to the namespace string since it will be used as the map key.
		HqString::AddRef(pNamespace);

		HqSourceFile::NamespaceMap::Insert(
			m_hSrcFile->namespaces, 
			pNamespace,
			false
		);
	}

	// Release the extra string reference on the namespace now that we're done with it.
	HqString::Release(pNamespace);

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

	HqString* const pQualifiedName = HqString::Create(qualifiedClassName.c_str());

	// Map each alias listed in the statement.
	for(auto& alias : aliasNames)
	{
		const std::string aliasedClassName = alias->getText();

		HqString* const pAliasedName = HqString::Create(aliasedClassName.c_str());

		// Check if the class alias has not already been declared.
		// If it has, it means there is an ambiguous reference to
		// that class name.
		if(!HqSourceFile::ClassAliasMap::Contains(m_hSrcFile->classAliases, pAliasedName))
		{
			// Add references to the strings going into the map.
			HqString::AddRef(pQualifiedName);
			HqString::AddRef(pAliasedName);

			HqSourceFile::ClassAliasMap::Insert(
				m_hSrcFile->classAliases, 
				pAliasedName,
				pQualifiedName
			);
		}
		else
		{
			// Report the duplicate alias name as an error.
			m_pErrorNotifier->Report(
				MessageCode::ErrorDuplicateAlias,
				alias->getSymbol(),
				"duplicate class alias '%s'",
				pAliasedName->data
			);
		}

		// Release the class alias string.
		HqString::Release(pAliasedName);
	}

	// Release the qualified class name string.
	HqString::Release(pQualifiedName);

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
		? (std::string(m_namespaceStack.back()->data) + "." + shortName) 
		: shortName;

	HqString* const pQualifiedName = HqString::Create(qualifiedName.c_str());

	m_namespaceStack.push_back(pQualifiedName);

	std::any result = visitChildren(pCtx);

	HqString::Release(m_namespaceStack.back());
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

	HqString* const pNamespace = isChildOfNamespace
		? m_namespaceStack.back()
		: nullptr;

	const std::string shortName = pCtx->Id()->getText();
	const std::string qualifiedName = isChildOfNamespace
		? (std::string(pNamespace->data) + "." + shortName) 
		: shortName;

	HqString* const pQualifiedName = HqString::Create(qualifiedName.c_str());
	HqString* const pShortName = HqString::Create(shortName.c_str());

	std::any result = defaultResult();

	// Check to see if this class has already been defined.
	if(!HqSourceFile::ClassMap::Contains(m_hSrcFile->classes, pQualifiedName))
	{
		const bool isStatic = (pCtx->StaticKw() != nullptr);

		const ClassType classType = prv_resolveClassType(pCtx->classType()->getText());
		const ScopeType scopeType = prv_resolveScopeType(pCtx->scopeModifier()->getText());

		if(classType == ClassType::Class || (classType == ClassType::Interface && !isStatic))
		{
			auto* const pClassInheritanceCtx = pCtx->classInheritance();

			// Create new class metadata.
			ClassMetaData* const pClass = new ClassMetaData();

			// Initialize the class data structures.
			ClassMetaData::StringArray::Initialize(pClass->implements);
			ClassMetaData::StringArray::Initialize(pClass->extends);

			// Handle the class inheritance.
			if(pClassInheritanceCtx)
			{
				auto* const pClassImplsCtx = pClassInheritanceCtx->classImpls();
				auto* const pClassExtendsCtx = pClassInheritanceCtx->classExtends();

				if(pClassImplsCtx)
				{
					const size_t length = pClassImplsCtx->qualifiedId().size();

					ClassMetaData::StringArray::Reserve(pClass->implements, length);
					pClass->implements.count = length;

					// Extract each interface name to implement.
					for(size_t i = 0; i < length; ++i)
					{
						pClass->implements.pData[i] = HqString::Create(pClassImplsCtx->qualifiedId(i)->getText().c_str());
					}
				}

				if(pClassExtendsCtx)
				{
					ClassMetaData::StringArray::Reserve(pClass->extends, 1);

					// Extract the class name to extend.
					pClass->extends.count = 1;
					pClass->extends.pData[0] = HqString::Create(pClassExtendsCtx->qualifiedId()->getText().c_str());
				}
			}

			// Fill in the class properties.
			pClass->pQualifiedName = pQualifiedName;
			pClass->pShortName = pShortName;
			pClass->pNamespace = pNamespace;
			pClass->type = classType;
			pClass->scope = scopeType;
			pClass->isStatic = isStatic;

			// Add string references for the class metadata.
			HqString::AddRef(pClass->pQualifiedName);
			HqString::AddRef(pClass->pShortName);
			HqString::AddRef(pClass->pNamespace);
		
			// Add a reference to the namespace string since it will be used as the map key.
			HqString::AddRef(pQualifiedName);

			HqSourceFile::ClassMap::Insert(
				m_hSrcFile->classes, 
				pQualifiedName,
				pClass
			);

			m_classStack.push_back(pClass);

			result = visitChildren(pCtx);

			m_classStack.pop_back();
		}
		else
		{
			// Report the invalid scenario of a static interface declaration.
			m_pErrorNotifier->Report(
				MessageCode::ErrorStaticInterface,
				pCtx->StaticKw()->getSymbol(),
				"illegal static interface declaration '%s'",
				pQualifiedName->data
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
			pQualifiedName->data
		);
	}

	// Release the extra string references.
	HqString::Release(pQualifiedName);
	HqString::Release(pShortName);

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

	HqString* const pClassName = m_classStack.back()->pQualifiedName;

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
		const std::string qualifiedName = std::string(pClassName->data) + "." + shortName;

		HqString* const pQualifiedName = HqString::Create(qualifiedName.c_str());

		if(!HqSourceFile::ClassVarMap::Contains(m_hSrcFile->classVars, pQualifiedName))
		{
			ClassVarMetaData* const pClassVar = new ClassVarMetaData();

			pClassVar->scope = scopeType;
			pClassVar->var.type = valueType;
			pClassVar->var.isStatic = isStatic;
			pClassVar->var.isConst = isConst;

			// Cache the fully qualified name of the variable.
			pClassVar->var.pName = pQualifiedName;
			HqString::AddRef(pQualifiedName);

			// Store the type name of the variable only if it's an object.
			pClassVar->var.pTypeName = (valueType == HQ_VALUE_TYPE_OBJECT)
				? HqString::Create(typeName.c_str())
				: nullptr;

			// Add a reference to the map key.
			HqString::AddRef(pQualifiedName);

			HqSourceFile::ClassVarMap::Insert(
				m_hSrcFile->classVars, 
				pQualifiedName,
				pClassVar
			);
		}
		else
		{
			// Report the duplicate variable name as an error.
			m_pErrorNotifier->Report(
				MessageCode::ErrorDuplicateVar,
				pVarDefCtx->Id()->getSymbol(),
				"duplicate class variable '%s'",
				pQualifiedName->data
			);
		}

		HqString::Release(pQualifiedName);
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
