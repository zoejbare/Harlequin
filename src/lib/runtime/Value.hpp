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

#include "../XenonScript.h"

#include "../base/String.hpp"
#include "../base/Reference.hpp"

#include "../common/Array.hpp"
#include "../common/Stack.hpp"

#include <SkipProbe/SkipProbe.hpp>

#include <string>

//----------------------------------------------------------------------------------------------------------------------

// TODO: Implement support for script objects.
struct XenonObject;

struct XENON_BASE_API XenonValue
{
	typedef XenonArray<XenonValueHandle> HandleArray;
	typedef XenonStack<XenonValueHandle> HandleStack;

	typedef SkipProbe::HashMap<
		XenonString*,
		XenonValueHandle,
		XenonString::StlHash,
		XenonString::StlCompare
	> StringToHandleMap;

	typedef SkipProbe::HashMap<
		XenonString*,
		bool,
		XenonString::StlHash,
		XenonString::StlCompare
	> StringToBoolMap;

	static XenonValue NullValue;

	static XenonValueHandle CreateBool(const bool value);
	static XenonValueHandle CreateInt8(const int8_t value);
	static XenonValueHandle CreateInt16(const int16_t value);
	static XenonValueHandle CreateInt32(const int32_t value);
	static XenonValueHandle CreateInt64(const int64_t value);
	static XenonValueHandle CreateUint8(const uint8_t value);
	static XenonValueHandle CreateUint16(const uint16_t value);
	static XenonValueHandle CreateUint32(const uint32_t value);
	static XenonValueHandle CreateUint64(const uint64_t value);
	static XenonValueHandle CreateFloat32(const float value);
	static XenonValueHandle CreateFloat64(const double value);
	static XenonValueHandle CreateNull();
	static XenonValueHandle CreateString(const char* const string);
	static XenonValueHandle CreateString(XenonString* const pString);
	static XenonValueHandle Copy(XenonValueHandle hValue);

	static int32_t AddRef(XenonValueHandle hValue);
	static int32_t Release(XenonValueHandle hValue);

	static std::string GetDebugString(XenonValueHandle hValue);

	static XenonValue* prv_onCreate(int);
	static void prv_onDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonReference ref;

	union
	{
		XenonString* pString;
		XenonObject* pObject;

		double float64;
		uint64_t uint64;
		int64_t int64;

		float float32;
		uint32_t uint32;
		int32_t int32;

		uint16_t uint16;
		int16_t int16;

		uint8_t uint8;
		int8_t int8;

		bool boolean;
	} as;

	int type;
};
