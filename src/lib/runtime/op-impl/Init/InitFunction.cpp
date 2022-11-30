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

void OpCodeExec_InitFunction(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t constIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hFuncName = XenonProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constIndex, &result);
	if(XenonValueIsString(hFuncName))
	{
		XenonFunctionHandle hFunction = XenonVm::GetFunction(hExec->hVm, hFuncName->as.pString, &result);
		if(hFunction)
		{
			XenonValueHandle hFuncValue = XenonValue::CreateFunction(hExec->hVm, hFunction);
			if(hFuncValue)
			{
				result = XenonFrame::SetGpRegister(hExec->hCurrentFrame, hFuncValue, registerIndex);
				if(result != XENON_SUCCESS)
				{
					// Raise a fatal script exception.
					XenonExecution::RaiseOpCodeException(
						hExec,
						XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
						"Failed to set general-purpose register: r(%" PRIu32 ")",
						registerIndex
					);
				}

				XenonValue::SetAutoMark(hFuncValue, false);
			}
		}
		else
		{
			// Raise a fatal script exception.
			XenonExecution::RaiseOpCodeException(
				hExec,
				XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Invalid function signature: %s",
				hFuncName->as.pString->data
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		XenonExecution::RaiseOpCodeException(
			hExec,
			XENON_STANDARD_EXCEPTION_TYPE_ERROR,
			"Type mismatch; expected string value: c(%" PRIu32 ")",
			constIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_InitFunction(XenonDisassemble& disasm)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t constIndex = XenonDecoder::LoadUint32(disasm.decoder);

	XenonValueHandle hValue = XenonProgram::GetConstant(disasm.hProgram, constIndex, &result);
	XenonString* const pValueData = XenonValue::GetDebugString(hValue);

	char instr[512];
	snprintf(instr, sizeof(instr), "INIT_FUNC r%" PRIu32 ", c%" PRIu32 " %s", registerIndex, constIndex, pValueData->data);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);

	XenonString::Release(pValueData);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
