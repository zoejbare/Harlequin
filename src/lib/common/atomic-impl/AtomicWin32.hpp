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

#include "../../Harlequin.h"

#include <intrin.h>

#if defined(HQ_DATA_WIDTH_32_BIT)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

//----------------------------------------------------------------------------------------------------------------------

struct HqAtomic
{
	static __forceinline int8_t FetchAdd(volatile int8_t* const ptr, const int8_t value)
	{
		return _InterlockedExchangeAdd8((volatile char*) ptr, char(value));
	}

	static __forceinline int16_t FetchAdd(volatile int16_t* const ptr, const int16_t value)
	{
		return _InterlockedExchangeAdd16(ptr, value);
	}

	static __forceinline int32_t FetchAdd(volatile int32_t* const ptr, const int32_t value)
	{
		return _InterlockedExchangeAdd((volatile long*) ptr, long(value));
	}

	static __forceinline int64_t FetchAdd(volatile int64_t* const ptr, const int64_t value)
	{
#if defined(HQ_DATA_WIDTH_32_BIT)
		int64_t original;
		int64_t exchange;

		// Attempt a compare-exchange until it takes.  Caching the original value first per each
		// iteration is important for determining consistency of its data during the compare-exchange
		// operation since that will tell us if something modified it somewhere in between.
		do
		{
			original = (*ptr);
			exchange = original + value;
			MemoryBarrier();
		} while (_InterlockedCompareExchange64(ptr, exchange, original) != original);

		return original;

#else
		return _InterlockedExchangeAdd64(ptr, value);

#endif
	}
};

//----------------------------------------------------------------------------------------------------------------------
