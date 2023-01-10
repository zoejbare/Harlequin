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
#include "OpDecl.hpp"

#include "../common/OpCodeEnum.hpp"

//----------------------------------------------------------------------------------------------------------------------

void HqVm::prv_setupOpCodes(HqVmHandle hVm)
{
	#define _HQ_BIND_OP_CODE(op_code, name) \
		hVm->opCodes.pData[HQ_OP_CODE_ ## op_code].execFn = OpCodeExec_ ## name; \
		hVm->opCodes.pData[HQ_OP_CODE_ ## op_code].disasmFn = OpCodeDisasm_ ## name; \
		hVm->opCodes.pData[HQ_OP_CODE_ ## op_code].endianFn = OpCodeEndian_ ## name

	_HQ_BIND_OP_CODE(NOP,    Nop);
	_HQ_BIND_OP_CODE(ABORT,  Abort);
	_HQ_BIND_OP_CODE(RETURN, Return);
	_HQ_BIND_OP_CODE(YIELD,  Yield);

	_HQ_BIND_OP_CODE(CALL,       Call);
	_HQ_BIND_OP_CODE(CALL_VALUE, CallValue);
	_HQ_BIND_OP_CODE(RAISE,      Raise);

	_HQ_BIND_OP_CODE(LOAD_IMM_NULL, LoadImmNull);
	_HQ_BIND_OP_CODE(LOAD_IMM_BOOL, LoadImmBool);
	_HQ_BIND_OP_CODE(LOAD_IMM_I8,   LoadImmI8);
	_HQ_BIND_OP_CODE(LOAD_IMM_I16,  LoadImmI16);
	_HQ_BIND_OP_CODE(LOAD_IMM_I32,  LoadImmI32);
	_HQ_BIND_OP_CODE(LOAD_IMM_I64,  LoadImmI64);
	_HQ_BIND_OP_CODE(LOAD_IMM_U8,   LoadImmU8);
	_HQ_BIND_OP_CODE(LOAD_IMM_U16,  LoadImmU16);
	_HQ_BIND_OP_CODE(LOAD_IMM_U32,  LoadImmU32);
	_HQ_BIND_OP_CODE(LOAD_IMM_U64,  LoadImmU64);
	_HQ_BIND_OP_CODE(LOAD_IMM_F32,  LoadImmF32);
	_HQ_BIND_OP_CODE(LOAD_IMM_F64,  LoadImmF64);
	_HQ_BIND_OP_CODE(LOAD_IMM_STR, LoadImmStr);

	_HQ_BIND_OP_CODE(LOAD_GLOBAL, LoadGlobal);
	_HQ_BIND_OP_CODE(LOAD_LOCAL, LoadLocal);
	_HQ_BIND_OP_CODE(LOAD_PARAM, LoadParam);
	_HQ_BIND_OP_CODE(LOAD_OBJECT, LoadObject);
	_HQ_BIND_OP_CODE(LOAD_ARRAY, LoadArray);

	_HQ_BIND_OP_CODE(STORE_GLOBAL, StoreGlobal);
	_HQ_BIND_OP_CODE(STORE_LOCAL, StoreLocal);
	_HQ_BIND_OP_CODE(STORE_PARAM, StoreParam);
	_HQ_BIND_OP_CODE(STORE_OBJECT, StoreObject);
	_HQ_BIND_OP_CODE(STORE_ARRAY, StoreArray);

	_HQ_BIND_OP_CODE(PULL_GLOBAL, PullGlobal);
	_HQ_BIND_OP_CODE(PULL_LOCAL, PullLocal);
	_HQ_BIND_OP_CODE(PULL_PARAM, PullParam);
	_HQ_BIND_OP_CODE(PULL_OBJECT, PullObject);
	_HQ_BIND_OP_CODE(PULL_ARRAY, PullArray);

	_HQ_BIND_OP_CODE(PUSH, Push);
	_HQ_BIND_OP_CODE(POP, Pop);

	_HQ_BIND_OP_CODE(INIT_OBJECT, InitObject);
	_HQ_BIND_OP_CODE(INIT_ARRAY, InitArray);
	_HQ_BIND_OP_CODE(INIT_FUNC, InitFunction);

	_HQ_BIND_OP_CODE(JMP,       Jump);
	_HQ_BIND_OP_CODE(JMP_TRUE,  JumpIfTrue);
	_HQ_BIND_OP_CODE(JMP_FALSE, JumpIfFalse);

	_HQ_BIND_OP_CODE(LENGTH, Length);

	_HQ_BIND_OP_CODE(ADD, Add);
	_HQ_BIND_OP_CODE(SUB, Sub);
	_HQ_BIND_OP_CODE(MUL, Mul);
	_HQ_BIND_OP_CODE(DIV, Div);
	_HQ_BIND_OP_CODE(MOD, Mod);

	_HQ_BIND_OP_CODE(AND, BitAnd);
	_HQ_BIND_OP_CODE(OR, BitOr);
	_HQ_BIND_OP_CODE(XOR, BitXor);
	_HQ_BIND_OP_CODE(LSH, LeftShift);
	_HQ_BIND_OP_CODE(RSH, RightShift);
	_HQ_BIND_OP_CODE(LROT, LeftRotate);
	_HQ_BIND_OP_CODE(RROT, RightRotate);

	_HQ_BIND_OP_CODE(CAST_I8,   CastInt8);
	_HQ_BIND_OP_CODE(CAST_I16,  CastInt16);
	_HQ_BIND_OP_CODE(CAST_I32,  CastInt32);
	_HQ_BIND_OP_CODE(CAST_I64,  CastInt64);
	_HQ_BIND_OP_CODE(CAST_U8,   CastUint8);
	_HQ_BIND_OP_CODE(CAST_U16,  CastUint16);
	_HQ_BIND_OP_CODE(CAST_U32,  CastUint32);
	_HQ_BIND_OP_CODE(CAST_U64,  CastUint64);
	_HQ_BIND_OP_CODE(CAST_F32,  CastFloat32);
	_HQ_BIND_OP_CODE(CAST_F64,  CastFloat64);
	_HQ_BIND_OP_CODE(CAST_BOOL, CastBool);
	_HQ_BIND_OP_CODE(CAST_STR,  CastString);

	_HQ_BIND_OP_CODE(CMP,  Compare);
	_HQ_BIND_OP_CODE(TEST, Test);
	_HQ_BIND_OP_CODE(MOV,  Move);
	_HQ_BIND_OP_CODE(COPY, Copy);

	#undef _HQ_BIND_OP_CODE
}

//----------------------------------------------------------------------------------------------------------------------
