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

#include "../Harlequin.h"

#include "GcProxy.hpp"

#include "../base/String.hpp"

#include "../common/Array.hpp"
#include "../common/Map.hpp"
#include "../common/Stack.hpp"
#include "../common/StlAllocator.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqNativeValueWrapper
{
	void* pObject;

	HqCallbackNativeValueCopy onCopy;
	HqCallbackNativeValueDestruct onDestruct;
	HqCallbackNativeValueEqual onTestEqual;
	HqCallbackNativeValueLessThan onTestLessThan;
};

//----------------------------------------------------------------------------------------------------------------------

struct HqScriptObject;

struct HqValue
{
	typedef HqArray<HqValueHandle> HandleArray;
	typedef HqStack<HqValueHandle> HandleStack;

	typedef HQ_MAP_TYPE<
		HqString*,
		HqValueHandle,
#if HQ_MAP_IS_UNORDERED
		HqString::StlHash,
		HqString::StlCompare,
#else
		HqString::StlLess,
#endif
		HqStlAllocator<HQ_MAP_NODE_TYPE(HqString*, HqValueHandle)>
	> StringToHandleMap;

	typedef HQ_MAP_TYPE<
		HqString*,
		bool,
#if HQ_MAP_IS_UNORDERED
		HqString::StlHash,
		HqString::StlCompare,
#else
		HqString::StlLess,
#endif
		HqStlAllocator<HQ_MAP_NODE_TYPE(HqString*, bool)>
	> StringToBoolMap;

	static HqValue NullValue;

	static HqValueHandle CreateBool(HqVmHandle hVm, const bool value);
	static HqValueHandle CreateInt8(HqVmHandle hVm, const int8_t value);
	static HqValueHandle CreateInt16(HqVmHandle hVm, const int16_t value);
	static HqValueHandle CreateInt32(HqVmHandle hVm, const int32_t value);
	static HqValueHandle CreateInt64(HqVmHandle hVm, const int64_t value);
	static HqValueHandle CreateUint8(HqVmHandle hVm, const uint8_t value);
	static HqValueHandle CreateUint16(HqVmHandle hVm, const uint16_t value);
	static HqValueHandle CreateUint32(HqVmHandle hVm, const uint32_t value);
	static HqValueHandle CreateUint64(HqVmHandle hVm, const uint64_t value);
	static HqValueHandle CreateFloat32(HqVmHandle hVm, const float value);
	static HqValueHandle CreateFloat64(HqVmHandle hVm, const double value);
	static HqValueHandle CreateNull();
	static HqValueHandle CreateString(HqVmHandle hVm, const char* const string);
	static HqValueHandle CreateString(HqVmHandle hVm, HqString* const pString);
	static HqValueHandle CreateObject(HqVmHandle hVm, HqScriptObject* const pObjectSchema);
	static HqValueHandle CreateFunction(HqVmHandle hVm, HqFunctionHandle hFunction);
	static HqValueHandle CreateArray(HqVmHandle hVm, const size_t count);
	static HqValueHandle CreateNative(
		HqVmHandle hVm,
		void* const pNativeObject,
		HqCallbackNativeValueCopy onCopy,
		HqCallbackNativeValueDestruct onDestruct,
		HqCallbackNativeValueEqual onTestEqual,
		HqCallbackNativeValueLessThan onTestLessThan
	);
	static HqValueHandle Copy(HqVmHandle hVm, HqValueHandle hValue);

	static HqString* GetDebugString(HqValueHandle hValue);

	static bool CanBeMarked(HqValueHandle hValue);
	static void SetAutoMark(HqValueHandle hValue, const bool autoMark);

	static HqValue* prv_onCreate(int, HqVmHandle);
	static void prv_onGcDiscovery(HqGarbageCollector&, void*);
	static void prv_onGcDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqVmHandle hVm;

	HqGcProxy gcProxy;

	union
	{
		HqNativeValueWrapper native;
		HandleArray array;

		HqString* pString;
		HqScriptObject* pObject;

		HqFunctionHandle hFunction;

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
