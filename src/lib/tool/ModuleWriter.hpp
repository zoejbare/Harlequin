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

#include "../common/module-format/FileHeader.hpp"
#include "../common/module-format/TableOfContents.hpp"

#include <deque>
#include <unordered_map>

//----------------------------------------------------------------------------------------------------------------------

struct HqModuleWriter
{
	static HqModuleWriterHandle Create(HqToolContextHandle hToolCtx);
	static void Dispose(HqModuleWriterHandle hWriter);
	static bool Serialize(HqModuleWriterHandle hModuleWriter, HqSerializerHandle hSerializer);

	static int LookupFunction(
		HqModuleWriterHandle hWriter,
		const char* const functionSignature,
		HqFunctionData** const ppOutFunction
	);

	static uint32_t AddString(HqModuleWriterHandle hWriter, HqString* const pString);

	static bool prv_writeFileHeader(HqSerializerHandle, const HqModuleFileHeader&, int&, size_t&);
	static bool prv_writeTableOfContents(HqSerializerHandle, const HqModuleTableOfContents&, int&, size_t&);

	static bool prv_writeBuffer(HqSerializerHandle, const void*, size_t, int&, size_t&);
	static bool prv_writeString(HqSerializerHandle, const char*, size_t, int&, size_t&);
	static bool prv_writeUint8(HqSerializerHandle, uint8_t, int&, size_t&);
	static bool prv_writeUint16(HqSerializerHandle, uint16_t, int&, size_t&);
	static bool prv_writeUint32(HqSerializerHandle, uint32_t, int&, size_t&);
	static bool prv_writeBool8(HqSerializerHandle, bool, int&, size_t&);
	static bool prv_writeBool32(HqSerializerHandle, bool, int&, size_t&);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	typedef std::unordered_set<
		HqString*,
		HqString::StlHash,
		HqString::StlCompare
	> DependencySet;

	typedef std::unordered_set<
		HqString*,
		HqString::StlHash,
		HqString::StlCompare
	> GlobalValueSet;

	typedef std::unordered_map<
		HqString*,
		size_t,
		HqString::StlHash,
		HqString::StlCompare
	> StringToSizeMap;

	HqToolContextHandle hToolCtx;

	DependencySet dependencies;
	GlobalValueSet globals;
	HqFunctionData::StringToFunctionMap functions;
	HqObjectData::StringToObjectMap objectTypes;

	HqFunctionData::Bytecode initBytecode;

	StringToSizeMap stringIndices;
	std::deque<HqString*> stringConstants;
};

//----------------------------------------------------------------------------------------------------------------------
