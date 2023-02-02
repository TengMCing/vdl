//
// Created by Patrick Li on 9/11/22.
//

#ifndef VDL_VDLERR_H
#define VDL_VDLERR_H

#include "util/vdlUtility.h"
#include "vdldef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Error code
 ----------------------------------------------------------------------------*/

/// @description Error code.
/// @details
/// VDL_ERR_RESOLVED: 0, No error. \n\n
/// VDL_ERR_BT: 1, Backtrace error. \n\n
/// VDL_ERR_POINTER: 2, Pointer error. \n\n
/// VDL_ERR_INDEX: 3, Index error. \n\n
/// VDL_ERR_TYPE: 4, Vector type error. \n\n
/// VDL_ERR_MODE: 5, Vector mode error. \n\n
/// VDL_ERR_MEM: 6, Memory error. \n\n
/// VDL_ERR_GC: 7, Garbage collector error. \n\n
/// VDL_ERR_UNIMPLEMENTED: 8, Unimplemented.
typedef enum VDL_ERR
{
    VDL_ERR_RESOLVED      = 0,
    VDL_ERR_BT            = 1,
    VDL_ERR_POINTER       = 2,
    VDL_ERR_INDEX         = 3,
    VDL_ERR_TYPE          = 4,
    VDL_ERR_MODE          = 5,
    VDL_ERR_MEM           = 6,
    VDL_ERR_GC            = 7,
    VDL_ERR_UNIMPLEMENTED = 8
} VDL_ERR;

/*-----------------------------------------------------------------------------
 |  Error definition
 ----------------------------------------------------------------------------*/

/// @description Global error.
/// @param CODE (VDL_ERR). A predefined error code.
/// @param MSG (const char*). A string to give extra information about the error.
static struct
{
    VDL_ERR CODE;
    const char *MSG;
} VDLINT_GERR = {VDL_ERR_RESOLVED, ""};

/*-----------------------------------------------------------------------------
 |  Get error
 ----------------------------------------------------------------------------*/

/// @description Get the global error code.
/// @return (VDL_ERR) An error code.
#define vdl_GetLastErr() ((VDL_ERR){VDLINT_GERR.CODE})

/// @description Get the global error message.
/// @return (const char *) A string.
#define vdl_GetLastErrMsg() ((const char *){VDLINT_GERR.MSG})

/*-----------------------------------------------------------------------------
 |  Set error
 ----------------------------------------------------------------------------*/

/// @description Set the global error.
/// @arg1 code (VDL_ERR). The target error code. The error message will be set to an empty string.
/// @arg2 code (VDL_ERR), msg (const char *). The target error message.
#define vdlint_SetErr(...) vdlint_GetArg3(__VA_ARGS__, vdlint_SetErr2, vdlint_SetErr1)(__VA_ARGS__)

#define vdlint_SetErr1(code)       \
    do {                           \
        VDLINT_GERR.CODE = (code); \
        VDLINT_GERR.MSG  = "";     \
    } while (0)

#define vdlint_SetErr2(code, msg)  \
    do {                           \
        VDLINT_GERR.CODE = (code); \
        VDLINT_GERR.MSG  = (msg);  \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Control error printing
 ----------------------------------------------------------------------------*/

/// @description A global variable to control the error printing.
static VDL_BOOL VDL_PRINT_ERR_MSG = VDL_BOOL_TRUE;

/*-----------------------------------------------------------------------------
 |  Warning definition
 ----------------------------------------------------------------------------*/

static struct
{
    VDL_BOOL FLAG;
    const char *MSG;
} VDLINT_GWARNING = {VDL_BOOL_FALSE, ""};

/*-----------------------------------------------------------------------------
 |  Get warning
 ----------------------------------------------------------------------------*/

#define vdl_GetLastWarning() VDLINT_GWARNING.MSG

/*-----------------------------------------------------------------------------
 |  Set warning
 ----------------------------------------------------------------------------*/

#define vdlint_SetWarning(msg)     \
    do {                           \
        VDLINT_GWARNING.MSG = msg; \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Reset warning
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 |  Check error
 ----------------------------------------------------------------------------*/

/// @description Check the error code.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
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

/// @description Check if a message is equal to the global error message.
/// @param msg (const char *). A string.
#define vdl_ErrMsgIs(msg) (strcmp(msg, VDLINT_GERR.MSG) == 0)

/// @description Final Exception handler. Use it as a context manager.
#define vdl_Finally() for (int _except = 0; _except < 1; VDLINT_GERR.CODE = VDL_ERR_RESOLVED, _except++)

/*-----------------------------------------------------------------------------
 |  Exit
 ----------------------------------------------------------------------------*/

/// @description Abort the program.
/// @NoBacktrace
_Noreturn static inline void vdl_Abort(void)
{
    printf("Program aborted!\n");
    exit(EXIT_FAILURE);
}

#endif//VDL_VDLERR_H
