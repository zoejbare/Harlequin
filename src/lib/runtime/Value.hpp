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
#include "../common/HashMap.hpp"
#include "../common/Stack.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqScriptObject;

struct HqValue
{
	typedef HqArray<HqValueHandle> HandleArray;
	typedef HqStack<HqValueHandle> HandleStack;

	typedef HqHashMap<
		HqString*,
		HqValueHandle,
		HqString::StlHash,
		HqString::StlCompare
	> StringToHandleMap;

	typedef HqHashMap<
		HqString*,
		bool,
		HqString::StlHash,
		HqString::StlCompare
	> StringToBoolMap;

	struct NativeWrapper
	{
		void* pObject;

		HqCallbackNativeValueCopy onCopy;
		HqCallbackNativeValueDestruct onDestruct;
	};

	struct GridWrapper
	{
		HandleArray array;

		size_t lengthX;
		size_t lengthY;
		size_t lengthZ;
	};

	static HqValueHandle CreateBool(HqVmHandle hVm, bool value);
	static HqValueHandle CreateInt8(HqVmHandle hVm, int8_t value);
	static HqValueHandle CreateInt16(HqVmHandle hVm, int16_t value);
	static HqValueHandle CreateInt32(HqVmHandle hVm, int32_t value);
	static HqValueHandle CreateInt64(HqVmHandle hVm, int64_t value);
	static HqValueHandle CreateUint8(HqVmHandle hVm, uint8_t value);
	static HqValueHandle CreateUint16(HqVmHandle hVm, uint16_t value);
	static HqValueHandle CreateUint32(HqVmHandle hVm, uint32_t value);
	static HqValueHandle CreateUint64(HqVmHandle hVm, uint64_t value);
	static HqValueHandle CreateFloat32(HqVmHandle hVm, float value);
	static HqValueHandle CreateFloat64(HqVmHandle hVm, double value);
	static HqValueHandle CreateString(HqVmHandle hVm, const char* string);
	static HqValueHandle CreateString(HqVmHandle hVm, HqString* pString);
	static HqValueHandle CreateObject(HqVmHandle hVm, HqScriptObject* pObjectSchema);
	static HqValueHandle CreateFunction(HqVmHandle hVm, HqFunctionHandle hFunction);
	static HqValueHandle CreateArray(HqVmHandle hVm, size_t count);
	static HqValueHandle CreateGrid(HqVmHandle hVm, size_t lengthX, size_t lengthY, size_t lengthZ);
	static HqValueHandle CreateNative(
		HqVmHandle hVm,
		void* pNativeObject,
		HqCallbackNativeValueCopy onCopy,
		HqCallbackNativeValueDestruct onDestruct
	);
	static HqValueHandle Copy(HqVmHandle hVm, HqValueHandle hValue);

	static HqString* GetDebugString(HqValueHandle hValue);

	static void SetAutoMark(HqValueHandle hValue, bool autoMark);

	static bool EvaluateAsBoolean(HqValueHandle hValue);

	static size_t CalculateGridIndex(
		size_t lengthX,
		size_t lengthY,
		size_t indexX,
		size_t indexY,
		size_t indexZ);

	static HqValue* _onCreate(int, HqVmHandle);
	static void _onGcDiscovery(HqGarbageCollector&, void*);
	static void _onGcDestruct(void*);

	void* operator new(size_t sizeInBytes);
	void operator delete(void* pObject);

	HqVmHandle hVm;

	HqGcProxy gcProxy;

	union
	{
		NativeWrapper native;
		GridWrapper grid;

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

//----------------------------------------------------------------------------------------------------------------------

inline size_t HqValue::CalculateGridIndex(
	const size_t lengthX,
	const size_t lengthY,
	const size_t indexX,
	const size_t indexY,
	const size_t indexZ)
{
	return (indexZ * lengthX * lengthY) + (indexY * lengthX) + indexX;
}

//----------------------------------------------------------------------------------------------------------------------