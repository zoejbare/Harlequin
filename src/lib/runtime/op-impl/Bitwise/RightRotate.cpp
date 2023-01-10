//
// Copyright (c) 2023, Zoe J. Bare
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

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Perform a bitwise right rotation on an integer value.
//
// 0x: LROT r#, r#, ##
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the source value to be rotated
//   ##          = Amount to shift the source value
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_RightRotate(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint8_t shiftAmount = HqDecoder::LoadUint8(hExec->hCurrentFrame->decoder);

	HqValueHandle hSource = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
	if(hSource)
	{
		HqValueHandle hOutput = HQ_VALUE_HANDLE_NULL;

		switch(hSource->type)
		{
			case HQ_VALUE_TYPE_INT8:
			{
				constexpr uint8_t bitCount = 8;
				const uint8_t rotAmount = shiftAmount % bitCount;
				const uint8_t altRotAmount = bitCount - rotAmount;
				const int8_t value = hSource->as.int8;

				hOutput = HqValue::CreateInt8(hExec->hVm, (value >> rotAmount) | (value << altRotAmount));
				break;
			}

			case HQ_VALUE_TYPE_INT16:
			{
				constexpr uint8_t bitCount = 16;
				const uint8_t rotAmount = shiftAmount % bitCount;
				const uint8_t altRotAmount = bitCount - rotAmount;
				const int16_t value = hSource->as.int16;

				hOutput = HqValue::CreateInt16(hExec->hVm, (value >> rotAmount) | (value << altRotAmount));
				break;
			}

			case HQ_VALUE_TYPE_INT32:
			{
				constexpr uint8_t bitCount = 32;
				const uint8_t rotAmount = shiftAmount % bitCount;
				const uint8_t altRotAmount = bitCount - rotAmount;
				const int32_t value = hSource->as.int32;

				hOutput = HqValue::CreateInt32(hExec->hVm, (value >> rotAmount) | (value << altRotAmount));
				break;
			}

			case HQ_VALUE_TYPE_INT64:
			{
				constexpr uint8_t bitCount = 64;
				const uint8_t rotAmount = shiftAmount % bitCount;
				const uint8_t altRotAmount = bitCount - rotAmount;
				const int64_t value = hSource->as.int64;

				hOutput = HqValue::CreateInt64(hExec->hVm, (value >> rotAmount) | (value << altRotAmount));
				break;
			}

			case HQ_VALUE_TYPE_UINT8:
			{
				constexpr uint8_t bitCount = 8;
				const uint8_t rotAmount = shiftAmount % bitCount;
				const uint8_t altRotAmount = bitCount - rotAmount;
				const uint8_t value = hSource->as.uint8;

				hOutput = HqValue::CreateUint8(hExec->hVm, (value >> rotAmount) | (value << altRotAmount));
				break;
			}

			case HQ_VALUE_TYPE_UINT16:
			{
				constexpr uint8_t bitCount = 16;
				const uint8_t rotAmount = shiftAmount % bitCount;
				const uint8_t altRotAmount = bitCount - rotAmount;
				const uint16_t value = hSource->as.uint16;

				hOutput = HqValue::CreateUint16(hExec->hVm, (value >> rotAmount) | (value << altRotAmount));
				break;
			}

			case HQ_VALUE_TYPE_UINT32:
			{
				constexpr uint8_t bitCount = 32;
				const uint8_t rotAmount = shiftAmount % bitCount;
				const uint8_t altRotAmount = bitCount - rotAmount;
				const uint32_t value = hSource->as.uint32;

				hOutput = HqValue::CreateUint32(hExec->hVm, (value >> rotAmount) | (value << altRotAmount));
				break;
			}

			case HQ_VALUE_TYPE_UINT64:
			{
				constexpr uint8_t bitCount = 64;
				const uint8_t rotAmount = shiftAmount % bitCount;
				const uint8_t altRotAmount = bitCount - rotAmount;
				const uint64_t value = hSource->as.uint64;

				hOutput = HqValue::CreateUint64(hExec->hVm, (value >> rotAmount) | (value << altRotAmount));
				break;
			}

			default:
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec, 
					HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
					"Type mismatch; expected integer type: r(%" PRIu32 ")", 
					gpSrcRegIndex
				);
				return;
			}
		}

		if(hOutput)
		{
			// Remove the auto-mark from the output value so it can be cleaned up when it's no longer referenced.
			HqValue::SetAutoMark(hOutput, false);

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
			HQ_STANDARD_EXCEPTION_RUNTIME_ERROR, 
			"Failed to retrieve general-purpose register: r(%" PRIu32 ")", 
			gpSrcRegIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_RightRotate(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint8_t shiftAmount = HqDecoder::LoadUint8(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "RROT r%" PRIu32 ", r%" PRIu32 ", #%" PRIu8, gpDstRegIndex, gpSrcRegIndex, shiftAmount);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_RightRotate(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
	HqDecoder::EndianSwapUint8(decoder); // ##
}

//----------------------------------------------------------------------------------------------------------------------
