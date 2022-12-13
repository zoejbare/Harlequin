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

#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------

HqVmInit ConstructInitObject(void* const pUserData, const int reportLevel, HqMessageCallback onMessageFn)
{
	HqVmInit output;
	output.common.report.onMessageFn = onMessageFn;
	output.common.report.pUserData = pUserData;
	output.common.report.reportLevel = reportLevel;
	output.gcThreadStackSize = HQ_VM_THREAD_DEFAULT_STACK_SIZE;
	output.gcMaxTimeSliceMs = HQ_VM_GC_DEFAULT_TIME_SLICE_MS;

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

struct ReportLine
{
	char message[32];
	int type;
};

void StoreReportLine(void* const pUserData, const int messageType, const char* const message)
{
	ReportLine* const pLine = reinterpret_cast<ReportLine*>(pUserData);

	strncpy(pLine->message, message, sizeof(pLine->message) - 1);

	pLine->type = messageType;
};

//----------------------------------------------------------------------------------------------------------------------

void DummyMessageCallback(void*, int, const char*)
{
	// Ignore all messages.
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestVm, CreateAndDisposeContext)
{
	HqVmInit init = ConstructInitObject(nullptr, HQ_MESSAGE_TYPE_FATAL, DummyMessageCallback);
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;

	// Create the VM context.
	const int createContextResult = HqVmCreate(&hVm, init);
	ASSERT_EQ(createContextResult, HQ_SUCCESS);

	// Dispose of the VM context.
	const int disposeContextResult = HqVmDispose(&hVm);
	EXPECT_EQ(disposeContextResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestVm, ReportMessages)
{
	ReportLine line;

	HqVmInit init = ConstructInitObject(&line, HQ_MESSAGE_TYPE_VERBOSE, StoreReportLine);
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;

	// Create the VM context.
	const int createContextResult = HqVmCreate(&hVm, init);
	ASSERT_EQ(createContextResult, HQ_SUCCESS);

	HqReportHandle hReport = HQ_REPORT_HANDLE_NULL;
	const int getReportHandleResult = HqVmGetReportHandle(hVm, &hReport);
	ASSERT_EQ(getReportHandleResult, HQ_SUCCESS);
	EXPECT_NE(hReport, HQ_REPORT_HANDLE_NULL);

	// Test writing a 'verbose' string.
	const int verboseMessageResult = HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Verbose message");
	EXPECT_EQ(verboseMessageResult, HQ_SUCCESS);
	EXPECT_EQ(line.type, HQ_MESSAGE_TYPE_VERBOSE);
	EXPECT_STREQ(line.message, "Verbose message");

	// Test writing an 'info' string.
	const int infoMessageResult = HqReportMessage(hReport, HQ_MESSAGE_TYPE_INFO, "Info message");
	EXPECT_EQ(infoMessageResult, HQ_SUCCESS);
	EXPECT_EQ(line.type, HQ_MESSAGE_TYPE_INFO);
	EXPECT_STREQ(line.message, "Info message");

	// Test writing a 'warning' string.
	const int warningMessageResult = HqReportMessage(hReport, HQ_MESSAGE_TYPE_WARNING, "Warning message");
	EXPECT_EQ(warningMessageResult, HQ_SUCCESS);
	EXPECT_EQ(line.type, HQ_MESSAGE_TYPE_WARNING);
	EXPECT_STREQ(line.message, "Warning message");

	// Test writing an 'error' string.
	const int errorMessageResult = HqReportMessage(hReport, HQ_MESSAGE_TYPE_ERROR, "Error message");
	EXPECT_EQ(errorMessageResult, HQ_SUCCESS);
	EXPECT_EQ(line.type, HQ_MESSAGE_TYPE_ERROR);
	EXPECT_STREQ(line.message, "Error message");

	// Test writing a 'fatal' string.
	const int fatalMessageResult = HqReportMessage(hReport, HQ_MESSAGE_TYPE_FATAL, "Fatal message");
	EXPECT_EQ(fatalMessageResult, HQ_SUCCESS);
	EXPECT_EQ(line.type, HQ_MESSAGE_TYPE_FATAL);
	EXPECT_STREQ(line.message, "Fatal message");

	// Dispose of the VM context.
	const int disposeContextResult = HqVmDispose(&hVm);
	EXPECT_EQ(disposeContextResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

// TODO: Restore this test once we can actually compile and execute script bytecode.
#if 0
TEST(TestVm, Execution)
{
	HqVmInit init = ConstructInitObject(nullptr, HQ_MESSAGE_TYPE_FATAL, DummyMessageCallback);
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqFunctionHandle hFunc = HQ_FUNCTION_HANDLE_NULL;
	HqExecutionHandle hExec = HQ_EXECUTION_HANDLE_NULL;

	// Create the VM context.
	const int createContextResult = HqVmCreate(&hVm, init);
	ASSERT_EQ(createContextResult, HQ_SUCCESS);

	// Create an execution handle.
	const int createExecutionResult = HqExecutionCreate(&hExec, hVm, hFunc);
	ASSERT_EQ(createExecutionResult, HQ_SUCCESS);

	const size_t testValueCount = 4;

	HqValue testValue[testValueCount] =
	{
		HqValueCreateBool(true),
		HqValueCreateUint32(123),
		HqValueCreateFloat64(1.23456789),
		HqValueCreateInt64(-234),
	};

	// Test the parameter stack.
	{
		// Check the initial stack size.
		const size_t stackSizeBeforePush = HqExecutionGetParameterStackSize(executionHandle);
		EXPECT_EQ(stackSizeBeforePush, 0);

		// Push the test values to the execution stack.
		{
			const int pushTestValue0Result = HqExecutionPushParameter(executionHandle, &testValue[0]);
			EXPECT_EQ(pushTestValue0Result, HQ_SUCCESS);

			const int pushTestValue1Result = HqExecutionPushParameter(executionHandle, &testValue[1]);
			EXPECT_EQ(pushTestValue1Result, HQ_SUCCESS);

			const int pushTestValue2Result = HqExecutionPushParameter(executionHandle, &testValue[2]);
			EXPECT_EQ(pushTestValue2Result, HQ_SUCCESS);

			const int pushTestValue3Result = HqExecutionPushParameter(executionHandle, &testValue[3]);
			EXPECT_EQ(pushTestValue3Result, HQ_SUCCESS);
		}

		// Check the stack size after pushing each test value to the stack.
		const size_t stackSizeAfterPush = HqExecutionGetParameterStackSize(executionHandle);
		EXPECT_EQ(stackSizeAfterPush, testValueCount);

		// Peek at the values on the stack.
		{
			HqValue peekValue = HqValueCreateNull();

			const int peekTestValue3Result = HqExecutionPeekParameter(executionHandle, &peekValue, 0);
			EXPECT_EQ(peekTestValue3Result, HQ_SUCCESS);
			EXPECT_EQ(peekValue.type, HQ_VALUE_TYPE_INT64);
			EXPECT_EQ(peekValue.as.int64, testValue[3].as.int64);

			const int peekTestValue3 = HqValueGcExpose(&peekValue);
			EXPECT_EQ(peekTestValue3, HQ_SUCCESS);

			peekValue = HqValueCreateNull();

			const int peekTestValue2Result = HqExecutionPeekParameter(executionHandle, &peekValue, 1);
			EXPECT_EQ(peekTestValue2Result, HQ_SUCCESS);
			EXPECT_EQ(peekValue.type, HQ_VALUE_TYPE_FLOAT64);
			EXPECT_EQ(peekValue.as.float64, testValue[2].as.float64);

			const int peekTestValue2 = HqValueGcExpose(&peekValue);
			EXPECT_EQ(peekTestValue2, HQ_SUCCESS);

			peekValue = HqValueCreateNull();

			const int peekTestValue1Result = HqExecutionPeekParameter(executionHandle, &peekValue, 2);
			EXPECT_EQ(peekTestValue1Result, HQ_SUCCESS);
			EXPECT_EQ(peekValue.type, HQ_VALUE_TYPE_UINT32);
			EXPECT_EQ(peekValue.as.uint32, testValue[1].as.uint32);

			const int peekTestValue1 = HqValueGcExpose(&peekValue);
			EXPECT_EQ(peekTestValue1, HQ_SUCCESS);

			peekValue = HqValueCreateNull();

			const int peekTestValue0Result = HqExecutionPeekParameter(executionHandle, &peekValue, 3);
			EXPECT_EQ(peekTestValue0Result, HQ_SUCCESS);
			EXPECT_EQ(peekValue.type, HQ_VALUE_TYPE_BOOL);
			EXPECT_EQ(peekValue.as.boolean, testValue[0].as.boolean);

			const int peekTestValue0 = HqValueGcExpose(&peekValue);
			EXPECT_EQ(peekTestValue0, HQ_SUCCESS);
		}

		// Check the stack size after peeking at each value on the stack.
		const size_t stackSizeAfterPeek = HqExecutionGetParameterStackSize(executionHandle);
		EXPECT_EQ(stackSizeAfterPeek, testValueCount);

		// Pop the values from the stack.
		{
			HqValue popValue = HqValueCreateNull();

			const int popTestValue3Result = HqExecutionPopParameter(executionHandle, &popValue);
			EXPECT_EQ(popTestValue3Result, HQ_SUCCESS);
			EXPECT_EQ(popValue.type, HQ_VALUE_TYPE_INT64);
			EXPECT_EQ(popValue.as.int64, testValue[3].as.int64);

			const int popTestValue3 = HqValueGcExpose(&popValue);
			EXPECT_EQ(popTestValue3, HQ_SUCCESS);

			popValue = HqValueCreateNull();

			const int popTestValue2Result = HqExecutionPopParameter(executionHandle, &popValue);
			EXPECT_EQ(popTestValue2Result, HQ_SUCCESS);
			EXPECT_EQ(popValue.type, HQ_VALUE_TYPE_FLOAT64);
			EXPECT_EQ(popValue.as.float64, testValue[2].as.float64);

			const int popTestValue2 = HqValueGcExpose(&popValue);
			EXPECT_EQ(popTestValue2, HQ_SUCCESS);

			popValue = HqValueCreateNull();

			const int popTestValue1Result = HqExecutionPopParameter(executionHandle, &popValue);
			EXPECT_EQ(popTestValue1Result, HQ_SUCCESS);
			EXPECT_EQ(popValue.type, HQ_VALUE_TYPE_UINT32);
			EXPECT_EQ(popValue.as.uint32, testValue[1].as.uint32);

			const int popTestValue1 = HqValueGcExpose(&popValue);
			EXPECT_EQ(popTestValue1, HQ_SUCCESS);

			popValue = HqValueCreateNull();

			const int popTestValue0Result = HqExecutionPopParameter(executionHandle, &popValue);
			EXPECT_EQ(popTestValue0Result, HQ_SUCCESS);
			EXPECT_EQ(popValue.type, HQ_VALUE_TYPE_BOOL);
			EXPECT_EQ(popValue.as.boolean, testValue[0].as.boolean);

			const int popTestValue0 = HqValueGcExpose(&popValue);
			EXPECT_EQ(popTestValue0, HQ_SUCCESS);
		}

		// Check the stack size after popping each value from the stack.
		const size_t stackSizeAfterPop = HqExecutionGetParameterStackSize(executionHandle);
		EXPECT_EQ(stackSizeAfterPop, 0);
	}

	// Test the local variable stack.
	{
		// Check the initial stack size.
		const size_t localCountBeforePush = HqExecutionGetLocalVariableCount(executionHandle);
		EXPECT_EQ(localCountBeforePush, 0);

		// Push the test values to the parameter stack.
		{
			const int pushTestValue0Result = HqExecutionPushLocalVariable(executionHandle, &testValue[0]);
			EXPECT_EQ(pushTestValue0Result, HQ_SUCCESS);

			const int pushTestValue1Result = HqExecutionPushLocalVariable(executionHandle, &testValue[1]);
			EXPECT_EQ(pushTestValue1Result, HQ_SUCCESS);

			const int pushTestValue2Result = HqExecutionPushLocalVariable(executionHandle, &testValue[2]);
			EXPECT_EQ(pushTestValue2Result, HQ_SUCCESS);

			const int pushTestValue3Result = HqExecutionPushLocalVariable(executionHandle, &testValue[3]);
			EXPECT_EQ(pushTestValue3Result, HQ_SUCCESS);
		}

		// Check the stack size after pushing each test value to the stack.
		const size_t localCountAfterPush = HqExecutionGetLocalVariableCount(executionHandle);
		EXPECT_EQ(localCountAfterPush, testValueCount);

		// Retrieve local variables currently on the local stack.
		{
			HqValue localValue = HqValueCreateNull();

			const int localTestValue0Result = HqExecutionGetLocalVariable(executionHandle, &localValue, 0);
			EXPECT_EQ(localTestValue0Result, HQ_SUCCESS);
			EXPECT_EQ(localValue.type, HQ_VALUE_TYPE_BOOL);
			EXPECT_EQ(localValue.as.boolean, testValue[0].as.boolean);

			const int localTestValue0 = HqValueGcExpose(&localValue);
			EXPECT_EQ(localTestValue0, HQ_SUCCESS);

			localValue = HqValueCreateNull();

			const int localTestValue1Result = HqExecutionGetLocalVariable(executionHandle, &localValue, 1);
			EXPECT_EQ(localTestValue1Result, HQ_SUCCESS);
			EXPECT_EQ(localValue.type, HQ_VALUE_TYPE_UINT32);
			EXPECT_EQ(localValue.as.uint32, testValue[1].as.uint32);

			const int localTestValue1 = HqValueGcExpose(&localValue);
			EXPECT_EQ(localTestValue1, HQ_SUCCESS);

			localValue = HqValueCreateNull();

			const int localTestValue2Result = HqExecutionGetLocalVariable(executionHandle, &localValue, 2);
			EXPECT_EQ(localTestValue2Result, HQ_SUCCESS);
			EXPECT_EQ(localValue.type, HQ_VALUE_TYPE_FLOAT64);
			EXPECT_EQ(localValue.as.float64, testValue[2].as.float64);

			const int localTestValue2 = HqValueGcExpose(&localValue);
			EXPECT_EQ(localTestValue2, HQ_SUCCESS);

			localValue = HqValueCreateNull();

			const int localTestValue3Result = HqExecutionGetLocalVariable(executionHandle, &localValue, 3);
			EXPECT_EQ(localTestValue3Result, HQ_SUCCESS);
			EXPECT_EQ(localValue.type, HQ_VALUE_TYPE_INT64);
			EXPECT_EQ(localValue.as.int64, testValue[3].as.int64);

			const int localTestValue3 = HqValueGcExpose(&localValue);
			EXPECT_EQ(localTestValue3, HQ_SUCCESS);
		}

		// Check the stack size after peeking at each value on the stack.
		const size_t localCountAfterPeek = HqExecutionGetLocalVariableCount(executionHandle);
		EXPECT_EQ(localCountAfterPeek, testValueCount);

		// Pop the values from the stack.
		{
			const int popTestValue3Result = HqExecutionPopLocalVariable(executionHandle);
			EXPECT_EQ(popTestValue3Result, HQ_SUCCESS);

			const int popTestValue2Result = HqExecutionPopLocalVariable(executionHandle);
			EXPECT_EQ(popTestValue2Result, HQ_SUCCESS);

			const int popTestValue1Result = HqExecutionPopLocalVariable(executionHandle);
			EXPECT_EQ(popTestValue1Result, HQ_SUCCESS);

			const int popTestValue0Result = HqExecutionPopLocalVariable(executionHandle);
			EXPECT_EQ(popTestValue0Result, HQ_SUCCESS);
		}

		// Check the stack size after popping each value from the stack.
		const size_t localCountAfterPop = HqExecutionGetLocalVariableCount(executionHandle);
		EXPECT_EQ(localCountAfterPop, 0);
	}

	// Dispose of the VM context.
	const int disposeContextResult = HqVmDispose(&hVm);
	EXPECT_EQ(disposeContextResult, HQ_SUCCESS);
}
#endif

//----------------------------------------------------------------------------------------------------------------------
