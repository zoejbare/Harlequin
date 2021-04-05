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

#include "Function.hpp"

#include "../shared/Array.hpp"
#include "../shared/String.hpp"

#include <SkipProbe/SkipProbe.hpp>

#include <deque>

//----------------------------------------------------------------------------------------------------------------------

struct XenonProgramWriter
{
	static XenonProgramWriterHandle Create();
	static void Dispose(XenonProgramWriterHandle hWriter);
	static bool Serialize(
		XenonProgramWriterHandle hProgramWriter,
		XenonCompilerHandle hCompiler,
		XenonSerializerHandle hSerializer,
		const int endianness
	);

	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const int8_t value);
	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const int16_t value);
	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const int32_t value);
	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const int64_t value);

	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const uint8_t value);
	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const uint16_t value);
	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const uint32_t value);
	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const uint64_t value);

	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const float value);
	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, const double value);

	static uint32_t AddConstant(XenonProgramWriterHandle hWriter, XenonString* const pString);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	typedef SkipProbe::HashMap<
		XenonString*,
		bool,
		XenonString::StlHash,
		XenonString::StlCompare
	> DependencySet;

	typedef SkipProbe::HashMap<
		XenonString*,
		uint32_t,
		XenonString::StlHash,
		XenonString::StlCompare
	> GlobalValueMap;

	typedef SkipProbe::HashMap<int8_t, uint32_t> IndexMapInt8;
	typedef SkipProbe::HashMap<int16_t, uint32_t> IndexMapInt16;
	typedef SkipProbe::HashMap<int32_t, uint32_t> IndexMapInt32;
	typedef SkipProbe::HashMap<int64_t, uint32_t> IndexMapInt64;

	typedef SkipProbe::HashMap<uint8_t, uint32_t> IndexMapUint8;
	typedef SkipProbe::HashMap<uint16_t, uint32_t> IndexMapUint16;
	typedef SkipProbe::HashMap<uint32_t, uint32_t> IndexMapUint32;
	typedef SkipProbe::HashMap<uint64_t, uint32_t> IndexMapUint64;

	typedef SkipProbe::HashMap<uint32_t, uint32_t> IndexMapFloat32;
	typedef SkipProbe::HashMap<uint64_t, uint32_t> IndexMapFloat64;

	typedef SkipProbe::HashMap<
		XenonString*,
		uint32_t,
		XenonString::StlHash,
		XenonString::StlCompare
	> IndexMapString;

	struct ValueContainer
	{
		int type;

		union
		{
			bool boolean;

			int8_t int8;
			int16_t int16;
			int32_t int32;
			int64_t int64;

			uint8_t uint8;
			uint16_t uint16;
			uint32_t uint32;
			uint64_t uint64;

			float float32;
			double float64;

			XenonString* pString;
		} as;
	};

	DependencySet dependencies;
	GlobalValueMap globals;
	XenonFunction::StringToFunctionMap functions;

	IndexMapInt8 indexMapInt8;
	IndexMapInt16 indexMapInt16;
	IndexMapInt32 indexMapInt32;
	IndexMapInt64 indexMapInt64;

	IndexMapUint8 indexMapUint8;
	IndexMapUint16 indexMapUint16;
	IndexMapUint32 indexMapUint32;
	IndexMapUint64 indexMapUint64;

	IndexMapFloat32 indexMapFloat32;
	IndexMapFloat64 indexMapFloat64;

	IndexMapString indexMapString;

	std::deque<ValueContainer> constants;

	uint32_t nullIndex;
	uint32_t boolTrueIndex;
	uint32_t boolFalseIndex;
};

//----------------------------------------------------------------------------------------------------------------------
