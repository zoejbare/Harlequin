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

detail::TypeDecl::Type CompilerUtil::GetDataType(const std::string& typeName)
{
	if(typeName == "int8")
	{
		return detail::TypeDecl::Type::Int8;
	}
	else if(typeName == "int16")
	{
		return detail::TypeDecl::Type::Int16;
	}
	else if(typeName == "int32")
	{
		return detail::TypeDecl::Type::Int32;
	}
	else if(typeName == "int64")
	{
		return detail::TypeDecl::Type::Int64;
	}
	else if(typeName == "uint8")
	{
		return detail::TypeDecl::Type::Uint8;
	}
	else if(typeName == "uint16")
	{
		return detail::TypeDecl::Type::Uint16;
	}
	else if(typeName == "uint32")
	{
		return detail::TypeDecl::Type::Uint32;
	}
	else if(typeName == "uint64")
	{
		return detail::TypeDecl::Type::Uint64;
	}
	else if(typeName == "float32")
	{
		return detail::TypeDecl::Type::Float32;
	}
	else if(typeName == "float64")
	{
		return detail::TypeDecl::Type::Float64;
	}
	else if(typeName == "bool")
	{
		return detail::TypeDecl::Type::Bool;
	}
	else if(typeName == "string")
	{
		return detail::TypeDecl::Type::String;
	}
	else if(typeName == "native")
	{
		return detail::TypeDecl::Type::Native;
	}

	return detail::TypeDecl::Type::Object;
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

std::string CompilerUtil::GetTypeSignature(const detail::VarDecl& varDecl, const detail::ConstDecl& constDecl)
{
	std::stringstream stream;

	if(constDecl.type != detail::ConstDecl::Type::None && varDecl.typeDecl.type == detail::TypeDecl::Type::Object)
	{
		// Only objects will need to care about the 'const' qualifier since their underlying data is not implicitly immutable.
		// Native types also don't need this since they are exclusively manipulated from managed code and no qualifier here
		// would be capable of enforcing whatever native code wants to do to them.
		stream << "const ";
	}

	switch(varDecl.typeDecl.type)
	{
		case detail::TypeDecl::Type::Int8:    stream << "int8";                      break;
		case detail::TypeDecl::Type::Int16:   stream << "int16";                     break;
		case detail::TypeDecl::Type::Int32:   stream << "int32";                     break;
		case detail::TypeDecl::Type::Int64:   stream << "int64";                     break;
		case detail::TypeDecl::Type::Uint8:   stream << "uint8";                     break;
		case detail::TypeDecl::Type::Uint16:  stream << "uint16";                    break;
		case detail::TypeDecl::Type::Uint32:  stream << "uint32";                    break;
		case detail::TypeDecl::Type::Uint64:  stream << "uint64";                    break;
		case detail::TypeDecl::Type::Float32: stream << "float32";                   break;
		case detail::TypeDecl::Type::Float64: stream << "float64";                   break;
		case detail::TypeDecl::Type::Bool:    stream << "bool";                      break;
		case detail::TypeDecl::Type::String:  stream << "string";                    break;
		case detail::TypeDecl::Type::Object:  stream << varDecl.typeDecl.objectName; break;
		case detail::TypeDecl::Type::Native:  stream << "native";                    break;

		default:
			// This should never happen.
			assert(false);
			break;
	}

	switch(varDecl.arrayDecl.type)
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
