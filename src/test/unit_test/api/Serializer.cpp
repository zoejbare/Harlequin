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

#include "../../common/Util.h"

#include <gtest/gtest.h>

#ifdef HQ_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define VC_EXTRALEAN
	#include <Windows.h>
	#include <fcntl.h>

	#define F_OK 0

	/* mkstemp extracted from libc/sysdeps/posix/tempname.c.  Copyright
	   (C) 1991-1999, 2000, 2001, 2006 Free Software Foundation, Inc.

	   The GNU C Library is free software; you can redistribute it and/or
	   modify it under the terms of the GNU Lesser General Public
	   License as published by the Free Software Foundation; either
	   version 2.1 of the License, or (at your option) any later version.  */

	static const char letters[] =
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	/* Generate a temporary file name based on TMPL.  TMPL must match the
	   rules for mk[s]temp (i.e. end in "XXXXXX").  The name constructed
	   does not exist at the time of the call to mkstemp.  TMPL is
	   overwritten with the result.  */
	int
	mkstemp (char *tmpl)
	{
	  int len;
	  char *XXXXXX;
	  static unsigned long long value;
	  unsigned long long random_time_bits;
	  unsigned int count;
	  int fd = -1;
	  int save_errno = errno;

	  /* A lower bound on the number of temporary files to attempt to
		 generate.  The maximum total number of temporary file names that
		 can exist for a given template is 62**6.  It should never be
		 necessary to try all these combinations.  Instead if a reasonable
		 number of names is tried (we define reasonable as 62**3) fail to
		 give the system administrator the chance to remove the problems.  */
	#define ATTEMPTS_MIN (62 * 62 * 62)

	  /* The number of times to attempt to generate a temporary file.  To
		 conform to POSIX, this must be no smaller than TMP_MAX.  */
	#if ATTEMPTS_MIN < TMP_MAX
	  unsigned int attempts = TMP_MAX;
	#else
	  unsigned int attempts = ATTEMPTS_MIN;
	#endif

	  len = (int) strlen (tmpl);
	  if (len < 6 || strcmp (&tmpl[len - 6], "XXXXXX"))
		{
		  errno = EINVAL;
		  return -1;
		}

	/* This is where the Xs start.  */
	  XXXXXX = &tmpl[len - 6];

	  /* Get some more or less random data.  */
	  {
		SYSTEMTIME      stNow;
		FILETIME ftNow;

		// get system time
		GetSystemTime(&stNow);
		stNow.wMilliseconds = 500;
		if (!SystemTimeToFileTime(&stNow, &ftNow))
		{
			errno = -1;
			return -1;
		}

		random_time_bits = (((unsigned long long)ftNow.dwHighDateTime << 32)
							| (unsigned long long)ftNow.dwLowDateTime);
	  }
	  value += random_time_bits ^ (unsigned long long)GetCurrentThreadId ();

	  for (count = 0; count < attempts; value += 7777, ++count)
		{
		  unsigned long long v = value;

		  /* Fill in the random bits.  */
		  XXXXXX[0] = letters[v % 62];
		  v /= 62;
		  XXXXXX[1] = letters[v % 62];
		  v /= 62;
		  XXXXXX[2] = letters[v % 62];
		  v /= 62;
		  XXXXXX[3] = letters[v % 62];
		  v /= 62;
		  XXXXXX[4] = letters[v % 62];
		  v /= 62;
		  XXXXXX[5] = letters[v % 62];

		  // NOTE: Changed the file mode to O_BINARY to work with our tests.
		  fd = open (tmpl, O_RDWR | O_CREAT | O_EXCL | O_BINARY, _S_IREAD | _S_IWRITE);
		  if (fd >= 0)
		{
		  errno = save_errno;
		  return fd;
		}
		  else if (errno != EEXIST)
		return -1;
		}

	  /* We got out of the loop because we ran out of combinations to try.  */
	  errno = EEXIST;
	  return -1;
	}
#endif

//----------------------------------------------------------------------------------------------------------------------

static const uint8_t SerializerTestData[] =
{
	0x0, 0x1, 0x2, 0x3,
	0x4, 0x5, 0x6, 0x7,
	0x8, 0x9, 0xa, 0xb,
	0xc, 0xd, 0xe, 0xf,
};

//----------------------------------------------------------------------------------------------------------------------

enum class TempFileBehavior
{
	Empty,
	Filled,
};

//----------------------------------------------------------------------------------------------------------------------

class TemporaryFile
{
public:

	explicit TemporaryFile(const TempFileBehavior behavior)
	{
		// The last 6 characters *must* end in "XXXXXX" unless we switch to using mkstemps().
		strcpy(m_filename, "hq_XXXXXX");

		// Generate a temporary filename, then open the file, returning an active file descriptor.
		const int fd = mkstemp(m_filename);
		assert(fd >= 0);

		switch(behavior)
		{
			case TempFileBehavior::Filled:
			{
				const int result = write(fd, SerializerTestData, sizeof(SerializerTestData));
				(void) result;
				break;
			}

			default:
				break;
		}

		close(fd);
	}

	~TemporaryFile()
	{
		// Delete the file if it exists.
		if(access(m_filename, F_OK) == 0)
		{
			remove(m_filename);
		}
	}

	const char* GetFilename() const
	{
		return m_filename;
	}


private:

	char m_filename[32];
};

//----------------------------------------------------------------------------------------------------------------------

class FileInfo
{
public:

	FileInfo(const char* const filename)
		: m_fileSize(0)
	{
		if(!filename || filename[0] == '\0')
		{
			return;
		}

		FILE* const pFile = fopen(filename, "rb");
		if(pFile)
		{
			fseek(pFile, 0, SEEK_END);

			m_fileSize = ftell(pFile);

			fseek(pFile, 0, SEEK_SET);
			fclose(pFile);
		}
	}

	size_t GetFileSize() const
	{
		return m_fileSize;
	}


private:

	size_t m_fileSize;
};

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), CreateAndDisposeReader)
{
	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer in 'read' mode.
	const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_READER);
	ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

	// Validate the serializer mode.
	const int serializerMode = HqSerializerGetMode(hSerializer);
	EXPECT_EQ(serializerMode, HQ_SERIALIZER_MODE_READER);

	// Dispose of the serializer.
	const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
	EXPECT_EQ(disposeSerializerResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), CreateAndDisposeWriter)
{
	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer in 'write' mode.
	const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_WRITER);
	ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

	// Validate the serializer mode.
	const int serializerMode = HqSerializerGetMode(hSerializer);
	EXPECT_EQ(serializerMode, HQ_SERIALIZER_MODE_WRITER);

	// Dispose of the serializer.
	const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
	EXPECT_EQ(disposeSerializerResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), AttemptToCreateWithUnknownMode)
{
	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Attempt to create the serializer in 'unknown' mode. We expect this to fail.
	const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_UNKNOWN);
	ASSERT_EQ(createSerializerResult, HQ_ERROR_INVALID_ARG);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), ReadStreamFromBuffer)
{
	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer in 'read' mode.
	const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_READER);
	ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

	// Verify the serializer is set to native endianness.
	const int serializerEndianness = HqSerializerGetEndianness(hSerializer);
	EXPECT_EQ(serializerEndianness, HQ_ENDIAN_ORDER_NATIVE);

	// Load the stream from the in-memory test data.
	const int loadStreamResult = HqSerializerLoadStreamFromBuffer(hSerializer, SerializerTestData, sizeof(SerializerTestData));
	EXPECT_EQ(loadStreamResult, HQ_SUCCESS);

	if(loadStreamResult == HQ_SUCCESS)
	{
		// Validate the length of the loaded stream.
		const size_t streamLength = HqSerializerGetStreamLength(hSerializer);
		EXPECT_EQ(streamLength, sizeof(SerializerTestData));

		// Verify the stream position has been set to the start of the stream.
		// This is the expected behavior when a stream is loaded to a 'read' serializer.
		const size_t streamPosition = HqSerializerGetStreamPosition(hSerializer);
		EXPECT_EQ(streamPosition, 0);
	}

	// Dispose of the serializer.
	const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
	EXPECT_EQ(disposeSerializerResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), ReadStreamFromFile)
{
	// Create a temporary file that will be deleted when destructed.
	TemporaryFile tempFile(TempFileBehavior::Filled);

	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer in 'read' mode.
	const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_READER);
	ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

	// Verify the serializer is set to native endianness.
	const int serializerEndianness = HqSerializerGetEndianness(hSerializer);
	EXPECT_EQ(serializerEndianness, HQ_ENDIAN_ORDER_NATIVE);

	// Load the stream from the temporary file.
	const int loadStreamResult = HqSerializerLoadStreamFromFile(hSerializer, tempFile.GetFilename());
	EXPECT_EQ(loadStreamResult, HQ_SUCCESS);

	if(loadStreamResult == HQ_SUCCESS)
	{
		// Validate the length of the loaded stream.
		const size_t streamLength = HqSerializerGetStreamLength(hSerializer);
		EXPECT_EQ(streamLength, sizeof(SerializerTestData));

		// Verify the stream position has been set to the start of the stream.
		// This is the expected behavior when a stream is loaded to a 'read' serializer.
		const size_t streamPosition = HqSerializerGetStreamPosition(hSerializer);
		EXPECT_EQ(streamPosition, 0);
	}

	// Dispose of the serializer.
	const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
	EXPECT_EQ(disposeSerializerResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), LoadFileThenWriteFile)
{
	// Create a series of temporary files that will all be deleted upon destruction.
	// The first file is just for the initial stream load.  The others are for testing.
	TemporaryFile protoFile(TempFileBehavior::Filled);
	TemporaryFile firstTempFile(TempFileBehavior::Filled);
	TemporaryFile secondTempFile(TempFileBehavior::Filled);

	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer in 'write' mode.
	const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_WRITER);
	ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

	// Verify the serializer is set to native endianness.
	const int serializerEndianness = HqSerializerGetEndianness(hSerializer);
	EXPECT_EQ(serializerEndianness, HQ_ENDIAN_ORDER_NATIVE);

	// Load the stream from the temporary, prototype file.
	const int loadStreamResult = HqSerializerLoadStreamFromFile(hSerializer, protoFile.GetFilename());
	EXPECT_EQ(loadStreamResult, HQ_SUCCESS);

	if(loadStreamResult == HQ_SUCCESS)
	{
		// Validate the length of the loaded stream.
		const size_t streamLength = HqSerializerGetStreamLength(hSerializer);
		EXPECT_EQ(streamLength, sizeof(SerializerTestData));

		// Verify the stream position has been set to the end of the stream.
		// This is the expected behavior when a stream is loaded to a 'write' serializer.
		const size_t streamPosition = HqSerializerGetStreamPosition(hSerializer);
		EXPECT_EQ(streamPosition, sizeof(SerializerTestData));

		// Save the stream to the first temp file. This should overwrite the existing file contents.
		const int saveStreamNoAppendResult = HqSerializerSaveStreamToFile(hSerializer, firstTempFile.GetFilename(), false);
		EXPECT_EQ(saveStreamNoAppendResult, HQ_SUCCESS);

		// Save the stream to the second temp file. This should append the stream data to the end of the file.
		const int saveStreamWithAppendResult = HqSerializerSaveStreamToFile(hSerializer, secondTempFile.GetFilename(), true);
		EXPECT_EQ(saveStreamWithAppendResult, HQ_SUCCESS);

		// Validate the length of the first temp file.
		FileInfo firstFileInfo(firstTempFile.GetFilename());
		EXPECT_EQ(firstFileInfo.GetFileSize(), sizeof(SerializerTestData));

		// Validate the length of the second temp file.
		FileInfo secondFileInfo(secondTempFile.GetFilename());
		EXPECT_EQ(secondFileInfo.GetFileSize(), sizeof(SerializerTestData) * 2);
	}

	// Dispose of the serializer.
	const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
	EXPECT_EQ(disposeSerializerResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), WriteNativeEndianData)
{
	constexpr size_t expectedStreamLength = 42;

	// Create an empty, temporary file so we have somewhere to save the test data.
	TemporaryFile tempFile(TempFileBehavior::Empty);

	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create a serializer that will write data to a file.
	{
		// Create the serializer in 'write' mode.
		const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_WRITER);
		ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

		// Verify the serializer is set to native endianness.
		const int endianness = HqSerializerGetEndianness(hSerializer);
		EXPECT_EQ(endianness, HQ_ENDIAN_ORDER_NATIVE);

		if(endianness == HQ_ENDIAN_ORDER_NATIVE)
		{
			// Write an 8-bit integer to the stream.
			const int writeInt8Result = HqSerializerWriteInt8(hSerializer, 1);
			EXPECT_EQ(writeInt8Result, HQ_SUCCESS);

			// Write a 16-bit integer to the stream.
			const int writeInt16Result = HqSerializerWriteInt16(hSerializer, 2);
			EXPECT_EQ(writeInt16Result, HQ_SUCCESS);

			// Write a 32-bit integer to the stream.
			const int writeInt32Result = HqSerializerWriteInt32(hSerializer, 3);
			EXPECT_EQ(writeInt32Result, HQ_SUCCESS);

			// Write a 64-bit integer to the stream.
			const int writeInt64Result = HqSerializerWriteInt64(hSerializer, 4);
			EXPECT_EQ(writeInt64Result, HQ_SUCCESS);

			// Write an 8-bit unsigned integer to the stream.
			const int writeUint8Result = HqSerializerWriteUint8(hSerializer, 5);
			EXPECT_EQ(writeUint8Result, HQ_SUCCESS);

			// Write a 16-bit unsigned integer to the stream.
			const int writeUint16Result = HqSerializerWriteUint16(hSerializer, 6);
			EXPECT_EQ(writeUint16Result, HQ_SUCCESS);

			// Write a 32-bit unsigned integer to the stream.
			const int writeUint32Result = HqSerializerWriteUint32(hSerializer, 7);
			EXPECT_EQ(writeUint32Result, HQ_SUCCESS);

			// Write a 64-bit unsigned integer to the stream.
			const int writeUint64Result = HqSerializerWriteUint64(hSerializer, 8);
			EXPECT_EQ(writeUint64Result, HQ_SUCCESS);

			// Write a 32-bit float to the stream.
			const int writeFloat32Result = HqSerializerWriteFloat32(hSerializer, 1.234567f);
			EXPECT_EQ(writeFloat32Result, HQ_SUCCESS);

			// Write a 64-bit float (double) to the stream.
			const int writeFloat64Result = HqSerializerWriteFloat64(hSerializer, 2.3456789);
			EXPECT_EQ(writeFloat64Result, HQ_SUCCESS);

			// Verify the stream is as long as we expect it would be by this point.
			const size_t streamLength = HqSerializerGetStreamLength(hSerializer);
			EXPECT_EQ(streamLength, expectedStreamLength);

			// Save the stream to the temp file.
			const int saveStreamResult = HqSerializerSaveStreamToFile(hSerializer, tempFile.GetFilename(), false);
			EXPECT_EQ(saveStreamResult, HQ_SUCCESS);
		}

		// Dispose of the serializer.
		const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
		ASSERT_EQ(disposeSerializerResult, HQ_SUCCESS);
	}

	// Create a serializer that will read the file contents.
	{
		// Create the serializer in 'read' mode.
		const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_READER);
		ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

		// Verify the serializer is set to native endianness.
		const int endianness = HqSerializerGetEndianness(hSerializer);
		EXPECT_EQ(endianness, HQ_ENDIAN_ORDER_NATIVE);

		if(endianness == HQ_ENDIAN_ORDER_NATIVE)
		{
			// Load the stream from the temp file.
			const int loadStreamResult = HqSerializerLoadStreamFromFile(hSerializer, tempFile.GetFilename());
			EXPECT_EQ(loadStreamResult, HQ_SUCCESS);

			if(loadStreamResult == HQ_SUCCESS)
			{
				// Verify the length of the loaded stream matches what was saved out.
				const size_t streamLength = HqSerializerGetStreamLength(hSerializer);
				EXPECT_EQ(streamLength, expectedStreamLength);

				if(streamLength == expectedStreamLength)
				{
					// Read an 8-bit integer from the stream.
					int8_t valueInt8 = 0;
					const int readInt8Result = HqSerializerReadInt8(hSerializer, &valueInt8);
					EXPECT_EQ(readInt8Result, HQ_SUCCESS);
					EXPECT_EQ(valueInt8, 1);

					// Read a 16-bit integer from the stream.
					int16_t valueInt16 = 0;
					const int readInt16Result = HqSerializerReadInt16(hSerializer, &valueInt16);
					EXPECT_EQ(readInt16Result, HQ_SUCCESS);
					EXPECT_EQ(valueInt16, 2);

					// Read a 32-bit integer from the stream.
					int32_t valueInt32 = 0;
					const int readInt32Result = HqSerializerReadInt32(hSerializer, &valueInt32);
					EXPECT_EQ(readInt32Result, HQ_SUCCESS);
					EXPECT_EQ(valueInt32, 3);

					// Read a 64-bit integer from the stream.
					int64_t valueInt64 = 0;
					const int readInt64Result = HqSerializerReadInt64(hSerializer, &valueInt64);
					EXPECT_EQ(readInt64Result, HQ_SUCCESS);
					EXPECT_EQ(valueInt64, 4);

					// Read an 8-bit unsigned integer from the stream.
					uint8_t valueUint8 = 0;
					const int readUint8Result = HqSerializerReadUint8(hSerializer, &valueUint8);
					EXPECT_EQ(readUint8Result, HQ_SUCCESS);
					EXPECT_EQ(valueUint8, 5);

					// Read a 16-bit unsigned integer from the stream.
					uint16_t valueUint16 = 0;
					const int readUint16Result = HqSerializerReadUint16(hSerializer, &valueUint16);
					EXPECT_EQ(readUint16Result, HQ_SUCCESS);
					EXPECT_EQ(valueUint16, 6);

					// Read a 32-bit unsigned integer from the stream.
					uint32_t valueUint32 = 0;
					const int readUint32Result = HqSerializerReadUint32(hSerializer, &valueUint32);
					EXPECT_EQ(readUint32Result, HQ_SUCCESS);
					EXPECT_EQ(valueUint32, 7ul);

					// Read a 64-bit unsigned integer from the stream.
					uint64_t valueUint64 = 0;
					const int readUint64Result = HqSerializerReadUint64(hSerializer, &valueUint64);
					EXPECT_EQ(readUint64Result, HQ_SUCCESS);
					EXPECT_EQ(valueUint64, 8ull);

					// Read a 32-bit float from the stream.
					float valueFloat32 = 0;
					const int readFloat32Result = HqSerializerReadFloat32(hSerializer, &valueFloat32);
					EXPECT_EQ(readFloat32Result, HQ_SUCCESS);
					EXPECT_NEAR(valueFloat32, 1.234567f, 0.0000001f);

					// Read a 64-bit float (double) from the stream.
					double valueFloat64 = 0;
					const int readFloat64Result = HqSerializerReadFloat64(hSerializer, &valueFloat64);
					EXPECT_EQ(readFloat64Result, HQ_SUCCESS);
					EXPECT_NEAR(valueFloat64, 2.3456789, 0.00000001);
				}
			}
		}

		// Dispose of the serializer.
		const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
		ASSERT_EQ(disposeSerializerResult, HQ_SUCCESS);
	}
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), WriteLittleEndianData)
{
	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer in 'write' mode.
	const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_WRITER);
	ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

	// Set the serializer to little endian.
	const int setLittleEndianResult = HqSerializerSetEndianness(hSerializer, HQ_ENDIAN_ORDER_LITTLE);
	EXPECT_EQ(setLittleEndianResult, HQ_SUCCESS);

	// Write a test value to the stream.
	const int writeResult = HqSerializerWriteUint64(hSerializer, 0x0102030405060708ull);
	EXPECT_EQ(writeResult, HQ_SUCCESS);

	// Reset the stream position back to the start of the stream.
	const int setPositionResult = HqSerializerSetStreamPosition(hSerializer, 0);
	EXPECT_EQ(setPositionResult, HQ_SUCCESS);

	// Reset to use the native CPU endianness to avoid endian-correcting the value being read.
	const int setNativeEndianResult = HqSerializerSetEndianness(hSerializer, HQ_ENDIAN_ORDER_NATIVE);
	EXPECT_EQ(setNativeEndianResult, HQ_SUCCESS);

	// Read the test value back out.
	uint64_t value = 0;
	const int readResult = HqSerializerReadUint64(hSerializer, &value);
	EXPECT_EQ(readResult, HQ_SUCCESS);

#if defined(HQ_CPU_ENDIAN_LITTLE)
	EXPECT_EQ(value, 0x0102030405060708ull);

#elif defined(HQ_CPU_ENDIAN_BIG)
	EXPECT_EQ(value, 0x0807060504030201ull);

#endif

	// Dispose of the serializer.
	const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
	EXPECT_EQ(disposeSerializerResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

TEST(_HQ_TEST_NAME(TestSerializer), WriteBigEndianData)
{
	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer in 'write' mode.
	const int createSerializerResult = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_WRITER);
	ASSERT_EQ(createSerializerResult, HQ_SUCCESS);

	// Set the serializer to big endian.
	const int setBigEndianResult = HqSerializerSetEndianness(hSerializer, HQ_ENDIAN_ORDER_BIG);
	EXPECT_EQ(setBigEndianResult, HQ_SUCCESS);

	// Write a test value to the stream.
	const int writeResult = HqSerializerWriteUint64(hSerializer, 0x0102030405060708ull);
	EXPECT_EQ(writeResult, HQ_SUCCESS);

	// Reset the stream position back to the start of the stream.
	const int setPositionResult = HqSerializerSetStreamPosition(hSerializer, 0);
	EXPECT_EQ(setPositionResult, HQ_SUCCESS);

	// Reset to use the native CPU endianness to avoid endian-correcting the value being read.
	const int setNativeEndianResult = HqSerializerSetEndianness(hSerializer, HQ_ENDIAN_ORDER_NATIVE);
	EXPECT_EQ(setNativeEndianResult, HQ_SUCCESS);

	// Read the test value back out.
	uint64_t value = 0;
	const int readResult = HqSerializerReadUint64(hSerializer, &value);
	EXPECT_EQ(readResult, HQ_SUCCESS);

#if defined(HQ_CPU_ENDIAN_LITTLE)
	EXPECT_EQ(value, 0x0807060504030201ull);

#elif defined(HQ_CPU_ENDIAN_BIG)
	EXPECT_EQ(value, 0x0102030405060708ull);

#endif

	// Dispose of the serializer.
	const int disposeSerializerResult = HqSerializerDispose(&hSerializer);
	EXPECT_EQ(disposeSerializerResult, HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------
