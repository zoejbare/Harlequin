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
	XENON_GC_PHASE_MARK_ACTIVE,
	XENON_GC_PHASE_COLLECT,
	XENON_GC_PHASE_DISPOSE,

	XENON_GC_PHASE__COUNT,
	XENON_GC_PHASE__START = XENON_GC_PHASE_LINK_PENDING,
	XENON_GC_PHASE__END = XENON_GC_PHASE_DISPOSE,
};

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::Initialize(XenonGarbageCollector& output, XenonVmHandle hVm, const uint32_t maxIterationCount)
{
	output.hVm = hVm;
	output.pActiveHead = nullptr;
	output.pPendingHead = nullptr;
	output.pIterCurrent = nullptr;
	output.pIterPrev = nullptr;
	output.phase = 0;
	output.lastPhase = 0;
	output.maxIterationCount = maxIterationCount;

	// Initialize the queue read ticket, which will improve performance when dequeuing.
	output.proxyQueue.InitializeReservationTicket(output.proxyReadTicket);

	// Reset the phases so we're guaranteed to kick off the garbage collector in a good state.
	prv_reset(output);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::Dispose(XenonGarbageCollector& gc)
{
	// Reset the GC state to release any held data.
	prv_reset(gc);

	// Dispose of all proxies in the active list.
	XenonGcProxy* pCurrent = gc.pActiveHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		prv_onDisposeProxy(pCurrent);

		pCurrent = pNext;
	}

	// Dispose of all proxies in the pending list.
	pCurrent = gc.pPendingHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		prv_onDisposeProxy(pCurrent);

		pCurrent = pNext;
	}

	// Dispose of all proxies in the unmarked list;
	pCurrent = gc.pUnmarkedHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		prv_onDisposeProxy(pCurrent);

		pCurrent = pNext;
	}

	gc.hVm = XENON_VM_HANDLE_NULL;
	gc.pActiveHead = nullptr;
	gc.pPendingHead = nullptr;
	gc.pUnmarkedHead = nullptr;
	gc.pIterCurrent = nullptr;
	gc.pIterPrev = nullptr;
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
			if(gc.lastPhase != gc.phase)
			{
				// The very first thing to be done is to clear any existing data held in the proxy queue.
				prv_clearQueue(gc);
			}
			else
			{
				for(uint32_t index = 0; index < gc.maxIterationCount; ++index)
				{
					if(!gc.pPendingHead)
					{
						// The end of the list has been reached.
						break;
					}

					XenonGcProxy* const pCurrent = gc.pPendingHead;
					XenonGcProxy* const pNext = pCurrent->pNext;

					// Link the proxy at the head of the pending list to the head of the active list.
					pCurrent->pNext = gc.pActiveHead;

					// Update the proxy to clear its 'pending' state.
					pCurrent->pending = false;

					// Update the heads of the active and pending lists.
					gc.pActiveHead = pCurrent;
					gc.pPendingHead = pNext;
				}
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
				gc.pIterCurrent = gc.pActiveHead;
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
				gc.pIterCurrent = gc.pActiveHead;
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

					// Add any auto-mark proxies to the queue so they and their sub-proxies
					// will be marked prior to the collection phase.
					if(gc.pIterCurrent->autoMark)
					{
						gc.proxyQueue.Enqueue(gc.pIterCurrent);
					}

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
				if(XenonValue::CanBeMarked(kv.value))
				{
					gc.proxyQueue.Enqueue(&kv.value->gcProxy);
				}
			}

			endOfPhase = true;
			break;
		}

		// Mark all active proxies.
		case XENON_GC_PHASE_MARK_ACTIVE:
		{
			uint32_t remainingIterations = gc.maxIterationCount;

#if 1
			XenonGcProxy::BatchedPtrQueue::BatchDequeueList proxyBatch;

			// Process as proxies up to the maximum number we're allowed.
			while(remainingIterations > 0)
			{
				// Pull out as many items from the queue as we can.
				gc.proxyQueue.DequeueBatch(proxyBatch, remainingIterations);

				uint32_t numProcessed = 0;

				// Process each dequeued proxy in the batch.
				while(proxyBatch.More())
				{
					XenonGcProxy* const pGcProxy = proxyBatch.Next();

					// Only mark and discover for proxies that have not already been marked.
					if(!pGcProxy->marked)
					{
						pGcProxy->marked = true;

						// Discover any sub-proxies that would be owned by the current proxy.
						pGcProxy->onGcDiscoveryFn(gc, pGcProxy->pObject);

						++numProcessed;
					}
				}

				if(numProcessed == 0)
				{
					// When there are no items left in the queue, we end the phase.
					endOfPhase = true;
					break;
				}

				remainingIterations -= numProcessed;
			}

#else
			while(remainingIterations > 0)
			{
				XenonGcProxy* pGcProxy;
				if(!gc.proxyQueue.Dequeue(pGcProxy, gc.proxyReadTicket))
				{
					endOfPhase = true;
					break;
				}

				if(!pGcProxy->marked)
				{
					pGcProxy->marked = true;

					pGcProxy->onGcDiscoveryFn(gc, pGcProxy->pObject);

					--remainingIterations;
				}
			}

#endif
			break;
		}

		// Look for anything in the active list that is not marked and thus needs to be collected.
		case XENON_GC_PHASE_COLLECT:
		{
			if(gc.lastPhase != gc.phase)
			{
				// For the start of the phase, setup the current and previous iteration proxy
				// pointers for traversing the active list. The previous proxy is needed in
				// case any nodes need to be unlinked from the list.
				gc.pIterCurrent = gc.pActiveHead;
				gc.pIterPrev = nullptr;
			}

			// Iterate over as many items in the list as we're allowed at one time.
			for(uint32_t index = 0; index < gc.maxIterationCount; ++index)
			{
				if(!gc.pIterCurrent)
				{
					// The end of the list has been reached.
					break;
				}

				XenonGcProxy* const pCurrent = gc.pIterCurrent;
				XenonGcProxy* const pNext = pCurrent->pNext;

				if(!pCurrent->marked)
				{
					// The current proxy is unmarked, so we need to unlink it from the active list
					// and move it to the unmarked list.
					if(pCurrent == gc.pActiveHead)
					{
						// Move the head pointer to its next node.
						gc.pActiveHead = gc.pActiveHead->pNext;
					}

					pCurrent->pNext = gc.pUnmarkedHead;

					gc.pUnmarkedHead = pCurrent;

					// The previous proxy will stay where it is, but it's next pointer needs to be updated.
					if(gc.pIterPrev)
					{
						gc.pIterPrev->pNext = pNext;
					}
				}
				else
				{
					// The current proxy will stay in the active list, so we can update the previous proxy as normal.
					gc.pIterPrev = gc.pIterCurrent;
				}

				// Move to the next proxy in the list.
				gc.pIterCurrent = pNext;
			}

			if(!gc.pIterCurrent)
			{
				// We have reached the end of the phase once all proxies in the list have been checked.
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
				prv_onDisposeProxy(gc.pUnmarkedHead);

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

void XenonGarbageCollector::LinkObject(XenonGarbageCollector& gc, XenonGcProxy& proxy)
{
	XenonGcProxy* const pProxy = &proxy;

	// Link the proxy the head of the pending list. All nodes in the pending list
	// will be transferred to the active list as the garbage collector is run.
	proxy.pNext = gc.pPendingHead;

	gc.pPendingHead = pProxy;
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::DiscoverProxy(XenonGarbageCollector& gc, XenonGcProxy& proxy)
{
	if(!proxy.marked && !proxy.pending)
	{
		gc.proxyQueue.Enqueue(&proxy);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::prv_reset(XenonGarbageCollector& gc)
{
	gc.phase = XENON_GC_PHASE__START;
	gc.lastPhase = XENON_GC_PHASE__END;

	prv_clearQueue(gc);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::prv_clearQueue(XenonGarbageCollector& gc)
{
	// Clear the proxy queue.
	XenonGcProxy* pProxy;
	while(gc.proxyQueue.Dequeue(pProxy, gc.proxyReadTicket))
	{
	}
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::prv_onDisposeProxy(XenonGcProxy* const pGcProxy)
{
	assert(pGcProxy != nullptr);

	pGcProxy->onGcDestructFn(pGcProxy->pObject);
}

//----------------------------------------------------------------------------------------------------------------------
