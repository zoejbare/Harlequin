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

#include "Vm.hpp"
#include "BuiltInDecl.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

void HqVm::prv_setupBuiltIns(HqVmHandle hVm)
{
	#define HQ_BUILT_IN(id, func, numParams, numRetVals) \
		{ \
			const char* const signature = HqGetBuiltInFunctionSignature(HQ_BUILT_IN_ ## id); \
			assert(signature != nullptr); \
			HqString* const pSignature = HqString::Create(signature); \
			HqMemFree((void*)(signature)); \
			assert(pSignature != nullptr); \
			assert(!HQ_MAP_FUNC_CONTAINS(hVm->functions, pSignature)); \
			HqFunctionHandle hFunction = HqFunction::CreateBuiltIn(pSignature, HqBuiltIn::func, numParams, numRetVals); \
			HQ_MAP_FUNC_INSERT(hVm->functions, pSignature, hFunction); \
		}

	HQ_BUILT_IN(OP_ADD_BOOL,    OpAddBool,    2, 1);
	HQ_BUILT_IN(OP_ADD_INT8,    OpAddInt8,    2, 1);
	HQ_BUILT_IN(OP_ADD_INT16,   OpAddInt16,   2, 1);
	HQ_BUILT_IN(OP_ADD_INT32,   OpAddInt32,   2, 1);
	HQ_BUILT_IN(OP_ADD_INT64,   OpAddInt64,   2, 1);
	HQ_BUILT_IN(OP_ADD_UINT8,   OpAddUint8,   2, 1);
	HQ_BUILT_IN(OP_ADD_UINT16,  OpAddUint16,  2, 1);
	HQ_BUILT_IN(OP_ADD_UINT32,  OpAddUint32,  2, 1);
	HQ_BUILT_IN(OP_ADD_UINT64,  OpAddUint64,  2, 1);
	HQ_BUILT_IN(OP_ADD_FLOAT32, OpAddFloat32, 2, 1);
	HQ_BUILT_IN(OP_ADD_FLOAT64, OpAddFloat64, 2, 1);
	HQ_BUILT_IN(OP_ADD_STRING,  OpAddString,  2, 1);

	HQ_BUILT_IN(OP_SUB_BOOL,    OpSubBool,    2, 1);
	HQ_BUILT_IN(OP_SUB_INT8,    OpSubInt8,    2, 1);
	HQ_BUILT_IN(OP_SUB_INT16,   OpSubInt16,   2, 1);
	HQ_BUILT_IN(OP_SUB_INT32,   OpSubInt32,   2, 1);
	HQ_BUILT_IN(OP_SUB_INT64,   OpSubInt64,   2, 1);
	HQ_BUILT_IN(OP_SUB_UINT8,   OpSubUint8,   2, 1);
	HQ_BUILT_IN(OP_SUB_UINT16,  OpSubUint16,  2, 1);
	HQ_BUILT_IN(OP_SUB_UINT32,  OpSubUint32,  2, 1);
	HQ_BUILT_IN(OP_SUB_UINT64,  OpSubUint64,  2, 1);
	HQ_BUILT_IN(OP_SUB_FLOAT32, OpSubFloat32, 2, 1);
	HQ_BUILT_IN(OP_SUB_FLOAT64, OpSubFloat64, 2, 1);

	HQ_BUILT_IN(OP_MUL_BOOL,    OpMulBool,    2, 1);
	HQ_BUILT_IN(OP_MUL_INT8,    OpMulInt8,    2, 1);
	HQ_BUILT_IN(OP_MUL_INT16,   OpMulInt16,   2, 1);
	HQ_BUILT_IN(OP_MUL_INT32,   OpMulInt32,   2, 1);
	HQ_BUILT_IN(OP_MUL_INT64,   OpMulInt64,   2, 1);
	HQ_BUILT_IN(OP_MUL_UINT8,   OpMulUint8,   2, 1);
	HQ_BUILT_IN(OP_MUL_UINT16,  OpMulUint16,  2, 1);
	HQ_BUILT_IN(OP_MUL_UINT32,  OpMulUint32,  2, 1);
	HQ_BUILT_IN(OP_MUL_UINT64,  OpMulUint64,  2, 1);
	HQ_BUILT_IN(OP_MUL_FLOAT32, OpMulFloat32, 2, 1);
	HQ_BUILT_IN(OP_MUL_FLOAT64, OpMulFloat64, 2, 1);

	HQ_BUILT_IN(OP_DIV_BOOL,    OpDivBool,    2, 1);
	HQ_BUILT_IN(OP_DIV_INT8,    OpDivInt8,    2, 1);
	HQ_BUILT_IN(OP_DIV_INT16,   OpDivInt16,   2, 1);
	HQ_BUILT_IN(OP_DIV_INT32,   OpDivInt32,   2, 1);
	HQ_BUILT_IN(OP_DIV_INT64,   OpDivInt64,   2, 1);
	HQ_BUILT_IN(OP_DIV_UINT8,   OpDivUint8,   2, 1);
	HQ_BUILT_IN(OP_DIV_UINT16,  OpDivUint16,  2, 1);
	HQ_BUILT_IN(OP_DIV_UINT32,  OpDivUint32,  2, 1);
	HQ_BUILT_IN(OP_DIV_UINT64,  OpDivUint64,  2, 1);
	HQ_BUILT_IN(OP_DIV_FLOAT32, OpDivFloat32, 2, 1);
	HQ_BUILT_IN(OP_DIV_FLOAT64, OpDivFloat64, 2, 1);

	HQ_BUILT_IN(OP_CAST_BOOL_TO_INT8,    OpCastBoolToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_INT16,   OpCastBoolToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_INT32,   OpCastBoolToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_INT64,   OpCastBoolToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_UINT8,   OpCastBoolToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_UINT16,  OpCastBoolToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_UINT32,  OpCastBoolToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_UINT64,  OpCastBoolToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_FLOAT32, OpCastBoolToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_FLOAT64, OpCastBoolToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_BOOL_TO_STRING,  OpCastBoolToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_INT8_TO_BOOL,    OpCastInt8ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_INT16,   OpCastInt8ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_INT32,   OpCastInt8ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_INT64,   OpCastInt8ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_UINT8,   OpCastInt8ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_UINT16,  OpCastInt8ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_UINT32,  OpCastInt8ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_UINT64,  OpCastInt8ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_FLOAT32, OpCastInt8ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_FLOAT64, OpCastInt8ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_INT8_TO_STRING,  OpCastInt8ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_INT16_TO_BOOL,    OpCastInt16ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_INT8,    OpCastInt16ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_INT32,   OpCastInt16ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_INT64,   OpCastInt16ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_UINT8,   OpCastInt16ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_UINT16,  OpCastInt16ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_UINT32,  OpCastInt16ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_UINT64,  OpCastInt16ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_FLOAT32, OpCastInt16ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_FLOAT64, OpCastInt16ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_INT16_TO_STRING,  OpCastInt16ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_INT32_TO_BOOL,    OpCastInt32ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_INT8,    OpCastInt32ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_INT16,   OpCastInt32ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_INT64,   OpCastInt32ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_UINT8,   OpCastInt32ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_UINT16,  OpCastInt32ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_UINT32,  OpCastInt32ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_UINT64,  OpCastInt32ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_FLOAT32, OpCastInt32ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_FLOAT64, OpCastInt32ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_INT32_TO_STRING,  OpCastInt32ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_INT64_TO_BOOL,    OpCastInt64ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_INT8,    OpCastInt64ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_INT16,   OpCastInt64ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_INT32,   OpCastInt64ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_UINT8,   OpCastInt64ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_UINT16,  OpCastInt64ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_UINT32,  OpCastInt64ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_UINT64,  OpCastInt64ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_FLOAT32, OpCastInt64ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_FLOAT64, OpCastInt64ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_INT64_TO_STRING,  OpCastInt64ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_UINT8_TO_BOOL,    OpCastUint8ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_INT8,    OpCastUint8ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_INT16,   OpCastUint8ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_INT32,   OpCastUint8ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_INT64,   OpCastUint8ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_UINT16,  OpCastUint8ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_UINT32,  OpCastUint8ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_UINT64,  OpCastUint8ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_FLOAT32, OpCastUint8ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_FLOAT64, OpCastUint8ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_UINT8_TO_STRING,  OpCastUint8ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_UINT16_TO_BOOL,    OpCastUint16ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_INT8,    OpCastUint16ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_INT16,   OpCastUint16ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_INT32,   OpCastUint16ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_INT64,   OpCastUint16ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_UINT8,   OpCastUint16ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_UINT32,  OpCastUint16ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_UINT64,  OpCastUint16ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_FLOAT32, OpCastUint16ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_FLOAT64, OpCastUint16ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_UINT16_TO_STRING,  OpCastUint16ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_UINT32_TO_BOOL,    OpCastUint32ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_INT8,    OpCastUint32ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_INT16,   OpCastUint32ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_INT32,   OpCastUint32ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_INT64,   OpCastUint32ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_UINT8,   OpCastUint32ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_UINT16,  OpCastUint32ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_UINT64,  OpCastUint32ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_FLOAT32, OpCastUint32ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_FLOAT64, OpCastUint32ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_UINT32_TO_STRING,  OpCastUint32ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_UINT64_TO_BOOL,    OpCastUint64ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_INT8,    OpCastUint64ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_INT16,   OpCastUint64ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_INT32,   OpCastUint64ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_INT64,   OpCastUint64ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_UINT8,   OpCastUint64ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_UINT16,  OpCastUint64ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_UINT32,  OpCastUint64ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_FLOAT32, OpCastUint64ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_FLOAT64, OpCastUint64ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_UINT64_TO_STRING,  OpCastUint64ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_BOOL,    OpCastFloat32ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_INT8,    OpCastFloat32ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_INT16,   OpCastFloat32ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_INT32,   OpCastFloat32ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_INT64,   OpCastFloat32ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_UINT8,   OpCastFloat32ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_UINT16,  OpCastFloat32ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_UINT32,  OpCastFloat32ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_UINT64,  OpCastFloat32ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_FLOAT64, OpCastFloat32ToFloat64, 1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT32_TO_STRING,  OpCastFloat32ToString,  1, 1);

	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_BOOL,    OpCastFloat64ToBool,    1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_INT8,    OpCastFloat64ToInt8,    1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_INT16,   OpCastFloat64ToInt16,   1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_INT32,   OpCastFloat64ToInt32,   1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_INT64,   OpCastFloat64ToInt64,   1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_UINT8,   OpCastFloat64ToUint8,   1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_UINT16,  OpCastFloat64ToUint16,  1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_UINT32,  OpCastFloat64ToUint32,  1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_UINT64,  OpCastFloat64ToUint64,  1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_FLOAT32, OpCastFloat64ToFloat32, 1, 1);
	HQ_BUILT_IN(OP_CAST_FLOAT64_TO_STRING,  OpCastFloat64ToString,  1, 1);

	HQ_BUILT_IN(OP_LEN_STRING, OpLenString, 1, 1);
	HQ_BUILT_IN(OP_LEN_ARRAY, OpLenArray, 1, 1);

	#undef HQ_BUILT_IN
}

//----------------------------------------------------------------------------------------------------------------------
