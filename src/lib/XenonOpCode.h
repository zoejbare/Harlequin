/**
 * Copyright (c) 2021, Zoe J. Bare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _XENON_OP_CODE_H_
#define _XENON_OP_CODE_H_

/*---------------------------------------------------------------------------------------------------------------------*/

enum XenonOpCode
{
	XENON_OP_CODE_NOP,
	XENON_OP_CODE_RETURN,
	XENON_OP_CODE_YIELD,
	XENON_OP_CODE_CALL,

	XENON_OP_CODE_LOAD_CONSTANT,
	XENON_OP_CODE_LOAD_GLOBAL,
	XENON_OP_CODE_LOAD_LOCAL,
	XENON_OP_CODE_LOAD_PARAM,

	XENON_OP_CODE_STORE_GLOBAL,
	XENON_OP_CODE_STORE_LOCAL,
	XENON_OP_CODE_STORE_PARAM,

	XENON_OP_CODE_PUSH,
	XENON_OP_CODE_POP,

	XENON_OP_CODE__TOTAL_COUNT,
	XENON_OP_CODE__FOCE_DWORD = 0x7FFFFFFFul,
};

/*---------------------------------------------------------------------------------------------------------------------*/

#endif /* _XENON_OP_CODE_H_ */
