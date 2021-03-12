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

#include "../OpDecl.hpp"

#include "../Decoder.hpp"
#include "../Execution.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Pop the value at the head of the stack, storing its value in the specified general-purpose register.
//
// 0x: POP r#
//
//   r# = Register index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_Pop(XenonExecutionHandle hExec)
{
	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hValue = XENON_VALUE_HANDLE_NULL;

	int result = XenonFrame::PopValue(hExec->hCurrentFrame, &hValue);
	if(result != XENON_SUCCESS)
	{
		// TODO: Raise exception
		hExec->exception = true;
		return;
	}

	XenonFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
	XenonValueDispose(hValue);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_Pop(XenonDisassemble& disasm)
{
	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);

	char str[16];
	snprintf(str, sizeof(str), "POP r%" PRIu32, registerIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
