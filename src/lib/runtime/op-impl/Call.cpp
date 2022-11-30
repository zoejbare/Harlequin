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

#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Call into a function.
//
// 0x: CALL c#
//
//   c# = Constant index to the name of the function to be called.
//
// 0x: CALL_VALUE r#
//
//   r# = Register containing the function value to call.
//
//----------------------------------------------------------------------------------------------------------------------

static void CallScriptFunction(XenonExecutionHandle hExec, XenonFunctionHandle hFunction)
{
	assert(hExec != XENON_EXECUTION_HANDLE_NULL);
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	// A new frame gets pushed for all functions, even native functions.
	// But for native functions, it's just a dummy frame for the sake of
	// any code that would wish to resolve the frame stack if a script
	// exception were to occur within the native function.
	XenonExecution::PushFrame(hExec, hFunction);

	if(hFunction->isNative)
	{
		// Native functions are called immediately.
		if(hFunction->nativeFn)
		{
			// We can't predict what native calls are going to do and since recursive locks on RwLocks
			// are not allowed, we unlock the GC RwLock here to prevent possible deadlocks. We'll put
			// a lock back on it immediately after it's finished, but during this time, the garbage
			// collector will likely be running.
			XenonRwLock::ReadUnlock(hExec->hVm->gcRwLock);
			hFunction->nativeFn(hExec, hFunction, hFunction->pNativeUserData);
			XenonRwLock::ReadLock(hExec->hVm->gcRwLock);

			if(!hExec->exception)
			{
				// If no script exception occurred within the native function,
				// we can pop the dummy frame from the frame stack.
				XenonExecution::PopFrame(hExec);
			}
		}
		else
		{
			// Raise a fatal script exception.
			XenonExecution::RaiseOpCodeException(
				hExec,
				XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
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

void OpCodeExec_Call(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t constIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hValue = XenonProgram::GetConstant(hExec->hCurrentFrame->hFunction->hProgram, constIndex, &result);
	if(XenonValueIsString(hValue))
	{
		XenonFunctionHandle hFunction = XenonVm::GetFunction(hExec->hVm, hValue->as.pString, &result);
		if(hFunction)
		{
			CallScriptFunction(hExec, hFunction);
		}
		else
		{
			// Raise a fatal script exception.
			XenonExecution::RaiseOpCodeException(
				hExec,
				XENON_STANDARD_EXCEPTION_RUNTIME_ERROR,
				"Script function does not exist: \"%s\"",
				hValue->as.pString->data
			);
		}
	}
	else
	{
		// Raise a fatal script exception.
		XenonExecution::RaiseOpCodeException(
			hExec,
			XENON_STANDARD_EXCEPTION_TYPE_ERROR,
			"Type mismatch; expected string value: c(%" PRIu32 ")",
			constIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_Call(XenonDisassemble& disasm)
{
	int result;

	const uint32_t constIndex = XenonDecoder::LoadUint32(disasm.decoder);

	XenonValueHandle hValue = XenonProgram::GetConstant(disasm.hProgram, constIndex, &result);
	XenonString* const pValueData = XenonValue::GetDebugString(hValue);

	char str[256];
	snprintf(str, sizeof(str), "CALL c%" PRIu32 " %s", constIndex, pValueData->data);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);

	XenonString::Release(pValueData);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeExec_CallValue(XenonExecutionHandle hExec)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(hExec->hCurrentFrame->decoder);

	XenonValueHandle hValue = XenonFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex, &result);
	if(XenonValueIsFunction(hValue))
	{
		CallScriptFunction(hExec, hValue->as.hFunction);
	}
	else
	{
		// Raise a fatal script exception.
		XenonExecution::RaiseOpCodeException(
			hExec,
			XENON_STANDARD_EXCEPTION_TYPE_ERROR,
			"Type mismatch; expected function value: r(%" PRIu32 ")",
			registerIndex
		);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_CallValue(XenonDisassemble& disasm)
{
	int result;

	const uint32_t registerIndex = XenonDecoder::LoadUint32(disasm.decoder);

	char str[256];
	snprintf(str, sizeof(str), "CALL_VALUE r%" PRIu32, registerIndex);
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
