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
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

#include <sys/stat.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _HqSysIsFile(const char* const path)
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

extern "C" bool _HqSysIsDir(const char* const path)
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

extern "C" bool _HqSysIsExe(const char* const path)
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

	// Open the directory at the root path.
	DIR* const pDir = opendir(rootPath);
	if(!pDir)
	{
		// Failed to open the root path as a directory.
		return;
	}

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

		if(pEntry->d_type == DT_DIR)
		{
			onDirFound(pUserData, rootPath, pEntry->d_name);
		}
		else if(pEntry->d_type == DT_REG)
		{
			onFileFound(pUserData, rootPath, pEntry->d_name);
		}
	}

	// Close the directory.
	closedir(pDir);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" const char* _HqSysGetPlatform()
{
#if defined(HQ_PLATFORM_MAC_OS)
	return "macOS";
#elif defined(HQ_PLATFORM_LINUX)
	return "Linux";
#else
	return "Unknown";
#endif
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" const char* _HqSysGetPathSep()
{
	return "/";
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" HqDllHandle _HqSysOpenLib(const char* const dllPath)
{
	assert(dllPath != nullptr);
	assert(dllPath[0] != '\0');

	return reinterpret_cast<HqDllHandle>(dlopen(dllPath, RTLD_LAZY | RTLD_LOCAL));
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqSysCloseLib(HqDllHandle hDll)
{
	assert(hDll != HQ_DLL_HANDLE_NULL);

	dlclose(reinterpret_cast<void*>(hDll));
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void* _HqSysGetSym(HqDllHandle hDll, const char* const symbolName)
{
	assert(hDll != HQ_DLL_HANDLE_NULL);
	assert(symbolName != nullptr);
	assert(symbolName[0] != '\0');

	return dlsym(reinterpret_cast<void*>(hDll), symbolName);
}

//----------------------------------------------------------------------------------------------------------------------
