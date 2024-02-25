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

//----------------------------------------------------------------------------------------------------------------------

class Memory
{
public:

	static Memory Instance;

	void SetContext(const char* context);
	void Validate();
	void Reset();


private:

	Memory();
	~Memory();

	void* _malloc(size_t memSize);
	void* _realloc(void* pMem, size_t memSize);
	void _free(void* pMem);
	void _onAlloc(size_t);

	HqMutex m_mutex;

	const char* m_context;

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

inline void Memory::SetContext(const char* const context)
{
	m_context = context;
}

//----------------------------------------------------------------------------------------------------------------------
