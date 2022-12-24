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

#define _HQ_GC_TIME_CHECK_INTERVAL 10

//----------------------------------------------------------------------------------------------------------------------

enum HqGcPhase
{
	_HQ_GC_PHASE_LINK_PENDING,
	_HQ_GC_PHASE_AUTO_MARK,
	_HQ_GC_PHASE_GLOBAL_MARK,
	_HQ_GC_PHASE_DISCOVERY,
	_HQ_GC_PHASE_DISPOSE,

	_HQ_GC_PHASE__COUNT,
	_HQ_GC_PHASE__START = _HQ_GC_PHASE_LINK_PENDING,
	_HQ_GC_PHASE__END = _HQ_GC_PHASE_DISPOSE,
};

enum HqGcRunResult
{
	_HQ_GC_RUN_MORE_WORK,
	_HQ_GC_RUN_TIME_OUT,
	_HQ_GC_RUN_COMPLETE,
};

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::Initialize(HqGarbageCollector& output, HqVmHandle hVm, const uint32_t maxTimeSliceMs)
{
	assert(hVm != HQ_VM_HANDLE_NULL);
	assert(maxTimeSliceMs > 0);

	HqRwLock::Create(output.rwLock);
	HqMutex::Create(output.pendingLock);

	output.hVm = hVm;
	output.pPendingHead = nullptr;
	output.pUnmarkedHead = nullptr;
	output.pMarkedLeafHead = nullptr;
	output.pMarkedLeafTail = nullptr;
	output.pMarkedHead = nullptr;
	output.pMarkedTail = nullptr;
	output.pIterCurrent = nullptr;
	output.pIterPrev = nullptr;
	output.maxTimeSlice = maxTimeSliceMs * HqClockGetFrequency() / 1000;
	output.startTime = 0;
	output.timeCheck = 0;
	output.currentMarkId = 0;
	output.lastMarkId = 0;
	output.phase = 0;
	output.lastPhase = 0;

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
	disableAutoMark(gc.pMarkedLeafHead);
	disableAutoMark(gc.pMarkedHead);

	// Continue running the garbage collector until everything has been released.
	for(;;)
	{
		RunFull(gc);

		if(!gc.pPendingHead && !gc.pMarkedLeafHead && !gc.pUnmarkedHead && !gc.pMarkedHead)
		{
			break;
		}
	};

	HqRwLock::Dispose(gc.rwLock);
	HqMutex::Dispose(gc.pendingLock);

	gc.hVm = HQ_VM_HANDLE_NULL;
	gc.pPendingHead = nullptr;
	gc.pUnmarkedHead = nullptr;
	gc.pMarkedLeafHead = nullptr;
	gc.pMarkedLeafTail = nullptr;
	gc.pMarkedHead = nullptr;
	gc.pMarkedTail = nullptr;
	gc.pIterCurrent = nullptr;
	gc.pIterPrev = nullptr;
	gc.maxTimeSlice = 0;
	gc.startTime = 0;
	gc.timeCheck = 0;
	gc.phase = 0;
	gc.lastPhase = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::RunStep(HqGarbageCollector& gc)
{
	HqScopedWriteLock writeLock(gc.rwLock, gc.hVm->isGcThreadEnabled);

	gc.startTime = HqClockGetTimestamp();
	gc.timeCheck = 0;

	// Keep running phases while the GC reports there is more work that can be done right now.
	while(prv_runPhase(gc) == _HQ_GC_RUN_MORE_WORK)
	{
	}
}

//----------------------------------------------------------------------------------------------------------------------

void HqGarbageCollector::RunFull(HqGarbageCollector& gc)
{
	HqScopedWriteLock writeLock(gc.rwLock, gc.hVm->isGcThreadEnabled);

	// Reset the garbage collector state so that running it again starts at the beginning of the 1st phase.
	prv_reset(gc);

	// To minimize the number of steps that need to be run, we cache the maximum time slice allowed for a single GC phase,
	// then override it to 0, effectively disabling the timeout. This will allow us to run each phase as a single step.
	const uint64_t oldMaxTimeSlice = gc.maxTimeSlice;
	gc.maxTimeSlice = 0;

	// Run the garbage collector until all phases have been run.
	for(;;)
	{
		const int result = prv_runPhase(gc);
		assert(result != _HQ_GC_RUN_TIME_OUT);

		if(result == _HQ_GC_RUN_COMPLETE)
		{
			break;
		}
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

	// Only mark proxies that we detect being unmarked and the ones we know are already in an active list.
	if(pGcProxy->markId != gc.currentMarkId)
	{
		// Update the proxy's mark ID.
		pGcProxy->markId = gc.currentMarkId;

		if(gc.pUnmarkedHead == pGcProxy)
		{
			// If the current proxy is the head of the unmarked list, we reassign the head to the next proxy.
			gc.pUnmarkedHead = pGcProxy->pNext;
		}

		if(gc.pPendingHead == pGcProxy)
		{
			// If the current proxy is the head of the pending list, we reassign the head to the next proxy.
			gc.pPendingHead = pGcProxy->pNext;
		}

		// Unlink the current proxy from its current list.
		prv_proxyUnlink(pGcProxy);

		if(pGcProxy->pending)
		{
			// Clear the 'pending' flag if it had not yet been linked into an active list.
			// Pending proxies need to be allowed normal marking behavior in case they have
			// dependent proxies that are already in an active list. Otherwise, discovery
			// will not happen, so the dependent proxies would get destructed.
			pGcProxy->pending = false;
		}

		if(pGcProxy->discover)
		{
			// This proxy has dependencies that need to be discovered, so it goes in the normal marked list.
			if(!gc.pMarkedHead)
			{
				// Nothing in the marked list currently, so the input proxy becomes the new marked list.
				gc.pMarkedHead = pGcProxy;
				gc.pMarkedTail = pGcProxy;
			}
			else
			{
				// Append this proxy to the end of the marked list.
				prv_proxyInsertAfter(gc.pMarkedTail, pGcProxy);

				// This proxy becomes the new tail of the marked list.
				gc.pMarkedTail = pGcProxy;
			}
		}
		else
		{
			// This proxy has no dependencies, so we can move it to the marked leaf list.
			// The marked leaf list receives no discovery phase of its own which means
			// it's removed from the unmarked list just so it doesn't get deleted.
			if(!gc.pMarkedLeafHead)
			{
				// Nothing in the marked leaf list currently, so the input proxy becomes the new marked leaf list.
				gc.pMarkedLeafHead = pGcProxy;
				gc.pMarkedLeafTail = pGcProxy;
			}
			else
			{
				// Append this proxy to the end of the marked leaf list.
				prv_proxyInsertAfter(gc.pMarkedLeafTail, pGcProxy);

				// This proxy becomes the new tail of the marked leaf list.
				gc.pMarkedLeafTail = pGcProxy;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline int HqGarbageCollector::prv_runPhase(HqGarbageCollector& gc)
{
	const bool isPhaseStart = (gc.lastPhase != gc.phase);

	bool endOfAllPhases = false;
	bool endOfPhase = false;
	bool timeOut = false;

	switch(gc.phase)
	{
		// Transfer all pending proxies to the active list.
		case _HQ_GC_PHASE_LINK_PENDING:
		{
			HqScopedMutex lock(gc.pendingLock);

			// Iterate until the end of the list has been reached.
			while(gc.pPendingHead)
			{
				if(prv_hasReachedTimeSlice(gc))
				{
					// We ran out of time.
					timeOut = true;
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

				// Initialize the proxy's mark ID so there's no chance an object going into an active list
				// will overlap with the current ID.
				pCurrent->markId = gc.lastMarkId;

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

		// Mark anything that is set to auto-mark.
		case _HQ_GC_PHASE_AUTO_MARK:
		{
			if(isPhaseStart)
			{
				// For the start of the phase, set the current proxy pointer to the head of the active list.
				gc.pIterCurrent = gc.pUnmarkedHead;
			}

			// Iterate until the end of the list has been reached.
			while(gc.pIterCurrent)
			{
				if(prv_hasReachedTimeSlice(gc))
				{
					// We ran out of time.
					timeOut = true;
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

		// Mark all global variables.
		case _HQ_GC_PHASE_GLOBAL_MARK:
		{
			// Force the time check counter so the timestamp is actually checked here.
			// We need this since the time check can only happen once in this phase.
			gc.timeCheck = _HQ_GC_TIME_CHECK_INTERVAL;

			// If we managed to reach this point right as we run out of time, it's better to signal the timeout now.
			// Doing this will allow the global marking phase longer to run without potentially exceeding the time
			// allowed for a single GC step.
			if(prv_hasReachedTimeSlice(gc))
			{
				// We ran out of time.
				timeOut = true;
				break;
			}

			// There's no good way to go over the globals incrementally since the map can hypothetically
			// change between steps of the garbage collector. We also can't rely on auto-marking because
			// globals can change what values they point to. Since we're just enqueuing proxies, it shouldn't
			// be too bad to loop over all the globals at once. This may only become problematic if a script
			// program contains many hundreds of globals or more. We'll cross that bridge when we come to it.
			HqValue::StringToHandleMap::Iterator iter;
			while(HqValue::StringToHandleMap::IterateNext(gc.hVm->globals, iter))
			{
				HqValueHandle hValue = iter.pData->value;

				if(hValue)
				{
					MarkObject(gc, &(hValue->gcProxy));
				}
			}

			// Reset the time check counter at the end of all this to guarantee
			// a time check is done at the start of the next phase.
			gc.timeCheck = _HQ_GC_TIME_CHECK_INTERVAL;

			endOfPhase = true;
			break;
		}

		// Mark any dependent proxies referenced by the current marked list.
		case _HQ_GC_PHASE_DISCOVERY:
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
				if(prv_hasReachedTimeSlice(gc))
				{
					// We ran out of time.
					timeOut = true;
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
		case _HQ_GC_PHASE_DISPOSE:
		{
			// Iterate until the end of the list has been reached.
			while(gc.pUnmarkedHead)
			{
				if(prv_hasReachedTimeSlice(gc))
				{
					// We ran out of time.
					timeOut = true;
					break;
				}

				// Cache the next proxy in case the current one is freed from memory when disposed.
				HqGcProxy* const pNext = gc.pUnmarkedHead->pNext;

				// Dispose of the current proxy.
				gc.pUnmarkedHead->onGcDisposeFn(gc.pUnmarkedHead->pObject);

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
		gc.phase = (gc.phase + 1) % _HQ_GC_PHASE__COUNT;

		// The end of all phases is triggered when we have looped back to the first phase.
		endOfAllPhases = (gc.phase == _HQ_GC_PHASE__START);
	}

	return endOfAllPhases
		? _HQ_GC_RUN_COMPLETE
		: timeOut
			? _HQ_GC_RUN_TIME_OUT
			: _HQ_GC_RUN_MORE_WORK;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqGarbageCollector::prv_hasReachedTimeSlice(HqGarbageCollector& gc)
{
	if(gc.maxTimeSlice == 0)
	{
		return false;
	}

	// Limit the number of times we're allowed to do the time slice check. Getting the current timestamp
	// isn't terribly expensive by itself, but it will add up when called in rapid succession.
	++gc.timeCheck;
	if((gc.timeCheck % _HQ_GC_TIME_CHECK_INTERVAL) != 0)
	{
		return false;
	}

	return (HqClockGetTimestamp() - gc.startTime) >= gc.maxTimeSlice;
}

//----------------------------------------------------------------------------------------------------------------------

inline void HqGarbageCollector::prv_reset(HqGarbageCollector& gc)
{
	if(gc.pMarkedLeafHead)
	{
		// The easiest way to concatenate the marked leaf list into everything else is to
		// just make it part of the marked list.
		if(gc.pMarkedHead)
		{
			// Link the marked list and the marked leaf list directly to each other.
			gc.pMarkedLeafHead->pPrev = gc.pMarkedTail;
			gc.pMarkedTail->pNext = gc.pMarkedLeafHead;
			gc.pMarkedTail = gc.pMarkedLeafTail;
		}
		else
		{
			// The marked leaf list becomes the marked list.
			gc.pMarkedHead = gc.pMarkedLeafHead;
			gc.pMarkedTail = gc.pMarkedLeafTail;
		}
	}

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

	gc.pMarkedLeafHead = nullptr;
	gc.pMarkedLeafTail = nullptr;

	gc.pMarkedHead = nullptr;
	gc.pMarkedTail = nullptr;

	gc.lastMarkId = gc.currentMarkId;
	++gc.currentMarkId;

	gc.phase = _HQ_GC_PHASE__START;
	gc.lastPhase = _HQ_GC_PHASE__END;
}

//----------------------------------------------------------------------------------------------------------------------

inline void HqGarbageCollector::prv_proxyInsertBefore(HqGcProxy* const pGcListProxy, HqGcProxy* const pGcInsertProxy)
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

inline void HqGarbageCollector::prv_proxyInsertAfter(HqGcProxy* const pGcListProxy, HqGcProxy* const pGcInsertProxy)
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

inline void HqGarbageCollector::prv_proxyUnlink(HqGcProxy* const pGcProxy)
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

#undef _HQ_GC_TIME_CHECK_INTERVAL

//----------------------------------------------------------------------------------------------------------------------
