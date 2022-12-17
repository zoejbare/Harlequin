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

struct HqDecoder
{
	static void Initialize(HqDecoder& output, HqProgramHandle hProgram, uint32_t offset);

	static bool LoadBool(HqDecoder& decoder);

	static int8_t LoadInt8(HqDecoder& decoder);
	static int16_t LoadInt16(HqDecoder& decoder);
	static int32_t LoadInt32(HqDecoder& decoder);
	static int64_t LoadInt64(HqDecoder& decoder);

	static uint8_t LoadUint8(HqDecoder& decoder);
	static uint16_t LoadUint16(HqDecoder& decoder);
	static uint32_t LoadUint32(HqDecoder& decoder);
	static uint64_t LoadUint64(HqDecoder& decoder);

	static float LoadFloat32(HqDecoder& decoder);
	static double LoadFloat64(HqDecoder& decoder);

	static bool EndianSwapBool(HqDecoder& decoder);

	static int8_t EndianSwapInt8(HqDecoder& decoder);
	static int16_t EndianSwapInt16(HqDecoder& decoder);
	static int32_t EndianSwapInt32(HqDecoder& decoder);
	static int64_t EndianSwapInt64(HqDecoder& decoder);

	static uint8_t EndianSwapUint8(HqDecoder& decoder);
	static uint16_t EndianSwapUint16(HqDecoder& decoder);
	static uint32_t EndianSwapUint32(HqDecoder& decoder);
	static uint64_t EndianSwapUint64(HqDecoder& decoder);

	static float EndianSwapFloat32(HqDecoder& decoder);
	static double EndianSwapFloat64(HqDecoder& decoder);

	uint8_t* ip;
	uint8_t* cachedIp;
};

//----------------------------------------------------------------------------------------------------------------------
