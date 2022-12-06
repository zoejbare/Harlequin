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

#include <stdint.h>
#include <time.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" uint64_t _HqClockImplGetFrequency()
{
	return 1000000000;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" uint64_t _HqClockImplGetTimestamp()
{
#if defined(HQ_PLATFORM_MAC_OS) || defined(HQ_PLATFORM_PS4)
	return uint64_t(clock_gettime_nsec_np(CLOCK_UPTIME_RAW));

#else
	timespec tspec = { 0, 0 };
	clock_gettime(CLOCK_MONOTONIC, &tspec);

	return uint64_t(tspec.tv_nsec) + (uint64_t(tspec.tv_sec) * 1000000000);

#endif
}

//----------------------------------------------------------------------------------------------------------------------
