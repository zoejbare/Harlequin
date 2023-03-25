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
#include "../../Module.hpp"
#include "../../Vm.hpp"

#include <stdio.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Initialize a new object by type name, storing it in a general-purpose register.
//
// 0x: INIT_OBJECT r#, s#
//
//   r# = General-purpose register where the new object will be stored.
//   s# = String table index to the name of the object type to create.
//
//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeExec_InitObject(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);
	const uint32_t stringIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqString* const pObjectTypeName= HqModule::GetString(hExec->hCurrentFrame->hFunction->hModule, stringIndex, &result);
	if(pObjectTypeName)
	{
		// Get the object schema matching the type name.
		HqScriptObject* const pObjectSchema = HqVm::GetObjectSchema(hExec->hVm, pObjectTypeName, &result);
		if(result == HQ_SUCCESS)
		{
			// Create a new object from the schema.
			HqValueHandle hObject = HqValue::CreateObject(hExec->hVm, pObjectSchema);

			// Verify the created value is an object.
			if(HqValueIsObject(hObject))
			{
				result = HqFrame::SetGpRegister(hExec->hCurrentFrame, hObject, registerIndex);
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

				HqValue::SetAutoMark(hObject, false);
			}
			else
			{
				// Raise a fatal script exception.
				HqExecution::RaiseOpCodeException(
					hExec,
					HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
					"Failed to create object value"
				);
			}
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec, 
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR, 
				"Failed to find object schema: type=%s",
				pObjectTypeName->data
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec, 
			HQ_STANDARD_EXCEPTION_RUNTIME_ERROR, 
			"String does not exist at index: c(%" PRIu32 ")",
			stringIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeDisasm_InitObject(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);
	const uint32_t stringIndex = HqDecoder::LoadUint32(disasm.decoder);

	char instr[512];
	snprintf(instr, sizeof(instr), "INIT_OBJECT r(%" PRIu32 "), s(%" PRIu32 ")", registerIndex, stringIndex);
	disasm.onDisasmFn(disasm.pUserData, instr, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void OpCodeEndian_InitObject(HqDecoder& decoder)
{
	HqDecoder::EndianSwapUint32(decoder); // r#
	HqDecoder::EndianSwapUint32(decoder); // s#
}

//----------------------------------------------------------------------------------------------------------------------
