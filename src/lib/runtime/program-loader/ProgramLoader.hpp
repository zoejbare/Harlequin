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

#include "../../common/program-format/FileHeader.hpp"
#include "../../common/program-format/ProgramHeader.hpp"

//----------------------------------------------------------------------------------------------------------------------

class HqProgramLoader
{
public:

	static bool Load(
		HqProgramHandle hProgram,
		HqVmHandle hVm,
		HqSerializerHandle hSerializer
	);


private:

#if HQ_MAP_IS_UNORDERED
	#define _HQ_RES_PTR_SET(type) \
		HQ_MAP_TYPE< \
			type, \
			bool, \
			std::hash<type>, \
			std::equal_to<type>, \
			HqStlAllocator<HQ_MAP_NODE_TYPE(HqString*, bool)> \
		>

#else
	#define _HQ_RES_PTR_SET(type) \
		HQ_MAP_TYPE< \
			type, \
			bool, \
			std::less<type>, \
			HqStlAllocator<HQ_MAP_NODE_TYPE(HqString*, bool)> \
		>

#endif

	typedef _HQ_RES_PTR_SET(HqString*) StringResources;

#undef _HQ_RES_PTR_SET

	HqProgramLoader(
		HqProgramHandle hProgram,
		HqVmHandle hVm,
		HqSerializerHandle hSerializer
	);
	~HqProgramLoader();

	bool prv_loadFile();
	void prv_finalize();

	bool prv_readProgramHeader();
	bool prv_validateProgramHeader();

	bool prv_readDependencyTable();
	bool prv_readObjectTable();
	bool prv_readConstantTable();
	bool prv_readGlobalTable();
	bool prv_readFunctions();
	bool prv_readBytecode();

	bool prv_readLocalVariables(HqString*, HqValue::StringToHandleMap&);
	bool prv_readGuardedBlocks(HqString*, HqGuardedBlock::Array&);

	void prv_trackString(HqString*);
	void prv_trackObjectSchema(HqString*, HqScriptObject*);
	void prv_trackGlobalValue(HqString*, HqValueHandle);
	void prv_trackFunction(HqString*, HqFunctionHandle);

	HqProgramHandle m_hProgram;
	HqVmHandle m_hVm;
	HqSerializerHandle m_hSerializer;
	HqReportHandle m_hReport;

	HqProgramHeader m_programHeader;

	StringResources m_strings;

	HqScriptObject::StringToPtrMap m_objectSchemas;
	HqValue::StringToHandleMap m_globalValues;
	HqFunction::StringToHandleMap m_functions;
};

//----------------------------------------------------------------------------------------------------------------------

inline void HqProgramLoader::prv_trackString(HqString* const pString)
{
	HQ_MAP_FUNC_INSERT(m_strings, pString, false);
}

//----------------------------------------------------------------------------------------------------------------------

inline void HqProgramLoader::prv_trackObjectSchema(HqString* const pTypeName, HqScriptObject* const pSchema)
{
	HQ_MAP_FUNC_INSERT(m_objectSchemas, pTypeName, pSchema);
}

//----------------------------------------------------------------------------------------------------------------------

inline void HqProgramLoader::prv_trackGlobalValue(HqString* const pGlobalName, HqValueHandle hValue)
{
	HQ_MAP_FUNC_INSERT(m_globalValues, pGlobalName, hValue);
}

//----------------------------------------------------------------------------------------------------------------------

inline void HqProgramLoader::prv_trackFunction(HqString* const pSignature, HqFunctionHandle hFunction)
{
	HQ_MAP_FUNC_INSERT(m_functions, pSignature, hFunction);
}

//----------------------------------------------------------------------------------------------------------------------
