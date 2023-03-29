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

#include "../Harlequin.h"

#include "../base/String.hpp"

#include <rapidxml_ns.hpp>

#include <unordered_map>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------

struct HqProject
{
	struct Reference
	{
		HqString* pPath;
		HqString* pName;
	};

	struct Define
	{
		HqString* pName;
		HqString* pValue;
	};

	typedef std::unordered_map<
		HqString*, 
		Reference, 
		HqString::StlHash, 
		HqString::StlCompare
	> ReferenceMap;

	typedef std::vector<HqString*> StringList;
	typedef std::vector<Define>    DefineList;

	struct File
	{
		DefineList defines;

		HqString* pPath;
	};

	typedef std::unordered_map<
		HqString*, 
		File, 
		HqString::StlHash, 
		HqString::StlCompare
	> FileMap;

	static HqProjectHandle Load(HqCompilerHandle hCompiler, const void* pProjectFileData, size_t projectFileSize);
	static void Dispose(HqProjectHandle hProject);

	static bool prv_loadXmlDoc(HqProjectHandle, HqReportHandle, const rapidxml_ns::xml_document<>&);
	static HqString* prv_createString(const char*, size_t);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqCompilerHandle hCompiler;

	HqString* pOutput;

	DefineList defines;
	ReferenceMap references;
	FileMap files;
};

//----------------------------------------------------------------------------------------------------------------------
