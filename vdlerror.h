//
// Created by Patrick Li on 9/11/22.
//

#ifndef VDL_VDLERROR_H
#define VDL_VDLERROR_H

#include <stdio.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------
 |  Error code
 ----------------------------------------------------------------------------*/

/// @description Error code.
typedef enum VDL_ERROR_CODE
{
    VDL_ERROR_RESOLVED                 = 0,
    VDL_ERROR_EXCEED_STACK_DEPTH_LIMIT = 1,
    VDL_ERROR_NULL_POINTER             = 2,
    VDL_ERROR_INDEX_OUT_OF_BOUND       = 3,
    VDL_ERROR_UNKNOWN_TYPE             = 4,
    VDL_ERROR_NOT_STACK_ALLOCATED      = 5,
    VDL_ERROR_NOT_HEAP_ALLOCATED       = 6,
    VDL_ERROR_INCOMPATIBLE_TYPE        = 7,
    VDL_ERROR_ZERO_CAPACITY            = 8,
    VDL_ERROR_ZERO_LENGTH_VECTOR       = 9,
    VDL_ERROR_ZERO_OBJECTS             = 10
} VDL_ERROR_CODE;

/*-----------------------------------------------------------------------------
 |  Error definition
 ----------------------------------------------------------------------------*/

/// @description Global error code.
static VDL_ERROR_CODE VDL_INTERNAL_GERROR = VDL_ERROR_RESOLVED;

/// @description Get a copy of the global error code.
#define vdl_GetError() ((VDL_ERROR_CODE){VDL_INTERNAL_GERROR})

/*-----------------------------------------------------------------------------
 |  Throw an error
 ----------------------------------------------------------------------------*/

#define VDL_THROW goto VDL_EXCEPTION

/*-----------------------------------------------------------------------------
 |  Check error
 ----------------------------------------------------------------------------*/

/// @description Check error code and jump to VDL_EXCEPTION if needed.
#define vdl_internal_CheckError()                      \
    do {                                               \
        if (VDL_INTERNAL_GERROR != VDL_ERROR_RESOLVED) \
            VDL_THROW;                                 \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Except
 ----------------------------------------------------------------------------*/

/// @description Exception handler. Use it as a context manager.
/// @param error_code (int). The target error code.
#define vdl_Except(error_code) for (int _except = VDL_INTERNAL_GERROR != (error_code); _except < 1; VDL_INTERNAL_GERROR = VDL_ERROR_RESOLVED, _except++)

/// @description Final exception handler. Use it as a context manager.
#define vdl_Finally() for (int _except = 0; _except < 1; VDL_INTERNAL_GERROR = VDL_ERROR_RESOLVED, _except++)

/*-----------------------------------------------------------------------------
 |  Exit
 ----------------------------------------------------------------------------*/

/// @description Abort the program.
_Noreturn static inline void vdl_Abort(void)
{
    printf("Program aborted!\n");
    exit(EXIT_FAILURE);
}

#endif//VDL_VDLERROR_H
