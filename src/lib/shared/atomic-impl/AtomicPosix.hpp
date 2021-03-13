/**
 * Copyright (c) 2021, Zoe J. Bare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

/*---------------------------------------------------------------------------------------------------------------------*/

#include <inttypes.h>

/*---------------------------------------------------------------------------------------------------------------------*/

struct XenonAtomic
{
	static inline __attribute__((always_inline)) int8_t FetchAdd(volatile int8_t* const ptr, const int8_t value)
	{
		return __atomic_fetch_add(ptr, value, __ATOMIC_SEQ_CST);
	}

	static inline __attribute__((always_inline)) int16_t FetchAdd(volatile int16_t* const ptr, const int16_t value)
	{
		return __atomic_fetch_add(ptr, value, __ATOMIC_SEQ_CST);
	}

	static inline __attribute__((always_inline)) int32_t FetchAdd(volatile int32_t* const ptr, const int32_t value)
	{
		return __atomic_fetch_add(ptr, value, __ATOMIC_SEQ_CST);
	}

	static inline __attribute__((always_inline)) int64_t FetchAdd(volatile int64_t* const ptr, const int64_t value)
	{
		return __atomic_fetch_add(ptr, value, __ATOMIC_SEQ_CST);
	}
};

/*---------------------------------------------------------------------------------------------------------------------*/
