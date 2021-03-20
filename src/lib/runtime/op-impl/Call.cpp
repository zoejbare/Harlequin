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
			// A new frame gets pushed for all functions, even native functions.
			// But for native functions, it's just a dummy frame for the sake of
			// any code that would wish to resolve the frame stack if a script
			// exception were to occur within the native function.
			XenonExecution::PushFrame(hExec, hFunction);

			if(hFunction->isNative)
			{
				if(hFunction->nativeBinding)
				{
					hFunction->nativeBinding(hExec, hFunction);

					if(!hExec->exception)
					{
						// If no script exception occurred within the native function,
						// we can pop the dummy frame from the frame stack.
						XenonExecution::PopFrame(hExec);
					}
				}
				else
				{
					// TODO: Raise script exception
					hExec->exception = true;
				}
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

	XenonValueDispose(hValue);
}

//----------------------------------------------------------------------------------------------------------------------

void OpCodeDisasm_Call(XenonDisassemble& disasm)
{
	int result;

	const uint32_t constIndex = XenonDecoder::LoadUint32(disasm.decoder);

	XenonValueHandle hValue = XenonProgram::GetConstant(disasm.hProgram, constIndex, &result);
	std::string valueData = XenonValue::GetDebugString(hValue);

	XenonValueDispose(hValue);

	char str[64];
	snprintf(str, sizeof(str), "CALL c%" PRIu32 " %s", constIndex, valueData.c_str());
	disasm.onDisasmFn(disasm.pUserData, str, disasm.opcodeOffset);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
