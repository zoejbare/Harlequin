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

class _HQ_TEST_NAME(TestValue)
	: public ::testing::Test
{
public:

	static void SetUpTestSuite()
	{
		HqVmInit init = GetDefaultHqVmInit(nullptr, nullptr, HQ_MESSAGE_TYPE_FATAL);

		// Create the VM context.
		const int createContextResult = HqVmCreate(&m_hVm, init);
		ASSERT_EQ(createContextResult, HQ_SUCCESS);
	}

	static void TearDownTestSuite()
	{
		// Dispose of the VM context.
		const int disposeContextResult = HqVmDispose(&m_hVm);
		EXPECT_EQ(disposeContextResult, HQ_SUCCESS);
	}

protected:

	static HqVmHandle m_hVm;
};

HqVmHandle _HQ_TEST_NAME(TestValue)::m_hVm = HQ_VM_HANDLE_NULL;

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateBoolValue)
{
	// Create a boolean value object.
	HqValueHandle hValue = HqValueCreateBool(m_hVm, true);

	ASSERT_TRUE(HqValueIsBool(hValue));
	EXPECT_TRUE(HqValueGetBool(hValue));

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateInt8Value)
{
	constexpr int8_t valueData = -123;

	// Create an int8 value object.
	HqValueHandle hValue = HqValueCreateInt8(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsInt8(hValue));
	EXPECT_EQ(HqValueGetInt8(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateInt16Value)
{
	constexpr int16_t valueData = -4567;

	// Create an int16 value object.
	HqValueHandle hValue = HqValueCreateInt16(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsInt16(hValue));
	EXPECT_EQ(HqValueGetInt16(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateInt32Value)
{
	constexpr int32_t valueData = -432895l;

	// Create an int32 value object.
	HqValueHandle hValue = HqValueCreateInt32(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsInt32(hValue));
	EXPECT_EQ(HqValueGetInt32(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateInt64Value)
{
	constexpr int64_t valueData = -348641864189ll;

	// Create an int64 value object.
	HqValueHandle hValue = HqValueCreateInt64(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsInt64(hValue));
	EXPECT_EQ(HqValueGetInt64(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateUint8Value)
{
	constexpr uint8_t valueData = 123;

	// Create a uint8 value object.
	HqValueHandle hValue = HqValueCreateUint8(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsUint8(hValue));
	EXPECT_EQ(HqValueGetUint8(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateUint16Value)
{
	constexpr uint16_t valueData = 4567;

	// Create a uint16 value object.
	HqValueHandle hValue = HqValueCreateUint16(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsUint16(hValue));
	EXPECT_EQ(HqValueGetUint16(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateUint32Value)
{
	constexpr uint32_t valueData = 432895ul;

	// Create a uint32 value object.
	HqValueHandle hValue = HqValueCreateUint32(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsUint32(hValue));
	EXPECT_EQ(HqValueGetUint32(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateUint64Value)
{
	constexpr uint64_t valueData = 348641864189ull;

	// Create a uint64 value object.
	HqValueHandle hValue = HqValueCreateUint64(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsUint64(hValue));
	EXPECT_EQ(HqValueGetUint64(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateFloat32Value)
{
	constexpr float valueData = 1234.56789f;

	// Create a float32 value object.
	HqValueHandle hValue = HqValueCreateFloat32(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsFloat32(hValue));
	EXPECT_EQ(HqValueGetFloat32(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateFloat64Value)
{
	constexpr double valueData = 123456.789;

	// Create a uint64 value object.
	HqValueHandle hValue = HqValueCreateFloat64(m_hVm, valueData);

	ASSERT_TRUE(HqValueIsFloat64(hValue));
	EXPECT_EQ(HqValueGetFloat64(hValue), valueData);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateStringValue)
{
	// Create native string value object.
	HqValueHandle hValue = HqValueCreateString(m_hVm, "This is a native string");

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

TEST_F(_HQ_TEST_NAME(TestValue), CreateFunctionValue)
{
	// Since we're not using the function we're creating, we can trick the runtime into thinking we have a valid one.
	const HqFunctionHandle hFunction = reinterpret_cast<HqFunctionHandle>(0x1234);

	// Create a function value object.
	HqValueHandle hValue = HqValueCreateFunction(m_hVm, hFunction);

	ASSERT_TRUE(HqValueIsFunction(hValue));
	EXPECT_EQ(HqValueGetFunction(hValue), hFunction);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateNullObjectValue)
{
	// Attempt to create an object value object from a non-existent typename.
	HqValueHandle hValue = HqValueCreateObject(m_hVm, "Foo");

	EXPECT_EQ(hValue, HQ_VALUE_HANDLE_NULL);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateArrayValue)
{
	constexpr size_t arraySize = 5;
	constexpr size_t index = 3;

	// Create a function value object.
	HqValueHandle hValue = HqValueCreateArray(m_hVm, arraySize);

	ASSERT_TRUE(HqValueIsArray(hValue));
	EXPECT_EQ(HqValueGetArrayLength(hValue), arraySize);

	// Create a sub-value object that will be inserted into the array.
	HqValueHandle hSubValue = HqValueCreateBool(m_hVm, true);

	ASSERT_TRUE(HqValueIsBool(hSubValue));
	EXPECT_TRUE(HqValueGetBool(hSubValue));

	// Set the sub-value on the array value object.
	const int setElementResult = HqValueSetArrayElement(hValue, index, hSubValue);

	ASSERT_EQ(setElementResult, HQ_SUCCESS);
	EXPECT_EQ(HqValueGetArrayElement(hValue, index), hSubValue);

	// Dispose of the value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreatePrimitiveValueCopy)
{
	// Create a primitive (int64) value object.
	HqValueHandle hOriginalIntValue = HqValueCreateInt64(m_hVm, 12345);
	EXPECT_TRUE(HqValueIsInt64(hOriginalIntValue));
	EXPECT_EQ(HqValueGetInt64(hOriginalIntValue), 12345);

	// Create a copy of the original value.
	HqValueHandle hCopiedIntValue = HqValueCopy(m_hVm, hOriginalIntValue);
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

TEST_F(_HQ_TEST_NAME(TestValue), CreateNativeValue)
{
	static void* const pValueData = reinterpret_cast<void*>(0x1234);

	auto onCopy = [](void** const ppOutput, void* const pInput)
	{
		EXPECT_EQ(pInput, pValueData);

		(*ppOutput) = pInput;
	};

	auto onDestruct = [](void* const pData)
	{
		EXPECT_EQ(pData, pValueData);
	};

	// Create a native value object.
	HqValueHandle hValue = HqValueCreateNative(m_hVm, pValueData, onCopy, onDestruct);

	ASSERT_TRUE(HqValueIsNative(hValue));
	EXPECT_EQ(HqValueGetNative(hValue), pValueData);

	// Create a copy of the original native object.
	HqValueHandle hCopy = HqValueCopy(m_hVm, hValue);

	ASSERT_TRUE(HqValueIsNative(hCopy));
	EXPECT_EQ(HqValueGetNative(hCopy), pValueData);

	// Dispose of the copied value object.
	const int disposeValueResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeValueResult, HQ_SUCCESS);

	// Dispose of the original value object.
	const int disposeCopyResult = HqValueGcExpose(hValue);
	EXPECT_EQ(disposeCopyResult, HQ_SUCCESS);

	// Run the garbage collector to make sure the native value destruct behavior works.
	const int runGcResult = HqVmRunGarbageCollector(m_hVm, HQ_RUN_FULL);
	EXPECT_EQ(runGcResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestValue), CreateStringValueCopy)
{
	// Create a native string value object.
	HqValueHandle hOriginalStringValue = HqValueCreateString(m_hVm, "Test string");
	EXPECT_TRUE(HqValueIsString(hOriginalStringValue));
	EXPECT_STREQ(HqValueGetString(hOriginalStringValue), "Test string");

	// Create a copy of the original value.
	HqValueHandle hCopiedStringValue = HqValueCopy(m_hVm, hOriginalStringValue);
	EXPECT_TRUE(HqValueIsString(hCopiedStringValue));
	EXPECT_STREQ(HqValueGetString(hCopiedStringValue), "Test string");

	// Dispose of the original value object.
	const int disposeOriginalStringValue = HqValueGcExpose(hOriginalStringValue);
	EXPECT_EQ(disposeOriginalStringValue, HQ_SUCCESS);

	// Dispose of the copied value object.
	const int disposeCopiedStringValue = HqValueGcExpose(hCopiedStringValue);
	EXPECT_EQ(disposeCopiedStringValue, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------
