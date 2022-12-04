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

//----------------------------------------------------------------------------------------------------------------------

#define HQ_DECLARE_BUILT_IN(name) static void name(HqExecutionHandle, HqFunctionHandle, void*)

//----------------------------------------------------------------------------------------------------------------------

struct HQ_MAIN_API HqBuiltIn
{
	HQ_DECLARE_BUILT_IN(OpAddBool);
	HQ_DECLARE_BUILT_IN(OpAddInt8);
	HQ_DECLARE_BUILT_IN(OpAddInt16);
	HQ_DECLARE_BUILT_IN(OpAddInt32);
	HQ_DECLARE_BUILT_IN(OpAddInt64);
	HQ_DECLARE_BUILT_IN(OpAddUint8);
	HQ_DECLARE_BUILT_IN(OpAddUint16);
	HQ_DECLARE_BUILT_IN(OpAddUint32);
	HQ_DECLARE_BUILT_IN(OpAddUint64);
	HQ_DECLARE_BUILT_IN(OpAddFloat32);
	HQ_DECLARE_BUILT_IN(OpAddFloat64);
	HQ_DECLARE_BUILT_IN(OpAddString);

	HQ_DECLARE_BUILT_IN(OpSubBool);
	HQ_DECLARE_BUILT_IN(OpSubInt8);
	HQ_DECLARE_BUILT_IN(OpSubInt16);
	HQ_DECLARE_BUILT_IN(OpSubInt32);
	HQ_DECLARE_BUILT_IN(OpSubInt64);
	HQ_DECLARE_BUILT_IN(OpSubUint8);
	HQ_DECLARE_BUILT_IN(OpSubUint16);
	HQ_DECLARE_BUILT_IN(OpSubUint32);
	HQ_DECLARE_BUILT_IN(OpSubUint64);
	HQ_DECLARE_BUILT_IN(OpSubFloat32);
	HQ_DECLARE_BUILT_IN(OpSubFloat64);

	HQ_DECLARE_BUILT_IN(OpMulBool);
	HQ_DECLARE_BUILT_IN(OpMulInt8);
	HQ_DECLARE_BUILT_IN(OpMulInt16);
	HQ_DECLARE_BUILT_IN(OpMulInt32);
	HQ_DECLARE_BUILT_IN(OpMulInt64);
	HQ_DECLARE_BUILT_IN(OpMulUint8);
	HQ_DECLARE_BUILT_IN(OpMulUint16);
	HQ_DECLARE_BUILT_IN(OpMulUint32);
	HQ_DECLARE_BUILT_IN(OpMulUint64);
	HQ_DECLARE_BUILT_IN(OpMulFloat32);
	HQ_DECLARE_BUILT_IN(OpMulFloat64);

	HQ_DECLARE_BUILT_IN(OpDivBool);
	HQ_DECLARE_BUILT_IN(OpDivInt8);
	HQ_DECLARE_BUILT_IN(OpDivInt16);
	HQ_DECLARE_BUILT_IN(OpDivInt32);
	HQ_DECLARE_BUILT_IN(OpDivInt64);
	HQ_DECLARE_BUILT_IN(OpDivUint8);
	HQ_DECLARE_BUILT_IN(OpDivUint16);
	HQ_DECLARE_BUILT_IN(OpDivUint32);
	HQ_DECLARE_BUILT_IN(OpDivUint64);
	HQ_DECLARE_BUILT_IN(OpDivFloat32);
	HQ_DECLARE_BUILT_IN(OpDivFloat64);

	HQ_DECLARE_BUILT_IN(OpCastBoolToInt8);
	HQ_DECLARE_BUILT_IN(OpCastBoolToInt16);
	HQ_DECLARE_BUILT_IN(OpCastBoolToInt32);
	HQ_DECLARE_BUILT_IN(OpCastBoolToInt64);
	HQ_DECLARE_BUILT_IN(OpCastBoolToUint8);
	HQ_DECLARE_BUILT_IN(OpCastBoolToUint16);
	HQ_DECLARE_BUILT_IN(OpCastBoolToUint32);
	HQ_DECLARE_BUILT_IN(OpCastBoolToUint64);
	HQ_DECLARE_BUILT_IN(OpCastBoolToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastBoolToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastBoolToString);

	HQ_DECLARE_BUILT_IN(OpCastInt8ToBool);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastInt8ToString);

	HQ_DECLARE_BUILT_IN(OpCastInt16ToBool);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastInt16ToString);

	HQ_DECLARE_BUILT_IN(OpCastInt32ToBool);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastInt32ToString);

	HQ_DECLARE_BUILT_IN(OpCastInt64ToBool);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastInt64ToString);

	HQ_DECLARE_BUILT_IN(OpCastUint8ToBool);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastUint8ToString);

	HQ_DECLARE_BUILT_IN(OpCastUint16ToBool);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastUint16ToString);

	HQ_DECLARE_BUILT_IN(OpCastUint32ToBool);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastUint32ToString);

	HQ_DECLARE_BUILT_IN(OpCastUint64ToBool);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastUint64ToString);

	HQ_DECLARE_BUILT_IN(OpCastFloat32ToBool);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToFloat64);
	HQ_DECLARE_BUILT_IN(OpCastFloat32ToString);

	HQ_DECLARE_BUILT_IN(OpCastFloat64ToBool);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToInt8);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToInt16);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToInt32);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToInt64);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToUint8);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToUint16);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToUint32);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToUint64);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToFloat32);
	HQ_DECLARE_BUILT_IN(OpCastFloat64ToString);

	HQ_DECLARE_BUILT_IN(OpLenString);
	HQ_DECLARE_BUILT_IN(OpLenArray);
};

//----------------------------------------------------------------------------------------------------------------------

#undef HQ_DECLARE_BUILT_IN

//----------------------------------------------------------------------------------------------------------------------
