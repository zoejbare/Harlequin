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
#include "Module.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

#define _HQ_SIGNED_INT_CLAMP(x, min, max) ( \
	((x) > (max)) \
		? (max) \
		: ((x) < (min)) \
			? (min) \
			: (x) \
)

#define _HQ_UNSIGNED_INT_CLAMP(x, max) ( \
	((x) > (max)) \
		? (max) \
		: (x) \
)

//----------------------------------------------------------------------------------------------------------------------

void HqDecoder::Initialize(HqDecoder& output, HqModuleHandle hModule, uint32_t offset)
{
	assert(hModule != HQ_MODULE_HANDLE_NULL);

	output.ip = hModule->code.pData + offset;
	output.cachedIp = output.ip;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqDecoder::LoadBool(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int32_t output = *reinterpret_cast<int32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	// Return the data.
	return output != 0;
}

//----------------------------------------------------------------------------------------------------------------------

int8_t HqDecoder::LoadInt8(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int32_t output = *reinterpret_cast<int32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	// Return the data.
	return int8_t(_HQ_SIGNED_INT_CLAMP(output, SCHAR_MIN, SCHAR_MAX));
}

//----------------------------------------------------------------------------------------------------------------------

int16_t HqDecoder::LoadInt16(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int32_t output = *reinterpret_cast<int32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	// Return the data.
	return int16_t(_HQ_SIGNED_INT_CLAMP(output, SHRT_MIN, SHRT_MAX));
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqDecoder::LoadInt32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int32_t output = *reinterpret_cast<int32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	// Return the data.
	return output;
}

//----------------------------------------------------------------------------------------------------------------------

int64_t HqDecoder::LoadInt64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const int64_t output = *reinterpret_cast<int64_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(int64_t);

	// Return the data.
	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint8_t HqDecoder::LoadUint8(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint32_t output = *reinterpret_cast<uint32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	// Return the data.
	return uint8_t(_HQ_UNSIGNED_INT_CLAMP(output, UCHAR_MAX));
}

//----------------------------------------------------------------------------------------------------------------------

uint16_t HqDecoder::LoadUint16(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint32_t output = *reinterpret_cast<uint32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	// Return the data.
	return uint16_t(_HQ_UNSIGNED_INT_CLAMP(output, USHRT_MAX));
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t HqDecoder::LoadUint32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint32_t output = *reinterpret_cast<uint32_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	// Return the data.
	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t HqDecoder::LoadUint64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const uint64_t output = *reinterpret_cast<uint64_t*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(uint64_t);

	// Return the data.
	return output;
}

//----------------------------------------------------------------------------------------------------------------------

float HqDecoder::LoadFloat32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const float output = *reinterpret_cast<float*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(float);

	// Return the data.
	return output;
}

//----------------------------------------------------------------------------------------------------------------------

double HqDecoder::LoadFloat64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	// Get the byte of the current position of the instruction pointer.
	const double output = *reinterpret_cast<double*>(decoder.ip);

	// Move the instruction pointer.
	decoder.ip += sizeof(double);

	// Return the data.
	return output;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqDecoder::EndianSwapBool(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	int32_t* const pData = reinterpret_cast<int32_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const bool output = (HqEndianSwapInt32(*pData) > 0);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

int8_t HqDecoder::EndianSwapInt8(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	int32_t* const pData = reinterpret_cast<int32_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const int32_t output = HqEndianSwapInt32(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	return int8_t(output);
}

//----------------------------------------------------------------------------------------------------------------------

int16_t HqDecoder::EndianSwapInt16(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	int32_t* const pData = reinterpret_cast<int32_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const int32_t output = HqEndianSwapInt32(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	return int16_t(output);
}

//----------------------------------------------------------------------------------------------------------------------

int32_t HqDecoder::EndianSwapInt32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	int32_t* const pData = reinterpret_cast<int32_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const int32_t output = HqEndianSwapInt32(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(int32_t);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

int64_t HqDecoder::EndianSwapInt64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	int64_t* const pData = reinterpret_cast<int64_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const int64_t output = HqEndianSwapInt64(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(int64_t);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint8_t HqDecoder::EndianSwapUint8(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	uint32_t* const pData = reinterpret_cast<uint32_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const uint32_t output = HqEndianSwapUint32(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	return uint8_t(output);
}

//----------------------------------------------------------------------------------------------------------------------

uint16_t HqDecoder::EndianSwapUint16(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	uint32_t* const pData = reinterpret_cast<uint32_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const uint32_t output = HqEndianSwapUint32(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	return uint16_t(output);
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t HqDecoder::EndianSwapUint32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	uint32_t* const pData = reinterpret_cast<uint32_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const uint32_t output = HqEndianSwapUint32(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(uint32_t);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

uint64_t HqDecoder::EndianSwapUint64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	uint64_t* const pData = reinterpret_cast<uint64_t*>(decoder.ip);

	// Endian swap the current bytecode data.
	const uint64_t output = HqEndianSwapUint64(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(uint64_t);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

float HqDecoder::EndianSwapFloat32(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	float* const pData = reinterpret_cast<float*>(decoder.ip);

	// Endian swap the current bytecode data.
	const float output = HqEndianSwapFloat32(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(float);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

double HqDecoder::EndianSwapFloat64(HqDecoder& decoder)
{
	assert(decoder.ip != nullptr);

	double* const pData = reinterpret_cast<double*>(decoder.ip);

	// Endian swap the current bytecode data.
	const double output = HqEndianSwapFloat64(*pData);

	// Store the endian swapped data back to the bytecode.
	(*pData) = output;

	// Move the instruction pointer.
	decoder.ip += sizeof(double);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

#undef _HQ_UNSIGNED_INT_CLAMP
#undef _HQ_SIGNED_INT_CLAMP

//----------------------------------------------------------------------------------------------------------------------
