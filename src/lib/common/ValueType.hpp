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

enum XenonValueType
{
	XENON_VALUE_TYPE_NULL,

	XENON_VALUE_TYPE_INT8,
	XENON_VALUE_TYPE_INT16,
	XENON_VALUE_TYPE_INT32,
	XENON_VALUE_TYPE_INT64,

	XENON_VALUE_TYPE_UINT8,
	XENON_VALUE_TYPE_UINT16,
	XENON_VALUE_TYPE_UINT32,
	XENON_VALUE_TYPE_UINT64,

	XENON_VALUE_TYPE_FLOAT32,
	XENON_VALUE_TYPE_FLOAT64,

	XENON_VALUE_TYPE_BOOL,

	XENON_VALUE_TYPE_STRING,
	XENON_VALUE_TYPE_OBJECT,

	XENON_VALUE_TYPE__MAX_VALUE = XENON_VALUE_TYPE_OBJECT,
};

//----------------------------------------------------------------------------------------------------------------------
