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

#include "../common/Stack.hpp"
#include "../common/StlAllocator.hpp"

#include <SkipProbe/SkipProbe.hpp>

//----------------------------------------------------------------------------------------------------------------------

struct XenonProgram;

struct XenonExecution
{
	typedef XenonStack<XenonExecutionHandle> HandleStack;

	typedef SkipProbe::HashMap<
		XenonExecutionHandle,
		bool,
		SkipProbe::Hash<XenonExecutionHandle>,
		std::equal_to<XenonExecutionHandle>,
		XenonStlAllocator<SkipProbe::LinkedNode<XenonExecutionHandle, bool>>
	> HandleToBoolMap;

	static XenonExecutionHandle Create(XenonVmHandle hVm, XenonFunctionHandle hEntryPoint);
	static void ReleaseWithNoDetach(XenonExecutionHandle hExec);
	static void DetachFromVm(XenonExecutionHandle hExec);

	static int PushFrame(XenonExecutionHandle hExec, XenonFunctionHandle hFunction);
	static int PopFrame(XenonExecutionHandle hExec);

	static int SetIoRegister(XenonExecutionHandle hExec, XenonValueHandle hValue, const size_t index);

	static XenonValueHandle GetIoRegister(XenonExecutionHandle hExec, const size_t index, int* const pOutResult);

	static void Run(XenonExecutionHandle hExec, const int runMode);

	static void prv_runStep(XenonExecutionHandle);
	static void prv_onGcDiscovery(XenonGarbageCollector&, void*);
	static void prv_onGcDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	XenonGcProxy gcProxy;

	XenonVmHandle hVm;
	XenonFrameHandle hCurrentFrame;

	XenonFrame::HandleStack frameStack;
	XenonValue::HandleArray registers;

	int endianness;

	bool yield;
	bool started;
	bool finished;
	bool exception;
};

//----------------------------------------------------------------------------------------------------------------------
