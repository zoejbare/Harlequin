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

#include "GcProxy.hpp"

#include "../base/String.hpp"

#include "../common/Array.hpp"
#include "../common/Map.hpp"
#include "../common/Stack.hpp"
#include "../common/StlAllocator.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonObject;

struct XenonValue
{
	typedef XenonArray<XenonValueHandle> HandleArray;
	typedef XenonStack<XenonValueHandle> HandleStack;

	typedef XENON_MAP_TYPE<
		XenonString*,
		XenonValueHandle,
#if XENON_MAP_IS_UNORDERED
		XenonString::StlHash,
		XenonString::StlCompare,
#else
		XenonString::StlLess,
#endif
		XenonStlAllocator<XENON_MAP_NODE_TYPE(XenonString*, XenonValueHandle)>
	> StringToHandleMap;

	typedef XENON_MAP_TYPE<
		XenonString*,
		bool,
#if XENON_MAP_IS_UNORDERED
		XenonString::StlHash,
		XenonString::StlCompare,
#else
		XenonString::StlLess,
#endif
		XenonStlAllocator<XENON_MAP_NODE_TYPE(XenonString*, bool)>
	> StringToBoolMap;

	static XenonValue NullValue;

	static XenonValueHandle CreateBool(XenonVmHandle hVm, const bool value);
	static XenonValueHandle CreateInt8(XenonVmHandle hVm, const int8_t value);
	static XenonValueHandle CreateInt16(XenonVmHandle hVm, const int16_t value);
	static XenonValueHandle CreateInt32(XenonVmHandle hVm, const int32_t value);
	static XenonValueHandle CreateInt64(XenonVmHandle hVm, const int64_t value);
	static XenonValueHandle CreateUint8(XenonVmHandle hVm, const uint8_t value);
	static XenonValueHandle CreateUint16(XenonVmHandle hVm, const uint16_t value);
	static XenonValueHandle CreateUint32(XenonVmHandle hVm, const uint32_t value);
	static XenonValueHandle CreateUint64(XenonVmHandle hVm, const uint64_t value);
	static XenonValueHandle CreateFloat32(XenonVmHandle hVm, const float value);
	static XenonValueHandle CreateFloat64(XenonVmHandle hVm, const double value);
	static XenonValueHandle CreateNull();
	static XenonValueHandle CreateString(XenonVmHandle hVm, const char* const string);
	static XenonValueHandle CreateString(XenonVmHandle hVm, XenonString* const pString);
	static XenonValueHandle CreateObject(XenonVmHandle hVm, XenonObject* const pObjectSchema);
	static XenonValueHandle Copy(XenonVmHandle hVm, XenonValueHandle hValue);

	static XenonString* GetDebugString(XenonValueHandle hValue);

	static bool CanBeMarked(XenonValueHandle hValue);
	static void SetAutoMark(XenonValueHandle hValue, const bool autoMark);

	static XenonValue* prv_onCreate(int, XenonVmHandle);
	static void prv_onGcDiscovery(XenonGarbageCollector&, void*);
	static void prv_onGcDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonVmHandle hVm;

	XenonGcProxy gcProxy;

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
