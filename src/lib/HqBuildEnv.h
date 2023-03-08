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

#ifndef _HQ_BUILD_ENV_H_
#define _HQ_BUILD_ENV_H_

/*---------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_HQ_BUILD_FOR_PS3_)
	#define HQ_PLATFORM_PS3

#elif defined(_HQ_BUILD_FOR_PS4_)
	#define HQ_PLATFORM_PS4

#elif defined(_HQ_BUILD_FOR_PS5_)
	#define HQ_PLATFORM_PS5

#elif defined(_HQ_BUILD_FOR_PSVITA_)
	#define HQ_PLATFORM_PSVITA

#elif defined(__ANDROID__)
	#define HQ_PLATFORM_ANDROID

#elif defined(__APPLE__)
	#define HQ_PLATFORM_MAC_OS

#elif defined(__linux__)
	#define HQ_PLATFORM_LINUX

#elif defined(_WIN32)
	#define HQ_PLATFORM_WINDOWS

#else
	#define HQ_PLATFORM_UNKNOWN

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(__powerpc__) || defined(__powerpc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_M_PPC) || defined(_M_PPCBE)
	#define HQ_CPU_TYPE_PPC

#elif defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || defined(_M_IX86) || defined(_M_X64) || defined(_M_AMD64)
	#define HQ_CPU_TYPE_X86

#elif defined(__arm__) || defined(__aarch64__)
	#define HQ_CPU_TYPE_ARM

#else
	#define HQ_CPU_TYPE_UNKNOWN

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__) || defined(__powerpc64__) || defined(__PPC64__) \
	|| defined(__aarch64__) || defined(_M_PPC) || defined(_M_PPCBE)
	#define HQ_DATA_WIDTH_64_BIT

	#if (UINTPTR_MAX == UINT_MAX)
		/* Some older platforms use a 64-bit CPU, but still define their native word to 32 bits. */
		#undef HQ_DATA_WIDTH_64_BIT
		#define HQ_DATA_WIDTH_32_BIT
	#endif

#elif defined(__i386__) || defined(__powerpc__) || defined(__PPC__) || defined(__arm__) || defined(_M_IX86)
	#define HQ_DATA_WIDTH_32_BIT

#else
	#define HQ_DATA_WIDTH_UNKNOWN

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) \
	|| defined(__MIPSEL) || defined(__MIPSEL__) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)) \
	|| (defined(__FLOAT_WORD_ORDER__) && (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)) || defined(HQ_PLATFORM_WINDOWS)
	#define HQ_CPU_ENDIAN_LITTLE

#elif defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) \
	|| defined(__MIPSEB) || defined(__MIPSEB__) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) \
	|| (defined(__FLOAT_WORD_ORDER__) && (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)) || defined(_M_PPCBE)
	#define HQ_CPU_ENDIAN_BIG

#else
	#define HQ_CPU_ENDIAN_UNKNOWN

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

/**
 * Define HQ_ALLOW_UNKNOWN_BUILD_OPTIONS to permit building in an environment that is not
 * officially recognized by the Harlequin build setup.
 */
#ifndef HQ_ALLOW_UNKNOWN_BUILD_OPTIONS
	#ifdef HQ_PLATFORM_UNKNOWN
		#error "Unknown build platform"
	#endif

	#ifdef HQ_CPU_TYPE_UNKNOWN
		#error "Unknown CPU type"
	#endif

	#ifdef HQ_DATA_WIDTH_UNKNOWN
		#error "Unknown data native bit width"
	#endif

	#ifdef HQ_CPU_ENDIAN_UNKNOWN
		#error "Unknown CPU endianness"
	#endif
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(HQ_PLATFORM_WINDOWS)
	#if defined(HQ_BUILD_STATIC_LIB)
		#define HQ_BASE_API
		#define HQ_MAIN_API

	#else
		#if defined(HQ_BUILD_BASE_LIB_EXPORT)
			#define HQ_BASE_API __declspec(dllexport)

		#else
			#define HQ_BASE_API __declspec(dllimport)

		#endif

		#if defined(HQ_BUILD_MAIN_LIB_EXPORT)
			#define HQ_MAIN_API __declspec(dllexport)

		#else
			#define HQ_MAIN_API __declspec(dllimport)

		#endif
	#endif

	#define HQ_NATIVE_API __declspec(dllexport)

#else
	#define HQ_BASE_API
	#define HQ_MAIN_API
	#define HQ_NATIVE_API

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* _HQ_BUILD_ENV_H_ */
