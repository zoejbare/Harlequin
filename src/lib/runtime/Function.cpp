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

#include "Function.hpp"
#include "Program.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonFunctionHandle XenonFunction::CreateScript(
	XenonProgramHandle hProgram,
	XenonString* const pSignature,
	XenonValue::StringToHandleMap& locals,
	const uint32_t bytecodeOffset,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	assert(hProgram != XENON_PROGRAM_HANDLE_NULL);
	assert(pSignature != nullptr);

	XenonFunction* const pOutput = new XenonFunction();
	if(!pOutput)
	{
		return XENON_FUNCTION_HANDLE_NULL;
	}

	pOutput->hProgram = hProgram;
	pOutput->pSignature = pSignature;
	pOutput->locals = std::move(locals);
	pOutput->bytecodeOffset = bytecodeOffset;
	pOutput->numParameters = numParameters;
	pOutput->numReturnValues = numReturnValues;
	pOutput->isNative = false;

	XenonString::AddRef(pOutput->pSignature);

	// Just in case the std::move() doesn't do the trick,
	// make sure the input locals are cleared out.
	locals.Clear();

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonFunctionHandle XenonFunction::CreateNative(
	XenonProgramHandle hProgram,
	XenonString* const pSignature,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	assert(hProgram != XENON_PROGRAM_HANDLE_NULL);
	assert(pSignature != nullptr);

	XenonFunction* const pOutput = new XenonFunction();
	if(!pOutput)
	{
		return XENON_FUNCTION_HANDLE_NULL;
	}

	pOutput->hProgram = hProgram;
	pOutput->pSignature = pSignature;
	pOutput->nativeFn = nullptr; // The callback will be provided externally.
	pOutput->numParameters = numParameters;
	pOutput->numReturnValues = numReturnValues;
	pOutput->isNative = true;

	XenonString::AddRef(pOutput->pSignature);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

XenonFunctionHandle XenonFunction::CreateBuiltIn(
	XenonString* const pSignature,
	XenonNativeFunction nativeFn,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	assert(pSignature != nullptr);
	assert(nativeFn != nullptr);

	XenonFunction* const pOutput = new XenonFunction();
	if(!pOutput)
	{
		return XENON_FUNCTION_HANDLE_NULL;
	}

	pOutput->hProgram = XENON_PROGRAM_HANDLE_NULL;
	pOutput->pSignature = pSignature;
	pOutput->nativeFn = nativeFn;
	pOutput->numParameters = numParameters;
	pOutput->numReturnValues = numReturnValues;
	pOutput->isNative = true;

	XenonString::AddRef(pOutput->pSignature);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonFunction::Dispose(XenonFunctionHandle hFunction)
{
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	XenonString::Release(hFunction->pSignature);

	// Dispose of the local variables.
	for(auto& kv : hFunction->locals)
	{
		XenonString::Release(kv.key);
	}

	delete hFunction;
}

//----------------------------------------------------------------------------------------------------------------------

XenonVmHandle XenonFunction::GetVm(XenonFunctionHandle hFunction)
{
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	return hFunction->hProgram
		? hFunction->hProgram->hVm
		: XENON_VM_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

void* XenonFunction::operator new(const size_t sizeInBytes)
{
	return XenonMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonFunction::operator delete(void* const pObject)
{
	XenonMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
