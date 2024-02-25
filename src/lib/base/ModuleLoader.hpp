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

#include "String.hpp"

#include "../common/Array.hpp"

#include "../common/module-format/FileHeader.hpp"
#include "../common/module-format/TableOfContents.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HQ_BASE_API HqModuleLoader
{
	enum Flags
	{
		NO_FLAGS         = 0x0,
		SILENT_ERROR     = 0x1,
		DISCARD_BYTECODE = 0x2,
	};

	struct ObjectType
	{
		struct MemberDefinition
		{
			HqString* pName;

			uint8_t type;
		};

		typedef HqArray<MemberDefinition> MemberDefinitionArray;

		MemberDefinitionArray memberDefinitions;

		HqString* pName;
	};

	struct Function
	{
		struct GuardedBlock
		{
			struct ExceptionHandler
			{
				HqString* pClassName;

				uint32_t offset;
				uint8_t type;
			};

			typedef HqArray<ExceptionHandler> ExceptionHandlerArray;

			ExceptionHandlerArray exceptionHandlers;

			uint32_t offset;
			uint32_t length;
		};

		typedef HqArray<GuardedBlock> GuardedBlockArray;

		GuardedBlockArray guardedBlocks;

		HqString* pSignature;

		uint32_t offset;
		uint32_t length;

		uint16_t numInputs;
		uint16_t numOutputs;

		bool isNative;
	};

	typedef HqArray<HqString*>  StringArray;
	typedef HqArray<uint8_t>    ByteArray;
	typedef HqArray<ObjectType> ObjectTypeArray;
	typedef HqArray<Function>   FunctionArray;

	static bool Load(HqModuleLoader& output, HqReportHandle hReport, const char* filePath, uint32_t flags);
	static bool Load(HqModuleLoader& output, HqReportHandle hReport, const void* fileData, size_t fileLength, uint32_t flags);
	static void Dispose(HqModuleLoader& output);

	static bool _load(HqModuleLoader&, HqReportHandle, HqSerializerHandle, uint32_t);
	static bool _loadFileHeader(HqModuleLoader&, HqReportHandle, HqSerializerHandle);
	static bool _loadTableOfContents(HqModuleLoader&, HqReportHandle, HqSerializerHandle);
	static bool _loadStrings(HqModuleLoader&, HqReportHandle, HqSerializerHandle);
	static bool _loadDependencies(HqModuleLoader&, HqReportHandle, HqSerializerHandle);
	static bool _loadGlobals(HqModuleLoader&, HqReportHandle, HqSerializerHandle);
	static bool _loadObjectTypes(HqModuleLoader&, HqReportHandle, HqSerializerHandle);
	static bool _loadFunctions(HqModuleLoader&, HqReportHandle, HqSerializerHandle);
	static bool _loadInitBytecode(HqModuleLoader&, HqReportHandle, HqSerializerHandle);
	static bool _loadBytecode(HqModuleLoader&, HqReportHandle, HqSerializerHandle);

	static void _initialize(HqModuleLoader&);

	static bool _readStringFromIndex(const HqModuleLoader&, HqSerializerHandle, HqString**, int&, size_t&);
	static bool _readBuffer(HqSerializerHandle, size_t, void*, int&, size_t&);
	static bool _readString(HqSerializerHandle, HqString**, int&, size_t&);
	static bool _readBool8(HqSerializerHandle, bool*, int&, size_t&);
	static bool _readBool32(HqSerializerHandle, bool*, int&, size_t&);
	static bool _readUint32(HqSerializerHandle, uint32_t*, int&, size_t&);
	static bool _readUint32AsUint8(HqSerializerHandle, uint8_t*, int&, size_t&);
	static bool _readUint16(HqSerializerHandle, uint16_t*, int&, size_t&);

	HqModuleFileHeader fileHeader;
	HqModuleTableOfContents contents;

	StringArray strings;
	StringArray dependencies;
	StringArray globals;
	ObjectTypeArray objectTypes;
	FunctionArray functions;
	ByteArray initBytecode;
	ByteArray bytecode;

	int endianness;
};

//----------------------------------------------------------------------------------------------------------------------
