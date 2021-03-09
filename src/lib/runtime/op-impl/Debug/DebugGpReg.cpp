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

#include "../../Execution.hpp"
#include "../../OpDecl.hpp"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Print the value information of a general-purpose register in the current frame.
//
// 0x: DEBUG_GP_REG r#
//
//   r# = General-purpose register index
//
//----------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void OpCodeImpl_DebugGpReg(XenonExecutionHandle hExec)
{
	const uint32_t registerIndex = XenonExecution::LoadBytecodeUint32(hExec);

	printf("DEBUG_GP_REG r%" PRIu32 "\n", registerIndex);
	printf("> GP Register: index=%" PRIu32 ", frame=\"%s\"\n", registerIndex, hExec->hCurrentFrame->hFunction->pSignature->data);

	XenonValueHandle hValue = XenonFrame::GetGpRegister(hExec->hCurrentFrame, registerIndex);

	// Account for a null value handle by creating a null value object.
	if(hValue == XENON_VALUE_HANDLE_NULL)
	{
		hValue = XenonValueCreateNull();
	}

	switch(hValue->type)
	{
		case XENON_VALUE_TYPE_INT8:    printf("\tInt8: %" PRId8 "\n", hValue->as.int8);      break;
		case XENON_VALUE_TYPE_INT16:   printf("\tInt16: %" PRId16 "\n", hValue->as.int16);   break;
		case XENON_VALUE_TYPE_INT32:   printf("\tInt32: %" PRId32 "\n", hValue->as.int32);   break;
		case XENON_VALUE_TYPE_INT64:   printf("\tInt64: %" PRId64 "\n", hValue->as.int64);   break;
		case XENON_VALUE_TYPE_UINT8:   printf("\tUint8: %" PRIu8 "\n", hValue->as.uint8);    break;
		case XENON_VALUE_TYPE_UINT16:  printf("\tUint16: %" PRIu16 "\n", hValue->as.uint16); break;
		case XENON_VALUE_TYPE_UINT32:  printf("\tUint32: %" PRIu32 "\n", hValue->as.uint32); break;
		case XENON_VALUE_TYPE_UINT64:  printf("\tUint64: %" PRIu64 "\n", hValue->as.uint64); break;
		case XENON_VALUE_TYPE_FLOAT32: printf("\tFloat32: %f\n", hValue->as.float32);        break;
		case XENON_VALUE_TYPE_FLOAT64: printf("\tFloat64: %f\n", hValue->as.float64);        break;

		case XENON_VALUE_TYPE_NULL:
			printf("\tNull");
			break;

		case XENON_VALUE_TYPE_STRING:
			printf(
				"\tString: data=\"%s\", length=%zu, hash=0x%" PRIXPTR "\n",
				hValue->as.pString->data,
				hValue->as.pString->length,
				hValue->as.pString->hash
			);
			break;

		case XENON_VALUE_TYPE_OBJECT:
			// TODO: Implement support for script objects.
			assert(false);
			break;

		default:
			// Unknown value type. This should never happen.
			assert(false);
			break;
	}

	XenonValueDispose(hValue);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------
