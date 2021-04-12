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
#include "Vm.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

enum XenonGcPhase
{
	XENON_GC_PHASE_LINK_PENDING,
	XENON_GC_PHASE_RESET_STATE,
	XENON_GC_PHASE_MARK_CONSTANTS,
	XENON_GC_PHASE_MARK_GLOBALS,
	XENON_GC_PHASE_MARK_FUNCTIONS,
	XENON_GC_PHASE_MARK_EXECUTIONS,
	XENON_GC_PHASE_COLLECT,
	XENON_GC_PHASE_DISPOSE,

	XENON_GC_PHASE__COUNT,
	XENON_GC_PHASE__START = XENON_GC_PHASE_LINK_PENDING,
	XENON_GC_PHASE__END = XENON_GC_PHASE_DISPOSE,
};

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::Initialize(XenonGarbageCollector& output, XenonVmHandle hVm)
{
	// TODO: Make these values configurable.
	constexpr const uint32_t maxIterationCount = 32;
	constexpr const uint64_t initialExecStackSize = 8;
	constexpr const uint64_t initialValueStackSize = 4096;

	XenonProgram::HandleStack::Initialize(output.programStack, hVm->programs.Size());
	XenonFunction::HandleStack::Initialize(output.functionStack, hVm->functions.Size());
	XenonExecution::HandleStack::Initialize(output.execStack, initialExecStackSize);
	XenonValue::HandleStack::Initialize(output.valueStack, initialValueStackSize);

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
	// Clear out any reference counted data.
	Reset(gc);

	// Dispose of all proxies in the active list.
	XenonGcProxy* pCurrent = gc.pActiveHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		XenonGcProxy::prv_onDispose(*pCurrent);

		pCurrent = pNext;
	}

	// Dispose of all proxies in the pending list.
	pCurrent = gc.pPendingHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		XenonGcProxy::prv_onDispose(*pCurrent);

		pCurrent = pNext;
	}

	// Dispose of all proxies in the unmarked list;
	pCurrent = gc.pUnmarkedHead;
	while(pCurrent)
	{
		XenonGcProxy* const pNext = pCurrent->pNext;

		XenonGcProxy::prv_onDispose(*pCurrent);

		pCurrent = pNext;
	}

	XenonValue::HandleStack::Dispose(gc.valueStack);
	XenonExecution::HandleStack::Dispose(gc.execStack);
	XenonFunction::HandleStack::Dispose(gc.functionStack);
	XenonProgram::HandleStack::Dispose(gc.programStack);

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
	gc.phase = XENON_GC_PHASE__START;
	gc.lastPhase = XENON_GC_PHASE__END;

	prv_clearStacks(gc);
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

				prv_clearStacks(gc);
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

		case XENON_GC_PHASE_MARK_CONSTANTS:
		{
			if(gc.lastPhase != gc.phase)
			{
				// Start the phase by filling up the program stack.
				for(auto& kv : gc.hVm->programs)
				{
					XenonProgram::HandleStack::Push(gc.programStack, kv.value);
				}
			}
			else
			{
				// When there are no values left in the stack, pop a program and fill the value stack
				// up with that program's constants.
				if(XenonValue::HandleStack::IsEmpty(gc.valueStack))
				{
					XenonProgramHandle hProgram;
					if(XenonProgram::HandleStack::Pop(gc.programStack, &hProgram) == XENON_ERROR_STACK_EMPTY)
					{
						// No more programs in the stack, time to move on to the next phase.
						endOfPhase = true;
						break;
					}

					// Cache the constant values from the program.
					for(size_t i = 0; i < hProgram->constants.count; ++i)
					{
						XenonValue::HandleStack::Push(gc.valueStack, hProgram->constants.pData[i]);
					}
				}
				else
				{
					// Iterate over as many values as we are allowed.
					for(uint32_t i = 0; i < gc.maxIterationCount; ++i)
					{
						XenonValueHandle hValue;
						if(XenonValue::HandleStack::Pop(gc.valueStack, &hValue) == XENON_ERROR_STACK_EMPTY)
						{
							// Stop iterating once the value stack is empty.
							break;
						}

						// TODO: Mark the value.
					}
				}
			}
			break;
		}

		case XENON_GC_PHASE_MARK_GLOBALS:
		{
			if(gc.lastPhase != gc.phase)
			{
				// Fill the value stack with all the globals in the VM.
				for(auto& kv : gc.hVm->globals)
				{
					XenonValue::HandleStack::Push(gc.valueStack, kv.value);
				}
			}
			else
			{
				for(uint32_t i = 0; i < gc.maxIterationCount; ++i)
				{
					XenonValueHandle hValue;
					if(XenonValue::HandleStack::Pop(gc.valueStack, &hValue) == XENON_ERROR_STACK_EMPTY)
					{
						// Stop iterating once the value stack is empty.
						endOfPhase = true;
						break;
					}

					// TODO: Mark the value;
				}
			}
			break;
		}

		case XENON_GC_PHASE_MARK_FUNCTIONS:
		{
			if(gc.lastPhase != gc.phase)
			{
				// Start the phase by filling the function stack.
				for(auto& kv : gc.hVm->functions)
				{
					XenonFunction::HandleStack::Push(gc.functionStack, kv.value);
				}
			}
			else
			{
				// When there are no values left in the stack, pop a function and fill the value stack
				// up with that function's local variable values.
				if(XenonValue::HandleStack::IsEmpty(gc.valueStack))
				{
					XenonFunctionHandle hFunction;
					if(XenonFunction::HandleStack::Pop(gc.functionStack, &hFunction) == XENON_ERROR_STACK_EMPTY)
					{
						// No more functions in the stack, time to move on to the next phase.
						endOfPhase = true;
						break;
					}

					// Cache the locals from the function.
					for(auto& kv : hFunction->locals)
					{
						XenonValue::HandleStack::Push(gc.valueStack, kv.value);
					}
				}
				else
				{
					// Iterate over as many values as we are allowed.
					for(uint32_t i = 0; i < gc.maxIterationCount; ++i)
					{
						XenonValueHandle hValue;
						if(XenonValue::HandleStack::Pop(gc.valueStack, &hValue) == XENON_ERROR_STACK_EMPTY)
						{
							// Stop iterating once the value stack is empty.
							break;
						}

						// TODO: Mark the value.
					}
				}
			}
			break;
		}

		case XENON_GC_PHASE_MARK_EXECUTIONS:
		{
			if(gc.lastPhase != gc.phase)
			{
				// At the start of the phase, we collect all the execution contexts saving
				// a reference to them to keep them alive in case the user releases any of
				// them before they're all processed here.
				for(auto& kv : gc.hVm->executionContexts)
				{
					XenonExecution::AddRef(kv.key);
					XenonExecution::HandleStack::Push(gc.execStack, kv.key);
				}
			}
			else
			{
				// Pop the execution contexts one-by-one until none are left.
				XenonExecutionHandle hExec = XENON_EXECUTION_HANDLE_NULL;
				XenonExecution::HandleStack::Pop(gc.execStack, &hExec);

				// TODO: Mark gc data in the execution context here.
				XenonExecution::Release(hExec);
			}

			if(XenonExecution::HandleStack::IsEmpty(gc.execStack))
			{
				// When the execution context stack is empty, we can move on to the next phase.
				endOfPhase = true;
			}
			break;
		}

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
				XenonGcProxy::prv_onDispose(*gc.pUnmarkedHead);

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
		gc.phase = (gc.phase + 1) % XENON_GC_PHASE__COUNT;

		endOfAllPhases = (gc.phase == XENON_GC_PHASE__START);
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

void XenonGarbageCollector::prv_clearStacks(XenonGarbageCollector& gc)
{
	// Release any held execution contexts.
	XenonExecutionHandle hExec;
	while(XenonExecution::HandleStack::Pop(gc.execStack, &hExec) == XENON_SUCCESS)
	{
		XenonExecution::Release(hExec);
	}

	// Clear the program stack.
	while(XenonProgram::HandleStack::Pop(gc.programStack) == XENON_SUCCESS)
	{
	}

	// Clear the function stack.
	while(XenonFunction::HandleStack::Pop(gc.functionStack) == XENON_SUCCESS)
	{
	}

	// Clear the value stack.
	while(XenonValue::HandleStack::Pop(gc.valueStack) == XENON_SUCCESS)
	{
	}
}

//----------------------------------------------------------------------------------------------------------------------
