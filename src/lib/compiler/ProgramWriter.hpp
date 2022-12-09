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

#include "FunctionData.hpp"
#include "ObjectData.hpp"

#include "../base/String.hpp"
#include "../common/Array.hpp"

#include <deque>
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------------------

struct HqProgramWriter
{
	static HqProgramWriterHandle Create();
	static void Dispose(HqProgramWriterHandle hWriter);
	static bool Serialize(
		HqProgramWriterHandle hProgramWriter,
		HqCompilerHandle hCompiler,
		HqSerializerHandle hSerializer
	);

	static int LookupFunction(
		HqProgramWriterHandle hWriter,
		const char* const functionSignature,
		HqFunctionData** const ppOutFunction
	);

	static uint32_t AddConstant(HqProgramWriterHandle hWriter, HqString* const pString);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	typedef std::unordered_map<
		HqString*,
		bool,
		HqString::StlHash,
		HqString::StlCompare
	> DependencySet;

	typedef std::unordered_map<
		HqString*,
		uint32_t,
		HqString::StlHash,
		HqString::StlCompare
	> GlobalValueMap;

	typedef std::unordered_map<
		HqString*,
		uint32_t,
		HqString::StlHash,
		HqString::StlCompare
	> IndexMapString;

	struct ValueContainer
	{
		int type;

		union
		{
			HqString* pString;
			HqString* pObjectType;
		} as;
	};

	DependencySet dependencies;
	GlobalValueMap globals;
	HqFunctionData::StringToFunctionMap functions;
	HqObjectData::StringToObjectMap objectTypes;

	HqFunctionData::Bytecode initBytecode;

	IndexMapString indexMapString;

	std::deque<ValueContainer> constants;

	uint32_t nullIndex;
	uint32_t boolTrueIndex;
	uint32_t boolFalseIndex;
};

//----------------------------------------------------------------------------------------------------------------------
