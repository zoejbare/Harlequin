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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "detail/AccessDecl.hpp"
#include "detail/ArrayDecl.hpp"
#include "detail/ClassDecl.hpp"
#include "detail/FuncDecl.hpp"
#include "detail/StorageDecl.hpp"
#include "detail/VarDecl.hpp"

#include "../../Harlequin.h"

#include <string>

//----------------------------------------------------------------------------------------------------------------------

class ParserErrorHandler;

class CompilerUtil
{
public:

	CompilerUtil() = delete;
	CompilerUtil(const CompilerUtil&) = delete;
	CompilerUtil(CompilerUtil&&) = delete;

	static detail::AccessDecl::Type GetAccessType(const std::string& accessType);
	static detail::ArrayDecl::Type GetArrayType(const std::string& arrayType);
	static detail::ClassDecl::Type GetClassType(const std::string& classType);
	static detail::FuncDecl::Type GetFunctionType(const std::string& funcType);
	static detail::StorageDecl::Type GetStorageType(const std::string& storageType);
	static detail::VarDecl::Type GetVarType(const std::string& typeName);

	static std::string GetTypeSignature(const std::string& typeName, detail::ArrayDecl::Type arrayType);
};

//----------------------------------------------------------------------------------------------------------------------
