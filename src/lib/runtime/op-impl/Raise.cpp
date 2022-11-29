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

#include "../OpDecl.hpp"

#include "../Decoder.hpp"
#include "../Execution.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Raise an exception using the value in the specified general-purpose register index as the exception value.
//
// 0x: RAISE r#
//
//   r# = Register index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_Raise(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hValue = XenonFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
	if(result == XENON_SUCCESS)
	{
		// Raise a regular exception from the value at the indicated register.
		XenonExecution::RaiseException(hExec, hValue, XENON_EXCEPTION_SEVERITY_NORMAL);
	}
	else
	{
		// Raise a fatal script exception.
		XenonExecution::RaiseOpCodeException(
			hExec, 
			XENON_STANDARD_EXCEPTION_RUNTIME_ERROR, 
			"Failed to retrieve general-purpose register: r(%" PRIu32 ")", 
			registerIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_Raise(XenonDisassemble& disasm)
{
	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);

	char str[24];
	snprintf(str, sizeof(str), "RAISE r%" PRIu32, registerIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
