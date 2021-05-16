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

#include "Object.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonObject* XenonObject::CreateTemplate(XenonString* const pTypeName, const MemberDefinitionMap& definitions)
{
	assert(pTypeName != nullptr);

	XenonObject* const pOutput = new XenonObject();
	assert(pOutput != nullptr);

	pOutput->pTypeName = pTypeName;
	pOutput->pSchema = nullptr;
	pOutput->definitions = definitions;

	// Initialize the members array and reserve enough space for each of them.
	XenonValue::HandleArray::Initialize(pOutput->members);
	XenonValue::HandleArray::Reserve(pOutput->members, definitions.Size());
	pOutput->members.count = definitions.Size();

	// Initialize the members to null values.
	for(size_t i = 0; i < pOutput->members.count; ++i)
	{
		pOutput->members.pData[i] = XenonValue::CreateNull();
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

XenonObject* XenonObject::CreateInstance(XenonObject* const pSchema)
{
	assert(pSchema != nullptr);

	XenonObject* const pOutput = XenonObject::prv_createObject(pSchema);
	assert(pOutput != nullptr);

	pOutput->pSchema = pSchema;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonObject* XenonObject::CreateCopy(XenonObject* const pObject)
{
	assert(pObject != nullptr);

	XenonObject* const pOutput = XenonObject::prv_createObject(pObject);
	assert(pOutput != nullptr);

	pOutput->pSchema = pObject->pSchema;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonObject::Dispose(XenonObject* const pObject)
{
	assert(pObject != nullptr);

	// Release all the member names from the definition table.
	for(auto& kv : pObject->definitions)
	{
		if(kv.key)
		{
			XenonString::Release(kv.key);
		}
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

XenonValueHandle XenonObject::GetMemberValue(XenonObject* const pObject, const uint32_t memberIndex, int* const pOutResult)
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

XenonObject::MemberDefinition XenonObject::GetMemberDefinition(
	XenonObject* const pObject,
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

	return kv->value;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonObject::SetMemberValue(XenonObject* const pObject, const uint32_t memberIndex, XenonValueHandle hValue)
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

XenonObject* XenonObject::prv_createObject(XenonObject* const pOriginalObject)
{
	assert(pOriginalObject != nullptr);

	XenonObject* const pOutput = new XenonObject();
	assert(pOutput != nullptr);

	pOutput->pTypeName = pOriginalObject->pTypeName;
	pOutput->definitions = pOriginalObject->definitions;

	// Add a reference for the type name.
	XenonString::AddRef(pOutput->pTypeName);

	// Add a reference for each member name.
	for(auto& kv : pOutput->definitions)
	{
		if(kv.key)
		{
			XenonString::AddRef(kv.key);
		}
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

void* XenonObject::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonObject::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
