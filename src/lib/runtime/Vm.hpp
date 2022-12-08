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

#include "Decoder.hpp"
#include "Execution.hpp"
#include "Function.hpp"
#include "GarbageCollector.hpp"
#include "OpDecl.hpp"
#include "Program.hpp"
#include "ScriptObject.hpp"
#include "Value.hpp"

#include "../base/Mutex.hpp"
#include "../base/Thread.hpp"

#include "../common/Dependency.hpp"
#include "../common/Report.hpp"

//----------------------------------------------------------------------------------------------------------------------

struct HqVm
{
	typedef HQ_MAP_TYPE<
		int,
		HqScriptObject*,
#if HQ_MAP_IS_UNORDERED
		std::hash<int>,
		std::equal_to<int>,
#else
		std::less<int>,
#endif
		HqStlAllocator<HQ_MAP_NODE_TYPE(int, HqScriptObject*)>
	> EmbeddedExceptionMap;

	struct OpCode
	{
		typedef void (*ExecuteCallback)(HqExecutionHandle);
		typedef void (*DisassembleCallback)(HqDisassemble&);

		ExecuteCallback execFn;
		DisassembleCallback disasmFn;
	};

	typedef HqArray<OpCode> OpCodeArray;

	static HqVmHandle Create(const HqVmInit& init);

	static void Dispose(HqVmHandle hVm);

	static bool AttachExec(HqVmHandle hVm, HqExecutionHandle hExec);
	static void DetachExec(HqVmHandle hVm, HqExecutionHandle hExec);

	static int SetGlobalVariable(HqVmHandle hVm, HqValueHandle hValue, HqString* const pVariableName);

	static HqProgramHandle GetProgram(HqVmHandle hVm, HqString* const pProgramName, int* const pOutResult);
	static HqFunctionHandle GetFunction(HqVmHandle hVm, HqString* const pFunctionSignature, int* const pOutResult);
	static HqValueHandle GetGlobalVariable(HqVmHandle hVm, HqString* const pVariableName, int* const pOutResult);
	static HqScriptObject* GetObjectSchema(HqVmHandle hVm, HqString* const pTypeName, int* const pOutResult);

	static HqValueHandle CreateStandardException(HqVmHandle hVm, const int exceptionType, const char* const message);

	static void ExecuteOpCode(HqVmHandle hVm, HqExecutionHandle hExec, const int opCode);
	static void DisassembleOpCode(HqVmHandle hVm, HqDisassemble& disasm, const int opCode);

	static void prv_setupOpCodes(HqVmHandle);
	static void prv_setupBuiltIns(HqVmHandle);
	static void prv_setupEmbeddedExceptions(HqVmHandle);

	static int32_t prv_gcThreadMain(void*);

	void* operator new(const size_t sizeInBytes);
	void operator delete(void* const pObject);

	OpCodeArray opCodes;
	EmbeddedExceptionMap embeddedExceptions;

	HqProgram::StringToHandleMap programs;
	HqFunction::StringToHandleMap functions;
	HqValue::StringToHandleMap globals;
	HqScriptObject::StringToPtrMap objectSchemas;
	HqExecution::HandleArray executionContexts;

	HqReport report;
	HqGarbageCollector gc;
	HqThread gcThread;
	HqMutex lock;

	bool isShuttingDown;
};

//----------------------------------------------------------------------------------------------------------------------
