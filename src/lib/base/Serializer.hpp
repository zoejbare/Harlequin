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

#include "../XenonScript.h"

#include "../common/ByteHelper.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XENON_BASE_API XenonSerializer
{
	static XenonSerializerHandle Create(int mode);
	static void Dispose(XenonSerializerHandle hSerializer);
	static int LoadFile(XenonSerializerHandle hSerializer, const char* const filePath);
	static int LoadBuffer(XenonSerializerHandle hSerializer, const void* const pBuffer, const size_t length);
	static int SaveFile(XenonSerializerHandle hSerializer, const char* const filePath, const bool append);
	static int SaveBuffer(XenonSerializerHandle hSerializer, void* const pOutBuffer, size_t* const pLength);
	static int WriteData(XenonSerializerHandle hSerializer, const uint8_t* const pSource, const size_t length);
	static int WriteRawData(XenonSerializerHandle hSerializer, const void* const pSource, const size_t length);
	static int ReadData(XenonSerializerHandle hSerializer, uint8_t* const pDest, const size_t length);
	static int ReadRawData(XenonSerializerHandle hSerializer, void* const pDest, const size_t length);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonByteHelper::Array stream;

	size_t position;

	int mode;
	int endianness;
};

//----------------------------------------------------------------------------------------------------------------------
