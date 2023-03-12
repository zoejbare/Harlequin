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

#include "../../common/Util.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

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

TEST(_HQ_TEST_NAME(TestVm), CreateAndDisposeContext)
{
	HqVmInit init = GetDefaultHqVmInit(nullptr, nullptr, HQ_MESSAGE_TYPE_FATAL);
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;

	// Create the VM context.
	const int createContextResult = HqVmCreate(&hVm, init);
	ASSERT_EQ(createContextResult, HQ_SUCCESS);

	// Dispose of the VM context.
	const int disposeContextResult = HqVmDispose(&hVm);
	EXPECT_EQ(disposeContextResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestVm), ReportMessages)
{
	ReportLine line;

	HqVmInit init = GetDefaultHqVmInit(&line, StoreReportLine, HQ_MESSAGE_TYPE_VERBOSE);
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

TEST(_HQ_TEST_NAME(TestVm), EmptyExecutionContext)
{
	HqVmInit init = GetDefaultHqVmInit(nullptr, nullptr, HQ_MESSAGE_TYPE_FATAL);
	HqVmHandle hVm = HQ_VM_HANDLE_NULL;
	HqExecutionHandle hExec = HQ_EXECUTION_HANDLE_NULL;

	// Create the VM context.
	const int createVmResult = HqVmCreate(&hVm, init);
	ASSERT_EQ(createVmResult, HQ_SUCCESS);
	EXPECT_NE(hVm, HQ_VM_HANDLE_NULL);

	// Create an execution context.
	const int createExecResult = HqExecutionCreate(&hExec, hVm);
	ASSERT_EQ(createExecResult, HQ_SUCCESS);
	EXPECT_NE(hExec, HQ_EXECUTION_HANDLE_NULL);

	// Reset the execution context; this should succeed even though the
	// context has not been initialized with a script function.
	const int resetExecResult = HqExecutionReset(hExec);
	ASSERT_EQ(resetExecResult, HQ_SUCCESS);

	// Attempt to run the execution context without a script function.
	const int runExecResult = HqExecutionRun(hExec, HQ_RUN_FULL);
	ASSERT_EQ(runExecResult, HQ_ERROR_SCRIPT_NO_FUNCTION);

	// Attempt to yield outside of bytecode execution.
	const int yieldExecResult = HqExecutionYield(hExec);
	ASSERT_EQ(yieldExecResult, HQ_ERROR_SCRIPT_NO_FUNCTION);

	// Attempt to raise a standard exception outside of bytecode execution.
	const int raiseStdExceptionResult = HqExecutionRaiseStandardException(
		hExec,
		HQ_EXCEPTION_SEVERITY_FATAL,
		HQ_STANDARD_EXCEPTION_RUNTIME_ERROR,
		"%s",
		"TestException"
	);
	ASSERT_EQ(raiseStdExceptionResult, HQ_ERROR_SCRIPT_NO_FUNCTION);

	// Attempt to raise a custom exception outside of bytecode execution.
	const int raiseExceptionResult = HqExecutionRaiseException(hExec, HQ_VALUE_HANDLE_NULL, HQ_EXCEPTION_SEVERITY_FATAL);
	ASSERT_EQ(raiseExceptionResult, HQ_ERROR_SCRIPT_NO_FUNCTION);

	// Get the VM that the execution context belongs to.
	HqVmHandle hTempVm = HQ_VM_HANDLE_NULL;
	const int getVmResult = HqExecutionGetVm(hExec, &hTempVm);
	ASSERT_EQ(getVmResult, HQ_SUCCESS);
	ASSERT_EQ(hTempVm, hVm);

	// Get the 'execution aborted' status.
	bool abortStatus = false;
	const int getAbortStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_ABORT, &abortStatus);
	ASSERT_EQ(getAbortStatusResult, HQ_SUCCESS);
	ASSERT_FALSE(abortStatus);

	// Get the 'execution complete' status.
	bool completeStatus = false;
	const int getCompleteStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_COMPLETE, &completeStatus);
	ASSERT_EQ(getCompleteStatusResult, HQ_SUCCESS);
	ASSERT_FALSE(completeStatus);

	// Get the 'exception raised' status.
	bool exceptionStatus = false;
	const int getExceptionStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_EXCEPTION, &exceptionStatus);
	ASSERT_EQ(getExceptionStatusResult, HQ_SUCCESS);
	ASSERT_FALSE(exceptionStatus);

	// Get the 'execution in progress' status.
	bool runningStatus = false;
	const int getRunningStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_RUNNING, &runningStatus);
	ASSERT_EQ(getRunningStatusResult, HQ_SUCCESS);
	ASSERT_FALSE(runningStatus);

	// Get the 'execution yielded' status.
	bool yieldStatus = false;
	const int getYieldStatusResult = HqExecutionGetStatus(hExec, HQ_EXEC_STATUS_YIELD, &yieldStatus);
	ASSERT_EQ(getYieldStatusResult, HQ_SUCCESS);
	ASSERT_FALSE(yieldStatus);

	// Get the unhandled exception flag.
	bool unhandledException = false;
	const int hasUnhandledExceptionResult = HqExecutionHasUnhandledExceptionOccurred(hExec, &unhandledException);
	ASSERT_EQ(hasUnhandledExceptionResult, HQ_SUCCESS);
	ASSERT_FALSE(unhandledException);

	// Get the current frame stack depth.
	size_t frameStackDepth = 0;
	const int getFrameStackDepthResult = HqExecutionGetFrameStackDepth(hExec, &frameStackDepth);
	ASSERT_EQ(getFrameStackDepthResult, HQ_SUCCESS);
	ASSERT_EQ(frameStackDepth, 0);

	// Get the current frame.
	HqFrameHandle hFrame = HQ_FRAME_HANDLE_NULL;
	const int getCurrentFrameResult = HqExecutionGetCurrentFrame(hExec, &hFrame);
	ASSERT_EQ(getCurrentFrameResult, HQ_SUCCESS);
	ASSERT_EQ(hFrame, HQ_FRAME_HANDLE_NULL);

	// Set each I/O register on the execution context.
	for(size_t i = 0; i < HQ_VM_IO_REGISTER_COUNT; ++i)
	{
		HqValueHandle hValue = HqValueCreateUint64(hVm, uint64_t(i));

		const int setIoRegisterResult = HqExecutionSetIoRegister(hExec, hValue, int(i));
		ASSERT_EQ(setIoRegisterResult, HQ_SUCCESS);

		const int gcExposeIoValueResult = HqValueGcExpose(hValue);
		ASSERT_EQ(gcExposeIoValueResult, HQ_SUCCESS);
	}

	// Verify each I/O register is correct.
	for(size_t i = 0; i < HQ_VM_IO_REGISTER_COUNT; ++i)
	{
		HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;

		const int getIoRegisterResult = HqExecutionGetIoRegister(hExec, &hValue, int(i));
		ASSERT_EQ(getIoRegisterResult, HQ_SUCCESS);

		const bool isIoValueUint64 = HqValueIsUint64(hValue);
		ASSERT_TRUE(isIoValueUint64);

		const uint64_t valueData = HqValueGetUint64(hValue);
		ASSERT_EQ(valueData, uint64_t(i));
	}

	// Dispose of the execution context.
	const int disposeExecResult = HqExecutionDispose(&hExec);
	ASSERT_EQ(disposeExecResult, HQ_SUCCESS);
	EXPECT_EQ(hExec, HQ_EXECUTION_HANDLE_NULL);

	// Dispose of the VM context.
	const int disposeVmResult = HqVmDispose(&hVm);
	EXPECT_EQ(disposeVmResult, HQ_SUCCESS);
	EXPECT_EQ(hVm, HQ_VM_HANDLE_NULL);
}

//----------------------------------------------------------------------------------------------------------------------
