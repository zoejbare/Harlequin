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
#include "../../ScriptObject.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Set the value in a general-purpose register to an index of an array.
//
// 0x: STORE_ARRAY r#, r#, ##
//
//   r# [first]  = General-purpose register index that contains the array where the value will be stored
//   r# [second] = General-purpose register index of the value to store in the array
//   ##          = Immediate integer used as the direct index into the array
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_StoreArray(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t arrayIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	// Load the array from the destination register.
	HqValueHandle hDestination = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpDstRegIndex, &result);
	if(result == HQ_SUCCESS)
	{
		// Verify the destination value is an array.
		if(HqValueIsArray(hDestination))
		{
			// Verify the array index is within the bounds of the array.
			if(size_t(arrayIndex) < hDestination->as.array.count)
			{
				// Load the source value to be placed into the array.
				HqValueHandle hSource = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
				if(result == HQ_SUCCESS)
				{
					hDestination->as.array.pData[arrayIndex] = hSource;
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
			else
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec,
					HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
					"Array index out of range: r(%" PRIu32 "), length=%zu, index=%" PRIu32,
					gpSrcRegIndex,
					hDestination->as.array.count,
					arrayIndex
				);
			}
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec,
				HQ_STANDARD_EXCEPTION_TYPE_ERROR,
				"Type mismatch; expected array: r(%" PRIu32 ")",
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
			"Failed to retrieve general-purpose regsiter: r(%" PRIu32 ")",
			gpDstRegIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_StoreArray(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t arrayIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "STORE_ARRAY r%" PRIu32 ", r%" PRIu32 ", #%" PRIu32, gpDstRegIndex, gpSrcRegIndex, arrayIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
