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

//----------------------------------------------------------------------------------------------------------------------

void XenonVm::prv_setupOpCodes(XenonVmHandle hVm)
{
	#define XENON_BIND_OP_CODE(opCode, name) \
		hVm->opCodes.pData[XENON_OP_CODE_ ## opCode].execFn = OpCodeExec_ ## name; \
		hVm->opCodes.pData[XENON_OP_CODE_ ## opCode].disasmFn = OpCodeDisasm_ ## name

	XENON_BIND_OP_CODE(NOP, Nop);
	XENON_BIND_OP_CODE(ABORT, Abort);
	XENON_BIND_OP_CODE(RETURN, Return);
	XENON_BIND_OP_CODE(YIELD, Yield);
	XENON_BIND_OP_CODE(CALL, Call);
	XENON_BIND_OP_CODE(LOAD_CONSTANT, LoadConstant);
	XENON_BIND_OP_CODE(LOAD_GLOBAL, LoadGlobal);
	XENON_BIND_OP_CODE(LOAD_LOCAL, LoadLocal);
	XENON_BIND_OP_CODE(LOAD_PARAM, LoadParam);
	XENON_BIND_OP_CODE(LOAD_OBJECT, LoadObject);
	XENON_BIND_OP_CODE(STORE_GLOBAL, StoreGlobal);
	XENON_BIND_OP_CODE(STORE_LOCAL, StoreLocal);
	XENON_BIND_OP_CODE(STORE_PARAM, StoreParam);
	XENON_BIND_OP_CODE(STORE_OBJECT, StoreObject);
	XENON_BIND_OP_CODE(PUSH, Push);
	XENON_BIND_OP_CODE(POP, Pop);
	XENON_BIND_OP_CODE(INIT_OBJECT, InitObject);
	XENON_BIND_OP_CODE(BRANCH, Branch);
	XENON_BIND_OP_CODE(BRANCH_IF_TRUE, BranchIfTrue);
	XENON_BIND_OP_CODE(BRANCH_IF_FALSE, BranchIfFalse);

	#undef XENON_BIND_OP_CODE
}

//----------------------------------------------------------------------------------------------------------------------
