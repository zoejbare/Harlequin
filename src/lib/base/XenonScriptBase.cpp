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

#include "../XenonBuiltIn.h"
#include "../XenonScript.h"

#include "../common/Report.hpp"
#include "Serializer.hpp"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------------------------------------------

#define XENON_SWAP_16(x) ( \
	((x) >> 8) | \
	((x) << 8) \
)

#define XENON_SWAP_32(x) ( \
	(((x) >> 24) & 0x000000FF) | \
	(((x) >> 8)  & 0x0000FF00) | \
	(((x) << 8)  & 0x00FF0000) | \
	(((x) << 24) & 0xFF000000) \
)

#define XENON_SWAP_64(x) ( \
	(((x) >> 56) & 0x00000000000000FFull) | \
	(((x) >> 40) & 0x000000000000FF00ull) | \
	(((x) >> 24) & 0x0000000000FF0000ull) | \
	(((x) >> 8)  & 0x00000000FF000000ull) | \
	(((x) << 8)  & 0x000000FF00000000ull) | \
	(((x) << 24) & 0x0000FF0000000000ull) | \
	(((x) << 40) & 0x00FF000000000000ull) | \
	(((x) << 56) & 0xFF00000000000000ull) \
)

#define XENON_SWITCH_CASE_RETURN_STRING(x) case x: return #x

//----------------------------------------------------------------------------------------------------------------------

static XenonMemAllocator globalAllocator = XenonMemGetDefaultAllocator();

extern "C" {

//----------------------------------------------------------------------------------------------------------------------

const char* XenonGetErrorCodeString(int errorCode)
{
	switch(errorCode)
	{
		XENON_SWITCH_CASE_RETURN_STRING(XENON_SUCCESS);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_INVALID_ARG);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_INVALID_TYPE);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_BAD_ALLOCATION);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_KEY_ALREADY_EXISTS);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_KEY_DOES_NOT_EXIST);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_FAILED_TO_OPEN_FILE);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_STREAM_END);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_STACK_EMPTY);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ERROR_STACK_FULL);

		default:
			break;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

const char* XenonGetEndianModeString(int endianness)
{
	switch(endianness)
	{
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ENDIAN_MODE_UNKNOWN);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ENDIAN_MODE_NATIVE);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ENDIAN_MODE_LITTLE);
		XENON_SWITCH_CASE_RETURN_STRING(XENON_ENDIAN_MODE_BIG);

		default:
			break;
	}

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonGetPlatformEndianMode()
{
#ifdef XENON_CPU_ENDIAN_LITTLE
	return XENON_ENDIAN_MODE_LITTLE;

#else
	return XENON_ENDIAN_MODE_BIG;

#endif
}

//----------------------------------------------------------------------------------------------------------------------

const char* XenonGetBuiltInFunctionSignature(int builtInFunctionId)
{
	switch(builtInFunctionId)
	{
		case XENON_BUILT_IN_OP_ADD_BOOL:    return "bool `builtin.OpAdd(bool, bool)";
		case XENON_BUILT_IN_OP_ADD_INT8:    return "int8 `builtin.OpAdd(int8, int8)";
		case XENON_BUILT_IN_OP_ADD_INT16:   return "int16 `builtin.OpAdd(int16, int16)";
		case XENON_BUILT_IN_OP_ADD_INT32:   return "int32 `builtin.OpAdd(int32, int32)";
		case XENON_BUILT_IN_OP_ADD_INT64:   return "int64 `builtin.OpAdd(int64, int64)";
		case XENON_BUILT_IN_OP_ADD_UINT8:   return "uint8 `builtin.OpAdd(uint8, uint8)";
		case XENON_BUILT_IN_OP_ADD_UINT16:  return "uint16 `builtin.OpAdd(uint16, uint16)";
		case XENON_BUILT_IN_OP_ADD_UINT32:  return "uint32 `builtin.OpAdd(uint32, uint32)";
		case XENON_BUILT_IN_OP_ADD_UINT64:  return "uint64 `builtin.OpAdd(uint64, uint64)";
		case XENON_BUILT_IN_OP_ADD_FLOAT32: return "float32 `builtin.OpAdd(float32, float32)";
		case XENON_BUILT_IN_OP_ADD_FLOAT64: return "float64 `builtin.OpAdd(float64, float64)";
		case XENON_BUILT_IN_OP_ADD_STRING:  return "string `builtin.OpAdd(string, string)";

		case XENON_BUILT_IN_OP_SUB_BOOL:    return "bool `builtin.OpSub(bool, bool)";
		case XENON_BUILT_IN_OP_SUB_INT8:    return "int8 `builtin.OpSub(int8, int8)";
		case XENON_BUILT_IN_OP_SUB_INT16:   return "int16 `builtin.OpSub(int16, int16)";
		case XENON_BUILT_IN_OP_SUB_INT32:   return "int32 `builtin.OpSub(int32, int32)";
		case XENON_BUILT_IN_OP_SUB_INT64:   return "int64 `builtin.OpSub(int64, int64)";
		case XENON_BUILT_IN_OP_SUB_UINT8:   return "uint8 `builtin.OpSub(uint8, uint8)";
		case XENON_BUILT_IN_OP_SUB_UINT16:  return "uint16 `builtin.OpSub(uint16, uint16)";
		case XENON_BUILT_IN_OP_SUB_UINT32:  return "uint32 `builtin.OpSub(uint32, uint32)";
		case XENON_BUILT_IN_OP_SUB_UINT64:  return "uint64 `builtin.OpSub(uint64, uint64)";
		case XENON_BUILT_IN_OP_SUB_FLOAT32: return "float32 `builtin.OpSub(float32, float32)";
		case XENON_BUILT_IN_OP_SUB_FLOAT64: return "float64 `builtin.OpSub(float64, float64)";

		case XENON_BUILT_IN_OP_MUL_BOOL:    return "bool `builtin.OpMul(bool, bool)";
		case XENON_BUILT_IN_OP_MUL_INT8:    return "int8 `builtin.OpMul(int8, int8)";
		case XENON_BUILT_IN_OP_MUL_INT16:   return "int16 `builtin.OpMul(int16, int16)";
		case XENON_BUILT_IN_OP_MUL_INT32:   return "int32 `builtin.OpMul(int32, int32)";
		case XENON_BUILT_IN_OP_MUL_INT64:   return "int64 `builtin.OpMul(int64, int64)";
		case XENON_BUILT_IN_OP_MUL_UINT8:   return "uint8 `builtin.OpMul(uint8, uint8)";
		case XENON_BUILT_IN_OP_MUL_UINT16:  return "uint16 `builtin.OpMul(uint16, uint16)";
		case XENON_BUILT_IN_OP_MUL_UINT32:  return "uint32 `builtin.OpMul(uint32, uint32)";
		case XENON_BUILT_IN_OP_MUL_UINT64:  return "uint64 `builtin.OpMul(uint64, uint64)";
		case XENON_BUILT_IN_OP_MUL_FLOAT32: return "float32 `builtin.OpMul(float32, float32)";
		case XENON_BUILT_IN_OP_MUL_FLOAT64: return "float64 `builtin.OpMul(float64, float64)";

		case XENON_BUILT_IN_OP_DIV_BOOL:    return "bool `builtin.OpDiv(bool, bool)";
		case XENON_BUILT_IN_OP_DIV_INT8:    return "int8 `builtin.OpDiv(int8, int8)";
		case XENON_BUILT_IN_OP_DIV_INT16:   return "int16 `builtin.OpDiv(int16, int16)";
		case XENON_BUILT_IN_OP_DIV_INT32:   return "int32 `builtin.OpDiv(int32, int32)";
		case XENON_BUILT_IN_OP_DIV_INT64:   return "int64 `builtin.OpDiv(int64, int64)";
		case XENON_BUILT_IN_OP_DIV_UINT8:   return "uint8 `builtin.OpDiv(uint8, uint8)";
		case XENON_BUILT_IN_OP_DIV_UINT16:  return "uint16 `builtin.OpDiv(uint16, uint16)";
		case XENON_BUILT_IN_OP_DIV_UINT32:  return "uint32 `builtin.OpDiv(uint32, uint32)";
		case XENON_BUILT_IN_OP_DIV_UINT64:  return "uint64 `builtin.OpDiv(uint64, uint64)";
		case XENON_BUILT_IN_OP_DIV_FLOAT32: return "float32 `builtin.OpDiv(float32, float32)";
		case XENON_BUILT_IN_OP_DIV_FLOAT64: return "float64 `builtin.OpDiv(float64, float64)";
	};

	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------

XenonMemAllocator XenonMemGetDefaultAllocator()
{
	XenonMemAllocator allocator;
	allocator.allocFn = malloc;
	allocator.reallocFn = realloc;
	allocator.freeFn = free;

	return allocator;
}

//----------------------------------------------------------------------------------------------------------------------

XenonMemAllocator XenonMemGetAllocator()
{
	return globalAllocator;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonMemSetAllocator(XenonMemAllocator allocator)
{
	if(!allocator.allocFn || !allocator.reallocFn || !allocator.freeFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	globalAllocator = allocator;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonMemAlloc(const size_t size)
{
	return globalAllocator.allocFn(size);
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonMemRealloc(void* const pMem, const size_t size)
{
	return globalAllocator.reallocFn(pMem, size);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonMemFree(void* const pMem)
{
	if(!pMem)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	globalAllocator.freeFn(pMem);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonReportMessage(XenonReportHandle hReport, const int messageType, const char* const fmt, ...)
{
	if(!hReport)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(!hReport->onMessageFn || hReport->level > messageType)
	{
		// Do nothing when we're unable to report anything for the given parameters.
		return XENON_SUCCESS;
	}

	va_list vl, vl2;

	// Make a copy of the variable args list since we need to go through them twice.
	va_start(vl, fmt);
	va_copy(vl2, vl);

	// Determine the required length of the message string.
	const int messageLength = vsnprintf(nullptr, 0, fmt, vl2);

	// Discard empty messages.
	if(messageLength > 0)
	{
		// Allocate a message string to the required length, including space for the null terminator.
		char* const message = reinterpret_cast<char*>(XenonMemAlloc(messageLength + 1));

		// Write the message to the string.
		vsnprintf(message, size_t(messageLength + 1), fmt, vl);

		// Send off the message, then free the message string.
		hReport->onMessageFn(hReport->pUserData, messageType, message);
		XenonMemFree(message);
	}

	va_end(vl2);
	va_end(vl);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonReportSetLevel(XenonReportHandle hReport, const int messageType)
{
	if(hReport && messageType <= XENON_MESSAGE_TYPE_FATAL)
	{
		hReport->level = messageType;
		return XENON_SUCCESS;
	}

	return XENON_ERROR_INVALID_ARG;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonReportGetLevel(XenonReportHandle hReport)
{
	if(hReport)
	{
		return hReport->level;
	}

	return XENON_MESSAGE_TYPE_FATAL;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerCreate(XenonSerializerHandle* phOutSerializer, int mode)
{
	if(!phOutSerializer
		|| (*phOutSerializer) != XENON_SERIALIZER_HANDLE_NULL
		|| mode < XENON_SERIALIZER_MODE_READER
		|| mode > XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	(*phOutSerializer) = XenonSerializer::Create(mode);

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerDispose(XenonSerializerHandle* phSerializer)
{
	if(phSerializer && (*phSerializer))
	{
		XenonSerializer::Dispose(*phSerializer);

		(*phSerializer) = XENON_SERIALIZER_HANDLE_NULL;

		return XENON_SUCCESS;
	}

	return XENON_ERROR_INVALID_ARG;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerGetMode(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return XENON_SERIALIZER_MODE_UNKNOWN;
	}

	return hSerializer->mode;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerGetEndianness(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return XENON_ENDIAN_MODE_UNKNOWN;
	}

	return hSerializer->endianness;
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonSerializerGetStreamLength(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return size_t(-1);
	}

	return hSerializer->stream.count;
}

//----------------------------------------------------------------------------------------------------------------------

size_t XenonSerializerGetStreamPosition(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return size_t(-1);
	}

	return hSerializer->position;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonSerializerGetRawStreamPointer(XenonSerializerHandle hSerializer)
{
	if(!hSerializer)
	{
		return nullptr;
	}

	return hSerializer->stream.pData;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerSetEndianness(XenonSerializerHandle hSerializer, int endianness)
{
	if(!hSerializer || endianness < XENON_ENDIAN_MODE_NATIVE || endianness > XENON_ENDIAN_MODE_BIG)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	hSerializer->endianness = endianness;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerSetStreamPosition(XenonSerializerHandle hSerializer, size_t position)
{
	if(!hSerializer || hSerializer->stream.count < position)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	hSerializer->position = position;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerLoadStreamFromFile(XenonSerializerHandle hSerializer, const char* filePath)
{
	if(!hSerializer || !filePath || filePath[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::LoadFile(hSerializer, filePath);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerLoadStreamFromBuffer(XenonSerializerHandle hSerializer, const void* pBuffer, size_t bufferLength)
{
	if(!hSerializer || !pBuffer || bufferLength == 0)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::LoadBuffer(hSerializer, pBuffer, bufferLength);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerSaveStreamToFile(XenonSerializerHandle hSerializer, const char* filePath, bool append)
{
	if(!hSerializer || !filePath || filePath[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::SaveFile(hSerializer, filePath, append);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerSaveStreamToBuffer(XenonSerializerHandle hSerializer, void* pBuffer, size_t* pBufferLength)
{
	if(!hSerializer || !pBufferLength)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::SaveBuffer(hSerializer, pBuffer, pBufferLength);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteBool(XenonSerializerHandle hSerializer, bool value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	uint8_t temp = value ? 1 : 0;

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&temp), sizeof(uint8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteInt8(XenonSerializerHandle hSerializer, int8_t value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(int8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteInt16(XenonSerializerHandle hSerializer, int16_t value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(int16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteInt32(XenonSerializerHandle hSerializer, int32_t value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(int32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteInt64(XenonSerializerHandle hSerializer, int64_t value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(int64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteUint8(XenonSerializerHandle hSerializer, uint8_t value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(uint8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteUint16(XenonSerializerHandle hSerializer, uint16_t value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(uint16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteUint32(XenonSerializerHandle hSerializer, uint32_t value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(uint32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteUint64(XenonSerializerHandle hSerializer, uint64_t value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(uint64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteFloat32(XenonSerializerHandle hSerializer, float value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(float));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteFloat64(XenonSerializerHandle hSerializer, double value)
{
	if(!hSerializer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteData(hSerializer, reinterpret_cast<uint8_t*>(&value), sizeof(double));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerWriteBuffer(XenonSerializerHandle hSerializer, size_t bufferLength, const void* pBuffer)
{
	if(!hSerializer || bufferLength == 0 || !pBuffer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(hSerializer->mode != XENON_SERIALIZER_MODE_WRITER)
	{
		return XENON_ERROR_INVALID_TYPE;
	}

	return XenonSerializer::WriteRawData(hSerializer, pBuffer, bufferLength);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadBool(XenonSerializerHandle hSerializer, bool* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	uint8_t temp = 0;

	int result = XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(&temp), sizeof(uint8_t));
	if(result == XENON_SUCCESS)
	{
		(*pOutValue) = temp ? true : false;
	}

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadInt8(XenonSerializerHandle hSerializer, int8_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(int8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadInt16(XenonSerializerHandle hSerializer, int16_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(int16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadInt32(XenonSerializerHandle hSerializer, int32_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(int32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadInt64(XenonSerializerHandle hSerializer, int64_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(int64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadUint8(XenonSerializerHandle hSerializer, uint8_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(uint8_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadUint16(XenonSerializerHandle hSerializer, uint16_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(uint16_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadUint32(XenonSerializerHandle hSerializer, uint32_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(uint32_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadUint64(XenonSerializerHandle hSerializer, uint64_t* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(uint64_t));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadFloat32(XenonSerializerHandle hSerializer, float* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(float));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadFloat64(XenonSerializerHandle hSerializer, double* pOutValue)
{
	if(!hSerializer || !pOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadData(hSerializer, reinterpret_cast<uint8_t*>(pOutValue), sizeof(double));
}

//----------------------------------------------------------------------------------------------------------------------

int XenonSerializerReadBuffer(XenonSerializerHandle hSerializer, size_t bufferLength, void* pOutBuffer)
{
	if(!hSerializer || bufferLength == 0 || !pOutBuffer)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonSerializer::ReadRawData(hSerializer, pOutBuffer, bufferLength);
}

//----------------------------------------------------------------------------------------------------------------------

int16_t XenonEndianSwapInt16(const int16_t value)
{
	return XENON_SWAP_16(value);
}

//----------------------------------------------------------------------------------------------------------------------

int32_t XenonEndianSwapInt32(const int32_t value)
{
	return XENON_SWAP_32(value);
}

//----------------------------------------------------------------------------------------------------------------------

int64_t XenonEndianSwapInt64(const int64_t value)
{
	return XENON_SWAP_64(value);
}

//----------------------------------------------------------------------------------------------------------------------

uint16_t XenonEndianSwapUint16(const uint16_t value)
{
	return XENON_SWAP_16(value);
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonEndianSwapUint32(const uint32_t value)
{
	return XENON_SWAP_32(value);
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t XenonEndianSwapUint64(const uint64_t value)
{
	return XENON_SWAP_64(value);
}

//----------------------------------------------------------------------------------------------------------------------

float XenonEndianSwapFloat32(const float value)
{
	union
	{
		float tempFloat;
		int32_t tempInt;
	};

	tempFloat = value;
	tempInt = XENON_SWAP_32(tempInt);

	return tempFloat;
}

//----------------------------------------------------------------------------------------------------------------------

double XenonEndianSwapFloat64(const double value)
{
	union
	{
		double tempFloat;
		int64_t tempInt;
	};

	tempFloat = value;
	tempInt = XENON_SWAP_64(tempInt);

	return tempFloat;
}

//----------------------------------------------------------------------------------------------------------------------

}
