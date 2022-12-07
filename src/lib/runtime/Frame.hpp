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

#include "Decoder.hpp"
#include "Function.hpp"
#include "GcProxy.hpp"
#include "Value.hpp"

#include "../common/Array.hpp"
#include "../common/Stack.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqFrame
{
	typedef HqArray<HqFrameHandle> HandleArray;
	typedef HqStack<HqFrameHandle> HandleStack;

	static HqFrameHandle Create(HqExecutionHandle hExec);

	static void Initialize(HqFrameHandle hFrame, HqFunctionHandle hFunction);
	static void Reset(HqFrameHandle hFrame);

	static int PushValue(HqFrameHandle hFrame, HqValueHandle hValue);
	static int PopValue(HqFrameHandle hFrame, HqValueHandle* const phOutValue);
	static int PeekValue(HqFrameHandle hFrame, HqValueHandle* const phOutValue, const size_t index);

	static int SetGpRegister(HqFrameHandle hFrame, HqValueHandle hValue, const uint32_t index);
	static int SetLocalVariable(HqFrameHandle hFrame, HqValueHandle hValue, HqString* const pVariableName);

	static HqValueHandle GetGpRegister(HqFrameHandle hFrame, const uint32_t index, int* const pOutResult);
	static HqValueHandle GetLocalVariable(HqFrameHandle hFrame, HqString* const pVariableName, int* const pOutResult);

	static void prv_onGcDiscovery(HqGarbageCollector&, void*);
	static void prv_onGcDestruct(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	HqGcProxy gcProxy;

	HqValue::HandleStack stack;
	HqValue::HandleArray registers;
	HqValue::StringToHandleMap locals;

	HqExecutionHandle hExec;
	HqFunctionHandle hFunction;

	HqDecoder decoder;
};

//----------------------------------------------------------------------------------------------------------------------
