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

//----------------------------------------------------------------------------------------------------------------------

#define XENON_DECLARE_BUILT_IN(name) static void name(XenonExecutionHandle, XenonFunctionHandle, void*)

//----------------------------------------------------------------------------------------------------------------------

struct XENON_MAIN_API XenonBuiltIn
{
	XENON_DECLARE_BUILT_IN(OpAddBool);
	XENON_DECLARE_BUILT_IN(OpAddInt8);
	XENON_DECLARE_BUILT_IN(OpAddInt16);
	XENON_DECLARE_BUILT_IN(OpAddInt32);
	XENON_DECLARE_BUILT_IN(OpAddInt64);
	XENON_DECLARE_BUILT_IN(OpAddUint8);
	XENON_DECLARE_BUILT_IN(OpAddUint16);
	XENON_DECLARE_BUILT_IN(OpAddUint32);
	XENON_DECLARE_BUILT_IN(OpAddUint64);
	XENON_DECLARE_BUILT_IN(OpAddFloat32);
	XENON_DECLARE_BUILT_IN(OpAddFloat64);
	XENON_DECLARE_BUILT_IN(OpAddString);

	XENON_DECLARE_BUILT_IN(OpSubBool);
	XENON_DECLARE_BUILT_IN(OpSubInt8);
	XENON_DECLARE_BUILT_IN(OpSubInt16);
	XENON_DECLARE_BUILT_IN(OpSubInt32);
	XENON_DECLARE_BUILT_IN(OpSubInt64);
	XENON_DECLARE_BUILT_IN(OpSubUint8);
	XENON_DECLARE_BUILT_IN(OpSubUint16);
	XENON_DECLARE_BUILT_IN(OpSubUint32);
	XENON_DECLARE_BUILT_IN(OpSubUint64);
	XENON_DECLARE_BUILT_IN(OpSubFloat32);
	XENON_DECLARE_BUILT_IN(OpSubFloat64);

	XENON_DECLARE_BUILT_IN(OpMulBool);
	XENON_DECLARE_BUILT_IN(OpMulInt8);
	XENON_DECLARE_BUILT_IN(OpMulInt16);
	XENON_DECLARE_BUILT_IN(OpMulInt32);
	XENON_DECLARE_BUILT_IN(OpMulInt64);
	XENON_DECLARE_BUILT_IN(OpMulUint8);
	XENON_DECLARE_BUILT_IN(OpMulUint16);
	XENON_DECLARE_BUILT_IN(OpMulUint32);
	XENON_DECLARE_BUILT_IN(OpMulUint64);
	XENON_DECLARE_BUILT_IN(OpMulFloat32);
	XENON_DECLARE_BUILT_IN(OpMulFloat64);

	XENON_DECLARE_BUILT_IN(OpDivBool);
	XENON_DECLARE_BUILT_IN(OpDivInt8);
	XENON_DECLARE_BUILT_IN(OpDivInt16);
	XENON_DECLARE_BUILT_IN(OpDivInt32);
	XENON_DECLARE_BUILT_IN(OpDivInt64);
	XENON_DECLARE_BUILT_IN(OpDivUint8);
	XENON_DECLARE_BUILT_IN(OpDivUint16);
	XENON_DECLARE_BUILT_IN(OpDivUint32);
	XENON_DECLARE_BUILT_IN(OpDivUint64);
	XENON_DECLARE_BUILT_IN(OpDivFloat32);
	XENON_DECLARE_BUILT_IN(OpDivFloat64);
};

//----------------------------------------------------------------------------------------------------------------------

#undef XENON_DECLARE_BUILT_IN

//----------------------------------------------------------------------------------------------------------------------
