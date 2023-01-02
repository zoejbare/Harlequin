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
#include "../Module.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Move the instruction pointer by a specified offset. Offset cannot exceed the bounds of the current function.
// The conditional jump instructions will depend on the boolean evaluation of a specified register.
//
// 0x: JMP ##
//   ## - Signed offset relative to the bytecode the current jump instruction
//
// 0x: JMP<TRUE> r#, ##
//   r# - Register to evaluate
//   ## - Signed offset relative to the bytecode the current jump instruction
//
// 0x: JMP<FALSE> r#, ##
//   r# - Register to evaluate
//   ## - Signed offset relative to the bytecode the current jump instruction
//
//----------------------------------------------------------------------------------------------------------------------

static inline void _MoveInstructionPointer(HqExecutionHandle hExec, const int32_t relativeOffset)
{
	HqFunctionHandle hFunction = hExec->hCurrentFrame->hFunction;
	HqModuleHandle hModule = hExec->hCurrentFrame->hFunction->hModule;

	uint8_t* const pNewIp = hExec->hCurrentFrame->decoder.cachedIp + relativeOffset;

	const uint8_t* const pFunctionStart = hModule->code.pData + hFunction->bytecodeOffsetStart;
	const uint8_t* const pFunctionEnd = hModule->code.pData + hFunction->bytecodeOffsetEnd;

	// Verify the new instruction pointer falls within the bounds of the current function.
	if(pNewIp < pFunctionStart || pNewIp >= pFunctionEnd)
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec, 
			HQ_STANDARD_EXCEPTION_RUNTIME_ERROR, 
			"Invalid jump offset: offset=%" PRId32 ", currentPosition=0x%" PRIXPTR ", functionStart=0x%" PRIXPTR ", functionEnd=0x%" PRIXPTR,
			relativeOffset,
			uintptr_t(hExec->hCurrentFrame->decoder.cachedIp),
			uintptr_t(pFunctionStart),
			uintptr_t(pFunctionEnd)
		);
	}
	else
	{
		hExec->hCurrentFrame->decoder.cachedIp = pNewIp;
		hExec->hCurrentFrame->decoder.ip = pNewIp;
	}
}

//----------------------------------------------------------------------------------------------------------------------

static inline bool _EvaluateValue(HqValueHandle hValue)
{
	if(!hValue)
	{
		return false;
	}

	switch(hValue->type)
	{
		case HQ_VALUE_TYPE_INT8:
			return hValue->as.int8 != 0;

		case HQ_VALUE_TYPE_INT16:
			return hValue->as.int16 != 0;

		case HQ_VALUE_TYPE_INT32:
			return hValue->as.int32 != 0;

		case HQ_VALUE_TYPE_INT64:
			return hValue->as.int64 != 0;

		case HQ_VALUE_TYPE_UINT8:
			return hValue->as.uint8 != 0;

		case HQ_VALUE_TYPE_UINT16:
			return hValue->as.uint16 != 0;

		case HQ_VALUE_TYPE_UINT32:
			return hValue->as.uint32 != 0;

		case HQ_VALUE_TYPE_UINT64:
			return hValue->as.uint64 != 0;

		case HQ_VALUE_TYPE_FLOAT32:
			return hValue->as.float32 != 0.0f;

		case HQ_VALUE_TYPE_FLOAT64:
			return hValue->as.float64 != 0.0;

		case HQ_VALUE_TYPE_BOOL:
			return hValue->as.boolean;

		case HQ_VALUE_TYPE_STRING:
			return hValue->as.pString->length != 0;

		case HQ_VALUE_TYPE_FUNCTION:
		case HQ_VALUE_TYPE_OBJECT:
			return true;

		case HQ_VALUE_TYPE_NATIVE:
			return hValue->as.native.pObject != nullptr;

		case HQ_VALUE_TYPE_ARRAY:
			return hValue->as.array.count > 0;

		default:
			assert(false);
			break;
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------------------

static inline void _RaiseFatalException_ObjectAsBool(HqExecutionHandle hExec, const uint32_t registerIndex)
{
	HqExecution::RaiseOpCodeException(
		hExec, 
		HQ_STANDARD_EXCEPTION_TYPE_ERROR, 
		"Type mismatch; expected boolean, null, primitive value, or string: r(%" PRIu32 ")", 
		registerIndex
	);
}

//----------------------------------------------------------------------------------------------------------------------

static inline void _RaiseFatalException_NoValueAtGpRegister(HqExecutionHandle hExec, const uint32_t registerIndex)
{
	HqExecution::RaiseOpCodeException(
		hExec, 
		HQ_STANDARD_EXCEPTION_RUNTIME_ERROR, 
		"Failed to retrieve general-purpose register: r(%" PRIu32 ")", 
		registerIndex
	);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_Jump(HqExecutionHandle hExec)
{
	const int32_t offset = HqDecoder::LoadInt32(hExec->hCurrentFrame->decoder);

	// No condition, just move the instruction pointer.
	_MoveInstructionPointer(hExec, offset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_Jump(HqDisassemble& disasm)
{
	const int32_t offset = HqDecoder::LoadInt32(disasm.decoder);
	const uintptr_t position = uintptr_t(intptr_t(disasm.opcodeOffset) + offset);

	char str[64];
	snprintf(str, sizeof(str), "JMP #%" PRId32 " (0x%" PRIXPTR ")", offset, position);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_Jump(HqDecoder& decoder)
{
	HqDecoder::EndianSwapInt32(decoder);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_JumpIfTrue(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const int32_t offset = HqDecoder::LoadInt32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
	if(result == HQ_SUCCESS)
	{
		// Object values cannot be evaluated directly.
		if(!HqValueIsObject(hValue))
		{
			const bool pass = _EvaluateValue(hValue);

			if(pass)
			{
				_MoveInstructionPointer(hExec, offset);
			}
		}
		else
		{
			// Raise a fatal script exception.
			_RaiseFatalException_ObjectAsBool(hExec, registerIndex);
		}
	}
	else
	{
		// Raise a fatal script exception.
		_RaiseFatalException_NoValueAtGpRegister(hExec, registerIndex);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_JumpIfTrue(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const int32_t offset = HqDecoder::LoadInt32(disasm.decoder);
	const uintptr_t position = uintptr_t(intptr_t(disasm.opcodeOffset) + offset);

	char str[64];
	snprintf(str, sizeof(str), "JMP_TRUE r%" PRIu32 ", #%" PRId32 " (0x%" PRIXPTR ")", registerIndex, offset, position);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_JumpIfTrue(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder);
	HqDecoder::EndianSwapInt32(decoder);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_JumpIfFalse(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const int32_t offset = HqDecoder::LoadInt32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
	if(result == HQ_SUCCESS)
	{
		// Object values cannot be evaluated directly.
		if(!HqValueIsObject(hValue))
		{
			const bool pass = !_EvaluateValue(hValue);

			if(pass)
			{
				_MoveInstructionPointer(hExec, offset);
			}
		}
		else
		{
			// Raise a fatal script exception.
			_RaiseFatalException_ObjectAsBool(hExec, registerIndex);
		}
	}
	else
	{
		// Raise a fatal script exception.
		_RaiseFatalException_NoValueAtGpRegister(hExec, registerIndex);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_JumpIfFalse(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const int32_t offset = HqDecoder::LoadInt32(disasm.decoder);
	const uintptr_t position = uintptr_t(intptr_t(disasm.opcodeOffset) + offset);

	char str[64];
	snprintf(str, sizeof(str), "JMP_FALSE r%" PRIu32 ", #%" PRId32 " (0x%" PRIXPTR ")", registerIndex, offset, position);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_JumpIfFalse(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder);
	HqDecoder::EndianSwapInt32(decoder);
}

//----------------------------------------------------------------------------------------------------------------------
