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

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonValue XenonValue::NullValue =
{
	{},
	{},
	XENON_VALUE_TYPE_NULL,
};

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateBool(const bool value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_BOOL);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.boolean = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateInt8(const int8_t value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_INT8);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.int8 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateInt16(const int16_t value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_INT16);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.int16 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateInt32(const int32_t value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_INT32);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.int32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateInt64(const int64_t value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_INT64);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.int64 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateUint8(const uint8_t value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_UINT8);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.uint8 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateUint16(const uint16_t value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_UINT16);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.uint16 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateUint32(const uint32_t value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_UINT32);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.uint32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateUint64(const uint64_t value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_UINT64);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.uint64 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateFloat32(const float value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_FLOAT32);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.float32 = value;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::CreateFloat64(const double value)
{
	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_FLOAT64);
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

XenonValueHandle XenonValue::CreateString(const char* const string)
{
	assert(string != nullptr);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_STRING);
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

XenonValueHandle XenonValue::CreateString(XenonString* const pString)
{
	assert(pString != nullptr);

	XenonValue* const pOutput = prv_onCreate(XENON_VALUE_TYPE_STRING);
	if(!pOutput)
	{
		return &NullValue;
	}

	pOutput->as.pString = pString;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonValue::Copy(XenonValueHandle hValue)
{
	if(!hValue)
	{
		return &NullValue;
	}

	XenonValue* pOutput = prv_onCreate(hValue->type);
	if(!pOutput)
	{
		return &NullValue;
	}

	switch(pOutput->type)
	{
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
			// TODO: Implement support for script objects.
			assert(false);
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

int XenonValue::Acquire(XenonValueHandle hValue)
{
	if(hValue && hValue->type != XENON_VALUE_TYPE_NULL)
	{
		return XenonReference::Acquire(&hValue->ref);
	}

	return -1;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonValue::Release(XenonValueHandle hValue)
{
	if(hValue && hValue->type != XENON_VALUE_TYPE_NULL)
	{
		return XenonReference::Release(&hValue->ref);
	}

	return -1;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValue* XenonValue::prv_onCreate(const int valueType)
{
	assert(valueType >= 0);
	assert(valueType <= XENON_VALUE_TYPE__MAX_VALUE);

	XenonValue* const pOutput = new XenonValue();

	pOutput->ref = XenonReference::Initialize(prv_onDestruct, pOutput);
	pOutput->type = valueType;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonValue::prv_onDestruct(void* const pOpaque)
{
	assert(pOpaque != nullptr);

	XenonValue* const pValue = reinterpret_cast<XenonValue*>(pOpaque);

	switch(pValue->type)
	{
		case XENON_VALUE_TYPE_NULL:
			// The null value should never be destructed since it isn't dynamic.
			assert(false);
			return;

		case XENON_VALUE_TYPE_STRING:
			XenonString::Dispose(pValue->as.pString);
			break;

		case XENON_VALUE_TYPE_OBJECT:
			// Destruct object.
			break;

		default:
			break;
	}

	delete pValue;
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
