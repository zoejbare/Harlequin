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

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Load two values from general-purpose registers and divide one by the other.
//
// 0x: DIV r#, r#, r#
//
//   r# [first]  = General-purpose register index where the result will be stored
//   r# [second] = General-purpose register index containing the left-hand operand value
//   r# [third]  = General-purpose register index containing the right-hand operand value
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_Div(HqExecutionHandle hExec)
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
						if(hRight->as.int8 == 0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [int8]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateInt8(hExec->hVm, hLeft->as.int8 / hRight->as.int8);
						break;

					case HQ_VALUE_TYPE_INT16:
						if(hRight->as.int16 == 0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [int16]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateInt16(hExec->hVm, hLeft->as.int16 / hRight->as.int16);
						break;

					case HQ_VALUE_TYPE_INT32:
						if(hRight->as.int32 == 0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [int32]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateInt32(hExec->hVm, hLeft->as.int32 / hRight->as.int32);
						break;

					case HQ_VALUE_TYPE_INT64:
						if(hRight->as.int64 == 0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [int64]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateInt64(hExec->hVm, hLeft->as.int64 / hRight->as.int64);
						break;

					case HQ_VALUE_TYPE_UINT8:
						if(hRight->as.uint8 == 0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [uint8]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateUint8(hExec->hVm, hLeft->as.uint8 / hRight->as.uint8);
						break;

					case HQ_VALUE_TYPE_UINT16:
						if(hRight->as.uint16 == 0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [uint16]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateUint16(hExec->hVm, hLeft->as.uint16 / hRight->as.uint16);
						break;

					case HQ_VALUE_TYPE_UINT32:
						if(hRight->as.uint32 == 0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [uint32]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateUint32(hExec->hVm, hLeft->as.uint32 / hRight->as.uint32);
						break;

					case HQ_VALUE_TYPE_UINT64:
						if(hRight->as.uint64 == 0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [uint64]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateUint64(hExec->hVm, hLeft->as.uint64 / hRight->as.uint64);
						break;

					case HQ_VALUE_TYPE_FLOAT32:
						if(hRight->as.float32 == 0.0f)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [float32]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateFloat32(hExec->hVm, hLeft->as.float32 / hRight->as.float32);
						break;

					case HQ_VALUE_TYPE_FLOAT64:
						if(hRight->as.float64 == 0.0)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [float64]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateFloat64(hExec->hVm, hLeft->as.float64 / hRight->as.float64);
						break;

					case HQ_VALUE_TYPE_BOOL:
						if(!hRight->as.boolean)
						{
							// Raise a fatal script exception.
							HqExecution::RaiseOpCodeException(
								hExec,
								HQ_STANDARD_EXCEPTION_DIVIDE_BY_ZERO_ERROR,
								"Divide-by-zero error [bool]: r(%" PRIu32 ")",
								gpSrcRightRegIndex
							);
							return;
						}

						hOutput = HqValue::CreateBool(hExec->hVm, hLeft->as.boolean);
						break;

					default:
					{
						// Raise a fatal script exception.
						HqExecution::RaiseOpCodeException(
							hExec, 
							HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
							"Type mismatch; expected primitive type: r(%" PRIu32 ")", 
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

extern "C" void OpCodeDisasm_Div(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcLeftRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRightRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[256];
	snprintf(str, sizeof(str), "DIV r(%" PRIu32 "), r(%" PRIu32 "), r(%" PRIu32 ")", gpDstRegIndex, gpSrcLeftRegIndex, gpSrcRightRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_Div(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r# [first]
	HqDecoder::EndianSwapUint32(decoder); // r# [second]
	HqDecoder::EndianSwapUint32(decoder); // r# [third]
}

//----------------------------------------------------------------------------------------------------------------------
