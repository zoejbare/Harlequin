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

#include "../OpDecl.hpp"

#include "../Decoder.hpp"
#include "../Execution.hpp"
#include "../Function.hpp"
#include "../Program.hpp"

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Move the instruction pointer by a specified offset. Offset cannot exceed the bounds of the current function.
// The conditional branch instructions will depend on the boolean evaluation of a specified register.
//
// 0x: BRANCH #
//   # - Relative bytecode offset
//
// 0x: BRANCH_IF_TRUE r#, #
//   r# - Register to evaluate
//   #  - Relative bytecode offset
//
// 0x: BRANCH_IF_FALSE r#, #
//   r# - Register to evaluate
//   #  - Relative bytecode offset
//
//----------------------------------------------------------------------------------------------------------------------

static void MoveInstructionPointer(XenonExecutionHandle hExec, const int32_t relativeOffset)
{
	XenonFunctionHandle hFunction = hExec->hCurrentFrame->hFunction;
	XenonProgramHandle hProgram = hExec->hCurrentFrame->hFunction->hProgram;

	uint8_t* const pNewIp = hExec->hCurrentFrame->decoder.cachedIp + relativeOffset;

	const uint8_t* const pFunctionStart = hProgram->code.pData + hFunction->bytecodeOffsetStart;
	const uint8_t* const pFunctionEnd = hProgram->code.pData + hFunction->bytecodeOffsetEnd;

	// Verify the new instruction pointer falls within the bounds of the current function.
	if(pNewIp < pFunctionStart || pNewIp >= pFunctionEnd)
	{
		// TODO: Raise script exception
		hExec->exception = true;
	}
	else
	{
		hExec->hCurrentFrame->decoder.cachedIp = pNewIp;
		hExec->hCurrentFrame->decoder.ip = pNewIp;
	}
}

//----------------------------------------------------------------------------------------------------------------------

static bool EvaluateValue(XenonValueHandle hValue)
{
	if(!hValue)
	{
		return false;
	}

	switch(hValue->type)
	{
		case XENON_VALUE_TYPE_NULL:
			return false;

		case XENON_VALUE_TYPE_INT8:
			return hValue->as.int8 != 0;

		case XENON_VALUE_TYPE_INT16:
			return hValue->as.int16 != 0;

		case XENON_VALUE_TYPE_INT32:
			return hValue->as.int32 != 0;

		case XENON_VALUE_TYPE_INT64:
			return hValue->as.int64 != 0;

		case XENON_VALUE_TYPE_UINT8:
			return hValue->as.uint8 != 0;

		case XENON_VALUE_TYPE_UINT16:
			return hValue->as.uint16 != 0;

		case XENON_VALUE_TYPE_UINT32:
			return hValue->as.uint32 != 0;

		case XENON_VALUE_TYPE_UINT64:
			return hValue->as.uint64 != 0;

		case XENON_VALUE_TYPE_FLOAT32:
			return hValue->as.float32 != 0;

		case XENON_VALUE_TYPE_FLOAT64:
			return hValue->as.float64 != 0;

		case XENON_VALUE_TYPE_BOOL:
			return hValue->as.boolean;

		case XENON_VALUE_TYPE_STRING:
			return hValue->as.pString->length != 0;

		default:
			assert(false);
			break;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_Branch(XenonExecutionHandle hExec)
{
	const int32_t offset = XenonDecoder::LoadInt32(hExec->hCurrentFrame->decoder);

	// No condition, just move the instruction pointer.
	MoveInstructionPointer(hExec, offset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_Branch(XenonDisassemble& disasm)
{
	const int32_t offset = XenonDecoder::LoadInt32(disasm.decoder);

	char str[32];
	snprintf(str, sizeof(str), "BRANCH %" PRId32, offset);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_BranchIfTrue(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const int32_t offset = XenonDecoder::LoadInt32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hValue = XenonFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
	if(result == XENON_SUCCESS)
	{
		// Object values cannot be evaluated directly.
		if(!XenonValueIsObject(hValue))
		{
			const bool pass = EvaluateValue(hValue);

			if(pass)
			{
				MoveInstructionPointer(hExec, offset);
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

void OpCodeDisasm_BranchIfTrue(XenonDisassemble& disasm)
{
	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const int32_t offset = XenonDecoder::LoadInt32(disasm.decoder);

	char str[32];
	snprintf(str, sizeof(str), "BRANCH_IF_TRUE r%" PRIu32 ", %" PRId32, registerIndex, offset);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_BranchIfFalse(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const int32_t offset = XenonDecoder::LoadInt32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hValue = XenonFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
	if(result == XENON_SUCCESS)
	{
		// Object values cannot be evaluated directly.
		if(!XenonValueIsObject(hValue))
		{
			const bool pass = !EvaluateValue(hValue);

			if(pass)
			{
				MoveInstructionPointer(hExec, offset);
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

void OpCodeDisasm_BranchIfFalse(XenonDisassemble& disasm)
{
	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);
	const int32_t offset = XenonDecoder::LoadInt32(disasm.decoder);

	char str[32];
	snprintf(str, sizeof(str), "BRANCH_IF_FALSE r%" PRIu32 ", %" PRId32, registerIndex, offset);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
