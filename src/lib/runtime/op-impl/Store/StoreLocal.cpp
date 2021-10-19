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
// 0x: STORE_LOCAL c#, r#
//
//   c# = Constant index of the name string of the local variable
//   r# = General-purpose register index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_StoreLocal(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t constantIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hNameValue = XenonProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constantIndex, &result);
	if(XenonValueIsString(hNameValue))
	{
		XenonValueHandle hRegisterValue = XenonFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
		if(result == XENON_SUCCESS)
		{
			result = XenonFrame::SetLocalVariable(hExec->hCurrentFrame, hRegisterValue, hNameValue->as.pString);
			if(result != XENON_SUCCESS)
			{
				// TODO: Raise script exception.
				hExec->exception = true;
			}
		}
		else
		{
			// TODO: Raise script exception
			hExec->exception = true;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_StoreLocal(XenonDisassemble& disasm)
{
	int result;

	const uint32_t constantIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);

	XenonValueHandle hNameValue = XenonProgram::GetConstant(disasm.hProgram, constantIndex, &result);
	XenonString* const pValueData = XenonValue::GetDebugString(hNameValue);

	char str[256];
	snprintf(str, sizeof(str), "STORE_LOCAL c%" PRIu32 " %s, r%" PRIu32, constantIndex, pValueData->data, registerIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);

	XenonString::Release(pValueData);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
