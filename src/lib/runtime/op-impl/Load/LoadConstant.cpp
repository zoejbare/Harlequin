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

#include "../../Execution.hpp"
#include "../../OpDecl.hpp"
#include "../../Program.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Load a constant to a general-purpose register in the current frame.
//
// 0x: LOAD_CONST r#, c#
//
//   r# = General-purpose register index
//   c# = Constant table index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeImpl_LoadConstant(XenonExecutionHandle hExec)
{
	const uint32_t registerIndex = XenonExecution::LoadBytecodeUint32(hExec);
	const uint32_t constIndex = XenonExecution::LoadBytecodeUint32(hExec);

	printf("LOAD_CONST r%" PRIu32 ", c%" PRIu32 "\n", registerIndex, constIndex);

	XenonValueHandle hValue = XenonProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constIndex);

	XenonFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
	XenonValueDispose(hValue);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
