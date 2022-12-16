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
#include "../Frame.hpp"
#include "../Program.hpp"
#include "../Vm.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Call into a function.
//
// 0x: CALL s#
//
//   s# = String table index to the name of the function to be called.
//
// 0x: CALL_VALUE r#
//
//   r# = Register containing the function value to call.
//
//----------------------------------------------------------------------------------------------------------------------

static void CallScriptFunction(HqExecutionHandle hExec, HqFunctionHandle hFunction)
{
	assert(hExec != HQ_EXECUTION_HANDLE_NULL);
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	// A new frame gets pushed for all functions, even native functions.
	// But for native functions, it's just a dummy frame for the sake of
	// any code that would wish to resolve the frame stack if a script
	// exception were to occur within the native function.
	HqExecution::PushFrame(hExec, hFunction);

	if(hFunction->isNative)
	{
		// Native functions are called immediately.
		if(hFunction->nativeFn)
		{
			if(hExec->hVm->isGcThreadEnabled)
			{
				// We can't predict what native calls are going to do and since recursive locks on RwLocks
				// are not allowed, we unlock the GC RwLock here to prevent possible deadlocks. We'll put
				// the lock back on it immediately after the call is finished, but during this time, the
				// garbage collector will likely be running.
				HqRwLock::ReadUnlock(hExec->hVm->gc.rwLock);
			}

			// Call the native function.
			hFunction->nativeFn(hExec, hFunction, hFunction->pNativeUserData);

			if(hExec->hVm->isGcThreadEnabled)
			{
				HqRwLock::ReadLock(hExec->hVm->gc.rwLock);
			}

			// Pop the dummy frame from the frame stack now that it's no longer needed.
			HqExecution::PopFrame(hExec);
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec,
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Script native function pointer is null: \"%s\"",
				hFunction->pSignature->data
			);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeExec_Call(HqExecutionHandle hExec)
{
	int result;

	const uint32_t stringIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqString* const pFuncName = HqProgram::GetString(hExec->hCurrentFrame->hFunction->hProgram, stringIndex, &result);
	if(pFuncName)
	{
		HqFunctionHandle hFunction = HqVm::GetFunction(hExec->hVm, pFuncName, &result);
		if(hFunction)
		{
			CallScriptFunction(hExec, hFunction);
		}
		else
		{
			// Raise a fatal script exception.
			HqExecution::RaiseOpCodeException(
				hExec,
				HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Script function does not exist: \"%s\"",
				pFuncName->data
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec,
			HQ_STANDARD_EXCEPTION_TYPE_ERROR,
			"String does not exist at index: s(%" PRIu32 ")",
			stringIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_Call(HqDisassemble& disasm)
{
	const uint32_t stringIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[256];
	snprintf(str, sizeof(str), "CALL s%" PRIu32, stringIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_CallValue(HqExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = HqDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	HqValueHandle hValue = HqFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
	if(HqValueIsFunction(hValue))
	{
		CallScriptFunction(hExec, hValue->as.hFunction);
	}
	else
	{
		// Raise a fatal script exception.
		HqExecution::RaiseOpCodeException(
			hExec,
			HQ_STANDARD_EXCEPTION_TYPE_ERROR,
			"Type mismatch; expected function value: r(%" PRIu32 ")",
			registerIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_CallValue(HqDisassemble& disasm)
{
	const uint32_t registerIndex = HqDecoder::LoadUint32(disasm.decoder);

	char str[256];
	snprintf(str, sizeof(str), "CALL_VALUE r%" PRIu32, registerIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
