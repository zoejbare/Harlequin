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
// 0x: LOAD_GLOBAL r#, c#
//
//   r# = General-purpose register index
//   c# = Constant index of the name of the global variable
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_LoadGlobal(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t constantIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hNameValue = XenonProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constantIndex, &result);
	if(XenonValueIsString(hNameValue))
	{
		XenonValueHandle hGlobalVariable = XenonVm::GetGlobalVariable(hExec->hVm, hNameValue->as.pString, &result);
		if(hGlobalVariable)
		{
			XenonFrame::SetGpRegister(hExec->hCurrentFrame, hGlobalVariable, registerIndex);
			XenonValueDispose(hGlobalVariable);
		}
		else
		{
			// TODO: Raise script exception.
			hExec->exception = true;
		}
	}
	else
	{
		// TODO: Raise script exception.
		hExec->exception = true;
	}

	XenonValueDispose(hNameValue);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadGlobal(XenonDisassemble& disasm)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t constantIndex = XenonDecoder::LoadUint32(disasm.decoder);

	XenonValueHandle hNameValue = XenonProgram::GetConstant(disasm.hProgram, constantIndex, &result);
	std::string valueData = XenonValue::GetDebugString(hNameValue);

	XenonValueDispose(hNameValue);

	char str[256];
	snprintf(str, sizeof(str), "LOAD_GLOBAL r%" PRIu32 ", c%" PRIu32 " %s", registerIndex, constantIndex, valueData.c_str());
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
