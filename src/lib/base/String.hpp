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

//----------------------------------------------------------------------------------------------------------------------

struct XENON_BASE_API XenonString
{
	struct XENON_BASE_API StlCompare
	{
		bool operator()(XenonString* const pLeft, XenonString* const pRight);
		bool operator()(const XenonString* const pLeft, const XenonString* const pRight) const;
	};

	struct XENON_BASE_API StlLess
	{
		bool operator()(XenonString* const pLeft, XenonString* const pRight);
		bool operator()(const XenonString* const pLeft, const XenonString* const pRight) const;
	};

	struct XENON_BASE_API StlHash
	{
		size_t operator()(XenonString* const pString);
		size_t operator()(const XenonString* const pString) const;
	};

	struct XENON_BASE_API StlRawCompare
	{
		bool operator()(char* const pLeft, char* const pRight);
		bool operator()(const char* const pLeft, const char* const pRight) const;
	};

	struct XENON_BASE_API StlRawHash
	{
		size_t operator()(char* const pString);
		size_t operator()(const char* const pString) const;
	};

	static XenonString* Create(const char* const stringData);
	static int32_t AddRef(XenonString* const pString);
	static int32_t Release(XenonString* const pString);
	static bool Compare(const XenonString* const pLeft, const XenonString* const pRight);
	static bool Less(const XenonString* const pLeft, const XenonString* const pRight);

	static bool RawCompare(const char* const left, const char* right);
	static size_t RawHash(const char* const string);

	static const char* RawFormatVarArgs(const char* const fmt, va_list vl);

	static void prv_onDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonReference ref;

	size_t length;
	size_t hash;

	char* data;
};

//----------------------------------------------------------------------------------------------------------------------
