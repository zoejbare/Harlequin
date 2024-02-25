//
// Copyright (c) 2023, Zoe J. Bare
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

#include "Memory.hpp"

#include <gtest/gtest.h>

#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

Memory Memory::Instance;

//----------------------------------------------------------------------------------------------------------------------

Memory::Memory()
	: m_mutex()
	, m_context("Unknown")
	, m_minAllocSize(0)
	, m_maxAllocSize(0)
	, m_peakMemUsage(0)
	, m_mallocCount(0)
	, m_reallocCount(0)
	, m_activeAllocCount(0)
	, m_totalAllocCount(0)
	, m_currentTotalSize(0)
{
	HqMutex::Create(m_mutex);

	auto mallocFn = [](const size_t memSize) -> void*
	{
		return Memory::Instance._malloc(memSize);
	};
	auto reallocFn = [](void* const pMem, const size_t memSize) -> void*
	{
		return Memory::Instance._realloc(pMem, memSize);
	};
	auto freeFn = [](void* const pMem)
	{
		return Memory::Instance._free(pMem);
	};

	HqMemAllocator allocator;
	allocator.allocFn = mallocFn;
	allocator.reallocFn = reallocFn;
	allocator.freeFn = freeFn;

	HqMemSetAllocator(allocator);

}

//----------------------------------------------------------------------------------------------------------------------

Memory::~Memory()
{
	HqMemSetAllocator(HqMemGetDefaultAllocator());

	HqMutex::Dispose(m_mutex);
}

//----------------------------------------------------------------------------------------------------------------------

void Memory::Validate()
{
	if(m_activeAllocCount > 0 || m_currentTotalSize > 0)
	{
		// Output memory allocation stats.
		fprintf(
			stderr,
			"Memory Stats:\n"
			"  Context: \"%s\"\n"
			"  [Size]\n"
			"    Min:     %zu\n"
			"    Max:     %zu\n"
			"    Peak:    %zu\n"
			"    Current: %zu\n"
			"  [Count]\n"
			"    Active:  %zu\n"
			"    Total:   %zu\n"
			"    Malloc:  %zu\n"
			"    Realloc: %zu\n",
			m_context,
			m_minAllocSize,
			m_maxAllocSize,
			m_peakMemUsage,
			m_currentTotalSize,
			m_activeAllocCount,
			m_totalAllocCount,
			m_mallocCount,
			m_reallocCount
		);
		fflush(stderr);
	}

	// Verify there were no active allocations.
	ASSERT_EQ(m_activeAllocCount, 0);
	ASSERT_EQ(m_currentTotalSize, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void Memory::Reset()
{
	m_context = "Unknown";
	m_minAllocSize = 0;
	m_maxAllocSize = 0;
	m_peakMemUsage = 0;
	m_mallocCount = 0;
	m_reallocCount = 0;
	m_activeAllocCount = 0;
	m_totalAllocCount = 0;
	m_currentTotalSize = 0;
}

//----------------------------------------------------------------------------------------------------------------------

inline void* Memory::_malloc(const size_t memSize)
{
	assert(memSize > 0);

	const size_t totalSize = memSize + sizeof(size_t);

	size_t* const pMem = reinterpret_cast<size_t*>(malloc(totalSize));
	assert(pMem != nullptr);

	(*pMem) = memSize;

	// Update stats.
	HqMutex::Lock(m_mutex);

	++m_activeAllocCount;
	++m_totalAllocCount;
	++m_mallocCount;

	_onAlloc(memSize);
	HqMutex::Unlock(m_mutex);

	return pMem + 1;
}

//----------------------------------------------------------------------------------------------------------------------

inline void* Memory::_realloc(void* const pMem, const size_t memSize)
{
	assert(memSize > 0);

	const size_t totalNewSize = memSize + sizeof(size_t);

	// Get the real allocation pointer and its size.
	size_t* const pAlloc = (pMem) ? (reinterpret_cast<size_t*>(pMem) - 1) : nullptr;
	const size_t oldSize = (pAlloc) ? (*pAlloc) : 0;

	// Re-allocate the input memory to the new size.
	size_t* const pNewMem = reinterpret_cast<size_t*>(realloc(pAlloc, totalNewSize));
	assert(pNewMem != nullptr);

	// Assign the new size to the re-allocated memory.
	(*pNewMem) = memSize;

	// Update stats.
	HqMutex::Lock(m_mutex);

	assert(m_currentTotalSize >= oldSize);
	m_currentTotalSize -= oldSize;

	if(oldSize == 0)
	{
		++m_activeAllocCount;
		++m_totalAllocCount;
		++m_mallocCount;
	}
	else
	{
		++m_reallocCount;
	}

	_onAlloc(memSize);
	HqMutex::Unlock(m_mutex);

	return pNewMem + 1;
}

//----------------------------------------------------------------------------------------------------------------------

inline void Memory::_free(void* const pMem)
{
	size_t* const pAlloc = (pMem) ? (reinterpret_cast<size_t*>(pMem) - 1) : nullptr;

	if(pAlloc)
	{
		// Update stats.
		const size_t size = (pAlloc) ? (*pAlloc) : 0;

		HqMutex::Lock(m_mutex);

		assert(m_currentTotalSize >= size);
		m_currentTotalSize -= size;

		assert(m_activeAllocCount > 0);
		--m_activeAllocCount;

		HqMutex::Unlock(m_mutex);
		free(pAlloc);
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline void Memory::_onAlloc(const size_t memSize)
{
	if(memSize > m_maxAllocSize)
	{
		m_maxAllocSize = memSize;
	}

	if(memSize < m_minAllocSize)
	{
		m_minAllocSize = memSize;
	}

	m_currentTotalSize += memSize;
	assert(m_currentTotalSize != 0xFFFFFFFFFFFFFF80ull);

	if(m_currentTotalSize >= m_peakMemUsage)
	{
		m_peakMemUsage = m_currentTotalSize;
	}
}

//----------------------------------------------------------------------------------------------------------------------
