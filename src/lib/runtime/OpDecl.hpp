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

#include "Decoder.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqDisassemble
{
	HqModuleHandle hModule;
	HqCallbackOpDisasm onDisasmFn;

	void* pUserData;

	uintptr_t opcodeOffset;

	HqDecoder decoder;
};

//----------------------------------------------------------------------------------------------------------------------

#define HQ_DECLARE_OP_CODE_FN(op_name) \
	void OpCodeExec_ ## op_name(HqExecutionHandle); \
	void OpCodeDisasm_ ## op_name(HqDisassemble&); \
	void OpCodeEndian_ ## op_name(HqDecoder&)

//----------------------------------------------------------------------------------------------------------------------

extern "C" {

//----------------------------------------------------------------------------------------------------------------------

HQ_DECLARE_OP_CODE_FN(Nop);
HQ_DECLARE_OP_CODE_FN(Abort);
HQ_DECLARE_OP_CODE_FN(Return);
HQ_DECLARE_OP_CODE_FN(Yield);

HQ_DECLARE_OP_CODE_FN(Call);
HQ_DECLARE_OP_CODE_FN(CallValue);
HQ_DECLARE_OP_CODE_FN(Raise);

HQ_DECLARE_OP_CODE_FN(LoadImmNull);
HQ_DECLARE_OP_CODE_FN(LoadImmBool);
HQ_DECLARE_OP_CODE_FN(LoadImmI8);
HQ_DECLARE_OP_CODE_FN(LoadImmI16);
HQ_DECLARE_OP_CODE_FN(LoadImmI32);
HQ_DECLARE_OP_CODE_FN(LoadImmI64);
HQ_DECLARE_OP_CODE_FN(LoadImmU8);
HQ_DECLARE_OP_CODE_FN(LoadImmU16);
HQ_DECLARE_OP_CODE_FN(LoadImmU32);
HQ_DECLARE_OP_CODE_FN(LoadImmU64);
HQ_DECLARE_OP_CODE_FN(LoadImmF32);
HQ_DECLARE_OP_CODE_FN(LoadImmF64);
HQ_DECLARE_OP_CODE_FN(LoadImmStr);

HQ_DECLARE_OP_CODE_FN(LoadGlobal);
HQ_DECLARE_OP_CODE_FN(LoadParam);
HQ_DECLARE_OP_CODE_FN(LoadObject);
HQ_DECLARE_OP_CODE_FN(LoadArray);

HQ_DECLARE_OP_CODE_FN(StoreGlobal);
HQ_DECLARE_OP_CODE_FN(StoreParam);
HQ_DECLARE_OP_CODE_FN(StoreObject);
HQ_DECLARE_OP_CODE_FN(StoreArray);

HQ_DECLARE_OP_CODE_FN(PullGlobal);
HQ_DECLARE_OP_CODE_FN(PullParam);
HQ_DECLARE_OP_CODE_FN(PullObject);
HQ_DECLARE_OP_CODE_FN(PullArray);

HQ_DECLARE_OP_CODE_FN(Push);
HQ_DECLARE_OP_CODE_FN(Pop);

HQ_DECLARE_OP_CODE_FN(InitObject);
HQ_DECLARE_OP_CODE_FN(InitArray);
HQ_DECLARE_OP_CODE_FN(InitFunction);

HQ_DECLARE_OP_CODE_FN(Jump);
HQ_DECLARE_OP_CODE_FN(JumpIfTrue);
HQ_DECLARE_OP_CODE_FN(JumpIfFalse);

HQ_DECLARE_OP_CODE_FN(Length);

HQ_DECLARE_OP_CODE_FN(Add);
HQ_DECLARE_OP_CODE_FN(Sub);
HQ_DECLARE_OP_CODE_FN(Mul);
HQ_DECLARE_OP_CODE_FN(Div);
HQ_DECLARE_OP_CODE_FN(Mod);
HQ_DECLARE_OP_CODE_FN(Exp);

HQ_DECLARE_OP_CODE_FN(BitAnd);
HQ_DECLARE_OP_CODE_FN(BitOr);
HQ_DECLARE_OP_CODE_FN(BitXor);
HQ_DECLARE_OP_CODE_FN(LeftShift);
HQ_DECLARE_OP_CODE_FN(RightShift);
HQ_DECLARE_OP_CODE_FN(LeftRotate);
HQ_DECLARE_OP_CODE_FN(RightRotate);

HQ_DECLARE_OP_CODE_FN(CastInt8);
HQ_DECLARE_OP_CODE_FN(CastInt16);
HQ_DECLARE_OP_CODE_FN(CastInt32);
HQ_DECLARE_OP_CODE_FN(CastInt64);
HQ_DECLARE_OP_CODE_FN(CastUint8);
HQ_DECLARE_OP_CODE_FN(CastUint16);
HQ_DECLARE_OP_CODE_FN(CastUint32);
HQ_DECLARE_OP_CODE_FN(CastUint64);
HQ_DECLARE_OP_CODE_FN(CastFloat32);
HQ_DECLARE_OP_CODE_FN(CastFloat64);
HQ_DECLARE_OP_CODE_FN(CastBool);
HQ_DECLARE_OP_CODE_FN(CastString);

HQ_DECLARE_OP_CODE_FN(Compare);
HQ_DECLARE_OP_CODE_FN(Test);
HQ_DECLARE_OP_CODE_FN(Move);
HQ_DECLARE_OP_CODE_FN(Copy);

//----------------------------------------------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------------------------------------------
