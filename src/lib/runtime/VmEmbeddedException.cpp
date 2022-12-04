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

#include "Vm.hpp"

//----------------------------------------------------------------------------------------------------------------------

void HqVm::prv_setupEmbeddedExceptions(HqVmHandle hVm)
{
	// Can't think of a more elegant way to handle this other than manually creating object values that match the
	// definitions of the base exception types in the standard library. As long as the type names and member definitions
	// match their equivalents in the standard library, this should work without issue, but the trick will be keeping
	// them all in sync with each other. The exception (no pun intended) to this rule is the 'FatalError' exception.
	// That one should is not intended to be catchable, so the standard library won't include a definition for it.
	// It's still possible for a user to implement the 'FatalError' type on their own, but as a convention, any time
	// the runtime raises that exception, it will be additionally marked as fatal to prevent it from being caught.

	HqScriptObject::MemberDefinitionMap memberDefs;
	HqScriptObject::MemberDefinition def;

#define HQ_EXCEPTION_ADD_MEMBER(value_type, name, index) \
	{ \
		def.bindingIndex = index; \
		def.valueType = HQ_VALUE_TYPE_ ## value_type; \
		HQ_MAP_FUNC_INSERT(memberDefs, HqString::Create(name), def); \
	}

#define HQ_EMBEDDED_EXCEPTION(type, name) \
	{ \
		HqString* const pTypeName = HqString::Create("Harlequin.System.Exception." name); \
		HqScriptObject* const pSchema = HqScriptObject::CreateSchema(pTypeName, memberDefs); \
		HqString::Release(pTypeName); \
		HQ_MAP_FUNC_INSERT(hVm->embeddedExceptions, HQ_STANDARD_EXCEPTION_ ## type, pSchema); \
	}

	// Add the object member data common to each exception type.
	HQ_EXCEPTION_ADD_MEMBER(STRING, "_message", 0);

	// Register the embedded exception types.
	HQ_EMBEDDED_EXCEPTION(RUNTIME_ERROR, "RuntimeError");
	HQ_EMBEDDED_EXCEPTION(TYPE_ERROR, "TypeError");
	HQ_EMBEDDED_EXCEPTION(DIVIDE_BY_ZERO_ERROR, "DivideByZeroError");

	// Clean up the member names.
	for(auto& kv : memberDefs)
	{
		HqString::Release(HQ_MAP_ITER_KEY(kv));
	}

#undef HQ_EMBEDDED_EXCEPTION
#undef HQ_EXCEPTION_ADD_MEMBER
}

//----------------------------------------------------------------------------------------------------------------------
