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

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

enum XenonGcPhase
{
	XENON_GC_PHASE_LINK_PENDING,
	XENON_GC_PHASE_RESET_STATE,
	XENON_GC_PHASE_AUTO_MARK_DISCOVERY,
	XENON_GC_PHASE_GLOBAL_DISCOVERY,
	XENON_GC_PHASE_MARK_RECURSIVE,
	XENON_GC_PHASE_DISPOSE,

	XENON_GC_PHASE__COUNT,
	XENON_GC_PHASE__START = XENON_GC_PHASE_LINK_PENDING,
	XENON_GC_PHASE__END = XENON_GC_PHASE_DISPOSE,
};

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::Initialize(XenonGarbageCollector& output, XenonVmHandle hVm, const uint32_t maxIterationCount)
{
	assert(hVm != XENON_VM_HANDLE_NULL);
	assert(maxIterationCount > 0);

	output.pendingLock = XenonMutex::Create();
	output.hVm = hVm;
	output.pPendingHead = nullptr;
	output.pUnmarkedHead = nullptr;
	output.pMarkedHead = nullptr;
	output.pMarkedTail = nullptr;
	output.pIterCurrent = nullptr;
	output.pIterPrev = nullptr;
	output.phase = 0;
	output.lastPhase = 0;
	output.maxIterationCount = maxIterationCount;

	// Reset the garbage collector so we're guaranteed to kick things off in a good state.
	prv_reset(output);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::Dispose(XenonGarbageCollector& gc)
{
	auto clearList = [](XenonGcProxy* const pListHead)
	{
		if(pListHead)
		{
			XenonGcProxy* pCurrent = pListHead;
			XenonGcProxy* pNext;

			// Dispose of all objects in the list.
			while(pCurrent)
			{
				pNext = pCurrent->pNext;

				prv_onDisposeObject(pCurrent);

				pCurrent = pNext;
			}
		}
	};

	clearList(gc.pPendingHead);
	clearList(gc.pUnmarkedHead);
	clearList(gc.pMarkedHead);

	XenonMutex::Dispose(gc.pendingLock);

	gc.hVm = XENON_VM_HANDLE_NULL;
	gc.pPendingHead = nullptr;
	gc.pUnmarkedHead = nullptr;
	gc.pMarkedHead = nullptr;
	gc.pMarkedTail = nullptr;
	gc.pIterCurrent = nullptr;
	gc.pIterPrev = nullptr;
	gc.phase = 0;
	gc.lastPhase = 0;
	gc.maxIterationCount = 0;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonGarbageCollector::RunStep(XenonGarbageCollector& gc)
{
	bool endOfAllPhases = false;
	bool endOfPhase = false;

	switch(gc.phase)
	{
		// Transfer all pending proxies to the active list.
		case XENON_GC_PHASE_LINK_PENDING:
		{
			XenonScopedMutex lock(gc.pendingLock);

			for(uint32_t index = 0; index < gc.maxIterationCount; ++index)
			{
				if(!gc.pPendingHead)
				{
					// The end of the list has been reached.
					break;
				}

				XenonGcProxy* const pCurrent = gc.pPendingHead;
				XenonGcProxy* const pNext = pCurrent->pNext;

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
		case XENON_GC_PHASE_RESET_STATE:
		{
			if(gc.lastPhase != gc.phase)
			{
				// For the start of the phase, set the current proxy pointer to the head of the active list.
				gc.pIterCurrent = gc.pUnmarkedHead;
			}
			else
			{
				// Iterate over as many items in the list as we're allowed at one time.
				for(uint32_t index = 0; index < gc.maxIterationCount; ++index)
				{
					if(!gc.pIterCurrent)
					{
						// The end of the list has been reached.
						break;
					}

					// Reset the proxy state.
					gc.pIterCurrent->marked = false;

					// Move to the next proxy in the list.
					gc.pIterCurrent = gc.pIterCurrent->pNext;
				}
			}

			if(!gc.pIterCurrent)
			{
				// We have reached the end of the phase once all proxies in the list have been checked.
				endOfPhase = true;
			}
			break;
		}

		// Discover anything that is set to auto-mark.
		case XENON_GC_PHASE_AUTO_MARK_DISCOVERY:
		{
			if(gc.lastPhase != gc.phase)
			{
				// For the start of the phase, set the current proxy pointer to the head of the active list.
				gc.pIterCurrent = gc.pUnmarkedHead;
			}
			else
			{
				// Iterate over as many items in the list as we're allowed at one time.
				for(uint32_t index = 0; index < gc.maxIterationCount; ++index)
				{
					if(!gc.pIterCurrent)
					{
						// The end of the list has been reached.
						break;
					}

					XenonGcProxy* const pNext = gc.pIterCurrent->pNext;

					// Add any auto-mark objects to the active marked list so they and
					// their sub-objects will be marked prior to the collection phase.
					if(gc.pIterCurrent->autoMark)
					{
						MarkObject(gc, gc.pIterCurrent);
					}

					// Move to the next proxy in the list.
					gc.pIterCurrent = pNext;
				}
			}

			if(!gc.pIterCurrent)
			{
				// We have reached the end of the phase once all objects in the list have been checked.
				endOfPhase = true;
			}
			break;
		}

		// Discover all global variables.
		case XENON_GC_PHASE_GLOBAL_DISCOVERY:
		{
			// There's no good way to go over the globals incrementally since the map can hypothetically
			// change between steps of the garbage collector. We also can't rely on auto-marking because
			// globals can change what values they point to. Since we're just enqueuing proxies, it shouldn't
			// be too bad to loop over all the globals at once. This may only become problematic if a script
			// program contains many hundreds of globals or more. We'll cross that bridge when we come to it.
			for(auto& kv : gc.hVm->globals)
			{
				XenonValueHandle hValue = XENON_MAP_ITER_VALUE(kv);

				if(XenonValue::CanBeMarked(hValue))
				{
					MarkObject(gc, &(hValue->gcProxy));
				}
			}

			endOfPhase = true;
			break;
		}

		// Recursively mark all active objects.
		case XENON_GC_PHASE_MARK_RECURSIVE:
		{
			if(gc.lastPhase != gc.phase)
			{
				// For the start of the phase, set the current proxy pointer to the head of the marked active list.
				// This will go through all marked objects and mark their dependencies.
				gc.pIterCurrent = gc.pMarkedHead;
			}
			else
			{
				// Iterate over as many items in the list as we're allowed at one time.
				for(uint32_t index = 0; index < gc.maxIterationCount; ++index)
				{
					if(!gc.pIterCurrent)
					{
						// The end of the list has been reached.
						break;
					}

					// Discover any garbage collected objects that need to be marked contained within the current object.
					gc.pIterCurrent->onGcDiscoveryFn(gc, gc.pIterCurrent->pObject);

					// Move to the next proxy in the list.
					gc.pIterCurrent = gc.pIterCurrent->pNext;
				}
			}

			if(!gc.pIterCurrent)
			{
				// We have reached the end of the phase once all objects in the list have been checked.
				endOfPhase = true;
			}

			break;
		}

		// Dispose of any objects that are no longer in use.
		case XENON_GC_PHASE_DISPOSE:
		{
			// Iterate over the unmarked items to free, up to as many as we're allowed in one run.
			for(uint32_t index = 0; index < gc.maxIterationCount; ++index)
			{
				if(!gc.pUnmarkedHead)
				{
					// The end of the list has been reached.
					break;
				}

				// Cache the next proxy in case the current one is freed from memory when disposed.
				XenonGcProxy* const pNext = gc.pUnmarkedHead->pNext;

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
		gc.phase = (gc.phase + 1) % XENON_GC_PHASE__COUNT;

		// The end of all phases is triggered when we have looped back to the first phase.
		endOfAllPhases = (gc.phase == XENON_GC_PHASE__START);
	}

	return endOfAllPhases;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::RunFull(XenonGarbageCollector& gc)
{
	// Reset the garbage collector state so that running it again starts at the beginning of the 1st phase.
	prv_reset(gc);

	while(!RunStep(gc))
	{
		// Run the garbage collector until all phases have been run.
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::LinkObject(XenonGarbageCollector& gc, XenonGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);

	XenonScopedMutex lock(gc.pendingLock);

	// Link the proxy the head of the pending list.
	pGcProxy->pending = true;

	if(gc.pPendingHead)
	{
		prv_proxyInsertBefore(gc.pPendingHead, pGcProxy);
	}

	gc.pPendingHead = pGcProxy;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::MarkObject(XenonGarbageCollector& gc, XenonGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);

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

void XenonGarbageCollector::prv_reset(XenonGarbageCollector& gc)
{
	if(gc.pMarkedTail && gc.pUnmarkedHead)
	{
		// Link the head of the unmarked objects to the end of the marked list.
		prv_proxyInsertAfter(gc.pMarkedTail, gc.pUnmarkedHead);
	}

	if(gc.pMarkedHead)
	{
		// The marked list now becomes the unmarked list.
		gc.pUnmarkedHead = gc.pMarkedHead;
	}

	gc.pMarkedHead = nullptr;
	gc.pMarkedTail = nullptr;

	gc.phase = XENON_GC_PHASE__START;
	gc.lastPhase = XENON_GC_PHASE__END;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::prv_onDisposeObject(XenonGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);

	pGcProxy->onGcDisposeFn(pGcProxy->pObject);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::prv_proxyInsertBefore(XenonGcProxy* const pGcListProxy, XenonGcProxy* const pGcInsertProxy)
{
	assert(pGcListProxy != nullptr);
	assert(pGcInsertProxy != nullptr);

	XenonGcProxy* const pListPrev = pGcListProxy->pPrev;

	if(pListPrev)
	{
		pListPrev->pNext = pGcInsertProxy;
	}

	pGcListProxy->pPrev = pGcInsertProxy;

	pGcInsertProxy->pPrev = pListPrev;
	pGcInsertProxy->pNext = pGcListProxy;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::prv_proxyInsertAfter(XenonGcProxy* const pGcListProxy, XenonGcProxy* const pGcInsertProxy)
{
	assert(pGcListProxy != nullptr);
	assert(pGcInsertProxy != nullptr);

	XenonGcProxy* const pListNext = pGcListProxy->pNext;

	if(pListNext)
	{
		pListNext->pPrev = pGcInsertProxy;
	}

	pGcListProxy->pNext = pGcInsertProxy;

	pGcInsertProxy->pPrev = pGcListProxy;
	pGcInsertProxy->pNext = pListNext;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::prv_proxyUnlink(XenonGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);

	XenonGcProxy* const pPrev = pGcProxy->pPrev;
	XenonGcProxy* const pNext = pGcProxy->pNext;

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
