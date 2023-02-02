/*
This software is licensed under the MIT License: Copyright (c) 2007-2021 Mark VanderVoord

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


/* This header file is adopted from https://github.com/ThrowTheSwitch/CException/blob/master/lib/CException.h */

#ifndef VDL_VDLEXCEPTION_H
#define VDL_VDLEXCEPTION_H

#include "../vdlGlobalConfig.h"

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------
 |  Exception type and none value
 ----------------------------------------------------------------------------*/

/// @description The actual exception type.
#define VDL_EXCEPTION_T int

/// @description The reserved value representing no exception.
#define VDL_EXCEPTION_NONE (1234)

/*-----------------------------------------------------------------------------
 |  Define exceptions IDs
 ----------------------------------------------------------------------------*/

/// @description Stack overflow.
#define VDL_EXCEPTION_EXCEED_STACK_LIMIT 0x1

/*-----------------------------------------------------------------------------
 |  Error message
 ----------------------------------------------------------------------------*/

/// @description Maximum length of error message.
#define VDL_ERROR_MESSAGE_MAX_LEN 32768

/// @desciption A global variable for storing error message.
/// @param Message (char [VDL_ERROR_MESSAGE_MAX_LEN]). The string.
/// @param Len (int). The length of the string.
static struct
{
    char Message[VDL_ERROR_MESSAGE_MAX_LEN];
    int Len;
} vdl_GlobalVar_ErrorMessage = {0};


/// @description Save error message to the global buffer.
/// @param exception_id (const VDL_EXCEPTION_T). The exception ID.
/// @param function_name (const char *). Function name.
/// @param file_name (const char *). File name.
/// @param line (const int). The line number.
/// @param message (const char *const). An additional error message.
static inline void vdl_SaveErrorMessage(const VDL_EXCEPTION_T exception_id,
                                        const char *function_name,
                                        const char *file_name,
                                        const int line,
                                        const char *const message)
{
    vdl_GlobalVar_ErrorMessage.Len = 0;

    int len = snprintf(vdl_GlobalVar_ErrorMessage.Message,
                       VDL_ERROR_MESSAGE_MAX_LEN,
                       "[E%03d] Error raised by <%s> at %s:%d: %s!\n",
                       exception_id,
                       function_name,
                       file_name,
                       line,
                       message);

    vdl_GlobalVar_ErrorMessage.Len = len;
}

/*-----------------------------------------------------------------------------
 |  Exception
 ----------------------------------------------------------------------------*/

/// @description A global variable for storing exception frames.
/// @param Frame (jmp_buf *). The current frame.
/// @param Exception (VDL_EXCEPTION_T volatile). The exception ID.
static volatile struct
{
    jmp_buf *Frame;
    VDL_EXCEPTION_T volatile Exception;
} vdl_GlobalVar_ExceptionFrames = {0};

/// @description Get the current exception ID.
#define vdl_GetExceptionID() ((VDL_EXCEPTION_T){vdl_GlobalVar_ExceptionFrames.Exception})

/*-----------------------------------------------------------------------------
 |  No catch handler
 ----------------------------------------------------------------------------*/

/// @description The default handler for no catch exception.
/// @details The default handler prints the error message and abort the program.
/// @param exception_id (const VDL_EXCEPTION_T). Exception ID.
_Noreturn static void vdl_DefaultNoCatchHandler(const VDL_EXCEPTION_T exception_id)
{
    (void) exception_id;
    for (int i = 0; i < vdl_GlobalVar_ErrorMessage.Len; i++)
        putchar(vdl_GlobalVar_ErrorMessage.Message[i]);
#ifdef VDL_EXCEPTION_DISABLE
    puts("Program abort in no exception mode!\n");
#else
    puts("Program abort!\n");
#endif//VDL_EXCEPTION_DISABLE
    exit(EXIT_FAILURE);
}

#ifndef vdl_NoCatchHandler
/// @description A special handler for no catch exception.
/// @param exception_id (VDL_EXCEPTION_T). Exception ID.
#define vdl_NoCatchHandler(exception_id) vdl_DefaultNoCatchHandler(exception_id)
#endif

/*-----------------------------------------------------------------------------
 |  Throw
 ----------------------------------------------------------------------------*/

// If the exception is disabled, no jump will be performed.
#ifdef VDL_EXCEPTION_DISABLE
#define vdl_Throw(exception_id, message)                                           \
    do {                                                                           \
        vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, message); \
        vdl_NoCatchHandler(exception_id);                                          \
    } while (0)
#else
/// @description Save an error message and throw an exception.
/// @param exception_id (VDL_EXCEPTION_T). Exception ID.
/// @param message (const char *). An error message.
#define vdl_Throw(exception_id, message)                                               \
    do {                                                                               \
        if ((message)[0] != '\0')                                                      \
            vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, message); \
        vdl_GlobalVar_ExceptionFrames.Exception = exception_id;                        \
        if (vdl_GlobalVar_ExceptionFrames.Frame)                                       \
            longjmp(*vdl_GlobalVar_ExceptionFrames.Frame, 1);                          \
        vdl_NoCatchHandler(exception_id);                                              \
    } while (0)
#endif//VDL_EXCEPTION_DISABLE

/*-----------------------------------------------------------------------------
 |  Hooks in try and catch
 ----------------------------------------------------------------------------*/

//These hooks allow you to inject custom code into places, particularly useful for saving and restoring additional state
#ifndef VDL_EXCEPTION_HOOK_START_TRY
/// @description Inject code before setjmp.
#define VDL_EXCEPTION_HOOK_START_TRY
#endif
#ifndef VDL_EXCEPTION_HOOK_HAPPY_TRY
/// @description Inject code if no exception thrown.
#define VDL_EXCEPTION_HOOK_HAPPY_TRY
#endif
#ifndef VDL_EXCEPTION_HOOK_AFTER_TRY
/// @description Inject code after try in regrades of exception.
#define VDL_EXCEPTION_HOOK_AFTER_TRY
#endif
#ifndef VDL_EXCEPTION_HOOK_START_CATCH
/// @description Inject code before catch (an exception thrown).
#define VDL_EXCEPTION_HOOK_START_CATCH
#endif

/*-----------------------------------------------------------------------------
 |  Try
 ----------------------------------------------------------------------------*/

// If the exception is disabled, no setup needs to be done.
#ifdef VDL_EXCEPTION_DISABLE
#define vdl_Try \
    {           \
        if (1)
#else
/// @dscription Try one or more statements.
/// @details vdl_Try needs to be paired with vdl_Catch.
/// @example
/// // Throw an exception with ID 1. \n
/// void foo(void) \n
/// { \n
/// vdl_Throw(1); \n
/// } \n
/// \n
/// // Try and print the exception ID. \n
/// vdl_Try { \n
/// foo(); \n
/// printf("This line will not be executed!"); \n
/// } vdl_Catch { \n
/// printf("%d", vdl_GetExceptionID()); \n
/// }
#define vdl_Try                                                                        \
    {                                                                                  \
        jmp_buf *PrevFrame, NewFrame;                                                  \
        PrevFrame                               = vdl_GlobalVar_ExceptionFrames.Frame; \
        vdl_GlobalVar_ExceptionFrames.Frame     = (jmp_buf *) (&NewFrame);             \
        vdl_GlobalVar_ExceptionFrames.Exception = VDL_EXCEPTION_NONE;                  \
        VDL_EXCEPTION_HOOK_START_TRY;                                                  \
        if (setjmp(NewFrame) == 0)                                                     \
        {                                                                              \
            if (1)
#endif//VDL_EXCEPTION_DISABLE


/*-----------------------------------------------------------------------------
 |  Catch
 ----------------------------------------------------------------------------*/

// If the exception is disabled, the catch scope should not be run.
#ifdef VDL_EXCEPTION_DISABLE
#define vdl_Catch \
    else {}       \
    }             \
    if (0)
#else
/// @dscription Catch an exception.
/// @details vdl_Try needs to be paired with vdl_Catch.
/// @example
/// // Throw an exception with ID 1. \n
/// void foo(void) \n
/// { \n
/// vdl_Throw(1); \n
/// } \n
/// \n
/// // Try and print the exception ID. \n
/// vdl_Try { \n
/// foo(); \n
/// printf("This line will not be executed!"); \n
/// } vdl_Catch { \n
/// printf("%d", vdl_GetExceptionID()); \n
/// }
#define vdl_Catch                                                 \
    else {}                                                       \
    vdl_GlobalVar_ExceptionFrames.Exception = VDL_EXCEPTION_NONE; \
    VDL_EXCEPTION_HOOK_HAPPY_TRY;                                 \
    }                                                             \
    else                                                          \
    {                                                             \
        VDL_EXCEPTION_HOOK_START_CATCH;                           \
    }                                                             \
    vdl_GlobalVar_ExceptionFrames.Frame = PrevFrame;              \
    VDL_EXCEPTION_HOOK_AFTER_TRY;                                 \
    }                                                             \
    if (vdl_GlobalVar_ExceptionFrames.Exception != VDL_EXCEPTION_NONE)
#endif//VDL_EXCEPTION_DISABLE

/*-----------------------------------------------------------------------------
 |  Exit Try
 ----------------------------------------------------------------------------*/

/// @description Exit the current vdl_Try block and skip one vdl_Catch.
#define vdl_ExitTry() vdl_Throw(VDL_EXCEPTION_NONE, "")

#endif// VDL_VDLEXCEPTION_H
