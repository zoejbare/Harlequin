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

#include "../../Harlequin.h"

#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//----------------------------------------------------------------------------------------------------------------------

inline wchar_t* _HqSysWin32MakeWideStr(const char* const string)
{
	assert(string != nullptr);

	// Calculate the lengths of the input and output strings.
	const size_t utf8Len = strlen(string);
	const size_t wideLen = (size_t) MultiByteToWideChar(
		CP_UTF8, 0,
		string,
		DWORD(utf8Len),
		nullptr, 0
	);

	// Allocate the output string.
	wchar_t* const outputString = reinterpret_cast<wchar_t*>(HqMemAlloc(sizeof(wchar_t) * (wideLen + 1)));
	assert(outputString != nullptr);

	// Convert the input string.
	MultiByteToWideChar(
		CP_UTF8, 0,
		string,
		DWORD(utf8Len),
		outputString,
		int(wideLen)
	);

	// Add a null-terminator to the output string.
	outputString[wideLen] = L'\0';

	return outputString;
}

//----------------------------------------------------------------------------------------------------------------------

inline char* _HqSysWin32MakeMultiByteStr(const wchar_t* const string)
{
	assert(string != nullptr);

	// Calculate the lengths of the input and output strings.
	const size_t wideLen = wcslen(string);
	const size_t utf8Len = (size_t) WideCharToMultiByte(
		CP_UTF8, 0,
		string,
		DWORD(wideLen),
		nullptr, 0,
		nullptr,
		nullptr
	);

	// Allocate the output string.
	char* const outputString = reinterpret_cast<char*>(HqMemAlloc(sizeof(char) * (utf8Len + 1)));
	assert(outputString != nullptr);

	// Convert the input string.
	WideCharToMultiByte(
		CP_UTF8, 0,
		string,
		DWORD(wideLen),
		outputString,
		int(utf8Len),
		nullptr,
		nullptr
	);

	// Add a null-terminator to the output string.
	outputString[utf8Len] = '\0';

	return outputString;
}

//----------------------------------------------------------------------------------------------------------------------

inline DWORD _HqSysWin32GetFileAttr(const char* const path)
{
	const wchar_t* const widePath = _HqSysWin32MakeWideStr(path);

	const DWORD fileAttr = GetFileAttributesW(widePath);
	HqMemFree((void*) widePath);

	return fileAttr;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _HqSysIsFile(const char* const path)
{
	if(!path || path[0] == '\0')
	{
		return false;
	}

	const DWORD fileAttr = _HqSysWin32GetFileAttr(path);
	if(fileAttr == INVALID_FILE_ATTRIBUTES || (fileAttr & FILE_ATTRIBUTE_DIRECTORY) != 0)
	{
		// The input path cannot be examined or is a directory.
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _HqSysIsDir(const char* const path)
{
	if(!path || path[0] == '\0')
	{
		return false;
	}

	const DWORD fileAttr = _HqSysWin32GetFileAttr(path);
	if(fileAttr == INVALID_FILE_ATTRIBUTES || (fileAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)
	{
		// The input path cannot be examined or is a file.
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _HqSysIsExe(const char* const path)
{
	// All files on windows are implicitly executable.
	return _HqSysIsFile(path);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqSysListDir(
	void* const pUserData, 
	const char* const rootPath, 
	HqListDirectoryCallback onFileFound,
	HqListDirectoryCallback onDirFound
)
{
	assert(rootPath != nullptr);
	assert(onFileFound != nullptr);
	assert(onDirFound != nullptr);

	// Allocate enough space for a custom path spec we can use for searching the input directory.
	const size_t pathLen = strlen(rootPath);
	char* const rootPathSpec = reinterpret_cast<char*>(HqMemAlloc(sizeof(char) * (pathLen + 3)));

	// Copy the input path to the path spec string.
	memcpy(rootPathSpec, rootPath, sizeof(char) * pathLen);

	// Finish constructing the path spec string.
	rootPathSpec[pathLen + 0] = '\\';
	rootPathSpec[pathLen + 1] = '*';
	rootPathSpec[pathLen + 2] = '\0';

	// Convert the path spec to a wide string.
	const wchar_t* const wideRootPathSpec = _HqSysWin32MakeWideStr(rootPathSpec);
	HqMemFree((void*) rootPathSpec);

	// Find the first entry in the directory to kick things off.
	WIN32_FIND_DATAW findData;
	HANDLE hFind = FindFirstFileW(wideRootPathSpec, &findData);

	// We don't need the path spec wide string anymore.
	HqMemFree((void*) wideRootPathSpec);

	do
	{
		if(_wcsicmp(findData.cFileName, L".") == 0 || _wcsicmp(findData.cFileName, L"..") == 0)
		{
			// Discard the "." and ".." entries.
			continue;
		}

		const char* const entryName = _HqSysWin32MakeMultiByteStr(findData.cFileName);

		if((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			// The current entry is a directory.
			onDirFound(pUserData, rootPath, entryName);
		}
		else
		{
			// The current entry is a file.
			onFileFound(pUserData, rootPath, entryName);
		}

		HqMemFree((void*) entryName);
	}
	// Find the next entry in the directory.
	while(FindNextFileW(hFind, &findData));
}

//----------------------------------------------------------------------------------------------------------------------
