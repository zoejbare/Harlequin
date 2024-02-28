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

	m_namespaceStack.pop_back();

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

	pAstNode->nameTokenSpan = detail::TokenSpan::Extract(pCtx->Id()->getSymbol());

	_getClassType(pClassTypeCtx, pAstNode->classType, pAstNode->classTypeTokenSpan);
	_getAccessSpec(pAccessSpecCtx, pAstNode->accessSpec, pAstNode->accessSpecTokenSpan);
	_getStorageSpec(pStorageSpecCtx, pAstNode->storageSpec, pAstNode->storageSpecTokenSpan);

	if(pInheritanceCtx)
	{
		auto* const pExtendsCtx = pInheritanceCtx->classExtends();
		auto* const pImplementsCtx = pInheritanceCtx->classImpls();

		if(pExtendsCtx)
		{
			auto* const pIdCtx = pExtendsCtx->qualifiedId();

			detail::TypeName baseType;
			baseType.name = pIdCtx->getText();
			baseType.tokenSpan = detail::TokenSpan::Extract(pIdCtx->Id().back()->getSymbol());

			pAstNode->extends.push_back(baseType);
		}

		if(pImplementsCtx)
		{
			auto idCtxList = pImplementsCtx->qualifiedId();

			for(auto* const pIdCtx : idCtxList)
			{
				detail::TypeName baseType;
				baseType.name = pIdCtx->getText();
				baseType.tokenSpan = detail::TokenSpan::Extract(pIdCtx->Id().back()->getSymbol());

				pAstNode->implements.push_back(baseType);
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

	m_classStack.pop_back();

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

	const std::string namespacePrefix = !m_namespaceStack.empty()
		? m_namespaceStack.back() + "."
		: "";

	const std::string classPrefix = !m_classStack.empty()
		? m_classStack.back() + "."
		: "";

	const std::string qualifiedNamePrefix = namespacePrefix + classPrefix;
	const std::string shortName = pVarDeclCtx->Id()->getText();

	pAstNode->shortName = shortName;
	pAstNode->qualifiedName = qualifiedNamePrefix + shortName;

	pAstNode->nameTokenSpan = detail::TokenSpan::Extract(pVarDeclCtx->Id()->getSymbol());

	_getAccessSpec(pAccessSpecCtx, pAstNode->accessSpec, pAstNode->accessSpecToken, pAstNode->limitedTypes);
	_getStorageSpec(pStorageSpecCtx, pAstNode->storageSpec, pAstNode->storageSpecToken);
	_getConstQualifier(pConstQualCtx, pAstNode->constType, pAstNode->constTypeToken);
	_getVarType(pTypeNameDeclCtx, pAstNode->varType, pAstNode->varTypeTokenSpan, pAstNode->arrayType, pAstNode->arrayTypeTokenSpan);

	// TODO: Build the variable's expression here.
	// TODO: Fix up the const type if the qualifier is present.

	return std::make_any<ClassVariableNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getClassType(
	HarlequinParser::ClassTypeContext* const pCtx,
	detail::ClassType& outputType,
	detail::TokenSpan& outputTokenSpan)
{
	// The context here should never be null.
	assert(pCtx != nullptr);

	outputType = CompilerUtil::GetClassType(pCtx->getText());
	outputTokenSpan = detail::TokenSpan::Extract(_getClassTypeToken(pCtx));
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getAccessSpec(
	HarlequinParser::AccessBaseSpecifierContext* const pCtx,
	detail::AccessType& outputSpec,
	detail::TokenSpan& outputTokenSpan)
{
	if(pCtx)
	{
		outputSpec = CompilerUtil::GetAccessType(pCtx->getText());
		outputTokenSpan = detail::TokenSpan::Extract(_getAccessSpecToken(pCtx));
	}
	else
	{
		outputSpec = detail::AccessType::Private;
		outputTokenSpan = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getAccessSpec(
	HarlequinParser::AccessSpecifierContext* const pCtx,
	detail::AccessType& outputSpec,
	detail::TokenSpan& outputTokenSpan,
	detail::TypeName::Deque& outputLimitedTypes)
{
	if(pCtx)
	{
		auto* const pBaseCtx = pCtx->accessBaseSpecifier();
		auto* const pLimitCtx = pCtx->accessLimitSpecifier();

		if(pLimitCtx)
		{
			outputSpec = detail::AccessType::Public;
			outputTokenSpan = detail::TokenSpan::Extract(pLimitCtx->LimitedKw()->getSymbol());

			const auto& typeIdCtxs = pLimitCtx->qualifiedId();

			// Get all the referenced limited type ID names.
			for(auto* const pIdCtx : typeIdCtxs)
			{
				detail::TypeName limitedType;
				limitedType.name = pIdCtx->getText();
				limitedType.tokenSpan = detail::TokenSpan::Extract(pIdCtx->Id().back()->getSymbol());

				outputLimitedTypes.push_back(std::move(limitedType));
			}
		}
		else
		{
			_getAccessSpec(pBaseCtx, outputSpec, outputTokenSpan);
			outputLimitedTypes.clear();
		}
	}
	else
	{
		outputSpec = detail::AccessType::Private;
		outputTokenSpan = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getStorageSpec(
	HarlequinParser::StorageSpecifierContext* const pCtx,
	detail::StorageType& outputSpec,
	detail::TokenSpan& outputTokenSpan)
{
	if(pCtx)
	{
		outputSpec = CompilerUtil::GetStorageType(pCtx->getText());
		outputTokenSpan = detail::TokenSpan::Extract(_getStorageSpecToken(pCtx));
	}
	else
	{
		outputSpec = detail::StorageType::Default;
		outputTokenSpan = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getConstQualifier(
	HarlequinParser::ConstQualifierContext* const pCtx,
	detail::ConstType& outputType,
	detail::TokenSpan& outputTokenSpan)
{
	if(pCtx)
	{
		// For now, we assume all const variables are just immutable. If any are actually literals,
		// their types will be promoted after their expression trees have been built.
		outputType = detail::ConstType::Immutable;
		outputTokenSpan = detail::TokenSpan::Extract(pCtx->ConstKw()->getSymbol());
	}
	else
	{
		outputType = detail::ConstType::None;
		outputTokenSpan = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getVarType(
	HarlequinParser::TypeNameDeclContext* const pCtx,
	detail::VarType& outputVarType,
	detail::TokenSpan& outputVarTypeTokenSpan,
	detail::ArrayType& outputArrayType,
	detail::TokenSpan& outputArrayTypeTokenSpan)
{
	// The type name context should never be null.
	assert(pCtx != nullptr);

	outputVarType = CompilerUtil::GetVarType(pCtx->Id()->getText());
	outputVarTypeTokenSpan = detail::TokenSpan::Extract(pCtx->Id()->getSymbol());

	auto* const pArrayTypeDeclCtx = pCtx->arrayTypeDecl();

	if(pArrayTypeDeclCtx)
	{
		const auto& arrayCommas = pArrayTypeDeclCtx->Comma();

		const size_t commaCount = arrayCommas.size();

		const detail::TokenSpan startSpan = detail::TokenSpan::Extract(pArrayTypeDeclCtx->LeftBracket()->getSymbol());
		const detail::TokenSpan endSpan = detail::TokenSpan::Extract(pArrayTypeDeclCtx->RightBracket()->getSymbol());
		
		// TODO: We need to support both start and end spans.
		outputArrayTypeTokenSpan = startSpan;

		switch(commaCount)
		{
			case 0:
				outputArrayType = detail::ArrayType::Linear;
				break;

			case 1:
				outputArrayType = detail::ArrayType::Grid2D;
				break;

			case 2:
				outputArrayType = detail::ArrayType::Grid3D;
				break;

			default:
				// This should never happen since the grammar enforces the syntax for all valid array declarations.
				assert(false);
				break;
		}
	}
	else
	{
		outputArrayType = detail::ArrayType::None;
		outputArrayTypeTokenSpan = detail::TokenSpan::Default();
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
