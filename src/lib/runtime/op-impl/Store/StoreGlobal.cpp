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
#include "../../Vm.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Load a global variable to a general-purpose register in the current frame.
//
// 0x: STORE_GLOBAL c#, r#
//
//   c# = Constant index of the name of the global variable
//   r# = General-purpose register index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_StoreGlobal(HqExecutionHandle hExec)
{
	int result;

	const uint32_t constantIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hNameValue = HqProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constantIndex, &result);
	if(HqValueIsString(hNameValue))
	{
		HqValueHandle hRegisterValue = HqFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
		if(result == HQ_SUCCESS)
		{
			result = HqVm::SetGlobalVariable(hExec->hVm, hRegisterValue, hNameValue->as.pString);
			if(result != HQ_SUCCESS)
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec,
					HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
					"Failed to set global variable: %s",
					hNameValue->as.pString->data
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
			"Type mismatch; expected string: c(%" PRIu32 ")",
			constantIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_StoreGlobal(HqDisassemble& disasm)
{
	int result;

	const uint32_t constantIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);

	HqValueHandle hNameValue = HqProgram::GetConstant(disasm.hProgram, constantIndex, &result);
	HqString* const pValueData = HqValue::GetDebugString(hNameValue);

	char str[256];
	snprintf(str, sizeof(str), "STORE_GLOBAL c%" PRIu32 " %s, r%" PRIu32, constantIndex, pValueData->data, registerIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);

	HqString::Release(pValueData);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
