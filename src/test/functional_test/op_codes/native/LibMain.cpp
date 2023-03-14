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

#include <Harlequin.h>

#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------

extern "C" HQ_NATIVE_API int HqDllInit(const HqDllRuntimeInfo* const pRuntime, HqVmHandle hVm)
{
	(void) hVm;
	if(pRuntime->version.major > HQ_VERSION_MAJOR)
	{
		return HQ_ERROR_MISMATCH;
	}

	HqFunctionHandle hCallNativeTestFunc = HQ_FUNCTION_HANDLE_NULL;
	HqVmGetFunction(hVm, &hCallNativeTestFunc, "int32_t testCallNative(int32_t)");

	if(hCallNativeTestFunc)
	{
		auto funcImpl = [](HqExecutionHandle hExec, HqFunctionHandle /*hFunc*/, void* /*pUserData*/)
		{
			HqValueHandle hValue = HQ_VALUE_HANDLE_NULL;

			const int getIoRegisterResult = HqExecutionGetIoRegister(hExec, &hValue, 0);
			assert(getIoRegisterResult == HQ_SUCCESS); (void) getIoRegisterResult;

			const int setIoRegisterResult = HqExecutionSetIoRegister(hExec, hValue, 1);
			assert(setIoRegisterResult == HQ_SUCCESS); (void) setIoRegisterResult;
		};

		const int setNativeBindingResult = HqFunctionSetNativeBinding(hCallNativeTestFunc, funcImpl, nullptr);
		assert(setNativeBindingResult == HQ_SUCCESS); (void) setNativeBindingResult;
	}

	return HQ_SUCCESS;
}

//----------------------------------------------------------------------------------------------------------------------
