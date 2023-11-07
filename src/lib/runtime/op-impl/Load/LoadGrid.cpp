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
// Load a value by index from an grid into a general-purpose register.
//
// 0x: LOAD_GRID r#, r#, r#, r#, r#
//
//   r# [first]  = General-purpose register index where the extracted member value will be stored
//   r# [second] = General-purpose register index that contains the grid where the desired element resides
//   r# [third]  = General-purpose register index that contains the grid X index
//   r# [fourth] = General-purpose register index that contains the grid Y index
//   r# [fifth]  = General-purpose register index that contains the grid Z index
//
//----------------------------------------------------------------------------------------------------------------------

inline bool _HqExtractGridIndexFromRegister(
	HqExecutionHandle hExec,
	const char* const dimensionLabel,
	const uint32_t registerIndex,
	const size_t gridLength,
	size_t& outputGridIndex)
{
	int result;
	HqValueHandle hIndexValue = HqFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
	if(result != HQ_SUCCESS)
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec,
			HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
			"Failed to retrieve general-purpose regsiter: r(%" PRIu32 ")",
			registerIndex
		);
		return false;
	}

	switch(hIndexValue->type)
	{
		case HQ_VALUE_TYPE_INT8:   outputGridIndex = size_t(hIndexValue->as.int8);   break;
		case HQ_VALUE_TYPE_INT16:  outputGridIndex = size_t(hIndexValue->as.int16);  break;
		case HQ_VALUE_TYPE_INT32:  outputGridIndex = size_t(hIndexValue->as.int32);  break;
		case HQ_VALUE_TYPE_INT64:  outputGridIndex = size_t(hIndexValue->as.int64);  break;
		case HQ_VALUE_TYPE_UINT8:  outputGridIndex = size_t(hIndexValue->as.uint8);  break;
		case HQ_VALUE_TYPE_UINT16: outputGridIndex = size_t(hIndexValue->as.uint16); break;
		case HQ_VALUE_TYPE_UINT32: outputGridIndex = size_t(hIndexValue->as.uint32); break;
		case HQ_VALUE_TYPE_UINT64: outputGridIndex = size_t(hIndexValue->as.uint64); break;

		default:
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec, 
				HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
				"Type mismatch; expected integer: r(%" PRIu32 ")", 
				registerIndex
			);
			return false;
	}

	if(outputGridIndex >= gridLength)
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec,
			HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
			"Grid %s index out of range: r(%" PRIu32 "), length=%zu, index=%" PRIu32,
			dimensionLabel,
			registerIndex,
			gridLength,
			outputGridIndex
		);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_LoadGrid(HqExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpGridIdxXRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpGridIdxYRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpGridIdxZRegIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	// Load the grid value from the source register.
	HqValueHandle hSource = HqFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
	if(result == HQ_SUCCESS)
	{
		// Verify the loaded value is an grid type.
		if(HqValueIsGrid(hSource))
		{
			size_t gridIndexX = 0;
			size_t gridIndexY = 0;
			size_t gridIndexZ = 0;

			// Extract the raw values for the grid indices.
			if(_HqExtractGridIndexFromRegister(hExec, "X", gpGridIdxXRegIndex, hSource->as.grid.lengthX, gridIndexX)
				&& _HqExtractGridIndexFromRegister(hExec, "Y", gpGridIdxYRegIndex, hSource->as.grid.lengthY, gridIndexY)
				&& _HqExtractGridIndexFromRegister(hExec, "Z", gpGridIdxZRegIndex, hSource->as.grid.lengthZ, gridIndexZ))
			{
				const size_t finalGridIndex = HqValue::CalculateGridIndex(
					hSource->as.grid.lengthX,
					hSource->as.grid.lengthY,
					gridIndexX,
					gridIndexY,
					gridIndexZ);

				// Load the element in the grid at the source index.
				HqValueHandle hElement = hSource->as.grid.array.pData[finalGridIndex];

				// Store the element in the destination register.
				result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hElement, gpDstRegIndex);
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
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec, 
				HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
				"Type mismatch; expected grid: r(%" PRIu32 ")", 
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
			"Failed to retrieve general-purpose register: r(%" PRIu32 ")", 
			gpSrcRegIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_LoadGrid(HqDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpGridIdxXRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpGridIdxYRegIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpGridIdxZRegIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[96];
	snprintf(str, sizeof(str), "LOAD_GRID r(%" PRIu32 "), r(%" PRIu32 "), r(%" PRIu32 "), r(%" PRIu32 "), r(%" PRIu32 ")", gpDstRegIndex, gpSrcRegIndex, gpGridIdxXRegIndex, gpGridIdxYRegIndex, gpGridIdxZRegIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_LoadGrid(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r#
	HqDecoder::EndianSwapUint32(decoder); // r#
	HqDecoder::EndianSwapUint32(decoder); // r#
	HqDecoder::EndianSwapUint32(decoder); // r#
	HqDecoder::EndianSwapUint32(decoder); // r#
}

//----------------------------------------------------------------------------------------------------------------------
