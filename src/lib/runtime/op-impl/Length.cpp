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
#include "../Frame.hpp"
#include "../ScriptObject.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Store the length of an array or string in a general-purpose register as a uint32 value.
//
// 0x: LENGTH r#, r#
//
//   r# [first]  = General-purpose register index where the length will be stored
//   r# [second] = General-purpose register index that contains the array or string
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_Length(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	// Load the value from the source register.
	HqValueHandle hSource = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
	if(result == HQ_SUCCESS)
	{
		uint32_t length = 0;
		bool validType = false;

		if(HqValueIsArray(hSource))
		{
			// Get the length of the value data as an array.
			length = uint32_t(hSource->as.array.count);
			validType = true;
		}
		else if(HqValueIsString(hSource))
		{
			// Get the length of the value data as a string.
			length = uint32_t(hSource->as.pString->length);
			validType = true;
		}

		if(validType)
		{
			// Create a value to hold the length.
			HqValueHandle hOutput = HqValue::CreateUint32(hExec->hVm, length);
			if(hOutput)
			{
				// Remove the auto-mark from the output value so it can be cleaned up when it's no longer referenced.
				HqValue::SetAutoMark(hOutput, false);

				// Store the output value in the destination register.
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
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec, 
				HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
				"Type mismatch; expected array or string: r(%" PRIu32 ")", 
				gpSrcRegIndex
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
			gpSrcRegIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_Length(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "LENGTH r(%" PRIu32 "), r(%" PRIu32 ")", gpDstRegIndex, gpSrcRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_Length(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
}

//----------------------------------------------------------------------------------------------------------------------
