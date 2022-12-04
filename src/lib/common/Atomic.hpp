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

#include "../Harlequin.h"

//----------------------------------------------------------------------------------------------------------------------

#if defined(HQ_PLATFORM_WINDOWS)
	#include "atomic-impl/AtomicWin32.hpp"

#elif defined(HQ_PLATFORM_LINUX) \
	|| defined(HQ_PLATFORM_MAC_OS) \
	|| defined(HQ_PLATFORM_ANDROID) \
	|| defined(HQ_PLATFORM_PS4) \
	|| defined(HQ_PLATFORM_PS5)
	#include "atomic-impl/AtomicPosix.hpp"

#elif defined(HQ_PLATFORM_PS3)
	#include "../../../support/Harlequin-PS3/lib/common/atomic/Atomic.hpp"

#elif defined(HQ_PLATFORM_PSVITA)
	#include "../../../support/Harlequin-PSVita/lib/common/atomic/Atomic.hpp"

#else
	#error "Atomic functions not implemented for this platform"

#endif

//----------------------------------------------------------------------------------------------------------------------
