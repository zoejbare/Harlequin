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

#include <stddef.h>
#include <utility>

//----------------------------------------------------------------------------------------------------------------------

template <typename T>
class XenonStlAllocator;

//----------------------------------------------------------------------------------------------------------------------

// The <void> implementation needs to be defined first since it's used in the generic implementation.
template<>
class XenonStlAllocator<void>
{
public:

	typedef void*       pointer;
	typedef const void* const_pointer;
	typedef void        value_type;

	template <class U>
	struct rebind
	{
		typedef XenonStlAllocator<U> other;
	};
};

//----------------------------------------------------------------------------------------------------------------------

template <class T>
class XenonStlAllocator
{
public:

	typedef T         value_type;
	typedef T*        pointer;
	typedef T&        reference;
	typedef const T*  const_pointer;
	typedef const T&  const_reference;
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;

	template <typename U>
	struct rebind
	{
		typedef XenonStlAllocator<U> other;
	};

	XenonStlAllocator()
	{
		// Default construct allocator; do nothing.
	}

	XenonStlAllocator(const XenonStlAllocator& /*other*/)
	{
		// Copy construct from other allocator; do nothing.
	}

	template <class U>
	XenonStlAllocator(const XenonStlAllocator<U>& /*other*/)
	{
		// Copy construct from other allocator of related type; do nothing.
	}

	XenonStlAllocator& operator =(const XenonStlAllocator& /*other*/)
	{
		// Assign another allocator to this one; effectively does nothing.
		return *this;
	}

	template <class U>
	XenonStlAllocator& operator =(const XenonStlAllocator<U>& /*other*/)
	{
		// Assign another allocator of a different type to this one; effectively does nothing.
		return *this;
	}

	bool operator ==(const XenonStlAllocator& /*other*/) const
	{
		// Compare equality of this allocator to another; always true.
		return true;
	}

	bool operator !=(const XenonStlAllocator& /*other*/) const
	{
		// Compare inequality of this allocator to another; always false.
		return false;
	}

	pointer address(reference ref) const
	{
		// Return address of mutable 'ref'.
		return reinterpret_cast<value_type*>(&reinterpret_cast<char&>(ref));
	}

	const_pointer address(const_reference ref) const
	{
		// Return address of non-mutable 'ref'.
		return reinterpret_cast<const value_type*>(&reinterpret_cast<const char&>(ref));
	}

	pointer allocate(size_type count, XenonStlAllocator<void>::const_pointer hint = nullptr)
	{
		// Allocate array of 'count' elements; ignore hint.
		(void)(hint);
		return reinterpret_cast<pointer>(XenonMemAlloc(sizeof(T) * count));
	}

	void deallocate(pointer ptr, size_type /*count*/)
	{
		// Free memory at 'ptr'; ignore count.
		XenonMemFree(ptr);
	}

	size_type max_size() const
	{
		// Estimate maximum array size.
		return size_type(-1);
	}

	void construct(pointer ptr)
	{
		// Default construct allocated object at 'ptr'.
		new(reinterpret_cast<void*>(ptr)) value_type();
	}

	void construct(pointer ptr, const_reference val)
	{
		// Construct allocated object at 'ptr' with parameter value 'val'.
		new(reinterpret_cast<void*>(ptr)) value_type(val);
	}

#if !defined(XENON_PLATFORM_PS3)
	template <class U, class... Args>
	void construct(U* const p, Args&&... args)
	{
		// Construct allocated object at 'ptr' with parameter value(s) 'args'.
		new(reinterpret_cast<void*>(p)) U(std::forward<Args>(args)...);
	}
#endif

	template <class U>
	void destroy(U* const ptr)
	{
		// Destruct the allocated object at 'ptr'.
		ptr->~U();
	}
};

//----------------------------------------------------------------------------------------------------------------------
