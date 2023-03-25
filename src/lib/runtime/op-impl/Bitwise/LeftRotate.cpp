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
// Perform a bitwise left rotation on an integer value.
//
// 0x: LROT r#, r#, r#
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the left-hand operand value
//   r# [third]  = General-purpose register index containing the right-hand operand value
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_LeftRotate(HqExecutionHandle hExec)
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
					case HQ_VALUE_TYPE_INT8:
					{
						constexpr int8_t bitCount = 8;
						const int8_t shiftAmount = hRight->as.int8 % bitCount;
						const int8_t rotateAmount = bitCount - shiftAmount;
						const int8_t value = hLeft->as.int8;

						hOutput = HqValue::CreateInt8(hExec->hVm, (value << shiftAmount) | (value >> rotateAmount));
						break;
					}

					case HQ_VALUE_TYPE_INT16:
					{
						constexpr int16_t bitCount = 16;
						const int16_t shiftAmount = hRight->as.int16 % bitCount;
						const int16_t rotateAmount = bitCount - shiftAmount;
						const int16_t value = hLeft->as.int16;

						hOutput = HqValue::CreateInt16(hExec->hVm, (value << shiftAmount) | (value >> rotateAmount));
						break;
					}

					case HQ_VALUE_TYPE_INT32:
					{
						constexpr int32_t bitCount = 32;
						const int32_t shiftAmount = hRight->as.int32 % bitCount;
						const int32_t rotateAmount = bitCount - shiftAmount;
						const int32_t value = hLeft->as.int32;

						hOutput = HqValue::CreateInt32(hExec->hVm, (value << shiftAmount) | (value >> rotateAmount));
						break;
					}

					case HQ_VALUE_TYPE_INT64:
					{
						constexpr int64_t bitCount = 64;
						const int64_t shiftAmount = hRight->as.int64 % bitCount;
						const int64_t rotateAmount = bitCount - shiftAmount;
						const int64_t value = hLeft->as.int64;

						hOutput = HqValue::CreateInt64(hExec->hVm, (value << shiftAmount) | (value >> rotateAmount));
						break;
					}

					case HQ_VALUE_TYPE_UINT8:
					{
						constexpr uint8_t bitCount = 8;
						const uint8_t shiftAmount = hRight->as.uint8 % bitCount;
						const uint8_t rotateAmount = bitCount - shiftAmount;
						const uint8_t value = hLeft->as.uint8;

						hOutput = HqValue::CreateUint8(hExec->hVm, (value << shiftAmount) | (value >> rotateAmount));
						break;
					}

					case HQ_VALUE_TYPE_UINT16:
					{
						constexpr uint16_t bitCount = 16;
						const uint16_t shiftAmount = hRight->as.uint16 % bitCount;
						const uint16_t rotateAmount = bitCount - shiftAmount;
						const uint16_t value = hLeft->as.uint16;

						hOutput = HqValue::CreateUint16(hExec->hVm, (value << shiftAmount) | (value >> rotateAmount));
						break;
					}

					case HQ_VALUE_TYPE_UINT32:
					{
						constexpr uint32_t bitCount = 32;
						const uint32_t shiftAmount = hRight->as.uint32 % bitCount;
						const uint32_t rotateAmount = bitCount - shiftAmount;
						const uint32_t value = hLeft->as.uint32;

						hOutput = HqValue::CreateUint32(hExec->hVm, (value << shiftAmount) | (value >> rotateAmount));
						break;
					}

					case HQ_VALUE_TYPE_UINT64:
					{
						constexpr uint64_t bitCount = 64;
						const uint64_t shiftAmount = hRight->as.uint64 % bitCount;
						const uint64_t rotateAmount = bitCount - shiftAmount;
						const uint64_t value = hLeft->as.uint64;

						hOutput = HqValue::CreateUint64(hExec->hVm, (value << shiftAmount) | (value >> rotateAmount));
						break;
					}

					default:
					{
						// Raise a fatal script exception.
						HqExecution::RaiseOpCodeException(
							hExec, 
							HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
							"Type mismatch; expected integer type: r(%" PRIu32 ")", 
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

extern "C" void OpCodeDisasm_LeftRotate(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcLeftRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRightRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "LROT r(%" PRIu32 "), r(%" PRIu32 "), r(%" PRIu32 ")", gpDstRegIndex, gpSrcLeftRegIndex, gpSrcRightRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_LeftRotate(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
	HqDecoder::EndianSwapUint32(decoder); // r# [third]
}

//----------------------------------------------------------------------------------------------------------------------
