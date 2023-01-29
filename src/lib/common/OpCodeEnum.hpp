//
// Copyright (c) 2022, Zoe J. Bare
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

enum HqOpCodeEnum
{
	HQ_OP_CODE_NOP,
	HQ_OP_CODE_ABORT,
	HQ_OP_CODE_RETURN,
	HQ_OP_CODE_YIELD,

	HQ_OP_CODE_CALL,
	HQ_OP_CODE_CALL_VALUE,
	HQ_OP_CODE_RAISE,

	HQ_OP_CODE_LOAD_IMM_NULL,
	HQ_OP_CODE_LOAD_IMM_BOOL,
	HQ_OP_CODE_LOAD_IMM_I8,
	HQ_OP_CODE_LOAD_IMM_I16,
	HQ_OP_CODE_LOAD_IMM_I32,
	HQ_OP_CODE_LOAD_IMM_I64,
	HQ_OP_CODE_LOAD_IMM_U8,
	HQ_OP_CODE_LOAD_IMM_U16,
	HQ_OP_CODE_LOAD_IMM_U32,
	HQ_OP_CODE_LOAD_IMM_U64,
	HQ_OP_CODE_LOAD_IMM_F32,
	HQ_OP_CODE_LOAD_IMM_F64,
	HQ_OP_CODE_LOAD_IMM_STR,

	HQ_OP_CODE_LOAD_GLOBAL,
	HQ_OP_CODE_LOAD_PARAM,
	HQ_OP_CODE_LOAD_OBJECT,
	HQ_OP_CODE_LOAD_ARRAY,

	HQ_OP_CODE_STORE_GLOBAL,
	HQ_OP_CODE_STORE_PARAM,
	HQ_OP_CODE_STORE_OBJECT,
	HQ_OP_CODE_STORE_ARRAY,

	HQ_OP_CODE_PULL_GLOBAL,
	HQ_OP_CODE_PULL_PARAM,
	HQ_OP_CODE_PULL_OBJECT,
	HQ_OP_CODE_PULL_ARRAY,

	HQ_OP_CODE_PUSH,
	HQ_OP_CODE_POP,

	HQ_OP_CODE_INIT_OBJECT,
	HQ_OP_CODE_INIT_ARRAY,
	HQ_OP_CODE_INIT_FUNC,

	HQ_OP_CODE_JMP,
	HQ_OP_CODE_JMP_TRUE,
	HQ_OP_CODE_JMP_FALSE,

	HQ_OP_CODE_LENGTH,

	HQ_OP_CODE_ADD,
	HQ_OP_CODE_SUB,
	HQ_OP_CODE_MUL,
	HQ_OP_CODE_DIV,
	HQ_OP_CODE_MOD,
	HQ_OP_CODE_EXP,

	HQ_OP_CODE_AND,
	HQ_OP_CODE_OR,
	HQ_OP_CODE_XOR,
	HQ_OP_CODE_LSH,
	HQ_OP_CODE_RSH,
	HQ_OP_CODE_LROT,
	HQ_OP_CODE_RROT,

	HQ_OP_CODE_CAST_I8,
	HQ_OP_CODE_CAST_I16,
	HQ_OP_CODE_CAST_I32,
	HQ_OP_CODE_CAST_I64,
	HQ_OP_CODE_CAST_U8,
	HQ_OP_CODE_CAST_U16,
	HQ_OP_CODE_CAST_U32,
	HQ_OP_CODE_CAST_U64,
	HQ_OP_CODE_CAST_F32,
	HQ_OP_CODE_CAST_F64,
	HQ_OP_CODE_CAST_BOOL,
	HQ_OP_CODE_CAST_STR,

	HQ_OP_CODE_CMP,
	HQ_OP_CODE_TEST,
	HQ_OP_CODE_MOV,
	HQ_OP_CODE_COPY,

	HQ_OP_CODE__TOTAL_COUNT,
};

//----------------------------------------------------------------------------------------------------------------------
