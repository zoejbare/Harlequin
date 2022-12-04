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
	#define HQ_BIND_OP_CODE(opCode, name) \
		hVm->opCodes.pData[HQ_OP_CODE_ ## opCode].execFn = OpCodeExec_ ## name; \
		hVm->opCodes.pData[HQ_OP_CODE_ ## opCode].disasmFn = OpCodeDisasm_ ## name

	HQ_BIND_OP_CODE(NOP, Nop);
	HQ_BIND_OP_CODE(ABORT, Abort);
	HQ_BIND_OP_CODE(RETURN, Return);
	HQ_BIND_OP_CODE(YIELD, Yield);

	HQ_BIND_OP_CODE(CALL, Call);
	HQ_BIND_OP_CODE(CALL_VALUE, CallValue);
	HQ_BIND_OP_CODE(RAISE, Raise);

	HQ_BIND_OP_CODE(LOAD_CONSTANT, LoadConstant);
	HQ_BIND_OP_CODE(LOAD_GLOBAL, LoadGlobal);
	HQ_BIND_OP_CODE(LOAD_LOCAL, LoadLocal);
	HQ_BIND_OP_CODE(LOAD_PARAM, LoadParam);
	HQ_BIND_OP_CODE(LOAD_OBJECT, LoadObject);
	HQ_BIND_OP_CODE(LOAD_ARRAY, LoadArray);

	HQ_BIND_OP_CODE(STORE_GLOBAL, StoreGlobal);
	HQ_BIND_OP_CODE(STORE_LOCAL, StoreLocal);
	HQ_BIND_OP_CODE(STORE_PARAM, StoreParam);
	HQ_BIND_OP_CODE(STORE_OBJECT, StoreObject);
	HQ_BIND_OP_CODE(STORE_ARRAY, StoreArray);

	HQ_BIND_OP_CODE(PULL_GLOBAL, PullGlobal);
	HQ_BIND_OP_CODE(PULL_LOCAL, PullLocal);
	HQ_BIND_OP_CODE(PULL_PARAM, PullParam);
	HQ_BIND_OP_CODE(PULL_OBJECT, PullObject);
	HQ_BIND_OP_CODE(PULL_ARRAY, PullArray);

	HQ_BIND_OP_CODE(PUSH, Push);
	HQ_BIND_OP_CODE(POP, Pop);

	HQ_BIND_OP_CODE(INIT_OBJECT, InitObject);
	HQ_BIND_OP_CODE(INIT_ARRAY, InitArray);
	HQ_BIND_OP_CODE(INIT_FUNCTION, InitFunction);

	HQ_BIND_OP_CODE(BRANCH, Branch);
	HQ_BIND_OP_CODE(BRANCH_IF_TRUE, BranchIfTrue);
	HQ_BIND_OP_CODE(BRANCH_IF_FALSE, BranchIfFalse);

	#undef HQ_BIND_OP_CODE
}

//----------------------------------------------------------------------------------------------------------------------
