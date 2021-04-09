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

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

enum XenonGcPhase
{
	XENON_GC_PHASE_LINK_PENDING,
	XENON_GC_PHASE_RESET_STATE,
	XENON_GC_PHASE_MARK_CONSTANTS,
	XENON_GC_PHASE_MARK_EXECUTIONS,
	XENON_GC_PHASE_COLLECT,
	XENON_GC_PHASE_DISPOSE,

	__XENON_GC_PHASE_COUNT,
	__XENON_GC_PHASE_START = XENON_GC_PHASE_LINK_PENDING,
	__XENON_GC_PHASE_END = XENON_GC_PHASE_DISPOSE,
};

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::Initialize(XenonGarbageCollector& output, XenonVmHandle hVm)
{
	// TODO: Make these values configurable.
	constexpr const uint32_t maxIterationCount = 32;

	output.hVm = hVm;
	output.pActiveHead = nullptr;
	output.pPendingHead = nullptr;
	output.pIterCurrent = nullptr;
	output.pIterPrev = nullptr;
	output.phase = 0;
	output.lastPhase = 0;
	output.maxIterationCount = maxIterationCount;

	// Reset the phases so we're guaranteed to kick off the garbage collector in a good state.
	Reset(output);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::Dispose(XenonGarbageCollector& gc)
{
	// Dispose of all proxies in the active list.
	XenonGcProxy* pCurrent = gc.pActiveHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		XenonGcProxy::Dispose(*pCurrent);

		pCurrent = pNext;
	}

	// Dispose of all proxies in the pending list.
	pCurrent = gc.pPendingHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		XenonGcProxy::Dispose(*pCurrent);

		pCurrent = pNext;
	}

	// Dispose of all proxies in the unmarked list;
	pCurrent = gc.pUnmarkedHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		XenonGcProxy::Dispose(*pCurrent);

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

void XenonGarbageCollector::Reset(XenonGarbageCollector& gc)
{
	gc.phase = __XENON_GC_PHASE_START;
	gc.lastPhase = __XENON_GC_PHASE_END;
}

//----------------------------------------------------------------------------------------------------------------------

bool XenonGarbageCollector::Run(XenonGarbageCollector& gc)
{
	bool endOfAllPhases = false;
	bool endOfPhase = false;

	switch(gc.phase)
	{
		case XENON_GC_PHASE_LINK_PENDING:
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

				// Update the heads of the active and pending lists.
				gc.pActiveHead = pCurrent;
				gc.pPendingHead = pNext;
			}

			if(!gc.pPendingHead)
			{
				// We have reached the end of the phase when there are no more proxies in the pending list.
				endOfPhase = true;
			}
			break;
		}

		case XENON_GC_PHASE_RESET_STATE:
		{
			if(gc.lastPhase != gc.phase)
			{
				// For the start of the phase, set the current proxy pointer to the head of the active list.
				gc.pIterCurrent = gc.pActiveHead;
			}

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

			if(!gc.pIterCurrent)
			{
				// We have reached the end of the phase once all proxies in the list have been checked.
				endOfPhase = true;
			}
			break;
		}

		case XENON_GC_PHASE_MARK_CONSTANTS:
			// TODO: Mark all constants in all loaded programs in the VM.
			endOfPhase = true;
			break;

		case XENON_GC_PHASE_MARK_EXECUTIONS:
			// TODO: Mark all tracked data contained in the VM's execution contexts.
			endOfPhase = true;
			break;

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
					// We've reached the end of the list, no need to check anything else.
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

		case XENON_GC_PHASE_DISPOSE:
		{
			// Iterate over the unmarked items to free, up to as many as we're allowed in one run.
			for(uint32_t index = 0; index < gc.maxIterationCount; ++index)
			{
				if(!gc.pUnmarkedHead)
				{
					break;
				}

				// Cache the next proxy in case the current one is freed from memory when disposed.
				XenonGcProxy* const pNext = gc.pUnmarkedHead->pNext;

				// Dispose of the current proxy.
				XenonGcProxy::Dispose(*gc.pUnmarkedHead);

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

	if(endOfPhase)
	{
		gc.lastPhase = gc.phase;
		gc.phase = (gc.phase + 1) % __XENON_GC_PHASE_COUNT;

		endOfAllPhases = (gc.phase == __XENON_GC_PHASE_START);
	}

	return endOfAllPhases;
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
