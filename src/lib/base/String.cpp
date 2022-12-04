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

#include <assert.h>
#include <stdio.h>
#include <string.h>

#if defined(HQ_PLATFORM_PSVITA)
	// Work around xxhash using the wrong 'restrict' keyword.
	#pragma push_macro("restrict")
	#define restrict __restrict__
#endif

#define XXH_INLINE_ALL 1
#define XXH_ACCEPT_NULL_INPUT_POINTER 1
#include <xxhash.h>

#if defined(HQ_PLATFORM_PSVITA)
	#pragma pop_macro("restrict")
#endif

//----------------------------------------------------------------------------------------------------------------------

bool HqString::StlCompare::operator()(
	HqString* const pLeft,
	HqString* const pRight
)
{
	return Compare(pLeft, pRight);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqString::StlCompare::operator()(
	const HqString* const pLeft,
	const HqString* const pRight
) const
{
	return Compare(pLeft, pRight) ;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqString::StlLess::operator()(
	HqString* const pLeft,
	HqString* const pRight
)
{
	return Less(pLeft, pRight);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqString::StlLess::operator()(
	const HqString* const pLeft,
	const HqString* const pRight
) const
{
	return Less(pLeft, pRight) ;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqString::StlHash::operator()(HqString* const pObject)
{
	return pObject->hash;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqString::StlHash::operator()(const HqString* const pObject) const
{
	return pObject->hash;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqString::StlRawCompare::operator()(
	char* const strLeft,
	char* const strRight
)
{
	return RawCompare(strLeft, strRight);
}

//----------------------------------------------------------------------------------------------------------------------

bool HqString::StlRawCompare::operator()(
	const char* const strLeft,
	const char* const strRight
) const
{
	return RawCompare(strLeft, strRight) ;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqString::StlRawHash::operator()(char* const str)
{
	return RawHash(str);
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqString::StlRawHash::operator()(const char* const str) const
{
	return RawHash(str);
}

//----------------------------------------------------------------------------------------------------------------------

HqString* HqString::Create(const char* const stringData)
{
	// TODO: Implement string pooling.

	HqString* const pOutput = new HqString();
	assert(pOutput != nullptr);

	const size_t length = (stringData) ? strlen(stringData) : 0;

	pOutput->length = length;
	pOutput->hash = RawHash(stringData ? stringData : "");
	pOutput->data = (length > 0) ? reinterpret_cast<char*>(HqMemAlloc(length + 1)) : nullptr;

	HqReference::Initialize(pOutput->ref, prv_onDestruct, pOutput);

	if(stringData && pOutput->data)
	{
		memcpy(pOutput->data, stringData, length);
		pOutput->data[length] = '\0';
	}

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

bool HqString::Compare(const HqString* const pLeft, const HqString* const pRight)
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
	// and have same hash. In reality, this should only ever happen if identical string data exists in two
	// separate string objects, so it's not likely to happen much.
	return memcmp(pLeft->data, pRight->data, pLeft->length) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqString::Less(const HqString* const pLeft, const HqString* const pRight)
{
	assert(pLeft != nullptr);
	assert(pLeft->data != nullptr);

	assert(pRight != nullptr);
	assert(pRight->data != nullptr);

	const size_t minSize = (pLeft->length < pRight->length) ? pLeft->length : pRight->length;
	const int cmp = memcmp(pLeft->data, pRight->data, minSize);

	if(cmp != 0)
	{
		return cmp < 0;
	}

	return pLeft->length < pRight->length;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqString::RawCompare(const char* left, const char* right)
{
	assert(left != nullptr);
	assert(right != nullptr);

	if(left == right)
	{
		// Same string in memory.
		return true;
	}

	return strcmp(left, right) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

size_t HqString::RawHash(const char* const string)
{
	auto calculateFnv1aHash = [](const char* const string, const size_t length) -> size_t
	{
#ifdef HQ_DATA_WIDTH_64_BIT
		const uint64_t fnvOffsetBasis = 0xCBF29CE484222325ull;
		const uint64_t fnvPrime = 0x00000100000001B3ull;

		uint64_t output = fnvOffsetBasis;

#else
		const uint32_t fnvOffsetBasis = 0x811C9DC5ul;
		const uint32_t fnvPrime = 0x01000193ul;

		uint32_t output = fnvOffsetBasis;

#endif

		// Calculate the FNV1a hash from the input data.
		for(size_t i = 0; i < length; ++i)
		{
			output ^= string[i];
			output *= fnvPrime;
		}

		return size_t(output);
	};

	assert(string != nullptr);

	const size_t length = strlen(string);
	const size_t seed = calculateFnv1aHash(string, length);

	return size_t(
#ifdef HQ_DATA_WIDTH_64_BIT
		XXH64
#else
		XXH32
#endif
		(string, length, seed)
	);
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

void HqString::prv_onDestruct(void* const pOpaque)
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
