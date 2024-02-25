//
// Copyright (c) 2024, Zoe J. Bare
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

#include "detail/MessageCode.hpp"
#include "detail/TokenSpan.hpp"

#include <Harlequin.h>
#include <BaseErrorListener.h>

#include <string>
#include <string_view>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------

class SourceContext
{
public:

	SourceContext();

	void Initialize(
		HqReportHandle hReport, 
		const char* filePath, 
		const char* fileData, 
		size_t fileLength);

	const std::string& GetFilePath() const;

	size_t GetErrorCount() const;
	size_t GetWarningCount() const;

	bool EncounteredErrors() const;

	void Report(MessageCode code, const detail::TokenSpan& span, const std::string& msg);
	void Report(MessageCode code, const detail::TokenSpan& span, const std::string& primaryMsg, const std::string& secondaryMsg);


private:

	typedef std::vector<std::string_view> Lines;

	enum class MsgType
	{
		Error,
		Warning,
	};

	int _getMsgType(MessageCode);

	HqReportHandle m_hReport;

	std::string m_filePath;
	std::string m_fileContents;

	Lines m_fileLines;

	size_t m_errorCount;
	size_t m_warningCount;
};

//----------------------------------------------------------------------------------------------------------------------

inline SourceContext::SourceContext()
	: m_hReport(HQ_REPORT_HANDLE_NULL)
	, m_filePath()
	, m_fileContents()
	, m_fileLines()
	, m_errorCount(0)
	, m_warningCount(0)
{
}

//----------------------------------------------------------------------------------------------------------------------

inline const std::string& SourceContext::GetFilePath() const
{
	return m_filePath;
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t SourceContext::GetErrorCount() const
{
	return m_errorCount;
}

//----------------------------------------------------------------------------------------------------------------------

inline size_t SourceContext::GetWarningCount() const
{
	return m_warningCount;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool SourceContext::EncounteredErrors() const
{
	return m_errorCount > 0;
}

//----------------------------------------------------------------------------------------------------------------------

inline void SourceContext::Report(
	const MessageCode code, 
	const detail::TokenSpan& span, 
	const std::string& msg)
{
	static const std::string empty;

	Report(code, span, msg, empty);
}

//----------------------------------------------------------------------------------------------------------------------

inline int SourceContext::_getMsgType(const MessageCode code)
{
	return (int(code) < int(MessageCode::_WarningStart_))
		? HQ_MESSAGE_TYPE_ERROR
		: HQ_MESSAGE_TYPE_WARNING;
}

//----------------------------------------------------------------------------------------------------------------------
