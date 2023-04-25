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

#include "FileDataListener.hpp"
#include "ParserErrorListener.hpp"

#include "../SourceFile.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

FileDataListener::FileDataListener(HqSourceFileHandle hSrcFile, IErrorNotifier* const pErrorNotifier)
	: m_hSrcFile(hSrcFile)
	, m_pErrorNotifier(pErrorNotifier)
{
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);
	assert(pErrorNotifier != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::enterUsingStmt(HarlequinParser::UsingStmtContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return;
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
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::enterUsingAliasStmt(HarlequinParser::UsingAliasStmtContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return;
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
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::enterNamespaceDecl(HarlequinParser::NamespaceDeclContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return;
	}

	const bool isChildOfNamespace = (m_namespaceStack.size() > 0);

	const std::string shortName = pCtx->qualifiedId()->getText();
	const std::string qualifiedName = isChildOfNamespace
		? (std::string(m_namespaceStack.back()->data) + "." + shortName) 
		: shortName;

	HqString* const pQualifiedName = HqString::Create(qualifiedName.c_str());

	m_namespaceStack.push_back(pQualifiedName);
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::exitNamespaceDecl(HarlequinParser::NamespaceDeclContext*)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return;
	}

	HqString::Release(m_namespaceStack.back());
	m_namespaceStack.pop_back();
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::enterClassDecl(HarlequinParser::ClassDeclContext* const pCtx)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return;
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

	// Check to see if this class has already been defined.
	if(!HqSourceFile::ClassMetaDataMap::Contains(m_hSrcFile->classes, pQualifiedName))
	{
		const bool isStatic = (pCtx->StaticKw() != nullptr);

		const std::string classTypeStr = pCtx->classType()->getText();
		const std::string scopeTypeStr = pCtx->scopeModifier()->getText();

		const ClassType classType = (classTypeStr == "interface")
			? ClassType::Interface
			: ClassType::Class;
		const ScopeType scopeType = (scopeTypeStr == "public")
			? ScopeType::Public
			: (scopeTypeStr == "protected")
				? ScopeType::Protected
				: ScopeType::Private;

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

			HqSourceFile::ClassMetaDataMap::Insert(
				m_hSrcFile->classes, 
				pQualifiedName,
				pClass
			);

			m_classStack.push_back(pClass);
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
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::exitClassDecl(HarlequinParser::ClassDeclContext*)
{
	if(m_pErrorNotifier->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return;
	}

	m_classStack.pop_back();
}

//----------------------------------------------------------------------------------------------------------------------
