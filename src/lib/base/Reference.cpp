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

#include "Reference.hpp"

#include "../common/Atomic.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

XenonReference XenonReference::Initialize(XenonReferenceDestructCallback onDestructFn, void* pObject)
{
	assert(onDestructFn != nullptr);
	assert(pObject != nullptr);

	XenonReference output;

	output.onDestructFn = onDestructFn;
	output.pObject = pObject;
	output.count = 1;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t XenonReference::AddRef(XenonReference* const pRef)
{
	return XenonAtomic::FetchAdd(&pRef->count, 1) + 1;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t XenonReference::Release(XenonReference* const pRef)
{
	const int32_t currentValue = XenonAtomic::FetchAdd(&pRef->count, -1) - 1;

	if(currentValue == 0)
	{
		// Don't do anything with the reference object after destructing because it has probably been freed.
		pRef->onDestructFn(pRef->pObject);
	}

	return currentValue;
}

//----------------------------------------------------------------------------------------------------------------------
