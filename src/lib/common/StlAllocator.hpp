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

#include "../XenonScript.h"

#include <type_traits>

//----------------------------------------------------------------------------------------------------------------------

template <typename T>
class XenonStlAllocator
{
public:

    using value_type = T;

	constexpr XenonStlAllocator() noexcept {}
	constexpr XenonStlAllocator(const XenonStlAllocator&) noexcept = default;

	template <typename _Other>
	constexpr XenonStlAllocator(const XenonStlAllocator<_Other>&) noexcept {}

    template <class _Other>
    struct rebind
	{
        using other = XenonStlAllocator<_Other>;
    };

    T* allocate(const size_t count)
	{
		return reinterpret_cast<T*>(XenonMemAlloc(sizeof(T) * count));
	}

    void deallocate(T* const pPtr, size_t)
	{
		XenonMemFree(pPtr);
	}
};

//----------------------------------------------------------------------------------------------------------------------
