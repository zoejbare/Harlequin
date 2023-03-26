//
// Copyright (c) 2022, Zoe J. Bare
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

#include <stdint.h>
#include <limits.h>

//----------------------------------------------------------------------------------------------------------------------

namespace HqStd 
{
	inline size_t Fnv1aHash(const uint8_t* const data, const size_t length)
	{
#ifdef HQ_CPU_WORD_64_BIT
		const uint64_t fnvOffsetBasis = 0xCBF29CE484222325ull;
		const uint64_t fnvPrime = 0x00000100000001B3ull;

		uint64_t output = fnvOffsetBasis;

#else
		const uint32_t fnvOffsetBasis = 0x811C9DC5ul;
		const uint32_t fnvPrime = 0x01000193ul;

		uint32_t output = fnvOffsetBasis;

#endif

		// Calculate the FNV1a hash from the input data.
		for(size_t i = 0; i < length; ++i)
		{
			output ^= data[i];
			output *= fnvPrime;
		}

		return size_t(output);
	}

	template<class TArg, class TResult>
	struct _UnaryFunction
	{
		typedef TArg argument_type;
		typedef TResult result_type;
	};

	template<class T>
	struct _BitwiseHash : public _UnaryFunction<T, size_t>
	{
		size_t operator()(const T& key) const
		{
			return Fnv1aHash(reinterpret_cast<const uint8_t*>(&key), sizeof(T));
		}
	};
	
	template<class T>
	struct Hash : public _BitwiseHash<T>
	{
	};

	template<>
	struct Hash<bool> : public _BitwiseHash<bool>
	{
	};

	template<>
	struct Hash<char> : public _BitwiseHash<char>
	{
	};

	template<>
	struct Hash<signed char> : public _BitwiseHash<signed char>
	{
	};

	template<>
	struct Hash<unsigned char> : public _BitwiseHash<unsigned char>
	{
	};

#if _HAS_CHAR16_T_LANGUAGE_SUPPORT
	template<>
	struct Hash<char16_t> : public _BitwiseHash<char16_t>
	{
	};

	template<>
	struct Hash<char32_t> : public _BitwiseHash<char32_t>
	{
	};
 #endif

	template<>
	struct Hash<wchar_t> : public _BitwiseHash<wchar_t>
	{
	};

	template<>
	struct Hash<short> : public _BitwiseHash<short>
	{
	};

	template<>
	struct Hash<unsigned short> : public _BitwiseHash<unsigned short>
	{
	};

	template<>
	struct Hash<int>
		: public _BitwiseHash<int>
	{
	};

	template<>
	struct Hash<unsigned int> : public _BitwiseHash<unsigned int>
	{
	};

	template<>
	struct Hash<long> : public _BitwiseHash<long>
	{
	};

	template<>
	struct Hash<unsigned long> : public _BitwiseHash<unsigned long>
	{
	};

	template<>
	struct Hash<long long> : public _BitwiseHash<long long>
	{
	};

	template<>
	struct Hash<unsigned long long> : public _BitwiseHash<unsigned long long>
	{
	};

	template<>
	struct Hash<float> : public _BitwiseHash<float>
	{
		typedef float                  key_type;
		typedef _BitwiseHash<key_type> base_type;

		size_t operator()(const key_type& key) const
		{
			return base_type::operator()(key);
		}
	};

	template<>
	struct Hash<double> : public _BitwiseHash<double>
	{
		typedef double                 key_type;
		typedef _BitwiseHash<key_type> base_type;

		size_t operator()(const key_type& key) const
		{
			return base_type::operator()(key);
		}
	};

	template<>
	struct Hash<long double> : public _BitwiseHash<long double>
	{
		typedef long double            key_type;
		typedef _BitwiseHash<key_type> base_type;

		size_t operator()(const key_type& key) const
		{
			return base_type::operator()(key);
		}
	};

	template<class T>
	struct Hash<T*>: public _BitwiseHash<T*>
	{
	};
}

//----------------------------------------------------------------------------------------------------------------------
