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

//----------------------------------------------------------------------------------------------------------------------

class HQ_MAIN_API JumpInstruction
{
public:

	enum class Behavior
	{
		Forward,
		Back,
	};

	enum class Condition
	{
		None,
		IfTrue,
		IfFalse,
	};

	JumpInstruction(const JumpInstruction&) = delete;
	JumpInstruction(JumpInstruction&&) = delete;

	JumpInstruction& operator =(const JumpInstruction&) = delete;
	JumpInstruction& operator =(JumpInstruction&&) = delete;

	JumpInstruction();

	void Begin(HqSerializerHandle hSerializer, Behavior behavior, Condition condition, uint32_t gpRegIndex);
	void End();


private:

	int _writeJump(int32_t);

	HqSerializerHandle m_hSerializer;
	size_t m_offset;
	uint32_t m_regIndex;
	Behavior m_beh;
	Condition m_cond;
};

//----------------------------------------------------------------------------------------------------------------------

inline JumpInstruction::JumpInstruction()
	: m_hSerializer(HQ_SERIALIZER_HANDLE_NULL)
	, m_offset(0)
	, m_regIndex(0)
	, m_beh(Behavior::Forward)
	, m_cond(Condition::None)
{
}

//----------------------------------------------------------------------------------------------------------------------
