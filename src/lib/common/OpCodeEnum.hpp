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

	HQ_OP_CODE_LOAD_CONSTANT,
	HQ_OP_CODE_LOAD_GLOBAL,
	HQ_OP_CODE_LOAD_LOCAL,
	HQ_OP_CODE_LOAD_PARAM,
	HQ_OP_CODE_LOAD_OBJECT,
	HQ_OP_CODE_LOAD_ARRAY,

	HQ_OP_CODE_STORE_GLOBAL,
	HQ_OP_CODE_STORE_LOCAL,
	HQ_OP_CODE_STORE_PARAM,
	HQ_OP_CODE_STORE_OBJECT,
	HQ_OP_CODE_STORE_ARRAY,

	HQ_OP_CODE_PULL_GLOBAL,
	HQ_OP_CODE_PULL_LOCAL,
	HQ_OP_CODE_PULL_PARAM,
	HQ_OP_CODE_PULL_OBJECT,
	HQ_OP_CODE_PULL_ARRAY,

	HQ_OP_CODE_PUSH,
	HQ_OP_CODE_POP,

	HQ_OP_CODE_INIT_OBJECT,
	HQ_OP_CODE_INIT_ARRAY,
	HQ_OP_CODE_INIT_FUNCTION,

	HQ_OP_CODE_BRANCH,
	HQ_OP_CODE_BRANCH_IF_TRUE,
	HQ_OP_CODE_BRANCH_IF_FALSE,

	HQ_OP_CODE__TOTAL_COUNT,
};

//----------------------------------------------------------------------------------------------------------------------
