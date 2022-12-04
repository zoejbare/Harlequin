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

#include "ExceptionHandler.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

HqExceptionHandler* HqExceptionHandler::Create(const uint8_t type, const uint32_t offset, HqString* const pClassName)
{
	HqExceptionHandler* const pOutput = new HqExceptionHandler();
	assert(pOutput != nullptr);

	pOutput->type = type;
	pOutput->offset = offset;
	pOutput->pClassName = pClassName;

	if(pOutput->pClassName)
	{
		HqString::AddRef(pOutput->pClassName);
	}

	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqExceptionHandler::Dispose(HqExceptionHandler* const pExceptionHandler)
{
	if(pExceptionHandler->pClassName)
	{
		HqString::Release(pExceptionHandler->pClassName);
	}

	delete pExceptionHandler;
}

//----------------------------------------------------------------------------------------------------------------------

void* HqExceptionHandler::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqExceptionHandler::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
