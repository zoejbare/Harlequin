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

#include "../common/module-format/FileHeader.hpp"
#include "../common/module-format/ModuleHeader.hpp"

#include <algorithm>
#include <assert.h>
#include <inttypes.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------

static bool SerializeString(
	HqSerializerHandle hSerializer,
	HqReportHandle hReport,
	const char* const stringData,
	const size_t stringLength
)
{
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);
	assert(hReport != HQ_REPORT_HANDLE_NULL);

	int result = 0;

	if(stringLength > 0)
	{
		assert(stringData != nullptr);

		result = HqSerializerWriteBuffer(hSerializer, stringLength, stringData);
		if(result != HQ_SUCCESS)
		{
			const char* const errorString = HqGetErrorCodeString(result);

			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to write string data: error=\"%s\", data=\"%s\"",
				errorString,
				stringData
			);

			return false;
		}
	}

	// Always write the null-terminator for strings.
	result = HqSerializerWriteUint8(hSerializer, 0);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to write string null-terminator: error=\"%s\"",
			errorString
		);

		return false;
	}

	return true;
}

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
	for(auto& kv : hModuleWriter->dependencies)
	{
		HqString::Release(kv.first);
	}

	// Dispose of all module globals.
	for(HqString* const pVarName : hModuleWriter->globals)
	{
		HqString::Release(pVarName);
	}

	// Dispose of all module strings.
	for(auto& kv : hModuleWriter->stringIndexMap)
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
	HqCompilerHandle hCompiler,
	HqSerializerHandle hSerializer
)
{
	assert(hModuleWriter != HQ_MODULE_WRITER_HANDLE_NULL);
	assert(hCompiler != HQ_COMPILER_HANDLE_NULL);
	assert(hSerializer != HQ_SERIALIZER_HANDLE_NULL);

	HqReportHandle hReport = &hCompiler->report;

	HqFileHeader fileHeader = {};
	HqModuleHeader moduleHeader = {};

	fileHeader.magicNumber[0] = 'H';
	fileHeader.magicNumber[1] = 'Q';
	fileHeader.magicNumber[2] = 'P';
	fileHeader.magicNumber[3] = 'R';
	fileHeader.magicNumber[4] = 'G';

	const int endianness = HqSerializerGetEndianness(hSerializer);

	// Set the big endian flag.
	switch(endianness)
	{
		case HQ_ENDIAN_ORDER_LITTLE:
			fileHeader.bigEndianFlag = 0;
			break;

		case HQ_ENDIAN_ORDER_BIG:
			fileHeader.bigEndianFlag = 1;
			break;

		default:
#ifdef HQ_CPU_ENDIAN_LITTLE
			fileHeader.bigEndianFlag = 0;
#else
			fileHeader.bigEndianFlag = 1;
#endif
			break;
	}

	auto getAlignedSize = [](const size_t size) -> size_t
	{
		// (size + (alignment - 1)) & ~(alignment - 1)
		return (size + 63) & ~63;
	};

	struct FunctionBinding
	{
		HqFunctionData* pFunction;
		HqString* pSignature;

		uint32_t offset;
		uint32_t length;
	};

	std::deque<FunctionBinding> functionBindings;
	HqFunctionData::Bytecode bytecode;

	// Get the function bindings and bytecode ready to be written out.
	{
		// The module's init function will always be at the start of the bytecode.
		// All other functions will be defined after it.
		uint32_t bytecodeLength = uint32_t(getAlignedSize(hModuleWriter->initBytecode.size()));

		// Build the binding data for each function while calculating total length of the bytecode.
		for(auto& kv : hModuleWriter->functions)
		{
			FunctionBinding binding;

			binding.pFunction = &kv.second;
			binding.pSignature = kv.first;
			binding.offset = uint32_t(bytecodeLength);
			binding.length = uint32_t(binding.pFunction->bytecode.size());

			// Align the bytecode length to add padding between each function.
			bytecodeLength += uint32_t(getAlignedSize(binding.length));

			functionBindings.push_back(binding);
		}

		// Allocate space for the entire block of bytecode for the module.
		bytecode.resize(bytecodeLength);

		// Clear the bytecode buffer.
		uint8_t* const pModuleBytecode = bytecode.data();
		memset(pModuleBytecode, 0, bytecode.size());

		// Copy the module's init function to the start of the bytecode.
		memcpy(bytecode.data(), hModuleWriter->initBytecode.data(), hModuleWriter->initBytecode.size());


		// Fill out the full module bytecode from each function's individual bytecode.
		for(const FunctionBinding& binding : functionBindings)
		{
			memcpy(
				pModuleBytecode + binding.offset,
				binding.pFunction->bytecode.data(),
				binding.pFunction->bytecode.size()
			);
		}
	}

	moduleHeader.dependencyTable.length = uint32_t(hModuleWriter->dependencies.size());
	moduleHeader.objectTable.length = uint32_t(hModuleWriter->objectTypes.size());
	moduleHeader.stringTable.length = uint32_t(hModuleWriter->strings.size());
	moduleHeader.globalTable.length = uint32_t(hModuleWriter->globals.size());
	moduleHeader.bytecode.length = uint32_t(bytecode.size());
	moduleHeader.functionTable.length = uint32_t(functionBindings.size());
	moduleHeader.initFunctionLength = uint32_t(hModuleWriter->initBytecode.size());

	// TODO: Add support for module extensions.
	moduleHeader.extensionTable.offset = 0;
	moduleHeader.extensionTable.length = 0;

	int result = HQ_SUCCESS;

	// Write the common header.
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.magicNumber[0]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.magicNumber[1]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.magicNumber[2]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.magicNumber[3]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.magicNumber[4]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[0]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[1]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[2]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[3]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[4]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[5]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[6]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[7]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[8]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.reserved[9]); }
	if(result == HQ_SUCCESS) { result = HqSerializerWriteUint8(hSerializer, fileHeader.bigEndianFlag); }

	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to write module common header: error=\"%s\"",
			errorString
		);

		return false;
	}

	auto writeVersionHeader = [&hSerializer, &moduleHeader]() -> int
	{
		int result = HQ_SUCCESS;

		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.dependencyTable.offset); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.dependencyTable.length); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.objectTable.offset); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.objectTable.length); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.stringTable.offset); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.stringTable.length); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.globalTable.offset); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.globalTable.length); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.functionTable.offset); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.functionTable.length); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.extensionTable.offset); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.extensionTable.length); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.bytecode.offset); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.bytecode.length); }
		if(result == HQ_SUCCESS) { result = HqSerializerWriteUint32(hSerializer, moduleHeader.initFunctionLength); }

		return result;
	};

	const size_t versionHeaderPosition = HqSerializerGetStreamPosition(hSerializer);

	// Write temporary data for the version header.
	// We'll come back to fill it out at the end.
	result = writeVersionHeader();

	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to write module version header data: error=\"%s\"",
			errorString
		);

		return false;
	}

	moduleHeader.dependencyTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

	// Write the dependency table.
	for(auto& kv : hModuleWriter->dependencies)
	{
		HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Serializing dependency: name=\"%s\"", kv.first->data);

		if(!SerializeString(hSerializer, hReport, kv.first->data, kv.first->length))
		{
			return false;
		}
	}

	moduleHeader.objectTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

	// Write the object type schemas.
	for(auto& typeKv : hModuleWriter->objectTypes)
	{
		HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Serializing object type: name=\"%s\"", typeKv.first->data);

		if(!SerializeString(hSerializer, hReport, typeKv.first->data, typeKv.first->length))
		{
			return false;
		}

		const uint32_t memberCount = uint32_t(typeKv.second.orderedMemberNames.size());

		// Write the number of members belonging to this object.
		result = HqSerializerWriteUint32(hSerializer, memberCount);
		if(result != HQ_SUCCESS)
		{
			const char* const errorString = HqGetErrorCodeString(result);

			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to serialize object member count: error=\"%s\", objectType=\"%s\", memberCount=%" PRIu32,
				errorString,
				typeKv.first->data,
				memberCount
			);

			return false;
		}

		for(size_t memberIndex = 0; memberIndex < typeKv.second.orderedMemberNames.size(); ++memberIndex)
		{
			HqString* const pMemberName = typeKv.second.orderedMemberNames[memberIndex];
			const uint8_t memberValueType = uint8_t(typeKv.second.members[pMemberName]);

			const char* const memberTypeString = HqGetValueTypeString(memberValueType);

			HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, " - Serializing object member: name=\"%s\", type=%s" , pMemberName->data, memberTypeString);

			// Write the member name string.
			if(!SerializeString(hSerializer, hReport, pMemberName->data, pMemberName->length))
			{
				return false;
			}

			// Write the member value type.
			result = HqSerializerWriteUint8(hSerializer, uint8_t(memberValueType));
			if(result != HQ_SUCCESS)
			{
				const char* const errorString = HqGetErrorCodeString(result);

				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to serialize object member type: error=\"%s\", objectType=\"%s\", memberName=\"%s\", memberType=%s",
					errorString,
					typeKv.first->data,
					pMemberName->data,
					memberTypeString
				);

				return false;
			}
		}
	}

	moduleHeader.stringTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

	// Write the constant table.
	for(size_t index = 0; index < hModuleWriter->strings.size(); ++index)
	{
		HqString* const pString = hModuleWriter->strings[index];

		HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Serializing string: index=%" PRIuPTR ", data=\"%s\"", index, pString->data);
		
		if(!SerializeString(hSerializer, hReport, pString->data, pString->length))
		{
			return false;
		}
	}

	moduleHeader.globalTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

	// Write the global variable table.
	for(HqString* const pVarName : hModuleWriter->globals)
	{
		HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, "Serializing global variable: name=\"%s\"", pVarName->data);

		// First, write the string key of the global.
		if(!SerializeString(hSerializer, hReport, pVarName->data, pVarName->length))
		{
			return false;
		}
	}

	moduleHeader.functionTable.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

	// Write the function table.
	for(const FunctionBinding& binding : functionBindings)
	{
		if(binding.pFunction->isNative)
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_VERBOSE,
				"Serializing native function: signature=\"%s\", numParams=%" PRIu16 ", numReturnValues=%" PRIu16,
				binding.pSignature->data,
				binding.pFunction->numParameters,
				binding.pFunction->numReturnValues
			);
		}
		else
		{
			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_VERBOSE,
				"Serializing script function: signature=\"%s\", numParams=%" PRIu16
				", numReturnValues=%" PRIu16 ", offset=0x%" PRIX32 ", length=%" PRIu32,
				binding.pSignature->data,
				binding.pFunction->numParameters,
				binding.pFunction->numReturnValues,
				binding.offset,
				binding.length
			);
		}

		// Write the function signature.
		if(!SerializeString(hSerializer, hReport, binding.pSignature->data, binding.pSignature->length))
		{
			return false;
		}

		// Write the function's native switch into the module file.
		result = HqSerializerWriteBool(hSerializer, binding.pFunction->isNative);
		if(result != HQ_SUCCESS)
		{
			const char* const errorString = HqGetErrorCodeString(result);

			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to serialize function 'isNative' flag: error=\"%s\", signature=\"%s\", native=%s",
				errorString,
				binding.pSignature->data,
				binding.pFunction->isNative ? "true" : "false"
			);

			return false;
		}

		// Write the function's parameter count into the module file.
		result = HqSerializerWriteUint16(hSerializer, binding.pFunction->numParameters);
		if(result != HQ_SUCCESS)
		{
			const char* const errorString = HqGetErrorCodeString(result);

			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to serialize function parameter count: error=\"%s\", signature=\"%s\", count=%" PRIu16,
				errorString,
				binding.pSignature->data,
				binding.pFunction->numParameters
			);

			return false;
		}

		// Write the function's return value count into the module file.
		result = HqSerializerWriteUint16(hSerializer, binding.pFunction->numReturnValues);
		if(result != HQ_SUCCESS)
		{
			const char* const errorString = HqGetErrorCodeString(result);

			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to serialize function return value count: error=\"%s\", signature=\"%s\", count=%" PRIu16,
				errorString,
				binding.pSignature->data,
				binding.pFunction->numReturnValues
			);

			return false;
		}

		if(!binding.pFunction->isNative)
		{
			// Write the function's offset into the module file.
			result = HqSerializerWriteUint32(hSerializer, binding.offset);
			if(result != HQ_SUCCESS)
			{
				const char* const errorString = HqGetErrorCodeString(result);

				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to serialize function offset: error=\"%s\", signature=\"%s\", offset=0x%" PRIX32,
					errorString,
					binding.pSignature->data,
					binding.offset
				);

				return false;
			}

			// Write the function's length in bytes into the module file.
			result = HqSerializerWriteUint32(hSerializer, binding.length);
			if(result != HQ_SUCCESS)
			{
				const char* const errorString = HqGetErrorCodeString(result);

				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to serialize function offset: error=\"%s\", signature=\"%s\", length=%" PRIu32,
					errorString,
					binding.pSignature->data,
					binding.length
				);

				return false;
			}

			// Write the function's local variable count.
			result = HqSerializerWriteUint32(hSerializer, uint32_t(binding.pFunction->locals.size()));
			if(result != HQ_SUCCESS)
			{
				const char* const errorString = HqGetErrorCodeString(result);

				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to serialize function local variable count: error=\"%s\", signature=\"%s\", count=%" PRIu32,
					errorString,
					binding.pSignature->data,
					uint32_t(binding.pFunction->locals.size())
				);

				return false;
			}

			// Write the function's local variable table.
			for(const HqString* const pVarName : binding.pFunction->locals)
			{
				HqReportMessage(hReport, HQ_MESSAGE_TYPE_VERBOSE, " - Serializing local variable: name=\"%s\"", pVarName->data);

				// First, write the string key of the local.
				if(!SerializeString(hSerializer, hReport, pVarName->data, pVarName->length))
				{
					return false;
				}
			}

			const size_t guardedBlockCount = uint32_t(binding.pFunction->guardedBlocks.size());

			// Write the function's guarded block count into the module file.
			result = HqSerializerWriteUint32(hSerializer, uint32_t(guardedBlockCount));
			if(result != HQ_SUCCESS)
			{
				const char* const errorString = HqGetErrorCodeString(result);

				HqReportMessage(
					hReport,
					HQ_MESSAGE_TYPE_ERROR,
					"Failed to serialize function guarded block count: error=\"%s\", signature=\"%s\", count=%zu",
					errorString,
					binding.pSignature->data,
					guardedBlockCount
				);

				return false;
			}

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

			// Sort the guarded blocks for this function.
			std::sort(
				binding.pFunction->guardedBlocks.begin(),
				binding.pFunction->guardedBlocks.end(),
				guardedBlockSortFunc
			);

			// Serialize the guarded blocks with their exception handlers.
			for(const HqFunctionData::GuardedBlock& block : binding.pFunction->guardedBlocks)
			{
				HqFunctionData::ExceptionHandler::Vector exceptionHandlers;
				exceptionHandlers.reserve(block.handlers.size());

				// Build a flat array of exception handlers for this guarded block.
				for(auto& kv : block.handlers)
				{
					exceptionHandlers.push_back(kv.second);
				}

				// Sort the array of exception handlers.
				std::sort(exceptionHandlers.begin(), exceptionHandlers.end(), exceptionHandlerSortFunc);

				// Calculate the offset from the start of the bytecode for this guarded block.
				const uint32_t blockOffset = binding.offset + block.offset;

				// Write the guarded block bytecode offset.
				result = HqSerializerWriteUint32(hSerializer, blockOffset);
				if(result != HQ_SUCCESS)
				{
					const char* const errorString = HqGetErrorCodeString(result);

					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to write guarded block bytecode offset: error=\"%s\", signature=\"%s\", offset=%" PRIu32,
						errorString,
						binding.pSignature->data,
						block.offset
					);

					return false;
				}

				// Write the guarded block bytecode length.
				result = HqSerializerWriteUint32(hSerializer, block.length);
				if(result != HQ_SUCCESS)
				{
					const char* const errorString = HqGetErrorCodeString(result);

					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to write guarded block bytecode length: error=\"%s\", signature=\"%s\", length=%" PRIu32,
						errorString,
						binding.pSignature->data,
						block.length
					);

					return false;
				}

				const uint32_t exceptionHandlerCount = uint32_t(exceptionHandlers.size());

				// Write the number of exception handlers contained in this guarded block.
				result = HqSerializerWriteUint32(hSerializer, exceptionHandlerCount);
				if(result != HQ_SUCCESS)
				{
					const char* const errorString = HqGetErrorCodeString(result);

					HqReportMessage(
						hReport,
						HQ_MESSAGE_TYPE_ERROR,
						"Failed to write guarded block exception handler count: error=\"%s\", signature=\"%s\", count=%" PRIu32,
						errorString,
						binding.pSignature->data,
						exceptionHandlerCount
					);

					return false;
				}

				// Serialize each exception handler contained by this guarded block.
				for(uint32_t handlerIndex = 0; handlerIndex < exceptionHandlerCount; ++handlerIndex)
				{
					const HqFunctionData::ExceptionHandler& handler = exceptionHandlers[handlerIndex];

					// Write the value type for this exception handler.
					result = HqSerializerWriteUint8(hSerializer, uint8_t(handler.type));
					if(result != HQ_SUCCESS)
					{
						const char* const errorString = HqGetErrorCodeString(result);
						const char* const valueTypeString = HqGetValueTypeString(handler.type);

						HqReportMessage(
							hReport,
							HQ_MESSAGE_TYPE_ERROR,
							"Failed to write exception handler type: error=\"%s\", signature=\"%s\", type=\"%s\"",
							errorString,
							binding.pSignature->data,
							valueTypeString
						);

						return false;
					}

					// Calculate the offset from the start of the bytecode for this exception handler.
					const uint32_t handlerOffset = binding.offset + handler.offset;

					// Write the offset where this exception handler is located.
					result = HqSerializerWriteUint32(hSerializer, handlerOffset);
					if(result != HQ_SUCCESS)
					{
						const char* const errorString = HqGetErrorCodeString(result);

						HqReportMessage(
							hReport,
							HQ_MESSAGE_TYPE_ERROR,
							"Failed to write exception handler offset: error=\"%s\", signature=\"%s\", offset=%" PRIu32,
							errorString,
							binding.pSignature->data,
							handler.offset
						);

						return false;
					}

					if(handler.type == HQ_VALUE_TYPE_OBJECT)
					{
						// Write the class name if this exception handler references an object type.
						if(!SerializeString(hSerializer, hReport, handler.pClassName->data, handler.pClassName->length))
						{
							return false;
						}
					}
				}
			}
		}
	}

	moduleHeader.bytecode.offset = uint32_t(HqSerializerGetStreamPosition(hSerializer));

	if(bytecode.size() > 0)
	{
		// Write the bytecode.
		result = HqSerializerWriteBuffer(hSerializer, bytecode.size(), bytecode.data());
		if(result != HQ_SUCCESS)
		{
			const char* const errorString = HqGetErrorCodeString(result);

			HqReportMessage(
				hReport,
				HQ_MESSAGE_TYPE_ERROR,
				"Failed to write module bytecode buffer: error=\"%s\"",
				errorString
			);

			return false;
		}
	}

	const size_t fileEndPosition = HqSerializerGetStreamPosition(hSerializer);

	// Move back to the version header so we can write the real data for it.
	result = HqSerializerSetStreamPosition(hSerializer, versionHeaderPosition);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to move serializer position to the start of the file version header: error=\"%s\", position=%" PRIuPTR,
			errorString,
			versionHeaderPosition
		);

		return false;
	}

	// Write the real version header now.
	result = writeVersionHeader();

	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to write module version header data (2nd pass): error=\"%s\"",
			errorString
		);

		return false;
	}

	result = HqSerializerSetStreamPosition(hSerializer, fileEndPosition);
	if(result != HQ_SUCCESS)
	{
		const char* const errorString = HqGetErrorCodeString(result);

		HqReportMessage(
			hReport,
			HQ_MESSAGE_TYPE_ERROR,
			"Failed to move serializer position to the end of the file stream: error=\"%s\", position=%" PRIuPTR,
			errorString,
			fileEndPosition
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

	auto kv = hWriter->stringIndexMap.find(pValue);
	if(kv != hWriter->stringIndexMap.end())
	{
		return kv->second;
	}

	const uint32_t output = uint32_t(hWriter->strings.size());

	HqString::AddRef(pValue);

	hWriter->stringIndexMap.emplace(pValue, output);
	hWriter->strings.push_back(pValue);

	return output;
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