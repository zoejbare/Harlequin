//
// Copyright (c) 2021, Zoe J. Bare
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

#include "Value.hpp"

#include "../base/String.hpp"
#include "../common/StlAllocator.hpp"

#include <SkipProbe/SkipProbe.hpp>

//----------------------------------------------------------------------------------------------------------------------

struct XenonObject
{
	struct MemberDefinition
	{
		uint32_t bindingIndex;
		uint8_t valueType;
	};

	typedef SkipProbe::HashMap<
		XenonString*,
		MemberDefinition,
		XenonString::StlHash,
		XenonString::StlCompare,
		XenonStlAllocator<SkipProbe::LinkedNode<XenonString*, MemberDefinition>>
	> MemberDefinitionMap;

	typedef SkipProbe::HashMap<
		XenonString*,
		XenonObject*,
		XenonString::StlHash,
		XenonString::StlCompare,
		XenonStlAllocator<SkipProbe::LinkedNode<XenonString*, XenonObject*>>
	> StringToPtrMap;

	static XenonObject* CreateSchema(XenonString* pTypeName, const MemberDefinitionMap& definitions);
	static XenonObject* CreateInstance(XenonObject* const pSchema);
	static XenonObject* CreateCopy(XenonObject* const pObject);
	static void Dispose(XenonObject* const pObject);

	static XenonValueHandle GetMemberValue(XenonObject* const pObject, const uint32_t memberIndex, int* const pOutResult);
	static MemberDefinition GetMemberDefinition(XenonObject* const pObject, XenonString* const pMemberName, int* const pOutResult);

	static int SetMemberValue(XenonObject* const pObject, const uint32_t memberIndex, XenonValueHandle hValue);

	static XenonObject* prv_createObject(XenonObject*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonString* pTypeName;
	XenonObject* pSchema;

	MemberDefinitionMap definitions;

	XenonValue::HandleArray members;
};

//----------------------------------------------------------------------------------------------------------------------
