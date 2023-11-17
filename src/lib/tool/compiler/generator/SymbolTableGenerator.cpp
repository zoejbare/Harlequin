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

#include "SymbolTableGenerator.hpp"

#include "CompilerUtil.hpp"
#include "ParserErrorHandler.hpp"

#include "../SourceFile.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

SymbolTableGenerator::SymbolTableGenerator(ParserErrorHandler* const pErrorHandler, SymbolTable* const pSymbolTable)
	: m_pErrorHandler(pErrorHandler)
	, m_pSymbolTable(pSymbolTable)
	, m_namespaceStack()
	, m_classStack()
	, m_classVarStack()
{
}

//----------------------------------------------------------------------------------------------------------------------

void SymbolTableGenerator::Run(
	antlr4::tree::ParseTree* const pParseTree, 
	ParserErrorHandler* const pErrorHandler,
	SymbolTable* const pSymbolTable)
{
	SymbolTableGenerator* pGenerator = new SymbolTableGenerator(pErrorHandler, pSymbolTable);

	// Walk the parse tree to discover the source file symbols
	// and fill the source code's symbole table.
	pGenerator->visit(pParseTree);

	delete pGenerator;
}

//----------------------------------------------------------------------------------------------------------------------

bool SymbolTableGenerator::shouldVisitNextChild(antlr4::tree::ParseTree* const pNode, const std::any& currentResult)
{
	(void) pNode;
	(void) currentResult;

	return !m_pErrorHandler->EncounteredError();
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolTableGenerator::visitUsingStmt(HarlequinParser::UsingStmtContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const std::string qualifiedNamespace = pCtx->qualifiedId()->getText();

	// Check to see if this namespace is already being used (we can ignore duplicate namespaces).
	if(!m_pSymbolTable->imports.count(qualifiedNamespace))
	{
		m_pSymbolTable->imports.insert(qualifiedNamespace);
	}

	return visitChildren(pCtx);
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolTableGenerator::visitUsingAliasStmt(HarlequinParser::UsingAliasStmtContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	const std::string qualifiedClassName = pCtx->qualifiedId()->getText();
	const auto aliasNames = pCtx->Id();

	// Map each alias listed in the statement.
	for(auto* const pAliasToken : aliasNames)
	{
		const std::string aliasedClassName = pAliasToken->getText();

		// Check if the class alias has not already been declared.
		// If it has, it means there is an ambiguous reference to
		// that class name.
		if(!m_pSymbolTable->classAliases.count(aliasedClassName))
		{
			m_pSymbolTable->classAliases.emplace(aliasedClassName, qualifiedClassName);
		}
		else
		{
			std::stringstream stream;
			stream << "duplicate class alias '" << aliasedClassName << "'";

			// Report the duplicate alias name as an error.
			m_pErrorHandler->Report(
				MessageCode::ErrorDuplicateAlias,
				TokenSpan::WithSourceText(pAliasToken->getSymbol()),
				stream.str());
		}
	}

	return visitChildren(pCtx);
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolTableGenerator::visitNamespaceDecl(HarlequinParser::NamespaceDeclContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	NamespaceSymbol* const pParentNamespace = m_namespaceStack.size() > 0
		? m_namespaceStack.back()
		: nullptr;

	NamespaceSymbol* pCurrentNamespace = nullptr;

	const std::string shortName = pCtx->qualifiedId()->getText();
	const std::string qualifiedName = pParentNamespace
		? pParentNamespace->qualifiedName + "." + shortName
		: shortName;

	// Check if this namespace has already been defined.
	auto kv = m_pSymbolTable->namespaces.find(shortName);
	if(kv == m_pSymbolTable->namespaces.end())
	{
		// This is the first time this namespace has been encountered.
		NamespaceSymbol::Ptr pNamespace = NamespaceSymbol::New();

		pNamespace->shortName = shortName;
		pNamespace->qualifiedName = qualifiedName;

		// Cache the raw pointer to the new namespace symbol.
		pCurrentNamespace = pNamespace.get();

		// Move the new namespace into the symbol table.
		m_pSymbolTable->namespaces.emplace(qualifiedName, std::move(pNamespace));
	}
	else
	{
		// Cache the raw pointer to the existing namespace symbol.
		pCurrentNamespace = kv->second.get();
	}

	// Add the current namespace to the stack.
	m_namespaceStack.push_back(pCurrentNamespace);

	// Recursively visit all children contained within this namespace.
	std::any result = visitChildren(pCtx);

	// Pop the current namespace from the stack.
	m_namespaceStack.pop_back();

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolTableGenerator::visitClassDecl(HarlequinParser::ClassDeclContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	NamespaceSymbol* const pParentNamespace = m_namespaceStack.size() > 0
		? m_namespaceStack.back()
		: nullptr;

	ClassSymbol* const pParentClass = m_classStack.size() > 0
		? m_classStack.back()
		: nullptr;

	const std::string shortName = pCtx->Id()->getText();
	const std::string qualifiedName = pParentClass
		? pParentClass->qualifiedName + "." + shortName
		: pParentNamespace
			? pParentNamespace->qualifiedName + "." + shortName
			: shortName;

	ClassSymbol::PtrMap* pDestMap = pParentClass
		? &pParentClass->classes
		: pParentNamespace
			? &pParentNamespace->classes
			: &m_pSymbolTable->rootClasses;

	// Check to see if this class has already been defined.
	auto kv = pDestMap->find(shortName);
	if(kv == pDestMap->end())
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
			pClass->pParentNamespace = !pParentClass ? pParentNamespace : nullptr;
			pClass->pParentClass = pParentClass;
			pClass->qualifiedName = qualifiedName;
			pClass->shortName = shortName;
			pClass->classType = classType;
			pClass->accessType = accessType;
			pClass->storageType = storageType;

			// Keep track of the current class.
			m_classStack.push_back(pClass.get());

			// Add the new class to the symbol table.
			assert(m_pSymbolTable->allClasses.count(qualifiedName) == 0);
			m_pSymbolTable->allClasses.emplace(qualifiedName, pClass.get());

			// Move the new class into the destination map.
			pDestMap->emplace(shortName, std::move(pClass));
		}
		else
		{
			std::stringstream stream;
			stream << "illegal static interface declaration: '" << qualifiedName << "'";

			// Report the invalid scenario of a static interface declaration.
			m_pErrorHandler->Report(
				MessageCode::ErrorStaticInterface,
				TokenSpan::WithSourceText(pStorageSpecCtx->StaticKw()->getSymbol()),
				stream.str());
		}
	}
	else
	{
		std::stringstream stream;
		stream << "duplicate class declaration: '" << qualifiedName << "'";

		// Report the duplicate class declaration as an error.
		m_pErrorHandler->Report(
			MessageCode::ErrorDuplicateClass,
			TokenSpan::WithSourceText(pCtx->Id()->getSymbol()),
			stream.str());
	}

	std::any result = defaultResult();

	// Visit each of this rule's children.
	if(!m_pErrorHandler->EncounteredError())
	{
		result = visitChildren(pCtx);

		m_classStack.pop_back();
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

std::any SymbolTableGenerator::visitClassVarDeclStmt(HarlequinParser::ClassVarDeclStmtContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	// There should always be a parent class when discovering class variables.
	assert(m_classStack.size() > 0);

	ClassSymbol* const pParentClass = m_classStack.back();

	const std::string& varNamePrefix = pParentClass->qualifiedName + ".";

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
		[this, &pParentClass, &varNamePrefix, &storageType, &accessType, &varBase, &accessLimitTypes]
		(antlr4::tree::TerminalNode* const pNameId)
	{
		const std::string varNameShort = pNameId->getText();

		// Construct the variable's fully qualified name.
		const std::string varNameQualified = varNamePrefix + varNameShort;

		if(!pParentClass->variables.count(varNameShort))
		{
			ClassVarSymbol::Ptr pClassVar = ClassVarSymbol::New();

			// Copy the access limiters to the variable.
			pClassVar->accessLimitTypes = accessLimitTypes;

			// Copy the variable common data we resolved from the start of the declaration.
			pClassVar->base = varBase;

			// Cache the variable's name.
			pClassVar->base.varName = varNameShort;

			// Cache the remaining variable properties.
			pClassVar->pParent = pParentClass;
			pClassVar->accessType = accessType;
			pClassVar->storageType = storageType;

			// Add the new class variable to the global map in the symbol table.
			assert(m_pSymbolTable->allClassVariables.count(varNameQualified) == 0);
			m_pSymbolTable->allClassVariables.emplace(varNameQualified, pClassVar.get());

			// Move the new class symbol to the parent class.
			pParentClass->variables.emplace(varNameShort, std::move(pClassVar));

			// TODO: Visit the variable expression if there is one.
		}
		else
		{
			std::stringstream stream;
			stream << "duplicate class variable: '" << varNameQualified << "'";

			// Report the duplicate variable name as an error.
			m_pErrorHandler->Report(
				MessageCode::ErrorDuplicateVar,
				TokenSpan::WithSourceText(pNameId->getSymbol()),
				stream.str());
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

std::any SymbolTableGenerator::visitCtorDecl(HarlequinParser::CtorDeclContext* const pCtx)
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

std::any SymbolTableGenerator::visitMethodDecl(HarlequinParser::MethodDeclContext* const pCtx)
{
	if(m_pErrorHandler->EncounteredError())
	{
		// Do nothing once an error has been encountered.
		return defaultResult();
	}

	// There should always be a parent class when discovering class variables.
	assert(m_classStack.size() > 0);

	ClassSymbol* const pParentClass = m_classStack.back();

	auto* const pNameId = pCtx->Id();

	const std::string shortName = pNameId->getText();
	const std::string qualifiedName = pParentClass->qualifiedName + "." + shortName;

	if(!pParentClass->methods.count(shortName))
	{
		auto* const pMethodDeclSpecCtx = pCtx->methodDeclSpecSeq();
		auto* const pMethodArgSeqCtx = pCtx->methodArgSeq();
		auto* const pTypeNameDeclCtx = pCtx->typeNameDecl();
		auto* const pConstQualCtx = pCtx->constQualifier();
		auto* const pCodeBlockCtx = pCtx->codeBlock();

		auto* const pAccessSpecCtx = pMethodDeclSpecCtx->accessSpecifier();
		auto* const pStorageSpecCtx = pMethodDeclSpecCtx->storageSpecifier();
		auto* const pFunctionSpecCtx = pMethodDeclSpecCtx->functionSpecifier();

		auto* const pArrayTypeDeclCtx = pTypeNameDeclCtx->arrayTypeDecl();

		const detail::VarType returnVarType = CompilerUtil::GetVarType(pTypeNameDeclCtx->Id()->getText());
		const detail::ArrayType returnArrayType = CompilerUtil::GetArrayType(pArrayTypeDeclCtx ? pArrayTypeDeclCtx->getText() : "");

		const detail::FunctionType funcType = CompilerUtil::GetFunctionType(pFunctionSpecCtx ? pFunctionSpecCtx->getText() : "");
		const detail::StorageType storageType = CompilerUtil::GetStorageType(pStorageSpecCtx ? pStorageSpecCtx->getText() : "");

		const bool isConst = (pConstQualCtx != nullptr);
		bool isAbstract = false;

		// Resolve the method's access specifier.
		detail::AccessType accessType;
		detail::StringArray accessLimitTypes;
		_resolveAccessSpecifier(accessType, accessLimitTypes, pAccessSpecCtx);

		// Validate the function type.
		switch(funcType)
		{
			case detail::FunctionType::Native:
			{
				// Native methods are not allowed to have implementations.
				if(pCodeBlockCtx)
				{
					std::stringstream stream;
					stream << "native method with implementation: '" << qualifiedName << "'";

					// Report the provided code block as an error.
					m_pErrorHandler->Report(
						MessageCode::ErrorNativeMethodWithBody,
						TokenSpan::WithSourceText(pFunctionSpecCtx->getStart()),
						stream.str());
				}
				break;
			}

			case detail::FunctionType::Virtual:
				// Verify the function isn't marked both 'virtual' and 'static'.
				if(storageType == detail::StorageType::Static)
				{
					std::stringstream stream;
					stream << "method is marked both 'virtual' and 'static': '" << qualifiedName << "'";

					// Report the invalid method spec as an error.
					m_pErrorHandler->Report(
						MessageCode::ErrorStaticVirtualMethod,
						TokenSpan::WithSourceText(pFunctionSpecCtx->getStart()),
						stream.str());
				}

				// Virtual methods are allowed to not have an implementation,
				// but when they don't, they're considered abstract.
				isAbstract = (pCodeBlockCtx == nullptr);
				break;

			default:
			{
				// All other function types are required to have an implementation.
				if(!pCodeBlockCtx)
				{
					std::stringstream stream;
					stream << "method has no implementation: '" << qualifiedName << "'";

					// Report the missing code block as an error.
					m_pErrorHandler->Report(
						MessageCode::ErrorNoMethodImpl,
						TokenSpan::WithSourceText(pFunctionSpecCtx->getStart()),
						stream.str());
				}
				break;
			}
		}

		// Verify the method isn't marked both 'virtual' and 'const'.
		if(storageType == detail::StorageType::Static && isConst)
		{
			std::stringstream stream;
			stream << "method is marked both 'static' and 'const': '" << qualifiedName << "'";

			// Report the invalid method spec as an error.
			m_pErrorHandler->Report(
				MessageCode::ErrorStaticConstMethod,
				TokenSpan::WithSourceText(pConstQualCtx->getStop()),
				stream.str());
		}

		if(!m_pErrorHandler->EncounteredError())
		{
			MethodSymbol::Ptr pMethod = MethodSymbol::New();

			pMethod->shortName = shortName;
			pMethod->qualifiedName = qualifiedName;

			pMethod->accessLimitTypes = std::move(accessLimitTypes);

			pMethod->retVal.varType = returnVarType;
			pMethod->retVal.arrayType = returnArrayType;

			pMethod->accessType = accessType;
			pMethod->funcType = funcType;
			pMethod->storageType = storageType;

			pMethod->isConst = isConst;
			pMethod->isAbstract = isAbstract;

			// TODO: Read the method params into the map of local variables.
			// TODO: Cache the current method, then visit the code block.

			// Add the new method to the global map in the symbol table.
			assert(m_pSymbolTable->allMethods.count(qualifiedName) == 0);
			m_pSymbolTable->allMethods.emplace(qualifiedName, pMethod.get());

			// Move the new method symbol to the parent class.
			pParentClass->methods.emplace(shortName, std::move(pMethod));
		}
	}
	else
	{
		std::stringstream stream;
		stream << "duplicate method: '" << qualifiedName <<"'";

		// Report the duplicate method name as an error.
		m_pErrorHandler->Report(
			MessageCode::ErrorDuplicateMethod,
			TokenSpan::WithSourceText(pNameId->getSymbol()),
			stream.str());
	}

	return defaultResult();
}

//----------------------------------------------------------------------------------------------------------------------

inline void SymbolTableGenerator::_resolveAccessSpecifier(
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
						std::stringstream stream;
						stream << "access specifier is not compatible with the 'limited' access specifier: '" << pAccessBaseSpecCtx->getText() << "'";

						// Report the invalid access specifier combination.
						m_pErrorHandler->Report(
							MessageCode::ErrorInvalidAccessSpec,
							TokenSpan::WithSourceText(pAccessBaseSpecCtx->getStart()),
							stream.str());

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

inline void SymbolTableGenerator::_resolveVariableCommonData(
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

inline void SymbolTableGenerator::_resolveQualifiedIdArray(
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
