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
#include <string.h>

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
	m_lineStrings.reserve(lineCount);
	m_lineIndices.reserve(lineCount);

	char* const fileStart = m_fileContents.data();
	char* lineStart = fileStart;

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

		m_lineStrings.push_back(std::string_view(lineStart, length));
		m_lineIndices.push_back(size_t(lineStart - fileStart));

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
	const detail::MessageCode code,
	const detail::TokenSpan& beginSpan,
	const detail::TokenSpan& endSpan,
	const std::string& primaryMsg,
	const std::string& secondaryMsg)
{
	// Both spans should refer to the same source file.
	assert(beginSpan.sourceName == endSpan.sourceName);

	const int msgType = _getMsgType(code);

	const char* typeStr = nullptr;
	const char* codePrefix = nullptr;
	uint32_t adjustedCode = 0;

	switch(msgType)
	{
		case HQ_MESSAGE_TYPE_ERROR:
			typeStr = "error ";
			codePrefix = "E";
			adjustedCode = uint32_t(code) - uint32_t(detail::MessageCode::_ErrorStart_);
			break;

		case HQ_MESSAGE_TYPE_WARNING:
			typeStr = "warning ";
			codePrefix = "W";
			adjustedCode = uint32_t(code) - uint32_t(detail::MessageCode::_WarningStart_);
			break;

		default:
			// This should never happen.
			assert(false);
			break;
	}

	// The message code outputs as a 4-digit integer, so it supports a maximum
	// of 10000 individual codes per category. If somehow we've adjusted to over
	// this amount, something has gone very wrong.
	assert(adjustedCode < 10000);

	char codeStr[6];
	snprintf(codeStr, sizeof(codeStr) / sizeof(char), "%s%04" PRIu16, codePrefix, adjustedCode);

	std::stringstream msgStream;
	msgStream
		<< beginSpan.sourceName
		<< "(" << beginSpan.lineNumber << "): "
		<< typeStr << codeStr << ": "
		<< primaryMsg;

	const size_t beginSpanIndex = beginSpan.startIndex;
	const size_t endSpanIndex = endSpan.stopIndex;

	if(beginSpanIndex < endSpanIndex)
	{
		for(size_t lineIndex = beginSpan.lineNumber - 1; lineIndex < endSpan.lineNumber; ++lineIndex)
		{
			const std::string_view& sourceCode = m_lineStrings[lineIndex];
			const size_t lineStartIndex = m_lineIndices[lineIndex];
			const size_t lineEndIndex = lineStartIndex + sourceCode.size();

			if(lineStartIndex == lineEndIndex)
			{
				// Skip empty lines to save a bit of terminal real estate.
				continue;
			}

			// Add the source code text to the message.
			msgStream << std::endl << sourceCode << std::endl;

			// Add the diagnostic section to the message stream.
			for(size_t absoluteIndex = lineStartIndex; absoluteIndex < lineEndIndex && absoluteIndex < endSpanIndex + 1; ++absoluteIndex)
			{
				const size_t lineLocalIndex = absoluteIndex - lineStartIndex;
				const char charInLine = sourceCode[lineLocalIndex];
				const bool isWhitespace = (isspace(charInLine) > 0);

				char charToWrite = '\0';

				if(absoluteIndex < beginSpanIndex)
				{
					// If this is the first line, there will be some padding necessary before
					// the underlining can begin. We'll fill this padded area with either an
					// explicit space character or whatever whitespace exists at the padded index.
					charToWrite = isWhitespace
						? charInLine
						: ' ';
				}
				else
				{
					// Within the specific token span, we only underline non-whitespace characters.
					charToWrite = isWhitespace
						? charInLine
						: '~';
				}

				msgStream << charToWrite;
			}
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
