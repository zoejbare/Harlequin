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

#include "CommonLoader.hpp"

#include "../../shared/Value.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

bool XenonProgramCommonLoader::CheckMagicNumber(const XenonProgramCommonHeader& commonHeader)
{
	return commonHeader.magicNumber[0] == 'X'
		|| commonHeader.magicNumber[1] == 'P'
		|| commonHeader.magicNumber[2] == 'R'
		|| commonHeader.magicNumber[3] == 'G'
		|| commonHeader.magicNumber[4] == '_';
}

//----------------------------------------------------------------------------------------------------------------------

XenonString* XenonProgramCommonLoader::ReadString(
	XenonSerializerHandle hSerializer,
	XenonReportHandle hReport
)
{
	assert(hSerializer != XENON_SERIALIZER_HANDLE_NULL);
	assert(hReport != XENON_REPORT_HANDLE_NULL);

	int result = 0;

	const size_t streamPosition = XenonSerializerGetStreamPosition(hSerializer);
	const size_t streamLength = XenonSerializerGetStreamLength(hSerializer);

	if(streamPosition == streamLength)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"ReadString error: End of data stream"
		);
		return nullptr;
	}

	// We can make reading native strings much more efficient by using the serializer's
	// stream directly. Doing this and embedding the null-terminator into the stream for
	// each string means we can create the native string directly from the data in the
	// serializer without having to allocate and copy to-then-from a staging string.
	// It also saves space due to not having the size baked in for each string.
	const char* const pStreamData = reinterpret_cast<const char*>(XenonSerializerGetRawStreamPointer(hSerializer));

	// The stream data retrieved from the serializer is at the start of its memory,
	// so we adjust to the current position in the stream to get to the beginning
	// of the string data.
	XenonString* const pString = XenonString::Create(pStreamData + streamPosition);
	if(!pString)
	{
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"ReadString error: Failed to create XenonString object"
		);
		return nullptr;
	}

	// Because we hacked our way into the stream to get the string data,
	// we need to manually adjust the stream position ourselves. And we
	// add 1 here to account for the null-terminator.
	result = XenonSerializerSetStreamPosition(hSerializer, streamPosition + pString->length + 1);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonString::Release(pString);
		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"ReadString error: Failed to update stream position: error=\"%s\"",
			errorString
		);
		return nullptr;
	}

	return pString;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonProgramCommonLoader::ReadValue(
	XenonSerializerHandle hSerializer,
	XenonReportHandle hReport
)
{
	assert(hSerializer != XENON_SERIALIZER_HANDLE_NULL);
	assert(hReport != XENON_REPORT_HANDLE_NULL);

	int result = 0;
	uint8_t valueType = 0;

	// Read the value type.
	result = XenonSerializerReadUint8(hSerializer, &valueType);
	if(result != XENON_SUCCESS)
	{
		const char* const errorString = XenonGetErrorCodeString(result);

		XenonReportMessage(
			hReport,
			XENON_MESSAGE_TYPE_ERROR,
			"ReadValue error: Failed to read value type: error=\"%s\"",
			errorString
		);
		return nullptr;
	}

	XenonValue temp;

	// Read the data based on the value type.
	switch(valueType)
	{
		case XENON_VALUE_TYPE_NULL:
		{
			return XenonValue::CreateNull();
		}

		case XENON_VALUE_TYPE_INT8:
		{
			result = XenonSerializerReadInt8(hSerializer, &temp.as.int8);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateInt8(temp.as.int8);
		}

		case XENON_VALUE_TYPE_INT16:
		{
			result = XenonSerializerReadInt16(hSerializer, &temp.as.int16);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateInt16(temp.as.int16);
		}

		case XENON_VALUE_TYPE_INT32:
		{
			result = XenonSerializerReadInt32(hSerializer, &temp.as.int32);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateInt32(temp.as.int32);
		}

		case XENON_VALUE_TYPE_INT64:
		{
			result = XenonSerializerReadInt64(hSerializer, &temp.as.int64);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateInt64(temp.as.int64);
		}

		case XENON_VALUE_TYPE_UINT8:
		{
			result = XenonSerializerReadUint8(hSerializer, &temp.as.uint8);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateUint8(temp.as.uint8);
		}

		case XENON_VALUE_TYPE_UINT16:
		{
			result = XenonSerializerReadUint16(hSerializer, &temp.as.uint16);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateUint16(temp.as.uint16);
		}

		case XENON_VALUE_TYPE_UINT32:
		{
			result = XenonSerializerReadUint32(hSerializer, &temp.as.uint32);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateUint32(temp.as.uint32);
		}

		case XENON_VALUE_TYPE_UINT64:
		{
			result = XenonSerializerReadUint64(hSerializer, &temp.as.uint64);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateUint64(temp.as.uint64);
		}

		case XENON_VALUE_TYPE_FLOAT32:
		{
			result = XenonSerializerReadFloat32(hSerializer, &temp.as.float32);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateFloat32(temp.as.float32);
		}

		case XENON_VALUE_TYPE_FLOAT64:
		{
			result = XenonSerializerReadFloat64(hSerializer, &temp.as.float64);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateFloat64(temp.as.float64);
		}

		case XENON_VALUE_TYPE_BOOL:
		{
			result = XenonSerializerReadBool(hSerializer, &temp.as.boolean);
			if(result != XENON_SUCCESS)
			{
				break;
			}

			return XenonValue::CreateBool(temp.as.boolean);
		}

		case XENON_VALUE_TYPE_STRING:
		{
			XenonString* const pString = ReadString(hSerializer, hReport);

			// Read the string from the serializer.
			if(!pString)
			{
				return nullptr;
			}

			return XenonValue::CreateString(pString);
		}

		case XENON_VALUE_TYPE_OBJECT:
			// TODO: Implement support for script objects.
			assert(false);
			return nullptr;

		default:
			// Unknown (or currently unhandled) value type.
			XenonReportMessage(
				hReport,
				XENON_MESSAGE_TYPE_ERROR,
				"ReadValue error: Unknown value type: type=%d",
				valueType
			);
			return nullptr;
	}

	const char* const errorString = XenonGetErrorCodeString(result);

	XenonReportMessage(
		hReport,
		XENON_MESSAGE_TYPE_ERROR,
		"ReadValue error: Failed to read value data: error=\"%s\"",
		errorString
	);

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
