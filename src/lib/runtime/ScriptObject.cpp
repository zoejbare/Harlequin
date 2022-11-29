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

#include "ScriptObject.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonScriptObject* XenonScriptObject::CreateSchema(XenonString* const pTypeName, const MemberDefinitionMap& definitions)
{
	assert(pTypeName != nullptr);

	XenonScriptObject* const pOutput = new XenonScriptObject();
	assert(pOutput != nullptr);

	pOutput->pTypeName = pTypeName;
	pOutput->pSchema = nullptr;
	pOutput->definitions = definitions;

	// Track string references.
	XenonString::AddRef(pOutput->pTypeName);

	for(auto& kv : pOutput->definitions)
	{
		XenonString::AddRef(XENON_MAP_ITER_KEY(kv));
	}

	const size_t defCount = XENON_MAP_FUNC_SIZE(definitions);

	// Initialize the members array and reserve enough space for each of them.
	XenonValue::HandleArray::Initialize(pOutput->members);
	XenonValue::HandleArray::Reserve(pOutput->members, defCount);
	pOutput->members.count = defCount;

	// Initialize the members to null values.
	for(size_t i = 0; i < defCount; ++i)
	{
		pOutput->members.pData[i] = XenonValue::CreateNull();
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonScriptObject* XenonScriptObject::CreateInstance(XenonScriptObject* const pSchema)
{
	assert(pSchema != nullptr);

	XenonScriptObject* const pOutput = XenonScriptObject::prv_createObject(pSchema);
	assert(pOutput != nullptr);

	pOutput->pSchema = pSchema;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonScriptObject* XenonScriptObject::CreateCopy(XenonScriptObject* const pObject)
{
	assert(pObject != nullptr);

	XenonScriptObject* const pOutput = XenonScriptObject::prv_createObject(pObject);
	assert(pOutput != nullptr);

	pOutput->pSchema = pObject->pSchema;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonScriptObject::Dispose(XenonScriptObject* const pObject)
{
	assert(pObject != nullptr);

	// Release all the member names from the definition table.
	for(auto& kv : pObject->definitions)
	{
		XenonString::Release(XENON_MAP_ITER_KEY(kv));
	}

	// Dispose of the member values.
	for(size_t i = 0; i < pObject->members.count; ++i)
	{
		XenonValueAbandon(pObject->members.pData[i]);
	}

	XenonValue::HandleArray::Dispose(pObject->members);
	XenonString::Release(pObject->pTypeName);

	delete pObject;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonScriptObject::GetMemberValue(XenonScriptObject* const pObject, const uint32_t memberIndex, int* const pOutResult)
{
	assert(pObject != nullptr);

	if(memberIndex < uint32_t(pObject->members.count))
	{
		(*pOutResult) = XENON_SUCCESS;

		return pObject->members.pData[memberIndex];
	}

	(*pOutResult) = XENON_ERROR_INDEX_OUT_OF_RANGE;

	return XENON_VALUE_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

XenonScriptObject::MemberDefinition XenonScriptObject::GetMemberDefinition(
	XenonScriptObject* const pObject,
	XenonString* const pMemberName,
	int* const pOutResult
)
{
	assert(pObject != nullptr);
	assert(pMemberName != nullptr);
	assert(pOutResult != nullptr);

	auto kv = pObject->definitions.find(pMemberName);
	if(kv == pObject->definitions.end())
	{
		(*pOutResult) = XENON_ERROR_KEY_DOES_NOT_EXIST;

		MemberDefinition dummyDef = { 0, 0 };
		return dummyDef;
	}

	(*pOutResult) = XENON_SUCCESS;

	return XENON_MAP_ITER_PTR_VALUE(kv);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonScriptObject::SetMemberValue(XenonScriptObject* const pObject, const uint32_t memberIndex, XenonValueHandle hValue)
{
	assert(pObject != nullptr);
	assert(hValue != XENON_VALUE_HANDLE_NULL);

	if(memberIndex < uint32_t(pObject->members.count))
	{
		pObject->members.pData[memberIndex] = hValue;
		return XENON_SUCCESS;
	}

	return XENON_ERROR_INDEX_OUT_OF_RANGE;
}

//----------------------------------------------------------------------------------------------------------------------

XenonScriptObject* XenonScriptObject::prv_createObject(XenonScriptObject* const pOriginalObject)
{
	assert(pOriginalObject != nullptr);

	XenonScriptObject* const pOutput = new XenonScriptObject();
	assert(pOutput != nullptr);

	pOutput->pTypeName = pOriginalObject->pTypeName;
	pOutput->definitions = pOriginalObject->definitions;

	// Object user data should always start out as null.
	pOutput->pUserData = nullptr;

	// Add a reference for the type name.
	XenonString::AddRef(pOutput->pTypeName);

	// Add a reference for each member name.
	for(auto& kv : pOutput->definitions)
	{
		XenonString::AddRef(XENON_MAP_ITER_KEY(kv));
	}

	// Initialize the members array and reserve enough space for each of them.
	XenonValue::HandleArray::Initialize(pOutput->members);
	XenonValue::HandleArray::Reserve(pOutput->members, pOriginalObject->members.count);
	pOutput->members.count = pOriginalObject->members.count;

	// Copy the member values to the new array.
	for(size_t i = 0; i < pOutput->members.count; ++i)
	{
		pOutput->members.pData[i] = pOriginalObject->members.pData[i];
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonScriptObject::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonScriptObject::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
