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
#include "../../Program.hpp"
#include "../../Vm.hpp"

#include <stdio.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Initialize a new array, storing it in a general-purpose register.
//
// 0x: INIT_ARRAY r#, ##
//
//   r# = General-purpose register where the new array will be stored.
//   ## = Literal integer for the initial size of the array.
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_InitArray(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t initialCount = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hArray = XenonValue::CreateArray(hExec->hVm, size_t(initialCount));
	if(XenonValueIsArray(hArray))
	{
		result = XenonFrame::SetGpRegister(hExec->hCurrentFrame, hArray, registerIndex);
		if(result != XENON_SUCCESS)
		{
			// Raise a fatal script exception.
			XenonExecutionRaiseStandardException(
				hExec,
				XENON_EXCEPTION_SEVERITY_FATAL,
				XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Failed to set general-purpose register: r(%" PRIu32 ")",
				registerIndex
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		XenonExecutionRaiseStandardException(
			hExec,
			XENON_EXCEPTION_SEVERITY_FATAL,
			XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
			"Failed to create array value"
		);
	}

	XenonValueAbandon(hArray);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_InitArray(XenonDisassemble& disasm)
{
	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const uint32_t initialCount = XenonDecoder::LoadUint32(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "INIT_ARRAY r%" PRIu32 ", #%" PRIu32, registerIndex, initialCount);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
