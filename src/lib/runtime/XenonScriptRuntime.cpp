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

#include "../XenonScript.h"

#include "../shared/String.hpp"

#include "Execution.hpp"
#include "Program.hpp"
#include "Vm.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" {

//----------------------------------------------------------------------------------------------------------------------

int XenonVmCreate(XenonVmHandle* phOutVm, XenonVmInit init)
{
	if(!phOutVm
		|| (*phOutVm)
		|| init.common.report.reportLevel < XENON_MESSAGE_TYPE_VERBOSE
		|| init.common.report.reportLevel > XENON_MESSAGE_TYPE_FATAL
		|| !init.dependency.onRequestFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonVm* const pVm = XenonVm::Create(init);

	// The message has to be printed *after* creating the VM state since the state is used in this function.
	XenonReportMessage(&pVm->report, XENON_MESSAGE_TYPE_VERBOSE, "Initializing Xenon runtime ...");

	(*phOutVm) = pVm;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmDispose(XenonVmHandle* phVm)
{
	if(!phVm || !(*phVm))
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonVmHandle hVm = (*phVm);

	XenonReportMessage(&hVm->report, XENON_MESSAGE_TYPE_VERBOSE, "Releasing Xenon runtime...");
	XenonVm::Dispose(hVm);

	(*phVm) = XENON_VM_HANDLE_NULL;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

XenonReportHandle XenonVmGetReportHandle(XenonVmHandle hVm)
{
	return hVm
		? &hVm->report
		: XENON_REPORT_HANDLE_NULL;
}
//----------------------------------------------------------------------------------------------------------------------

XenonFunctionHandle XenonVmGetFunction(XenonVmHandle hVm, const char* signature)
{
	if(!hVm || !signature || signature[0] == '\0')
	{
		return XENON_FUNCTION_HANDLE_NULL;
	}

	// Create a string object since we need that to look up into the map.
	XenonString* const pSig = XenonString::Create(signature);
	if(!pSig)
	{
		return XENON_FUNCTION_HANDLE_NULL;
	}

	XenonFunctionHandle hFunction = XenonVm::GetFunction(hVm, pSig);

	// We no longer need the string object for the function signature.
	XenonString::Dispose(pSig);

	return hFunction;
}

//----------------------------------------------------------------------------------------------------------------------

XenonValueHandle XenonVmGetObjectProfile(XenonVmHandle hVm, const char* objectTypeName)
{
	// TODO: Implement this
	(void) hVm;
	(void) objectTypeName;

	return XENON_VALUE_HANDLE_NULL;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmLoadProgram(
	XenonVmHandle hVm,
	const char* programName,
	const void* pProgramFileData,
	size_t programFileSize
)
{
	if(!hVm || !programName || programName[0] == '\0' || !pProgramFileData || programFileSize == 0)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create a string to be the key in the program map.
	XenonString* const pKey = XenonString::Create(programName);
	if(!pKey)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Check if a program with this name has already been loaded.
	if(hVm->programs.Contains(pKey))
	{
		XenonString::Dispose(pKey);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	// Attempt to load the program file.
	XenonProgramHandle hProgram = XenonProgram::Create(hVm, programName, pProgramFileData, programFileSize);
	if(!hProgram)
	{
		XenonString::Dispose(pKey);
		return XENON_ERROR_FAILED_TO_OPEN_FILE;
	}

	// Map the program inside the VM state.
	hVm->programs.Insert(pKey, hProgram);

	// Report the program dependencies to the user code.
	for(auto& kv : hProgram->dependencies)
	{
		// Only report the dependency if it hasn't already been loaded.
		if(!hVm->programs.Contains(kv.key))
		{
			hVm->dependency.onRequestFn(hVm->dependency.pUserData, kv.key->data);
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonVmLoadProgramFromFile(XenonVmHandle hVm, const char* filePath)
{
	if(!hVm || !filePath || filePath[0] == '\0')
	{
		return XENON_ERROR_INVALID_ARG;
	}

	// Create a string to be the key in the program map.
	XenonString* const pKey = XenonString::Create(filePath);
	if(!pKey)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Check if a program with this name has already been loaded.
	if(hVm->programs.Contains(pKey))
	{
		XenonString::Dispose(pKey);
		return XENON_ERROR_KEY_ALREADY_EXISTS;
	}

	// Attempt to load the program file.
	XenonProgram* const hProgram = XenonProgram::Create(hVm, filePath);
	if(!hProgram)
	{
		XenonString::Dispose(pKey);
		return XENON_ERROR_FAILED_TO_OPEN_FILE;
	}

	// Map the program inside the VM state.
	hVm->programs.Insert(pKey, hProgram);

	// Report the program dependencies to the user code.
	for(auto& kv : hProgram->dependencies)
	{
		// Only report the dependency if it hasn't already been loaded.
		if(!hVm->programs.Contains(kv.key))
		{
			hVm->dependency.onRequestFn(hVm->dependency.pUserData, kv.key->data);
		}
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionCreate(
	XenonExecutionHandle* phOutExecution,
	XenonVmHandle hVm,
	XenonFunctionHandle hEntryPoint
)
{
	if(!phOutExecution
		|| (*phOutExecution)
		|| !hVm
		|| !hEntryPoint)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonExecutionHandle hExec = XenonExecution::Create(hVm, hEntryPoint);
	if(!hExec)
	{
		return XENON_ERROR_BAD_ALLOCATION;
	}

	// Use the execution map as a set to keep track of all execution contexts that belong to the input VM.
	hVm->executionContexts.Insert(hExec, false);

	(*phOutExecution) = hExec;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionDispose(XenonExecutionHandle* phExecution)
{
	if(!phExecution || !(*phExecution))
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonExecutionHandle hExec = (*phExecution);

	hExec->hVm->executionContexts.Delete(hExec);
	XenonExecution::Dispose(hExec);

	(*phExecution) = XENON_EXECUTION_HANDLE_NULL;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionRun(XenonExecutionHandle hExec, int runMode)
{
	if(!hExec)
	{
		return XENON_ERROR_INVALID_ARG;
	}
	else if(!hExec->hCurrentFrame)
	{
		return XENON_ERROR_SCRIPT_NO_FUNCTION;
	}

	// Set the 'started' flag to indicate that execution has started.
	// We also reset the 'yield' flag here since it's only useful for
	// pausing execution and we no longer need it paused until the
	// next yield.
	hExec->started = true;
	hExec->yield = false;

	switch(runMode)
	{
		case XENON_RUN_STEP:
			XenonExecution::RunStep(hExec);
			break;

		case XENON_RUN_TO_COMPLETION:
			while(!hExec->finished && !hExec->exception && !hExec->yield)
			{
				XenonExecution::RunStep(hExec);
			}
			break;
	}

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionYield(XenonExecutionHandle hExec)
{
	if(!hExec)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	hExec->yield = true;

	return XENON_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonExecutionHasYielded(XenonExecutionHandle hExec)
{
	return hExec ? hExec->yield : false;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonExecutionHasStarted(XenonExecutionHandle hExec)
{
	return hExec ? hExec->started : false;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonExecutionHasFinished(XenonExecutionHandle hExec)
{
	return hExec ? hExec->finished : false;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonExecutionHasExceptionOccurred(XenonExecutionHandle hExec)
{
	return hExec ? hExec->exception : false;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonExecutionResolveFrameStack(
	XenonExecutionHandle hExec,
	XenonCallbackResolveFrame onResolveFrameFn,
	void* pUserData
)
{
	if(!hExec || !onResolveFrameFn)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonExecution::ResolveFrameStack(hExec, onResolveFrameFn, pUserData);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFramePushValue(XenonFrameHandle hFrame, XenonValueHandle hValue)
{
	if(!hFrame)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	return XenonFrame::PushValue(hFrame, hValue);
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFramePopValue(XenonFrameHandle hFrame, XenonValueHandle* phOutValue)
{
	if(!hFrame)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	XenonValueHandle hValue = XENON_VALUE_HANDLE_NULL;
	int result = XenonFrame::PopValue(hFrame, &hValue);

	if(phOutValue)
	{
		if(*phOutValue)
		{
			XenonValueDispose(*phOutValue);
		}

		(*phOutValue) = XenonValueReference(hValue);
	}

	XenonValueDispose(hValue);

	return result;
}

//----------------------------------------------------------------------------------------------------------------------

int XenonFramePeekValue(XenonFrameHandle hFrame, XenonValueHandle* phOutValue, int stackIndex)
{
	if(!hFrame || !phOutValue)
	{
		return XENON_ERROR_INVALID_ARG;
	}

	if(*phOutValue)
	{
		XenonValueDispose(*phOutValue);
	}

	return XenonFrame::PeekValue(hFrame, phOutValue, stackIndex);
}

//----------------------------------------------------------------------------------------------------------------------

}
