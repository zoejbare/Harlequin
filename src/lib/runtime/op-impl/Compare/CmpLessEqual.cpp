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

#include "CmpUtil.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Compare two general-purpose register values against each other to see if one is less than
// or equal to the other, storing a new boolean value in the destination general-purpose register.
// 
// NOTE: Only primitive comparisons are supported, meaning comparison of anything that
//       is not boolean, integer, or floating point type values will be rejected.
//
// 0x: CMP_LE r#, r#, r#
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the left-hand operand value
//   r# [third]  = General-purpose register index containing the right-hand operand value
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_CompareLessEqual(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcLeftRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRightRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hLeft = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcLeftRegIndex, &result);
	if(result == HQ_SUCCESS)
	{
		HqValueHandle hRight = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRightRegIndex, &result);
		if(result == HQ_SUCCESS)
		{
			if((!hLeft && !hRight) || (hLeft == hRight))
			{
				// Both operands are considered equal since they point to the same memory.
				CmpUtil::SetResult(hExec, gpDstRegIndex, true);
			}
			else
			{
				// Verify both operands reference the same type.
				if(hLeft && hRight && hLeft->type == hRight->type)
				{
					bool cmpResult = false;

					switch(hLeft->type)
					{
						case HQ_VALUE_TYPE_INT8:     cmpResult = (hLeft->as.int8 <= hRight->as.int8);                                 break;
						case HQ_VALUE_TYPE_INT16:    cmpResult = (hLeft->as.int16 <= hRight->as.int16);                               break;
						case HQ_VALUE_TYPE_INT32:    cmpResult = (hLeft->as.int32 <= hRight->as.int32);                               break;
						case HQ_VALUE_TYPE_INT64:    cmpResult = (hLeft->as.int64 <= hRight->as.int64);                               break;
						case HQ_VALUE_TYPE_UINT8:    cmpResult = (hLeft->as.uint8 <= hRight->as.uint8);                               break;
						case HQ_VALUE_TYPE_UINT16:   cmpResult = (hLeft->as.uint16 <= hRight->as.uint16);                             break;
						case HQ_VALUE_TYPE_UINT32:   cmpResult = (hLeft->as.uint32 <= hRight->as.uint32);                             break;
						case HQ_VALUE_TYPE_UINT64:   cmpResult = (hLeft->as.uint64 <= hRight->as.uint64);                             break;
						case HQ_VALUE_TYPE_FLOAT32:  cmpResult = (hLeft->as.float32 <= hRight->as.float32);                           break;
						case HQ_VALUE_TYPE_FLOAT64:  cmpResult = (hLeft->as.float64 <= hRight->as.float64);                           break;
						case HQ_VALUE_TYPE_BOOL:     cmpResult = (hLeft->as.boolean <= hRight->as.boolean);                           break;
						case HQ_VALUE_TYPE_STRING:   cmpResult = (HqString::SlowCompare(hLeft->as.pString, hRight->as.pString) <= 0); break;
						case HQ_VALUE_TYPE_FUNCTION: cmpResult = (hLeft->as.hFunction <= hRight->as.hFunction);                       break;
						case HQ_VALUE_TYPE_NATIVE:   cmpResult = (hLeft->as.native.pObject <= hRight->as.native.pObject);             break;

						case HQ_VALUE_TYPE_OBJECT:
						case HQ_VALUE_TYPE_ARRAY:
							// These types are implicitly non-equal because there is no way for two separate
							// values their types to reference the same underlying native pointers.
							break;

						default:
							// This should never happen.
							assert(false);
							break;
					}

					// Set the comparison result.
					CmpUtil::SetResult(hExec, gpDstRegIndex, cmpResult);
				}
				else
				{
					// Raise a fatal script exception.
					HqExecution::RaiseOpCodeException(
						hExec, 
						HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
						"Type mismatch; cannot compare register operands of different types: r(%" PRIu32 "), r(%" PRIu32 ")",
						gpSrcLeftRegIndex,
						gpSrcRightRegIndex
					);
				}
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

extern "C" void OpCodeDisasm_CompareLessEqual(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcLeftRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRightRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "CMP_LE r(%" PRIu32 "), r(%" PRIu32 "), r(%" PRIu32 ")", gpDstRegIndex, gpSrcLeftRegIndex, gpSrcRightRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_CompareLessEqual(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
	HqDecoder::EndianSwapUint32(decoder); // r# [third]
}

//----------------------------------------------------------------------------------------------------------------------
