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
// Perform a bitwise left shift on an integer value.
//
// 0x: RSH r#, r#, ##
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the source value to be shifted
//   ##          = Amount to shift the source value
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_LeftShift(HqExecutionHandle hExec)
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
			case HQ_VALUE_TYPE_INT8:    hOutput = HqValue::CreateInt8(hExec->hVm, hSource->as.int8 << shiftAmount);     break;
			case HQ_VALUE_TYPE_INT16:   hOutput = HqValue::CreateInt16(hExec->hVm, hSource->as.int16 << shiftAmount);   break;
			case HQ_VALUE_TYPE_INT32:   hOutput = HqValue::CreateInt32(hExec->hVm, hSource->as.int32 << shiftAmount);   break;
			case HQ_VALUE_TYPE_INT64:   hOutput = HqValue::CreateInt64(hExec->hVm, hSource->as.int64 << shiftAmount);   break;
			case HQ_VALUE_TYPE_UINT8:   hOutput = HqValue::CreateUint8(hExec->hVm, hSource->as.uint8 << shiftAmount);   break;
			case HQ_VALUE_TYPE_UINT16:  hOutput = HqValue::CreateUint16(hExec->hVm, hSource->as.uint16 << shiftAmount); break;
			case HQ_VALUE_TYPE_UINT32:  hOutput = HqValue::CreateUint32(hExec->hVm, hSource->as.uint32 << shiftAmount); break;
			case HQ_VALUE_TYPE_UINT64:  hOutput = HqValue::CreateUint64(hExec->hVm, hSource->as.uint64 << shiftAmount); break;

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

extern "C" void OpCodeDisasm_LeftShift(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint8_t shiftAmount = HqDecoder::LoadUint8(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "LSH r%" PRIu32 ", r%" PRIu32 ", #%" PRIu8, gpDstRegIndex, gpSrcRegIndex, shiftAmount);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_LeftShift(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
	HqDecoder::EndianSwapUint8(decoder); // ##
}

//----------------------------------------------------------------------------------------------------------------------
