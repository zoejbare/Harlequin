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

#include "../../Harlequin.h"

#include "../../base/String.hpp"

#include "../../common/Array.hpp"
#include "../../common/HashMap.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqSourceFile
{
	typedef HqArray<uint8_t> FileData;
	typedef HqHashMap<
		HqString*,
		HqString*,
		HqString::StlHash,
		HqString::StlCompare
	> ClassAliasMap;
	typedef HqHashMap<
		HqString*,
		bool,
		HqString::StlHash,
		HqString::StlCompare
	> NamespaceMap;

	static HqSourceFileHandle Load(HqToolContextHandle hToolCtx, const void* pFileData, size_t fileSize, int* pErrorReason);
	static void Dispose(HqSourceFileHandle hSrcFile);

	static int Parse(HqSourceFileHandle hSrcFile);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqToolContextHandle hToolCtx;

	FileData fileData;

	NamespaceMap namespaces;
	ClassAliasMap classAliases;
};

//----------------------------------------------------------------------------------------------------------------------
