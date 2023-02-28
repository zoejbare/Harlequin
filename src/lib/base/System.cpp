//
// Copyright (c) 2023, Zoe J. Bare
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

#include "System.hpp"

#include "unicode/utf32-record-table.h"

#include <assert.h>
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

extern "C" size_t _HqSysUtf32ToUtf8(char* const pOutMbSeq, const char32_t codePoint)
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

extern "C" char32_t _HqSysUtf8ToUtf32(size_t* const pOutMbLen, const char* const mbSeq)
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

char32_t _HqSysToUtf32SimpleLower(const char32_t codePoint)
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

char32_t _HqSysToUtf32SimpleUpper(const char32_t codePoint)
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

char32_t _HqSysToUtf32SimpleTitle(const char32_t codePoint)
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

size_t _HqSysToUtf32FullLower(char32_t* const pOutCodePoints, const char32_t codePoint)
{
	assert(pOutCodePoints != nullptr);

	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT) 
		? utf32RecordTable.indices[codePoint] 
		: 0;

	if(recordIndex > 0)
	{
		const HqUtf32Mapping& mapping = utf32RecordTable.records[recordIndex].lowerCase;

		memcpy(pOutCodePoints, mapping.full, sizeof(char32_t) * mapping.fullLength);

		return mapping.fullLength;
	}

	// The input code point is out of the supported range or has no case mapping data.
	(*pOutCodePoints) = codePoint;
	return 1;
}

//----------------------------------------------------------------------------------------------------------------------

size_t _HqSysToUtf32FullUpper(char32_t* const pOutCodePoints, const char32_t codePoint)
{
	assert(pOutCodePoints != nullptr);

	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT) 
		? utf32RecordTable.indices[codePoint] 
		: 0;

	if(recordIndex > 0)
	{
		const HqUtf32Mapping& mapping = utf32RecordTable.records[recordIndex].upperCase;

		memcpy(pOutCodePoints, mapping.full, sizeof(char32_t) * mapping.fullLength);

		return mapping.fullLength;
	}

	// The input code point is out of the supported range or has no case mapping data.
	(*pOutCodePoints) = codePoint;
	return 1;
}

//----------------------------------------------------------------------------------------------------------------------

size_t _HqSysToUtf32FullTitle(char32_t* const pOutCodePoints, const char32_t codePoint)
{
	assert(pOutCodePoints != nullptr);

	const HQ_UTF32_RECORD_INDEX_TYPE recordIndex = (codePoint < HQ_UTF32_RECORD_INDEX_COUNT) 
		? utf32RecordTable.indices[codePoint] 
		: 0;

	if(recordIndex > 0)
	{
		const HqUtf32Mapping& mapping = utf32RecordTable.records[recordIndex].titleCase;

		memcpy(pOutCodePoints, mapping.full, sizeof(char32_t) * mapping.fullLength);

		return mapping.fullLength;
	}

	// The input code point is out of the supported range or has no case mapping data.
	(*pOutCodePoints) = codePoint;
	return 1;
}

//----------------------------------------------------------------------------------------------------------------------
