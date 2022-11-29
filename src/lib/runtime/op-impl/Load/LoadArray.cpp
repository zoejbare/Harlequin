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
// Load a value by index from an array into a general-purpose register.
//
// 0x: LOAD_ARRAY r#, r#, ##
//
//   r# [first]  = General-purpose register index where the extracted member value will be stored
//   r# [second] = General-purpose register index that contains the object where the member resides
//   ##          = Immediate integer used as the direct index into the array
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_LoadArray(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t arrayIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	// Load the array value from the source register.
	XenonValueHandle hSource = XenonFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
	if(result == XENON_SUCCESS)
	{
		// Verify the loaded value is an array type.
		if(XenonValueIsArray(hSource))
		{
			if(size_t(arrayIndex) < hSource->as.array.count)
			{
				// Load the element in the array at the source index.
				XenonValueHandle hElement = hSource->as.array.pData[arrayIndex];

				// Store the element in the destination register.
				result = XenonFrame::SetGpRegister(hExec->hCurrentFrame, hElement, gpDstRegIndex);
				if(result != XENON_SUCCESS)
				{
					// Raise a fatal script exception.
					XenonExecution::RaiseOpCodeException(
						hExec, 
						XENON_STANDARD_EXCEPTION_RUNTIME_ERROR, 
						"Failed to set general-purpose register: r(%" PRIu32 ")", 
						gpDstRegIndex
					);
				}
			}
			else
			{
				// Raise a fatal script exception.
				XenonExecution::RaiseOpCodeException(
					hExec, 
					XENON_STANDARD_EXCEPTION_RUNTIME_ERROR, 
					"Array index out of range: r(%" PRIu32 "), length=%zu, index=%" PRIu32,
					gpSrcRegIndex,
					hSource->as.array.count,
					arrayIndex
				);
			}
		}
		else
		{
			// Raise a fatal script exception.
			XenonExecution::RaiseOpCodeException(
				hExec, 
				XENON_STANDARD_EXCEPTION_TYPE_ERROR, 
				"Type mismatch; expected array: r(%" PRIu32 ")", 
				gpSrcRegIndex
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		XenonExecution::RaiseOpCodeException(
			hExec, 
			XENON_STANDARD_EXCEPTION_RUNTIME_ERROR, 
			"Failed to retrieve general-purpose register: r(%" PRIu32 ")", 
			gpSrcRegIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadArray(XenonDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t arrayIndex = XenonDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "LOAD_ARRAY r%" PRIu32 ", r%" PRIu32 ", #%" PRIu32, gpDstRegIndex, gpSrcRegIndex, arrayIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
