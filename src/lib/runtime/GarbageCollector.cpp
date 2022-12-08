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

#include "GarbageCollector.hpp"
#include "GcProxy.hpp"
#include "Value.hpp"
#include "Vm.hpp"

#include "../base/Clock.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

enum HqGcPhase
{
	HQ_GC_PHASE_LINK_PENDING,
	HQ_GC_PHASE_RESET_STATE,
	HQ_GC_PHASE_AUTO_MARK_DISCOVERY,
	HQ_GC_PHASE_GLOBAL_DISCOVERY,
	HQ_GC_PHASE_MARK_RECURSIVE,
	HQ_GC_PHASE_DISPOSE,

	HQ_GC_PHASE__COUNT,
	HQ_GC_PHASE__START = HQ_GC_PHASE_LINK_PENDING,
	HQ_GC_PHASE__END = HQ_GC_PHASE_DISPOSE,
};

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::Initialize(HqGarbageCollector& output, HqVmHandle hVm, const uint32_t maxTimeSliceMs)
{
	assert(hVm != HQ_VM_HANDLE_NULL);

	HqRwLock::Create(output.rwLock);
	HqMutex::Create(output.pendingLock);

	output.hVm = hVm;
	output.pPendingHead = nullptr;
	output.pUnmarkedHead = nullptr;
	output.pMarkedHead = nullptr;
	output.pMarkedTail = nullptr;
	output.pIterCurrent = nullptr;
	output.pIterPrev = nullptr;
	output.phase = 0;
	output.lastPhase = 0;
	output.maxTimeSlice = maxTimeSliceMs * HqClockGetFrequency() / 1000;

	// Reset the garbage collector so we're guaranteed to kick things off in a good state.
	prv_reset(output);
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::Dispose(HqGarbageCollector& gc)
{
	auto disableAutoMark = [](HqGcProxy* const pListHead)
	{
		if(pListHead)
		{
			HqGcProxy* pCurrent = pListHead;

			// Disable auto-mark on each proxy.
			while(pCurrent)
			{
				pCurrent->autoMark = false;
				pCurrent = pCurrent->pNext;
			}
		}
	};

	// Disable auto-mark on all proxies. They should all be have auto-mark removed prior to getting
	// to this point, but any user code that screwed up and didn't abandon some values or other
	// object types might still have auto-mark enabled.
	disableAutoMark(gc.pPendingHead);
	disableAutoMark(gc.pUnmarkedHead);
	disableAutoMark(gc.pMarkedHead);

	// Continue running the garbage collector until everything has been released.
	for(;;)
	{
		RunFull(gc);

		if(!gc.pPendingHead && !gc.pUnmarkedHead && !gc.pMarkedHead)
		{
			break;
		}
	};

	HqRwLock::Dispose(gc.rwLock);
	HqMutex::Dispose(gc.pendingLock);

	gc.hVm = HQ_VM_HANDLE_NULL;
	gc.pPendingHead = nullptr;
	gc.pUnmarkedHead = nullptr;
	gc.pMarkedHead = nullptr;
	gc.pMarkedTail = nullptr;
	gc.pIterCurrent = nullptr;
	gc.pIterPrev = nullptr;
	gc.phase = 0;
	gc.lastPhase = 0;
	gc.maxTimeSlice = 0;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqGarbageCollector::RunPhase(HqGarbageCollector& gc)
{
	HqScopedWriteLock writeLock(gc.rwLock);

	return prv_runPhase(gc);
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::RunFull(HqGarbageCollector& gc)
{
	HqScopedWriteLock writeLock(gc.rwLock);

	// Reset the garbage collector state so that running it again starts at the beginning of the 1st phase.
	prv_reset(gc);

	// To minimize the number of steps that need to be run, we cache the maximum time slice allowed for a single GC phase,
	// then override it to 0, effectively disabling the timeout. This will allow us to run each phase as a single step.
	const uint64_t oldMaxTimeSlice = gc.maxTimeSlice;
	gc.maxTimeSlice = 0;

	while(!prv_runPhase(gc))
	{
		// Run the garbage collector until all phases have been run.
	}

	// Restore the maximum time slice.
	gc.maxTimeSlice = oldMaxTimeSlice;
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::LinkObject(HqGarbageCollector& gc, HqGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);

	HqScopedMutex lock(gc.pendingLock);

	// Link the proxy the head of the pending list.
	pGcProxy->pending = true;

	if(gc.pPendingHead)
	{
		prv_proxyInsertBefore(gc.pPendingHead, pGcProxy);
	}

	gc.pPendingHead = pGcProxy;
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::MarkObject(HqGarbageCollector& gc, HqGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);
	assert(pGcProxy->pObject != nullptr);

	if(!pGcProxy->marked && !pGcProxy->pending)
	{
		pGcProxy->marked = true;

		if(gc.pUnmarkedHead == pGcProxy)
		{
			gc.pUnmarkedHead = pGcProxy->pNext;
		}

		prv_proxyUnlink(pGcProxy);

		if(!gc.pMarkedHead)
		{
			// Nothing in the marked list current, so the input proxy becomes the new marked list.
			gc.pMarkedHead = pGcProxy;
			gc.pMarkedTail = pGcProxy;
		}
		else
		{
			prv_proxyInsertAfter(gc.pMarkedTail, pGcProxy);

			gc.pMarkedTail = pGcProxy;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

bool HqGarbageCollector::prv_runPhase(HqGarbageCollector& gc)
{
	const bool isPhaseStart = (gc.lastPhase != gc.phase);

	bool endOfAllPhases = false;
	bool endOfPhase = false;

	const uint64_t startTime = HqClockGetTimestamp();

	switch(gc.phase)
	{
		// Transfer all pending proxies to the active list.
		case HQ_GC_PHASE_LINK_PENDING:
		{
			HqScopedMutex lock(gc.pendingLock);

			const uint64_t pendingStartTime = HqClockGetTimestamp();

			// Iterate until the end of the list has been reached.
			while(gc.pPendingHead)
			{
				if(prv_hasReachedTimeSlice(gc, pendingStartTime))
				{
					// We ran out of time.
					break;
				}

				HqGcProxy* const pCurrent = gc.pPendingHead;
				HqGcProxy* const pNext = pCurrent->pNext;

				prv_proxyUnlink(pCurrent);

				// Link the object at the head of the pending list to the head of the active unmarked list.
				if(gc.pUnmarkedHead)
				{
					prv_proxyInsertBefore(gc.pUnmarkedHead, pCurrent);
				}

				// Clear the proxy's 'pending' state.
				pCurrent->pending = false;

				// Update the heads of the unmarked and pending lists.
				gc.pUnmarkedHead = pCurrent;
				gc.pPendingHead = pNext;
			}

			if(!gc.pPendingHead)
			{
				// We have reached the end of the phase when there are no more proxies in the pending list.
				endOfPhase = true;
			}
			break;
		}

		// Reset the 'marked' state for each active proxy.
		case HQ_GC_PHASE_RESET_STATE:
		{
			if(isPhaseStart)
			{
				// For the start of the phase, set the current proxy pointer to the head of the active list.
				gc.pIterCurrent = gc.pUnmarkedHead;
			}

			// Iterate until the end of the list has been reached.
			while(gc.pIterCurrent)
			{
				if(prv_hasReachedTimeSlice(gc, startTime))
				{
					// We ran out of time.
					break;
				}

				// Reset the proxy state.
				gc.pIterCurrent->marked = false;

				// Move to the next proxy in the list.
				gc.pIterCurrent = gc.pIterCurrent->pNext;
			}

			if(!gc.pIterCurrent)
			{
				// We have reached the end of the phase once all proxies in the list have been checked.
				endOfPhase = true;
			}
			break;
		}

		// Discover anything that is set to auto-mark.
		case HQ_GC_PHASE_AUTO_MARK_DISCOVERY:
		{
			if(isPhaseStart)
			{
				// For the start of the phase, set the current proxy pointer to the head of the active list.
				gc.pIterCurrent = gc.pUnmarkedHead;
			}

			// Iterate until the end of the list has been reached.
			while(gc.pIterCurrent)
			{
				if(prv_hasReachedTimeSlice(gc, startTime))
				{
					// We ran out of time.
					break;
				}

				HqGcProxy* const pNext = gc.pIterCurrent->pNext;

				// Add any auto-mark objects to the active marked list so they and
				// their sub-objects will be marked prior to the collection phase.
				if(gc.pIterCurrent->autoMark)
				{
					MarkObject(gc, gc.pIterCurrent);
				}

				// Move to the next proxy in the list.
				gc.pIterCurrent = pNext;
			}

			if(!gc.pIterCurrent)
			{
				// We have reached the end of the phase once all objects in the list have been checked.
				endOfPhase = true;
			}
			break;
		}

		// Discover all global variables.
		case HQ_GC_PHASE_GLOBAL_DISCOVERY:
		{
			// There's no good way to go over the globals incrementally since the map can hypothetically
			// change between steps of the garbage collector. We also can't rely on auto-marking because
			// globals can change what values they point to. Since we're just enqueuing proxies, it shouldn't
			// be too bad to loop over all the globals at once. This may only become problematic if a script
			// program contains many hundreds of globals or more. We'll cross that bridge when we come to it.
			for(auto& kv : gc.hVm->globals)
			{
				HqValueHandle hValue = HQ_MAP_ITER_VALUE(kv);

				if(HqValue::CanBeMarked(hValue))
				{
					MarkObject(gc, &(hValue->gcProxy));
				}
			}

			endOfPhase = true;
			break;
		}

		// Recursively mark all active objects.
		case HQ_GC_PHASE_MARK_RECURSIVE:
		{
			if(isPhaseStart)
			{
				// For the start of the phase, set the current proxy pointer to the head of the marked active list.
				// This will go through all marked objects and mark their dependencies.
				gc.pIterCurrent = gc.pMarkedHead;
			}

			// Iterate until the end of the list has been reached.
			while(gc.pIterCurrent)
			{
				if(prv_hasReachedTimeSlice(gc, startTime))
				{
					// We ran out of time.
					break;
				}

				// Discover any garbage collected objects that need to be marked contained within the current object.
				gc.pIterCurrent->onGcDiscoveryFn(gc, gc.pIterCurrent->pObject);

				// Move to the next proxy in the list.
				gc.pIterCurrent = gc.pIterCurrent->pNext;
			}

			if(!gc.pIterCurrent)
			{
				// We have reached the end of the phase once all objects in the list have been checked.
				endOfPhase = true;
			}
			break;
		}

		// Dispose of any objects that are no longer in use.
		case HQ_GC_PHASE_DISPOSE:
		{
			// Iterate until the end of the list has been reached.
			while(gc.pUnmarkedHead)
			{
				if(prv_hasReachedTimeSlice(gc, startTime))
				{
					// We ran out of time.
					break;
				}

				// Cache the next proxy in case the current one is freed from memory when disposed.
				HqGcProxy* const pNext = gc.pUnmarkedHead->pNext;

				// Dispose of the current proxy.
				prv_onDisposeObject(gc.pUnmarkedHead);

				// Update the head of the unmarked list.
				gc.pUnmarkedHead = pNext;
			}

			if(!gc.pUnmarkedHead)
			{
				// The end of the phase is when there are no unmarked proxies remaining.
				endOfPhase = true;
			}
			break;
		}

		default:
			// This should never happen.
			assert(false);
			break;
	}

	// Cache the current phase so we know if the last step was running a different phase.
	// This effectively lets us detect the start of a phase.
	gc.lastPhase = gc.phase;

	if(endOfPhase)
	{
		// Move to the next phase.
		gc.phase = (gc.phase + 1) % HQ_GC_PHASE__COUNT;

		// The end of all phases is triggered when we have looped back to the first phase.
		endOfAllPhases = (gc.phase == HQ_GC_PHASE__START);
	}

	return endOfAllPhases;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqGarbageCollector::prv_hasReachedTimeSlice(HqGarbageCollector& gc, const uint64_t startTime)
{
	return (gc.maxTimeSlice > 0) && ((HqClockGetTimestamp() - startTime) >= gc.maxTimeSlice);
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::prv_reset(HqGarbageCollector& gc)
{
	if(gc.pMarkedHead)
	{
		if(gc.pUnmarkedHead)
		{
			// Link the head of the unmarked objects directly to the end of the marked list.
			// We need to do this manually to preserve the integrity of the unmarked list.
			// Otherwise, some items in the unmarked list will be silently unlinked which
			// will cause memory leaks.
			gc.pMarkedTail->pNext = gc.pUnmarkedHead;
			gc.pUnmarkedHead->pPrev = gc.pMarkedTail;
		}

		// The entire marked list now becomes the unmarked list.
		gc.pUnmarkedHead = gc.pMarkedHead;
	}

	gc.pMarkedHead = nullptr;
	gc.pMarkedTail = nullptr;

	gc.phase = HQ_GC_PHASE__START;
	gc.lastPhase = HQ_GC_PHASE__END;
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::prv_onDisposeObject(HqGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);

	pGcProxy->onGcDisposeFn(pGcProxy->pObject);
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::prv_proxyInsertBefore(HqGcProxy* const pGcListProxy, HqGcProxy* const pGcInsertProxy)
{
	assert(pGcListProxy != nullptr);
	assert(pGcInsertProxy != nullptr);

	HqGcProxy* const pListPrev = pGcListProxy->pPrev;

	if(pListPrev)
	{
		pListPrev->pNext = pGcInsertProxy;
	}

	pGcListProxy->pPrev = pGcInsertProxy;

	pGcInsertProxy->pPrev = pListPrev;
	pGcInsertProxy->pNext = pGcListProxy;
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::prv_proxyInsertAfter(HqGcProxy* const pGcListProxy, HqGcProxy* const pGcInsertProxy)
{
	assert(pGcListProxy != nullptr);
	assert(pGcInsertProxy != nullptr);

	HqGcProxy* const pListNext = pGcListProxy->pNext;

	if(pListNext)
	{
		pListNext->pPrev = pGcInsertProxy;
	}

	pGcListProxy->pNext = pGcInsertProxy;

	pGcInsertProxy->pPrev = pGcListProxy;
	pGcInsertProxy->pNext = pListNext;
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::prv_proxyUnlink(HqGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);

	HqGcProxy* const pPrev = pGcProxy->pPrev;
	HqGcProxy* const pNext = pGcProxy->pNext;

	if(pPrev)
	{
		pPrev->pNext = pNext;
	}

	if(pNext)
	{
		pNext->pPrev = pPrev;
	}

	pGcProxy->pPrev = nullptr;
	pGcProxy->pNext = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
