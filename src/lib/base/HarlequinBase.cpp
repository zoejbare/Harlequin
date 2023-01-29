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

#include "../Harlequin.h"

#include "../common/Report.hpp"

#include "Clock.hpp"
#include "Serializer.hpp"
#include "String.hpp"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

#define _HQ_SWAP_16(x) ( \
	((x) >> 8) | \
	((x) << 8) \
)

#define _HQ_SWAP_32(x) ( \
	(((x) >> 24) & 0x000000FF) | \
	(((x) >> 8)  & 0x0000FF00) | \
	(((x) << 8)  & 0x00FF0000) | \
	(((x) << 24) & 0xFF000000) \
)

#define _HQ_SWAP_64(x) ( \
	(((x) >> 56) & 0x00000000000000FFull) | \
	(((x) >> 40) & 0x000000000000FF00ull) | \
	(((x) >> 24) & 0x0000000000FF0000ull) | \
	(((x) >> 8)  & 0x00000000FF000000ull) | \
	(((x) << 8)  & 0x000000FF00000000ull) | \
	(((x) << 24) & 0x0000FF0000000000ull) | \
	(((x) << 40) & 0x00FF000000000000ull) | \
	(((x) << 56) & 0xFF00000000000000ull) \
)

#define _HQ_SWITCH_CASE_RETURN_STRING(x) case x: return #x

//----------------------------------------------------------------------------------------------------------------------

static HqMemAllocator globalAllocator = HqMemGetDefaultAllocator();

extern "C" {

//----------------------------------------------------------------------------------------------------------------------

const char* HqGetErrorCodeString(int errorCode)
{
	switch(errorCode)
	{
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_SUCCESS);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_UNSPECIFIED_FAILURE);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_INVALID_ARG);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_INVALID_TYPE);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_INVALID_DATA);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_INVALID_RANGE);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_INVALID_OPERATION);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_BAD_ALLOCATION);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_KEY_ALREADY_EXISTS);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_KEY_DOES_NOT_EXIST);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_FAILED_TO_OPEN_FILE);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_STREAM_END);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_STACK_EMPTY);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_STACK_FULL);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_INDEX_OUT_OF_RANGE);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_SCRIPT_NO_FUNCTION);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_MISMATCH);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_UNKNOWN_ID);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ERROR_NO_WRITE);

		default:
			break;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

const char* HqGetEndiannessString(int endianness)
{
	switch(endianness)
	{
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ENDIAN_ORDER_UNKNOWN);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ENDIAN_ORDER_NATIVE);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ENDIAN_ORDER_LITTLE);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_ENDIAN_ORDER_BIG);

		default:
			break;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

const char* HqGetValueTypeString(int valueType)
{
	switch(valueType)
	{
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_INT8);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_INT16);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_INT32);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_INT64);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_UINT8);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_UINT16);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_UINT32);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_UINT64);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_FLOAT32);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_FLOAT64);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_BOOL);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_STRING);
		_HQ_SWITCH_CASE_RETURN_STRING(HQ_VALUE_TYPE_OBJECT);

		default:
			break;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

int HqGetPlatformEndianness()
{
#ifdef HQ_CPU_ENDIAN_LITTLE
	return HQ_ENDIAN_ORDER_LITTLE;

#else
	return HQ_ENDIAN_ORDER_BIG;

#endif
}

//----------------------------------------------------------------------------------------------------------------------

HqMemAllocator HqMemGetDefaultAllocator()
{
	HqMemAllocator allocator;
	allocator.allocFn = malloc;
	allocator.reallocFn = realloc;
	allocator.freeFn = free;

	return allocator;
}

//----------------------------------------------------------------------------------------------------------------------

HqMemAllocator HqMemGetAllocator()
{
	return globalAllocator;
}

//----------------------------------------------------------------------------------------------------------------------

int HqMemSetAllocator(HqMemAllocator allocator)
{
	if(!allocator.allocFn || !allocator.reallocFn || !allocator.freeFn)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	globalAllocator = allocator;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqMemAlloc(const size_t size)
{
	return globalAllocator.allocFn(size);
}

//----------------------------------------------------------------------------------------------------------------------

void* HqMemRealloc(void* const pMem, const size_t size)
{
	return globalAllocator.reallocFn(pMem, size);
}

//----------------------------------------------------------------------------------------------------------------------

int HqMemFree(void* const pMem)
{
	if(!pMem)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	globalAllocator.freeFn(pMem);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqReportMessage(HqReportHandle hReport, const int messageType, const char* const fmt, ...)
{
	if(!hReport)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(!hReport->onMessageFn || hReport->level > messageType)
	{
		// Do nothing when we're unable to report anything for the given parameters.
		return HQ_SUCCESS;
	}

	va_list vl;
	va_start(vl, fmt);
	const char* const message = HqString::RawFormatVarArgs(fmt, vl);
	va_end(vl);

	// Send off the message, then free the message string.
	hReport->onMessageFn(hReport->pUserData, messageType, message);
	HqMemFree((void*)(message));

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqReportSetLevel(HqReportHandle hReport, const int messageType)
{
	if(hReport && messageType <= HQ_MESSAGE_TYPE_FATAL)
	{
		hReport->level = messageType;
		return HQ_SUCCESS;
	}

	return HQ_ERROR_INVALID_ARG;
}

//----------------------------------------------------------------------------------------------------------------------

int HqReportGetLevel(HqReportHandle hReport)
{
	if(hReport)
	{
		return hReport->level;
	}

	return HQ_MESSAGE_TYPE_FATAL;
}

//----------------------------------------------------------------------------------------------------------------------

char* HqFormatString(const char* const fmt, ...)
{
	va_list vl;
	va_start(vl, fmt);

	char* output = HqString::RawFormatVarArgs(fmt, vl);

	va_end(vl);
	return output;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerCreate(HqSerializerHandle* phOutSerializer, int mode)
{
	if(!phOutSerializer
		|| (*phOutSerializer) != HQ_SERIALIZER_HANDLE_NULL
		|| mode < HQ_SERIALIZER_MODE_READER
		|| mode > HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	(*phOutSerializer) = HqSerializer::Create(mode);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerDispose(HqSerializerHandle* phSerializer)
{
	if(phSerializer && (*phSerializer))
	{
		HqSerializer::Dispose(*phSerializer);

		(*phSerializer) = HQ_SERIALIZER_HANDLE_NULL;

		return HQ_SUCCESS;
	}

	return HQ_ERROR_INVALID_ARG;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerGetMode(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_SERIALIZER_MODE_UNKNOWN;
	}

	return hSerializer->mode;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerGetEndianness(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return HQ_ENDIAN_ORDER_UNKNOWN;
	}

	return hSerializer->endianness;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqSerializerGetStreamLength(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return size_t(-1);
	}

	return hSerializer->stream.count;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqSerializerGetStreamPosition(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return size_t(-1);
	}

	return hSerializer->position;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqSerializerGetRawStreamPointer(HqSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return nullptr;
	}

	return hSerializer->stream.pData;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerSetEndianness(HqSerializerHandle hSerializer, int endianness)
{
	if(!hSerializer || endianness < HQ_ENDIAN_ORDER_NATIVE || endianness > HQ_ENDIAN_ORDER_BIG)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	hSerializer->endianness = endianness;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerSetStreamPosition(HqSerializerHandle hSerializer, size_t position)
{
	if(!hSerializer || hSerializer->stream.count < position)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	hSerializer->position = position;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerLoadStreamFromFile(HqSerializerHandle hSerializer, const char* filePath)
{
	if(!hSerializer || !filePath || filePath[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::LoadFile(hSerializer, filePath);
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerLoadStreamFromBuffer(HqSerializerHandle hSerializer, const void* pBuffer, size_t bufferLength)
{
	if(!hSerializer || !pBuffer || bufferLength == 0)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::LoadBuffer(hSerializer, pBuffer, bufferLength);
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerSaveStreamToFile(HqSerializerHandle hSerializer, const char* filePath, bool append)
{
	if(!hSerializer || !filePath || filePath[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::SaveFile(hSerializer, filePath, append);
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerSaveStreamToBuffer(HqSerializerHandle hSerializer, void* pBuffer, size_t* pBufferLength)
{
	if(!hSerializer || !pBufferLength)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::SaveBuffer(hSerializer, pBuffer, pBufferLength);
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteBool8(HqSerializerHandle hSerializer, bool value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	const uint8_t temp = value ? uint8_t(0xFF) : 0;

	return HqSerializer::WriteRawData(hSerializer, &temp, sizeof(uint8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteBool16(HqSerializerHandle hSerializer, bool value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	const uint16_t temp = value ? 0xFFFF : 0;

	return HqSerializer::WriteRawData(hSerializer, &temp, sizeof(uint16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteBool32(HqSerializerHandle hSerializer, bool value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	const uint32_t temp = value ? 0xFFFFFFFFul : 0;

	return HqSerializer::WriteRawData(hSerializer, &temp, sizeof(uint32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteBool64(HqSerializerHandle hSerializer, bool value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	const uint64_t temp = value ? 0xFFFFFFFFFFFFFFFFull : 0;

	return HqSerializer::WriteRawData(hSerializer, &temp, sizeof(uint64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteInt8(HqSerializerHandle hSerializer, int8_t value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(int8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteInt16(HqSerializerHandle hSerializer, int16_t value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(int16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteInt32(HqSerializerHandle hSerializer, int32_t value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(int32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteInt64(HqSerializerHandle hSerializer, int64_t value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(int64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteUint8(HqSerializerHandle hSerializer, uint8_t value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(uint8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteUint16(HqSerializerHandle hSerializer, uint16_t value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(uint16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteUint32(HqSerializerHandle hSerializer, uint32_t value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(uint32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteUint64(HqSerializerHandle hSerializer, uint64_t value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(uint64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteFloat32(HqSerializerHandle hSerializer, float value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(float));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteFloat64(HqSerializerHandle hSerializer, double value)
{
	if(!hSerializer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(double));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerWriteBuffer(HqSerializerHandle hSerializer, size_t bufferLength, const void* pBuffer)
{
	if(!hSerializer || bufferLength == 0 || !pBuffer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != HQ_SERIALIZER_MODE_WRITER)
	{
		return HQ_ERROR_INVALID_TYPE;
	}

	return HqSerializer::WriteRawData(hSerializer, pBuffer, bufferLength);
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadBool8(HqSerializerHandle hSerializer, bool* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	uint8_t temp = 0;

	int result = HqSerializer::ReadRawData(hSerializer, &temp, sizeof(uint8_t));
	if(result == HQ_SUCCESS)
	{
		(*pOutValue) = temp ? true : false;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadBool16(HqSerializerHandle hSerializer, bool* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	uint16_t temp = 0;

	int result = HqSerializer::ReadRawData(hSerializer, &temp, sizeof(uint16_t));
	if(result == HQ_SUCCESS)
	{
		(*pOutValue) = temp ? true : false;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadBool32(HqSerializerHandle hSerializer, bool* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	uint32_t temp = 0;

	int result = HqSerializer::ReadRawData(hSerializer, &temp, sizeof(uint32_t));
	if(result == HQ_SUCCESS)
	{
		(*pOutValue) = temp ? true : false;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadBool64(HqSerializerHandle hSerializer, bool* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	uint64_t temp = 0;

	int result = HqSerializer::ReadRawData(hSerializer, &temp, sizeof(uint64_t));
	if(result == HQ_SUCCESS)
	{
		(*pOutValue) = temp ? true : false;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadInt8(HqSerializerHandle hSerializer, int8_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(int8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadInt16(HqSerializerHandle hSerializer, int16_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(int16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadInt32(HqSerializerHandle hSerializer, int32_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(int32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadInt64(HqSerializerHandle hSerializer, int64_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(int64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadUint8(HqSerializerHandle hSerializer, uint8_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(uint8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadUint16(HqSerializerHandle hSerializer, uint16_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(uint16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadUint32(HqSerializerHandle hSerializer, uint32_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(uint32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadUint64(HqSerializerHandle hSerializer, uint64_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(uint64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadFloat32(HqSerializerHandle hSerializer, float* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(float));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadFloat64(HqSerializerHandle hSerializer, double* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(double));
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializerReadBuffer(HqSerializerHandle hSerializer, size_t bufferLength, void* pOutBuffer)
{
	if(!hSerializer || bufferLength == 0 || !pOutBuffer)
	{
		return HQ_ERROR_INVALID_ARG;
	}

	return HqSerializer::ReadRawData(hSerializer, pOutBuffer, bufferLength);
}

//----------------------------------------------------------------------------------------------------------------------

int16_t HqEndianSwapInt16(const int16_t value)
{
	return _HQ_SWAP_16(value);
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqEndianSwapInt32(const int32_t value)
{
	return _HQ_SWAP_32(value);
}

//----------------------------------------------------------------------------------------------------------------------

int64_t HqEndianSwapInt64(const int64_t value)
{
	return _HQ_SWAP_64(value);
}

//----------------------------------------------------------------------------------------------------------------------

uint16_t HqEndianSwapUint16(const uint16_t value)
{
	return _HQ_SWAP_16(value);
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t HqEndianSwapUint32(const uint32_t value)
{
	return _HQ_SWAP_32(value);
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t HqEndianSwapUint64(const uint64_t value)
{
	return _HQ_SWAP_64(value);
}

//----------------------------------------------------------------------------------------------------------------------

float HqEndianSwapFloat32(const float value)
{
	union
	{
		float tempFloat;
		int32_t tempInt;
	};

	tempFloat = value;
	tempInt = _HQ_SWAP_32(tempInt);

	return tempFloat;
}

//----------------------------------------------------------------------------------------------------------------------

double HqEndianSwapFloat64(const double value)
{
	union
	{
		double tempFloat;
		int64_t tempInt;
	};

	tempFloat = value;
	tempInt = _HQ_SWAP_64(tempInt);

	return tempFloat;
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t HqClockGetFrequency()
{
	return _HqClockImplGetFrequency();
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t HqClockGetTimestamp()
{
	return _HqClockImplGetTimestamp();
}

//----------------------------------------------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------------------------------------------

#undef _HQ_SWAP_16
#undef _HQ_SWAP_32
#undef _HQ_SWAP_64
#undef _HQ_SWITCH_CASE_RETURN_STRING