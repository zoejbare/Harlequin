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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include <base/Mutex.hpp>

#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------

class MemoryHandler
{
public:

	static MemoryHandler Instance;

	void Initialize();
	void Shutdown();

	bool HasActiveAllocations() const;
	void PrintStats(FILE* pOutputStream) const;

	size_t GetActiveCount() const;
	size_t GetCurrentSize() const;

private:

	MemoryHandler();

	void* prv_malloc(size_t memSize);
	void* prv_realloc(void* pMem, size_t memSize);
	void prv_free(void* pMem);
	void prv_onAlloc(size_t);

	HqMutex m_mutex;

	size_t m_minAllocSize;
	size_t m_maxAllocSize;
	size_t m_peakMemUsage;
	size_t m_mallocCount;
	size_t m_reallocCount;
	size_t m_activeAllocCount;
	size_t m_totalAllocCount;
	size_t m_currentTotalSize;
};

//----------------------------------------------------------------------------------------------------------------------

inline MemoryHandler::MemoryHandler()
	: m_mutex()
	, m_minAllocSize(0)
	, m_maxAllocSize(0)
	, m_peakMemUsage(0)
	, m_mallocCount(0)
	, m_reallocCount(0)
	, m_activeAllocCount(0)
	, m_totalAllocCount(0)
	, m_currentTotalSize(0)
{
}

//----------------------------------------------------------------------------------------------------------------------

inline bool MemoryHandler::HasActiveAllocations() const
{
	return (m_activeAllocCount > 0) || (m_currentTotalSize > 0);
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t MemoryHandler::GetActiveCount() const
{
	return m_activeAllocCount;
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t MemoryHandler::GetCurrentSize() const
{
	return m_currentTotalSize;
}

//----------------------------------------------------------------------------------------------------------------------
