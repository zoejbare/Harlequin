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
#include "../../common/ListDirectoryCallback.hpp"

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

#include <sys/stat.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" wchar_t* HqSysMakeWideStr(const char* const string)
{
	assert(string != nullptr);

	// Calculate the lengths of the input and output strings.
	const size_t utf8Len = strlen(string);
	const size_t wideLen = mbstowcs(nullptr, string, utf8Len);

	// Allocate the output string.
	wchar_t* const outputString = reinterpret_cast<wchar_t*>(HqMemAlloc(sizeof(wchar_t) * (wideLen + 1)));
	assert(outputString != nullptr);

	// Convert the input string.
	mbstowcs(outputString, string, utf8Len);

	// Add a null-terminator to the output string.
	outputString[wideLen] = L'\0';

	return outputString;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" char* HqSysMakeUtf8Str(const wchar_t* const string)
{
	assert(string != nullptr);

	// Calculate the lengths of the input and output strings.
	const size_t wideLen = wcslen(string);
	const size_t utf8Len = wcstombs(nullptr, string, wideLen);

	// Allocate the output string.
	char* const outputString = reinterpret_cast<char*>(HqMemAlloc(sizeof(char) * (utf8Len + 1)));
	assert(outputString != nullptr);

	// Convert the input string.
	wcstombs(outputString, string, wideLen);

	// Add a null-terminator to the output string.
	outputString[utf8Len] = '\0';

	return outputString;
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool HqSysIsFile(const char* const path)
{
	if(!path || path[0] == '\0')
	{
		return false;
	}

	struct stat statBuf;

	const int result = stat(path, &statBuf);
	if(result != 0)
	{
		// The input path cannot be examined.
		return false;
	}

	return S_ISREG(statBuf.st_mode);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool HqSysIsDir(const char* const path)
{
	if(!path || path[0] == '\0')
	{
		return false;
	}

	struct stat statBuf;

	const int result = stat(path, &statBuf);
	if(result != 0)
	{
		// The input path cannot be examined.
		return false;
	}

	return S_ISDIR(statBuf.st_mode);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool HqSysIsExe(const char* const path)
{
	if(!path || path[0] == '\0')
	{
		return false;
	}

	struct stat statBuf;

	const int result = stat(path, &statBuf);
	if(result != 0)
	{
		// The input path cannot be examined.
		return false;
	}

	// Directories are usually implicitly executable, so we need to exclude them so we only check files and symlinks.
	return !S_ISDIR(statBuf.st_mode) && (statBuf.st_mode & S_IEXEC);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void HqSysListDir(
	void* const pUserData, 
	const char* const rootPath, 
	HqListDirectoryCallback onFileFound,
	HqListDirectoryCallback onDirFound
)
{
	assert(rootPath != nullptr);
	assert(onFileFound != nullptr);
	assert(onDirFound != nullptr);

	// Open the directory at the root path.
	DIR* const pDir = opendir(rootPath);
	if(!pDir)
	{
		// Failed to open the root path as a directory.
		return;
	}

	// Calculate the maximum lengh of the temporary string that will be used for constructing the full paths for each entry.
	const size_t rootPathLen = strlen(rootPath);
	const size_t maxPathLen = rootPathLen + (sizeof(dirent::d_name) / sizeof(char)) + 1;

	char* const tempPath = reinterpret_cast<char*>(HqMemAlloc(sizeof(char) * (maxPathLen + 1)));

	// Construct the start of the temporary path string.
	strncpy(tempPath, rootPath, rootPathLen);
	strncat(tempPath, "/", 1);

	// Iterate over each entry in the directory.
	for(;;)
	{
		struct dirent* const pEntry = readdir(pDir);

		if(!pEntry)
		{
			// The end of the directory has been reached.
			break;
		}

		if(strcasecmp(pEntry->d_name, ".") == 0 || strcasecmp(pEntry->d_name, "..") == 0)
		{
			// Discard the "." and ".." entries.
			continue;
		}

		// Finish constructing the temporary path for the current entry.
		strcpy(tempPath + rootPathLen + 1, pEntry->d_name);

		struct stat statBuf;

		const int statResult = stat(tempPath, &statBuf);
		if(statResult != 0)
		{
			// This probably shouldn't be able to happen, but if we can't examine
			// the entry's file system properties, we move on to the next entry.
			continue;
		}

		if(S_ISDIR(statBuf.st_mode))
		{
			onDirFound(pUserData, rootPath, pEntry->d_name);
		}
		else if(S_ISREG(statBuf.st_mode))
		{
			onFileFound(pUserData, rootPath, pEntry->d_name);
		}
	}

	// Free the temporary path string and close the directory.
	HqMemFree((void*) tempPath);
	closedir(pDir);
}

//----------------------------------------------------------------------------------------------------------------------
