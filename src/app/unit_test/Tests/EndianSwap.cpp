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

#include <gtest/gtest.h>

#include <XenonScript.h>

//----------------------------------------------------------------------------------------------------------------------

TEST(TestEndianSwap, Int16)
{
	const int16_t value = XenonEndianSwapInt16(0x1020);
	EXPECT_EQ(value, 0x2010);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestEndianSwap, Int32)
{
	const int32_t value = XenonEndianSwapInt32(0x10203040);
	EXPECT_EQ(value, 0x40302010);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestEndianSwap, Int64)
{
	const int64_t value = XenonEndianSwapInt64(0x0010203040506070ll);
	EXPECT_EQ(value, 0x7060504030201000ll);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestEndianSwap, Uint16)
{
	const uint16_t value = XenonEndianSwapUint16(0x1020);
	EXPECT_EQ(value, 0x2010);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestEndianSwap, Uint32)
{
	const uint32_t value = XenonEndianSwapUint32(0x10203040);
	EXPECT_EQ(value, 0x40302010ul);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestEndianSwap, Uint64)
{
	const uint64_t value = XenonEndianSwapUint64(0x0010203040506070ull);
	EXPECT_EQ(value, 0x7060504030201000ull);
}

//----------------------------------------------------------------------------------------------------------------------
