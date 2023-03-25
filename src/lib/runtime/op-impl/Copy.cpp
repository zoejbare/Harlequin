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
// Copy a value from one general-purpose register to another general-purpose register.
//
// 0x: COPY r#, r#
//
//   r# [first]  = General-purpose register index where the copied value will be stored
//   r# [second] = General-purpose register index containing the value to be copied
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_Copy(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hSource = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
	if(hSource)
	{
		// Create a new copy of the source value.
		HqValueHandle hOutput = HqValue::Copy(hExec->hVm, hSource);
		if(hOutput)
		{
			result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hOutput, gpDstRegIndex);
			if(result != HQ_SUCCESS)
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec,
					HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
					"Failed to set general-purpose register: r(%" PRIu32 ")",
					gpDstRegIndex
				);
			}
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec, 
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR, 
				"Failed to create output value"
			);
		}
	}
	else
	{
		// When the input register references a null value, we can set the output register directly to null.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, HQ_VALUE_HANDLE_NULL, gpDstRegIndex);
		if(result != HQ_SUCCESS)
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec,
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Failed to set general-purpose register: r(%" PRIu32 ")",
				gpDstRegIndex
			);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_Copy(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "COPY r(%" PRIu32 "), r(%" PRIu32 ")", gpDstRegIndex, gpSrcRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_Copy(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
}

//----------------------------------------------------------------------------------------------------------------------
