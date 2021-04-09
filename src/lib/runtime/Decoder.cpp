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

#include "Decoder.hpp"
#include "Program.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

void XenonDecoder::Initialize(XenonDecoder& output, XenonProgramHandle hProgram, uint32_t offset)
{
	assert(hProgram != XENON_PROGRAM_HANDLE_NULL);

	output.ip = hProgram->code.pData + offset;
	output.cachedIp = output.ip;
	output.sameEndian = (hProgram->endianness == XenonGetPlatformEndianMode());
}

//----------------------------------------------------------------------------------------------------------------------

uint8_t XenonDecoder::LoadUint8(XenonDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint8_t output = *decoder.ip;

	// Move the instruction pointer.
	decoder.ip += sizeof(uint8_t);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t XenonDecoder::LoadUint32(XenonDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint32_t output = *reinterpret_cast<uint32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	// Return the data, endian swapping it if needed.
	return decoder.sameEndian
		? output
		: XenonEndianSwapUint32(output);
}

//----------------------------------------------------------------------------------------------------------------------
