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

#include "Decoder.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonDisassemble
{
	XenonProgramHandle hProgram;
	XenonCallbackOpDisasm onDisasmFn;

	void* pUserData;

	uintptr_t opcodeOffset;

	XenonDecoder decoder;
};

//----------------------------------------------------------------------------------------------------------------------

#define XENON_DECLARE_OP_CODE_FN(op_name) \
	void OpCodeExec_ ## op_name(XenonExecutionHandle); \
	void OpCodeDisasm_ ## op_name(XenonDisassemble&)

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------

XENON_DECLARE_OP_CODE_FN(Nop);
XENON_DECLARE_OP_CODE_FN(Abort);
XENON_DECLARE_OP_CODE_FN(Return);
XENON_DECLARE_OP_CODE_FN(Yield);
XENON_DECLARE_OP_CODE_FN(Call);
XENON_DECLARE_OP_CODE_FN(Raise);

XENON_DECLARE_OP_CODE_FN(LoadConstant);
XENON_DECLARE_OP_CODE_FN(LoadGlobal);
XENON_DECLARE_OP_CODE_FN(LoadLocal);
XENON_DECLARE_OP_CODE_FN(LoadParam);
XENON_DECLARE_OP_CODE_FN(LoadObject);

XENON_DECLARE_OP_CODE_FN(StoreGlobal);
XENON_DECLARE_OP_CODE_FN(StoreLocal);
XENON_DECLARE_OP_CODE_FN(StoreParam);
XENON_DECLARE_OP_CODE_FN(StoreObject);

XENON_DECLARE_OP_CODE_FN(PullGlobal);
XENON_DECLARE_OP_CODE_FN(PullLocal);
XENON_DECLARE_OP_CODE_FN(PullParam);
XENON_DECLARE_OP_CODE_FN(PullObject);

XENON_DECLARE_OP_CODE_FN(Push);
XENON_DECLARE_OP_CODE_FN(Pop);

XENON_DECLARE_OP_CODE_FN(InitObject);

XENON_DECLARE_OP_CODE_FN(Branch);
XENON_DECLARE_OP_CODE_FN(BranchIfTrue);
XENON_DECLARE_OP_CODE_FN(BranchIfFalse);

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
