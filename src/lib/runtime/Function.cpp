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
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqFunction::CreateInit(
	HqProgramHandle hProgram,
	const uint32_t bytecodeLength
)
{
	assert(hProgram != HQ_PROGRAM_HANDLE_NULL);
	assert(bytecodeLength > 0);

	HqFunction* const pOutput = new HqFunction();
	assert(pOutput != HQ_FUNCTION_HANDLE_NULL);

	char funcName[512];
	snprintf(funcName, sizeof(funcName), "void `.init-program-'%s'()", hProgram->pName->data);

	pOutput->hProgram = hProgram;
	pOutput->pSignature = HqString::Create(funcName);
	pOutput->bytecodeOffsetStart = 0;
	pOutput->bytecodeOffsetEnd = bytecodeLength;
	pOutput->numParameters = 0;
	pOutput->numReturnValues = 0;
	pOutput->isNative = false;

	StringToBoolMap::Initialize(pOutput->locals);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqFunction::CreateScript(
	HqProgramHandle hProgram,
	HqString* const pSignature,
	StringToBoolMap& locals,
	HqGuardedBlock::Array& guardedBlocks,
	const uint32_t bytecodeOffset,
	const uint32_t bytecodeLength,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	assert(hProgram != HQ_PROGRAM_HANDLE_NULL);
	assert(pSignature != nullptr);

	HqFunction* const pOutput = new HqFunction();
	assert(pOutput != HQ_FUNCTION_HANDLE_NULL);

	pOutput->hProgram = hProgram;
	pOutput->pSignature = pSignature;
	pOutput->locals = locals;
	pOutput->guardedBlocks = guardedBlocks;
	pOutput->bytecodeOffsetStart = bytecodeOffset;
	pOutput->bytecodeOffsetEnd = bytecodeOffset + bytecodeLength;
	pOutput->numParameters = numParameters;
	pOutput->numReturnValues = numReturnValues;
	pOutput->isNative = false;

	HqString::AddRef(pOutput->pSignature);

	// Make a copy of the input local variable map to store on this function.
	HqFunction::StringToBoolMap::Initialize(pOutput->locals);
	HqFunction::StringToBoolMap::Copy(pOutput->locals, locals);

	// Track the name strings in the local variables map.
	{
		HqFunction::StringToBoolMap::Iterator iter;
		while(HqFunction::StringToBoolMap::IterateNext(pOutput->locals, iter))
		{
			HqString::AddRef(iter.pData->key);
		}
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqFunction::CreateNative(
	HqProgramHandle hProgram,
	HqString* const pSignature,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	assert(hProgram != HQ_PROGRAM_HANDLE_NULL);
	assert(pSignature != nullptr);

	HqFunction* const pOutput = new HqFunction();
	if(!pOutput)
	{
		return HQ_FUNCTION_HANDLE_NULL;
	}

	pOutput->hProgram = hProgram;
	pOutput->pSignature = pSignature;
	pOutput->nativeFn = nullptr; // The callback will be provided externally.
	pOutput->numParameters = numParameters;
	pOutput->numReturnValues = numReturnValues;
	pOutput->isNative = true;

	StringToBoolMap::Initialize(pOutput->locals);
	HqString::AddRef(pOutput->pSignature);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqFunction::CreateBuiltIn(
	HqString* const pSignature,
	HqNativeFunction nativeFn,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	assert(pSignature != nullptr);
	assert(nativeFn != nullptr);

	HqFunction* const pOutput = new HqFunction();
	if(!pOutput)
	{
		return HQ_FUNCTION_HANDLE_NULL;
	}

	pOutput->hProgram = HQ_PROGRAM_HANDLE_NULL;
	pOutput->pSignature = pSignature;
	pOutput->nativeFn = nativeFn;
	pOutput->numParameters = numParameters;
	pOutput->numReturnValues = numReturnValues;
	pOutput->isNative = true;

	StringToBoolMap::Initialize(pOutput->locals);
	HqString::AddRef(pOutput->pSignature);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqFunction::Dispose(HqFunctionHandle hFunction)
{
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	// Dispose of the local variables.
	{
		StringToBoolMap::Iterator iter;
		while(StringToBoolMap::IterateNext(hFunction->locals, iter))
		{
			HqString::Release(iter.pData->key);
		}

		StringToBoolMap::Dispose(hFunction->locals);
	}

	// Release each guarded block.
	for(size_t blockIndex = 0; blockIndex < hFunction->guardedBlocks.count; ++blockIndex)
	{
		HqGuardedBlock::Dispose(hFunction->guardedBlocks.pData[blockIndex]);
	}

	HqGuardedBlock::Array::Dispose(hFunction->guardedBlocks);
	HqString::Release(hFunction->pSignature);

	delete hFunction;
}

//----------------------------------------------------------------------------------------------------------------------

HqVmHandle HqFunction::GetVm(HqFunctionHandle hFunction)
{
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	return hFunction->hProgram
		? hFunction->hProgram->hVm
		: HQ_VM_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqFunction::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqFunction::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
