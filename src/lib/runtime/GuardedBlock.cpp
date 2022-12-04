//
// Copyright (c) 2022, Zoe J. Bare
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

#include "GuardedBlock.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

HqGuardedBlock* HqGuardedBlock::Create(const uint32_t offset, const uint32_t length, const size_t handlerCount)
{
	HqGuardedBlock* const pOutput = new HqGuardedBlock();
	assert(pOutput != nullptr);

	pOutput->bytecodeOffsetStart = offset;
	pOutput->bytecodeOffsetEnd = offset + length;

	// Initialize the array of exception handlers.
	HqExceptionHandler::Array::Initialize(pOutput->handlers);
	HqExceptionHandler::Array::Reserve(pOutput->handlers, handlerCount);

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqGuardedBlock::Dispose(HqGuardedBlock* const pGuardedBlock)
{
	for(size_t i = 0; i < pGuardedBlock->handlers.count; ++i)
	{
		HqExceptionHandler* const pHandler = pGuardedBlock->handlers.pData[i];

		HqExceptionHandler::Dispose(pHandler);
	}

	HqExceptionHandler::Array::Dispose(pGuardedBlock->handlers);

	delete pGuardedBlock;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqGuardedBlock::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqGuardedBlock::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
