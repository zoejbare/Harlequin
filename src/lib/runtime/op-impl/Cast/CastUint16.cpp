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
#include "../../Function.hpp"
#include "../../Module.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Load a value from a general-purpose register, then cast it to a uint16 value, storing the result in a general-purpose register.
//
// 0x: CAST_U16 r#, r#
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the value to be cast to the new type
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_CastUint16(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hSource = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
	if(hSource)
	{
		HqValueHandle hOutput = HQ_VALUE_HANDLE_NULL;

		switch(hSource->type)
		{
			case HQ_VALUE_TYPE_INT8:    hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.int8));    break;
			case HQ_VALUE_TYPE_INT16:   hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.int16));   break;
			case HQ_VALUE_TYPE_INT32:   hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.int32));   break;
			case HQ_VALUE_TYPE_INT64:   hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.int64));   break;
			case HQ_VALUE_TYPE_UINT8:   hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.uint8));   break;
			case HQ_VALUE_TYPE_UINT16:  hOutput = hSource;                                                          break;
			case HQ_VALUE_TYPE_UINT32:  hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.uint32));  break;
			case HQ_VALUE_TYPE_UINT64:  hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.uint64));  break;
			case HQ_VALUE_TYPE_FLOAT32: hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.float32)); break;
			case HQ_VALUE_TYPE_FLOAT64: hOutput = HqValue::CreateUint16(hExec->hVm, uint16_t(hSource->as.float64)); break;
			case HQ_VALUE_TYPE_BOOL:    hOutput = HqValue::CreateUint16(hExec->hVm, hSource->as.boolean ? 1 : 0);   break;

			default:
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec, 
					HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
					"Type mismatch; expected primitive type: r(%" PRIu32 ")", 
					gpSrcRegIndex
				);
				return;
			}
		}

		if(hOutput)
		{
			if(hSource->type != HQ_VALUE_TYPE_UINT16)
			{
				// Remove the auto-mark from the output value so it can be cleaned up when it's no longer referenced.
				HqValue::SetAutoMark(hOutput, false);
			}

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

extern "C" void OpCodeDisasm_CastUint16(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[256];
	snprintf(str, sizeof(str), "CAST_U16 r(%" PRIu32 "), r(%" PRIu32 ")", gpDstRegIndex, gpSrcRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_CastUint16(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
}

//----------------------------------------------------------------------------------------------------------------------
