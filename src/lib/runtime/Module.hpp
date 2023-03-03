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

#include "Function.hpp"
#include "ScriptObject.hpp"
#include "Value.hpp"

#include "../base/ModuleLoader.hpp"
#include "../base/String.hpp"

#include "../common/ByteHelper.hpp"
#include "../common/HashMap.hpp"
#include "../common/Stack.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqModule
{
	typedef HqHashMap<
		HqString*,
		HqModuleHandle,
		HqString::StlHash,
		HqString::StlCompare
	> StringToHandleMap;

	typedef HqStack<HqModuleHandle> HandleStack;
	typedef HqArray<HqString*> StringArray;

	static HqModuleHandle Create(HqVmHandle hVm, HqString* const pModuleName, const char* const filePath);
	static HqModuleHandle Create(
		HqVmHandle hVm,
		HqString* const pModuleName,
		const void* const pFileData,
		const size_t fileLength
	);
	static void Dispose(HqModuleHandle hModule);

	static HqString* GetString(HqModuleHandle hModule, const uint32_t index, int* const pOutResult);

	static bool prv_init(HqVmHandle, HqReportHandle, HqModuleHandle, HqModuleLoader&, HqString*);
	static bool prv_verify(HqVmHandle, HqReportHandle, HqModuleLoader&, HqString*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqValue::StringToBoolMap dependencies;
	HqValue::StringToBoolMap globals;

	HqScriptObject::StringToPtrMap objectSchemas;
	HqFunction::StringToHandleMap functions;

	StringArray strings;
	HqByteHelper::Array code;

	HqVmHandle hVm;
	HqFunctionHandle hInitFunction;
	HqDllHandle hDll;

	HqDllEntryPoint pDllEntryPoint;

	HqString* pName;
};

//----------------------------------------------------------------------------------------------------------------------
