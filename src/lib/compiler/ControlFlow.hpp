//
// Copyright (c) 2023, Zoe J. Bare
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

#include "../common/Array.hpp"

//----------------------------------------------------------------------------------------------------------------------

class HQ_MAIN_API ControlFlow
{
public:

	enum class Behavior
	{
		If,
		While,
		DoWhile,
	};

	enum class Condition
	{
		None,
		True,
		False,
	};

	ControlFlow(const ControlFlow&) = delete;
	ControlFlow(ControlFlow&&) = delete;

	ControlFlow& operator =(const ControlFlow&) = delete;
	ControlFlow& operator =(ControlFlow&&) = delete;

	ControlFlow();
	~ControlFlow();

	void Begin(HqSerializerHandle hSerializer, Behavior behavior, Condition condition, uint32_t gpRegIndex);
	void End();

	void GoToStart();
	void GoToEnd();


private:

	enum class JmpType
	{
		Normal,
		Condition,
	};

	enum class JmpLoc
	{
		Start,
		End,
	};

	struct Command
	{
		size_t offset;
		JmpType type;
		JmpLoc loc;
	};

	void _writeJump(int32_t, JmpType);
	void _addCommand(JmpType, JmpLoc);

	typedef HqArray<Command> CommandList;

	CommandList m_cmds;

	HqSerializerHandle m_hSerializer;
	size_t m_offStart;
	size_t m_offEnd;
	uint32_t m_reg;
	Behavior m_beh;
	Condition m_cond;
};

//----------------------------------------------------------------------------------------------------------------------

inline ControlFlow::ControlFlow()
	: m_cmds()
	, m_hSerializer(HQ_SERIALIZER_HANDLE_NULL)
	, m_offStart(0)
	, m_offEnd(0)
	, m_reg(0)
	, m_beh(Behavior::If)
	, m_cond(Condition::None)
{
	CommandList::Initialize(m_cmds);
}

//----------------------------------------------------------------------------------------------------------------------

inline ControlFlow::~ControlFlow()
{
	CommandList::Dispose(m_cmds);
}

//----------------------------------------------------------------------------------------------------------------------
