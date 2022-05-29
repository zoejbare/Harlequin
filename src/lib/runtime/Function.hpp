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

#include "GuardedBlock.hpp"
#include "Value.hpp"

#include "../base/String.hpp"

#include "../common/Map.hpp"
#include "../common/Stack.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonProgram;

struct XenonFunction
{
	typedef XENON_MAP_TYPE<
		XenonString*,
		XenonFunctionHandle,
#if XENON_MAP_IS_UNORDERED
		XenonString::StlHash,
		XenonString::StlCompare,
#else
		XenonString::StlLess,
#endif
		XenonStlAllocator<XENON_MAP_NODE_TYPE(XenonString*, XenonFunctionHandle)>
	> StringToHandleMap;

	typedef XENON_MAP_TYPE<
		XenonString*,
		bool,
#if XENON_MAP_IS_UNORDERED
		XenonString::StlHash,
		XenonString::StlCompare,
#else
		XenonString::StlLess,
#endif
		XenonStlAllocator<XENON_MAP_NODE_TYPE(XenonString*, bool)>
	> StringToBoolMap;

	typedef XenonStack<XenonFunctionHandle> HandleStack;

	static XenonFunctionHandle CreateInit(XenonProgramHandle hProgram, uint32_t bytecodeLength);
	static XenonFunctionHandle CreateScript(
		XenonProgramHandle hProgram,
		XenonString* pSignature,
		XenonValue::StringToHandleMap& locals,
		XenonGuardedBlock::Array& guardedBlocks,
		uint32_t bytecodeOffsetStart,
		uint32_t bytecodeOffsetEnd,
		uint16_t numParameters,
		uint16_t numReturnValues
	);
	static XenonFunctionHandle CreateNative(
		XenonProgramHandle hProgram,
		XenonString* pSignature,
		uint16_t numParameters,
		uint16_t numReturnValues
	);
	static XenonFunctionHandle CreateBuiltIn(
		XenonString* pSignature,
		XenonNativeFunction nativeFn,
		uint16_t numParameters,
		uint16_t numReturnValues
	);
	static void Dispose(XenonFunctionHandle hFunction);

	static XenonVmHandle GetVm(XenonFunctionHandle hFunction);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonProgramHandle hProgram;
	XenonNativeFunction nativeFn;

	XenonString* pSignature;
	void* pNativeUserData;

	XenonGuardedBlock::Array guardedBlocks;
	XenonValue::StringToHandleMap locals;

	uint32_t bytecodeOffsetStart;
	uint32_t bytecodeOffsetEnd;

	uint16_t numParameters;
	uint16_t numReturnValues;

	bool isNative;
};


//----------------------------------------------------------------------------------------------------------------------
