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

#include "Branch.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

Branch::Branch()
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

Branch::~Branch()
{
	CommandList::Dispose(m_cmds);
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::Begin(
	HqSerializerHandle hSerializer,
	const Behavior behavior, 
	const Condition condition, 
	const uint32_t gpRegIndex)
{
	assert(m_hSerializer == HQ_SERIALIZER_HANDLE_NULL);

	m_hSerializer = hSerializer;
	m_reg = gpRegIndex;
	m_beh = behavior;
	m_cond = condition;

	m_offStart = HqSerializerGetStreamPosition(m_hSerializer);

	switch(m_beh)
	{
		case Behavior::If:
		case Behavior::While:
			_addCommand(JmpType::Condition, JmpLoc::End);
			break;

		default:
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::End()
{
	assert(m_hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	switch(m_beh)
	{
		case Behavior::While:
			_addCommand(JmpType::Normal, JmpLoc::Start);
			break;

		case Behavior::DoWhile:
			_addCommand(JmpType::Condition, JmpLoc::Start);
			break;

		default:
			break;
	}

	// Save the offset at the end of the branch.
	m_offEnd = HqSerializerGetStreamPosition(m_hSerializer);

	// Process each enqueued command to fill in all the temporary JMP instructions.
	for(size_t i = 0; i < m_cmds.count; ++i)
	{
		const Command& cmd = m_cmds.pData[i];

		int32_t jumpDist = 0;

		if(cmd.loc == JmpLoc::Start)
		{
			// Calculate the distance from the current command's offset back to the beginning of the branch.
			jumpDist = -int32_t(cmd.offset - m_offStart);
		}
		else
		{
			// Calculate the distance to the end of the branch.
			jumpDist = int32_t(m_offEnd - cmd.offset);
		}

		// Go the offset of the temporary JMP instruction.
		const int setCmdOffset = HqSerializerSetStreamPosition(m_hSerializer, cmd.offset);
		assert(setCmdOffset == HQ_SUCCESS); (void) setCmdOffset;

		// Write the real JMP instruction using the calculated distance.
		_writeJump(jumpDist, cmd.type);
	}

	const int returnToEndOffset = HqSerializerSetStreamPosition(m_hSerializer, m_offEnd);
	assert(returnToEndOffset == HQ_SUCCESS); (void) returnToEndOffset;

	m_cmds.count = 0;

	m_hSerializer = HQ_SERIALIZER_HANDLE_NULL;
	m_offStart = 0;
	m_offEnd = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::GoToStart()
{
	assert(m_hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	_addCommand(JmpType::Normal, JmpLoc::Start);
}

//----------------------------------------------------------------------------------------------------------------------

void Branch::GoToEnd()
{
	assert(m_hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	_addCommand(JmpType::Normal, JmpLoc::End);
}

//----------------------------------------------------------------------------------------------------------------------

inline void Branch::_writeJump(const int32_t offset, const JmpType type)
{
	int writeJumpResult = HQ_SUCCESS;

	if(type == JmpType::Condition)
	{
		switch(m_cond)
		{
			case Condition::True:
				writeJumpResult = HqBytecodeWriteJumpIfTrue(m_hSerializer, m_reg, offset);
				break;

			case Condition::False:
				writeJumpResult = HqBytecodeWriteJumpIfFalse(m_hSerializer, m_reg, offset);
				break;

			default:
				writeJumpResult = HqBytecodeWriteJump(m_hSerializer, offset);
				break;
		}
	}
	else
	{
		writeJumpResult = HqBytecodeWriteJump(m_hSerializer, offset);
	}

	assert(writeJumpResult == HQ_SUCCESS); (void) writeJumpResult;
}

//----------------------------------------------------------------------------------------------------------------------

inline void Branch::_addCommand(const JmpType type, const JmpLoc loc)
{
	Command cmd;
	cmd.offset = HqSerializerGetStreamPosition(m_hSerializer);
	cmd.type = type;
	cmd.loc = loc;

	if(m_cmds.count == m_cmds.capacity)
	{
		// The list has run out of space, so make it larger.
		CommandList::Reserve(m_cmds, m_cmds.count + 1);
	}

	// Add the new command to the list.
	m_cmds.pData[m_cmds.count] = cmd;
	++m_cmds.count;

	// Write a temporary jump instruction that we will come back to fill in at the end.
	_writeJump(0, type);
}

//----------------------------------------------------------------------------------------------------------------------
