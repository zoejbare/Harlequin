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
#include "../../Function.hpp"
#include "../../Module.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Load a value from a general-purpose register, then cast it to a string value, storing the result in a general-purpose register.
//
// 0x: CAST_STR r#, r#
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the value to be cast to the new type
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_CastString(HqExecutionHandle hExec)
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
			case HQ_VALUE_TYPE_INT8:
			{
				HqString* const pOutputString = HqString::CreateFmt("%" PRId8, hSource->as.int8);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_INT16:
			{
				HqString* const pOutputString = HqString::CreateFmt("%" PRId16, hSource->as.int16);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_INT32:
			{
				HqString* const pOutputString = HqString::CreateFmt("%" PRId32, hSource->as.int32);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_INT64:
			{
				HqString* const pOutputString = HqString::CreateFmt("%" PRId64, hSource->as.int64);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_UINT8:
			{
				HqString* const pOutputString = HqString::CreateFmt("%" PRIu8, hSource->as.uint8);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_UINT16:
			{
				HqString* const pOutputString = HqString::CreateFmt("%" PRIu16, hSource->as.uint16);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_UINT32:
			{
				HqString* const pOutputString = HqString::CreateFmt("%" PRIu32, hSource->as.uint32);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_UINT64:
			{
				HqString* const pOutputString = HqString::CreateFmt("%" PRIu64, hSource->as.uint64);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_FLOAT32:
			{
				HqString* const pOutputString = HqString::CreateFmt("%f", hSource->as.float32);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_FLOAT64:
			{
				HqString* const pOutputString = HqString::CreateFmt("%f", hSource->as.float64);
				hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
				HqString::Release(pOutputString);
				break;
			}

			case HQ_VALUE_TYPE_BOOL:
			{
				hOutput = HqValue::CreateString(hExec->hVm, hSource->as.boolean ? "true" : "false");
				break;
			}


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

extern "C" void OpCodeDisasm_CastString(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[256];
	snprintf(str, sizeof(str), "CAST_STR r%" PRIu32 ", r%" PRIu32, gpDstRegIndex, gpSrcRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_CastString(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
}

//----------------------------------------------------------------------------------------------------------------------
