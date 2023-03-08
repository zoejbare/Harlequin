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

#include "ModuleWriter.hpp"

#include "Compiler.hpp"

#include <algorithm>
#include <assert.h>
#include <inttypes.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

HqModuleWriterHandle HqModuleWriter::Create()
{

	HqModuleWriter* const pOutput = new HqModuleWriter();
	return pOutput;
}

//----------------------------------------------------------------------------------------------------------------------

void HqModuleWriter::Dispose(HqModuleWriterHandle hModuleWriter)
{
	assert(hModuleWriter != HQ_MODULE_WRITER_HANDLE_NULL);

	// Dispose of all dependency names.
	for(HqString* const pDepName : hModuleWriter->dependencies)
	{
		HqString::Release(pDepName);
	}

	// Dispose of all module globals.
	for(HqString* const pVarName : hModuleWriter->globals)
	{
		HqString::Release(pVarName);
	}

	// Dispose of all module strings.
	for(auto& kv : hModuleWriter->stringIndices)
	{
		HqString::Release(kv.first);
	}

	// Dispose of all functions.
	for(auto& funcKv : hModuleWriter->functions)
	{
		HqString::Release(funcKv.first);

		for(HqString* pVarName : funcKv.second.locals)
		{
			HqString::Release(pVarName);
		}

		// Dispose of each guarded block.
		for(HqFunctionData::GuardedBlock& guardedBlock : funcKv.second.guardedBlocks)
		{
			// Dispose of each exception handler registered under this guarded block.
			for(auto& handlerKv : guardedBlock.handlers)
			{
				if(handlerKv.second.pClassName)
				{
					HqString::Release(handlerKv.second.pClassName);
				}
			}
		}
	}

	// Dispose of all object types.
	for(auto& typeKv : hModuleWriter->objectTypes)
	{
		HqString::Release(typeKv.second.pTypeName);

		// Dispose of all members belonging to the current object type.
		for(auto& memberKv : typeKv.second.members)
		{
			HqString::Release(memberKv.first);
		}
	}

	delete hModuleWriter;
}

//----------------------------------------------------------------------------------------------------------------------

bool HqModuleWriter::Serialize(
	HqModuleWriterHandle hModuleWriter,
	HqReportHandle hReport,
	HqSerializerHandle hSerializer
)
{
	assert(hModuleWriter != HQ_MODULE_WRITER_HANDLE_NULL);
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	auto getAlignedSize = [](const size_t size) -> size_t
	{
		// (size + (alignment - 1)) & ~(alignment - 1)
		return (size + 63) & ~63;
	};

	auto guardedBlockSortFunc = [](
		const HqFunctionData::GuardedBlock& left,
		const HqFunctionData::GuardedBlock& right
	) -> bool
	{
		if(left.offset < right.offset)
		{
			// Earlier guarded blocks are sorted before the blocks that come after them.
			return true;
		}
		else if(left.offset == right.offset)
		{
			// Nested blocks are sorted *after* the blocks that contain them.
			if(left.length > right.length)
			{
				return true;
			}
		}

		return false;
	};

	auto exceptionHandlerSortFunc = [](
		const HqFunctionData::ExceptionHandler& left,
		const HqFunctionData::ExceptionHandler& right
	) -> bool
	{
		return left.offset < right.offset;
	};

	// Sort the guarded blocks for each function.
	for(auto& kv : hModuleWriter->functions)
	{
		// Native functions don't use guarded blocks.
		if(!kv.second.isNative)
		{
			std::sort(
				kv.second.guardedBlocks.begin(),
				kv.second.guardedBlocks.end(),
				guardedBlockSortFunc
			);
		}
	}

	typedef std::unordered_map<size_t, HqString*> SizeToStringMap;

	// We needs maps for both string-to-index and index-to-string. This is so we can both build a set
	// of indicies for every string and iterate over the indices from start to end for each string.
	StringToSizeMap stringToIndexMap;
	SizeToStringMap indexToStringMap;

	StringToSizeMap functionOffsets;
	HqFunctionData::Bytecode bytecode;

	auto mapStringIndex = [&stringToIndexMap, &indexToStringMap](HqString* const pString)
	{
		if(stringToIndexMap.count(pString) == 0)
		{
			const size_t newIndex = stringToIndexMap.size();

			stringToIndexMap.emplace(pString, newIndex);
			indexToStringMap.emplace(newIndex, pString);
		}
	};

	// Copy the strings-to-indices map since it already contains the expected indices for all string constants.
	stringToIndexMap = hModuleWriter->stringIndices;

	// Fill out the indices-to-strings map from the existing string constant indicies.
	for(auto& kv : hModuleWriter->stringIndices)
	{
		indexToStringMap.emplace(kv.second, kv.first);
	}

	// Add the dependency names to the string index maps.
	for(HqString* const pDepName : hModuleWriter->dependencies)
	{
		mapStringIndex(pDepName);
	}

	// Add the global variable names to the string index maps.
	for(HqString* const pVarName : hModuleWriter->globals)
	{
		mapStringIndex(pVarName);
	}

	// Add the function signatures to the string index maps.
	for(auto& kv : hModuleWriter->functions)
	{
		mapStringIndex(kv.first);
	}

	// Calculate the aligned length of the init function bytecode.
	const size_t initFuncAlignedLength = getAlignedSize(hModuleWriter->initBytecode.size());

	// Get the function bindings and bytecode ready to be written out.
	{
		size_t bytecodeAlignedLength = 0;

		// Build the binding data for each function while calculating total length of the bytecode.
		for(auto& kv : hModuleWriter->functions)
		{
			if(!kv.second.isNative)
			{
				const size_t offset = bytecodeAlignedLength;
				const size_t length = kv.second.bytecode.size();

				// Align the bytecode length to add padding between each function.
				bytecodeAlignedLength += getAlignedSize(length);

				functionOffsets.emplace(kv.first, offset);
			}
		}

		// Allocate space for the entire block of bytecode for the module.
		bytecode.resize(bytecodeAlignedLength);

		// Clear the bytecode buffer.
		uint8_t* const pModuleBytecode = bytecode.data();
		memset(pModuleBytecode, 0, bytecode.size());

		// Fill out the module's final bytecode from each function's individual bytecode.
		for(auto& kv : hModuleWriter->functions)
		{
			if(!kv.second.isNative)
			{
				const void* const pFuncBytecode = kv.second.bytecode.data();
				const size_t length = kv.second.bytecode.size();
				const size_t offset = functionOffsets.at(kv.first);

				memcpy(pModuleBytecode + offset, pFuncBytecode, length);
			}
		}
	}

	HqModuleFileHeader fileHeader = {};
	HqModuleTableOfContents contents = {};

	// Initialize the module file header to set its properties to their default values.
	HqModuleFileHeader::Initialize(fileHeader);

	const int endianness = HqSerializerGetEndianness(hSerializer);

	// Set the big endian flag if endianness is being forced.
	switch(endianness)
	{
		case HQ_ENDIAN_ORDER_LITTLE:
			fileHeader.isBigEndian = false;
			break;

		case HQ_ENDIAN_ORDER_BIG:
			fileHeader.isBigEndian = true;
			break;

		default:
			break;
	}

	// Fill in the section lengths in the table of contents since they won't be changing.
	contents.stringTable.length = uint32_t(indexToStringMap.size());
	contents.dependencyTable.length = uint32_t(hModuleWriter->dependencies.size());
	contents.globalTable.length = uint32_t(hModuleWriter->globals.size());
	contents.objectTable.length = uint32_t(hModuleWriter->objectTypes.size());
	contents.functionTable.length = uint32_t(hModuleWriter->functions.size());
	contents.initBytecode.length = uint32_t(hModuleWriter->initBytecode.size());
	contents.bytecode.length = uint32_t(bytecode.size());

	int result = HQ_SUCCESS;
	size_t streamOffset = 0;

	// Write the file header.
	if(!prv_writeFileHeader(hSerializer, fileHeader, result, streamOffset))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to write module file header"
				": error='%s'"
				", streamOffset=%zu",
			HqGetErrorCodeString(result),
			streamOffset
		);

		return false;
	}

	const size_t tableOfContentsOffset = HqSerializerGetStreamPosition(hSerializer);

	// Write temporary data for the version header.
	// We'll come back to fill it out at the end.
	if(!prv_writeTableOfContents(hSerializer, contents, result, streamOffset))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to write module table of contents stub"
				": error='%s'"
				", streamOffset=%zu",
			HqGetErrorCodeString(result),
			streamOffset
		);

		return false;
	}

	// String table
	{
		// Cache the current stream position for the table of contents.
		contents.stringTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

		// Iterate over the module's strings.
		for(size_t index = 0; index < indexToStringMap.size(); ++index)
		{
			HqString* const pString = indexToStringMap.at(index);

			// Write the string data.
			if(!prv_writeString(hSerializer, pString->data, pString->length, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write string"
						": error='%s'"
						", streamOffset=%zu"
						", stringIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					index
				);
				return false;
			}
		}
	}

	// Dependency table
	{
		// Cache the current stream position for the table of contents.
		contents.dependencyTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

		// Iterate over the module's dependency.
		for(HqString* const pDepName : hModuleWriter->dependencies)
		{
			const size_t stringIndex = stringToIndexMap.at(pDepName);

			// Write the dependency name string index.
			if(!prv_writeUint32(hSerializer, uint32_t(stringIndex), result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write dependency string index"
						": error='%s'"
						", streamOffset=%zu"
						", depName='%s'"
						", stringIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					pDepName,
					stringIndex
				);
				return false;
			}
		}
	}

	// Global variable table
	{
		// Cache the current stream position for the table of contents.
		contents.globalTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

		// Iterate over the module's global variables.
		for(HqString* const pVarName : hModuleWriter->globals)
		{
			const size_t stringIndex = stringToIndexMap.at(pVarName);

			// Write the global variable string index.
			if(!prv_writeUint32(hSerializer, uint32_t(stringIndex), result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write global variable string index"
						": error='%s'"
						", streamOffset=%zu"
						", varName='%s'"
						", stringIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					pVarName->data,
					stringIndex
				);
				return false;
			}
		}
	}

	// Object table
	{
		// Cache the current stream position for the table of contents.
		contents.objectTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

		// Iterate over the module's object types.
		for(auto& typeKv : hModuleWriter->objectTypes)
		{
			HqString* const pTypeName = typeKv.first;

			const size_t typeStringIndex = stringToIndexMap.at(pTypeName);
			const size_t memberCount = typeKv.second.orderedMemberNames.size();

			// Write the type name string index.
			if(!prv_writeUint32(hSerializer, uint32_t(typeStringIndex), result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write object type name string index"
						": error='%s'"
						", streamOffset=%zu"
						", typeName='%s'"
						", stringIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					pTypeName->data,
					typeStringIndex
				);
				return false;
			}

			// Write the number of members belonging to the current object type.
			if(!prv_writeUint32(hSerializer, uint32_t(memberCount), result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write object type member count"
						": error='%s'"
						", streamOffset=%zu"
						", typeName='%s'"
						", memberCount=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					pTypeName->data,
					memberCount
				);
				return false;
			}

			// Iterate over each member in the current object type.
			for(size_t memberIndex = 0; memberIndex < typeKv.second.orderedMemberNames.size(); ++memberIndex)
			{
				HqString* const pMemberName = typeKv.second.orderedMemberNames[memberIndex];

				const size_t memberStringIndex = stringToIndexMap.at(pMemberName);
				const uint32_t memberType = uint32_t(typeKv.second.members.at(pMemberName));

				// Write the member name string index.
				if(!prv_writeUint32(hSerializer, uint32_t(memberStringIndex), result, streamOffset))
				{
					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to write object member name string index"
							": error='%s'"
							", streamOffset=%zu"
							", typeName='%s'"
							", memberName='%s'"
							", stringIndex=%zu",
						HqGetErrorCodeString(result),
						streamOffset,
						pTypeName->data,
						pMemberName->data,
						memberStringIndex
					);
					return false;
				}

				// Write the member value type.
				if(!prv_writeUint32(hSerializer, memberType, result, streamOffset))
				{
					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to write object member name"
							": error='%s'"
							", streamOffset=%zu"
							", typeName='%s'"
							", memberType='%s'",
						HqGetErrorCodeString(result),
						streamOffset,
						pTypeName->data,
						HqGetValueTypeString(memberType)
					);
					return false;
				}
			}
		}
	}

	// Function table
	{
		// Cache the current stream position for the table of contents.
		contents.functionTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

		// Iterate over the module's functions.
		for(auto& funcKv : hModuleWriter->functions)
		{
			HqString* const pFuncSig = funcKv.first;

			const size_t sigStringIndex = stringToIndexMap.at(pFuncSig);
			const uint16_t numInputs = funcKv.second.numParameters;
			const uint16_t numOutputs = funcKv.second.numReturnValues;
			const bool isNative = funcKv.second.isNative;

			// Write the function signature string index.
			if(!prv_writeUint32(hSerializer, uint32_t(sigStringIndex), result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write function signature string index"
						": error='%s'"
						", streamOffset=%zu"
						", signature='%s'"
						", stringIndex=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					pFuncSig->data,
					sigStringIndex
				);
				return false;
			}

			// Write the function's number of input values.
			if(!prv_writeUint16(hSerializer, numInputs, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write function input value count"
						": error='%s'"
						", streamOffset=%zu"
						", signature='%s'"
						", numInputs=%" PRIu16,
					HqGetErrorCodeString(result),
					streamOffset,
					pFuncSig->data,
					numInputs
				);
				return false;
			}

			// Write the function's number of output values.
			if(!prv_writeUint16(hSerializer, numOutputs, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write function input value count"
						": error='%s'"
						", streamOffset=%zu"
						", signature='%s'"
						", numOutputs=%" PRIu16,
					HqGetErrorCodeString(result),
					streamOffset,
					pFuncSig->data,
					numOutputs
				);
				return false;
			}

			// Write the function's 'isNative' flag.
			if(!prv_writeBool32(hSerializer, isNative, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write function 'isNative' flag"
						": error='%s'"
						", streamOffset=%zu"
						", signature='%s'"
						", isNative=%s",
					HqGetErrorCodeString(result),
					streamOffset,
					pFuncSig->data,
					isNative ? "true" : "false"
				);
				return false;
			}

			if(!isNative)
			{
				const size_t funcOffset = functionOffsets.at(pFuncSig);
				const size_t funcLength = funcKv.second.bytecode.size();
				const size_t numGuardedBlocks = funcKv.second.guardedBlocks.size();

				// Write the function's bytecode offset.
				if(!prv_writeUint32(hSerializer, uint32_t(funcOffset), result, streamOffset))
				{
					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to write function's bytecode offset"
							": error='%s'"
							", streamOffset=%zu"
							", signature='%s'"
							", bytecodeOffset=%zu",
						HqGetErrorCodeString(result),
						streamOffset,
						pFuncSig->data,
						funcOffset
					);
					return false;
				}

				// Write the function's bytecode length.
				if(!prv_writeUint32(hSerializer, uint32_t(funcLength), result, streamOffset))
				{
					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to write function's bytecode length"
							": error='%s'"
							", streamOffset=%zu"
							", signature='%s'"
							", length=%zu",
						HqGetErrorCodeString(result),
						streamOffset,
						pFuncSig->data,
						funcLength
					);
					return false;
				}

				// Write the function's number of guarded blocks.
				if(!prv_writeUint32(hSerializer, uint32_t(numGuardedBlocks), result, streamOffset))
				{
					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to write function's number of guarded blocks"
							": error='%s'"
							", streamOffset=%zu"
							", signature='%s'"
							", blockCount=%zu",
						HqGetErrorCodeString(result),
						streamOffset,
						pFuncSig->data,
						numGuardedBlocks
					);
					return false;
				}

				// Iterate over the current function's guarded blocks.
				for(size_t blockIndex = 0; blockIndex < funcKv.second.guardedBlocks.size(); ++blockIndex)
				{
					const HqFunctionData::GuardedBlock& guardedBlock = funcKv.second.guardedBlocks.at(blockIndex);
					const size_t numExceptionHandlers = guardedBlock.handlers.size();

					HqFunctionData::ExceptionHandler::Vector exceptionHandlers;
					if(numExceptionHandlers > 0)
					{
						// Preallocate space for each exception handler.
						exceptionHandlers.reserve(numExceptionHandlers);

						// Transfer each exception handler in the map to a flat array.
						for(auto& excKv : guardedBlock.handlers)
						{
							exceptionHandlers.push_back(excKv.second);
						}

						// Sort the flat array of exception handlers for the current guarded block.
						std::sort(
							exceptionHandlers.begin(),
							exceptionHandlers.end(),
							exceptionHandlerSortFunc
						);
					}

					// Calculate the offset from the start of the bytecode for this guarded block.
					const uint32_t blockOffset = uint32_t(funcOffset) + guardedBlock.offset;
					const uint32_t blockLength = guardedBlock.length;

					// Write the guarded block's bytecode offset.
					if(!prv_writeUint32(hSerializer, blockOffset, result, streamOffset))
					{
						HqReportMessage(
							hReport,
							HQ_MESSAGE_TYPE_ERROR,
							"Failed to write guarded block's bytecode offset"
								": error='%s'"
								", streamOffset=%zu"
								", signature='%s'"
								", blockIndex=%zu"
								", bytecodeOffset=%" PRIu32,
							HqGetErrorCodeString(result),
							streamOffset,
							pFuncSig->data,
							blockIndex,
							blockOffset
						);
						return false;
					}

					// Write the guarded block's bytecode length.
					if(!prv_writeUint32(hSerializer, blockLength, result, streamOffset))
					{
						HqReportMessage(
							hReport,
							HQ_MESSAGE_TYPE_ERROR,
							"Failed to write guarded block's bytecode length"
								": error='%s'"
								", streamOffset=%zu"
								", signature='%s'"
								", blockIndex=%zu"
								", length=%" PRIu32,
							HqGetErrorCodeString(result),
							streamOffset,
							pFuncSig->data,
							blockIndex,
							blockLength
						);
						return false;
					}

					// Write the guarded block's number of exception handlers.
					if(!prv_writeUint32(hSerializer, uint32_t(numExceptionHandlers), result, streamOffset))
					{
						HqReportMessage(
							hReport,
							HQ_MESSAGE_TYPE_ERROR,
							"Failed to write guarded block's bytecode length"
								": error='%s'"
								", streamOffset=%zu"
								", signature='%s'"
								", blockIndex=%zu"
								", handlerCount=%zu",
							HqGetErrorCodeString(result),
							streamOffset,
							pFuncSig->data,
							blockIndex,
							numExceptionHandlers
						);
						return false;
					}

					for(size_t handlerIndex = 0; handlerIndex < numExceptionHandlers; ++handlerIndex)
					{
						const HqFunctionData::ExceptionHandler& handler = exceptionHandlers.at(handlerIndex);

						// Calculate the offset from the start of the bytecode for this exception handler.
						const uint32_t handlerOffset = uint32_t(funcOffset) + handler.offset;
						const uint32_t handledType = uint32_t(handler.type);

						// Write the exception handler's bytecode offset.
						if(!prv_writeUint32(hSerializer, handlerOffset, result, streamOffset))
						{
							HqReportMessage(
								hReport,
								HQ_MESSAGE_TYPE_ERROR,
								"Failed to write exception handler's bytecode offset"
									": error='%s'"
									", streamOffset=%zu"
									", signature='%s'"
									", blockIndex=%zu"
									", handlerIndex=%zu"
									", bytecodeOffset=%" PRIu32,
								HqGetErrorCodeString(result),
								streamOffset,
								pFuncSig->data,
								blockIndex,
								handlerIndex,
								handlerOffset
							);
							return false;
						}

						// Write the exception handler's handled type.
						if(!prv_writeUint32(hSerializer, handledType, result, streamOffset))
						{
							HqReportMessage(
								hReport,
								HQ_MESSAGE_TYPE_ERROR,
								"Failed to write exception handler's handled type"
									": error='%s'"
									", streamOffset=%zu"
									", signature='%s'"
									", blockIndex=%zu"
									", handlerIndex=%zu"
									", type='%s'",
								HqGetErrorCodeString(result),
								streamOffset,
								pFuncSig->data,
								blockIndex,
								handlerIndex,
								HqGetValueTypeString(handledType)
							);
							return false;
						}

						if(handledType == HQ_VALUE_TYPE_OBJECT)
						{
							const size_t typeStringIndex = stringToIndexMap.at(handler.pClassName);

							// Write the string index for the class name handled by the current exception handler.
							if(!prv_writeUint32(hSerializer, handledType, result, streamOffset))
							{
								HqReportMessage(
									hReport,
									HQ_MESSAGE_TYPE_ERROR,
									"Failed to write exception handler's class name string index"
										": error='%s'"
										", streamOffset=%zu"
										", signature='%s'"
										", blockIndex=%zu"
										", handlerIndex=%zu"
										", className='%s'"
										", stringIndex=%zu",
									HqGetErrorCodeString(result),
									streamOffset,
									pFuncSig->data,
									blockIndex,
									handlerIndex,
									handler.pClassName->data,
									typeStringIndex
								);
								return false;
							}
						}
					}
				}
			}
		}
	}

	// Module init function bytecode
	{
		// Cache the current stream position for the table of contents.
		contents.initBytecode.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

		const size_t initFunctionLength = hModuleWriter->initBytecode.size();

		// Write the module's init function bytecode.
		if(!prv_writeBuffer(hSerializer, hModuleWriter->initBytecode.data(), initFunctionLength, result, streamOffset))
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to write module init function bytecode"
					": error='%s'"
					", streamOffset=%zu"
					", length=%zu",
				HqGetErrorCodeString(result),
				streamOffset,
				initFunctionLength
			);

			return false;
		}

		const size_t totalPadding = initFuncAlignedLength - initFunctionLength;

		// Write the required padding after the init function so the next section is aligned.
		for(size_t paddingIndex = 0; paddingIndex < totalPadding; ++paddingIndex)
		{
			if(!prv_writeUint8(hSerializer, 0, result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write padding to the end of the module init function"
						": error='%s'"
						", streamOffset=%zu"
						", paddingIndex=%zu/%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					paddingIndex,
					totalPadding
				);

				return false;
			}
		}
	}

	// General bytecode
	{
		// Cache the current stream position for the table of contents.
		contents.bytecode.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

		if(bytecode.size() > 0)
		{
			// Write the module's bytecode.
			if(!prv_writeBuffer(hSerializer, bytecode.data(), bytecode.size(), result, streamOffset))
			{
				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to write module bytecode"
						": error='%s'"
						", streamOffset=%zu"
						", length=%zu",
					HqGetErrorCodeString(result),
					streamOffset,
					bytecode.size()
				);

				return false;
			}
		}
	}

	const size_t fileEndOffset = HqSerializerGetStreamPosition(hSerializer);

	// Move back to the table of contents so we can write the real data for it.
	result = HqSerializerSetStreamPosition(hSerializer, tableOfContentsOffset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to move serializer position to the start of the module table of contents"
				": error='%s'"
				", streamOffset=%zu",
			HqGetErrorCodeString(result),
			tableOfContentsOffset
		);

		return false;
	}

	// Write the real table contents.
	if(!prv_writeTableOfContents(hSerializer, contents, result, streamOffset))
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to write module table of contents"
				": error='%s'"
				", streamOffset=%zu",
			HqGetErrorCodeString(result),
			streamOffset
		);

		return false;
	}

	// Return to the end of the file stream.
	result = HqSerializerSetStreamPosition(hSerializer, fileEndOffset);
	if(result != HQ_SUCCESS)
	{
		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to move serializer position to the end of the file stream"
				": error='%s'"
				", streamOffset=%zu",
			HqGetErrorCodeString(result),
			fileEndOffset
		);

		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------

int HqModuleWriter::LookupFunction(
	HqModuleWriterHandle hWriter,
	const char* const functionSignature,
	HqFunctionData** const ppOutFunction
)
{
	if(!hWriter || !functionSignature || functionSignature[0] == '\0')
	{
		return HQ_ERROR_INVALID_ARG;
	}

	HqString* const pSignature = HqString::Create(functionSignature);
	if(!pSignature)
	{
		return HQ_ERROR_BAD_ALLOCATION;
	}

	// Find the desired function by checking what we currently have mapped.
	auto kv = hWriter->functions.find(pSignature);
	if(kv == hWriter->functions.end())
	{
		HqString::Release(pSignature);
		return HQ_ERROR_KEY_DOES_NOT_EXIST;
	}

	// The string object for the function signature is no longer needed.
	HqString::Release(pSignature);

	(*ppOutFunction) = &kv->second;

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------

uint32_t HqModuleWriter::AddString(HqModuleWriterHandle hWriter, HqString* const pValue)
{
	assert(hWriter != HQ_MODULE_WRITER_HANDLE_NULL);
	assert(pValue != nullptr);

	auto kv = hWriter->stringIndices.find(pValue);
	if(kv != hWriter->stringIndices.end())
	{
		return uint32_t(kv->second);
	}

	const uint32_t output = uint32_t(hWriter->stringConstants.size());

	HqString::AddRef(pValue);

	hWriter->stringIndices.emplace(pValue, output);
	hWriter->stringConstants.push_back(pValue);

	return output;
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeFileHeader(
	HqSerializerHandle hSerializer,
	const HqModuleFileHeader& fileHeader,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.magicNumber[0], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.magicNumber[1], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.magicNumber[2], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.magicNumber[3], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[0], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[1], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[2], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[3], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[4], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[5], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[6], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[7], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[8], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[9], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint8(hSerializer, fileHeader.reserved[10], outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeBool8(hSerializer, fileHeader.isBigEndian, outResult, outStreamOffset); }

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeTableOfContents(
	HqSerializerHandle hSerializer,
	const HqModuleTableOfContents& contents,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.stringTable.offset, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.stringTable.length, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.dependencyTable.offset, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.dependencyTable.length, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.globalTable.offset, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.globalTable.length, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.objectTable.offset, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.objectTable.length, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.functionTable.offset, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.functionTable.length, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.initBytecode.offset, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.initBytecode.length, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.bytecode.offset, outResult, outStreamOffset); }
	if(outResult == HQ_SUCCESS) { prv_writeUint32(hSerializer, contents.bytecode.length, outResult, outStreamOffset); }

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeBuffer(
	HqSerializerHandle hSerializer,
	const void* const pBuffer,
	const size_t length,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	outStreamOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerWriteBuffer(hSerializer, length, pBuffer);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeString(
	HqSerializerHandle hSerializer,
	const char* const stringData,
	const size_t stringLength,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	if(stringLength > 0)
	{
		assert(stringData != nullptr);

		outStreamOffset = HqSerializerGetStreamPosition(hSerializer);
		outResult = HqSerializerWriteBuffer(hSerializer, stringLength, stringData);

		if(outResult != HQ_SUCCESS)
		{
			return false;
		}
	}

	// Always write the null-terminator for strings.
	outStreamOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerWriteUint8(hSerializer, 0);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeUint8(
	HqSerializerHandle hSerializer,
	const uint8_t value,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	outStreamOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerWriteUint8(hSerializer, value);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeUint16(
	HqSerializerHandle hSerializer,
	const uint16_t value,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	outStreamOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerWriteUint16(hSerializer, value);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeUint32(
	HqSerializerHandle hSerializer,
	const uint32_t value,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	outStreamOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerWriteUint32(hSerializer, value);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeBool8(
	HqSerializerHandle hSerializer,
	const bool value,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	outStreamOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerWriteBool8(hSerializer, value);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

inline bool HqModuleWriter::prv_writeBool32(
	HqSerializerHandle hSerializer,
	const bool value,
	int& outResult,
	size_t& outStreamOffset
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	outStreamOffset = HqSerializerGetStreamPosition(hSerializer);
	outResult = HqSerializerWriteBool32(hSerializer, value);

	return (outResult == HQ_SUCCESS);
}

//----------------------------------------------------------------------------------------------------------------------

void* HqModuleWriter::operator new(const size_t sizeInBytes)
{
	return HqMemAlloc(sizeInBytes);
}

//----------------------------------------------------------------------------------------------------------------------

void HqModuleWriter::operator delete(void* const pObject)
{
	HqMemFree(pObject);
}

//----------------------------------------------------------------------------------------------------------------------
