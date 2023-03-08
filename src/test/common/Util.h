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

#include <Harlequin.h>

//----------------------------------------------------------------------------------------------------------------------

#if defined(_HQ_CONFIG_DEBUG)
	#define __HQ_TEST_NAME_PREFIX Dbg_

#elif defined(_HQ_CONFIG_FASTDEBUG)
	#define __HQ_TEST_NAME_PREFIX FDbg_

#elif defined(_HQ_CONFIG_RELEASE)
	#define __HQ_TEST_NAME_PREFIX Rel_

#else
	#error Unknown build configuration

#endif

#define __HQ_NAME_CONCAT_EVAL2(prefix, name) prefix ## name
#define __HQ_NAME_CONCAT_EVAL1(prefix, name) __HQ_NAME_CONCAT_EVAL2(prefix, name)

#define _HQ_TEST_NAME(test_name) __HQ_NAME_CONCAT_EVAL1(__HQ_TEST_NAME_PREFIX, test_name)

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------

extern void DefaultMessageCallback(void* pUserData, int messageType, const char* message);

//----------------------------------------------------------------------------------------------------------------------

extern HqVmInit GetDefaultHqVmInit(void* pUserData, HqMessageCallback onMessageFn, int reportLevel);
extern HqCompilerInit GetDefaultHqCompilerInit(void* pUserData, HqMessageCallback onMessageFn, int reportLevel);

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
