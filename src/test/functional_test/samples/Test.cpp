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

#include "../FuncTestUtil.hpp"
#include "../Memory.hpp"

#include <develop/compiler/ControlFlow.hpp>
#include <gtest/gtest.h>

//----------------------------------------------------------------------------------------------------------------------

class _HQ_TEST_NAME(TestSamples)
	: public ::testing::Test
{
public:

	virtual void TearDown() override
	{
		// Force the memory handler to reset after each test.
		Memory::Instance.Reset();
	}
};

//----------------------------------------------------------------------------------------------------------------------

TEST_F(_HQ_TEST_NAME(TestSamples), ComputeCovarianceMatrix)
{
	enum
	{
		TYPE_MATRIX,
		TYPE_VECTOR,

		TYPE__COUNT,
	};

	enum
	{
		MATRIX_00, MATRIX_01, MATRIX_02,
		MATRIX_10, MATRIX_11, MATRIX_12,
		MATRIX_20, MATRIX_21, MATRIX_22,

		MATRIX__COUNT,
	};

	enum
	{
		VECTOR_X,
		VECTOR_Y,
		VECTOR_Z,

		VECTOR__COUNT,
	};

	static constexpr const char* const mainFuncSig = "(matrix, vector) covarianceMatrix(vector[])";

	static constexpr const char* const typeName[TYPE__COUNT] =
	{
		"matrix",
		"vector",
	};

	static constexpr const char* const matrixMemberName[MATRIX__COUNT] =
	{
		"m00",
		"m01",
		"m02",
		"m10",
		"m11",
		"m12",
		"m20",
		"m21",
		"m22",
	};

	static constexpr const char* const vectorMemberName[VECTOR__COUNT] =
	{
		"x",
		"y",
		"z",
	};

	auto compilerCallback = [](HqModuleWriterHandle hModuleWriter, int endianness)
	{
		HqSerializerHandle hFuncSerializer = HQ_SERIALIZER_HANDLE_NULL;

		// String table indices
		uint32_t typeStrIdx[TYPE__COUNT];
		uint32_t matVarStrIdx[MATRIX__COUNT];
		uint32_t vecVarStrIdx[VECTOR__COUNT];

		// Object member indices
		uint32_t matVarIdx[MATRIX__COUNT];
		uint32_t vecVarIdx[VECTOR__COUNT];

		// Add our custom object types to the module.
		for(size_t i = 0; i < TYPE__COUNT; ++i)
		{
			ASSERT_EQ(HqModuleWriterAddString(hModuleWriter, typeName[i], &typeStrIdx[i]), HQ_SUCCESS);
			ASSERT_EQ(HqModuleWriterAddObjectType(hModuleWriter, typeName[i]), HQ_SUCCESS);
		}

		// Add the member names to the matrix object type.
		for(size_t i = 0; i < MATRIX__COUNT; ++i)
		{
			ASSERT_EQ(HqModuleWriterAddString(hModuleWriter, matrixMemberName[i], &matVarStrIdx[i]), HQ_SUCCESS);
			ASSERT_EQ(
				HqModuleWriterAddObjectMember(
					hModuleWriter,
					typeName[TYPE_MATRIX],
					matrixMemberName[i],
					HQ_VALUE_TYPE_FLOAT32,
					&matVarIdx[i]
				),
				HQ_SUCCESS
			);
		}

		// Add the member names to the vector object type.
		for(size_t i = 0; i < VECTOR__COUNT; ++i)
		{
			ASSERT_EQ(HqModuleWriterAddString(hModuleWriter, vectorMemberName[i], &vecVarStrIdx[i]), HQ_SUCCESS);
			ASSERT_EQ(
				HqModuleWriterAddObjectMember(
					hModuleWriter,
					typeName[TYPE_VECTOR],
					vectorMemberName[i],
					HQ_VALUE_TYPE_FLOAT32,
					&vecVarIdx[i]
				),
				HQ_SUCCESS
			);
		}

		uint32_t gpRegStart = 0;
		auto reg = [&gpRegStart](const uint32_t index) -> uint32_t
		{
			return gpRegStart + index;
		};

		// Set the function serializer.
		Util::SetupFunctionSerializer(hFuncSerializer, endianness);
		{
			// Initialize our output values.
			const uint32_t outMatrixReg = gpRegStart++;
			ASSERT_EQ(HqBytecodeEmitInitObject(hFuncSerializer, outMatrixReg, typeStrIdx[TYPE_MATRIX]), HQ_SUCCESS);

			const uint32_t outCentroidReg = gpRegStart++;
			ASSERT_EQ(HqBytecodeEmitInitObject(hFuncSerializer, outCentroidReg, typeStrIdx[TYPE_VECTOR]), HQ_SUCCESS);

			// Initialize the members of the output values.
			ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, reg(0), 0.0f), HQ_SUCCESS);
			for(size_t i = 0; i < MATRIX__COUNT; ++i)
			{
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, reg(0), matVarIdx[i]), HQ_SUCCESS);
			}

			// Load the input parameter for easy access.
			const uint32_t inputReg = gpRegStart++;
			ASSERT_EQ(HqBytecodeEmitLoadParam(hFuncSerializer, inputReg, 0), HQ_SUCCESS);

			const uint32_t arrayLenIntReg = reg(0);
			const uint32_t arrayLenFltReg = reg(1);
			const uint32_t arrayInvLenReg = reg(2);

			// CODE: float32 invPointCount = 1.0f / len(points)
			ASSERT_EQ(HqBytecodeEmitLength(hFuncSerializer, arrayLenIntReg, inputReg), HQ_SUCCESS);
			ASSERT_EQ(HqBytecodeEmitCastFloat32(hFuncSerializer, arrayLenFltReg, arrayLenIntReg), HQ_SUCCESS);
			ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, arrayInvLenReg, 1.0f), HQ_SUCCESS);
			ASSERT_EQ(HqBytecodeEmitDiv(hFuncSerializer, arrayInvLenReg, arrayInvLenReg, arrayLenFltReg), HQ_SUCCESS);

			const uint32_t centroidXReg = reg(3);
			const uint32_t centroidYReg = reg(4);
			const uint32_t centroidZReg = reg(5);

			// Calculate the centroid from the input points.
			{
				const uint32_t iterReg = reg(6);
				const uint32_t condReg = reg(7);
				const uint32_t incrReg = reg(8);

				// Initialize the loop variables.
				ASSERT_EQ(HqBytecodeEmitLoadImmU32(hFuncSerializer, iterReg, 0), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmU32(hFuncSerializer, incrReg, 1), HQ_SUCCESS);

				// Initialize the condition variable by doing the initial condition check.
				ASSERT_EQ(HqBytecodeEmitCompareLess(hFuncSerializer, condReg, iterReg, arrayLenIntReg), HQ_SUCCESS);

				// Load the initial values of the centroid member values into registers.
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, centroidXReg, 0.0f), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, centroidYReg, 0.0f), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, centroidZReg, 0.0f), HQ_SUCCESS);

				// CODE: for(uint32_t i = 0; i < vec_array_len; ++i)
				ControlFlow ctrl;
				ctrl.Begin(hFuncSerializer, ControlFlow::Behavior::While, ControlFlow::Condition::False, condReg);
				{
					const uint32_t pointReg = reg(9);
					const uint32_t pointXReg = reg(10);
					const uint32_t pointYReg = reg(11);
					const uint32_t pointZReg = reg(9);

					// centroid += vec_array[i]
					ASSERT_EQ(HqBytecodeEmitLoadArray(hFuncSerializer, pointReg, inputReg, iterReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitLoadObject(hFuncSerializer, pointXReg, pointReg, vecVarIdx[VECTOR_X]), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitLoadObject(hFuncSerializer, pointYReg, pointReg, vecVarIdx[VECTOR_Y]), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitLoadObject(hFuncSerializer, pointZReg, pointReg, vecVarIdx[VECTOR_Z]), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, centroidXReg, centroidXReg, pointXReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, centroidYReg, centroidYReg, pointYReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, centroidZReg, centroidZReg, pointZReg), HQ_SUCCESS);

					// Increment the iterator variable, then compare it against the loop break condition.
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, iterReg, iterReg, incrReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitCompareLess(hFuncSerializer, condReg, iterReg, arrayLenIntReg), HQ_SUCCESS);
				}
				ctrl.End();

				// centroid *= inv_vec_array_len
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, centroidXReg, centroidXReg, arrayInvLenReg), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, centroidYReg, centroidYReg, arrayInvLenReg), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, centroidZReg, centroidZReg, arrayInvLenReg), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outCentroidReg, centroidXReg, vecVarIdx[VECTOR_X]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outCentroidReg, centroidYReg, vecVarIdx[VECTOR_Y]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outCentroidReg, centroidZReg, vecVarIdx[VECTOR_Z]), HQ_SUCCESS);
			}

			// Calculate the upper triangle and diagonal of the covariance matrix.
			{
				const uint32_t iterReg = reg(6);
				const uint32_t condReg = reg(7);
				const uint32_t incrReg = reg(8);

				const uint32_t m00Reg = reg(9);
				const uint32_t m01Reg = reg(10);
				const uint32_t m02Reg = reg(11);
				const uint32_t m11Reg = reg(12);
				const uint32_t m12Reg = reg(13);
				const uint32_t m22Reg = reg(14);

				// Initialize the loop variables.
				ASSERT_EQ(HqBytecodeEmitLoadImmU32(hFuncSerializer, iterReg, 0), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmU32(hFuncSerializer, incrReg, 1), HQ_SUCCESS);

				// Initialize the condition variable by doing the initial condition check.
				ASSERT_EQ(HqBytecodeEmitCompareLess(hFuncSerializer, condReg, iterReg, arrayLenIntReg), HQ_SUCCESS);

				// Initialize the matrix element variables.
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, m00Reg, 1.0f), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, m01Reg, 0.0f), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, m02Reg, 0.0f), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, m11Reg, 1.0f), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, m12Reg, 0.0f), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitLoadImmF32(hFuncSerializer, m22Reg, 1.0f), HQ_SUCCESS);

				// CODE: for(uint32_t i = 0; i < vec_array_len; ++i)
				ControlFlow ctrl;
				ctrl.Begin(hFuncSerializer, ControlFlow::Behavior::While, ControlFlow::Condition::False, condReg);
				{
					const uint32_t relVecXReg = reg(15);
					const uint32_t relVecYReg = reg(16);
					const uint32_t relVecZReg = reg(17);

					const uint32_t pointReg = reg(18);
					const uint32_t pointXReg = reg(19);
					const uint32_t pointYReg = reg(20);
					const uint32_t pointZReg = reg(18);

					const uint32_t tempReg = reg(21);

					// relVec = vec_array[i] - centroid
					ASSERT_EQ(HqBytecodeEmitLoadArray(hFuncSerializer, pointReg, inputReg, iterReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitLoadObject(hFuncSerializer, pointXReg, pointReg, vecVarIdx[VECTOR_X]), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitLoadObject(hFuncSerializer, pointYReg, pointReg, vecVarIdx[VECTOR_Y]), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitLoadObject(hFuncSerializer, pointZReg, pointReg, vecVarIdx[VECTOR_Z]), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitSub(hFuncSerializer, relVecXReg, pointXReg, centroidXReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitSub(hFuncSerializer, relVecYReg, pointYReg, centroidYReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitSub(hFuncSerializer, relVecZReg, pointZReg, centroidZReg), HQ_SUCCESS);

					// CODE:
					// m00 += relVec.x * relVec.x
					// m11 += relVec.y * relVec.y
					// m22 += relVec.z * relVec.z
					ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, tempReg, relVecXReg, relVecXReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, m00Reg, m00Reg, tempReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, tempReg, relVecYReg, relVecYReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, m11Reg, m11Reg, tempReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, tempReg, relVecZReg, relVecZReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, m22Reg, m22Reg, tempReg), HQ_SUCCESS);

					// CODE:
					// m01 += relVec.x * relVec.y;
					// m02 += relVec.x * relVec.z;
					ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, tempReg, relVecXReg, relVecYReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, m01Reg, m01Reg, tempReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, tempReg, relVecXReg, relVecZReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, m02Reg, m02Reg, tempReg), HQ_SUCCESS);

					// CODE:
					// m12 += relVec.y * relVec.z;
					ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, tempReg, relVecYReg, relVecZReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, m12Reg, m12Reg, tempReg), HQ_SUCCESS);

					// Increment the iterator variable, then compare it against the loop break condition.
					ASSERT_EQ(HqBytecodeEmitAdd(hFuncSerializer, iterReg, iterReg, incrReg), HQ_SUCCESS);
					ASSERT_EQ(HqBytecodeEmitCompareLess(hFuncSerializer, condReg, iterReg, arrayLenIntReg), HQ_SUCCESS);
				}
				ctrl.End();

				// matrix *= inv_vec_array_len
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, m00Reg, m00Reg, arrayInvLenReg), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, m01Reg, m01Reg, arrayInvLenReg), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, m02Reg, m02Reg, arrayInvLenReg), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, m11Reg, m11Reg, arrayInvLenReg), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, m12Reg, m12Reg, arrayInvLenReg), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitMul(hFuncSerializer, m22Reg, m22Reg, arrayInvLenReg), HQ_SUCCESS);

				// Store the matrix elements in the covariance matrix.
				// A covariance matrix will be symmetric, so we copy
				// the upper triangle into the lower triangle.
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m00Reg, matVarIdx[MATRIX_00]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m01Reg, matVarIdx[MATRIX_01]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m02Reg, matVarIdx[MATRIX_02]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m01Reg, matVarIdx[MATRIX_10]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m11Reg, matVarIdx[MATRIX_11]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m12Reg, matVarIdx[MATRIX_12]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m02Reg, matVarIdx[MATRIX_20]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m12Reg, matVarIdx[MATRIX_21]), HQ_SUCCESS);
				ASSERT_EQ(HqBytecodeEmitStoreObject(hFuncSerializer, outMatrixReg, m22Reg, matVarIdx[MATRIX_22]), HQ_SUCCESS);
			}

			// Return the output values.
			ASSERT_EQ(HqBytecodeEmitStoreParam(hFuncSerializer, 0, outMatrixReg), HQ_SUCCESS);
			ASSERT_EQ(HqBytecodeEmitStoreParam(hFuncSerializer, 1, outCentroidReg), HQ_SUCCESS);
		}
		// Finalize the serializer and add it to the module.
		Util::FinalizeFunctionSerializer(hFuncSerializer, hModuleWriter, mainFuncSig);
	};

	auto runtimeCallback = [](HqVmHandle hVm, HqExecutionHandle hExec)
	{
		constexpr size_t pointCount = 4;

		// Create an array of points for our input argument.
		HqValueHandle hPointArray = HqValueCreateArray(hVm, pointCount);
		ASSERT_NE(hPointArray, HQ_VALUE_HANDLE_NULL);

		size_t nextPointIndex = 0;

		auto addPoint = [hVm, hPointArray, &nextPointIndex](const float x, const float y, const float z)
		{
			// Create a new point object.
			HqValueHandle hPoint = HqValueCreateObject(hVm, typeName[TYPE_VECTOR]);
			ASSERT_NE(hPoint, HQ_VALUE_HANDLE_NULL);

			HqValueHandle hMemberX = HqValueCreateFloat32(hVm, x);
			ASSERT_NE(hMemberX, HQ_VALUE_HANDLE_NULL);

			HqValueHandle hMemberY = HqValueCreateFloat32(hVm, y);
			ASSERT_NE(hMemberY, HQ_VALUE_HANDLE_NULL);

			HqValueHandle hMemberZ = HqValueCreateFloat32(hVm, z);
			ASSERT_NE(hMemberZ, HQ_VALUE_HANDLE_NULL);

			// Add the member data to the point object, then add the point to the array.
			ASSERT_EQ(HqValueSetObjectMemberValue(hPoint, vectorMemberName[VECTOR_X], hMemberX), HQ_SUCCESS);
			ASSERT_EQ(HqValueSetObjectMemberValue(hPoint, vectorMemberName[VECTOR_Y], hMemberY), HQ_SUCCESS);
			ASSERT_EQ(HqValueSetObjectMemberValue(hPoint, vectorMemberName[VECTOR_Z], hMemberZ), HQ_SUCCESS);
			ASSERT_EQ(HqValueSetArrayElement(hPointArray, nextPointIndex, hPoint), HQ_SUCCESS);

			++nextPointIndex;
		};

		// Add some points to the array.
		addPoint(0.0f, 0.0f, 0.0f);
		addPoint(1.0f, 4.0f, 0.0f);
		addPoint(2.0f, 2.0f, 0.0f);
		addPoint(5.0f, 1.0f, 0.0f);

		// Set the function input argument.
		ASSERT_EQ(HqExecutionSetIoRegister(hExec, hPointArray, 0), HQ_SUCCESS);

		// Run the execution context.
		const int execRunResult = HqExecutionRun(hExec, HQ_RUN_FULL);
		ASSERT_EQ(execRunResult, HQ_SUCCESS);

		// Get the status of the execution context.
		ExecStatus status;
		Util::GetExecutionStatus(status, hExec);
		ASSERT_FALSE(status.yield);
		ASSERT_FALSE(status.running);
		ASSERT_TRUE(status.complete);
		ASSERT_FALSE(status.exception);
		ASSERT_FALSE(status.abort);

		// Get the returned covariance matrix.
		HqValueHandle hMatrix = HQ_VALUE_HANDLE_NULL;
		Util::GetIoRegister(hMatrix, hExec, 0);
		ASSERT_TRUE(HqValueIsObject(hMatrix));
		ASSERT_STREQ(HqValueGetObjectTypeName(hMatrix), typeName[TYPE_MATRIX]);

		// Get the returned centroid vector.
		HqValueHandle hCentroid = HQ_VALUE_HANDLE_NULL;
		Util::GetIoRegister(hCentroid, hExec, 1);
		ASSERT_TRUE(HqValueIsObject(hCentroid));
		ASSERT_STREQ(HqValueGetObjectTypeName(hCentroid), typeName[TYPE_VECTOR]);

		// Get the matrix member values.
		HqValueHandle hMatrix00 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_00]);
		HqValueHandle hMatrix01 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_01]);
		HqValueHandle hMatrix02 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_02]);
		HqValueHandle hMatrix10 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_10]);
		HqValueHandle hMatrix11 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_11]);
		HqValueHandle hMatrix12 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_12]);
		HqValueHandle hMatrix20 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_20]);
		HqValueHandle hMatrix21 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_21]);
		HqValueHandle hMatrix22 = HqValueGetObjectMemberValue(hMatrix, matrixMemberName[MATRIX_22]);

		// Verify the matrix data is correct.
		ASSERT_TRUE(HqValueIsFloat32(hMatrix00));
		ASSERT_TRUE(HqValueIsFloat32(hMatrix01));
		ASSERT_TRUE(HqValueIsFloat32(hMatrix02));
		ASSERT_TRUE(HqValueIsFloat32(hMatrix10));
		ASSERT_TRUE(HqValueIsFloat32(hMatrix11));
		ASSERT_TRUE(HqValueIsFloat32(hMatrix12));
		ASSERT_TRUE(HqValueIsFloat32(hMatrix20));
		ASSERT_TRUE(HqValueIsFloat32(hMatrix21));
		ASSERT_TRUE(HqValueIsFloat32(hMatrix22));
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix00), 3.75f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix01), -0.25f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix02), 0.0f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix10), -0.25f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix11), 2.4375f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix12), 0.0f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix20), 0.0f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix21), 0.0f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hMatrix22), 0.25f);

		// Get the centroid member values.
		HqValueHandle hCentroidX = HqValueGetObjectMemberValue(hCentroid, vectorMemberName[VECTOR_X]);
		HqValueHandle hCentroidY = HqValueGetObjectMemberValue(hCentroid, vectorMemberName[VECTOR_Y]);
		HqValueHandle hCentroidZ = HqValueGetObjectMemberValue(hCentroid, vectorMemberName[VECTOR_Z]);

		// Verify the centroid data is correct.
		ASSERT_TRUE(HqValueIsFloat32(hCentroidX));
		ASSERT_TRUE(HqValueIsFloat32(hCentroidY));
		ASSERT_TRUE(HqValueIsFloat32(hCentroidZ));
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hCentroidX), 2.0f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hCentroidY), 1.75f);
		ASSERT_FLOAT_EQ(HqValueGetFloat32(hCentroidZ), 0.0f);
	};

	std::vector<uint8_t> bytecode;

	// Compiling with just an empty init function is enough to test the RETURN opcode since
	// that gets added to the module by default if no other bytecode is supplied for it.
	Util::CompileBytecode(bytecode, compilerCallback);
	ASSERT_GT(bytecode.size(), 0u);

	Util::ProcessBytecode("TestSamples", mainFuncSig, runtimeCallback, bytecode);
}

//----------------------------------------------------------------------------------------------------------------------
