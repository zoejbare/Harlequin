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

#include "Execution.hpp"
#include "Program.hpp"
#include "Value.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonGcProxy;
struct XenonGarbageCollector
{
	static void Initialize(XenonGarbageCollector& output, XenonVmHandle hVm, const uint32_t maxIterationCount);
	static void Dispose(XenonGarbageCollector& gc);

	static bool RunStep(XenonGarbageCollector& gc);
	static void RunFull(XenonGarbageCollector& gc);

	static void LinkObject(XenonGarbageCollector& gc, XenonGcProxy& proxy);
	static void DiscoverProxy(XenonGarbageCollector& gc, XenonGcProxy& proxy);

	static void prv_reset(XenonGarbageCollector&);
	static void prv_clearQueue(XenonGarbageCollector&);
	static void prv_onDisposeProxy(XenonGcProxy*);

	XenonGcProxy::BatchedPtrQueue proxyQueue;
	XenonGcProxy::BatchedPtrQueue::ReadReservationTicket proxyReadTicket;

	XenonVmHandle hVm;

	XenonGcProxy* pActiveHead;
	XenonGcProxy* pPendingHead;
	XenonGcProxy* pUnmarkedHead;

	XenonGcProxy* pIterCurrent;
	XenonGcProxy* pIterPrev;

	int phase;
	int lastPhase;

	uint32_t maxIterationCount;
};

//----------------------------------------------------------------------------------------------------------------------
