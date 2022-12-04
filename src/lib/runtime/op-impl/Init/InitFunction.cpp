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

#include "../../OpDecl.hpp"

#include "../../Decoder.hpp"
#include "../../Execution.hpp"
#include "../../Frame.hpp"
#include "../../Program.hpp"
#include "../../Vm.hpp"

#include <stdio.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Initialize a new array, storing it in a general-purpose register.
//
// 0x: INIT_FUNC r#, c#
//
//   r# = General-purpose register where the new array will be stored
//   ## = Constant index containing the signature of the function to store in the destination value
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_InitFunction(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t constIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hFuncName = HqProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constIndex, &result);
	if(HqValueIsString(hFuncName))
	{
		HqFunctionHandle hFunction = HqVm::GetFunction(hExec->hVm, hFuncName->as.pString, &result);
		if(hFunction)
		{
			HqValueHandle hFuncValue = HqValue::CreateFunction(hExec->hVm, hFunction);
			if(hFuncValue)
			{
				result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hFuncValue, registerIndex);
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

				HqValue::SetAutoMark(hFuncValue, false);
			}
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec,
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Invalid function signature: %s",
				hFuncName->as.pString->data
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec,
			HQ_STANDARD_EXCEPTION_TYPE_ERROR,
			"Type mismatch; expected string value: c(%" PRIu32 ")",
			constIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_InitFunction(HqDisassemble& disasm)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t constIndex = HqDecoder::LoadUint32(disasm.decoder);

	HqValueHandle hValue = HqProgram::GetConstant(disasm.hProgram, constIndex, &result);
	HqString* const pValueData = HqValue::GetDebugString(hValue);

	char instr[512];
	snprintf(instr, sizeof(instr), "INIT_FUNC r%" PRIu32 ", c%" PRIu32 " %s", registerIndex, constIndex, pValueData->data);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);

	HqString::Release(pValueData);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
