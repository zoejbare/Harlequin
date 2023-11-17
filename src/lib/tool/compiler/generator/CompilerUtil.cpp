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

detail::AccessType CompilerUtil::GetAccessType(const std::string& accessType)
{
	return (accessType == "public")
		? detail::AccessType::Public
		: (accessType == "protected")
			? detail::AccessType::Protected
			: detail::AccessType::Private;
}

//----------------------------------------------------------------------------------------------------------------------

detail::ArrayType CompilerUtil::GetArrayType(const std::string& arraySpec)
{
	if(arraySpec.empty())
	{
		return detail::ArrayType::None;
	}

	const size_t commaCount = std::count(arraySpec.begin(), arraySpec.end(), ',');

	switch(commaCount)
	{
		case 0:  return detail::ArrayType::Linear;
		case 1:  return detail::ArrayType::Grid2D;
		default: return detail::ArrayType::Grid3D;
	}
}

//----------------------------------------------------------------------------------------------------------------------

detail::ClassType CompilerUtil::GetClassType(const std::string& classType)
{
	return (classType == "interface")
		? detail::ClassType::Interface
		: detail::ClassType::Class;
}

//----------------------------------------------------------------------------------------------------------------------

detail::FunctionType CompilerUtil::GetFunctionType(const std::string& funcType)
{
	return (funcType == "inline")
		? detail::FunctionType::Inline
		: (funcType == "virtual")
			? detail::FunctionType::Virtual
			: (funcType == "native")
				? detail::FunctionType::Native
				: detail::FunctionType::Default;
}

//----------------------------------------------------------------------------------------------------------------------

detail::StorageType CompilerUtil::GetStorageType(const std::string& storageType)
{
	return (storageType == "static")
		? detail::StorageType::Static
		: detail::StorageType::Default;
}

//----------------------------------------------------------------------------------------------------------------------

detail::VarType CompilerUtil::GetVarType(const std::string& typeName)
{
	if(typeName == "int8")
	{
		return detail::VarType::Int8;
	}
	else if(typeName == "int16")
	{
		return detail::VarType::Int16;
	}
	else if(typeName == "int32")
	{
		return detail::VarType::Int32;
	}
	else if(typeName == "int64")
	{
		return detail::VarType::Int64;
	}
	else if(typeName == "uint8")
	{
		return detail::VarType::Uint8;
	}
	else if(typeName == "uint16")
	{
		return detail::VarType::Uint16;
	}
	else if(typeName == "uint32")
	{
		return detail::VarType::Uint32;
	}
	else if(typeName == "uint64")
	{
		return detail::VarType::Uint64;
	}
	else if(typeName == "float32")
	{
		return detail::VarType::Float32;
	}
	else if(typeName == "float64")
	{
		return detail::VarType::Float64;
	}
	else if(typeName == "bool")
	{
		return detail::VarType::Bool;
	}
	else if(typeName == "string")
	{
		return detail::VarType::String;
	}
	else if(typeName == "native")
	{
		return detail::VarType::Native;
	}

	return detail::VarType::Object;
}

//----------------------------------------------------------------------------------------------------------------------

std::string CompilerUtil::GetVarTypeSignature(const std::string& typeName, const detail::ArrayType& arrayType)
{
	std::stringstream stream;
	stream << typeName;

	switch(arrayType)
	{
		case detail::ArrayType::Linear: stream << "[]";   break;
		case detail::ArrayType::Grid2D: stream << "[,]";  break;
		case detail::ArrayType::Grid3D: stream << "[,,]"; break;

		default:
			break;
	}

	return stream.str();
}

//----------------------------------------------------------------------------------------------------------------------
