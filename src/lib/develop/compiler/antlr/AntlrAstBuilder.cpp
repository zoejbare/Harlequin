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

#include "AntlrAstBuilder.hpp"

#include "../CompilerUtil.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

#define _HQ_ANTLR_AST_BUILDER_PREAMBLE() \
	if(m_pSrcCtx->EncounteredErrors()) \
	{ \
		/* Do nothing once an error has been encountered. */ \
		return defaultResult(); \
	}

//----------------------------------------------------------------------------------------------------------------------

AntlrAstBuilder::AntlrAstBuilder(SourceContext& srcCtx)
	: m_pSrcCtx(&srcCtx)
	, m_namespaceStack()
	, m_classStack()
{
	assert(m_pSrcCtx != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

RootNode::Ptr AntlrAstBuilder::Run(SourceContext& srcCtx, antlr4::tree::ParseTree* const pParseTree)
{
	AntlrAstBuilder builder(srcCtx);

	// Walk the parse tree to discover the source file symbols
	// and fill the source code's symbole table.
	std::any result = builder.visit(pParseTree);

	return std::any_cast<RootNode::Ptr>(result);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitRoot(HarlequinParser::RootContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	RootNode::Ptr pAstNode = RootNode::New();

	// Visit each 'using' statement.
	for(auto* const pUsingCtx : pCtx->usingStmt())
	{
		std::any result = visit(pUsingCtx);
		if(result.has_value())
		{
			pAstNode->imports.push_back(std::any_cast<UsingNode::Ptr>(result));
		}
	}

	// Visit each 'using alias' statement.
	for(auto* const pAliasCtx : pCtx->usingAliasStmt())
	{
		std::any result = visit(pAliasCtx);
		if(result.has_value())
		{
			pAstNode->aliases.push_back(std::any_cast<AliasNode::Ptr>(result));
		}
	}

	// Visit each 'namespace' declaration.
	for(auto* const pNamespaceDeclCtx : pCtx->namespaceDecl())
	{
		std::any result = visit(pNamespaceDeclCtx);
		if(result.has_value())
		{
			pAstNode->namespaces.push_back(std::any_cast<NamespaceNode::Ptr>(result));
		}
	}

	return pAstNode;
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitUsingStmt(HarlequinParser::UsingStmtContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	UsingNode::Ptr pAstNode = UsingNode::New();

	auto* const pQualifiedIdCtx = pCtx->qualifiedId();

	pAstNode->name = pQualifiedIdCtx->getText();
	pAstNode->nameTokenSpan = detail::TokenSpan::Extract(pQualifiedIdCtx->Id().back()->getSymbol());

	return std::make_any<UsingNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	AliasNode::Ptr pAstNode = AliasNode::New();

	auto* const pQualifiedIdCtx = pCtx->qualifiedId();
	auto* const pAliasToken = pCtx->Id();

	pAstNode->originalName = pQualifiedIdCtx->getText();
	pAstNode->aliasedName = pAliasToken->getText();

	pAstNode->originalNameTokenSpan = detail::TokenSpan::Extract(pQualifiedIdCtx->Id().back()->getSymbol());
	pAstNode->aliasedNameTokenSpan = detail::TokenSpan::Extract(pAliasToken->getSymbol());

	return std::make_any<AliasNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitNamespaceDecl(HarlequinParser::NamespaceDeclContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	NamespaceNode::Ptr pAstNode = NamespaceNode::New();

	auto* const pQualifiedIdCtx = pCtx->qualifiedId();
	auto* const pNamespaceDefCtx = pCtx->namespaceDef();

	pAstNode->shortName = pQualifiedIdCtx->getText();
	pAstNode->qualifiedName = !m_namespaceStack.empty()
		? m_namespaceStack.back() + "." + pAstNode->shortName
		: pAstNode->shortName;

	m_namespaceStack.push_back(pAstNode->qualifiedName);
	
	// Visit each 'namespace' declaration.
	for(auto* const pNamespaceDeclCtx : pNamespaceDefCtx->namespaceDecl())
	{
		std::any result = visit(pNamespaceDeclCtx);
		if(result.has_value())
		{
			pAstNode->namespaces.push_back(std::any_cast<NamespaceNode::Ptr>(result));
		}
	}

	// Visit each 'class' declaration.
	for(auto* const pClassDeclCtx : pNamespaceDefCtx->classDecl())
	{
		std::any result = visit(pClassDeclCtx);
		if(result.has_value())
		{
			pAstNode->classes.push_back(std::any_cast<ClassNode::Ptr>(result));
		}
	}

	m_namespaceStack.pop_back();

	return std::make_any<NamespaceNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitClassDecl(HarlequinParser::ClassDeclContext* const pCtx)
{
	static const std::string emptyString;

	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	ClassNode::Ptr pAstNode = ClassNode::New();

	auto* const pAccessSpecCtx = pCtx->accessBaseSpecifier();
	auto* const pStorageSpecCtx = pCtx->storageSpecifier();
	auto* const pClassTypeCtx = pCtx->classType();
	auto* const pInheritanceCtx = pCtx->classInheritance();
	auto* const pClassDefCtx = pCtx->classDef();

	const std::string namespacePrefix = !m_namespaceStack.empty()
		? m_namespaceStack.back() + "."
		: "";

	const std::string classPrefix = !m_classStack.empty()
		? m_classStack.back() + "."
		: "";

	const std::string qualifiedNamePrefix = namespacePrefix + classPrefix;
	const std::string shortName = pCtx->Id()->getText();

	m_classStack.push_back(classPrefix + shortName);

	pAstNode->shortName = shortName;
	pAstNode->qualifiedName = qualifiedNamePrefix + shortName;
	pAstNode->classType = CompilerUtil::GetClassType(pClassTypeCtx->getText());
	pAstNode->accessSpec = CompilerUtil::GetAccessType(pAccessSpecCtx ? pAccessSpecCtx->getText() : emptyString);
	pAstNode->storageSpec = CompilerUtil::GetStorageType(pStorageSpecCtx ? pStorageSpecCtx->getText() : emptyString);

	pAstNode->nameTokenSpan = detail::TokenSpan::Extract(pCtx->Id()->getSymbol());
	pAstNode->classTypeTokenSpan = detail::TokenSpan::Extract(_getClassTypeToken(pClassTypeCtx));
	pAstNode->accessSpecTokenSpan = detail::TokenSpan::Extract(pAccessSpecCtx ? _getAccessSpecToken(pAccessSpecCtx) : nullptr);
	pAstNode->storageSpecTokenSpan = detail::TokenSpan::Extract(pStorageSpecCtx ? _getStorageSpecToken(pStorageSpecCtx) : nullptr);

	if(pInheritanceCtx)
	{
		auto* const pExtendsCtx = pInheritanceCtx->classExtends();
		auto* const pImplementsCtx = pInheritanceCtx->classImpls();

		if(pExtendsCtx)
		{
			auto* const pIdCtx = pExtendsCtx->qualifiedId();

			ClassNode::BaseType baseType;
			baseType.name = pIdCtx->getText();
			baseType.tokenSpan = detail::TokenSpan::Extract(pIdCtx->Id().back()->getSymbol());

			pAstNode->extends.push_back(baseType);
		}

		if(pImplementsCtx)
		{
			auto idCtxList = pImplementsCtx->qualifiedId();

			for(auto* const pIdCtx : idCtxList)
			{
				ClassNode::BaseType baseType;
				baseType.name = pIdCtx->getText();
				baseType.tokenSpan = detail::TokenSpan::Extract(pIdCtx->Id().back()->getSymbol());

				pAstNode->implements.push_back(baseType);
			}
		}
	}

	// Visit each internal 'class' context.
	{
		auto subClassDecls = pClassDefCtx->classDecl();
		
		for(auto* const pSubClassDeclCtx : subClassDecls)
		{
			std::any result = visit(pSubClassDeclCtx);
			if(result.has_value())
			{
				pAstNode->internalClasses.push_back(std::any_cast<ClassNode::Ptr>(result));
			}
		}
	}

	m_classStack.pop_back();

	return std::make_any<ClassNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

antlr4::Token* AntlrAstBuilder::_getAccessSpecToken(HarlequinParser::AccessBaseSpecifierContext* const pCtx)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	TerminalNode* const pPublicToken = pCtx->PublicKw();
	TerminalNode* const pProtectedToken = pCtx->ProtectedKw();
	TerminalNode* const pPrivateToken = pCtx->PrivateKw();

	Token* const pTypeToken = pPublicToken
		? pPublicToken->getSymbol()
		: pProtectedToken
			? pProtectedToken->getSymbol()
			: pPrivateToken->getSymbol();
	assert(pTypeToken != nullptr);

	return pTypeToken;
}

//----------------------------------------------------------------------------------------------------------------------

antlr4::Token* AntlrAstBuilder::_getStorageSpecToken(HarlequinParser::StorageSpecifierContext* const pCtx)
{
	using namespace antlr4::tree;

	TerminalNode* const pStaticToken = pCtx->StaticKw();

	return pStaticToken
		? pStaticToken->getSymbol()
		: nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

antlr4::Token* AntlrAstBuilder::_getClassTypeToken(HarlequinParser::ClassTypeContext* const pCtx)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	TerminalNode* const pClassToken = pCtx->ClassKw();
	TerminalNode* const pInterfaceToken = pCtx->InterfaceKw();

	Token* const pTypeToken = pClassToken
		? pClassToken->getSymbol()
		: pInterfaceToken->getSymbol();
	assert(pTypeToken != nullptr);

	return pTypeToken;
}

//----------------------------------------------------------------------------------------------------------------------

#undef _HQ_ANTLR_AST_BUILDER_PREAMBLE

//----------------------------------------------------------------------------------------------------------------------
