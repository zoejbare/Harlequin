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

#include "../../base/Reference.hpp"
#include "../../base/String.hpp"

#include "../../common/Array.hpp"
#include "../../common/HashMap.hpp"

//----------------------------------------------------------------------------------------------------------------------

namespace antlr4
{
	class ANTLRInputStream;
	class CommonTokenStream;

	namespace tree
	{
		class ParseTree;
	}
}

class HarlequinLexer;
class HarlequinParser;

//----------------------------------------------------------------------------------------------------------------------

struct HqSourceFile
{
	typedef HqArray<HqString*> LineArray;
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

	static HqSourceFileHandle Load(HqToolContextHandle hToolCtx, const char* filePath,int* pErrorReason);

	static int32_t AddRef(HqSourceFileHandle hSrcFile);
	static int32_t Release(HqSourceFileHandle hSrcFile);

	static int Parse(HqSourceFileHandle hSrcFile);
	static bool WasParsedSuccessfully(HqSourceFileHandle hSrcFile);

	static void prv_onDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqReference ref;
	HqToolContextHandle hToolCtx;
	HqSerializerHandle hSerializer;

	antlr4::ANTLRInputStream* pInputStream;
	antlr4::CommonTokenStream* pTokenStream;

	HarlequinLexer* pLexer;
	HarlequinParser* pParser;

	antlr4::tree::ParseTree* pAstRoot;

	LineArray fileLines;

	NamespaceMap namespaces;
	ClassAliasMap classAliases;

	bool parsed;
};

//----------------------------------------------------------------------------------------------------------------------
