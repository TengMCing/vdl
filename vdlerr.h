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
    VDL_ERR_INCOMPATIBLE_MODE  = 6,
    VDL_ERR_ALLOCATE_FAILED    = 7,
    VDL_ERR_GC_FAIL            = 8,
    VDL_ERR_UNIMPLEMENTED      = 9
} VDL_ERR;

/*-----------------------------------------------------------------------------
 |  Error definition
 ----------------------------------------------------------------------------*/

/// @description Global error
/// @param CODE (VDL_ERR). A predefined error code.
/// @param MSG (const char*). A string to give extra information about the error.
static struct
{
    VDL_ERR CODE;
    const char *MSG;
} VDLINT_GERR = {VDL_ERR_RESOLVED, "\0"};

/// @description Get the global error code.
/// @return (VDL_ERR) An error code.
#define vdl_GetErrCode() ((VDL_ERR){VDLINT_GERR.CODE})

/// @description Get the global error message.
/// @return (const char *) A string.
#define vdl_GetErrMsg() ((const char *){VDLINT_GERR.MSG})


/*-----------------------------------------------------------------------------
 |  Control error printing
 ----------------------------------------------------------------------------*/

/// @description A global variable to control the error printing.
static int VDL_ERR_MSG_ON = 1;

/*-----------------------------------------------------------------------------
 |  Check error
 ----------------------------------------------------------------------------*/

/// @description Check the error code and jump to VDL_EXCEPTION if needed.
#define vdlint_e_CheckErr()                       \
    do {                                          \
        if (VDLINT_GERR.CODE != VDL_ERR_RESOLVED) \
            goto VDL_EXCEPTION;                   \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Except
 ----------------------------------------------------------------------------*/

/// @description Exception handler. Use it as a context manager.
/// @param code (int). The target error code.
#define vdl_Catch(code) for (int _except = VDLINT_GERR.CODE != (code); _except < 1; VDLINT_GERR.CODE = VDL_ERR_RESOLVED, _except++)

/// @description Final exception handler. Use it as a context manager.
#define vdl_Finally() for (int _except = 0; _except < 1; VDLINT_GERR.CODE = VDL_ERR_RESOLVED, _except++)

/*-----------------------------------------------------------------------------
 |  Exit
 ----------------------------------------------------------------------------*/

/// @description Abort the program.
/// @NoReturn
/// @NoBacktrace
_Noreturn static inline void vdl_Abort(void)
{
    printf("Program aborted!\n");
    exit(EXIT_FAILURE);
}

#endif//VDL_VDLERR_H
