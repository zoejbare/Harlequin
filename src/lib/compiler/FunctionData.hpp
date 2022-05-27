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

#include "../base/String.hpp"

#include <vector>
#include <unordered_map>
#include <unordered_set>

//----------------------------------------------------------------------------------------------------------------------

struct XenonFunctionData
{
	typedef std::vector<uint8_t> Bytecode;

	typedef std::unordered_map<
		XenonString*,
		XenonFunctionData,
		XenonString::StlHash,
		XenonString::StlCompare
	> StringToFunctionMap;

	typedef std::unordered_map<
		XenonString*,
		uint32_t,
		XenonString::StlHash,
		XenonString::StlCompare
	> LocalVariableMap;

	struct ExceptionHandler
	{
		typedef std::unordered_map<uint32_t, ExceptionHandler> Map;
		typedef std::vector<ExceptionHandler> Vector;

		XenonString* pClassName;

		uint32_t offset;
		int type;
	};

	struct GuardedBlock
	{
		typedef std::vector<GuardedBlock> Vector;

		ExceptionHandler::Map handlers;

		uint32_t id;

		uint32_t offset;
		uint32_t length;
	};

	LocalVariableMap locals;
	GuardedBlock::Vector guardedBlocks;
	Bytecode bytecode;

	uint16_t numParameters;
	uint16_t numReturnValues;

	bool isNative;
};

//----------------------------------------------------------------------------------------------------------------------
