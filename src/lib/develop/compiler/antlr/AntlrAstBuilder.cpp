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

			detail::SimpleIdentifier id;
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
				detail::SimpleIdentifier id;
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

	// Visit each class method declaration context.
	{
		auto methodDecls = pClassDefCtx->methodDecl();

		for(auto* const pMethodDeclCtx : methodDecls)
		{
			std::any result = visit(pMethodDeclCtx);
			if(result.has_value())
			{
				pAstNode->methods.push_back(std::any_cast<MethodNode::Ptr>(result));
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

	_getAccessDecl(pAccessSpecCtx, pAstNode->accessDecl);
	_getStorageDecl(pStorageSpecCtx, pAstNode->storageDecl);
	_getConstDecl(pConstQualCtx, pAstNode->constDecl);
	_getVarDecl(pTypeNameDeclCtx, pAstNode->varDecl);

	// TODO: Build the variable's expression here.
	// TODO: Fix up the const type if the qualifier is present.

	return std::make_any<ClassVariableNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitMethodDecl(HarlequinParser::MethodDeclContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	auto* const pIdToken = pCtx->Id();

	auto* const pTypeNameDeclCtx = pCtx->typeNameDecl();
	auto* const pConstQualCtx = pCtx->constQualifier();
	auto* const pSpecSeqCtx = pCtx->methodDeclSpecSeq();
	auto* const pArgSeqCtx = pCtx->methodArgSeq();
	auto* const pCodeBlockCtx = pCtx->codeBlock();

	auto* const pAccessSpecCtx = pSpecSeqCtx->accessSpecifier();
	auto* const pStorageSpecCtx = pSpecSeqCtx->storageSpecifier();
	auto* const pFuncSpecCtx = pSpecSeqCtx->functionSpecifier();

	MethodNode::Ptr pAstNode = MethodNode::New();

	_getStorageDecl(pStorageSpecCtx, pAstNode->storageDecl);
	_getConstDecl(pConstQualCtx, pAstNode->constDecl);
	_getFuncDecl(pFuncSpecCtx, pAstNode->funcDecl);
	_getAccessDecl(pAccessSpecCtx, pAstNode->accessDecl);
	_getVarDecl(pTypeNameDeclCtx, pAstNode->returnType);

	_getMethodArgs(pArgSeqCtx, pAstNode->args);
	_getMethodSignature(pIdToken->getText(), pIdToken, pAstNode->args, pAstNode->id);

	// Check if a native function was given a code block.
	if(pAstNode->funcDecl.type == detail::FuncDecl::Type::Native)
	{
		if(pAstNode->constDecl.type != detail::ConstDecl::Type::None)
		{
			m_pSrcCtx->Report(
				detail::MessageCode::WarningNativeConstMethod,
				pAstNode->constDecl.span,
				"Native method was unnecessarily declared with the 'const' modifier; ignoring 'const'");

			// Overwrite with the default const declaration for this method since it should be ignored.
			_getConstDecl(nullptr, pAstNode->constDecl);
		}

		if(pCodeBlockCtx)
		{
			m_pSrcCtx->Report(
				detail::MessageCode::ErrorNativeMethodImpl,
				pAstNode->id.nameSpan,
				"Native method was declared with implementation");

			return defaultResult();
		}
	}
	else if(!pCodeBlockCtx)
	{
		m_pSrcCtx->Report(
			detail::MessageCode::ErrorNoMethodImpl,
			pAstNode->id.nameSpan,
			"Method was declared without implementation");

		return defaultResult();
	}

	return std::make_any<MethodNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

std::any AntlrAstBuilder::visitMethodArg(HarlequinParser::MethodArgContext* const pCtx)
{
	_HQ_ANTLR_AST_BUILDER_PREAMBLE();

	auto* const pVarDeclSpecSeqCtx = pCtx->argVarDeclSpecSeq();
	auto* const pTypeNameDeclCtx = pCtx->typeNameDecl();
	auto* const pExprCtx = pCtx->expr();

	auto* const pConstQualCtx = pVarDeclSpecSeqCtx->constQualifier();

	ArgVariableNode::Ptr pAstNode = ArgVariableNode::New();

	_getQualifiedName(pCtx->Id(), pAstNode->id);

	_getConstDecl(pConstQualCtx, pAstNode->constDecl);
	_getVarDecl(pTypeNameDeclCtx, pAstNode->varDecl);

	if(pAstNode->constDecl.type != detail::ConstDecl::Type::None 
		&& pAstNode->varDecl.typeDecl.type == detail::TypeDecl::Type::Native)
	{
		m_pSrcCtx->Report(
			detail::MessageCode::ErrorConstNativeVar, 
			pAstNode->constDecl.span, 
			"Native types are not permitted to have the 'const' qualifier");
		return defaultResult();
	}

	// TODO: Build the assignment expression.

	return std::make_any<ArgVariableNode::Ptr>(pAstNode);
}

//----------------------------------------------------------------------------------------------------------------------

inline std::string AntlrAstBuilder::_getIdPrefix() const
{
	std::stringstream stream;

	if(!m_prefixStack.empty())
	{
		stream << m_prefixStack.back() << ".";
	}

	return stream.str();
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getQualifiedName(HarlequinParser::QualifiedIdContext* const pCtx, detail::QualifiedIdentifier& outId) const
{
	const std::string prefix = _getIdPrefix();

	outId.shortName = pCtx->getText();
	outId.longName = prefix + outId.shortName;
	outId.beginSpan = detail::TokenSpan::Extract(pCtx->Id().front()->getSymbol());
	outId.endSpan = detail::TokenSpan::Extract(pCtx->Id().back()->getSymbol());
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getQualifiedName(antlr4::tree::TerminalNode* const pToken, detail::QualifiedIdentifier& outId) const
{
	if(pToken)
	{
		const std::string prefix = _getIdPrefix();

		outId.shortName = pToken->getText();
		outId.longName = prefix + outId.shortName;
		outId.beginSpan = detail::TokenSpan::Extract(pToken->getSymbol());
		outId.endSpan = outId.beginSpan;
	}
	else
	{
		outId.shortName.clear();
		outId.longName.clear();

		outId.beginSpan = detail::TokenSpan::Default();
		outId.endSpan = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getMethodSignature(
	const std::string& methodName, 
	antlr4::tree::TerminalNode* const pNameToken, 
	const ArgVariableNode::PtrDeque& args, 
	detail::MethodIdentifier& outId) const
{
	const std::string prefix = _getIdPrefix();

	std::stringstream stream;
	stream << prefix << methodName << "(";

	bool firstArg = true;
	for(const auto& pArg : args)
	{
		if(!firstArg)
		{
			stream << ",";
		}

		stream << CompilerUtil::GetTypeSignature(pArg->varDecl, pArg->constDecl);
		firstArg = false;
	}

	stream << ")";

	outId.name = methodName;
	outId.signature = stream.str();
	outId.nameSpan = detail::TokenSpan::Extract(pNameToken->getSymbol());
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getMethodArgs(
	HarlequinParser::MethodArgSeqContext* const pCtx, 
	ArgVariableNode::PtrDeque& outputArgs)
{
	if(pCtx)
	{
		const auto methodArgs = pCtx->methodArg();

		for(auto* const pArgCtx : methodArgs)
		{
			std::any result = visit(pArgCtx);
			if(result.has_value())
			{
				outputArgs.push_back(std::any_cast<ArgVariableNode::Ptr>(result));
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void AntlrAstBuilder::_getAccessDecl(
	HarlequinParser::AccessBaseSpecifierContext* const pCtx, 
	detail::AccessDecl& outputDecl) const
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
	detail::AccessDecl& outputDecl) const
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
				const auto& idTokens = pIdCtx->Id();

				detail::SimpleIdentifier id;
				id.name = pIdCtx->getText();
				id.beginSpan = detail::TokenSpan::Extract(idTokens.front()->getSymbol());
				id.endSpan = detail::TokenSpan::Extract(idTokens.back()->getSymbol());

				outputDecl.limitedTypeIds.push_back(std::move(id));
			}
		}
		else
		{
			_getAccessDecl(pBaseCtx, outputDecl);
			outputDecl.limitedTypeIds.clear();
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

inline void AntlrAstBuilder::_getConstDecl(HarlequinParser::ConstQualifierContext* const pCtx, detail::ConstDecl& outputDecl) const
{
	if(pCtx)
	{
		// For now, we assume all const variables are just immutable. If any are actually literals,
		// their types will be promoted after their expression trees have been built. And it doesn't
		// really matter in the case of methods since 'const' has a different meaning for them.
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

inline void AntlrAstBuilder::_getFuncDecl(HarlequinParser::FunctionSpecifierContext* const pCtx, detail::FuncDecl& outputDecl) const
{
	if(pCtx)
	{
		outputDecl.type = CompilerUtil::GetFunctionType(pCtx->getText());
		outputDecl.span = detail::TokenSpan::Extract(_getFuncSpecToken(pCtx));
	}
	else
	{
		outputDecl.type = detail::FuncDecl::Type::Default;
		outputDecl.span = detail::TokenSpan::Default();
	}
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

inline void AntlrAstBuilder::_getVarDecl(
	HarlequinParser::TypeNameDeclContext* const pCtx,
	detail::VarDecl& outputVarDecl) const
{
	// The type name context should never be null.
	assert(pCtx != nullptr);

	auto* const pToken = pCtx->Id();

	std::string tokenText = pToken->getText();

	outputVarDecl.typeDecl.type = CompilerUtil::GetDataType(tokenText);
	outputVarDecl.typeDecl.span = detail::TokenSpan::Extract(pToken->getSymbol());

	if(outputVarDecl.typeDecl.type == detail::TypeDecl::Type::Object)
	{
		// Store the identifier's text if we've detected it's an object type.
		outputVarDecl.typeDecl.objectName = std::move(tokenText);
	}

	auto* const pArrayTypeDeclCtx = pCtx->arrayTypeDecl();

	if(pArrayTypeDeclCtx)
	{
		const auto& arrayCommas = pArrayTypeDeclCtx->Comma();
		const size_t commaCount = arrayCommas.size();

		switch(commaCount)
		{
			case 0: outputVarDecl.arrayDecl.type = detail::ArrayDecl::Type::Linear; break;
			case 1: outputVarDecl.arrayDecl.type = detail::ArrayDecl::Type::Grid2D; break;
			case 2: outputVarDecl.arrayDecl.type = detail::ArrayDecl::Type::Grid3D; break;

			default:
				// This should never happen since the grammar enforces the syntax for all valid array declarations.
				assert(false);
				break;
		}

		outputVarDecl.arrayDecl.beginSpan = detail::TokenSpan::Extract(pArrayTypeDeclCtx->LeftBracket()->getSymbol());
		outputVarDecl.arrayDecl.endSpan = detail::TokenSpan::Extract(pArrayTypeDeclCtx->RightBracket()->getSymbol());
	}
	else
	{
		outputVarDecl.arrayDecl.type = detail::ArrayDecl::Type::None;
		outputVarDecl.arrayDecl.beginSpan = detail::TokenSpan::Default();
		outputVarDecl.arrayDecl.endSpan = detail::TokenSpan::Default();
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline antlr4::Token* AntlrAstBuilder::_getClassTypeToken(HarlequinParser::ClassTypeContext* const pCtx)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	TerminalNode* const pClassToken = pCtx->ClassKw();
	TerminalNode* const pInterfaceToken = pCtx->InterfaceKw();

	Token* const pOutputToken = pClassToken
		? pClassToken->getSymbol()
		: pInterfaceToken
			? pInterfaceToken->getSymbol()
			: nullptr;

	assert(pOutputToken != nullptr);
	return pOutputToken;
}

//----------------------------------------------------------------------------------------------------------------------

inline antlr4::Token* AntlrAstBuilder::_getAccessSpecToken(HarlequinParser::AccessBaseSpecifierContext* const pCtx)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	TerminalNode* const pPublicToken = pCtx->PublicKw();
	TerminalNode* const pProtectedToken = pCtx->ProtectedKw();
	TerminalNode* const pPrivateToken = pCtx->PrivateKw();

	Token* const pOutputToken = pPublicToken
		? pPublicToken->getSymbol()
		: pProtectedToken
			? pProtectedToken->getSymbol()
			: pPrivateToken
				? pPrivateToken->getSymbol()
				: nullptr;

	assert(pOutputToken != nullptr);
	return pOutputToken;
}

//----------------------------------------------------------------------------------------------------------------------

inline antlr4::Token* AntlrAstBuilder::_getFuncSpecToken(HarlequinParser::FunctionSpecifierContext* const pCtx)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	TerminalNode* const pInlineToken = pCtx->InlineKw();
	TerminalNode* const pVirtualToken = pCtx->VirtualKw();
	TerminalNode* const pNativeToken = pCtx->NativeKw();

	Token* const pOutputToken = pInlineToken
		? pInlineToken->getSymbol()
		: pVirtualToken
			? pVirtualToken->getSymbol()
			: pNativeToken
				? pNativeToken->getSymbol()
				: nullptr;

	assert(pOutputToken != nullptr);
	return pOutputToken;
}

//----------------------------------------------------------------------------------------------------------------------

inline antlr4::Token* AntlrAstBuilder::_getStorageSpecToken(HarlequinParser::StorageSpecifierContext* const pCtx)
{
	using namespace antlr4;
	using namespace antlr4::tree;

	TerminalNode* const pStaticToken = pCtx->StaticKw();

	Token* const pOutputToken = pStaticToken
		? pStaticToken->getSymbol()
		: nullptr;

	assert(pOutputToken != nullptr);
	return pOutputToken;
}

//----------------------------------------------------------------------------------------------------------------------

#undef _HQ_ANTLR_AST_BUILDER_PREAMBLE

//----------------------------------------------------------------------------------------------------------------------
