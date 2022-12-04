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

#include "../Value.hpp"
#include "../Vm.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramCommonLoader::CheckMagicNumber(const HqFileHeader& fileHeader)
{
	return fileHeader.magicNumber[0] == 'H'
		|| fileHeader.magicNumber[1] == 'Q'
		|| fileHeader.magicNumber[2] == 'P'
		|| fileHeader.magicNumber[3] == 'R'
		|| fileHeader.magicNumber[4] == 'G';
}

//----------------------------------------------------------------------------------------------------------------------

HqString* HqProgramCommonLoader::ReadString(
	HqSerializerHandle hSerializer,
	HqReportHandle hReport
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(hReport != HQ_REPORT_HANDLE_NULL);

	int result = 0;

	const size_t streamPosition = HqSerializerGetStreamPosition(hSerializer);
	const size_t streamLength = HqSerializerGetStreamLength(hSerializer);

	if(streamPosition == streamLength)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"ReadString error: End of data stream"
		);
		return nullptr;
	}

	// We can make reading native strings much more efficient by using the serializer's
	// stream directly. Doing this and embedding the null-terminator into the stream for
	// each string means we can create the native string directly from the data in the
	// serializer without having to allocate and copy to-then-from a staging string.
	// It also saves space due to not having the size baked in for each string.
	const char* const pStreamData = reinterpret_cast<const char*>(HqSerializerGetRawStreamPointer(hSerializer));

	// The stream data retrieved from the serializer is at the start of its memory,
	// so we adjust to the current position in the stream to get to the beginning
	// of the string data.
	HqString* const pString = HqString::Create(pStreamData + streamPosition);
	if(!pString)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"ReadString error: Failed to create HqString object"
		);
		return nullptr;
	}

	// Because we hacked our way into the stream to get the string data,
	// we need to manually adjust the stream position ourselves. And we
	// add 1 here to account for the null-terminator.
	result = HqSerializerSetStreamPosition(hSerializer, streamPosition + pString->length + 1);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqString::Release(pString);
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"ReadString error: Failed to update stream position: error=\"%s\"",
			errorString
		);
		return nullptr;
	}

	return pString;
}

//----------------------------------------------------------------------------------------------------------------------

HqValueHandle HqProgramCommonLoader::ReadValue(
	HqSerializerHandle hSerializer,
	HqVmHandle hVm,
	HqReportHandle hReport
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(hReport != HQ_REPORT_HANDLE_NULL);

	int result = 0;
	uint8_t valueType = 0;

	// Read the value type.
	result = HqSerializerReadUint8(hSerializer, &valueType);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"ReadValue error: Failed to read value type: error=\"%s\"",
			errorString
		);
		return HQ_VALUE_HANDLE_NULL;
	}

	HqValue temp;

	// Read the data based on the value type.
	switch(valueType)
	{
		case HQ_VALUE_TYPE_NULL:
		{
			return HqValue::CreateNull();
		}

		case HQ_VALUE_TYPE_INT8:
		{
			result = HqSerializerReadInt8(hSerializer, &temp.as.int8);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateInt8(hVm, temp.as.int8);
		}

		case HQ_VALUE_TYPE_INT16:
		{
			result = HqSerializerReadInt16(hSerializer, &temp.as.int16);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateInt16(hVm, temp.as.int16);
		}

		case HQ_VALUE_TYPE_INT32:
		{
			result = HqSerializerReadInt32(hSerializer, &temp.as.int32);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateInt32(hVm, temp.as.int32);
		}

		case HQ_VALUE_TYPE_INT64:
		{
			result = HqSerializerReadInt64(hSerializer, &temp.as.int64);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateInt64(hVm, temp.as.int64);
		}

		case HQ_VALUE_TYPE_UINT8:
		{
			result = HqSerializerReadUint8(hSerializer, &temp.as.uint8);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateUint8(hVm, temp.as.uint8);
		}

		case HQ_VALUE_TYPE_UINT16:
		{
			result = HqSerializerReadUint16(hSerializer, &temp.as.uint16);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateUint16(hVm, temp.as.uint16);
		}

		case HQ_VALUE_TYPE_UINT32:
		{
			result = HqSerializerReadUint32(hSerializer, &temp.as.uint32);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateUint32(hVm, temp.as.uint32);
		}

		case HQ_VALUE_TYPE_UINT64:
		{
			result = HqSerializerReadUint64(hSerializer, &temp.as.uint64);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateUint64(hVm, temp.as.uint64);
		}

		case HQ_VALUE_TYPE_FLOAT32:
		{
			result = HqSerializerReadFloat32(hSerializer, &temp.as.float32);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateFloat32(hVm, temp.as.float32);
		}

		case HQ_VALUE_TYPE_FLOAT64:
		{
			result = HqSerializerReadFloat64(hSerializer, &temp.as.float64);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateFloat64(hVm, temp.as.float64);
		}

		case HQ_VALUE_TYPE_BOOL:
		{
			result = HqSerializerReadBool(hSerializer, &temp.as.boolean);
			if(result != HQ_SUCCESS)
			{
				break;
			}

			return HqValue::CreateBool(hVm, temp.as.boolean);
		}

		case HQ_VALUE_TYPE_STRING:
		{
			// Read the string from the serializer.
			HqString* const pString = ReadString(hSerializer, hReport);
			if(!pString)
			{
				return HQ_VALUE_HANDLE_NULL;
			}

			return HqValue::CreateString(hVm, pString);
		}

		case HQ_VALUE_TYPE_OBJECT:
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"ReadValue error: Found object value type, but they are currently unsupported"
			);
			return HQ_VALUE_HANDLE_NULL;

		default:
			// Unknown (or currently unhandled) value type.
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"ReadValue error: Unknown value type: type=%d",
				valueType
			);
			return HQ_VALUE_HANDLE_NULL;
	}

	const char* const errorString = HqGetErrorCodeString(result);

	HqReportMessage(
		hReport,
		HQ_MESSAGE_TYPE_ERROR,
		"ReadValue error: Failed to read value data: error=\"%s\"",
		errorString
	);

	return HQ_VALUE_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------
