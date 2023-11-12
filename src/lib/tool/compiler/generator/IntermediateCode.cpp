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

#include "CompilerUtil.hpp"
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

	// The error handler is unnecessary now that we've finished walking the parse tree.
	pOutput->m_pErrorHandler = nullptr;

	// Dispose of the code IR object if there were errors while walking the parse tree.
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
		auto* const pAccessBaseSpecCtx = pCtx->accessBaseSpecifier();
		auto* const pStorageSpecCtx = pCtx->storageSpecifier();

		const detail::ClassType classType = CompilerUtil::GetClassType(pCtx->classType()->getText());
		const detail::AccessType accessType = CompilerUtil::GetAccessType(pAccessBaseSpecCtx ? pAccessBaseSpecCtx->getText() : "");
		const detail::StorageType storageType = CompilerUtil::GetStorageType(pStorageSpecCtx ? pStorageSpecCtx->getText() : "");

		// Only create class symbols for classes and non-static interfaces.
		if(classType == detail::ClassType::Class 
			|| (classType == detail::ClassType::Interface && storageType == detail::StorageType::Default))
		{
			auto* const pClassInheritanceCtx = pCtx->classInheritance();

			// Create new class symbol.
			ClassSymbol::Ptr pClass = ClassSymbol::New();

			// Handle the class inheritance.
			if(pClassInheritanceCtx)
			{
				auto* const pClassImplsCtx = pClassInheritanceCtx->classImpls();
				auto* const pClassExtendsCtx = pClassInheritanceCtx->classExtends();

				if(pClassImplsCtx)
				{
					_resolveQualifiedIdArray(pClass->implements, pClassImplsCtx->qualifiedId());
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
			pClass->classType = classType;
			pClass->accessType = accessType;
			pClass->storageType = storageType;

			// Keep track of the current class.
			m_classStack.push_back(pClass.get());

			// Add the class to the symbol table.
			m_symbolTable.classSymbols.emplace(qualifiedName, std::move(pClass));
		}
		else
		{
			// Report the invalid scenario of a static interface declaration.
			m_pErrorHandler->Report(
				MessageCode::ErrorStaticInterface,
				pCtx->storageSpecifier()->StaticKw()->getSymbol(),
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

std::any IntermediateCode::visitClassVarDeclStmt(HarlequinParser::ClassVarDeclStmtContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const std::string& className = m_classStack.back()->qualifiedName;
	const std::string& varNamePrefix = className + ".";

	auto* const pVarDeclSpecSeq = pCtx->classVarDeclSpecSeq();

	auto* const pAccessSpecCtx = pVarDeclSpecSeq->accessSpecifier();
	auto* const pStorageSpecCtx = pVarDeclSpecSeq->storageSpecifier();
	auto* const pConstQualCtx = pVarDeclSpecSeq->constQualifier();

	auto* const pVarDeclStmtCtx = pCtx->varDeclStmt();
	auto* const pVarDeclCtx = pVarDeclStmtCtx->varDecl();

	//auto* const pAssignExprCtx = pVarDeclCtx->expr();
	const auto nextVarDecls = pVarDeclCtx->nextVarDecl();

	const detail::StorageType storageType = CompilerUtil::GetStorageType(pStorageSpecCtx ? pStorageSpecCtx->getText() : "");

	// Resolve data that will shared by all variables defined here.
	detail::VariableBase varBase;
	_resolveVariableCommonData(varBase, pVarDeclCtx, pConstQualCtx);

	// Resolve the variable's access specifier.
	detail::AccessType accessType;
	detail::StringArray accessLimitTypes;
	_resolveAccessSpecifier(accessType, accessLimitTypes, pAccessSpecCtx);

	auto registerVariable = 
		[this, &varNamePrefix, &storageType, &accessType, &varBase, &accessLimitTypes]
		(antlr4::tree::TerminalNode* const pNameId)
	{
		const std::string varNameShort = pNameId->getText();

		// Construct the variable's fully qualified name.
		const std::string varNameQualified = varNamePrefix + varNameShort;

		if(!m_symbolTable.classVarSymbols.count(varNameQualified))
		{
			ClassVarSymbol::Ptr pClassVar = ClassVarSymbol::New();

			// Copy the access limiters to the variable.
			pClassVar->accessLimitTypes = accessLimitTypes;

			// Copy the variable common data we resolved from the start of the declaration.
			pClassVar->base = varBase;

			// Cache the variable's name.
			pClassVar->base.varName = varNameQualified;

			// Cache the remaining variable properties.
			pClassVar->accessType = accessType;
			pClassVar->storageType = storageType;

			// Map the new variable symbol.
			m_symbolTable.classVarSymbols.emplace(varNameQualified, std::move(pClassVar));
		}
		else
		{
			// Report the duplicate variable name as an error.
			m_pErrorHandler->Report(
				MessageCode::ErrorDuplicateVar,
				pNameId->getSymbol(),
				"duplicate class variable '%s'",
				varNameQualified.c_str()
			);
		}
	};

	// Register the first variable in the declaration.
	registerVariable(pVarDeclCtx->Id());

	// Register each subsequent variable for this declaration.
	for(auto* const pNextVarDeclCtx : nextVarDecls)
	{
		registerVariable(pNextVarDeclCtx->Id());
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

inline void IntermediateCode::_resolveAccessSpecifier(
	detail::AccessType& outputAccessType,
	detail::StringArray& outputLimitTypes,
	HarlequinParser::AccessSpecifierContext* const pAccessSpecCtx)
{
	if(pAccessSpecCtx)
	{
		auto* const pAccessBaseSpecCtx = pAccessSpecCtx->accessBaseSpecifier();
		auto* const pAccessLimitSpecCtx = pAccessSpecCtx->accessLimitSpecifier();

		// The default access type is 'private', so setting that here ahead of time will simply some of the logic below.
		outputAccessType = detail::AccessType::Private;

		const detail::AccessType tempAccessType = pAccessBaseSpecCtx
			? CompilerUtil::GetAccessType(pAccessBaseSpecCtx->getText())
			: outputAccessType;

		if(pAccessLimitSpecCtx)
		{
			// Get the type IDs specified in the access limiter list.
			_resolveQualifiedIdArray(outputLimitTypes, pAccessLimitSpecCtx->qualifiedId());

			// The grammar should not allow specifying an empty list.
			assert(outputLimitTypes.size() > 0);

			if(pAccessBaseSpecCtx)
			{
				// This is to handle special cases where valid base+limited specifier combinations have slightly different behavior.
				switch(tempAccessType)
				{
					case detail::AccessType::Protected:
						// Valid specifier combination.
						outputAccessType = tempAccessType;
						break;

					default:
					{
						// Report the invalid access specifier combination.
						m_pErrorHandler->Report(
							MessageCode::ErrorInvalidAccessSpec,
							pAccessBaseSpecCtx->getStart(),
							"'%s' access specifier is not compatible with the 'limited' access specifier",
							pAccessBaseSpecCtx->getText().c_str());
						break;
					}
				}
			}
		}
		else if(pAccessBaseSpecCtx)
		{
			// No special case handling here, just use the base type as-is.
			outputAccessType = tempAccessType;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void IntermediateCode::_resolveVariableCommonData(
	detail::VariableBase& output, 
	HarlequinParser::VarDeclContext* const pVarDeclCtx,
	HarlequinParser::ConstQualifierContext* const pConstQualCtx)
{
	auto* const pTypeNameDeclCtx = pVarDeclCtx->typeNameDecl();
	auto* const pArrayTypeDeclCtx = pTypeNameDeclCtx->arrayTypeDecl();

	output.typeName = pTypeNameDeclCtx->Id()->getText();
	output.varType = CompilerUtil::GetVarType(output.typeName);
	output.arrayType = CompilerUtil::GetArrayType(pArrayTypeDeclCtx ? pArrayTypeDeclCtx->getText() : "");

	// All variable with the 'const' qualifier will start off as immutable.
	// They can be upgraded to literal during expression resolution.
	output.constType = (pConstQualCtx) ? detail::ConstType::Immutable : detail::ConstType::None;
}

//----------------------------------------------------------------------------------------------------------------------

inline void IntermediateCode::_resolveQualifiedIdArray(
	detail::StringArray& output,
	const std::vector<HarlequinParser::QualifiedIdContext*>& qualifiedIds)
{
	const size_t length = qualifiedIds.size();

	// Resize the output to the exact number of type names given.
	output.resize(length);

	// Extract each type name ID.
	for(size_t i = 0; i < length; ++i)
	{
		output[i] = qualifiedIds[i]->getText();
	}
}

//----------------------------------------------------------------------------------------------------------------------
