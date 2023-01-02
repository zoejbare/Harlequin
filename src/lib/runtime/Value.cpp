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

#include "Value.hpp"

#include "ScriptObject.hpp"
#include "Vm.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

#define _HQ_ARRAY_DEFAULT_CAPACITY 8

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateBool(HqVmHandle hVm, const bool value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_BOOL, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.boolean = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateInt8(HqVmHandle hVm, const int8_t value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_INT8, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.int8 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateInt16(HqVmHandle hVm, const int16_t value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_INT16, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.int16 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateInt32(HqVmHandle hVm, const int32_t value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_INT32, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.int32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateInt64(HqVmHandle hVm, const int64_t value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_INT64, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.int64 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateUint8(HqVmHandle hVm, const uint8_t value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_UINT8, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.uint8 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateUint16(HqVmHandle hVm, const uint16_t value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_UINT16, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.uint16 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateUint32(HqVmHandle hVm, const uint32_t value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_UINT32, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.uint32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateUint64(HqVmHandle hVm, const uint64_t value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_UINT64, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.uint64 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateFloat32(HqVmHandle hVm, const float value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_FLOAT32, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.float32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateFloat64(HqVmHandle hVm, const double value)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_FLOAT64, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.float64 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateString(HqVmHandle hVm, const char* const string)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(string != nullptr);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_STRING, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.pString = HqString::Create(string);
	if(!pOutput->as.pString)
	{
		delete pOutput;
		return HQ_VALUE_HANDLE_NULL;
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateString(HqVmHandle hVm, HqString* const pString)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pString != nullptr);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_STRING, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.pString = pString;

	HqString::AddRef(pString);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateObject(HqVmHandle hVm, HqScriptObject* const pObjectSchema)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(pObjectSchema != nullptr);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_OBJECT, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	// Make a copy of the input object template for this value.
	pOutput->as.pObject = HqScriptObject::CreateInstance(pObjectSchema);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateFunction(HqVmHandle hVm, HqFunctionHandle hFunction)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hFunction != nullptr);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_FUNCTION, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.hFunction = hFunction;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateArray(HqVmHandle hVm, const size_t count)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_ARRAY, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	const size_t capacity = (count > _HQ_ARRAY_DEFAULT_CAPACITY) ? count : _HQ_ARRAY_DEFAULT_CAPACITY;

	// Initialize the array storage.
	HandleArray::Initialize(pOutput->as.array);
	HandleArray::Reserve(pOutput->as.array, capacity);

	pOutput->as.array.count = count;

	if(pOutput->as.array.count > 0)
	{
		// Initialize the array memory so it's not filled with garbage data.
		memset(pOutput->as.array.pData, 0, sizeof(HqValueHandle) * pOutput->as.array.count);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::CreateNative(
	HqVmHandle hVm,
	void* const pNativeObject,
	HqCallbackNativeValueCopy onCopy,
	HqCallbackNativeValueDestruct onDestruct
)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(onCopy != nullptr);
	assert(onDestruct != nullptr);

	HqValue* const pOutput = prv_onCreate(HQ_VALUE_TYPE_NATIVE, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	pOutput->as.native.pObject = pNativeObject;
	pOutput->as.native.onCopy = onCopy;
	pOutput->as.native.onDestruct = onDestruct;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqValue::Copy(HqVmHandle hVm, HqValueHandle hValue)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	if(!hValue)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	HqValue* pOutput = prv_onCreate(hValue->type, hVm);
	if(!pOutput)
	{
		return HQ_VALUE_HANDLE_NULL;
	}

	switch(pOutput->type)
	{
		case HQ_VALUE_TYPE_BOOL:
			pOutput->as.boolean = hValue->as.boolean;
			break;

		case HQ_VALUE_TYPE_INT8:
			pOutput->as.int8 = hValue->as.int8;
			break;

		case HQ_VALUE_TYPE_INT16:
			pOutput->as.int16 = hValue->as.int16;
			break;

		case HQ_VALUE_TYPE_INT32:
			pOutput->as.int32 = hValue->as.int32;
			break;

		case HQ_VALUE_TYPE_INT64:
			pOutput->as.int64 = hValue->as.int64;
			break;

		case HQ_VALUE_TYPE_UINT8:
			pOutput->as.uint8 = hValue->as.uint8;
			break;

		case HQ_VALUE_TYPE_UINT16:
			pOutput->as.uint16 = hValue->as.uint16;
			break;

		case HQ_VALUE_TYPE_UINT32:
			pOutput->as.uint32 = hValue->as.uint32;
			break;

		case HQ_VALUE_TYPE_UINT64:
			pOutput->as.uint64 = hValue->as.uint64;
			break;

		case HQ_VALUE_TYPE_FLOAT32:
			pOutput->as.float32 = hValue->as.float32;
			break;

		case HQ_VALUE_TYPE_FLOAT64:
			pOutput->as.float64 = hValue->as.float64;
			break;

		case HQ_VALUE_TYPE_STRING:
			pOutput->as.pString = HqString::Create(hValue->as.pString->data);
			break;

		case HQ_VALUE_TYPE_FUNCTION:
			pOutput->as.hFunction = hValue->as.hFunction;
			break;

		case HQ_VALUE_TYPE_OBJECT:
			pOutput->as.pObject = HqScriptObject::CreateCopy(hValue->as.pObject);
			break;

		case HQ_VALUE_TYPE_ARRAY:
			HandleArray::Initialize(pOutput->as.array);
			HandleArray::Reserve(
				pOutput->as.array,
				(hValue->as.array.count > _HQ_ARRAY_DEFAULT_CAPACITY)
					? hValue->as.array.count
					: _HQ_ARRAY_DEFAULT_CAPACITY
			);

			pOutput->as.array.count = hValue->as.array.count;

			if(pOutput->as.array.count > 0)
			{
				memcpy(
					pOutput->as.array.pData,
					hValue->as.array.pData,
					sizeof(HqValueHandle) * pOutput->as.array.count
				);
			}
			break;

		case HQ_VALUE_TYPE_NATIVE:
			pOutput->as.native.onCopy = hValue->as.native.onCopy;
			pOutput->as.native.onDestruct = hValue->as.native.onDestruct;

			pOutput->as.native.onCopy(&pOutput->as.native.pObject, hValue->as.native.pObject);
			break;

		default:
			// This should never happen. If it does, it indicates an unimplemented type here.
			assert(false);
			delete pOutput;
			return HQ_VALUE_HANDLE_NULL;
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqString* HqValue::GetDebugString(HqValueHandle hValue)
{
	// Temporary buffer used by the numerical primitive types. We know the exact maximum size for the non-float types,
	// but the float types are a little more nebulous. It's easier to just pre-allocate a buffer that we know will
	// definitely be large enough for all of those types (as well as the object type) and call it a day.
	char str[256] = { '<', 'n', 'u', 'l', 'l', '>', '\0' };

	if(hValue)
	{
		switch(hValue->type)
		{
			case HQ_VALUE_TYPE_INT8:
				snprintf(str, sizeof(str), "<int8: %" PRId8 ">", hValue->as.int8);
				break;

			case HQ_VALUE_TYPE_INT16:
				snprintf(str, sizeof(str), "<int16: %" PRId16 ">", hValue->as.int16);
				break;

			case HQ_VALUE_TYPE_INT32:
				snprintf(str, sizeof(str), "<int32: %" PRId32 ">", hValue->as.int32);
				break;

			case HQ_VALUE_TYPE_INT64:
				snprintf(str, sizeof(str), "<int64: %" PRId64 ">", hValue->as.int64);
				break;

			case HQ_VALUE_TYPE_UINT8:
				snprintf(str, sizeof(str), "<uint8: %" PRIu8 ">", hValue->as.uint8);
				break;

			case HQ_VALUE_TYPE_UINT16:
				snprintf(str, sizeof(str), "<uint16: %" PRIu16 ">", hValue->as.uint16);
				break;

			case HQ_VALUE_TYPE_UINT32:
				snprintf(str, sizeof(str), "<uint32: %" PRIu32 ">", hValue->as.uint32);
				break;

			case HQ_VALUE_TYPE_UINT64:
				snprintf(str, sizeof(str), "<uint64: %" PRIu64 ">", hValue->as.uint64);
				break;

			case HQ_VALUE_TYPE_FLOAT32:
				snprintf(str, sizeof(str), "<float32: %f>", hValue->as.float32);
				break;

			case HQ_VALUE_TYPE_FLOAT64:
				snprintf(str, sizeof(str), "<float64: %f>", hValue->as.float64);
				break;

			case HQ_VALUE_TYPE_BOOL:
				snprintf(str, sizeof(str), "<bool: %s>", hValue->as.boolean ? "true" : "false");
				break;

			case HQ_VALUE_TYPE_STRING:
				// The input value is already a string, so we can use it as-is.
				snprintf(
					str,
					sizeof(str),
					"<string: \"%.48s\"%s>",
					hValue->as.pString->data,
					(hValue->as.pString->length > 48) ? "..." : ""
				);
				break;

			case HQ_VALUE_TYPE_OBJECT:
				snprintf(
					str,
					sizeof(str),
					"<object: 0x%" PRIXPTR ", \"%.48s\"%s>",
					reinterpret_cast<uintptr_t>(hValue->as.pObject),
					hValue->as.pObject->pTypeName->data,
					(hValue->as.pObject->pTypeName->length > 48) ? "..." : ""
				);
				break;

			case HQ_VALUE_TYPE_ARRAY:
				snprintf(
					str,
					sizeof(str),
					"<array: 0x%" PRIXPTR ">",
					reinterpret_cast<uintptr_t>(hValue->as.array.pData)
				);
				break;

			case HQ_VALUE_TYPE_NATIVE:
				snprintf(
					str,
					sizeof(str),
					"<native: 0x%" PRIXPTR ">",
					reinterpret_cast<uintptr_t>(hValue->as.native.pObject)
				);
				break;

			default:
				// This should never happen.
				assert(false);
				break;
		}
	}

	return HqString::Create(str);
}

//----------------------------------------------------------------------------------------------------------------------

void HqValue::SetAutoMark(HqValueHandle hValue, const bool autoMark)
{
	if(hValue)
	{
		hValue->gcProxy.autoMark = autoMark;
	}
}

//----------------------------------------------------------------------------------------------------------------------

HqValue* HqValue::prv_onCreate(const int valueType, HqVmHandle hVm)
{
	assert(valueType >= 0);
	assert(valueType <= HQ_VALUE_TYPE__MAX_VALUE);

	HqValue* const pOutput = new HqValue();
	assert(pOutput != HQ_VALUE_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->type = valueType;

	const bool needsDiscovery = (valueType == HQ_VALUE_TYPE_OBJECT || valueType == HQ_VALUE_TYPE_ARRAY);

	// All values will auto-mark initially. This will allow values to be kept alive outside of script execution.
	HqGcProxy::Initialize(
		pOutput->gcProxy,
		hVm->gc,
		prv_onGcDiscovery,
		prv_onGcDestruct,
		pOutput,
		true,
		needsDiscovery
	);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqValue::prv_onGcDiscovery(HqGarbageCollector& gc, void* const pOpaque)
{
	HqValueHandle hValue = reinterpret_cast<HqValueHandle>(pOpaque);
	assert(hValue != HQ_VALUE_HANDLE_NULL);

	switch(hValue->type)
	{
		case HQ_VALUE_TYPE_OBJECT:
		{
			HqScriptObject* const pScriptObject = hValue->as.pObject;

			// Mark each member inside the object.
			for(size_t i = 0; i < pScriptObject->members.count; ++i)
			{
				HqValueHandle hMemberValue = pScriptObject->members.pData[i];
				if(hMemberValue)
				{
					HqGarbageCollector::MarkObject(gc, &hMemberValue->gcProxy);
				}
			}

			break;
		}

		case HQ_VALUE_TYPE_ARRAY:
		{
			HandleArray& array = hValue->as.array;

			// Mark each element in the array.
			for(size_t i = 0; i < array.count; ++i)
			{
				HqValueHandle hIndexValue = array.pData[i];
				if(hIndexValue)
				{
					HqGarbageCollector::MarkObject(gc, &hIndexValue->gcProxy);
				}
			}

			break;
		}

		default:
			// This should never happen since discovery should be disabled
			// on all value types without dependent GC data references.
			assert(false);
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqValue::prv_onGcDestruct(void* const pOpaqueValue)
{
	assert(pOpaqueValue != nullptr);

	HqValueHandle hValue = reinterpret_cast<HqValueHandle>(pOpaqueValue);

	switch(hValue->type)
	{
		case HQ_VALUE_TYPE_NATIVE:
			hValue->as.native.onDestruct(hValue->as.native.pObject);
			break;

		case HQ_VALUE_TYPE_STRING:
			HqString::Release(hValue->as.pString);
			break;

		case HQ_VALUE_TYPE_OBJECT:
			HqScriptObject::Dispose(hValue->as.pObject);
			break;

		case HQ_VALUE_TYPE_ARRAY:
			HandleArray::Dispose(hValue->as.array);
			break;

		default:
			break;
	}

	delete hValue;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqValue::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqValue::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
