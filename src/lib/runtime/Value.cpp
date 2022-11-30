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

#define _XENON_ARRAY_DEFAULT_CAPACITY 8

//----------------------------------------------------------------------------------------------------------------------

XenonValue XenonValue::NullValue =
{
	XENON_VM_HANDLE_NULL,
	{},
	{},
	XENON_VALUE_TYPE_NULL,
};

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateBool(XenonVmHandle hVm, const bool value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_BOOL, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.boolean = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateInt8(XenonVmHandle hVm, const int8_t value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_INT8, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.int8 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateInt16(XenonVmHandle hVm, const int16_t value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_INT16, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.int16 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateInt32(XenonVmHandle hVm, const int32_t value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_INT32, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.int32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateInt64(XenonVmHandle hVm, const int64_t value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_INT64, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.int64 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateUint8(XenonVmHandle hVm, const uint8_t value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_UINT8, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.uint8 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateUint16(XenonVmHandle hVm, const uint16_t value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_UINT16, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.uint16 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateUint32(XenonVmHandle hVm, const uint32_t value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_UINT32, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.uint32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateUint64(XenonVmHandle hVm, const uint64_t value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_UINT64, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.uint64 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateFloat32(XenonVmHandle hVm, const float value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_FLOAT32, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.float32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateFloat64(XenonVmHandle hVm, const double value)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_FLOAT64, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.float64 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateNull()
{
	return &NullValue;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateString(XenonVmHandle hVm, const char* const string)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(string != nullptr);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_STRING, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.pString = XenonString::Create(string);
	if(!pOutput->as.pString)
	{
		delete pOutput;
		return &NullValue;
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateString(XenonVmHandle hVm, XenonString* const pString)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pString != nullptr);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_STRING, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.pString = pString;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateObject(XenonVmHandle hVm, XenonScriptObject* const pObjectSchema)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(pObjectSchema != nullptr);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_OBJECT, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	// Make a copy of the input object template for this value.
	pOutput->as.pObject = XenonScriptObject::CreateInstance(pObjectSchema);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateFunction(XenonVmHandle hVm, XenonFunctionHandle hFunction)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(hFunction != nullptr);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_FUNCTION, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.hFunction = hFunction;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateArray(XenonVmHandle hVm, const size_t count)
{
	assert(hVm != XENON_VM_HANDLE_NULL);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_ARRAY, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	const size_t capacity = (count > _XENON_ARRAY_DEFAULT_CAPACITY) ? count : _XENON_ARRAY_DEFAULT_CAPACITY;

	// Initialize the array storage.
	HandleArray::Initialize(pOutput->as.array);
	HandleArray::Reserve(pOutput->as.array, capacity);

	pOutput->as.array.count = count;

	if(pOutput->as.array.count > 0)
	{
		// Initialize the array memory so it's not filled with garbage data.
		memset(pOutput->as.array.pData, 0, sizeof(XenonValueHandle) * pOutput->as.array.count);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateNative(
	XenonVmHandle hVm,
	void* const pNativeObject,
	XenonCallbackNativeValueCopy onCopy,
	XenonCallbackNativeValueDestruct onDestruct,
	XenonCallbackNativeValueEqual onTestEqual,
	XenonCallbackNativeValueLessThan onTestLessThan
)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(onCopy != nullptr);
	assert(onDestruct != nullptr);
	assert(onTestEqual != nullptr);
	assert(onTestLessThan != nullptr);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_NATIVE, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.native.pObject = pNativeObject;
	pOutput->as.native.onCopy = onCopy;
	pOutput->as.native.onDestruct = onDestruct;
	pOutput->as.native.onTestEqual = onTestEqual;
	pOutput->as.native.onTestLessThan = onTestLessThan;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::Copy(XenonVmHandle hVm, XenonValueHandle hValue)
{
	if(!hValue || hValue->type == XENON_VALUE_TYPE_NULL)
	{
		return &NullValue;
	}

	XenonValue* pOutput = prv_onCreate(hValue->type, hVm);
	if(!pOutput)
	{
		return &NullValue;
	}

	switch(pOutput->type)
	{
		case XENON_VALUE_TYPE_BOOL:
			pOutput->as.boolean = hValue->as.boolean;
			break;

		case XENON_VALUE_TYPE_INT8:
			pOutput->as.int8 = hValue->as.int8;
			break;

		case XENON_VALUE_TYPE_INT16:
			pOutput->as.int16 = hValue->as.int16;
			break;

		case XENON_VALUE_TYPE_INT32:
			pOutput->as.int32 = hValue->as.int32;
			break;

		case XENON_VALUE_TYPE_INT64:
			pOutput->as.int64 = hValue->as.int64;
			break;

		case XENON_VALUE_TYPE_UINT8:
			pOutput->as.uint8 = hValue->as.uint8;
			break;

		case XENON_VALUE_TYPE_UINT16:
			pOutput->as.uint16 = hValue->as.uint16;
			break;

		case XENON_VALUE_TYPE_UINT32:
			pOutput->as.uint32 = hValue->as.uint32;
			break;

		case XENON_VALUE_TYPE_UINT64:
			pOutput->as.uint64 = hValue->as.uint64;
			break;

		case XENON_VALUE_TYPE_FLOAT32:
			pOutput->as.float32 = hValue->as.float32;
			break;

		case XENON_VALUE_TYPE_FLOAT64:
			pOutput->as.float64 = hValue->as.float64;
			break;

		case XENON_VALUE_TYPE_STRING:
			pOutput->as.pString = XenonString::Create(hValue->as.pString->data);
			break;

		case XENON_VALUE_TYPE_OBJECT:
			pOutput->as.pObject = XenonScriptObject::CreateCopy(hValue->as.pObject->pSchema);
			break;

		case XENON_VALUE_TYPE_ARRAY:
			HandleArray::Initialize(pOutput->as.array);
			HandleArray::Reserve(
				pOutput->as.array,
				(hValue->as.array.count > _XENON_ARRAY_DEFAULT_CAPACITY)
					? hValue->as.array.count
					: _XENON_ARRAY_DEFAULT_CAPACITY
			);

			pOutput->as.array.count = hValue->as.array.count;

			if(pOutput->as.array.count > 0)
			{
				memcpy(
					pOutput->as.array.pData,
					hValue->as.array.pData,
					sizeof(XenonValueHandle) * pOutput->as.array.count
				);
			}
			break;

		case XENON_VALUE_TYPE_NATIVE:
			pOutput->as.native.onCopy = hValue->as.native.onCopy;
			pOutput->as.native.onDestruct = hValue->as.native.onDestruct;

			pOutput->as.native.onCopy(&pOutput->as.native.pObject, hValue->as.native.pObject);
			break;

		default:
			// This should never happen. If it does, it indicates an unimplemented type here.
			assert(false);
			delete pOutput;
			return &NullValue;
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonString* XenonValue::GetDebugString(XenonValueHandle hValue)
{
	// Temporary buffer used by the numerical primitive types. We know the exact maximum size for the non-float types,
	// but the float types are a little more nebulous. It's easier to just pre-allocate a buffer that we know will
	// definitely be large enough for all of those types (as well as the object type) and call it a day.
	char str[256] = { '<', 'n', 'u', 'l', 'l', '>', '\0' };

	if(hValue)
	{
		switch(hValue->type)
		{
			case XENON_VALUE_TYPE_NULL:
				// Break here to use the same string value you would get from a null value handle.
				break;

			case XENON_VALUE_TYPE_INT8:
				snprintf(str, sizeof(str), "<int8: %" PRId8 ">", hValue->as.int8);
				break;

			case XENON_VALUE_TYPE_INT16:
				snprintf(str, sizeof(str), "<int16: %" PRId16 ">", hValue->as.int16);
				break;

			case XENON_VALUE_TYPE_INT32:
				snprintf(str, sizeof(str), "<int32: %" PRId32 ">", hValue->as.int32);
				break;

			case XENON_VALUE_TYPE_INT64:
				snprintf(str, sizeof(str), "<int64: %" PRId64 ">", hValue->as.int64);
				break;

			case XENON_VALUE_TYPE_UINT8:
				snprintf(str, sizeof(str), "<uint8: %" PRIu8 ">", hValue->as.uint8);
				break;

			case XENON_VALUE_TYPE_UINT16:
				snprintf(str, sizeof(str), "<uint16: %" PRIu16 ">", hValue->as.uint16);
				break;

			case XENON_VALUE_TYPE_UINT32:
				snprintf(str, sizeof(str), "<uint32: %" PRIu32 ">", hValue->as.uint32);
				break;

			case XENON_VALUE_TYPE_UINT64:
				snprintf(str, sizeof(str), "<uint64: %" PRIu64 ">", hValue->as.uint64);
				break;

			case XENON_VALUE_TYPE_FLOAT32:
				snprintf(str, sizeof(str), "<float32: %f>", hValue->as.float32);
				break;

			case XENON_VALUE_TYPE_FLOAT64:
				snprintf(str, sizeof(str), "<float64: %f>", hValue->as.float64);
				break;

			case XENON_VALUE_TYPE_BOOL:
				snprintf(str, sizeof(str), "<bool: %s>", hValue->as.boolean ? "true" : "false");
				break;

			case XENON_VALUE_TYPE_STRING:
				// The input value is already a string, so we can use it as-is.
				snprintf(
					str,
					sizeof(str),
					"<string: \"%.48s\"%s>",
					hValue->as.pString->data,
					(hValue->as.pString->length > 48) ? "..." : ""
				);
				break;

			case XENON_VALUE_TYPE_OBJECT:
				snprintf(
					str,
					sizeof(str),
					"<object: 0x%" PRIXPTR ", \"%.48s\"%s>",
					reinterpret_cast<uintptr_t>(hValue->as.pObject),
					hValue->as.pObject->pTypeName->data,
					(hValue->as.pObject->pTypeName->length > 48) ? "..." : ""
				);
				break;

			case XENON_VALUE_TYPE_ARRAY:
				snprintf(
					str,
					sizeof(str),
					"<array: 0x%" PRIXPTR ">",
					reinterpret_cast<uintptr_t>(hValue->as.array.pData)
				);
				break;

			case XENON_VALUE_TYPE_NATIVE:
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

	return XenonString::Create(str);
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonValue::CanBeMarked(XenonValueHandle hValue)
{
	return hValue
		&& hValue->type != XENON_VALUE_TYPE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonValue::SetAutoMark(XenonValueHandle hValue, const bool autoMark)
{
	if(hValue && hValue->type != XENON_VALUE_TYPE_NULL)
	{
		hValue->gcProxy.autoMark = autoMark;
	}
}

//----------------------------------------------------------------------------------------------------------------------

XenonValue* XenonValue::prv_onCreate(const int valueType, XenonVmHandle hVm)
{
	assert(valueType >= 0);
	assert(valueType <= XENON_VALUE_TYPE__MAX_VALUE);

	XenonValue* const pOutput = new XenonValue();
	assert(pOutput != XENON_VALUE_HANDLE_NULL);

	pOutput->hVm = hVm;
	pOutput->type = valueType;

	// All values will auto-mark initially, until they are 'disposed' of.
	// This will allow values to be kept alive outside of script execution.
	XenonGcProxy::Initialize(pOutput->gcProxy, hVm->gc, prv_onGcDiscovery, prv_onGcDestruct, pOutput, true);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonValue::prv_onGcDiscovery(XenonGarbageCollector& gc, void* const pOpaque)
{
	XenonValueHandle hValue = reinterpret_cast<XenonValueHandle>(pOpaque);
	assert(hValue != XENON_VALUE_HANDLE_NULL);

	switch(hValue->type)
	{
		case XENON_VALUE_TYPE_OBJECT:
		{
			XenonScriptObject* const pScriptObject = hValue->as.pObject;

			// Mark each member inside the object.
			for(size_t i = 0; i < pScriptObject->members.count; ++i)
			{
				XenonGarbageCollector::MarkObject(gc, &pScriptObject->members.pData[i]->gcProxy);
			}

			break;
		}

		case XENON_VALUE_TYPE_ARRAY:
		{
			HandleArray& array = hValue->as.array;

			// Mark each element in the array.
			for(size_t i = 0; i < array.count; ++i)
			{
				XenonGarbageCollector::MarkObject(gc, &array.pData[i]->gcProxy);
			}

			break;
		}

		default:
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonValue::prv_onGcDestruct(void* const pOpaqueValue)
{
	assert(pOpaqueValue != nullptr);

	XenonValueHandle hValue = reinterpret_cast<XenonValueHandle>(pOpaqueValue);

	switch(hValue->type)
	{
		case XENON_VALUE_TYPE_NULL:
			// The null value should never be destructed since it isn't dynamic.
			assert(false);
			return;

		case XENON_VALUE_TYPE_NATIVE:
			hValue->as.native.onDestruct(hValue->as.native.pObject);
			break;

		case XENON_VALUE_TYPE_STRING:
			XenonString::Release(hValue->as.pString);
			break;

		case XENON_VALUE_TYPE_OBJECT:
			XenonScriptObject::Dispose(hValue->as.pObject);
			break;

		case XENON_VALUE_TYPE_ARRAY:
			HandleArray::Dispose(hValue->as.array);
			break;

		default:
			break;
	}

	delete hValue;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonValue::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonValue::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
