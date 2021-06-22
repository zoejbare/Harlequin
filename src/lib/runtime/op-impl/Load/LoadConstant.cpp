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

#include "../../OpDecl.hpp"

#include "../../Decoder.hpp"
#include "../../Execution.hpp"
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

void OpCodeExec_LoadConstant(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t constantIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hValue = XenonProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constantIndex, &result);
	if(result == XENON_SUCCESS)
	{
		// Make a copy of the value, then upload that copy to the register.
		// A copy must be made to avoid accidentally changing the underlying
		// data of the constant.
		result = XenonFrame::SetGpRegister(hExec->hCurrentFrame, XenonValue::Copy(hExec->hVm, hValue), registerIndex);
		if(result != XENON_SUCCESS)
		{
			// TODO: Raise script exception
			hExec->exception = true;
		}
	}
	else
	{
		// TODO: Raise script exception
		hExec->exception = true;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstant(XenonDisassemble& disasm)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t constantIndex = XenonDecoder::LoadUint32(disasm.decoder);

	XenonValueHandle hValue = XenonProgram::GetConstant(disasm.hProgram, constantIndex, &result);
	XenonString* const pValueData = XenonValue::GetDebugString(hValue);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST r%" PRIu32 ", c%" PRIu32 " %s", registerIndex, constantIndex, pValueData->data);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);

	XenonString::Release(pValueData);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
