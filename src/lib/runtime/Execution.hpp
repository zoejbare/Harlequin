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

#include "../common/Map.hpp"
#include "../common/Stack.hpp"
#include "../common/StlAllocator.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqProgram;

struct HqExecution
{
	typedef HqStack<HqExecutionHandle> HandleStack;

	typedef HQ_MAP_TYPE<
		HqExecutionHandle,
		bool,
#if HQ_MAP_IS_UNORDERED
		std::hash<HqExecutionHandle>,
		std::equal_to<HqExecutionHandle>,
#else
		std::less<HqExecutionHandle>,
#endif
		HqStlAllocator<HQ_MAP_NODE_TYPE(HqExecutionHandle, bool)>
	> HandleToBoolMap;

	static HqExecutionHandle Create(HqVmHandle hVm, HqFunctionHandle hEntryPoint);
	static void ReleaseWithNoDetach(HqExecutionHandle hExec);
	static void DetachFromVm(HqExecutionHandle hExec);

	static int PushFrame(HqExecutionHandle hExec, HqFunctionHandle hFunction);
	static int PopFrame(HqExecutionHandle hExec);

	static int SetIoRegister(HqExecutionHandle hExec, HqValueHandle hValue, const size_t index);

	static HqValueHandle GetIoRegister(HqExecutionHandle hExec, const size_t index, int* const pOutResult);

	static void Run(HqExecutionHandle hExec, const int runMode);

	static void RaiseException(HqExecutionHandle hExec, HqValueHandle hValue, const int severity);
	static void RaiseOpCodeException(HqExecutionHandle hExec, const int type, const char* const fmt, ...);

	static void prv_runStep(HqExecutionHandle);
	static void prv_onGcDiscovery(HqGarbageCollector&, void*);
	static void prv_onGcDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqGcProxy gcProxy;

	HqVmHandle hVm;
	HqFrameHandle hCurrentFrame;

	HqFrame::HandleStack frameStack;
	HqFrame::HandleStack framePool;

	HqValue::HandleArray registers;

	uint8_t* pExceptionLocation;

	int endianness;

	bool yield;
	bool started;
	bool finished;
	bool exception;
	bool abort;
};

//----------------------------------------------------------------------------------------------------------------------
