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

#include "../shared/ByteHelper.hpp"
#include "../shared/String.hpp"
#include "../shared/Value.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonProgram
{
	typedef SkipProbe::HashMap<
		XenonString*,
		XenonProgramHandle,
		XenonString::StlHash,
		XenonString::StlCompare
	> StringToHandleMap;

	struct LoadData
	{
		XenonValue::StringToHandleMap globals;
		XenonFunction::StringToHandleMap functions;
	};

	static XenonProgramHandle Create(XenonVmHandle hVm, const char* const programName, const char* const filePath);
	static XenonProgramHandle Create(
		XenonVmHandle hVm,
		const char* const programName,
		const void* const pFileData,
		const size_t fileLength
	);
	static void Dispose(XenonProgramHandle hProgram);

	static XenonValueHandle GetConstant(XenonProgramHandle hProgram, const uint32_t index, int* const pOutResult);

	static void prv_copyDataToVm(const LoadData& loadData, XenonVmHandle);
	static void prv_freeLoadData(LoadData& loadData);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonValue::StringToHandleMap dependencies;
	XenonFunction::StringToBoolMap functions;
	XenonValue::StringToBoolMap globals;
	XenonValue::HandleArray constants;
	XenonByteHelper::Array code;

	XenonVmHandle hVm;

	XenonString* pName;

	int endianness;
};

//----------------------------------------------------------------------------------------------------------------------
