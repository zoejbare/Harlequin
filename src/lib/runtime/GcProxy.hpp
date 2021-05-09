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

#include "../XenonScript.h"

#include "../common/DestructCallback.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct XenonGarbageCollector;

typedef void (*XenonGcDiscoveryCallback)(XenonGarbageCollector&, void*);

//----------------------------------------------------------------------------------------------------------------------

struct XenonGcProxy
{
	static void Initialize(
		XenonGcProxy& output,
		XenonGarbageCollector& gc,
		XenonGcDiscoveryCallback onGcDiscoveryFn,
		XenonDestructCallback onGcDestructFn,
		void* const pObject,
		const bool autoMark
	);

	static void InsertBefore(XenonGcProxy* const pGcListProxy, XenonGcProxy* const pGcInsertProxy);
	static void InsertAfter(XenonGcProxy* const pGcListProxy, XenonGcProxy* const pGcInsertProxy);
	static void Unlink(XenonGcProxy* const pGcProxy);

	XenonGcDiscoveryCallback onGcDiscoveryFn;
	XenonDestructCallback onGcDestructFn;

	XenonGarbageCollector* pGc;

	XenonGcProxy* pPrev;
	XenonGcProxy* pNext;

	void* pObject;

	bool pending;
	bool marked;
	bool autoMark;
};

//----------------------------------------------------------------------------------------------------------------------
