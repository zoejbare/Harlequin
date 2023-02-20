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

#include "../common/ListDirectoryCallback.hpp"

//----------------------------------------------------------------------------------------------------------------------

extern "C"
{
	bool _HqSysIsFile(const char*);
	bool _HqSysIsDir(const char*);
	bool _HqSysIsExe(const char*);
	void _HqSysListDir(void*, const char*, HqListDirectoryCallback, HqListDirectoryCallback);
	size_t _HqSysUtf32ToUtf8(char*, char32_t);
	char32_t _HqSysUtf8ToUtf32(size_t*, const char*);
}

//----------------------------------------------------------------------------------------------------------------------

struct HQ_BASE_API HqSys
{
	static bool IsFile(const char* const path)
	{
		return _HqSysIsFile(path);
	}

	static bool IsDirectory(const char* const path)
	{
		return _HqSysIsDir(path);
	}

	static bool IsExecutable(const char* const path)
	{
		return _HqSysIsExe(path);
	}

	static void ListDirectory(
		void* pUserData, 
		const char* rootPath, 
		HqListDirectoryCallback onFileFound, 
		HqListDirectoryCallback onDirFound)
	{
		_HqSysListDir(pUserData, rootPath, onFileFound, onDirFound);
	}

	static size_t ConvertUtf32ToUtf8(char* const pOutMbSeq, const char32_t codePoint)
	{
		return _HqSysUtf32ToUtf8(pOutMbSeq, codePoint);
	}

	static char32_t ConvertUtf8ToUtf32(size_t* const pOutMbLen, const char* const pMbSeq)
	{
		return _HqSysUtf8ToUtf32(pOutMbLen, pMbSeq);
	}
};

//----------------------------------------------------------------------------------------------------------------------
