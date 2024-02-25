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

#include "SourceContext.hpp"

#include <assert.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------

void SourceContext::Initialize(
	HqReportHandle hReport,
	const char* const filePath,
	const char* const fileData, 
	const size_t fileLength)
{
	assert(hReport != HQ_REPORT_HANDLE_NULL);
	assert(filePath != nullptr);
	assert(filePath[0] != '\0');
	assert(fileData != nullptr);
	assert(m_hReport == HQ_REPORT_HANDLE_NULL); // Allow initialization only once per context.

	m_hReport = hReport;

	m_filePath = std::string(filePath);
	m_fileContents = std::string(fileData, fileLength);

	// Replace all carriage returns in the file text with whitespaces.
	std::replace(m_fileContents.begin(), m_fileContents.end(), '\r', ' ');

	// Calculate the number of lines there are in the file text.
	const size_t lineCount = std::count(m_fileContents.begin(), m_fileContents.end(), '\n') + 1;

	// Reserve enough space in the array for each line.
	m_fileLines.reserve(lineCount);

	char* lineStart = m_fileContents.data();

	// Keep track of each line as a string reference back into the file contents string.
	for(;;)
	{
		char* const lineEnd = strchr(lineStart, '\n');

		// We'll calculate the string view at the proper length so the newline character at the
		// end of the line is functionally a null terminator. If there is no newline character,
		// we can let the line go all the way to end of the main string.
		size_t length = lineEnd 
			? size_t(lineEnd - lineStart) 
			: strlen(lineStart);

		// Trim the whitespace from the end of the line.
		while(length > 0 && isspace(lineStart[length - 1]))
		{
			--length;
		}

		m_fileLines.push_back(std::string_view(lineStart, length));

		if(!lineEnd)
		{
			// End of the file has been reached.
			break;
		}

		// Set the start of the next line just past the end of the current line.
		lineStart = lineEnd + 1;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void SourceContext::Report(
	const MessageCode code,
	const detail::TokenSpan& span,
	const std::string& primaryMsg,
	const std::string& secondaryMsg)
{
	const int msgType = _getMsgType(code);

	const char* typeStr = nullptr;
	switch(msgType)
	{
		case HQ_MESSAGE_TYPE_ERROR:
			typeStr = "error ";
			break;

		case HQ_MESSAGE_TYPE_WARNING:
			typeStr = "warning ";
			break;

		default:
			// This should never happen.
			assert(false);
			break;
	}

	char codeStr[6];
	snprintf(codeStr, sizeof(codeStr) / sizeof(char), "C%04" PRIu16, uint16_t(code));

	std::stringstream msgStream;
	msgStream
		<< span.sourceName
		<< "(" << span.lineNumber << "): "
		<< typeStr << codeStr << ": "
		<< primaryMsg;

	if(span.startIndex < span.stopIndex)
	{
		const std::string_view& sourceCode = m_fileLines[span.lineNumber - 1];

		// Add the source code text to the message.
		msgStream << std::endl << sourceCode << std::endl;

		// Add the whitespace leading up to the underlining.
		for(size_t i = 0; i < span.positionInLine; ++i)
		{
			const char charInLine = sourceCode[i];
			const char charToWrite = (isspace(charInLine) > 0)
				? charInLine
				: ' ';

			msgStream << charToWrite;
		}

		// Add the underline that is as long as the offending token.
		for(size_t i = span.startIndex; i <= span.stopIndex; ++i)
		{
			msgStream << '~';
		}
	}

	if(!secondaryMsg.empty())
	{
		// Append the secondary message to the end of the stream.
		msgStream << std::endl << primaryMsg;
	}

	// Pass the final message string to the API's reporting mechanism.
	HqReportMessage(m_hReport, msgType, "%s", msgStream.str().c_str());

	if(msgType == HQ_MESSAGE_TYPE_ERROR)
	{
		++m_errorCount;
	}
}

//----------------------------------------------------------------------------------------------------------------------
