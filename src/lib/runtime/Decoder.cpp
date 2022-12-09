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

void HqDecoder::Initialize(HqDecoder& output, HqProgramHandle hProgram, uint32_t offset)
{
	assert(hProgram != HQ_PROGRAM_HANDLE_NULL);

	output.ip = hProgram->code.pData + offset;
	output.cachedIp = output.ip;
	output.sameEndian = (hProgram->endianness == HqGetPlatformEndianMode());
}

//----------------------------------------------------------------------------------------------------------------------

bool HqDecoder::LoadBool(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int32_t output = *reinterpret_cast<int32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	// Return the data, endian swapping it if needed.
	return (decoder.sameEndian
		? output
		: HqEndianSwapInt32(output)) != 0;
}

//----------------------------------------------------------------------------------------------------------------------

int8_t HqDecoder::LoadInt8(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int32_t output = *reinterpret_cast<int32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	// Return the data, endian swapping it if needed.
	return int8_t(decoder.sameEndian
		? output
		: HqEndianSwapInt32(output));
}

//----------------------------------------------------------------------------------------------------------------------

int16_t HqDecoder::LoadInt16(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int32_t output = *reinterpret_cast<int32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	// Return the data, endian swapping it if needed.
	return int16_t(decoder.sameEndian
		? output
		: HqEndianSwapInt32(output));
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqDecoder::LoadInt32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int32_t output = *reinterpret_cast<int32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	// Return the data, endian swapping it if needed.
	return decoder.sameEndian
		? output
		: HqEndianSwapInt32(output);
}

//----------------------------------------------------------------------------------------------------------------------

int64_t HqDecoder::LoadInt64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int64_t output = *reinterpret_cast<int64_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int64_t);

	// Return the data, endian swapping it if needed.
	return decoder.sameEndian
		? output
		: HqEndianSwapInt64(output);
}

//----------------------------------------------------------------------------------------------------------------------

uint8_t HqDecoder::LoadUint8(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint32_t output = *reinterpret_cast<uint32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	// Return the data, endian swapping it if needed.
	return uint8_t(decoder.sameEndian
		? output
		: HqEndianSwapUint32(output));
}

//----------------------------------------------------------------------------------------------------------------------

uint16_t HqDecoder::LoadUint16(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint32_t output = *reinterpret_cast<uint32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	// Return the data, endian swapping it if needed.
	return uint16_t(decoder.sameEndian
		? output
		: HqEndianSwapUint32(output));
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t HqDecoder::LoadUint32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint32_t output = *reinterpret_cast<uint32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	// Return the data, endian swapping it if needed.
	return decoder.sameEndian
		? output
		: HqEndianSwapUint32(output);
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t HqDecoder::LoadUint64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint64_t output = *reinterpret_cast<uint64_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint64_t);

	// Return the data, endian swapping it if needed.
	return decoder.sameEndian
		? output
		: HqEndianSwapUint64(output);
}

//----------------------------------------------------------------------------------------------------------------------

float HqDecoder::LoadFloat32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const float output = *reinterpret_cast<float*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(float);

	// Return the data, endian swapping it if needed.
	return decoder.sameEndian
		? output
		: HqEndianSwapFloat32(output);
}

//----------------------------------------------------------------------------------------------------------------------

double HqDecoder::LoadFloat64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const double output = *reinterpret_cast<double*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(double);

	// Return the data, endian swapping it if needed.
	return decoder.sameEndian
		? output
		: HqEndianSwapFloat64(output);
}

//----------------------------------------------------------------------------------------------------------------------
