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
	#define __HQ_TEST_NAME_CONFIG _dbg

#elif defined(_HQ_CONFIG_FASTDEBUG)
	#define __HQ_TEST_NAME_CONFIG _fdbg

#elif defined(_HQ_CONFIG_RELEASE)
	#define __HQ_TEST_NAME_CONFIG _rel

#else
	#error Unknown build configuration

#endif

//----------------------------------------------------------------------------------------------------------------------

#if defined(HQ_CPU_TYPE_X86)
	#if defined(HQ_CPU_WORD_64_BIT)
		#define __HQ_TEST_NAME_ARCH _x64
	#elif defined(HQ_CPU_WORD_32_BIT)
		#define __HQ_TEST_NAME_ARCH _x86
	#endif
#elif defined(HQ_CPU_TYPE_PPC)
	#if defined(HQ_CPU_WORD_64_BIT)
		#define __HQ_TEST_NAME_ARCH _ppc64
	#elif defined(HQ_CPU_WORD_32_BIT)
		#define __HQ_TEST_NAME_ARCH _ppc
	#endif
#elif defined(HQ_CPU_TYPE_ARM)
	#if defined(HQ_CPU_WORD_64_BIT)
		#define __HQ_TEST_NAME_ARCH _arm64
	#elif defined(HQ_CPU_WORD_32_BIT)
		#define __HQ_TEST_NAME_ARCH _arm
	#endif

#endif

//----------------------------------------------------------------------------------------------------------------------

#if defined(__clang__)
	#define __HQ_TEST_NAME_COMPILER _clang

#elif defined(__GNUC__)
	#define __HQ_TEST_NAME_COMPILER _gcc

#elif defined(__CC_ARM)
	#define __HQ_TEST_NAME_COMPILER _armcc

#elif defined(_MSC_VER)
	#define __HQ_TEST_NAME_COMPILER _msvc

#elif defined(__INTEL_COMPILER)
	#define __HQ_TEST_NAME_COMPILER _icc

#else
	#error Unknown compiler

#endif

//----------------------------------------------------------------------------------------------------------------------

#define __HQ_NAME_CONCAT_EVAL2(name, config, arch, compiler) name ## config ## arch ## compiler
#define __HQ_NAME_CONCAT_EVAL1(name, config, arch, compiler) __HQ_NAME_CONCAT_EVAL2(name, config, arch, compiler)

#define _HQ_TEST_NAME(test_name) \
	__HQ_NAME_CONCAT_EVAL1(test_name, __HQ_TEST_NAME_CONFIG, __HQ_TEST_NAME_ARCH, __HQ_TEST_NAME_COMPILER)

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------------------------------------------------

extern void DefaultMessageCallback(void* pUserData, int messageType, const char* message);

//----------------------------------------------------------------------------------------------------------------------

extern HqVmInit GetDefaultHqVmInit(void* pUserData, HqMessageCallback onMessageFn, int reportLevel);
extern HqToolCoreInit GetDefaultHqToolCoreInit(void* pUserData, HqMessageCallback onMessageFn, int reportLevel);

//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
