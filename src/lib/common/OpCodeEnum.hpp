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

enum XenonOpCodeEnum
{
	XENON_OP_CODE_NOP,
	XENON_OP_CODE_ABORT,
	XENON_OP_CODE_RETURN,
	XENON_OP_CODE_YIELD,

	XENON_OP_CODE_CALL,
	XENON_OP_CODE_CALL_VALUE,
	XENON_OP_CODE_RAISE,

	XENON_OP_CODE_LOAD_CONSTANT,
	XENON_OP_CODE_LOAD_GLOBAL,
	XENON_OP_CODE_LOAD_LOCAL,
	XENON_OP_CODE_LOAD_PARAM,
	XENON_OP_CODE_LOAD_OBJECT,
	XENON_OP_CODE_LOAD_ARRAY,

	XENON_OP_CODE_STORE_GLOBAL,
	XENON_OP_CODE_STORE_LOCAL,
	XENON_OP_CODE_STORE_PARAM,
	XENON_OP_CODE_STORE_OBJECT,
	XENON_OP_CODE_STORE_ARRAY,

	XENON_OP_CODE_PULL_GLOBAL,
	XENON_OP_CODE_PULL_LOCAL,
	XENON_OP_CODE_PULL_PARAM,
	XENON_OP_CODE_PULL_OBJECT,
	XENON_OP_CODE_PULL_ARRAY,

	XENON_OP_CODE_PUSH,
	XENON_OP_CODE_POP,

	XENON_OP_CODE_INIT_OBJECT,
	XENON_OP_CODE_INIT_ARRAY,
	XENON_OP_CODE_INIT_FUNCTION,

	XENON_OP_CODE_BRANCH,
	XENON_OP_CODE_BRANCH_IF_TRUE,
	XENON_OP_CODE_BRANCH_IF_FALSE,

	XENON_OP_CODE__TOTAL_COUNT,
};

//----------------------------------------------------------------------------------------------------------------------
