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
	XENON_GC_PHASE_MARK_AUTO,
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

void XenonGarbageCollector::Initialize(XenonGarbageCollector& output, XenonVmHandle hVm, const uint32_t maxIterationCount)
{
	constexpr const size_t initialProgramStackSize = 256;
	constexpr const size_t initialFunctionStackSize = 2048;
	constexpr const size_t initialExecStackSize = 64;
	constexpr const size_t initialValueStackSize = 4096;

	XenonProgram::HandleStack::Initialize(output.programStack, initialProgramStackSize);
	XenonFunction::HandleStack::Initialize(output.functionStack, initialFunctionStackSize);
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
	prv_reset(output);
}

//----------------------------------------------------------------------------------------------------------------------

void XenonGarbageCollector::Dispose(XenonGarbageCollector& gc)
{
	// Clear out any reference counted data.
	prv_reset(gc);

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

bool XenonGarbageCollector::RunStep(XenonGarbageCollector& gc)
{
	bool endOfAllPhases = false;
	bool endOfPhase = false;

	switch(gc.phase)
	{
		case XENON_GC_PHASE_LINK_PENDING:
		{
			if(gc.lastPhase != gc.phase)
			{
				// The very first thing to be done is to clear any existing data held in the stacks.
				prv_clearStacks(gc);
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

		case XENON_GC_PHASE_MARK_AUTO:
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

					// Any proxies set to auto-mark will be marked here.
					if(gc.pIterCurrent->autoMark)
					{
						index += XenonGcProxy::Mark(*gc.pIterCurrent);
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

		case XENON_GC_PHASE_MARK_GLOBALS:
		{
			if(gc.lastPhase != gc.phase)
			{
				const size_t requiredStackSize = gc.hVm->globals.Size();
				const size_t currentStackSize = gc.valueStack.memory.count;

				// Resize the stack if it's too small.
				if(requiredStackSize > currentStackSize)
				{
					XenonValue::HandleStack::Dispose(gc.valueStack);
					XenonValue::HandleStack::Initialize(gc.valueStack, requiredStackSize * 3 / 2);
				}

				// Fill the value stack with all the globals in the VM.
				for(auto& kv : gc.hVm->globals)
				{
					XenonValue::HandleStack::Push(gc.valueStack, kv.value);
				}
			}
			else
			{
				for(uint32_t i = 0; i < gc.maxIterationCount;)
				{
					XenonValueHandle hValue;
					if(XenonValue::HandleStack::Pop(gc.valueStack, &hValue) == XENON_ERROR_STACK_EMPTY)
					{
						// Stop iterating once the value stack is empty.
						endOfPhase = true;
						break;
					}

					i += XenonValue::Mark(hValue);
				}
			}
			break;
		}

		case XENON_GC_PHASE_MARK_FUNCTIONS:
		{
			if(gc.lastPhase != gc.phase)
			{
				const size_t requiredStackSize = gc.hVm->functions.Size();
				const size_t currentStackSize = gc.functionStack.memory.count;

				// Resize the stack if it's too small.
				if(requiredStackSize > currentStackSize)
				{
					XenonFunction::HandleStack::Dispose(gc.functionStack);
					XenonFunction::HandleStack::Initialize(gc.functionStack, requiredStackSize * 3 / 2);
				}

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

					const size_t requiredStackSize = hFunction->locals.Size();
					const size_t currentStackSize = gc.valueStack.memory.count;

					// Resize the stack if it's too small.
					if(requiredStackSize > currentStackSize)
					{
						XenonValue::HandleStack::Dispose(gc.valueStack);
						XenonValue::HandleStack::Initialize(gc.valueStack, requiredStackSize * 3 / 2);
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
					for(uint32_t i = 0; i < gc.maxIterationCount;)
					{
						XenonValueHandle hValue;
						if(XenonValue::HandleStack::Pop(gc.valueStack, &hValue) == XENON_ERROR_STACK_EMPTY)
						{
							// Stop iterating once the value stack is empty.
							break;
						}

						i += XenonValue::Mark(hValue);
					}
				}
			}
			break;
		}

		case XENON_GC_PHASE_MARK_EXECUTIONS:
		{
			if(gc.lastPhase != gc.phase)
			{
				const size_t requiredStackSize = gc.hVm->executionContexts.Size();
				const size_t currentStackSize = gc.valueStack.memory.count;

				// Resize the stack if it's too small.
				if(requiredStackSize > currentStackSize)
				{
					XenonValue::HandleStack::Dispose(gc.valueStack);
					XenonValue::HandleStack::Initialize(gc.valueStack, requiredStackSize * 3 / 2);
				}

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
				// When there are no values left in the stack, pop a function and fill the value stack
				// up with that function's local variable values.
				if(XenonValue::HandleStack::IsEmpty(gc.valueStack))
				{
					// Pop the execution contexts one-by-one until none are left.
					XenonExecutionHandle hExec;
					if(XenonExecution::HandleStack::Pop(gc.execStack, &hExec) == XENON_ERROR_STACK_EMPTY)
					{
						// No more functions in the stack, time to move on to the next phase.
						endOfPhase = true;
						break;
					}

					XenonExecution::Release(hExec);

					// Calculate the total size we need in the value stack.
					size_t requiredStackSize = XENON_VM_IO_REGISTER_COUNT;
					for(size_t i = 0; i < hExec->frameStack.nextIndex; ++i)
					{
						XenonFrameHandle hFrame = hExec->frameStack.memory.pData[i];

						requiredStackSize += XENON_VM_GP_REGISTER_COUNT
							+ hFrame->stack.nextIndex
							+ hFrame->locals.Size();
					}

					const size_t currenStackSize = gc.valueStack.memory.count;

					// Resize the stack if it's too small.
					if(requiredStackSize > currenStackSize)
					{
						XenonValue::HandleStack::Dispose(gc.valueStack);
						XenonValue::HandleStack::Initialize(gc.valueStack, requiredStackSize * 3 / 2);
					}

					// Push each of the I/O registers to the value stack.
					for(size_t i = 0; i < XENON_VM_IO_REGISTER_COUNT; ++i)
					{
						XenonValueHandle hValue = hExec->registers.pData[i];

						XenonValue::HandleStack::Push(gc.valueStack, hValue);
					}

					// Iterate over the entire frame stack in the execution context.
					for(size_t frameIndex = 0; frameIndex < hExec->frameStack.nextIndex; ++frameIndex)
					{
						XenonFrameHandle hFrame = hExec->frameStack.memory.pData[frameIndex];

						// Push each of the frame's general purpose registers to the value stack.
						for(size_t registerIndex = 0; registerIndex < XENON_VM_GP_REGISTER_COUNT; ++registerIndex)
						{
							XenonValueHandle hValue = hFrame->registers.pData[registerIndex];

							XenonValue::HandleStack::Push(gc.valueStack, hValue);
						}

						// Push all values current in the frame's value stack to our value stack.
						for(size_t stackIndex = 0; stackIndex < hFrame->stack.nextIndex; ++stackIndex)
						{
							XenonValueHandle hValue = hFrame->stack.memory.pData[stackIndex];

							XenonValue::HandleStack::Push(gc.valueStack, hValue);
						}

						// Push each of the frame's local variables to the value stack.
						for(auto& kv : hFrame->locals)
						{
							XenonValue::HandleStack::Push(gc.valueStack, kv.value);
						}
					}
				}
				else
				{
					// Iterate over as many values as we are allowed.
					for(uint32_t i = 0; i < gc.maxIterationCount;)
					{
						XenonValueHandle hValue;
						if(XenonValue::HandleStack::Pop(gc.valueStack, &hValue) == XENON_ERROR_STACK_EMPTY)
						{
							// Stop iterating once the value stack is empty.
							break;
						}

						i += XenonValue::Mark(hValue);
					}
				}
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

	gc.lastPhase = gc.phase;

	if(endOfPhase)
	{
		gc.phase = (gc.phase + 1) % XENON_GC_PHASE__COUNT;

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

void XenonGarbageCollector::prv_reset(XenonGarbageCollector& gc)
{
	gc.phase = XENON_GC_PHASE__START;
	gc.lastPhase = XENON_GC_PHASE__END;

	prv_clearStacks(gc);
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
