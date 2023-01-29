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
#include "Module.hpp"

#include <assert.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqFunction::CreateInit(
	HqModuleHandle hModule,
	const uint32_t bytecodeOffset,
	const uint32_t bytecodeLength
)
{
	assert(hModule != HQ_MODULE_HANDLE_NULL);
	assert(bytecodeLength > 0);

	HqFunction* const pOutput = new HqFunction();
	assert(pOutput != HQ_FUNCTION_HANDLE_NULL);

	char funcName[512];
	snprintf(funcName, sizeof(funcName), "void `.init-module-'%s'()", hModule->pName->data);

	pOutput->hModule = hModule;
	pOutput->pSignature = HqString::Create(funcName);
	pOutput->bytecodeOffsetStart = bytecodeOffset;
	pOutput->bytecodeOffsetEnd = bytecodeOffset + bytecodeLength;
	pOutput->numParameters = 0;
	pOutput->numReturnValues = 0;
	pOutput->isNative = false;

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqFunction::CreateScript(
	HqModuleHandle hModule,
	HqString* const pSignature,
	HqGuardedBlock::PtrArray& guardedBlocks,
	const uint32_t bytecodeOffset,
	const uint32_t bytecodeLength,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	assert(hModule != HQ_MODULE_HANDLE_NULL);
	assert(pSignature != nullptr);

	HqFunction* const pOutput = new HqFunction();
	assert(pOutput != HQ_FUNCTION_HANDLE_NULL);

	pOutput->hModule = hModule;
	pOutput->pSignature = pSignature;
	pOutput->bytecodeOffsetStart = bytecodeOffset;
	pOutput->bytecodeOffsetEnd = bytecodeOffset + bytecodeLength;
	pOutput->numParameters = numParameters;
	pOutput->numReturnValues = numReturnValues;
	pOutput->isNative = false;

	// Take ownership of the guarded block array.
	HqGuardedBlock::PtrArray::Move(pOutput->guardedBlocks, guardedBlocks);

	// Add a reference to the function signature string.
	HqString::AddRef(pOutput->pSignature);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

HqFunctionHandle HqFunction::CreateNative(
	HqModuleHandle hModule,
	HqString* const pSignature,
	const uint16_t numParameters,
	const uint16_t numReturnValues
)
{
	assert(hModule != HQ_MODULE_HANDLE_NULL);
	assert(pSignature != nullptr);

	HqFunction* const pOutput = new HqFunction();
	if(!pOutput)
	{
		return HQ_FUNCTION_HANDLE_NULL;
	}

	pOutput->hModule = hModule;
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

	pOutput->hModule = HQ_MODULE_HANDLE_NULL;
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

	// Release each guarded block.
	for(size_t blockIndex = 0; blockIndex < hFunction->guardedBlocks.count; ++blockIndex)
	{
		HqGuardedBlock::Dispose(hFunction->guardedBlocks.pData[blockIndex]);
	}

	HqGuardedBlock::PtrArray::Dispose(hFunction->guardedBlocks);
	HqString::Release(hFunction->pSignature);

	delete hFunction;
}

//----------------------------------------------------------------------------------------------------------------------

HqVmHandle HqFunction::GetVm(HqFunctionHandle hFunction)
{
	assert(hFunction != HQ_FUNCTION_HANDLE_NULL);

	return hFunction->hModule
		? hFunction->hModule->hVm
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
