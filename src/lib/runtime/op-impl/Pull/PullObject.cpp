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
#include "../../Frame.hpp"
#include "../../Object.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Dual operation to load a member variable from an object value into a general-purpose register, then clear the variable.
//
// 0x: PULL_OBJECT r#, r#, ##
//
//   r# [first]  = General-purpose register index where the extracted member value will be stored
//   r# [second] = General-purpose register index that contains the object where the member resides
//   ##          = Immediate integer used as the direct index into the object's member table
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_PullObject(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t gpDstRegIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t gpSrcRegIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t memberIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	// Load the object value from the source register.
	XenonValueHandle hSource = XenonFrame::GetGpRegister(hExec->hCurrentFrame, gpSrcRegIndex, &result);
	if(result == XENON_SUCCESS)
	{
		// Verify the loaded value is an object type.
		if(XenonValueIsObject(hSource))
		{
			XenonObject* const pScriptObject = hSource->as.pObject;

			// Load the member variable of the object value from the source index.
			XenonValueHandle hMember = XenonObject::GetMemberValue(pScriptObject, memberIndex, &result);
			if(result == XENON_SUCCESS)
			{
				// Store the variable's value in the destination register.
				result = XenonFrame::SetGpRegister(hExec->hCurrentFrame, hMember, gpDstRegIndex);
				if(result == XENON_SUCCESS)
				{
					// Clear the variable.
					result = XenonObject::SetMemberValue(pScriptObject, memberIndex, XENON_VALUE_HANDLE_NULL);
					if(result != XENON_SUCCESS)
					{
						// TODO: Raise script exception
						hExec->exception = true;
					}
				}
				else
				{
					// TODO: Raise script exception
					hExec->exception = true;
				}
			}
			else
			{
				// TODO: Raise script exception
				hExec->exception = true;
			}
		}
		else
		{
			// TODO: Raise script exception
			hExec->exception = true;
		}
	}
	else
	{
		// TODO: Raise script exception
		hExec->exception = true;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_PullObject(XenonDisassemble& disasm)
{
	const uint32_t gpDstRegIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t gpSrcRegIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t memberIndex = XenonDecoder::LoadUint32(disasm.decoder);

	char str[64];
	snprintf(str, sizeof(str), "PULL_OBJECT r%" PRIu32 ", r%" PRIu32 ", #%" PRIu32, gpDstRegIndex, gpSrcRegIndex, memberIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
