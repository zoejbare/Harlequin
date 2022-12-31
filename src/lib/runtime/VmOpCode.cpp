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

	_HQ_BIND_OP_CODE(NOP, Nop);
	_HQ_BIND_OP_CODE(ABORT, Abort);
	_HQ_BIND_OP_CODE(RETURN, Return);
	_HQ_BIND_OP_CODE(YIELD, Yield);

	_HQ_BIND_OP_CODE(CALL, Call);
	_HQ_BIND_OP_CODE(CALL_VALUE, CallValue);
	_HQ_BIND_OP_CODE(RAISE, Raise);

	_HQ_BIND_OP_CODE(LOAD_CONST_NULL, LoadConstNull);
	_HQ_BIND_OP_CODE(LOAD_CONST_BOOL, LoadConstBool);
	_HQ_BIND_OP_CODE(LOAD_CONST_I8, LoadConstI8);
	_HQ_BIND_OP_CODE(LOAD_CONST_I16, LoadConstI16);
	_HQ_BIND_OP_CODE(LOAD_CONST_I32, LoadConstI32);
	_HQ_BIND_OP_CODE(LOAD_CONST_I64, LoadConstI64);
	_HQ_BIND_OP_CODE(LOAD_CONST_U8, LoadConstU8);
	_HQ_BIND_OP_CODE(LOAD_CONST_U16, LoadConstU16);
	_HQ_BIND_OP_CODE(LOAD_CONST_U32, LoadConstU32);
	_HQ_BIND_OP_CODE(LOAD_CONST_U64, LoadConstU64);
	_HQ_BIND_OP_CODE(LOAD_CONST_F32, LoadConstF32);
	_HQ_BIND_OP_CODE(LOAD_CONST_F64, LoadConstF64);
	_HQ_BIND_OP_CODE(LOAD_CONST_STR, LoadConstStr);

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
	_HQ_BIND_OP_CODE(INIT_FUNCTION, InitFunction);

	_HQ_BIND_OP_CODE(BRANCH, Branch);
	_HQ_BIND_OP_CODE(BRANCH_IF_TRUE, BranchIfTrue);
	_HQ_BIND_OP_CODE(BRANCH_IF_FALSE, BranchIfFalse);

	_HQ_BIND_OP_CODE(LENGTH, Length);

	_HQ_BIND_OP_CODE(ADD, Add);
	_HQ_BIND_OP_CODE(SUB, Sub);
	_HQ_BIND_OP_CODE(MUL, Mul);
	_HQ_BIND_OP_CODE(DIV, Div);

	_HQ_BIND_OP_CODE(CAST_I8, CastInt8);
	_HQ_BIND_OP_CODE(CAST_I16, CastInt16);
	_HQ_BIND_OP_CODE(CAST_I32, CastInt32);
	_HQ_BIND_OP_CODE(CAST_I64, CastInt64);
	_HQ_BIND_OP_CODE(CAST_U8, CastUint8);
	_HQ_BIND_OP_CODE(CAST_U16, CastUint16);
	_HQ_BIND_OP_CODE(CAST_U32, CastUint32);
	_HQ_BIND_OP_CODE(CAST_U64, CastUint64);
	_HQ_BIND_OP_CODE(CAST_F32, CastFloat32);
	_HQ_BIND_OP_CODE(CAST_F64, CastFloat64);
	_HQ_BIND_OP_CODE(CAST_BOOL, CastBool);
	_HQ_BIND_OP_CODE(CAST_STR, CastString);

	#undef _HQ_BIND_OP_CODE
}

//----------------------------------------------------------------------------------------------------------------------
