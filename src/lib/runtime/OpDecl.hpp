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
	HqProgramHandle hProgram;
	HqCallbackOpDisasm onDisasmFn;

	void* pUserData;

	uintptr_t opcodeOffset;

	HqDecoder decoder;
};

//----------------------------------------------------------------------------------------------------------------------

#define HQ_DECLARE_OP_CODE_FN(op_name) \
	void OpCodeExec_ ## op_name(HqExecutionHandle); \
	void OpCodeDisasm_ ## op_name(HqDisassemble&)

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------

HQ_DECLARE_OP_CODE_FN(Nop);
HQ_DECLARE_OP_CODE_FN(Abort);
HQ_DECLARE_OP_CODE_FN(Return);
HQ_DECLARE_OP_CODE_FN(Yield);
HQ_DECLARE_OP_CODE_FN(Call);
HQ_DECLARE_OP_CODE_FN(CallValue);
HQ_DECLARE_OP_CODE_FN(Raise);

HQ_DECLARE_OP_CODE_FN(LoadConst);
HQ_DECLARE_OP_CODE_FN(LoadConstNull);
HQ_DECLARE_OP_CODE_FN(LoadConstBool);
HQ_DECLARE_OP_CODE_FN(LoadConstI8);
HQ_DECLARE_OP_CODE_FN(LoadConstI16);
HQ_DECLARE_OP_CODE_FN(LoadConstI32);
HQ_DECLARE_OP_CODE_FN(LoadConstI64);
HQ_DECLARE_OP_CODE_FN(LoadConstU8);
HQ_DECLARE_OP_CODE_FN(LoadConstU16);
HQ_DECLARE_OP_CODE_FN(LoadConstU32);
HQ_DECLARE_OP_CODE_FN(LoadConstU64);
HQ_DECLARE_OP_CODE_FN(LoadConstF32);
HQ_DECLARE_OP_CODE_FN(LoadConstF64);
HQ_DECLARE_OP_CODE_FN(LoadGlobal);
HQ_DECLARE_OP_CODE_FN(LoadLocal);
HQ_DECLARE_OP_CODE_FN(LoadParam);
HQ_DECLARE_OP_CODE_FN(LoadObject);
HQ_DECLARE_OP_CODE_FN(LoadArray);

HQ_DECLARE_OP_CODE_FN(StoreGlobal);
HQ_DECLARE_OP_CODE_FN(StoreLocal);
HQ_DECLARE_OP_CODE_FN(StoreParam);
HQ_DECLARE_OP_CODE_FN(StoreObject);
HQ_DECLARE_OP_CODE_FN(StoreArray);

HQ_DECLARE_OP_CODE_FN(PullGlobal);
HQ_DECLARE_OP_CODE_FN(PullLocal);
HQ_DECLARE_OP_CODE_FN(PullParam);
HQ_DECLARE_OP_CODE_FN(PullObject);
HQ_DECLARE_OP_CODE_FN(PullArray);

HQ_DECLARE_OP_CODE_FN(Push);
HQ_DECLARE_OP_CODE_FN(Pop);

HQ_DECLARE_OP_CODE_FN(InitObject);
HQ_DECLARE_OP_CODE_FN(InitArray);
HQ_DECLARE_OP_CODE_FN(InitFunction);

HQ_DECLARE_OP_CODE_FN(Branch);
HQ_DECLARE_OP_CODE_FN(BranchIfTrue);
HQ_DECLARE_OP_CODE_FN(BranchIfFalse);

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
