//
// Copyright (c) 2022, Zoe J. Bare
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

#include "../Fiber.hpp"

#include <assert.h>

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include <Windows.h>

//----------------------------------------------------------------------------------------------------------------------

struct _HqInternalFiberConfig
{
	HqInternalFiber* pObj;
	HqFiberConfig data;
};

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplCreate(HqInternalFiber& obj, const HqFiberConfig& fiberConfig)
{
	assert(obj.pFiber == nullptr);

	auto fiberEntryPoint = [](void* const pOpaqueConfig)
	{
		// Copy internal fiber config.
		_HqInternalFiberConfig config = *reinterpret_cast<_HqInternalFiberConfig*>(pOpaqueConfig);

		// Free the config's original backing memory now that we have a local copy.
		HqMemFree(pOpaqueConfig);

		// Call the fiber function.
		config.data.mainFn(config.data.pArg);

		void* const pReturnPoint = config.pObj->pReturnPoint;
		assert(pReturnPoint != nullptr);

		config.pObj->pReturnPoint = nullptr;
		config.pObj->completed = true;

		// Yield fiber execution to the context that ran it.
		SwitchToFiber(pReturnPoint);
	};

	// Make a copy of the input config data for the fiber.
	_HqInternalFiberConfig* const pConfigMem =
		reinterpret_cast<_HqInternalFiberConfig*>(HqMemAlloc(sizeof(_HqInternalFiberConfig)));

	pConfigMem->pObj = &obj;
	pConfigMem->data = fiberConfig;

	// Create the native fiber.
	obj.pFiber = CreateFiber(fiberConfig.stackSize, fiberEntryPoint, pConfigMem);
	assert(obj.pFiber != nullptr);
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplDispose(HqInternalFiber& obj)
{
	assert(obj.pFiber != nullptr);

	// Clean up the internal fiber resources.
	DeleteFiber(obj.pFiber);

	obj = HqInternalFiber();
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplRun(HqInternalFiber& obj)
{
	assert(obj.pFiber != nullptr);

	if(!obj.completed)
	{
		assert(obj.pReturnPoint == nullptr);

		// Save the point where we'll return to once the fiber yields.
		obj.pReturnPoint = ConvertThreadToFiber(nullptr);

		if(!obj.pReturnPoint)
		{
			// The return point has already been converted to a fiber.
			obj.pReturnPoint = GetCurrentFiber();
		}

		// Switch the active context to running the fiber.
		SwitchToFiber(obj.pFiber);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" void _HqFiberImplWait(HqInternalFiber& obj)
{
	assert(obj.pFiber != nullptr);

	if(!obj.completed)
	{
		assert(obj.pReturnPoint != nullptr);

		// Cache the return point, then clear it in the input container.
		void* const pReturnPoint = obj.pReturnPoint;
		obj.pReturnPoint = nullptr;

		// Yield fiber execution back to the context that ran it.
		SwitchToFiber(pReturnPoint);
	}
}

//----------------------------------------------------------------------------------------------------------------------

extern "C" bool _HqFiberImplIsComplete(HqInternalFiber& obj)
{
	assert(obj.pFiber != nullptr);

	return obj.completed;
}

//----------------------------------------------------------------------------------------------------------------------
