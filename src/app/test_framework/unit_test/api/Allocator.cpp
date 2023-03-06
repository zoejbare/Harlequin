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

TEST(TestAllocator, SetCustomAllocator)
{
	auto customAlloc = [](const size_t size) -> void*
	{
		return malloc(size);
	};

	auto customRealloc = [](void* const pMem, const size_t size) -> void*
	{
		return realloc(pMem, size);
	};

	auto customFree = [](void* const pMem)
	{
		return free(pMem);
	};

	const HqMemAllocator defaultAllocator = HqMemGetDefaultAllocator();
	HqMemAllocator currentAllocator = HqMemGetAllocator();

	// Verify the initial allocator matches the default allocator.
	EXPECT_EQ(currentAllocator.allocFn, defaultAllocator.allocFn);
	EXPECT_EQ(currentAllocator.reallocFn, defaultAllocator.reallocFn);
	EXPECT_EQ(currentAllocator.freeFn, defaultAllocator.freeFn);

	HqMemAllocator customAllocator;
	customAllocator.allocFn = customAlloc;
	customAllocator.reallocFn = customRealloc;
	customAllocator.freeFn = customFree;

	// Set a custom allocator.
	EXPECT_EQ(HqMemSetAllocator(customAllocator), HQ_SUCCESS);
	currentAllocator = HqMemGetAllocator();

	// Verify the current allocator matches the custom allocator.
	EXPECT_EQ(currentAllocator.allocFn, customAllocator.allocFn);
	EXPECT_EQ(currentAllocator.reallocFn, customAllocator.reallocFn);
	EXPECT_EQ(currentAllocator.freeFn, customAllocator.freeFn);

	// Reset back to the default allocator.
	EXPECT_EQ(HqMemSetAllocator(defaultAllocator), HQ_SUCCESS);
	currentAllocator = HqMemGetAllocator();

	// Verify the current allocator matches the default allocator.
	EXPECT_EQ(currentAllocator.allocFn, defaultAllocator.allocFn);
	EXPECT_EQ(currentAllocator.reallocFn, defaultAllocator.reallocFn);
	EXPECT_EQ(currentAllocator.freeFn, defaultAllocator.freeFn);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestAllocator, AllocThenFree)
{
	HqMemAllocator defaultAllocator = HqMemGetDefaultAllocator();

	EXPECT_EQ(HqMemSetAllocator(defaultAllocator), HQ_SUCCESS);

	void* const pMem = HqMemAlloc(sizeof(size_t));

	EXPECT_NE(pMem, nullptr);
	EXPECT_EQ(HqMemFree(pMem), HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestAllocator, ReallocNullThenFree)
{
	HqMemAllocator defaultAllocator = HqMemGetDefaultAllocator();

	EXPECT_EQ(HqMemSetAllocator(defaultAllocator), HQ_SUCCESS);

	void* const pMem = HqMemRealloc(nullptr, sizeof(size_t));

	EXPECT_NE(pMem, nullptr);
	EXPECT_EQ(HqMemFree(pMem), HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestAllocator, AllocReallocThenFree)
{
	HqMemAllocator defaultAllocator = HqMemGetDefaultAllocator();

	EXPECT_EQ(HqMemSetAllocator(defaultAllocator), HQ_SUCCESS);

	// Make the initial allocation.
	void* const pOriginalMem = HqMemAlloc(sizeof(size_t) * 4);
	EXPECT_NE(pOriginalMem, nullptr);

	// Re-allocate to the same size as the original.
	void* const pReallocEqualMem = HqMemRealloc(pOriginalMem, sizeof(size_t) * 4);
	EXPECT_NE(pReallocEqualMem, nullptr);

	// Re-allocate to a smaller size than the original.
	void* const pReallocSmallMem = HqMemRealloc(pReallocEqualMem, sizeof(size_t));
	EXPECT_NE(pReallocSmallMem, nullptr);

	// Re-allocate to a much larger size than the original.
	void* const pReallocLargeMem = HqMemRealloc(pReallocSmallMem, sizeof(size_t) * 64);
	EXPECT_NE(pReallocLargeMem, nullptr);

	// Free the allocation.
	EXPECT_EQ(HqMemFree(pReallocLargeMem), HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------
