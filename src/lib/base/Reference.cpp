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

void HqReference::Initialize(
	HqReference& output,
	HqDisposeCallback onDisposeFn,
	void* pObject
)
{
	assert(onDisposeFn != nullptr);
	assert(pObject != nullptr);

	output.onDisposeFn = onDisposeFn;
	output.pObject = pObject;
	output.count = 1;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqReference::AddRef(HqReference& ref)
{
	return HqAtomic::FetchAdd(&ref.count, 1) + 1;
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqReference::Release(HqReference& ref)
{
	const int32_t currentValue = HqAtomic::FetchAdd(&ref.count, -1) - 1;

	if(currentValue == 0)
	{
		// Don't do anything with the reference object after disposing because it will probably been freed by this call.
		ref.onDisposeFn(ref.pObject);
	}

	return currentValue;
}

//----------------------------------------------------------------------------------------------------------------------
