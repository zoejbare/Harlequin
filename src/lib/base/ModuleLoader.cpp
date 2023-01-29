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

#include "ModuleLoader.hpp"

#include <assert.h>
#include <string.h>
#include <inttypes.h>

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::Load(HqModuleLoader& output, HqReportHandle hReport, const char* const filePath, const uint32_t flags)
{
	assert(filePath != nullptr);

	prv_initialize(output);

	int result = HQ_SUCCESS;

	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer.
	result = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_READER);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to create serializer to load module: error='%s'",
			HqGetErrorCodeString(result)
		);
		return false;
	}

	// Load the input buffer into the serializer.
	result = HqSerializerLoadStreamFromFile(hSerializer, filePath);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to load module file data from input file path into serializer"
				": error='%s'"
				", filePath='%s'", 
			HqGetErrorCodeString(result), 
			filePath
		);
		HqSerializerDispose(&hSerializer);
		return false;
	}

	// Load the module from the serializer.
	const bool loadSuccess = prv_load(output, hReport, hSerializer, flags);

	// Dispose of the serializer now that we're done with it.
	HqSerializerDispose(&hSerializer);

	return loadSuccess;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleLoader::Load(
	HqModuleLoader& output, 
	HqReportHandle hReport, 
	const void* const fileData, 
	const size_t fileLength, 
	const uint32_t flags
)
{
	assert(fileData != nullptr);
	assert(fileLength > 0);

	prv_initialize(output);

	int result = HQ_SUCCESS;

	HqSerializerHandle hSerializer = HQ_SERIALIZER_HANDLE_NULL;

	// Create the serializer.
	result = HqSerializerCreate(&hSerializer, HQ_SERIALIZER_MODE_READER);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to create serializer to load module: error='%s'",
			HqGetErrorCodeString(result)
		);
		return false;
	}

	// Load the input buffer into the serializer.
	result = HqSerializerLoadStreamFromBuffer(hSerializer, fileData, fileLength);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to load module file data from input buffer into serializer: error='%s'", 
			HqGetErrorCodeString(result)
		);
		HqSerializerDispose(&hSerializer);
		return false;
	}

	// Load the module from the serializer.
	const bool loadSuccess = prv_load(output, hReport, hSerializer, flags);

	// Dispose of the serializer now that we're done with it.
	HqSerializerDispose(&hSerializer);

	return loadSuccess;
}

//----------------------------------------------------------------------------------------------------------------------

void HqModuleLoader::Dispose(HqModuleLoader& output)
{
	// Release all strings loaded for the module. This will cover all strings in the module since everything
	// else in the loader will point directly to these strings without taking an explicit reference.
	for(size_t stringIndex = 0; stringIndex < output.strings.count; ++stringIndex)
	{
		HqString::Release(output.strings.pData[stringIndex]);
	}

	// Dispose of all object type data in the module.
	for(size_t typeIndex = 0; typeIndex < output.objectTypes.count; ++typeIndex)
	{
		ObjectType& obj = output.objectTypes.pData[typeIndex];

		ObjectType::MemberDefinitionArray::Dispose(obj.memberDefinitions);
	}

	// Dispose of all function data in the module.
	for(size_t functionIndex = 0; functionIndex < output.functions.count; ++functionIndex)
	{
		Function& func = output.functions.pData[functionIndex];

		for(size_t blockIndex = 0; blockIndex < func.guardedBlocks.count; ++blockIndex)
		{
			Function::GuardedBlock block = func.guardedBlocks.pData[blockIndex];

			Function::GuardedBlock::ExceptionHandlerArray::Dispose(block.exceptionHandlers);
		}

		Function::GuardedBlockArray::Dispose(func.guardedBlocks);
	}

	// Dispose of all module resources.
	StringArray::Dispose(output.strings);
	StringArray::Dispose(output.dependencies);
	StringArray::Dispose(output.globals);
	ObjectTypeArray::Dispose(output.objectTypes);
	FunctionArray::Dispose(output.functions);
	ByteArray::Dispose(output.initBytecode);
	ByteArray::Dispose(output.bytecode);

	// Initialize the module loader object to prevent issues if it happens to be disposed of multiple times.
	prv_initialize(output);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_load(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer, const uint32_t flags)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	// Load the file header data.
	if(!prv_loadFileHeader(output, hReport, hSerializer))
	{
		return false;
	}

	// Load the module's table of contents.
	if(!prv_loadTableOfContents(output, hReport, hSerializer))
	{
		return false;
	}

	// Load the string table.
	if(!prv_loadStrings(output, hReport, hSerializer))
	{
		return false;
	}

	// Load the module dependencies.
	if(!prv_loadDependencies(output, hReport, hSerializer))
	{
		return false;
	}

	// Load the global variable table.
	if(!prv_loadGlobals(output, hReport, hSerializer))
	{
		return false;
	}

	// Load the object types defined in this module.
	if(!prv_loadObjectTypes(output, hReport, hSerializer))
	{
		return false;
	}

	// Load the functions defined in this module.
	if(!prv_loadFunctions(output, hReport, hSerializer))
	{
		return false;
	}

	if((flags & DISCARD_BYTECODE) == 0)
	{
		// Load the module init function bytecode.
		if(!prv_loadInitBytecode(output, hReport, hSerializer))
		{
			return false;
		}

		// Load the module bytecode.
		if(!prv_loadBytecode(output, hReport, hSerializer))
		{
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadFileHeader(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Read the file magic number.
	if(!prv_readBuffer(hSerializer, sizeof(output.fileHeader.magicNumber), output.fileHeader.magicNumber, result, streamOffset))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to read file magic number"
				": error='%s'"
				", streamOffset=%zu",
			HqGetErrorCodeString(result),
			streamOffset
		);
		return false;
	}

	// Initialize a temporary file header so we have the expected file magic number ready to compare against.
	HqModuleFileHeader tempHeader;
	HqModuleFileHeader::Initialize(tempHeader);

	// Verify the magic number is correct.
	if(memcmp(output.fileHeader.magicNumber, tempHeader.magicNumber, sizeof(tempHeader.magicNumber)) != 0)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Invalid module file magic number"
				": magicNumber='%s'"
				", expected='%s'",
			reinterpret_cast<const char*>(output.fileHeader.magicNumber),
			reinterpret_cast<const char*>(tempHeader.magicNumber)
		);
		return false;
	}
	
	// Read the reserved section of the file header.
	if(!prv_readBuffer(hSerializer, sizeof(output.fileHeader.reserved), output.fileHeader.reserved, result, streamOffset))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to read file header reserved section"
				": error='%s'"
				", streamOffset=%zu",
			HqGetErrorCodeString(result),
			streamOffset
		);
		return false;
	}

	// Read the 'isBigEndian' flag.
	if(!prv_readBool8(hSerializer, &output.fileHeader.isBigEndian, result, streamOffset))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to read module file big endian flag"
				": error='%s'"
				", streamOffset=%zu",
			HqGetErrorCodeString(result),
			streamOffset
		);
		return false;
	}

	output.endianness = output.fileHeader.isBigEndian
		? HQ_ENDIAN_ORDER_BIG
		: HQ_ENDIAN_ORDER_LITTLE;

	// Now that we know the endianness, we can set it on the serializer.
	result = HqSerializerSetEndianness(hSerializer, output.endianness);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to set endian mode on the module file serializer: error='%s'",
			HqGetErrorCodeString(result)
		);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadTableOfContents(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// String table
	{
		// Read the byte offset.
		if(!prv_readUint32(hSerializer, &output.contents.stringTable.offset, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module string table byte offset"": error='%s'"", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}

		// Read the number of entries.
		if(!prv_readUint32(hSerializer, &output.contents.stringTable.length, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module string table entry count"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}
	}

	// Dependency table
	{
		// Read the byte offset.
		if(!prv_readUint32(hSerializer, &output.contents.dependencyTable.offset, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module dependency table byte offset"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}

		// Read the number of entries.
		if(!prv_readUint32(hSerializer, &output.contents.dependencyTable.length, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module dependency table entry count"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}
	}

	// Global variable table
	{
		// Read the byte offset.
		if(!prv_readUint32(hSerializer, &output.contents.globalTable.offset, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module global variable table byte offset"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}

		// Read the number of entries.
		if(!prv_readUint32(hSerializer, &output.contents.globalTable.length, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module global variable table entry count"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}
	}

	// Object table
	{
		// Read the byte offset.
		if(!prv_readUint32(hSerializer, &output.contents.objectTable.offset, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module object table byte offset"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}

		// Read the number of entries.
		if(!prv_readUint32(hSerializer, &output.contents.objectTable.length, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module object table entry count"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}
	}

	// Function table
	{
		// Read the byte offset.
		if(!prv_readUint32(hSerializer, &output.contents.functionTable.offset, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module function table byte offset"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}

		// Read the number of entries.
		if(!prv_readUint32(hSerializer, &output.contents.functionTable.length, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module function table entry count"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}
	}

	// Module init bytecode
	{
		// Read the byte offset.
		if(!prv_readUint32(hSerializer, &output.contents.initBytecode.offset, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module initialization function byte offset"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}

		// Read the byte length.
		if(!prv_readUint32(hSerializer, &output.contents.initBytecode.length, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module initialization function byte length"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}
	}

	// General bytecode
	{
		// Read the byte offset.
		if(!prv_readUint32(hSerializer, &output.contents.bytecode.offset, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module bytecode byte offset"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}

		// Read the byte length.
		if(!prv_readUint32(hSerializer, &output.contents.bytecode.length, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module bytecode byte length"
					": error='%s'"
					", streamOffset=%zu",
				HqGetErrorCodeString(result),
				streamOffset
			);
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadStrings(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(output.contents.stringTable.length == 0)
	{
		// No data to load.
		return true;
	}

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Move the stream position to the start of the string table.
	result = HqSerializerSetStreamPosition(hSerializer, output.contents.stringTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to set file stream position to string table offset"
				": error='%s'"
				", streamOffset=%" PRIu32, 
			HqGetErrorCodeString(result), 
			output.contents.stringTable.offset
		);
		return false;
	}

	// Make space in the string table.
	StringArray::Reserve(output.strings, output.contents.stringTable.length);

	// Iterate for each string.
	for(; output.strings.count < output.contents.stringTable.length; ++output.strings.count)
	{
		// Read the string.
		if(!prv_readString(hSerializer, &output.strings.pData[output.strings.count], result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read string from module file"
					": error='%s'"
					", streamOffset=%zu"
					", index=%zu", 
				HqGetErrorCodeString(result),
				streamOffset,
				output.strings.count
			);
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadDependencies(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(output.contents.dependencyTable.length == 0)
	{
		// No data to load.
		return true;
	}

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Move the stream position to the start of the dependency table.
	result = HqSerializerSetStreamPosition(hSerializer, output.contents.dependencyTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to set file stream position to dependency table offset"
				": error='%s'"
				", streamOffset=%" PRIu32, 
			HqGetErrorCodeString(result), 
			output.contents.stringTable.offset
		);
		return false;
	}

	// Make space in the dependency table.
	StringArray::Reserve(output.dependencies, output.contents.dependencyTable.length);

	// Iterate for each dependency namme.
	for(; output.dependencies.count < output.contents.dependencyTable.length; ++output.dependencies.count)
	{
		// Read the global variable string.
		if(!prv_readStringFromIndex(output, hSerializer, &output.dependencies.pData[output.dependencies.count], result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module dependency string"
					": error='%s'"
					", streamOffset=%zu"
					", objectTypeIndex=%zu",
				HqGetErrorCodeString(result),
				streamOffset,
				output.dependencies.count
			);
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadGlobals(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(output.contents.globalTable.length == 0)
	{
		// No data to load.
		return true;
	}

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Move the stream position to the start of the global variable table.
	result = HqSerializerSetStreamPosition(hSerializer, output.contents.globalTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to set file stream position to global variable table offset"
				": error='%s'"
				", streamOffset=%" PRIu32, 
			HqGetErrorCodeString(result), 
			output.contents.globalTable.offset
		);
		return false;
	}

	// Make space in the global variable table.
	StringArray::Reserve(output.globals, output.contents.globalTable.length);

	// Iterate for each global variable name.
	for(; output.globals.count < output.contents.globalTable.length; ++output.globals.count)
	{
		// Read the global variable string.
		if(!prv_readStringFromIndex(output, hSerializer, &output.globals.pData[output.globals.count], result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module global variable string"
					": error='%s'"
					", streamOffset=%zu"
					", objectTypeIndex=%zu",
				HqGetErrorCodeString(result),
				streamOffset,
				output.globals.count
			);
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadObjectTypes(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(output.contents.stringTable.length == 0)
	{
		// No data to load.
		return true;
	}

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Move the stream position to the start of the object schema table.
	result = HqSerializerSetStreamPosition(hSerializer, output.contents.objectTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to set file stream position to object schema table offset"
				": error='%s'"
				", streamOffset=%" PRIu32, 
			HqGetErrorCodeString(result), 
			output.contents.objectTable.offset
		);
		return false;
	}

	// Make space in the object schema table.
	ObjectTypeArray::Reserve(output.objectTypes, output.contents.objectTable.length);

	// Iterate for each object schema.
	for(; output.objectTypes.count < size_t(output.contents.objectTable.length); ++output.objectTypes.count)
	{
		ObjectType& type = output.objectTypes.pData[output.objectTypes.count];

		// Initialize the object's member definition array.
		ObjectType::MemberDefinitionArray::Initialize(type.memberDefinitions);

		// Read the object member name string.
		if(!prv_readStringFromIndex(output, hSerializer, &type.pName, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module object type name string"
					": error='%s'"
					", streamOffset=%zu"
					", objectTypeIndex=%zu",
				HqGetErrorCodeString(result),
				streamOffset,
				output.objectTypes.count
			);
			return false;
		}

		// Read the number of members on the object.
		uint32_t memberCount = 0;
		if(!prv_readUint32(hSerializer, &memberCount, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module object type member count"
					": error='%s'"
					", streamOffset=%zu"
					", objectTypeIndex=%zu", 
				HqGetErrorCodeString(result),
				streamOffset,
				output.objectTypes.count
			);
			return false;
		}

		// Reserve space for each object member definition.
		ObjectType::MemberDefinitionArray::Reserve(type.memberDefinitions, size_t(memberCount));

		// Iterate for each member in the current object schema.
		for(; type.memberDefinitions.count < size_t(memberCount); ++type.memberDefinitions.count)
		{
			ObjectType::MemberDefinition& memberDef = type.memberDefinitions.pData[type.memberDefinitions.count];

			// Read the object member name string.
			if(!prv_readStringFromIndex(output, hSerializer, &memberDef.pName, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read module object member name string"
						": error='%s'"
						", streamOffset=%zu"
						", objectTypeIndex=%zu"
						", memberIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					output.objectTypes.count,
					type.memberDefinitions.count
				);
				return false;
			}

			// Read the type of the current object member.
			if(!prv_readUint32AsUint8(hSerializer, &memberDef.type, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read module object member type"
						": error='%s'"
						", streamOffset=%zu"
						", objectTypeIndex=%zu"
						", memberIndex=%zu", 
					HqGetErrorCodeString(result),
					streamOffset,
					output.objectTypes.count,
					type.memberDefinitions.count
				);
				return false;
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadFunctions(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(output.contents.functionTable.length == 0)
	{
		// No data to load.
		return true;
	}

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Move the stream position to the start of the function table.
	result = HqSerializerSetStreamPosition(hSerializer, output.contents.functionTable.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to set file stream position to function table offset"
				": error='%s'"
				", streamOffset=%" PRIu32, 
			HqGetErrorCodeString(result), 
			output.contents.functionTable.offset
		);
		return false;
	}

	// Make space in the function table.
	FunctionArray::Reserve(output.functions, output.contents.functionTable.length);

	// Iterate for each function.
	for(; output.functions.count < size_t(output.contents.functionTable.length); ++output.functions.count)
	{
		Function& func = output.functions.pData[output.functions.count];

		// Initialize the function's guarded block array.
		Function::GuardedBlockArray::Initialize(func.guardedBlocks);

		// Read the function signature string.
		if(!prv_readStringFromIndex(output, hSerializer, &func.pSignature, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module function signature string"
					": error='%s'"
					", streamOffset=%zu"
					", funcIndex=%zu",
				HqGetErrorCodeString(result),
				streamOffset,
				output.functions.count
			);
			return false;
		}

		// Read the function's number of input values.
		if(!prv_readUint16(hSerializer, &func.numInputs, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read number of input values for the module function"
					": error='%s'"
					", streamOffset=%zu"
					", funcIndex=%zu",
				HqGetErrorCodeString(result),
				streamOffset,
				output.functions.count
			);
			return false;
		}

		// Read the function's number of output values.
		if(!prv_readUint16(hSerializer, &func.numOutputs, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read number of output values for the module function"
					": error='%s'"
					", streamOffset=%zu"
					", funcIndex=%zu",
				HqGetErrorCodeString(result),
				streamOffset,
				output.functions.count
			);
			return false;
		}

		// Read the function 'isNative' flag.
		if(!prv_readBool32(hSerializer, &func.isNative, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to read module function 'isNative' flag"
					": error='%s'"
					", streamOffset=%zu"
					", funcIndex=%zu",
				HqGetErrorCodeString(result),
				streamOffset,
				output.functions.count
			);
			return false;
		}

		if(!func.isNative)
		{
			// Read the function's bytecode offset.
			if(!prv_readUint32(hSerializer, &func.offset, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read the module function bytecode offset"
						": error='%s'"
						", streamOffset=%zu"
						", funcIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					output.functions.count
				);
				return false;
			}

			// Read the function's bytecode length.
			if(!prv_readUint32(hSerializer, &func.length, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read the module function bytecode length"
						": error='%s'"
						", streamOffset=%zu"
						", funcIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					output.functions.count
				);
				return false;
			}

			// Read the number of guarded blocks belonging to the function.
			uint32_t numGuardedBlocks = 0;
			if(!prv_readUint32(hSerializer, &numGuardedBlocks, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to read the module function's number of guarded blocks"
						": error='%s'"
						", streamOffset=%zu"
						", funcIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					output.functions.count
				);
				return false;
			}

			// Reserve space for each guarded block.
			Function::GuardedBlockArray::Reserve(func.guardedBlocks, numGuardedBlocks);

			// Iterate over each of the function's guarded blocks.
			for(; func.guardedBlocks.count < size_t(numGuardedBlocks); ++func.guardedBlocks.count)
			{
				Function::GuardedBlock& block = func.guardedBlocks.pData[func.guardedBlocks.count];

				// Initialize the guarded block's exception handler array.
				Function::GuardedBlock::ExceptionHandlerArray::Initialize(block.exceptionHandlers);

				// Read the guarded block's bytecode offset.
				if(!prv_readUint32(hSerializer, &block.offset, result, streamOffset))
				{
					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to read the module function guarded block bytecode offset"
							": error='%s'"
							", streamOffset=%zu"
							", funcIndex=%zu"
							", blockIndex=%zu",
						HqGetErrorCodeString(result),
						streamOffset,
						output.functions.count,
						func.guardedBlocks.count
					);
					return false;
				}

				// Read the guarded block's bytecode length.
				if(!prv_readUint32(hSerializer, &block.length, result, streamOffset))
				{
					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to read the module function guarded block bytecode length"
							": error='%s'"
							", streamOffset=%zu"
							", funcIndex=%zu"
							", blockIndex=%zu",
						HqGetErrorCodeString(result),
						streamOffset,
						output.functions.count,
						func.guardedBlocks.count
					);
					return false;
				}

				// Read the guarded block's number of exception handlers.
				uint32_t numExceptionHandlers = 0;
				if(!prv_readUint32(hSerializer, &numExceptionHandlers, result, streamOffset))
				{
					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to read the number of exception handlers for the module function guarded block"
							": error='%s'"
							", streamOffset=%zu"
							", funcIndex=%zu"
							", blockIndex=%zu",
						HqGetErrorCodeString(result),
						streamOffset,
						output.functions.count,
						func.guardedBlocks.count
					);
					return false;
				}

				// Reserve space for each exception handler.
				Function::GuardedBlock::ExceptionHandlerArray::Reserve(block.exceptionHandlers, numExceptionHandlers);

				for(; block.exceptionHandlers.count < size_t(numExceptionHandlers); ++block.exceptionHandlers.count)
				{
					Function::GuardedBlock::ExceptionHandler& handler = block.exceptionHandlers.pData[block.exceptionHandlers.count];

					handler.pClassName = nullptr;

					// Read the exception handler's bytecode offset.
					if(!prv_readUint32(hSerializer, &handler.offset, result, streamOffset))
					{
						HqReportMessage(
							hReport,
							HQ_MESSAGE_TYPE_ERROR,
							"Failed to read the handled type for the module exception handler"
								": error='%s'"
								", streamOffset=%zu"
								", funcIndex=%zu"
								", blockIndex=%zu"
								", handlerIndex=%zu",
							HqGetErrorCodeString(result),
							streamOffset,
							output.functions.count,
							func.guardedBlocks.count,
							block.exceptionHandlers.count
						);
						return false;
					}

					// Read the exception handler's handled value type.
					if(!prv_readUint32AsUint8(hSerializer, &handler.type, result, streamOffset))
					{
						HqReportMessage(
							hReport,
							HQ_MESSAGE_TYPE_ERROR,
							"Failed to read the handled type for the module exception handler"
								": error='%s'"
								", streamOffset=%zu"
								", funcIndex=%zu"
								", blockIndex=%zu"
								", handlerIndex=%zu",
							HqGetErrorCodeString(result),
							streamOffset,
							output.functions.count,
							func.guardedBlocks.count,
							block.exceptionHandlers.count
						);
						return false;
					}

					if(handler.type == HQ_VALUE_TYPE_OBJECT)
					{
						// Read the exception handler object type string.
						if(!prv_readStringFromIndex(output, hSerializer, &handler.pClassName, result, streamOffset))
						{
							HqReportMessage(
								hReport,
								HQ_MESSAGE_TYPE_ERROR,
								"Failed to read module exception handler typename string"
									": error='%s'"
									", streamOffset=%zu"
									", funcIndex=%zu"
									", blockIndex=%zu"
									", handlerIndex=%zu",
								HqGetErrorCodeString(result),
								streamOffset,
								output.functions.count,
								func.guardedBlocks.count,
								block.exceptionHandlers.count
							);
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadInitBytecode(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(output.contents.initBytecode.length == 0)
	{
		// No data to load.
		return true;
	}

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Initialize the initialization bytecode array.
	ByteArray::Initialize(output.initBytecode);

	// Move the stream position to the start of the module bytecode.
	result = HqSerializerSetStreamPosition(hSerializer, output.contents.initBytecode.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to set file stream position to module init function bytecode offset: error='%s', streamOffset=%" PRIu32, 
			HqGetErrorCodeString(result), 
			output.contents.initBytecode.offset
		);
		return false;
	}

	// Reserve enough space for the module init function bytecode.
	ByteArray::Reserve(output.initBytecode, output.contents.initBytecode.length);

	output.initBytecode.count = size_t(output.contents.initBytecode.length);

	// Read the module init function bytecode into the output buffer.
	if(!prv_readBuffer(hSerializer, output.initBytecode.count, output.initBytecode.pData, result, streamOffset))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to read module init function bytecode: error='%s', streamOffset=%zu",
			HqGetErrorCodeString(result),
			streamOffset
		);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_loadBytecode(HqModuleLoader& output, HqReportHandle hReport, HqSerializerHandle hSerializer)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(output.contents.bytecode.length == 0)
	{
		// No data to load.
		return true;
	}

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Initialize the bytecode array.
	ByteArray::Initialize(output.bytecode);

	// Move the stream position to the start of the module bytecode.
	result = HqSerializerSetStreamPosition(hSerializer, output.contents.bytecode.offset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to set file stream position to module bytecode offset: error='%s', streamOffset=%" PRIu32, 
			HqGetErrorCodeString(result), 
			output.contents.bytecode.offset
		);
		return false;
	}

	// Reserve enough space for the module bytecode.
	ByteArray::Reserve(output.bytecode, output.contents.bytecode.length);

	output.bytecode.count = size_t(output.contents.bytecode.length);

	// Read the module bytecode into the output buffer.
	if(!prv_readBuffer(hSerializer, output.bytecode.count, output.bytecode.pData, result, streamOffset))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to read module bytecode: error='%s', streamOffset=%zu",
			HqGetErrorCodeString(result),
			streamOffset
		);
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline void HqModuleLoader::prv_initialize(HqModuleLoader& output)
{
	memset(&output.fileHeader, 0, sizeof(output.fileHeader));
	memset(&output.contents, 0, sizeof(output.contents));

	StringArray::Initialize(output.strings);
	StringArray::Initialize(output.dependencies);
	StringArray::Initialize(output.globals);
	ObjectTypeArray::Initialize(output.objectTypes);
	FunctionArray::Initialize(output.functions);
	ByteArray::Initialize(output.bytecode);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_readBuffer(
	HqSerializerHandle hSerializer,
	const size_t bufferLength,
	void* const pOutBuffer,
	int& outResult,
	size_t& outOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(bufferLength > 0);
	assert(pOutBuffer != nullptr);

	outOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerReadBuffer(hSerializer, bufferLength, pOutBuffer);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_readString(
	HqSerializerHandle hSerializer,
	HqString** const ppOutString,
	int& outResult,
	size_t& outOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(ppOutString != nullptr);

	const size_t streamPosition = HqSerializerGetStreamPosition(hSerializer);
	const size_t streamLength = HqSerializerGetStreamLength(hSerializer);

	outOffset = streamPosition;

	if(streamPosition == streamLength)
	{
		outResult = HQ_ERROR_STREAM_END;
		return false;
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
		outResult = HQ_ERROR_BAD_ALLOCATION;
		return false;
	}

	// Because we hacked our way into the stream to get the string data,
	// we need to manually adjust the stream position ourselves. And we
	// add one here to account for the null-terminator.
	outResult = HqSerializerSetStreamPosition(hSerializer, streamPosition + pString->length + 1);
	if(outResult != HQ_SUCCESS)
	{
		HqString::Release(pString);
		return false;
	}

	(*ppOutString) = pString;

	return pString;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_readStringFromIndex(
	const HqModuleLoader& output,
	HqSerializerHandle hSerializer,
	HqString** const ppOutString,
	int& outResult,
	size_t& outOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(ppOutString != nullptr);

	// Read the string table index.
	uint32_t stringIndex = 0;
	if(!prv_readUint32(hSerializer, &stringIndex, outResult, outOffset))
	{
		return false;
	}

	// Verify the string index is within the correct bounds.
	if(stringIndex >= output.strings.count)
	{
		outResult = HQ_ERROR_INDEX_OUT_OF_RANGE;
		return false;
	}

	(*ppOutString) = output.strings.pData[stringIndex];

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_readBool8(
	HqSerializerHandle hSerializer, 
	bool* pOutValue, 
	int& outResult, 
	size_t& outOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pOutValue != nullptr);

	outOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerReadBool8(hSerializer, pOutValue);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_readBool32(
	HqSerializerHandle hSerializer, 
	bool* pOutValue, 
	int& outResult, 
	size_t& outOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pOutValue != nullptr);

	outOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerReadBool32(hSerializer, pOutValue);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_readUint32(
	HqSerializerHandle hSerializer,
	uint32_t* pOutValue,
	int& outResult,
	size_t& outOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pOutValue != nullptr);

	outOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerReadUint32(hSerializer, pOutValue);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_readUint32AsUint8(
	HqSerializerHandle hSerializer,
	uint8_t* pOutValue,
	int& outResult,
	size_t& outOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pOutValue != nullptr);

	uint32_t temp = 0;

	outOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerReadUint32(hSerializer, &temp);
	(*pOutValue) = uint8_t(temp);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleLoader::prv_readUint16(
	HqSerializerHandle hSerializer,
	uint16_t* pOutValue,
	int& outResult,
	size_t& outOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(pOutValue != nullptr);

	outOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerReadUint16(hSerializer, pOutValue);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------
