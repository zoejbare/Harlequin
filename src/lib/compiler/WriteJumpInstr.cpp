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

#include "WriteJumpInstr.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

WriteJumpInstr::WriteJumpInstr(
	HqSerializerHandle hSerializer,
	const JumpBehavior behavior, 
	const JumpCondition condition, 
	const uint32_t gpRegIndex)
	: m_hSerializer(hSerializer)
	, m_offset(HqSerializerGetStreamPosition(hSerializer))
	, m_regIndex(gpRegIndex)
	, m_beh(behavior)
	, m_cond(condition)
{
	if(m_beh == JumpBehavior::Forward)
	{
		// Write a temporary instruction since we'll come back to fill it in at the end.
		const int writeDummyJumpInstrResult = prv_writeJump(0);
		assert(writeDummyJumpInstrResult == HQ_SUCCESS);
	}
}

//----------------------------------------------------------------------------------------------------------------------

WriteJumpInstr::~WriteJumpInstr()
{
	const size_t currentOffset = HqSerializerGetStreamPosition(m_hSerializer);

	if(m_beh == JumpBehavior::Forward)
	{
		const int setStreamPositionBackResult = HqSerializerSetStreamPosition(m_hSerializer, m_offset);
		assert(setStreamPositionBackResult == HQ_SUCCESS);

		const int writeRealJumpInstrResult = prv_writeJump(int32_t(currentOffset - m_offset));
		assert(writeRealJumpInstrResult == HQ_SUCCESS);

		const int restoreStreamPositionResult = HqSerializerSetStreamPosition(m_hSerializer, currentOffset);
		assert(restoreStreamPositionResult == HQ_SUCCESS);
	}
	else
	{
		const int writeRealJumpInstrResult = prv_writeJump(-int32_t(currentOffset - m_offset));
		assert(writeRealJumpInstrResult == HQ_SUCCESS);
	}
}

//----------------------------------------------------------------------------------------------------------------------

inline int WriteJumpInstr::prv_writeJump(const int32_t offset)
{
	switch(m_cond)
	{
		case JumpCondition::IfTrue:
			return HqBytecodeWriteJumpIfTrue(m_hSerializer, m_regIndex, offset);

		case JumpCondition::IfFalse:
			return HqBytecodeWriteJumpIfFalse(m_hSerializer, m_regIndex, offset);

		default:
			return HqBytecodeWriteJump(m_hSerializer, offset);
	}
}

//----------------------------------------------------------------------------------------------------------------------
