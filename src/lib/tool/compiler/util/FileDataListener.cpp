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
#include "ParserErrorHandler.hpp"

#include "../SourceFile.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

FileDataListener::FileDataListener(HqSourceFileHandle hSrcFile, ParserErrorHandler& errorHandler)
	: m_hSrcFile(hSrcFile)
	, m_pErrorHandler(&errorHandler)
{
	assert(hSrcFile != HQ_SOURCE_FILE_HANDLE_NULL);
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::enterUsingStmt(HarlequinParser::UsingStmtContext* const pCtx)
{
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
			// TODO: Log a compile error.
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
	const std::string shortName = pCtx->qualifiedId()->getText();
	const std::string qualifiedName = (m_namespaceStack.size() > 0)
		? (m_namespaceStack.back() + "." + shortName) 
		: shortName;

	m_namespaceStack.push_back(qualifiedName);
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::exitNamespaceDecl(HarlequinParser::NamespaceDeclContext*)
{
	m_namespaceStack.pop_back();
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::enterClassDecl(HarlequinParser::ClassDeclContext* const pCtx)
{
	const std::string shortName = pCtx->Id()->getText();
	const std::string qualifiedName = (m_namespaceStack.size() > 0)
		? (m_namespaceStack.back() + "." + shortName) 
		: shortName;

	// TODO: Setup the class declaration and push it to the class stack.
}

//----------------------------------------------------------------------------------------------------------------------

void FileDataListener::exitClassDecl(HarlequinParser::ClassDeclContext*)
{
	// TODO: Pop the class stack
}

//----------------------------------------------------------------------------------------------------------------------
