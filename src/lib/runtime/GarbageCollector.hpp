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

#include "../base/Mutex.hpp"
#include "../base/RwLock.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqGcProxy;
struct HqGarbageCollector
{
	static void Initialize(HqGarbageCollector& output, HqVmHandle hVm, const uint32_t maxTimeSliceMs);
	static void Dispose(HqGarbageCollector& gc);

	static void RunStep(HqGarbageCollector& gc);
	static void RunFull(HqGarbageCollector& gc);

	static void LinkObject(HqGarbageCollector& gc, HqGcProxy* const pGcProxy);
	static void MarkObject(HqGarbageCollector& gc, HqGcProxy* const pGcProxy);

	static int prv_runPhase(HqGarbageCollector&);
	static bool prv_hasReachedTimeSlice(HqGarbageCollector&);
	static void prv_reset(HqGarbageCollector&);
	static void prv_onDisposeObject(HqGcProxy*);
	static void prv_proxyInsertBefore(HqGcProxy*, HqGcProxy*);
	static void prv_proxyInsertAfter(HqGcProxy*, HqGcProxy*);
	static void prv_proxyUnlink(HqGcProxy*);

	HqRwLock rwLock;
	HqMutex pendingLock;

	HqVmHandle hVm;

	HqGcProxy* pPendingHead;
	HqGcProxy* pUnmarkedHead;
	HqGcProxy* pMarkedLeafHead;
	HqGcProxy* pMarkedLeafTail;
	HqGcProxy* pMarkedHead;
	HqGcProxy* pMarkedTail;

	HqGcProxy* pIterCurrent;
	HqGcProxy* pIterPrev;

	uint64_t maxTimeSlice;
	uint64_t startTime;

	uint32_t currentMarkId;
	uint32_t lastMarkId;

	int phase;
	int lastPhase;
};

//----------------------------------------------------------------------------------------------------------------------
