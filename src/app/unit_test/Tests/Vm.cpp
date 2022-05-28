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

#include <string>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------

XenonVmInit ConstructInitObject(void* const pUserData, const int reportLevel, XenonMessageCallback onMessageFn)
{
	auto onDependencyRequested = [](void*, const char*)
	{
		// Don't need this right now, just stubbing it in to keep the context creation from failing.
	};

	XenonVmInit output;
	output.common.report.onMessageFn = onMessageFn;
	output.common.report.pUserData = pUserData;
	output.common.report.reportLevel = reportLevel;
	output.gcThreadStackSize = XENON_VM_THREAD_DEFAULT_STACK_SIZE;
	output.gcMaxIterationCount = XENON_VM_GC_DEFAULT_ITERATION_COUNT;

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
	XenonVmInit init = ConstructInitObject(nullptr, XENON_MESSAGE_TYPE_FATAL, DummyMessageCallback);
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;

	// Create the VM context.
	const int createContextResult = XenonVmCreate(&hVm, init);
	ASSERT_EQ(createContextResult, XENON_SUCCESS);

	// Dispose of the VM context.
	const int disposeContextResult = XenonVmDispose(&hVm);
	EXPECT_EQ(disposeContextResult, XENON_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(TestVm, ReportMessages)
{
	ReportLine line;

	XenonVmInit init = ConstructInitObject(&line, XENON_MESSAGE_TYPE_VERBOSE, StoreReportLine);
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;

	// Create the VM context.
	const int createContextResult = XenonVmCreate(&hVm, init);
	ASSERT_EQ(createContextResult, XENON_SUCCESS);

	XenonReportHandle hReport = XENON_REPORT_HANDLE_NULL;
	const int getReportHandleResult = XenonVmGetReportHandle(hVm, &hReport);
	ASSERT_EQ(getReportHandleResult, XENON_SUCCESS);
	EXPECT_NE(hReport, XENON_REPORT_HANDLE_NULL);

	// Test writing a 'verbose' string.
	const int verboseMessageResult = XenonReportMessage(hReport, XENON_MESSAGE_TYPE_VERBOSE, "Verbose message");
	EXPECT_EQ(verboseMessageResult, XENON_SUCCESS);
	EXPECT_EQ(line.type, XENON_MESSAGE_TYPE_VERBOSE);
	EXPECT_STREQ(line.message, "Verbose message");

	// Test writing an 'info' string.
	const int infoMessageResult = XenonReportMessage(hReport, XENON_MESSAGE_TYPE_INFO, "Info message");
	EXPECT_EQ(infoMessageResult, XENON_SUCCESS);
	EXPECT_EQ(line.type, XENON_MESSAGE_TYPE_INFO);
	EXPECT_STREQ(line.message, "Info message");

	// Test writing a 'warning' string.
	const int warningMessageResult = XenonReportMessage(hReport, XENON_MESSAGE_TYPE_WARNING, "Warning message");
	EXPECT_EQ(warningMessageResult, XENON_SUCCESS);
	EXPECT_EQ(line.type, XENON_MESSAGE_TYPE_WARNING);
	EXPECT_STREQ(line.message, "Warning message");

	// Test writing an 'error' string.
	const int errorMessageResult = XenonReportMessage(hReport, XENON_MESSAGE_TYPE_ERROR, "Error message");
	EXPECT_EQ(errorMessageResult, XENON_SUCCESS);
	EXPECT_EQ(line.type, XENON_MESSAGE_TYPE_ERROR);
	EXPECT_STREQ(line.message, "Error message");

	// Test writing a 'fatal' string.
	const int fatalMessageResult = XenonReportMessage(hReport, XENON_MESSAGE_TYPE_FATAL, "Fatal message");
	EXPECT_EQ(fatalMessageResult, XENON_SUCCESS);
	EXPECT_EQ(line.type, XENON_MESSAGE_TYPE_FATAL);
	EXPECT_STREQ(line.message, "Fatal message");

	// Dispose of the VM context.
	const int disposeContextResult = XenonVmDispose(&hVm);
	EXPECT_EQ(disposeContextResult, XENON_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

// TODO: Restore this test once we can actually compile and execute script bytecode.
#if 0
TEST(TestVm, Execution)
{
	XenonVmInit init = ConstructInitObject(nullptr, XENON_MESSAGE_TYPE_FATAL, DummyMessageCallback);
	XenonVmHandle hVm = XENON_VM_HANDLE_NULL;
	XenonFunctionHandle hFunc = XENON_FUNCTION_HANDLE_NULL;
	XenonExecutionHandle hExec = XENON_EXECUTION_HANDLE_NULL;

	// Create the VM context.
	const int createContextResult = XenonVmCreate(&hVm, init);
	ASSERT_EQ(createContextResult, XENON_SUCCESS);

	// Create an execution handle.
	const int createExecutionResult = XenonExecutionCreate(&hExec, hVm, hFunc);
	ASSERT_EQ(createExecutionResult, XENON_SUCCESS);

	const size_t testValueCount = 4;

	XenonValue testValue[testValueCount] =
	{
		XenonValueCreateBool(true),
		XenonValueCreateUint32(123),
		XenonValueCreateFloat64(1.23456789),
		XenonValueCreateInt64(-234),
	};

	// Test the parameter stack.
	{
		// Check the initial stack size.
		const size_t stackSizeBeforePush = XenonExecutionGetParameterStackSize(executionHandle);
		EXPECT_EQ(stackSizeBeforePush, 0);

		// Push the test values to the execution stack.
		{
			const int pushTestValue0Result = XenonExecutionPushParameter(executionHandle, &testValue[0]);
			EXPECT_EQ(pushTestValue0Result, XENON_SUCCESS);

			const int pushTestValue1Result = XenonExecutionPushParameter(executionHandle, &testValue[1]);
			EXPECT_EQ(pushTestValue1Result, XENON_SUCCESS);

			const int pushTestValue2Result = XenonExecutionPushParameter(executionHandle, &testValue[2]);
			EXPECT_EQ(pushTestValue2Result, XENON_SUCCESS);

			const int pushTestValue3Result = XenonExecutionPushParameter(executionHandle, &testValue[3]);
			EXPECT_EQ(pushTestValue3Result, XENON_SUCCESS);
		}

		// Check the stack size after pushing each test value to the stack.
		const size_t stackSizeAfterPush = XenonExecutionGetParameterStackSize(executionHandle);
		EXPECT_EQ(stackSizeAfterPush, testValueCount);

		// Peek at the values on the stack.
		{
			XenonValue peekValue = XenonValueCreateNull();

			const int peekTestValue3Result = XenonExecutionPeekParameter(executionHandle, &peekValue, 0);
			EXPECT_EQ(peekTestValue3Result, XENON_SUCCESS);
			EXPECT_EQ(peekValue.type, XENON_VALUE_TYPE_INT64);
			EXPECT_EQ(peekValue.as.int64, testValue[3].as.int64);

			const int peekTestValue3 = XenonValueAbandon(&peekValue);
			EXPECT_EQ(peekTestValue3, XENON_SUCCESS);

			peekValue = XenonValueCreateNull();

			const int peekTestValue2Result = XenonExecutionPeekParameter(executionHandle, &peekValue, 1);
			EXPECT_EQ(peekTestValue2Result, XENON_SUCCESS);
			EXPECT_EQ(peekValue.type, XENON_VALUE_TYPE_FLOAT64);
			EXPECT_EQ(peekValue.as.float64, testValue[2].as.float64);

			const int peekTestValue2 = XenonValueAbandon(&peekValue);
			EXPECT_EQ(peekTestValue2, XENON_SUCCESS);

			peekValue = XenonValueCreateNull();

			const int peekTestValue1Result = XenonExecutionPeekParameter(executionHandle, &peekValue, 2);
			EXPECT_EQ(peekTestValue1Result, XENON_SUCCESS);
			EXPECT_EQ(peekValue.type, XENON_VALUE_TYPE_UINT32);
			EXPECT_EQ(peekValue.as.uint32, testValue[1].as.uint32);

			const int peekTestValue1 = XenonValueAbandon(&peekValue);
			EXPECT_EQ(peekTestValue1, XENON_SUCCESS);

			peekValue = XenonValueCreateNull();

			const int peekTestValue0Result = XenonExecutionPeekParameter(executionHandle, &peekValue, 3);
			EXPECT_EQ(peekTestValue0Result, XENON_SUCCESS);
			EXPECT_EQ(peekValue.type, XENON_VALUE_TYPE_BOOL);
			EXPECT_EQ(peekValue.as.boolean, testValue[0].as.boolean);

			const int peekTestValue0 = XenonValueAbandon(&peekValue);
			EXPECT_EQ(peekTestValue0, XENON_SUCCESS);
		}

		// Check the stack size after peeking at each value on the stack.
		const size_t stackSizeAfterPeek = XenonExecutionGetParameterStackSize(executionHandle);
		EXPECT_EQ(stackSizeAfterPeek, testValueCount);

		// Pop the values from the stack.
		{
			XenonValue popValue = XenonValueCreateNull();

			const int popTestValue3Result = XenonExecutionPopParameter(executionHandle, &popValue);
			EXPECT_EQ(popTestValue3Result, XENON_SUCCESS);
			EXPECT_EQ(popValue.type, XENON_VALUE_TYPE_INT64);
			EXPECT_EQ(popValue.as.int64, testValue[3].as.int64);

			const int popTestValue3 = XenonValueAbandon(&popValue);
			EXPECT_EQ(popTestValue3, XENON_SUCCESS);

			popValue = XenonValueCreateNull();

			const int popTestValue2Result = XenonExecutionPopParameter(executionHandle, &popValue);
			EXPECT_EQ(popTestValue2Result, XENON_SUCCESS);
			EXPECT_EQ(popValue.type, XENON_VALUE_TYPE_FLOAT64);
			EXPECT_EQ(popValue.as.float64, testValue[2].as.float64);

			const int popTestValue2 = XenonValueAbandon(&popValue);
			EXPECT_EQ(popTestValue2, XENON_SUCCESS);

			popValue = XenonValueCreateNull();

			const int popTestValue1Result = XenonExecutionPopParameter(executionHandle, &popValue);
			EXPECT_EQ(popTestValue1Result, XENON_SUCCESS);
			EXPECT_EQ(popValue.type, XENON_VALUE_TYPE_UINT32);
			EXPECT_EQ(popValue.as.uint32, testValue[1].as.uint32);

			const int popTestValue1 = XenonValueAbandon(&popValue);
			EXPECT_EQ(popTestValue1, XENON_SUCCESS);

			popValue = XenonValueCreateNull();

			const int popTestValue0Result = XenonExecutionPopParameter(executionHandle, &popValue);
			EXPECT_EQ(popTestValue0Result, XENON_SUCCESS);
			EXPECT_EQ(popValue.type, XENON_VALUE_TYPE_BOOL);
			EXPECT_EQ(popValue.as.boolean, testValue[0].as.boolean);

			const int popTestValue0 = XenonValueAbandon(&popValue);
			EXPECT_EQ(popTestValue0, XENON_SUCCESS);
		}

		// Check the stack size after popping each value from the stack.
		const size_t stackSizeAfterPop = XenonExecutionGetParameterStackSize(executionHandle);
		EXPECT_EQ(stackSizeAfterPop, 0);
	}

	// Test the local variable stack.
	{
		// Check the initial stack size.
		const size_t localCountBeforePush = XenonExecutionGetLocalVariableCount(executionHandle);
		EXPECT_EQ(localCountBeforePush, 0);

		// Push the test values to the parameter stack.
		{
			const int pushTestValue0Result = XenonExecutionPushLocalVariable(executionHandle, &testValue[0]);
			EXPECT_EQ(pushTestValue0Result, XENON_SUCCESS);

			const int pushTestValue1Result = XenonExecutionPushLocalVariable(executionHandle, &testValue[1]);
			EXPECT_EQ(pushTestValue1Result, XENON_SUCCESS);

			const int pushTestValue2Result = XenonExecutionPushLocalVariable(executionHandle, &testValue[2]);
			EXPECT_EQ(pushTestValue2Result, XENON_SUCCESS);

			const int pushTestValue3Result = XenonExecutionPushLocalVariable(executionHandle, &testValue[3]);
			EXPECT_EQ(pushTestValue3Result, XENON_SUCCESS);
		}

		// Check the stack size after pushing each test value to the stack.
		const size_t localCountAfterPush = XenonExecutionGetLocalVariableCount(executionHandle);
		EXPECT_EQ(localCountAfterPush, testValueCount);

		// Retrieve local variables currently on the local stack.
		{
			XenonValue localValue = XenonValueCreateNull();

			const int localTestValue0Result = XenonExecutionGetLocalVariable(executionHandle, &localValue, 0);
			EXPECT_EQ(localTestValue0Result, XENON_SUCCESS);
			EXPECT_EQ(localValue.type, XENON_VALUE_TYPE_BOOL);
			EXPECT_EQ(localValue.as.boolean, testValue[0].as.boolean);

			const int localTestValue0 = XenonValueAbandon(&localValue);
			EXPECT_EQ(localTestValue0, XENON_SUCCESS);

			localValue = XenonValueCreateNull();

			const int localTestValue1Result = XenonExecutionGetLocalVariable(executionHandle, &localValue, 1);
			EXPECT_EQ(localTestValue1Result, XENON_SUCCESS);
			EXPECT_EQ(localValue.type, XENON_VALUE_TYPE_UINT32);
			EXPECT_EQ(localValue.as.uint32, testValue[1].as.uint32);

			const int localTestValue1 = XenonValueAbandon(&localValue);
			EXPECT_EQ(localTestValue1, XENON_SUCCESS);

			localValue = XenonValueCreateNull();

			const int localTestValue2Result = XenonExecutionGetLocalVariable(executionHandle, &localValue, 2);
			EXPECT_EQ(localTestValue2Result, XENON_SUCCESS);
			EXPECT_EQ(localValue.type, XENON_VALUE_TYPE_FLOAT64);
			EXPECT_EQ(localValue.as.float64, testValue[2].as.float64);

			const int localTestValue2 = XenonValueAbandon(&localValue);
			EXPECT_EQ(localTestValue2, XENON_SUCCESS);

			localValue = XenonValueCreateNull();

			const int localTestValue3Result = XenonExecutionGetLocalVariable(executionHandle, &localValue, 3);
			EXPECT_EQ(localTestValue3Result, XENON_SUCCESS);
			EXPECT_EQ(localValue.type, XENON_VALUE_TYPE_INT64);
			EXPECT_EQ(localValue.as.int64, testValue[3].as.int64);

			const int localTestValue3 = XenonValueAbandon(&localValue);
			EXPECT_EQ(localTestValue3, XENON_SUCCESS);
		}

		// Check the stack size after peeking at each value on the stack.
		const size_t localCountAfterPeek = XenonExecutionGetLocalVariableCount(executionHandle);
		EXPECT_EQ(localCountAfterPeek, testValueCount);

		// Pop the values from the stack.
		{
			const int popTestValue3Result = XenonExecutionPopLocalVariable(executionHandle);
			EXPECT_EQ(popTestValue3Result, XENON_SUCCESS);

			const int popTestValue2Result = XenonExecutionPopLocalVariable(executionHandle);
			EXPECT_EQ(popTestValue2Result, XENON_SUCCESS);

			const int popTestValue1Result = XenonExecutionPopLocalVariable(executionHandle);
			EXPECT_EQ(popTestValue1Result, XENON_SUCCESS);

			const int popTestValue0Result = XenonExecutionPopLocalVariable(executionHandle);
			EXPECT_EQ(popTestValue0Result, XENON_SUCCESS);
		}

		// Check the stack size after popping each value from the stack.
		const size_t localCountAfterPop = XenonExecutionGetLocalVariableCount(executionHandle);
		EXPECT_EQ(localCountAfterPop, 0);
	}

	// Dispose of the VM context.
	const int disposeContextResult = XenonVmDispose(&hVm);
	EXPECT_EQ(disposeContextResult, XENON_SUCCESS);
}
#endif

//----------------------------------------------------------------------------------------------------------------------
