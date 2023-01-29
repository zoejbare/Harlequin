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

HqGuardedBlock* HqGuardedBlock::Create(
	ExceptionHandlerArray& exceptionHandlers, 
	const uint32_t bytecodeOffset, 
	const uint32_t bytecodeLength
)
{
	HqGuardedBlock* const pOutput = new HqGuardedBlock();
	assert(pOutput != nullptr);

	pOutput->bytecodeOffsetStart = bytecodeOffset;
	pOutput->bytecodeOffsetEnd = bytecodeOffset + bytecodeLength;

	// Take ownership over the exception handler array.
	ExceptionHandlerArray::Move(pOutput->handlers, exceptionHandlers);

	// Add a reference to the class name strings in each exception handler.
	for(size_t i = 0; i < pOutput->handlers.count; ++i)
	{
		ExceptionHandler& handler = pOutput->handlers.pData[i];

		HqString::AddRef(handler.pClassName);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqGuardedBlock::Dispose(HqGuardedBlock* const pGuardedBlock)
{
	// Release the exception handler class name strings.
	for(size_t i = 0; i < pGuardedBlock->handlers.count; ++i)
	{
		ExceptionHandler& handler = pGuardedBlock->handlers.pData[i];

		HqString::Release(handler.pClassName);
	}

	ExceptionHandlerArray::Dispose(pGuardedBlock->handlers);

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
