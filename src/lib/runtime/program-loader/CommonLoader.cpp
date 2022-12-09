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

#include "CommonLoader.hpp"

#include "../Value.hpp"
#include "../Vm.hpp"

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

bool HqProgramCommonLoader::CheckMagicNumber(const HqFileHeader& fileHeader)
{
	return fileHeader.magicNumber[0] == 'H'
		|| fileHeader.magicNumber[1] == 'Q'
		|| fileHeader.magicNumber[2] == 'P'
		|| fileHeader.magicNumber[3] == 'R'
		|| fileHeader.magicNumber[4] == 'G';
}

//----------------------------------------------------------------------------------------------------------------------

HqString* HqProgramCommonLoader::ReadString(
	HqSerializerHandle hSerializer,
	HqReportHandle hReport
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(hReport != HQ_REPORT_HANDLE_NULL);

	int result = 0;

	const size_t streamPosition = HqSerializerGetStreamPosition(hSerializer);
	const size_t streamLength = HqSerializerGetStreamLength(hSerializer);

	if(streamPosition == streamLength)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"ReadString error: End of data stream"
		);
		return nullptr;
	}

	// We can make reading native strings much more efficient by using the serializer's
	// stream directly. Doing this and embedding the null-terminator into the stream for
	// each string means we can create the native string directly from the data in the
	// serializer without having to allocate and copy to-then-from a staging string.
	// It also saves space due to not having the size baked in for each string.
	const char* const pStreamData = reinterpret_cast<const char*>(HqSerializerGetRawStreamPointer(hSerializer));

	// The stream data retrieved from the serializer is at the start of its memory,
	// so we adjust to the current position in the stream to get to the beginning
	// of the string data.
	HqString* const pString = HqString::Create(pStreamData + streamPosition);
	if(!pString)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"ReadString error: Failed to create HqString object"
		);
		return nullptr;
	}

	// Because we hacked our way into the stream to get the string data,
	// we need to manually adjust the stream position ourselves. And we
	// add 1 here to account for the null-terminator.
	result = HqSerializerSetStreamPosition(hSerializer, streamPosition + pString->length + 1);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqString::Release(pString);
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"ReadString error: Failed to update stream position: error=\"%s\"",
			errorString
		);
		return nullptr;
	}

	return pString;
}

//----------------------------------------------------------------------------------------------------------------------
