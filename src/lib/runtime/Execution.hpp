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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "Frame.hpp"
#include "GcProxy.hpp"
#include "Value.hpp"

#include "../base/Fiber.hpp"
#include "../common/Array.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqProgram;

struct HqExecution
{
	typedef HqArray<HqExecutionHandle> HandleArray;

	static HqExecutionHandle Create(HqVmHandle hVm);

	static void Dispose(HqExecutionHandle hExec);

	static int Initialize(HqExecutionHandle hExec, HqFunctionHandle hEntryPoint);
	static int Reset(HqExecutionHandle hExec);

	static int PushFrame(HqExecutionHandle hExec, HqFunctionHandle hFunction);
	static int PopFrame(HqExecutionHandle hExec);

	static int SetIoRegister(HqExecutionHandle hExec, HqValueHandle hValue, const size_t index);

	static HqValueHandle GetIoRegister(HqExecutionHandle hExec, const size_t index, int* const pOutResult);

	static void Run(HqExecutionHandle hExec, const int runMode);
	static void Pause(HqExecutionHandle hExec);

	static void RaiseException(HqExecutionHandle hExec, HqValueHandle hValue, const int severity);
	static void RaiseOpCodeException(HqExecutionHandle hExec, const int type, const char* const fmt, ...);

	static void prv_createMainFiber(HqExecutionHandle);
	static void prv_runFiberLoop(void*);
	static void prv_runStep(HqExecutionHandle);
	static void prv_onGcDiscovery(HqGarbageCollector&, void*);
	static void prv_onGcDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqGcProxy gcProxy;

	HqVmHandle hVm;
	HqFunctionHandle hFunction;
	HqFrameHandle hCurrentFrame;

	HqFrame::HandleStack frameStack;
	HqFrame::HandleStack framePool;

	HqValue::HandleArray registers;

	HqFiber mainFiber;

	uint8_t* pExceptionLocation;

	uint32_t endianness;
	uint32_t lastOpCode;

	volatile uint32_t runMode;
	
	struct InternalState
	{
		bool yield : 1;
		bool started : 1;
		bool finished : 1;
		bool exception : 1;
		bool abort : 1;
	};

	union
	{
		volatile InternalState state;
		volatile uint32_t stateBits;
	};

	bool firstRun;
	bool frameStackDirty;

	bool created;
};

//----------------------------------------------------------------------------------------------------------------------
