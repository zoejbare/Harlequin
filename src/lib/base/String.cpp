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

#include "String.hpp"
#include "System.hpp"

#include "unicode/utf32-record-table.h"

#include "../common/Hash.hpp"

#include <assert.h>
#include <stdio.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

#define _HQ_INVALID_CODE_POINT 0xFFFD

//----------------------------------------------------------------------------------------------------------------------

// UTF-8 upper sequence bits
constexpr uint8_t _seqId[] =
{
	0x80, // 0b10......
	0xC0, // 0b110.....
	0xE0, // 0b1110....
	0xF0, // 0b11110...
};

// UTF-8 bit masks for extracting code points from each corresponding sequence byte.
// The exception is index 0, which is for extracting the continuation bytes for the
// sequence. When a starting byte is less than 0x80, the byte is considered ASCII,
// which means it will be used as is.
constexpr uint8_t _valueMask[] =
{
	uint8_t(~_seqId[0]) >> 1,
	uint8_t(~_seqId[1]) >> 1,
	uint8_t(~_seqId[2]) >> 1,
	uint8_t(~_seqId[3]) >> 1,
};

// Mask for checking the validity of a given byte.
constexpr uint8_t _checkMask[] =
{
	uint8_t(~_valueMask[0]),
	uint8_t(~_valueMask[1]),
	uint8_t(~_valueMask[2]),
	uint8_t(~_valueMask[3]),
};

//----------------------------------------------------------------------------------------------------------------------

inline size_t _HqCharUtf32ToUtf8(char* const pOutMbSeq, const char32_t codePoint)
{
	// Upper limit of the range for each code point set.
	constexpr char32_t range[] =
	{
		0x000080,
		0x000800,
		0x010000,
		0x110000,
	};

	char32_t inputValue = codePoint;

utf8ConvBegin:

	if(inputValue < range[0])
	{
		// ASCII character.
		pOutMbSeq[0] = char(inputValue);
		return 1;
	}
	else if(inputValue < range[1])
	{
		// 2-byte sequence.
		pOutMbSeq[1] = char(_seqId[0] | (uint8_t(inputValue >> 0) & _valueMask[0]));
		pOutMbSeq[0] = char(_seqId[1] | (uint8_t(inputValue >> 6) & _valueMask[1]));
		return 2;
	}
	else if(inputValue < range[2])
	{
		// 3-byte sequence.
		pOutMbSeq[2] = char(_seqId[0] | (uint8_t(inputValue >>  0) & _valueMask[0]));
		pOutMbSeq[1] = char(_seqId[0] | (uint8_t(inputValue >>  6) & _valueMask[0]));
		pOutMbSeq[0] = char(_seqId[2] | (uint8_t(inputValue >> 12) & _valueMask[2]));
		return 3;
	}
	else if(inputValue < range[3])
	{
		// 4-byte sequence.
		pOutMbSeq[3] = char(_seqId[0] | (uint8_t(inputValue >>  0) & _valueMask[0]));
		pOutMbSeq[2] = char(_seqId[0] | (uint8_t(inputValue >>  6) & _valueMask[0]));
		pOutMbSeq[1] = char(_seqId[0] | (uint8_t(inputValue >> 12) & _valueMask[0]));
		pOutMbSeq[0] = char(_seqId[3] | (uint8_t(inputValue >> 18) & _valueMask[3]));
		return 4;
	}
	else
	{
		// Exceeds UTF-8 limits, meaning it's not a valid character.
		// In this case, we set the code point to a known valid character
		// to represent the input value is unknown.
		inputValue = _HQ_INVALID_CODE_POINT;
		goto utf8ConvBegin;
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline char32_t _HqCharUtf8ToUtf32(size_t* const pOutMbLen, const char* const mbSeq)
{
	assert(pOutMbLen != nullptr);
	assert(mbSeq != nullptr);

	// Set the default output in case there is an error decoding the input sequence.
	char32_t output = _HQ_INVALID_CODE_POINT;
	size_t mbLen = 1;

	char32_t codePoint = output;

	// Cache the first byte in the sequence since we'll be accessing it a few times.
	const uint8_t byte0 = uint8_t(mbSeq[0]);

	// 4-byte sequence
	if((byte0 & _checkMask[3]) == _seqId[3])
	{
		codePoint = char32_t(byte0 & _valueMask[3]);
		mbLen = 4;

		const uint8_t byte1 = uint8_t(mbSeq[1]);
		const uint8_t byte2 = uint8_t(mbSeq[2]);
		const uint8_t byte3 = uint8_t(mbSeq[3]);

		if((byte1 & _checkMask[0]) != _seqId[0]
			|| (byte2 & _checkMask[0]) != _seqId[0]
			|| (byte3 & _checkMask[0]) != _seqId[0])
		{
			goto utf32ConvFinish;
		}

		codePoint <<= 6;
		codePoint |= char32_t(byte1 & _valueMask[0]);

		codePoint <<= 6;
		codePoint |= char32_t(byte2 & _valueMask[0]);

		codePoint <<= 6;
		codePoint |= char32_t(byte3 & _valueMask[0]);
	}

	// 3-byte sequence
	else if((byte0 & _checkMask[2]) == _seqId[2])
	{
		codePoint = char32_t(byte0 & _valueMask[2]);
		mbLen = 3;

		const uint8_t byte1 = uint8_t(mbSeq[1]);
		const uint8_t byte2 = uint8_t(mbSeq[2]);

		if((byte1 & _checkMask[0]) != _seqId[0]
			|| (byte2 & _checkMask[0]) != _seqId[0])
		{
			goto utf32ConvFinish;
		}

		codePoint <<= 6;
		codePoint |= char32_t(byte1 & _valueMask[0]);

		codePoint <<= 6;
		codePoint |= char32_t(byte2 & _valueMask[0]);
	}

	// 2-byte sequence
	else if((byte0 & _checkMask[1]) == _seqId[1])
	{
		codePoint = char32_t(byte0 & _valueMask[1]);
		mbLen = 2;

		const uint8_t byte1 = uint8_t(mbSeq[1]);

		if((byte1 & _checkMask[0]) != _seqId[0])
		{
			goto utf32ConvFinish;
		}

		codePoint <<= 6;
		codePoint |= char32_t(byte1 & _valueMask[0]);
	}

	// ASCII character
	else if(byte0 < _seqId[0])
	{
		codePoint = byte0;
		mbLen = 1;
	}

	output = codePoint;

utf32ConvFinish:
	(*pOutMbLen) = mbLen;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

inline char32_t _HqCharToUtf32SimpleLower(const char32_t codePoint)
{
	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT)
		? utf32RecordTable.indices[codePoint]
		: 0;

	if(recordIndex > 0)
	{
		return utf32RecordTable.records[recordIndex].lowerCase.simple;
	}

	// The input code point is out of the supported range or has no case mapping data.
	return codePoint;
}

//----------------------------------------------------------------------------------------------------------------------

inline char32_t _HqCharToUtf32SimpleUpper(const char32_t codePoint)
{
	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT)
		? utf32RecordTable.indices[codePoint]
		: 0;

	if(recordIndex > 0)
	{
		return utf32RecordTable.records[recordIndex].upperCase.simple;
	}

	// The input code point is out of the supported range or has no case mapping data.
	return codePoint;
}

//----------------------------------------------------------------------------------------------------------------------

inline char32_t _HqCharToUtf32SimpleTitle(const char32_t codePoint)
{
	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT)
		? utf32RecordTable.indices[codePoint]
		: 0;

	if(recordIndex > 0)
	{
		return utf32RecordTable.records[recordIndex].titleCase.simple;
	}

	// The input code point is out of the supported range or has no case mapping data.
	return codePoint;
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t _HqCharToUtf32StrictLower(char32_t* const pOutCodePoints, const char32_t codePoint)
{
	assert(pOutCodePoints != nullptr);

	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT)
		? utf32RecordTable.indices[codePoint]
		: 0;

	if(recordIndex > 0)
	{
		const HqUtf32Mapping& mapping = utf32RecordTable.records[recordIndex].lowerCase;

		memcpy(pOutCodePoints, mapping.strict, sizeof(char32_t) * mapping.strictLength);

		return mapping.strictLength;
	}

	// The input code point is out of the supported range or has no case mapping data.
	(*pOutCodePoints) = codePoint;
	return 1;
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t _HqCharToUtf32StrictUpper(char32_t* const pOutCodePoints, const char32_t codePoint)
{
	assert(pOutCodePoints != nullptr);

	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT)
		? utf32RecordTable.indices[codePoint]
		: 0;

	if(recordIndex > 0)
	{
		const HqUtf32Mapping& mapping = utf32RecordTable.records[recordIndex].upperCase;

		memcpy(pOutCodePoints, mapping.strict, sizeof(char32_t) * mapping.strictLength);

		return mapping.strictLength;
	}

	// The input code point is out of the supported range or has no case mapping data.
	(*pOutCodePoints) = codePoint;
	return 1;
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t _HqCharToUtf32StrictTitle(char32_t* const pOutCodePoints, const char32_t codePoint)
{
	assert(pOutCodePoints != nullptr);

	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT)
		? utf32RecordTable.indices[codePoint]
		: 0;

	if(recordIndex > 0)
	{
		const HqUtf32Mapping& mapping = utf32RecordTable.records[recordIndex].titleCase;

		memcpy(pOutCodePoints, mapping.strict, sizeof(char32_t) * mapping.strictLength);

		return mapping.strictLength;
	}

	// The input code point is out of the supported range or has no case mapping data.
	(*pOutCodePoints) = codePoint;
	return 1;
}

//----------------------------------------------------------------------------------------------------------------------

inline void _HqStringSimpleCaseOperation(HqString* const pString, char32_t (*SimpleCaseOpFunc)(char32_t))
{
	assert(pString != nullptr);
	assert(SimpleCaseOpFunc != nullptr);

	if(pString->length > 0)
	{
		size_t outputStringLength = 0;
		char temp[4];

		// Calculate the required size for the output string.
		// This is not optimal, but the output string can potentially
		// have a different length from the input string.
		for(size_t inputIndex = 0; inputIndex < pString->length;)
		{
			size_t inputByteLength = 0;
			const char32_t originalCodePoint = _HqCharUtf8ToUtf32(&inputByteLength, &pString->data[inputIndex]);
			const char32_t convertedCodePoint = SimpleCaseOpFunc(originalCodePoint);
			const size_t outputByteLength = _HqCharUtf32ToUtf8(temp, convertedCodePoint);

			inputIndex += inputByteLength;
			outputStringLength += outputByteLength;
		}

		// Allocate a new string.
		char* const outputData = reinterpret_cast<char*>(HqMemAlloc(sizeof(char) * (outputStringLength + 1)));

		// Add the null terminator to the end of the new string.
		outputData[outputStringLength] = '\0';

		// Convert the input string, storing the new characters in the output string.
		for(size_t inputIndex = 0, outputIndex = 0; inputIndex < pString->length;)
		{
			size_t inputByteLength = 0;
			const char32_t originalCodePoint = _HqCharUtf8ToUtf32(&inputByteLength, &pString->data[inputIndex]);
			const char32_t convertedCodePoint = SimpleCaseOpFunc(originalCodePoint);
			const size_t outputByteLength = _HqCharUtf32ToUtf8(temp, convertedCodePoint);

			// Copy the converted bytes to the output string.
			for(size_t subIndex = 0; subIndex < outputByteLength; ++subIndex)
			{
				outputData[subIndex + outputIndex] = temp[subIndex];
			}

			inputIndex += inputByteLength;
			outputIndex += outputByteLength;
		}

		// Free the old string memory.
		HqMemFree(pString->data);

		pString->data = outputData;
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void _HqStringStrictCaseOperation(HqString* const pString, size_t (*FullCaseOpFunc)(char32_t*, char32_t))
{
	assert(pString != nullptr);
	assert(FullCaseOpFunc != nullptr);

	if(pString->length > 0)
	{
		size_t outputStringLength = 0;
		char tempUtf8[4];
		char32_t tempUtf32[HQ_UTF32_STRICT_MAPPING_ARRAY_LENGTH];

		// Calculate the required size for the output string.
		// This is not optimal, but the output string can potentially
		// have a different length from the input string.
		for(size_t inputIndex = 0; inputIndex < pString->length;)
		{
			size_t inputByteLength = 0;
			const char32_t originalCodePoint = _HqCharUtf8ToUtf32(&inputByteLength, &pString->data[inputIndex]);
			const size_t convertedLength = FullCaseOpFunc(tempUtf32, originalCodePoint);

			// Calculate the length for each converted code point.
			for(size_t subIndex = 0; subIndex < convertedLength; ++subIndex)
			{
				const size_t outputByteLength = _HqCharUtf32ToUtf8(tempUtf8, tempUtf32[subIndex]);

				outputStringLength += outputByteLength;
			}

			inputIndex += inputByteLength;
		}

		// Allocate a new string.
		char* const outputData = reinterpret_cast<char*>(HqMemAlloc(sizeof(char) * (outputStringLength + 1)));

		// Add the null terminator to the end of the new string.
		outputData[outputStringLength] = '\0';

		// Convert the input string, storing the new characters in the output string.
		for(size_t inputIndex = 0, outputIndex = 0; inputIndex < pString->length;)
		{
			size_t inputByteLength = 0;
			const char32_t originalCodePoint = _HqCharUtf8ToUtf32(&inputByteLength, &pString->data[inputIndex]);
			const size_t convertedLength = FullCaseOpFunc(tempUtf32, originalCodePoint);

			// Calculate the length for each converted code point.
			for(size_t codePointIndex = 0; codePointIndex < convertedLength; ++codePointIndex)
			{
				const size_t outputByteLength = _HqCharUtf32ToUtf8(tempUtf8, tempUtf32[codePointIndex]);

				// Copy the converted bytes to the output string.
				for(size_t subIndex = 0; subIndex < outputByteLength; ++subIndex)
				{
					outputData[subIndex + outputIndex] = tempUtf8[subIndex];
				}

				outputIndex += outputByteLength;
			}

			inputIndex += inputByteLength;
		}

		// Free the old string memory.
		HqMemFree(pString->data);

		pString->data = outputData;
	}
}

//----------------------------------------------------------------------------------------------------------------------

HqString* HqString::Create(const char* const stringData)
{
	HqString* const pOutput = new HqString();
	assert(pOutput != nullptr);

	const size_t length = (stringData) ? strlen(stringData) : 0;

	pOutput->length = length;
	pOutput->hash = RawHash(stringData ? stringData : "");
	pOutput->data = reinterpret_cast<char*>(HqMemAlloc(length + 1));

	HqReference::Initialize(pOutput->ref, _onDestruct, pOutput);

	if(stringData && pOutput->data)
	{
		memcpy(pOutput->data, stringData, length);
		pOutput->data[length] = '\0';
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqString* HqString::CreateFmt(const char* const fmt, ...)
{
	if(!fmt || fmt[0] == '\0')
	{
		return Create("");
	}

	HqString* const pOutput = new HqString();
	assert(pOutput != nullptr);

	va_list vl;
	va_start(vl, fmt);

	char* const stringData = HqString::RawFormatVarArgs(fmt, vl);
	assert(stringData != nullptr);

	va_end(vl);

	const size_t length = strlen(stringData);

	pOutput->length = length;
	pOutput->hash = RawHash(stringData);
	pOutput->data = stringData;

	HqReference::Initialize(pOutput->ref, _onDestruct, pOutput);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqString::AddRef(HqString* const pString)
{
	return (pString)
		? HqReference::AddRef(pString->ref)
		: -1;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqString::Release(HqString* const pString)
{
	return (pString)
		? HqReference::Release(pString->ref)
		: -1;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqString::SlowCompare(const HqString* const pLeft, const HqString* const pRight)
{
	assert(pLeft != nullptr);
	assert(pLeft->data != nullptr);

	assert(pRight != nullptr);
	assert(pRight->data != nullptr);

	if(pLeft->data == pRight->data)
	{
		// Same string in memory.
		return 0;
	}

	// Let strcmp() handle the rest for us (hence this being the 'slow' compare).
	return strcmp(pLeft->data, pRight->data);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqString::FastCompare(const HqString* const pLeft, const HqString* const pRight)
{
	assert(pLeft != nullptr);
	assert(pLeft->data != nullptr);

	assert(pRight != nullptr);
	assert(pRight->data != nullptr);

	if(pLeft->data == pRight->data)
	{
		// Same string in memory.
		return true;
	}

	if(pLeft->length != pRight->length)
	{
		// Different string lengths.
		return false;
	}

	if(pLeft->hash != pRight->hash)
	{
		// Different hashes which can only be generated by different
		// strings since the hashing is deterministic.
		return false;
	}

	// We should only get here if the two strings are located in different spots in memory, have the same length,
	// and have same hash. This case is *possible* for two different strings if they have their lengths are the same,
	// but contain different data. That would be the result of a hash collision, but more often than not, it'll be
	// because the two string objects are separate objects, but happen to contain identical data.
	return memcmp(pLeft->data, pRight->data, pLeft->length) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqString::RawHash(const char* const string)
{
	assert(string != nullptr);

	const size_t length = strlen(string);

	return HqStd::Fnv2zHash(reinterpret_cast<const uint8_t*>(string), length);
}

//----------------------------------------------------------------------------------------------------------------------

char* HqString::RawFormatVarArgs(const char* const fmt, va_list vl)
{
	char* output = nullptr;

	if(fmt)
	{
		va_list vl2;

		// Make a copy of the variable args list since we need to go through them twice.
		va_copy(vl2, vl);

		// Determine the required length of the message string.
		const int messageLength = vsnprintf(nullptr, 0, fmt, vl2);

		// Discard empty messages.
		if(messageLength > 0)
		{
			// Allocate a message string to the required length, including space for the null terminator.
			char* const message = reinterpret_cast<char*>(HqMemAlloc(size_t(messageLength) + 1));

			// Write the message to the string.
			vsnprintf(message, size_t(messageLength) + 1, fmt, vl);

			output = message;
		}

		va_end(vl2);
	}

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

void HqString::ToSimpleLowerCase(HqString* const pString)
{
	_HqStringSimpleCaseOperation(pString, _HqCharToUtf32SimpleLower);
}

//----------------------------------------------------------------------------------------------------------------------

void HqString::ToSimpleUpperCase(HqString* const pString)
{
	_HqStringSimpleCaseOperation(pString, _HqCharToUtf32SimpleUpper);
}

//----------------------------------------------------------------------------------------------------------------------

void HqString::ToSimpleTitleCase(HqString* const pString)
{
	_HqStringSimpleCaseOperation(pString, _HqCharToUtf32SimpleTitle);
}

//----------------------------------------------------------------------------------------------------------------------

void HqString::ToStrictLowerCase(HqString* const pString)
{
	_HqStringStrictCaseOperation(pString, _HqCharToUtf32StrictLower);
}

//----------------------------------------------------------------------------------------------------------------------

void HqString::ToStrictUpperCase(HqString* const pString)
{
	_HqStringStrictCaseOperation(pString, _HqCharToUtf32StrictUpper);
}

//----------------------------------------------------------------------------------------------------------------------

void HqString::ToStrictTitleCase(HqString* const pString)
{
	_HqStringStrictCaseOperation(pString, _HqCharToUtf32StrictTitle);
}

//----------------------------------------------------------------------------------------------------------------------

void HqString::_onDestruct(void* const pOpaque)
{
	HqString* const pString = reinterpret_cast<HqString*>(pOpaque);

	if(pString->data)
	{
		HqMemFree(pString->data);
	}

	delete pString;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqString::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqString::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
