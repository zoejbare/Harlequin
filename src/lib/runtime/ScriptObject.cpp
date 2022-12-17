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
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

HqScriptObject* HqScriptObject::CreateSchema(HqString* const pTypeName, const MemberDefinitionMap& definitions)
{
	assert(pTypeName != nullptr);

	HqScriptObject* const pOutput = new HqScriptObject();
	assert(pOutput != nullptr);

	pOutput->pTypeName = pTypeName;
	pOutput->pSchema = nullptr;

	MemberDefinitionMap::Initialize(pOutput->definitions);
	MemberDefinitionMap::Copy(pOutput->definitions, definitions);

	// Track string references.
	HqString::AddRef(pOutput->pTypeName);

	// Track the member name string references.
	{
		MemberDefinitionMap::Iterator iter;
		while(MemberDefinitionMap::IterateNext(pOutput->definitions, iter))
		{
			HqString::AddRef(iter.pData->key);
		}
	}

	const size_t defCount = definitions.count;

	// Initialize the members array and reserve enough space for each of them.
	HqValue::HandleArray::Initialize(pOutput->members);
	HqValue::HandleArray::Reserve(pOutput->members, defCount);
	pOutput->members.count = defCount;

	// Initialize the members to null values.
	memset(pOutput->members.pData, 0, sizeof(HqValueHandle) * defCount);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqScriptObject* HqScriptObject::CreateInstance(HqScriptObject* const pSchema)
{
	assert(pSchema != nullptr);

	HqScriptObject* const pOutput = HqScriptObject::prv_createObject(pSchema);
	assert(pOutput != nullptr);

	pOutput->pSchema = pSchema;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqScriptObject* HqScriptObject::CreateCopy(HqScriptObject* const pObject)
{
	assert(pObject != nullptr);

	HqScriptObject* const pOutput = HqScriptObject::prv_createObject(pObject);
	assert(pOutput != nullptr);

	pOutput->pSchema = pObject->pSchema;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqScriptObject::Dispose(HqScriptObject* const pObject)
{
	assert(pObject != nullptr);

	// Release all the member names from the definition table.
	if(!pObject->pSchema)
	{
		MemberDefinitionMap::Iterator iter;
		while(MemberDefinitionMap::IterateNext(pObject->definitions, iter))
		{
			HqString::Release(iter.pData->key);
		}

		MemberDefinitionMap::Dispose(pObject->definitions);
	}

	HqValue::HandleArray::Dispose(pObject->members);
	HqString::Release(pObject->pTypeName);

	delete pObject;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqScriptObject::GetMemberValue(HqScriptObject* const pObject, const uint32_t memberIndex, int* const pOutResult)
{
	assert(pObject != nullptr);

	if(memberIndex < uint32_t(pObject->members.count))
	{
		(*pOutResult) = HQ_SUCCESS;

		return pObject->members.pData[memberIndex];
	}

	(*pOutResult) = HQ_ERROR_INDEX_OUT_OF_RANGE;

	return HQ_VALUE_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

HqScriptObject::MemberDefinition HqScriptObject::GetMemberDefinition(
	HqScriptObject* const pObject,
	HqString* const pMemberName,
	int* const pOutResult
)
{
	assert(pObject != nullptr);
	assert(pMemberName != nullptr);
	assert(pOutResult != nullptr);

	// If the input object has a schema, we should use it's definition map since that will be the real one.
	HqScriptObject::MemberDefinitionMap& definitions = (pObject->pSchema) 
		? pObject->pSchema->definitions 
		: pObject->definitions;

	HqScriptObject::MemberDefinition def = { 0, 0 };
	if(!HqScriptObject::MemberDefinitionMap::Get(definitions, pMemberName, def))
	{
		(*pOutResult) = HQ_ERROR_KEY_DOES_NOT_EXIST;
	}
	else
	{
		(*pOutResult) = HQ_SUCCESS;
	}

	return def;
}

//----------------------------------------------------------------------------------------------------------------------

int HqScriptObject::SetMemberValue(HqScriptObject* const pObject, const uint32_t memberIndex, HqValueHandle hValue)
{
	assert(pObject != nullptr);
	assert(hValue != HQ_VALUE_HANDLE_NULL);

	if(memberIndex < uint32_t(pObject->members.count))
	{
		pObject->members.pData[memberIndex] = hValue;
		return HQ_SUCCESS;
	}

	return HQ_ERROR_INDEX_OUT_OF_RANGE;
}

//----------------------------------------------------------------------------------------------------------------------

HqScriptObject* HqScriptObject::prv_createObject(HqScriptObject* const pOriginalObject)
{
	assert(pOriginalObject != nullptr);

	HqScriptObject* const pOutput = new HqScriptObject();
	assert(pOutput != nullptr);

	pOutput->pTypeName = pOriginalObject->pTypeName;

	// The schema's definition map will be used instead to save memory.
	MemberDefinitionMap::Initialize(pOutput->definitions);

	// Object user data should always start out as null.
	pOutput->pUserData = nullptr;

	// Add a reference for the type name.
	HqString::AddRef(pOutput->pTypeName);

	// Initialize the members array and reserve enough space for each of them.
	HqValue::HandleArray::Initialize(pOutput->members);
	HqValue::HandleArray::Reserve(pOutput->members, pOriginalObject->members.count);
	pOutput->members.count = pOriginalObject->members.count;

	// Copy the member values to the new array.
	for(size_t i = 0; i < pOutput->members.count; ++i)
	{
		pOutput->members.pData[i] = pOriginalObject->members.pData[i];
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqScriptObject::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqScriptObject::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
