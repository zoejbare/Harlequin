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

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonFunctionHandle XenonFunction::Create(XenonProgramHandle hProgram)
{
	XenonFunction* const pOutput = new XenonFunction();
	if(!pOutput)
	{
		return XENON_FUNCTION_HANDLE_NULL;
	}

	pOutput->hProgram = hProgram;
	pOutput->pSignature = nullptr;
	pOutput->offset = 0;
	pOutput->numParameters = 0;
	pOutput->numReturnValues = 0;
	pOutput->isNative = false;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonFunction::Dispose(XenonFunctionHandle hFunction)
{
	assert(hFunction != XENON_FUNCTION_HANDLE_NULL);

	XenonString::Dispose(hFunction->pSignature);

	// Dispose of the local variables.
	for(auto& kv : hFunction->locals)
	{
		XenonString::Dispose(kv.key);
		XenonValueDispose(kv.value);
	}

	delete hFunction;
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
