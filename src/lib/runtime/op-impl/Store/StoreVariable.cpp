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
// Set the value in a general-purpose register to a variable register.
//
// 0x: STORE_VAR v#, r#
//
//   v# = Variable register index
//   r# = General-purpose register index
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_StoreVariable(HqExecutionHandle hExec)
{
	int result;

	const uint32_t vrRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpRegIndex, &result);
	if(result == HQ_SUCCESS)
	{
		result = HqFrame::SetVrRegister(hExec->hCurrentFrame, hValue, vrRegIndex);
		if(result != HQ_SUCCESS)
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec,
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Failed to set variable register: v(%" PRIu32 ")",
				vrRegIndex
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
			gpRegIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_StoreVariable(HqDisassemble& disasm)
{
	const uint32_t vrRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "STORE_VAR v(%" PRIu32 "), r(%" PRIu32 ")", vrRegIndex, gpRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_StoreVariable(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // v#
	HqDecoder::EndianSwapUint32(decoder); // r#
}

//----------------------------------------------------------------------------------------------------------------------
