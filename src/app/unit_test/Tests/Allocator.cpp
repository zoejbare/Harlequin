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

	const XenonMemAllocator defaultAllocator = XenonMemGetDefaultAllocator();

	XenonMemAllocator currentAllocator = XenonMemGetAllocator();

	// Verify the initial allocator matches the default allocator.
	EXPECT_EQ(currentAllocator.allocFn, defaultAllocator.allocFn);
	EXPECT_EQ(currentAllocator.reallocFn, defaultAllocator.reallocFn);
	EXPECT_EQ(currentAllocator.freeFn, defaultAllocator.freeFn);

	XenonMemAllocator customAllocator;
	customAllocator.allocFn = customAlloc;
	customAllocator.reallocFn = customRealloc;
	customAllocator.freeFn = customFree;

	// Set a custom allocator.
	XenonMemSetAllocator(customAllocator);
	currentAllocator = XenonMemGetAllocator();

	// Verify the current allocator matches the custom allocator.
	EXPECT_EQ(currentAllocator.allocFn, customAllocator.allocFn);
	EXPECT_EQ(currentAllocator.reallocFn, customAllocator.reallocFn);
	EXPECT_EQ(currentAllocator.freeFn, customAllocator.freeFn);

	// Reset back to the default allocator.
	XenonMemSetAllocator(defaultAllocator);
	currentAllocator = XenonMemGetAllocator();

	// Verify the current allocator matches the default allocator.
	EXPECT_EQ(currentAllocator.allocFn, defaultAllocator.allocFn);
	EXPECT_EQ(currentAllocator.reallocFn, defaultAllocator.reallocFn);
	EXPECT_EQ(currentAllocator.freeFn, defaultAllocator.freeFn);
}

//----------------------------------------------------------------------------------------------------------------------
