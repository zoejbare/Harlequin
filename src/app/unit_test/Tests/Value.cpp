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

#include <Harlequin.h>

//----------------------------------------------------------------------------------------------------------------------
#if 0
TEST(TestValue, CreateStringValue)
{
	// Create native string value object.
	HqValueHandle hValue = HqValueCreateString("This is a native string");

	ASSERT_TRUE(HqValueIsString(hValue));
	EXPECT_EQ(HqValueGetStringLength(hValue), 23);
	EXPECT_STREQ(HqValueGetString(hValue), "This is a native string");

	const size_t nativeStringHash = HqValueGetStringHash(hValue);

#ifdef HQ_DATA_WIDTH_64_BIT
	EXPECT_EQ(nativeStringHash, 8219377559199927259);

#else
	EXPECT_EQ(nativeStringHash, 2965409582);

#endif

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestValue, CreatePrimitiveValueCopy)
{
	// Create a primitive (int64) value object.
	HqValueHandle hOriginalIntValue = HqValueCreateInt64(12345);
	EXPECT_TRUE(HqValueIsInt64(hOriginalIntValue));
	EXPECT_EQ(HqValueGetInt64(hOriginalIntValue), 12345);

	// Create a copy of the original value.
	HqValueHandle hCopiedIntValue = HqValueCopy(hOriginalIntValue);
	EXPECT_TRUE(HqValueIsInt64(hCopiedIntValue));
	EXPECT_EQ(HqValueGetInt64(hCopiedIntValue), 12345);

	// Dispose of the original value object.
	const int disposeOriginalIntValueResult = HqValueGcExpose(hOriginalIntValue);
	EXPECT_EQ(disposeOriginalIntValueResult, HQ_SUCCESS);

	// Dispose of the copied value object.
	const int disposeCopiedIntValueResult = HqValueGcExpose(hCopiedIntValue);
	EXPECT_EQ(disposeCopiedIntValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestValue, CreateNativeStringValueCopy)
{
	// Create a native string value object.
	HqValueHandle hOriginalStringValue = HqValueCreateString("Test string");
	EXPECT_TRUE(HqValueIsString(hOriginalStringValue));
	EXPECT_STREQ(HqValueGetString(hOriginalStringValue), "Test string");

	// Create a copy of the original value.
	HqValueHandle hCopiedStringValue = HqValueCopy(hOriginalStringValue);
	EXPECT_TRUE(HqValueIsString(hCopiedStringValue));
	EXPECT_STREQ(HqValueGetString(hCopiedStringValue), "Test string");

	// Dispose of the original value object.
	const int disposeOriginalStringValue = HqValueGcExpose(hOriginalStringValue);
	EXPECT_EQ(disposeOriginalStringValue, HQ_SUCCESS);

	// Dispose of the copied value object.
	const int disposeCopiedStringValue = HqValueGcExpose(hCopiedStringValue);
	EXPECT_EQ(disposeCopiedStringValue, HQ_SUCCESS);
}
#endif
//----------------------------------------------------------------------------------------------------------------------
