//
// Created by Patrick Li on 14/1/23.
//

#ifndef VDL_VDLBACKTRACE_H
#define VDL_VDLBACKTRACE_H

#include "../Exception/vdlExceptionExport.h"
#include "../Utility/vdlUtilityExport.h"
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Backtrace struct and Frame struct
 ----------------------------------------------------------------------------*/

/// @description Maximum frame numbers supported by the backtrace.
#define VDL_BACKTRACE_MAX_FRAME_NUM 1000

/// @description Extra frames at the end of the backtrace.
#define VDL_BACKTRACE_EXTRA_FRAME_NUM 5

/// @description Maximum length of a function name.
#define VDL_BACKTRACE_MAX_FUNCTION_NAME 1000

/// @description Backtrace structure.
/// @param FrameCount (int). The current frame number.
/// @param Line (int [VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM]). Line numbers of all the calls.
/// @param FileName (const char * [VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM]). File names of all the calls.
/// @param FunctionName (const char * [VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM]). Function names of all the calls.
typedef struct VDL_BACKTRACE_T
{
    int FrameCount;
    int Line[VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM];
    const char *FileName[VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM];
    const char *FunctionName[VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM];
} VDL_BACKTRACE_T;

/// @description A global variable for storing the backtrace information.
static VDL_BACKTRACE_T vdl_GlobalVar_Backtrace = {0};

/// @description A global variable for storing the backtrace copy.
/// This is useful when an exception occurred.
static VDL_BACKTRACE_T vdl_GlobalVar_BacktraceCopy = {0};

static int vdl_GlobalVar_FrameCountBeforeTry = 0;

/// @description Backtrace frame structure.
/// @param FileName (const char *const). File name of the caller.
/// @param FunctionName (const char *const). Function name of the call.
/// @param Line (unsigned int). Line number of the call.
typedef struct VDL_FRAME_T
{
    const char *const FileName;
    const char *const FunctionName;
    int Line;
} VDL_FRAME_T;

/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

static inline void vdl_PrintBacktrace(const VDL_BACKTRACE_T bt)
{
    if (bt.FrameCount < 1)
        return;
    printf("Backtrace - %d stack frames:\n", bt.FrameCount);
    char spaces[VDL_BACKTRACE_MAX_FRAME_NUM]     = {'\0'};
    int func_length[VDL_BACKTRACE_MAX_FRAME_NUM] = {0};
    int max_func_length                          = -1;
    vdl_for_i(bt.FrameCount)
    {
        func_length[i] = (int) strlen(bt.FunctionName[i]);
        if (func_length[i] > max_func_length)
            max_func_length = func_length[i];
    }

    for (int i = bt.FrameCount - 1; i >= 0; i--)
    {
        if (i != 0)
            printf("  ║═");
        else
            printf("  ╚═");

        int space_length = max_func_length - (int) strlen(bt.FunctionName[i]);

        vdl_for_j(space_length) spaces[j] = ' ';
        spaces[space_length]              = '\0';

        printf("[%d] Calling <%.*s>%s from %s:%d\n",
               bt.FrameCount - i - 1,
               (int) strlen(bt.FunctionName[i]),
               bt.FunctionName[i],
               spaces,
               bt.FileName[i],
               bt.Line[i]);
    }
}

/*-----------------------------------------------------------------------------
 |  Redefine the special handler for unhandled exceptions
 ----------------------------------------------------------------------------*/

/// @description The customized handler for no catch exception.
/// It can print backtrace.
/// @param exception_id (const VDL_EXCEPTION_T). Exception ID.
_Noreturn static void vdl_BacktraceNoCatchHandler(const VDL_EXCEPTION_T exception_id)
{
    (void) exception_id;
    vdl_PrintBacktrace(vdl_GlobalVar_BacktraceCopy);
    for (int i = 0; i < vdl_GlobalVar_ErrorMessage.Len; i++)
        putchar(vdl_GlobalVar_ErrorMessage.Message[i]);
    puts("Program aborted!");
    exit(EXIT_FAILURE);
}

#ifdef vdl_NoCatchHandler
#undef vdl_NoCatchHandler
#endif
/// @description A special handler for no catch exception.
/// @param exception_id (VDL_EXCEPTION_T). Exception ID.
#define vdl_NoCatchHandler(exception_id) vdl_BacktraceNoCatchHandler(exception_id)

/*-----------------------------------------------------------------------------
 |  Redefine the exception hooks such that the backtrace can work correctly
 ----------------------------------------------------------------------------*/

// The backtrace frame count needs to be saved before a try and restore after a try
// such that a long jump will not cause the frame count to be incorrect.

#ifdef VDL_EXCEPTION_HOOK_START_TRY
#undef VDL_EXCEPTION_HOOK_START_TRY
#endif
#define VDL_EXCEPTION_HOOK_START_TRY                                            \
    do {                                                                        \
        vdl_GlobalVar_FrameCountBeforeTry = vdl_GlobalVar_Backtrace.FrameCount; \
    } while (0)

#ifdef VDL_EXCEPTION_HOOK_AFTER_TRY
#undef VDL_EXCEPTION_HOOK_AFTER_TRY
#endif
#define VDL_EXCEPTION_HOOK_AFTER_TRY                                            \
    do {                                                                        \
        vdl_GlobalVar_Backtrace.FrameCount = vdl_GlobalVar_FrameCountBeforeTry; \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Redefine vdl_Throw to save backtrace
 ----------------------------------------------------------------------------*/

#ifdef vdl_Throw
#undef vdl_Throw
#endif

/// @description Save the backtrace and an error message and throw an exception.
/// @param exception_id (VDL_EXCEPTION_T). Exception ID.
/// @param message (const char *). An error message.
#define vdl_Throw(exception_id, message)                                                           \
    do {                                                                                           \
        vdl_GlobalVar_BacktraceCopy.FrameCount = vdl_GlobalVar_Backtrace.FrameCount;               \
        vdl_for_i(vdl_GlobalVar_Backtrace.FrameCount)                                              \
        {                                                                                          \
            vdl_GlobalVar_BacktraceCopy.FunctionName[i] = vdl_GlobalVar_Backtrace.FunctionName[i]; \
            vdl_GlobalVar_BacktraceCopy.FileName[i]     = vdl_GlobalVar_Backtrace.FileName[i];     \
            vdl_GlobalVar_BacktraceCopy.Line[i]         = vdl_GlobalVar_Backtrace.Line[i];         \
        }                                                                                          \
        vdl_GlobalVar_BacktraceCopy = vdl_GlobalVar_Backtrace;                                     \
        vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, message);                 \
        vdl_GlobalVar_ExceptionFrames.Exception = exception_id;                                    \
        if (vdl_GlobalVar_ExceptionFrames.Frame)                                                   \
            longjmp(*vdl_GlobalVar_ExceptionFrames.Frame, 1);                                      \
        vdl_NoCatchHandler(exception_id);                                                          \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Make a frame for the caller
 ----------------------------------------------------------------------------*/

/// @description Make a backtrace frame struct to capture the current file name,
/// line number and the name of the function to be called.
/// @param function_name (const char * const). A string literal.
#define vdl_MakeFrame(function_name)                                            \
    (VDL_FRAME_T)                                                               \
    {                                                                           \
        .Line = __LINE__, .FileName = __FILE__, .FunctionName = (function_name) \
    }

/*-----------------------------------------------------------------------------
 |  Push frame to backtrace
 ----------------------------------------------------------------------------*/

/// @description Push the frame to the top of the backtrace stack.
/// @param fr (VDL_FRAME_T). The frame.
static inline void vdl_PushFrameToBacktrace(const VDL_FRAME_T fr)
{
    // Record the current frame (a call to the current function)
    vdl_GlobalVar_Backtrace.Line[vdl_GlobalVar_Backtrace.FrameCount]         = fr.Line;
    vdl_GlobalVar_Backtrace.FunctionName[vdl_GlobalVar_Backtrace.FrameCount] = fr.FunctionName;
    vdl_GlobalVar_Backtrace.FileName[vdl_GlobalVar_Backtrace.FrameCount]     = fr.FileName;

    vdl_GlobalVar_Backtrace.FrameCount++;

    // Check if the stack depth exceed the limit.
    if (vdl_GlobalVar_Backtrace.FrameCount > VDL_BACKTRACE_MAX_FRAME_NUM)
    {
        char message[100] = {0};
        snprintf(message, 100, "Exceed the stack limit allowed by the backtrace [%d]", VDL_BACKTRACE_MAX_FRAME_NUM);
        vdl_Throw(VDL_EXCEPTION_EXCEED_STACK_LIMIT, message);
    }
}

/*-----------------------------------------------------------------------------
 |  Pop frame from the backtrace
 ----------------------------------------------------------------------------*/

/// @description Pop a frame from the backtrace stack.
#define vdl_PopFrameFromBacktrace()           \
    do {                                      \
        vdl_GlobalVar_Backtrace.FrameCount--; \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Caller to automatically make and pop backtrace
 ----------------------------------------------------------------------------*/

/// @description Call a function while maintaining the backtrace.
/// @details Users should create a `rt foo_BT()` function and define a macro
/// `#define foo(...) vdl_CallFunction(foo_BT, rt, ##__VA_ARGS__)` as a wrapper.
/// @param function (identifier). The function identifier.
/// @param return_type (type). The return type.
/// @param ... Additional arguments passed to the function call.
#define vdl_CallFunction(function, return_type, ...)        \
    ({                                                      \
        vdl_PushFrameToBacktrace(vdl_MakeFrame(#function)); \
        return_type _return_value = function(__VA_ARGS__);  \
        vdl_PopFrameFromBacktrace();                        \
        _return_value;                                      \
    })


/// @description Call a void function while maintaining the backtrace.
/// @details Users should create a `void foo_BT()` function and define a macro
/// `#define foo(...) vdl_CallVoidFunction(foo_BT, ##__VA_ARGS__)` as a wrapper.
/// @param function (identifier). The function identifier.
/// @param ... Additional arguments passed to the function call.
#define vdl_CallVoidFunction(function, ...)                 \
    do {                                                    \
        vdl_PushFrameToBacktrace(vdl_MakeFrame(#function)); \
        function(##__VA_ARGS__);                            \
        vdl_PopFrameFromBacktrace();                        \
    } while (0)


#endif//VDL_VDLBACKTRACE_H
