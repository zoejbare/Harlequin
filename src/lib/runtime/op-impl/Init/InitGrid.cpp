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
#include "../../Module.hpp"
#include "../../Vm.hpp"

#include <stdio.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Initialize a new grid, storing it in a general-purpose register.
//
// 0x: INIT_GRID r#, ##, ##, ##
//
//   r# = General-purpose register where the new grid will be stored
//   ## = Immediate integer for the grid X length
//   ## = Immediate integer for the grid Y length
//   ## = Immediate integer for the grid Z length
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_InitGrid(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t lengthX = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t lengthY = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t lengthZ = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hGrid = HqValue::CreateGrid(hExec->hVm, size_t(lengthX), size_t(lengthY), size_t(lengthZ));
	if(HqValueIsGrid(hGrid))
	{
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hGrid, registerIndex);
		if(result != HQ_SUCCESS)
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec, 
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR, 
				"Failed to set general-purpose register: r(%" PRIu32 ")", 
				registerIndex
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec, 
			HQ_STANDARD_EXCEPTION_RUNTIME_ERROR, 
			"Failed to create grid value"
		);

	}

	HqValue::SetAutoMark(hGrid, false);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_InitGrid(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t lengthX = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t lengthY = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t lengthZ = HqDecoder::LoadUint32(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "INIT_GRID r(%" PRIu32 "), %" PRIu32 ", %" PRIu32 ", %" PRIu32, registerIndex, lengthX, lengthY, lengthZ);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_InitGrid(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r#
	HqDecoder::EndianSwapUint32(decoder); // ##
	HqDecoder::EndianSwapUint32(decoder); // ##
	HqDecoder::EndianSwapUint32(decoder); // ##
}

//----------------------------------------------------------------------------------------------------------------------
