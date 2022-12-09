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
#include "../../Function.hpp"
#include "../../Program.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Load a local variable to a general-purpose register in the current frame.
//
// 0x: STORE_LOCAL s#, r#
//
//   s# = String table index of the name string of the local variable
//   r# = General-purpose register index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_StoreLocal(HqExecutionHandle hExec)
{
	int result;

	const uint32_t stringIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqString* const pVarName = HqProgram::GetString(hExec->hCurrentFrame->hFunction->hProgram, stringIndex, &result);
	if(pVarName)
	{
		HqValueHandle hRegisterValue = HqFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
		if(result == HQ_SUCCESS)
		{
			result = HqFrame::SetLocalVariable(hExec->hCurrentFrame, hRegisterValue, pVarName);
			if(result != HQ_SUCCESS)
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec,
					HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
					"Failed to set local variable: %s",
					pVarName->data
				);
			}
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec,
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Failed to retrieve general-purpose register: r(%" PRIu32 ")",
				registerIndex
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec,
			HQ_STANDARD_EXCEPTION_TYPE_ERROR,
			"String does not exist at index: s(%" PRIu32 ")",
			stringIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_StoreLocal(HqDisassemble& disasm)
{
	const uint32_t stringIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[256];
	snprintf(str, sizeof(str), "STORE_LOCAL s%" PRIu32 ", r%" PRIu32, stringIndex, registerIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
