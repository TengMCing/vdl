//
// Created by Patrick Li on 9/11/22.
//

#ifndef VDL_VDLERR_H
#define VDL_VDLERR_H

#include <stdio.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------
 |  Error code
 ----------------------------------------------------------------------------*/

/// @description Error code.
typedef enum VDL_ERR
{
    VDL_ERR_RESOLVED           = 0,
    VDL_ERR_STACK_LIMIT        = 1,
    VDL_ERR_NULL_POINTER       = 2,
    VDL_ERR_INDEX_OUT_OF_BOUND = 3,
    VDL_ERR_UNKNOWN_TYPE       = 4,
    VDL_ERR_INCOMPATIBLE_TYPE  = 5,
    VDL_ERR_INCORRECT_MODE     = 6,
    VDL_ERR_INCOMPATIBLE_MODE  = 8,
    VDL_ERR_ALLOCATE_FAIL      = 9,
    VDL_ERR_GC_FAIL            = 10,
    VDL_ERR_UNIMPLEMENTED      = 11
} VDL_ERR;

/*-----------------------------------------------------------------------------
 |  Error definition
 ----------------------------------------------------------------------------*/

/// @description Global error
/// @param ERR (VDL_ERR). A predefined error code.
/// @param MSG (const char*). A string to give extra information about the error.
static struct
{
    VDL_ERR ERR;
    const char *MSG;
} VDLINT_GERR = {VDL_ERR_RESOLVED, "\0"};

/// @description Get the global error code.
#define vdl_geterr() ((VDL_ERR){VDLINT_GERR.ERR})
/// @description Get the global error message.
#define vdl_geterrmsg() ((const char *){VDLINT_GERR.MSG})

/*-----------------------------------------------------------------------------
 |  Control error printing
 ----------------------------------------------------------------------------*/

/// @description A global variable to control the error printing.
static int VDL_ERRMSG_ON = 1;

/*-----------------------------------------------------------------------------
 |  Check error
 ----------------------------------------------------------------------------*/

/// @description Check the error code and jump to VDL_EXCEPTION if needed.
#define vdlint_ckerr()                           \
    do {                                         \
        if (VDLINT_GERR.ERR != VDL_ERR_RESOLVED) \
            goto VDL_EXCEPTION;                  \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Except
 ----------------------------------------------------------------------------*/

/// @description Exception handler. Use it as a context manager.
/// @param err (int). The target error code.
#define vdl_catch(err) for (int _except = VDLINT_GERR.ERR != (err); _except < 1; VDLINT_GERR.ERR = VDL_ERR_RESOLVED, _except++)

/// @description Final exception handler. Use it as a context manager.
#define vdl_finally() for (int _except = 0; _except < 1; VDLINT_GERR.ERR = VDL_ERR_RESOLVED, _except++)

/*-----------------------------------------------------------------------------
 |  Exit
 ----------------------------------------------------------------------------*/

/// @description Abort the program.
/// @nobacktrace
_Noreturn static inline void vdl_abort(void)
{
    printf("Program aborted!\n");
    exit(EXIT_FAILURE);
}

#endif//VDL_VDLERR_H
