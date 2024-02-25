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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "Reference.hpp"

#include <stdarg.h>

//----------------------------------------------------------------------------------------------------------------------

struct HQ_BASE_API HqString
{
	struct HQ_BASE_API StlCompare
	{
		inline bool operator()(HqString* pLeft, HqString* pRight) { return FastCompare(pLeft, pRight); }
		inline bool operator()(const HqString* pLeft, const HqString* pRight) const { return FastCompare(pLeft, pRight); }
	};

	struct HQ_BASE_API StlHash
	{
		inline size_t operator()(HqString* pString) { return pString->hash; }
		inline size_t operator()(const HqString* pString) const { return pString->hash; }
	};

	static HqString* Create(const char* stringData);
	static HqString* CreateFmt(const char* fmt, ...);
	static int32_t AddRef(HqString* pString);
	static int32_t Release(HqString* pString);
	static int32_t SlowCompare(const HqString* pLeft, const HqString* pRight);
	static bool FastCompare(const HqString* pLeft, const HqString* pRight);

	static size_t RawHash(const char* string);

	static char* RawFormatVarArgs(const char* fmt, va_list vl);

	static void ToSimpleLowerCase(HqString* pString);
	static void ToSimpleUpperCase(HqString* pString);
	static void ToSimpleTitleCase(HqString* pString);

	static void ToStrictLowerCase(HqString* pString);
	static void ToStrictUpperCase(HqString* pString);
	static void ToStrictTitleCase(HqString* pString);

	static void _onDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqReference ref;

	size_t length;
	size_t hash;

	char* data;
};

//----------------------------------------------------------------------------------------------------------------------
