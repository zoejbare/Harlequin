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

#include "../../common/Util.hpp"

#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestEndianSwap), Int16)
{
	const int16_t value = HqEndianSwapInt16(0x1020);
	EXPECT_EQ(value, 0x2010);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestEndianSwap), Int32)
{
	const int32_t value = HqEndianSwapInt32(0x10203040);
	EXPECT_EQ(value, 0x40302010);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestEndianSwap), Int64)
{
	const int64_t value = HqEndianSwapInt64(0x0010203040506070ll);
	EXPECT_EQ(value, 0x7060504030201000ll);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestEndianSwap), Uint16)
{
	const uint16_t value = HqEndianSwapUint16(0x1020);
	EXPECT_EQ(value, 0x2010);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestEndianSwap), Uint32)
{
	const uint32_t value = HqEndianSwapUint32(0x10203040);
	EXPECT_EQ(value, 0x40302010ul);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestEndianSwap), Uint64)
{
	const uint64_t value = HqEndianSwapUint64(0x0010203040506070ull);
	EXPECT_EQ(value, 0x7060504030201000ull);
}

//----------------------------------------------------------------------------------------------------------------------
