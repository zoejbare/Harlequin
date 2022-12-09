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
#include "../../Program.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Load a constant to a general-purpose register in the current frame.
//
// 0x: LOAD_CONST r#, c#
//
//   r# = General-purpose register index
//   c# = Constant table index
//
// 0x: LOAD_CONST_NULL r#
//
//   r# = General-purpose register index
// 
// 0x: LOAD_CONST_BOOL r#, ##
// 0x: LOAD_CONST_I8   r#, ##
// 0x: LOAD_CONST_I16  r#, ##
// 0x: LOAD_CONST_I32  r#, ##
// 0x: LOAD_CONST_I64  r#, ##
// 0x: LOAD_CONST_U8   r#, ##
// 0x: LOAD_CONST_U16  r#, ##
// 0x: LOAD_CONST_U32  r#, ##
// 0x: LOAD_CONST_U64  r#, ##
// 0x: LOAD_CONST_F32  r#, ##
// 0x: LOAD_CONST_F64  r#, ##
//
//   r# = General-purpose register index
//   ## = Constant value to directly load
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_LoadConst(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t constantIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constantIndex, &result);
	if(result == HQ_SUCCESS)
	{
		HqValueHandle hValueCopy = HqValue::Copy(hExec->hVm, hValue);
		if(hValueCopy)
		{
			// Make a copy of the value, then upload that copy to the register.
			// A copy must be made to avoid accidentally changing the underlying
			// data of the constant.
			result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValueCopy, registerIndex);
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
				"Failed to create copy of constant value: c(%" PRIu32 ")",
				constantIndex
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec,
			HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
			"Failed to retrieve constant value: c(%" PRIu32 ")",
			constantIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConst(HqDisassemble& disasm)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t constantIndex = HqDecoder::LoadUint32(disasm.decoder);

	HqValueHandle hValue = HqProgram::GetConstant(disasm.hProgram, constantIndex, &result);
	HqString* const pValueData = HqValue::GetDebugString(hValue);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST r%" PRIu32 ", c%" PRIu32 " %s", registerIndex, constantIndex, pValueData->data);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);

	HqString::Release(pValueData);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstNull(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateNull();

	// Upload the constant to the destination register.
	result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstNull(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_NULL r%" PRIu32, registerIndex);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstBool(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const bool rawValue = HqDecoder::LoadBool(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateBool(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant bool value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstBool(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const bool rawValue = HqDecoder::LoadBool(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_BOOL r%" PRIu32 ", %s", registerIndex, rawValue ? "true" : "false");
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstI8(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const int8_t rawValue = HqDecoder::LoadInt8(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateInt8(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant int8 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstI8(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const int8_t rawValue = HqDecoder::LoadInt8(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_I8 r%" PRIu32 ", %" PRId8, registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstI16(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const int16_t rawValue = HqDecoder::LoadInt16(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateInt16(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant int16 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstI16(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const int16_t rawValue = HqDecoder::LoadInt16(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_I16 r%" PRIu32 ", %" PRId16, registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstI32(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const int32_t rawValue = HqDecoder::LoadInt32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateInt32(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant int32 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstI32(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const int32_t rawValue = HqDecoder::LoadInt32(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_I32 r%" PRIu32 ", %" PRId32, registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstI64(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const int64_t rawValue = HqDecoder::LoadInt64(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateInt64(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant int64 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstI64(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const int64_t rawValue = HqDecoder::LoadInt64(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_I64 r%" PRIu32 ", %" PRId64, registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstU8(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint8_t rawValue = HqDecoder::LoadUint8(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateUint8(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant uint8 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstU8(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint8_t rawValue = HqDecoder::LoadUint8(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_U8 r%" PRIu32 ", %" PRIu8, registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstU16(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint16_t rawValue = HqDecoder::LoadUint16(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateUint16(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant uint16 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstU16(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint16_t rawValue = HqDecoder::LoadUint16(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_U16 r%" PRIu32 ", %" PRIu16, registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstU32(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t rawValue = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateUint32(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant uint32 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstU32(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t rawValue = HqDecoder::LoadUint32(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_U32 r%" PRIu32 ", %" PRIu32, registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstU64(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint64_t rawValue = HqDecoder::LoadUint64(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateUint64(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant uint64 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstU64(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint64_t rawValue = HqDecoder::LoadUint64(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_U64 r%" PRIu32 ", %" PRIu64, registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstF32(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const float rawValue = HqDecoder::LoadFloat32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateFloat32(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant float32 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstF32(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const float rawValue = HqDecoder::LoadFloat32(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_F32 r%" PRIu32 ", %f", registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_LoadConstF64(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const double rawValue = HqDecoder::LoadFloat64(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqValue::CreateFloat64(hExec->hVm, rawValue);
	if(hValue)
	{
		HqValue::SetAutoMark(hValue, false);

		// Upload the constant to the destination register.
		result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hValue, registerIndex);
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
			"Failed to create constant float64 value"
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_LoadConstF64(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const double rawValue = HqDecoder::LoadFloat64(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "LOAD_CONST_F64 r%" PRIu32 ", %f", registerIndex, rawValue);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
