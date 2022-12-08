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
// 0x: LOAD_CONSTANT r#, c#
//
//   r# = General-purpose register index
//   c# = Constant table index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_LoadConstant(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t constantIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constantIndex, &result);
	if(result == HQ_SUCCESS)
	{
		HqValueHandle hValueCopy = HqValue::Copy(hExec->hVm, hValue);
		if(hValueCopy)
		{
			// Make a copy of the value, then upload that copy to the register.
			// A copy must be made to avoid accidentally changing the underlying
			// data of the constant.
			result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValueCopy, registerIndex);
			if(result != HQ_SUCCESS)
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec,
					HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
					"Failed to set general-purpose register: r(%" PRIu32 ")",
					registerIndex
				);
			}
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec,
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Failed to create copy of constant value: c(%" PRIu32 ")",
				constantIndex
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec,
			HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
			"Failed to retrieve constant value: c(%" PRIu32 ")",
			constantIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstant(HqDisassemble& disasm)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t constantIndex = HqDecoder::LoadUint32(disasm.decoder);

	HqValueHandle hValue = HqProgram::GetConstant(disasm.hProgram, constantIndex, &result);
	HqString* const pValueData = HqValue::GetDebugString(hValue);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONSTANT r%" PRIu32 ", c%" PRIu32 " %s", registerIndex, constantIndex, pValueData->data);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);

	HqString::Release(pValueData);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
