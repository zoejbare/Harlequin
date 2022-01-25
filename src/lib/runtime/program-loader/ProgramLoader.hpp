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

#pragma once

//----------------------------------------------------------------------------------------------------------------------

#include "../Program.hpp"
#include "../ScriptObject.hpp"

#include "../../common/program-format/CommonHeader.hpp"
#include "../../common/program-format/ProgramHeader.hpp"

//----------------------------------------------------------------------------------------------------------------------

class XenonProgramLoader
{
public:

	static bool Load(
		XenonProgramHandle hProgram,
		XenonVmHandle hVm,
		XenonSerializerHandle hSerializer
	);


private:

#if XENON_MAP_IS_UNORDERED
	#define _XENON_RES_PTR_SET(type) \
		XENON_MAP_TYPE< \
			type, \
			bool, \
			std::hash<type>, \
			std::equal_to<type>, \
			XenonStlAllocator<XENON_MAP_NODE_TYPE(XenonString*, bool)> \
		>

#else
	#define _XENON_RES_PTR_SET(type) \
		XENON_MAP_TYPE< \
			type, \
			bool, \
			std::less<type>, \
			XenonStlAllocator<XENON_MAP_NODE_TYPE(XenonString*, bool)> \
		>

#endif

	typedef _XENON_RES_PTR_SET(XenonString*) StringResources;

#undef _XENON_RES_PTR_SET

	XenonProgramLoader(
		XenonProgramHandle hProgram,
		XenonVmHandle hVm,
		XenonSerializerHandle hSerializer
	);
	~XenonProgramLoader();

	bool prv_loadFile();
	bool prv_finalize();

	bool prv_readFileHeader();
	bool prv_validateFileHeader();

	bool prv_readDependencyTable();
	bool prv_readObjectTable();
	bool prv_readConstantTable();
	bool prv_readGlobalTable();
	bool prv_readFunctions();

	bool prv_readLocalVariables(XenonString*, XenonValue::StringToHandleMap&);
	bool prv_readGuardedBlocks(XenonString*, XenonGuardedBlock::Array&);

	void prv_trackString(XenonString*);
	void prv_trackObjectSchema(XenonString*, XenonScriptObject*);
	void prv_trackGlobalValue(XenonString*, XenonValueHandle);
	void prv_trackFunction(XenonString*, XenonFunctionHandle);

	XenonProgramHandle m_hProgram;
	XenonVmHandle m_hVm;
	XenonSerializerHandle m_hSerializer;
	XenonReportHandle m_hReport;

	XenonProgramHeader m_fileHeader;

	StringResources m_strings;

	XenonScriptObject::StringToPtrMap m_objectSchemas;
	XenonValue::StringToHandleMap m_globalValues;
	XenonFunction::StringToHandleMap m_functions;
};

//----------------------------------------------------------------------------------------------------------------------

inline void XenonProgramLoader::prv_trackString(XenonString* const pString)
{
	XENON_MAP_FUNC_INSERT(m_strings, pString, false);
}

//----------------------------------------------------------------------------------------------------------------------

inline void XenonProgramLoader::prv_trackObjectSchema(XenonString* const pTypeName, XenonScriptObject* const pSchema)
{
	XENON_MAP_FUNC_INSERT(m_objectSchemas, pTypeName, pSchema);
}

//----------------------------------------------------------------------------------------------------------------------

inline void XenonProgramLoader::prv_trackGlobalValue(XenonString* const pGlobalName, XenonValueHandle hValue)
{
	XENON_MAP_FUNC_INSERT(m_globalValues, pGlobalName, hValue);
}

//----------------------------------------------------------------------------------------------------------------------

inline void XenonProgramLoader::prv_trackFunction(XenonString* const pSignature, XenonFunctionHandle hFunction)
{
	XENON_MAP_FUNC_INSERT(m_functions, pSignature, hFunction);
}

//----------------------------------------------------------------------------------------------------------------------
