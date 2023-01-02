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

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Load two values from general-purpose registers and add them together.
//
// 0x: ADD r#, r#, r#
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the left-hand operand value
//   r# [third]  = General-purpose register index containing the right-hand operand value
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_Add(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcLeftRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRightRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hLeft = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcLeftRegIndex, &result);
	if(hLeft)
	{
		HqValueHandle hRight = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRightRegIndex, &result);
		if(hRight)
		{
			if(hLeft->type == hRight->type)
			{
				HqValueHandle hOutput = HQ_VALUE_HANDLE_NULL;

				switch(hLeft->type)
				{
					case HQ_VALUE_TYPE_INT8:    hOutput = HqValue::CreateInt8(hExec->hVm, hLeft->as.int8 + hRight->as.int8);          break;
					case HQ_VALUE_TYPE_INT16:   hOutput = HqValue::CreateInt16(hExec->hVm, hLeft->as.int16 + hRight->as.int16);       break;
					case HQ_VALUE_TYPE_INT32:   hOutput = HqValue::CreateInt32(hExec->hVm, hLeft->as.int32 + hRight->as.int32);       break;
					case HQ_VALUE_TYPE_INT64:   hOutput = HqValue::CreateInt64(hExec->hVm, hLeft->as.int64 + hRight->as.int64);       break;
					case HQ_VALUE_TYPE_UINT8:   hOutput = HqValue::CreateUint8(hExec->hVm, hLeft->as.uint8 + hRight->as.uint8);       break;
					case HQ_VALUE_TYPE_UINT16:  hOutput = HqValue::CreateUint16(hExec->hVm, hLeft->as.uint16 + hRight->as.uint16);    break;
					case HQ_VALUE_TYPE_UINT32:  hOutput = HqValue::CreateUint32(hExec->hVm, hLeft->as.uint32 + hRight->as.uint32);    break;
					case HQ_VALUE_TYPE_UINT64:  hOutput = HqValue::CreateUint64(hExec->hVm, hLeft->as.uint64 + hRight->as.uint64);    break;
					case HQ_VALUE_TYPE_FLOAT32: hOutput = HqValue::CreateFloat32(hExec->hVm, hLeft->as.float32 + hRight->as.float32); break;
					case HQ_VALUE_TYPE_FLOAT64: hOutput = HqValue::CreateFloat64(hExec->hVm, hLeft->as.float64 + hRight->as.float64); break;
					case HQ_VALUE_TYPE_BOOL:    hOutput = HqValue::CreateBool(hExec->hVm, hLeft->as.boolean + hRight->as.boolean);    break;

					case HQ_VALUE_TYPE_STRING: 
					{
						HqString* const pOutputString = HqString::CreateFmt("%s%s", hLeft->as.pString->data, hRight->as.pString->data);

						hOutput = HqValue::CreateString(hExec->hVm, pOutputString);
						HqString::Release(pOutputString);
						break;
					}

					case HQ_VALUE_TYPE_ARRAY:
					{
						const size_t leftCount = hLeft->as.array.count;
						const size_t rightCount = hRight->as.array.count;

						hOutput = HqValue::CreateArray(hExec->hVm, leftCount + rightCount);

						size_t index = 0;

						// Shallow copy the elements from the left-hand array operand.
						for(; index < leftCount; ++index)
						{
							hOutput->as.array.pData[index] = hLeft->as.array.pData[index];
						}

						// Shallow copy the elements from the right-hand array operand.
						for(size_t endIndex = leftCount + rightCount; index < endIndex; ++index)
						{
							hOutput->as.array.pData[index] = hRight->as.array.pData[index - leftCount];
						}
						break;
					}

					default:
					{
						// Raise a fatal script exception.
						HqExecution::RaiseOpCodeException(
							hExec, 
							HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
							"Type mismatch; expected primitive type, array, or string: r(%" PRIu32 ")", 
							gpSrcLeftRegIndex
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
					HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
					"Type mismatch; operand registers contain values of differing types: r(%" PRIu32 "), r(%" PRIu32 ")", 
					gpSrcLeftRegIndex,
					gpSrcRightRegIndex
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
				gpSrcRightRegIndex
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
			gpSrcLeftRegIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_Add(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcLeftRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRightRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[256];
	snprintf(str, sizeof(str), "ADD r%" PRIu32 ", r%" PRIu32 ", r%" PRIu32, gpDstRegIndex, gpSrcLeftRegIndex, gpSrcRightRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_Add(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
	HqDecoder::EndianSwapUint32(decoder); // r# [third]
}

//----------------------------------------------------------------------------------------------------------------------
