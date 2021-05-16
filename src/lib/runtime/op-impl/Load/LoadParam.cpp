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
#include "../../Frame.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Get a value in an I/O register, storing it to a general-purpose register.
// The I/O register will then be cleared to avoid leaking value data.
//
// 0x: LOAD_PARAM r#, p#
//
//   r# = General-purpose register index
//   p# = I/O register index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_LoadParam(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t gpRegIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t ioRegIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hValue = XenonExecution::GetIoRegister(hExec, ioRegIndex, &result);
	if(result == XENON_SUCCESS)
	{
		result = XenonFrame::SetGpRegister(hExec->hCurrentFrame, hValue, gpRegIndex);
		if(result == XENON_SUCCESS)
		{
			// Clear the I/O register to prevent leaking values left in there.
			result = XenonExecution::SetIoRegister(hExec, XENON_VALUE_HANDLE_NULL, ioRegIndex);
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
	else
	{
		// TODO: Raise script exception
		hExec->exception = true;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadParam(XenonDisassemble& disasm)
{
	const uint32_t gpRegIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t ioRegIndex = XenonDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "LOAD_PARAM r%" PRIu32 ", p%" PRIu32, gpRegIndex, ioRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
