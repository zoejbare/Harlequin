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

	// Just in case the std::move() doesn't do the trick,
	// make sure the input locals are cleared out.
	HQ_MAP_FUNC_CLEAR(locals);

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

	HqString::AddRef(pOutput->pSignature);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqFunction::Dispose(HqFunctionHandle hFunction)
{
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	HqString::Release(hFunction->pSignature);

	// Dispose of the local variables.
	for(auto& kv : hFunction->locals)
	{
		HqString::Release(HQ_MAP_ITER_KEY(kv));
	}

	// Release each guarded block.
	for(size_t blockIndex = 0; blockIndex < hFunction->guardedBlocks.count; ++blockIndex)
	{
		HqGuardedBlock::Dispose(hFunction->guardedBlocks.pData[blockIndex]);
	}

	HqGuardedBlock::Array::Dispose(hFunction->guardedBlocks);

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
