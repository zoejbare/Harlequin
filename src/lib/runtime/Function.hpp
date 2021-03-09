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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "../shared/String.hpp"
#include "../shared/Value.hpp"

#include <SkipProbe/SkipProbe.hpp>

//----------------------------------------------------------------------------------------------------------------------

struct XenonProgram;

struct XenonFunction
{
	typedef SkipProbe::HashMap<
		XenonString*,
		XenonFunctionHandle,
		XenonString::StlHash,
		XenonString::StlCompare
	> StringToHandleMap;

	static XenonFunctionHandle Create(
		XenonProgramHandle hProgram,
		XenonString* const pSignature,
		const uint32_t bytecodeOffset,
		const uint16_t numParameters,
		const uint16_t numReturnValues
	);
	static void Dispose(XenonFunctionHandle hFunction);

	static XenonValueHandle GetLocalVariable(XenonFunctionHandle hFunction, XenonString* const pName);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonProgramHandle hProgram;

	XenonString* pSignature;

	XenonValue::StringToHandleMap locals;

	uint32_t offset;
	uint16_t numParameters;
	uint16_t numReturnValues;
};


//----------------------------------------------------------------------------------------------------------------------
