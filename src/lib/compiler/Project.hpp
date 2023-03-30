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

	typedef std::unordered_map<
		HqString*, 
		Reference, 
		HqString::StlHash, 
		HqString::StlCompare
	> ReferenceMap;

	typedef std::vector<HqString*> StringList;

	struct File
	{
		StringList defines;

		HqString* pPath;
	};

	typedef std::unordered_map<
		HqString*, 
		File, 
		HqString::StlHash, 
		HqString::StlCompare
	> FileMap;

	static HqProjectHandle Create(HqCompilerHandle hCompiler);
	static void Dispose(HqProjectHandle hProject);

	static int SetOutput(HqProjectHandle hProject, HqString* pPath);
	static int AddReference(HqProjectHandle hProject, HqString* pPath, HqString* pName);
	static int AddFile(HqProjectHandle hProject, HqString* pPath);
	static int AddDefine(HqProjectHandle hProject, HqString* pDefine);
	static int AddFileDefine(HqProjectHandle hProject, HqString* pFilePath, HqString* pDefine);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqCompilerHandle hCompiler;

	HqString* pOutput;

	StringList defines;
	ReferenceMap references;
	FileMap files;
};

//----------------------------------------------------------------------------------------------------------------------
