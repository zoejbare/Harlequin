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
#include "Value.hpp"

#include "../base/String.hpp"

#include "../common/ByteHelper.hpp"
#include "../common/Map.hpp"
#include "../common/Stack.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqProgram
{
	typedef HQ_MAP_TYPE<
		HqString*,
		HqProgramHandle,
#if HQ_MAP_IS_UNORDERED
		HqString::StlHash,
		HqString::StlCompare,
#else
		HqString::StlLess,
#endif
		HqStlAllocator<HQ_MAP_NODE_TYPE(HqString*, HqProgramHandle)>
	> StringToHandleMap;

	typedef HqStack<HqProgramHandle> HandleStack;

	static HqProgramHandle Create(HqVmHandle hVm, HqString* const pProgramName, const char* const filePath);
	static HqProgramHandle Create(
		HqVmHandle hVm,
		HqString* const pProgramName,
		const void* const pFileData,
		const size_t fileLength
	);
	static void Dispose(HqProgramHandle hProgram);

	static HqValueHandle GetConstant(HqProgramHandle hProgram, const uint32_t index, int* const pOutResult);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqValue::StringToHandleMap dependencies;
	HqFunction::StringToBoolMap functions;
	HqValue::StringToBoolMap objectSchemas;
	HqValue::StringToBoolMap globals;
	HqValue::HandleArray constants;
	HqByteHelper::Array code;

	HqVmHandle hVm;
	HqFunctionHandle hInitFunction;

	HqString* pName;

	int endianness;
};

//----------------------------------------------------------------------------------------------------------------------
