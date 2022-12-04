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

#include "Serializer.hpp"

#include <assert.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

namespace SerializerUtility
{
	void CopyData(uint8_t* const pDest, const uint8_t* const pSrc, const size_t length, const int endianness)
	{
		bool reverseEndian = false;

		switch(endianness)
		{
#ifndef HQ_CPU_ENDIAN_UNKNOWN
	#ifdef HQ_CPU_ENDIAN_BIG
			case HQ_ENDIAN_ORDER_LITTLE:
	#else
			case HQ_ENDIAN_ORDER_BIG:
	#endif
				reverseEndian = true;
				break;
#endif

			default:
				break;
		}

		if(!reverseEndian)
		{
			// Copy the source data as it is.
			for(size_t i = 0; i < length; ++i)
			{
				pDest[i] = pSrc[i];
			}
		}
		else
		{
			// Copy the source data in reverse order,
			// effectively endian swapping it.
			for(size_t i = 0; i < length; ++i)
			{
				pDest[i] = pSrc[length - 1 - i];
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

HqSerializerHandle HqSerializer::Create(int mode)
{
	HqSerializer* const pOutput = new HqSerializer();
	if(!pOutput)
	{
		return HQ_SERIALIZER_HANDLE_NULL;
	}

	HqByteHelper::Array::Initialize(pOutput->stream);

	pOutput->position = 0;
	pOutput->mode = mode;
	pOutput->endianness = HQ_ENDIAN_ORDER_NATIVE;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqSerializer::Dispose(HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	HqByteHelper::Array::Dispose(hSerializer->stream);

	delete hSerializer;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializer::LoadFile(HqSerializerHandle hSerializer, const char* const filePath)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(filePath != nullptr);
	assert(filePath[0] != '\0');

	FILE* const pFile = fopen(filePath, "rb");
	if(!pFile)
	{
		return HQ_ERROR_FAILED_TO_OPEN_FILE;
	}

	// Move to the end of the file, then get the position within file.  This will give us the total
	// size in bytes of the file.  Then, we move the position back to the beginning of the file.
	fseek(pFile, 0, SEEK_END);
	const size_t fileSize = size_t(ftell(pFile));
	fseek(pFile, 0, SEEK_SET);

	// Discard the old stream contents.
	HqByteHelper::Array::Dispose(hSerializer->stream);

	if(fileSize > 0)
	{
		// Reserve space in the stream for the new contents.
		HqByteHelper::Array::Reserve(hSerializer->stream, fileSize);
		assert(hSerializer->stream.pData != nullptr);

		// Read the file data into the stream.
		const size_t itemsRead = fread(hSerializer->stream.pData, fileSize, 1, pFile);
		assert(itemsRead == 1);

		// Avoid compiler warning in release builds.
		(void)itemsRead;

		// Since we explicitly loaded new contents into the stream, we set the size of the stream
		// and reset the serializer position based on the serializer mode.
		hSerializer->stream.count = fileSize;
		hSerializer->position = (hSerializer->mode == HQ_SERIALIZER_MODE_WRITER) ? fileSize : 0;
	}

	fclose(pFile);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializer::LoadBuffer(HqSerializerHandle hSerializer, const void* const pBuffer, const size_t length)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pBuffer != nullptr);
	assert(length > 0);

	// Discard the current stream contents, then reserve space for the new contents.
	HqByteHelper::Array::Dispose(hSerializer->stream);
	HqByteHelper::Array::Reserve(hSerializer->stream, length);
	assert(hSerializer->stream.pData != nullptr);

	// Copy the input memory contents to the internal stream.
	memcpy(hSerializer->stream.pData, pBuffer, length);

	hSerializer->stream.count = length;
	hSerializer->position = (hSerializer->mode == HQ_SERIALIZER_MODE_WRITER) ? length : 0;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializer::SaveFile(HqSerializerHandle hSerializer, const char* const filePath, const bool append)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(filePath != nullptr);
	assert(filePath[0] != '\0');

	// Open the file either for appending or as a new file.
	FILE* const pFile = fopen(filePath, append ? "ab" : "wb");
	if(!pFile)
	{
		return HQ_ERROR_FAILED_TO_OPEN_FILE;
	}

	const size_t streamSize = hSerializer->stream.count;

	if(streamSize > 0)
	{
		// Write the stream data into the file.
		const size_t itemsWritten = fwrite(hSerializer->stream.pData, streamSize, 1, pFile);
		assert(itemsWritten == 1);

		// Avoid compiler warning in release builds.
		(void)itemsWritten;
	}

	fclose(pFile);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializer::SaveBuffer(HqSerializerHandle hSerializer, void* const pOutBuffer, size_t* const pLength)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pOutBuffer != nullptr);
	assert(pLength != nullptr);

	if(!pOutBuffer)
	{
		(*pLength) = hSerializer->stream.count;
		return HQ_SUCCESS;
	}

	if(hSerializer->stream.pData)
	{
		memcpy(pOutBuffer, hSerializer->stream.pData, *pLength);
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializer::WriteData(HqSerializerHandle hSerializer, const uint8_t* const pSource, const size_t length)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pSource != nullptr);
	assert(length > 0);

	const size_t requiredSize = hSerializer->stream.count + length;
	if(requiredSize > hSerializer->stream.capacity)
	{
		HqByteHelper::Array::Reserve(hSerializer->stream, requiredSize);
		assert(hSerializer->stream.pData != nullptr);
	}

	uint8_t* const pDest = hSerializer->stream.pData + hSerializer->position;

	hSerializer->position += length;

	// Update the stream length of the position has moved past it.
	if(hSerializer->stream.count < hSerializer->position)
	{
		hSerializer->stream.count = hSerializer->position;
	}

	SerializerUtility::CopyData(pDest, pSource, length, hSerializer->endianness);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializer::WriteRawData(HqSerializerHandle hSerializer, const void* const pSource, const size_t length)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pSource != nullptr);
	assert(length > 0);

	const size_t requiredSize = hSerializer->stream.count + length;
	if(requiredSize > hSerializer->stream.capacity)
	{
		HqByteHelper::Array::Reserve(hSerializer->stream, requiredSize);
		assert(hSerializer->stream.pData != nullptr);
	}

	uint8_t* const pDest = hSerializer->stream.pData + hSerializer->position;

	hSerializer->position += length;

	// Update the stream length of the position has moved past it.
	if(hSerializer->stream.count < hSerializer->position)
	{
		hSerializer->stream.count = hSerializer->position;
	}

	// Copy the entire buffer as it is.
	memcpy(pDest, pSource, length);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializer::ReadData(HqSerializerHandle hSerializer, uint8_t* const pDest, const size_t length)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pDest != nullptr);
	assert(length > 0);

	if(hSerializer->stream.count < hSerializer->position + length)
	{
		return HQ_ERROR_STREAM_END;
	}

	const uint8_t* const pSource = hSerializer->stream.pData + hSerializer->position;
	hSerializer->position += length;

	SerializerUtility::CopyData(pDest, pSource, length, hSerializer->endianness);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int HqSerializer::ReadRawData(HqSerializerHandle hSerializer, void* const pDest, const size_t length)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pDest != nullptr);
	assert(length > 0);

	if(hSerializer->stream.count < hSerializer->position + length)
	{
		return HQ_ERROR_STREAM_END;
	}

	const uint8_t* const pSource = hSerializer->stream.pData + hSerializer->position;
	hSerializer->position += length;

	memcpy(pDest, pSource, length);

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqSerializer::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqSerializer::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
