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

#ifndef _XENON_BUILD_ENV_H_
#define _XENON_BUILD_ENV_H_

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(__ANDROID__)
	#define XENON_PLATFORM_ANDROID

#elif defined(__ORBIS__)
	#define XENON_PLATFORM_PS4

#elif defined(__PS3__)
	#define XENON_PLATFORM_PS3

#elif defined(__psp2__)
	#define XENON_PLATFORM_PSVITA

#elif defined(_XBOX)
	#include <xtl.h>
	#if _XBOX_VER >= 200 && _XBOX_VER < 300
		#define XENON_PLATFORM_XBOX_360

	#else
		#error "Unrecognized Xbox platform!"

	#endif

#elif defined(__APPLE__)
	#define XENON_PLATFORM_MAC_OS

#elif defined(__linux__)
	#define XENON_PLATFORM_LINUX

#elif defined(_WIN32)
	#define XENON_PLATFORM_WINDOWS

#else
	#define XENON_PLATFORM_UNKNOWN

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(__powerpc__) || defined(__powerpc64__) || defined(__PPC__) || defined(__PPC64__)
	#define XENON_CPU_TYPE_PPC

#elif defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || defined(_M_IX86) || defined(_M_X64) || defined(_M_AMD64)
	#define XENON_CPU_TYPE_X86

#elif defined(__arm__) || defined(__aarch64__)
	#define XENON_CPU_TYPE_ARM

#else
	#define XENON_CPU_TYPE_UNKNOWN

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__) || defined(__powerpc64__) || defined(__PPC64__) || defined(__aarch64__)
	#define XENON_CPU_WIDTH_64_BIT

#elif defined(__i386__) || defined(__powerpc__) || defined(__PPC__) || defined(__arm__) || defined(_M_IX86)
	#define XENON_CPU_WIDTH_32_BIT

#else
	#define XENON_CPU_WIDTH_UNKNOWN

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) \
	|| defined(__MIPSEL) || defined(__MIPSEL__) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)) \
	|| (defined(__FLOAT_WORD_ORDER__) && (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)) || defined(XENON_PLATFORM_WINDOWS)
	#define XENON_CPU_ENDIAN_LITTLE

#elif defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) \
	|| defined(__MIPSEB) || defined(__MIPSEB__) || (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) \
	|| (defined(__FLOAT_WORD_ORDER__) && (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__))
	#define XENON_CPU_ENDIAN_BIG

#else
	#define XENON_CPU_ENDIAN_UNKNOWN

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

/**
 * Define XENON_ALLOW_UNKNOWN_BUILD_OPTIONS to permit building in an environment that is not
 * officially recognized by the Xenon build setup.
 */
#ifndef XENON_ALLOW_UNKNOWN_BUILD_OPTIONS
	#ifdef XENON_PLATFORM_UNKNOWN
		#error "Unknown build platform"
	#endif

	#ifdef XENON_CPU_TYPE_UNKNOWN
		#error "Unknown CPU type"
	#endif

	#ifdef XENON_CPU_WIDTH_UNKNOWN
		#error "Unknown CPU bit width"
	#endif

	#ifdef XENON_CPU_ENDIAN_UNKNOWN
		#error "Unknown CPU endianness"
	#endif
#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#if defined(XENON_PLATFORM_WINDOWS)
	#if defined(XENON_BUILD_STATIC_LIB)
		#define XENON_BASE_API
		#define XENON_MAIN_API

	#else
		#if defined(XENON_BUILD_BASE_LIB_EXPORT)
			#define XENON_BASE_API __declspec(dllexport)

		#else
			#define XENON_BASE_API __declspec(dllimport)

		#endif

		#if defined(XENON_BUILD_MAIN_LIB_EXPORT)
			#define XENON_MAIN_API __declspec(dllexport)

		#else
			#define XENON_MAIN_API __declspec(dllimport)

		#endif
	#endif

#else
	#define XENON_BASE_API
	#define XENON_MAIN_API

#endif

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* _XENON_BUILD_ENV_H_ */
