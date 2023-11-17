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

#include "detail/AccessType.hpp"
#include "detail/StorageType.hpp"
#include "detail/StringArray.hpp"
#include "detail/VariableBase.hpp"

#include <memory>
#include <string>
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------------------

struct ClassSymbol;
struct ClassVarSymbol
{
	typedef std::unique_ptr<ClassVarSymbol>                  Ptr;
	typedef std::unordered_map<std::string, Ptr>             PtrMap;
	typedef std::unordered_map<std::string, ClassVarSymbol*> RawPtrMap;

	inline static Ptr New()
	{
		return std::make_unique<ClassVarSymbol>();
	}

	ClassSymbol* pParent;

	detail::StringArray accessLimits;
	detail::VariableBase base;

	detail::AccessType accessType;
	detail::StorageType storageType;
};

//----------------------------------------------------------------------------------------------------------------------
