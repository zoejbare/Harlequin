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
	, m_prefixStack()
{
	assert(m_pSrcCtx != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

RootNode::Ptr AntlrAstBuilder::Run(SourceContext& srcCtx, antlr4::tree::ParseTree* const pParseTree)
{
	AntlrAstBuilder builder(srcCtx);

	// Walk the parse tree to build the AST, returning its root node.
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

	pAstNode->id.name = pQualifiedIdCtx->getText();
	pAstNode->id.beginSpan = detail::TokenSpan::Extract(pQualifiedIdCtx->Id().front()->getSymbol());
	pAstNode->id.endSpan = detail::TokenSpan::Extract(pQualifiedIdCtx->Id().back()->getSymbol());

	return std::make_any<UsingNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	AliasNode::Ptr pAstNode = AliasNode::New();

	auto* const pQualifiedIdCtx = pCtx->qualifiedId();
	auto* const pAliasToken = pCtx->Id();

	pAstNode->originalId.name = pQualifiedIdCtx->getText();
	pAstNode->originalId.beginSpan = detail::TokenSpan::Extract(pQualifiedIdCtx->Id().front()->getSymbol());
	pAstNode->originalId.endSpan = detail::TokenSpan::Extract(pQualifiedIdCtx->Id().back()->getSymbol());

	pAstNode->aliasedId.name = pAliasToken->getText();
	pAstNode->aliasedId.beginSpan = detail::TokenSpan::Extract(pAliasToken->getSymbol());
	pAstNode->aliasedId.endSpan = pAstNode->aliasedId.beginSpan;

	return std::make_any<AliasNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitNamespaceDecl(HarlequinParser::NamespaceDeclContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	NamespaceNode::Ptr pAstNode = NamespaceNode::New();

	auto* const pQualifiedIdCtx = pCtx->qualifiedId();
	auto* const pNamespaceDefCtx = pCtx->namespaceDef();

	_getQualifiedName(pQualifiedIdCtx, pAstNode->id);

	m_prefixStack.push_back(pAstNode->id.longName);
	
	// Visit each namespace declaration.
	for(auto* const pNamespaceDeclCtx : pNamespaceDefCtx->namespaceDecl())
	{
		std::any result = visit(pNamespaceDeclCtx);
		if(result.has_value())
		{
			pAstNode->namespaces.push_back(std::any_cast<NamespaceNode::Ptr>(result));
		}
	}

	// Visit each class declaration.
	for(auto* const pClassDeclCtx : pNamespaceDefCtx->classDecl())
	{
		std::any result = visit(pClassDeclCtx);
		if(result.has_value())
		{
			pAstNode->classes.push_back(std::any_cast<ClassNode::Ptr>(result));
		}
	}

	m_prefixStack.pop_back();

	return std::make_any<NamespaceNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitClassDecl(HarlequinParser::ClassDeclContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	ClassNode::Ptr pAstNode = ClassNode::New();

	auto* const pAccessSpecCtx = pCtx->accessBaseSpecifier();
	auto* const pStorageSpecCtx = pCtx->storageSpecifier();
	auto* const pClassTypeCtx = pCtx->classType();
	auto* const pInheritanceCtx = pCtx->classInheritance();
	auto* const pClassDefCtx = pCtx->classDef();

	_getQualifiedName(pCtx->Id(), pAstNode->id);

	_getAccessDecl(pAccessSpecCtx, pAstNode->accessDecl);
	_getClassDecl(pClassTypeCtx, pAstNode->classDecl);
	_getStorageDecl(pStorageSpecCtx, pAstNode->storageDecl);

	m_prefixStack.push_back(pAstNode->id.longName);

	if(pInheritanceCtx)
	{
		auto* const pExtendsCtx = pInheritanceCtx->classExtends();
		auto* const pImplementsCtx = pInheritanceCtx->classImpls();

		if(pExtendsCtx)
		{
			auto* const pIdCtx = pExtendsCtx->qualifiedId();

			detail::TypeName id;
			id.name = pIdCtx->getText();
			id.beginSpan = detail::TokenSpan::Extract(pIdCtx->Id().front()->getSymbol());
			id.endSpan = detail::TokenSpan::Extract(pIdCtx->Id().back()->getSymbol());

			pAstNode->extends.push_back(id);
		}

		if(pImplementsCtx)
		{
			auto idCtxList = pImplementsCtx->qualifiedId();

			for(auto* const pIdCtx : idCtxList)
			{
				detail::TypeName id;
				id.name = pIdCtx->getText();
				id.beginSpan = detail::TokenSpan::Extract(pIdCtx->Id().front()->getSymbol());
				id.endSpan = detail::TokenSpan::Extract(pIdCtx->Id().back()->getSymbol());

				pAstNode->implements.push_back(id);
			}
		}
	}

	// Visit each internal class context.
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

	// Visit each class variable declaration context.
	{
		auto classVarDecls = pClassDefCtx->classVarDeclStmt();

		for(auto* const pClassVarDeclCtx : classVarDecls)
		{
			std::any result = visit(pClassVarDeclCtx);
			if(result.has_value())
			{
				pAstNode->variables.push_back(std::any_cast<ClassVariableNode::Ptr>(result));
			}
		}
	}

	m_prefixStack.pop_back();

	return std::make_any<ClassNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitClassVarDeclStmt(HarlequinParser::ClassVarDeclStmtContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	auto* const pSpecSeqCtx = pCtx->classVarDeclSpecSeq();
	auto* const pAccessSpecCtx = pSpecSeqCtx->accessSpecifier();
	auto* const pStorageSpecCtx = pSpecSeqCtx->storageSpecifier();
	auto* const pConstQualCtx = pSpecSeqCtx->constQualifier();

	auto* const pVarDeclStmtCtx = pCtx->varDeclStmt();
	auto* const pVarDeclCtx = pVarDeclStmtCtx->varDecl();
	auto* const pTypeNameDeclCtx = pVarDeclCtx->typeNameDecl();

	ClassVariableNode::Ptr pAstNode = ClassVariableNode::New();

	_getQualifiedName(pVarDeclCtx->Id(), pAstNode->id);

	_getAccessDecl(pAccessSpecCtx, pAstNode->accessDecl, pAstNode->limitedTypeIds);
	_getStorageDecl(pStorageSpecCtx, pAstNode->storageDecl);
	_getConstDecl(pConstQualCtx, pAstNode->constDecl);
	_getVarDecl(pTypeNameDeclCtx, pAstNode->varDecl, pAstNode->arrayDecl);

	// TODO: Build the variable's expression here.
	// TODO: Fix up the const type if the qualifier is present.

	return std::make_any<ClassVariableNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

void AntlrAstBuilder::_getQualifiedName(HarlequinParser::QualifiedIdContext* const pCtx, detail::QualifiedTypeName& outId) const
{
	const std::string prefix = !m_prefixStack.empty()
		? std::string(m_prefixStack.back()) + "."
		: "";

	outId.shortName = pCtx->getText();
	outId.longName = prefix + outId.shortName;
	outId.beginSpan = detail::TokenSpan::Extract(pCtx->Id().front()->getSymbol());
	outId.endSpan = detail::TokenSpan::Extract(pCtx->Id().back()->getSymbol());
}

//----------------------------------------------------------------------------------------------------------------------

void AntlrAstBuilder::_getQualifiedName(antlr4::tree::TerminalNode* const pToken, detail::QualifiedTypeName& outId) const
{
	const std::string prefix = !m_prefixStack.empty()
		? std::string(m_prefixStack.back()) + "."
		: "";

	outId.shortName = pToken->getText();
	outId.longName = prefix + outId.shortName;
	outId.beginSpan = detail::TokenSpan::Extract(pToken->getSymbol());
	outId.endSpan = outId.beginSpan;
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getAccessDecl(HarlequinParser::AccessBaseSpecifierContext* const pCtx, detail::AccessDecl& outputDecl) const
{
	if(pCtx)
	{
		outputDecl.type = CompilerUtil::GetAccessType(pCtx->getText());
		outputDecl.span = detail::TokenSpan::Extract(_getAccessSpecToken(pCtx));
	}
	else
	{
		outputDecl.type = detail::AccessDecl::Type::Private;
		outputDecl.span = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getAccessDecl(
	HarlequinParser::AccessSpecifierContext* const pCtx,
	detail::AccessDecl& outputDecl,
	detail::TypeName::Deque& outputLimitedTypes) const
{
	if(pCtx)
	{
		auto* const pBaseCtx = pCtx->accessBaseSpecifier();
		auto* const pLimitCtx = pCtx->accessLimitSpecifier();

		if(pLimitCtx)
		{
			outputDecl.type = detail::AccessDecl::Type::Public;
			outputDecl.span = detail::TokenSpan::Extract(pLimitCtx->LimitedKw()->getSymbol());

			const auto& typeIdCtxs = pLimitCtx->qualifiedId();

			// Get all the referenced limited type ID names.
			for(auto* const pIdCtx : typeIdCtxs)
			{
				detail::TypeName id;
				id.name = pIdCtx->getText();
				id.beginSpan = detail::TokenSpan::Extract(pIdCtx->Id().front()->getSymbol());
				id.endSpan = detail::TokenSpan::Extract(pIdCtx->Id().back()->getSymbol());

				outputLimitedTypes.push_back(std::move(id));
			}
		}
		else
		{
			_getAccessDecl(pBaseCtx, outputDecl);
			outputLimitedTypes.clear();
		}
	}
	else
	{
		outputDecl.type = detail::AccessDecl::Type::Private;
		outputDecl.span = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getClassDecl(HarlequinParser::ClassTypeContext* const pCtx, detail::ClassDecl& outputDecl) const
{
	// The context here should never be null.
	assert(pCtx != nullptr);

	outputDecl.type = CompilerUtil::GetClassType(pCtx->getText());
	outputDecl.span = detail::TokenSpan::Extract(_getClassTypeToken(pCtx));
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getStorageDecl(HarlequinParser::StorageSpecifierContext* const pCtx, detail::StorageDecl& outputDecl) const
{
	if(pCtx)
	{
		outputDecl.type = CompilerUtil::GetStorageType(pCtx->getText());
		outputDecl.span = detail::TokenSpan::Extract(_getStorageSpecToken(pCtx));
	}
	else
	{
		outputDecl.type = detail::StorageDecl::Type::Default;
		outputDecl.span = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getConstDecl(HarlequinParser::ConstQualifierContext* const pCtx, detail::ConstDecl& outputDecl) const
{
	if(pCtx)
	{
		// For now, we assume all const variables are just immutable. If any are actually literals,
		// their types will be promoted after their expression trees have been built.
		outputDecl.type = detail::ConstDecl::Type::Immutable;
		outputDecl.span = detail::TokenSpan::Extract(pCtx->ConstKw()->getSymbol());
	}
	else
	{
		outputDecl.type = detail::ConstDecl::Type::None;
		outputDecl.span = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getVarDecl(
	HarlequinParser::TypeNameDeclContext* const pCtx,
	detail::VarDecl& outputVarDecl,
	detail::ArrayDecl& outputArrayDecl) const
{
	// The type name context should never be null.
	assert(pCtx != nullptr);

	outputVarDecl.type = CompilerUtil::GetVarType(pCtx->Id()->getText());
	outputVarDecl.span = detail::TokenSpan::Extract(pCtx->Id()->getSymbol());

	auto* const pArrayTypeDeclCtx = pCtx->arrayTypeDecl();

	if(pArrayTypeDeclCtx)
	{
		const auto& arrayCommas = pArrayTypeDeclCtx->Comma();
		const size_t commaCount = arrayCommas.size();

		switch(commaCount)
		{
			case 0: outputArrayDecl.type = detail::ArrayDecl::Type::Linear; break;
			case 1: outputArrayDecl.type = detail::ArrayDecl::Type::Grid2D; break;
			case 2: outputArrayDecl.type = detail::ArrayDecl::Type::Grid3D; break;

			default:
				// This should never happen since the grammar enforces the syntax for all valid array declarations.
				assert(false);
				break;
		}

		outputArrayDecl.beginSpan = detail::TokenSpan::Extract(pArrayTypeDeclCtx->LeftBracket()->getSymbol());
		outputArrayDecl.endSpan = detail::TokenSpan::Extract(pArrayTypeDeclCtx->RightBracket()->getSymbol());
	}
	else
	{
		outputArrayDecl.type = detail::ArrayDecl::Type::None;
		outputArrayDecl.beginSpan = detail::TokenSpan::Default();
		outputArrayDecl.endSpan = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline antlr4::Token* AntlrAstBuilder::_getAccessSpecToken(HarlequinParser::AccessBaseSpecifierContext* const pCtx)
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

inline antlr4::Token* AntlrAstBuilder::_getClassTypeToken(HarlequinParser::ClassTypeContext* const pCtx)
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

inline antlr4::Token* AntlrAstBuilder::_getStorageSpecToken(HarlequinParser::StorageSpecifierContext* const pCtx)
{
	using namespace antlr4::tree;

	TerminalNode* const pStaticToken = pCtx->StaticKw();

	return pStaticToken
		? pStaticToken->getSymbol()
		: nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

#undef _HQ_ANTLR_AST_BUILDER_PREAMBLE

//----------------------------------------------------------------------------------------------------------------------
