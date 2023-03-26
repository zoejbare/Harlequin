//
// Copyright (c) 2023, Zoe J. Bare
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

#include "../../common/Util.h"

#include <base/String.hpp>

#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestHqString), CreateThenRelease)
{
	const char* const stringData = "test";
	const size_t stringLength = strlen(stringData);

	// Create a new string object.
	HqString* const pString = HqString::Create(stringData);
	ASSERT_NE(pString, nullptr);
	EXPECT_EQ(pString->length, stringLength);
	EXPECT_STREQ(pString->data, stringData);

#ifdef HQ_CPU_WORD_64_BIT
	EXPECT_EQ(pString->hash, 14140177864371587597ull);

#else
	EXPECT_EQ(pString->hash, 2121145288u);

#endif

	// Release the string object.
	const size_t refCount = HqString::Release(pString);
	ASSERT_EQ(refCount, 0);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestHqString), CreateFmtThenRelease)
{
	const char* const stringData = "test";
	const size_t stringLength = strlen(stringData);

	// Create a new string object.
	HqString* const pString = HqString::CreateFmt("%s", stringData);
	ASSERT_NE(pString, nullptr);
	EXPECT_EQ(pString->length, stringLength);
	EXPECT_STREQ(pString->data, stringData);

#ifdef HQ_CPU_WORD_64_BIT
	EXPECT_EQ(pString->hash, 14140177864371587597ull);

#else
	EXPECT_EQ(pString->hash, 2121145288u);

#endif

	// Release the string object.
	const size_t refCount = HqString::Release(pString);
	ASSERT_EQ(refCount, 0);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestHqString), AddRef)
{
	// Create a new string object.
	HqString* const pString = HqString::Create("test");
	ASSERT_NE(pString, nullptr);

	// Add a reference to the string, checking that the returned value is equal to the new reference count.
	size_t refCount = HqString::AddRef(pString);
	ASSERT_EQ(refCount, 2);

	// Release the extra reference.
	refCount = HqString::Release(pString);
	ASSERT_EQ(refCount, 1);

	// Release the string object.
	refCount = HqString::Release(pString);
	ASSERT_EQ(refCount, 0);
}

//----------------------------------------------------------------------------------------------------------------------
