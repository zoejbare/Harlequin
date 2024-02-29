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

#include "CompilerUtil.hpp"

#include <algorithm>
#include <sstream>

//----------------------------------------------------------------------------------------------------------------------

detail::AccessDecl::Type CompilerUtil::GetAccessType(const std::string& accessType)
{
	return (accessType == "public")
		? detail::AccessDecl::Type::Public
		: (accessType == "protected")
			? detail::AccessDecl::Type::Protected
			: detail::AccessDecl::Type::Private;
}

//----------------------------------------------------------------------------------------------------------------------

detail::ArrayDecl::Type CompilerUtil::GetArrayType(const std::string& arraySpec)
{
	if(arraySpec.empty())
	{
		return detail::ArrayDecl::Type::None;
	}

	const size_t commaCount = std::count(arraySpec.begin(), arraySpec.end(), ',');

	switch(commaCount)
	{
		case 0:  return detail::ArrayDecl::Type::Linear;
		case 1:  return detail::ArrayDecl::Type::Grid2D;
		default: return detail::ArrayDecl::Type::Grid3D;
	}
}

//----------------------------------------------------------------------------------------------------------------------

detail::ClassDecl::Type CompilerUtil::GetClassType(const std::string& classType)
{
	return (classType == "interface")
		? detail::ClassDecl::Type::Interface
		: detail::ClassDecl::Type::Class;
}

//----------------------------------------------------------------------------------------------------------------------

detail::FuncDecl::Type CompilerUtil::GetFunctionType(const std::string& funcType)
{
	return (funcType == "inline")
		? detail::FuncDecl::Type::Inline
		: (funcType == "virtual")
			? detail::FuncDecl::Type::Virtual
			: (funcType == "native")
				? detail::FuncDecl::Type::Native
				: detail::FuncDecl::Type::Default;
}

//----------------------------------------------------------------------------------------------------------------------

detail::StorageDecl::Type CompilerUtil::GetStorageType(const std::string& storageType)
{
	return (storageType == "static")
		? detail::StorageDecl::Type::Static
		: detail::StorageDecl::Type::Default;
}

//----------------------------------------------------------------------------------------------------------------------

detail::VarDecl::Type CompilerUtil::GetVarType(const std::string& typeName)
{
	if(typeName == "int8")
	{
		return detail::VarDecl::Type::Int8;
	}
	else if(typeName == "int16")
	{
		return detail::VarDecl::Type::Int16;
	}
	else if(typeName == "int32")
	{
		return detail::VarDecl::Type::Int32;
	}
	else if(typeName == "int64")
	{
		return detail::VarDecl::Type::Int64;
	}
	else if(typeName == "uint8")
	{
		return detail::VarDecl::Type::Uint8;
	}
	else if(typeName == "uint16")
	{
		return detail::VarDecl::Type::Uint16;
	}
	else if(typeName == "uint32")
	{
		return detail::VarDecl::Type::Uint32;
	}
	else if(typeName == "uint64")
	{
		return detail::VarDecl::Type::Uint64;
	}
	else if(typeName == "float32")
	{
		return detail::VarDecl::Type::Float32;
	}
	else if(typeName == "float64")
	{
		return detail::VarDecl::Type::Float64;
	}
	else if(typeName == "bool")
	{
		return detail::VarDecl::Type::Bool;
	}
	else if(typeName == "string")
	{
		return detail::VarDecl::Type::String;
	}
	else if(typeName == "native")
	{
		return detail::VarDecl::Type::Native;
	}

	return detail::VarDecl::Type::Object;
}

//----------------------------------------------------------------------------------------------------------------------

std::string CompilerUtil::GetTypeSignature(const std::string& typeName, const detail::ArrayDecl::Type arrayType)
{
	std::stringstream stream;
	stream << typeName;

	switch(arrayType)
	{
		case detail::ArrayDecl::Type::Linear: stream << "[]";   break;
		case detail::ArrayDecl::Type::Grid2D: stream << "[,]";  break;
		case detail::ArrayDecl::Type::Grid3D: stream << "[,,]"; break;

		default:
			break;
	}

	return stream.str();
}

//----------------------------------------------------------------------------------------------------------------------
