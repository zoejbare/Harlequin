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
// Load a value from a general-purpose register and perform a bitwise NOT operation on it, flipping its bits.
//
// 0x: AND r#, r#
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the input operand value
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_BitNot(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hSrc = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
	if(hSrc)
	{
		HqValueHandle hOutput = HQ_VALUE_HANDLE_NULL;

		switch(hSrc->type)
		{
			case HQ_VALUE_TYPE_INT8:   hOutput = HqValue::CreateInt8(hExec->hVm, ~hSrc->as.int8);     break;
			case HQ_VALUE_TYPE_INT16:  hOutput = HqValue::CreateInt16(hExec->hVm, ~hSrc->as.int16);   break;
			case HQ_VALUE_TYPE_INT32:  hOutput = HqValue::CreateInt32(hExec->hVm, ~hSrc->as.int32);   break;
			case HQ_VALUE_TYPE_INT64:  hOutput = HqValue::CreateInt64(hExec->hVm, ~hSrc->as.int64);   break;
			case HQ_VALUE_TYPE_UINT8:  hOutput = HqValue::CreateUint8(hExec->hVm, ~hSrc->as.uint8);   break;
			case HQ_VALUE_TYPE_UINT16: hOutput = HqValue::CreateUint16(hExec->hVm, ~hSrc->as.uint16); break;
			case HQ_VALUE_TYPE_UINT32: hOutput = HqValue::CreateUint32(hExec->hVm, ~hSrc->as.uint32); break;
			case HQ_VALUE_TYPE_UINT64: hOutput = HqValue::CreateUint64(hExec->hVm, ~hSrc->as.uint64); break;
			case HQ_VALUE_TYPE_BOOL:   hOutput = HqValue::CreateBool(hExec->hVm, !hSrc->as.boolean);  break;

			default:
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec, 
					HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
					"Type mismatch; expected integer or boolean type: r(%" PRIu32 ")", 
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

extern "C" void OpCodeDisasm_BitNot(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "NOT r%" PRIu32 ", r%" PRIu32, gpDstRegIndex, gpSrcRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_BitNot(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
}

//----------------------------------------------------------------------------------------------------------------------
