#!/usr/bin/env python3
#
# Copyright (c) 2023, Zoe J. Bare
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
# documentation files (the "Software"), to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions
# of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
# TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
# CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#

import argparse
import os
import requests
import sys
import textwrap

from datetime import datetime

########################################################################################################################

_THIS_PATH = os.path.abspath(os.path.dirname(__file__))
_UNICODE_DATA_REQUIRED_SEGMENT_COUNT = 15
_SPECIAL_CASING_REQUIRED_SEGMENT_COUNT = 5
_VERBOSE_LOGGING = False

########################################################################################################################

class RecordRange(object):
	def __init__(self, startIndex, endIndex):
		self._start = startIndex
		self._end = endIndex

	@property
	def start(self):
		return self._start

	@property
	def end(self):
		return self._end

########################################################################################################################

class CharacterMapping(object):
	def __init__(self, simple):
		self.simple = simple
		self.strict = [simple]

########################################################################################################################

class CharacterRecord(object):
	def __init__(self, codePointStr, lowerCaseStr, upperCaseStr, titleCaseStr):
		self._tableIndex = 0
		self._codePoint = int(codePointStr, 16)

		lowerCase = int(lowerCaseStr, 16) if lowerCaseStr else self._codePoint
		upperCase = int(upperCaseStr, 16) if upperCaseStr else self._codePoint
		titleCase = int(titleCaseStr, 16) if titleCaseStr else upperCase

		self._upperMapping = CharacterMapping(upperCase)
		self._lowerMapping = CharacterMapping(lowerCase)
		self._titleMapping = CharacterMapping(titleCase)

	@property
	def tableIndex(self):
		return self._tableIndex

	@property
	def codePoint(self):
		return self._codePoint

	@property
	def upperMapping(self):
		return self._upperMapping

	@property
	def lowerMapping(self):
		return self._lowerMapping

	@property
	def titleMapping(self):
		return self._titleMapping

	def updateTableIndex(self, index):
		self._tableIndex = index

########################################################################################################################

def _removeCommentsAndEmptyLines(fileLines):
	output = []
	for line in fileLines:
		try:
			idx = line.index("#")
			line = line[:idx]

		except:
			pass

		output.append(line.strip())

	return [x for x in output if x]

########################################################################################################################

def _parseUnicodeDataLine(line):
	segments = line.split(";")
	segmentCount = len(segments)
	assert \
		segmentCount == _UNICODE_DATA_REQUIRED_SEGMENT_COUNT, \
		f"Expected {_UNICODE_DATA_REQUIRED_SEGMENT_COUNT} segments in each UnicodeData.txt line," \
		f"but found {segmentCount} instead"

	stripSegment = lambda string, default: \
		string.strip() if string else default

	codePointStr = stripSegment(segments[0], None)
	assert \
		codePointStr, \
		f"UnicodeData.txt line did not contain a valid code point: \"{line}\""

	category = stripSegment(segments[2], None)
	if not category or category not in ["Lu", "Ll", "Lt", "Nl", ]:
		# Skip entries that we know we don't care about.
		return None

	upperCaseStr = stripSegment(segments[12], None)
	lowerCaseStr = stripSegment(segments[13], None)
	titleCaseStr = stripSegment(segments[14], None)

	record = CharacterRecord(codePointStr, lowerCaseStr, upperCaseStr, titleCaseStr)

	return record

########################################################################################################################

def _parseSpecialCasingLine(line, recordTable):
	segments = line.split(";")
	segmentCount = len(segments)
	assert \
		segmentCount >= _SPECIAL_CASING_REQUIRED_SEGMENT_COUNT, \
		f"Expected {_SPECIAL_CASING_REQUIRED_SEGMENT_COUNT} segments in each SpecialCasing.txt line," \
		f"but found {segmentCount} instead on line: \"{line}\""

	stripSegment = lambda string, default: \
		string.strip() if string else default
	stripAndSplitSegment = lambda string, default: \
		[x for x in string.strip().split(" ") if x] if string else default

	condition = stripSegment(segments[4], None)
	if condition:
		# Skip any entries that require special conditions.
		return

	codePoint = stripSegment(segments[0], None)
	assert \
		codePoint, \
		f"SpecialCasing.txt line did not contain a valid code point: \"{line}\""

	lowerCaseMapping = stripAndSplitSegment(segments[1], [])
	titleCaseMapping = stripAndSplitSegment(segments[2], [])
	upperCaseMapping = stripAndSplitSegment(segments[3], [])

	codePoint = int(codePoint, 16)
	if codePoint not in recordTable:
		# Disregard any code point that was not already added to the record table.
		return

	lowerCaseMapping = [int(x, 16) for x in lowerCaseMapping if x]
	titleCaseMapping = [int(x, 16) for x in titleCaseMapping if x]
	upperCaseMapping = [int(x, 16) for x in upperCaseMapping if x]

	record = recordTable[codePoint]

	if lowerCaseMapping:
		record.lowerMapping.strict = lowerCaseMapping

	if titleCaseMapping:
		record.titleMapping.strict = titleCaseMapping

	if upperCaseMapping:
		record.upperMapping.strict = upperCaseMapping

########################################################################################################################

def _downloadUnicodeDataFile():
	url = "https://www.unicode.org/Public/16.0.0/ucd/UnicodeData.txt"
	
	print(f"Downloading {url} ...")

	req = requests.get(url)
	lines = _removeCommentsAndEmptyLines(req.content.decode("utf-8").splitlines())

	if _VERBOSE_LOGGING:
		print(f"\t-> File size: {len(req.content)} byte(s)")

	return lines

########################################################################################################################

def _downloadSpecialCasingFile():
	url = "https://www.unicode.org/Public/16.0.0/ucd/SpecialCasing.txt"

	print(f"Downloading {url} ...")

	req = requests.get(url)
	lines = _removeCommentsAndEmptyLines(req.content.decode("utf-8").splitlines())

	if _VERBOSE_LOGGING:
		print(f"\t-> File size: {len(req.content)} byte(s)")

	return lines

########################################################################################################################

def _generateRecordTable(unicodeDataLines, specialCasingLines):
	print("Generating table ...")

	recordTable = dict()
	maxCodePoint = 0

	# Build the initial table from the UnicodeData.txt entries.
	for line in unicodeDataLines:
		record = _parseUnicodeDataLine(line)

		if record:
			recordTable[record.codePoint] = record

			# Keep track of the maximum code point value.
			if maxCodePoint < record.codePoint:
				maxCodePoint = record.codePoint

	# Fix up table entries based on rules defined in SpecialCasing.txt.
	for line in specialCasingLines:
		_parseSpecialCasingLine(line, recordTable)

	codePointRange = maxCodePoint + 1
	tableIndex = 0

	# Update the table indices for each record.
	for index in range(0, codePointRange):
		record = recordTable.get(index, None)
		if record:
			# Increment the table index *before* updating it on the record to account
			# for the dummy entry that will be inserted into the output table later.
			tableIndex = tableIndex + 1

			record.updateTableIndex(tableIndex)

	if _VERBOSE_LOGGING:
		print(f"\t-> Record table length: {len(recordTable)}")
		print(f"\t-> Largest code point:  {hex(maxCodePoint)}")

	return recordTable

########################################################################################################################

def _writeSourceFiles(recordTable):
	print("Writing source files ...")

	mappingArrayLen = 1
	maxCodePoint = 0

	# Calculate the required bounds of the record table.
	for _, record in recordTable.items():
		strictUpperLen = len(record.upperMapping.strict)
		strictLowerLen = len(record.lowerMapping.strict)
		strictTitleLen = len(record.titleMapping.strict)

		if mappingArrayLen < strictUpperLen:
			mappingArrayLen = strictUpperLen
		if mappingArrayLen < strictLowerLen:
			mappingArrayLen = strictLowerLen
		if mappingArrayLen < strictTitleLen:
			mappingArrayLen = strictTitleLen

		if maxCodePoint < record.codePoint:
			maxCodePoint = record.codePoint

	recordTableLength = len(recordTable) + 1
	codePointRange = maxCodePoint + 1

	# The first record will be a dummy entry for characters that are not represented in the record table.
	recordLines = ["{ { {0}, 0, 0 }, { {0}, 0, 0 }, { {0}, 0, 0 }, 0 }"]

	# Preallocate space in the index list, defaulting to an index of 0 which represents entries without a record.
	tableIndices = ["0"] * codePointRange

	# Build the lines that will represent each record in the table.
	for index in range(0, codePointRange):
		record = recordTable.get(index, None)

		if record:
			strictLowerMappings = ", ".join([hex(x) for x in record.lowerMapping.strict])
			strictUpperMappings = ", ".join([hex(x) for x in record.upperMapping.strict])
			strictTitleMappings = ", ".join([hex(x) for x in record.titleMapping.strict])

			line = "{ " \
				f"{{ {{ {strictLowerMappings} }}, {hex(record.lowerMapping.simple)}, {len(record.lowerMapping.strict)} }}, " \
				f"{{ {{ {strictUpperMappings} }}, {hex(record.upperMapping.simple)}, {len(record.upperMapping.strict)} }}, " \
				f"{{ {{ {strictTitleMappings} }}, {hex(record.titleMapping.simple)}, {len(record.titleMapping.strict)} }}, " \
				f"{hex(record.codePoint)}" \
				" }"

			recordLines.append(line)

			tableIndices[index] = hex(record.tableIndex)

	# Format the lines so they can be inserted directly into the output file.
	recordLines = ",\n\t\t".join(recordLines)
	tableIndices = textwrap.wrap(", ".join(tableIndices), width=120, replace_whitespace=False)
	tableIndices = "\n\t\t".join(tableIndices)

	# Determine the index type based on how many entries there are in the record table.
	if recordTableLength > 0xFFFFFFFF:
		indexType = "uint64_t"
	elif recordTableLength > 0xFFFF:
		indexType = "uint32_t"
	elif recordTableLength > 0xFF:
		indexType = "uint16_t"
	else:
		indexType = "uint8_t"

	fileNameNoExt = "utf32-record-table"
	filePreamble = f"""/* Copyright (C) {datetime.now().year}, Zoe J. Bare */
/* DO NOT EDIT, changes will be lost! This file was automatically generated by {os.path.basename(__file__)}. */
"""

	sourceFileData = f"""{filePreamble}
#include "{fileNameNoExt}.h"

extern "C" {{

HQ_BASE_API const struct HqUtf32RecordTable utf32RecordTable =
{{
	{{
		{recordLines}
	}},
	{{
		{tableIndices}
	}}
}};

}}
"""

	headerFileData = f"""{filePreamble}
#include "../../HqBuildEnv.h"

#ifdef __cplusplus
extern "C" {{
#endif

#define HQ_UTF32_STRICT_MAPPING_ARRAY_LENGTH {mappingArrayLen}
#define HQ_UTF32_RECORD_ENTRY_COUNT          {recordTableLength}
#define HQ_UTF32_RECORD_INDEX_COUNT          {codePointRange}
#define HQ_UTF32_RECORD_INDEX_TYPE           {indexType}

struct HqUtf32Mapping
{{
	char32_t strict[HQ_UTF32_STRICT_MAPPING_ARRAY_LENGTH];
	char32_t simple;
	size_t strictLength;
}};

struct HqUtf32Record
{{
	struct HqUtf32Mapping lowerCase;
	struct HqUtf32Mapping upperCase;
	struct HqUtf32Mapping titleCase;
	char32_t codePoint;
}};

struct HqUtf32RecordTable
{{
	struct HqUtf32Record records[HQ_UTF32_RECORD_ENTRY_COUNT];
	HQ_UTF32_RECORD_INDEX_TYPE indices[HQ_UTF32_RECORD_INDEX_COUNT];
}};

HQ_BASE_API extern const struct HqUtf32RecordTable utf32RecordTable;

#ifdef __cplusplus
}}
#endif
"""

	outputDirPath = os.path.join(_THIS_PATH, "src", "lib", "base", "unicode")
	sourceFilePath = os.path.join(outputDirPath, f"{fileNameNoExt}.cpp")
	headerFilePath = os.path.join(outputDirPath, f"{fileNameNoExt}.h")

	# Create the output directory if it doesn't exist.
	if not os.access(outputDirPath, os.F_OK):
		os.makedirs(outputDirPath)

	# Write the source file data.
	with open(sourceFilePath, "w") as f:
		f.write(sourceFileData)

	# Write the header file data.
	with open(headerFilePath, "w") as f:
		f.write(headerFileData)

########################################################################################################################

def main():
	global _VERBOSE_LOGGING

	parser = argparse.ArgumentParser()

	parser.add_argument(
		"-v", "--verbose",
		action="store_true",
		help="Enable verbose logging"
	)

	args = parser.parse_args()
	_VERBOSE_LOGGING = args.verbose

	unicodeDataLines = _downloadUnicodeDataFile()
	specialCasingLines = _downloadSpecialCasingFile()
	recordTable = _generateRecordTable(unicodeDataLines, specialCasingLines)

	_writeSourceFiles(recordTable)

	print("... done!")

########################################################################################################################

if __name__ == "__main__":
	main()
