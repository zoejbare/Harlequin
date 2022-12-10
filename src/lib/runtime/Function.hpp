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

#include "../common/HashMap.hpp"
#include "../common/Stack.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqProgram;

struct HqFunction
{
	typedef HqHashMap<
		HqString*, 
		HqFunctionHandle, 
		HqString::StlHash, 
		HqString::StlCompare
	> StringToHandleMap;

	typedef HqHashMap<
		HqString*, 
		bool, 
		HqString::StlHash, 
		HqString::StlCompare
	> StringToBoolMap;

	typedef HqStack<HqFunctionHandle> HandleStack;

	static HqFunctionHandle CreateInit(HqProgramHandle hProgram, uint32_t bytecodeLength);
	static HqFunctionHandle CreateScript(
		HqProgramHandle hProgram,
		HqString* pSignature,
		StringToBoolMap& locals,
		HqGuardedBlock::Array& guardedBlocks,
		uint32_t bytecodeOffsetStart,
		uint32_t bytecodeOffsetEnd,
		uint16_t numParameters,
		uint16_t numReturnValues
	);
	static HqFunctionHandle CreateNative(
		HqProgramHandle hProgram,
		HqString* pSignature,
		uint16_t numParameters,
		uint16_t numReturnValues
	);
	static HqFunctionHandle CreateBuiltIn(
		HqString* pSignature,
		HqNativeFunction nativeFn,
		uint16_t numParameters,
		uint16_t numReturnValues
	);
	static void Dispose(HqFunctionHandle hFunction);

	static HqVmHandle GetVm(HqFunctionHandle hFunction);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqProgramHandle hProgram;
	HqNativeFunction nativeFn;

	HqString* pSignature;
	void* pNativeUserData;

	HqGuardedBlock::Array guardedBlocks;
	StringToBoolMap locals;

	uint32_t bytecodeOffsetStart;
	uint32_t bytecodeOffsetEnd;

	uint16_t numParameters;
	uint16_t numReturnValues;

	bool isNative;
};


//----------------------------------------------------------------------------------------------------------------------
