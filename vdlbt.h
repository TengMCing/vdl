//
// Created by Patrick Li on 23/10/22.
//

#ifndef VDL_VDLBT_H
#define VDL_VDLBT_H

#include "vdlerr.h"
#include "vdlutil.h"
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Backtrace definition
 ----------------------------------------------------------------------------*/

/// @description Stack frame limit
#define VDL_BT_STACK_LIMIT 1000

/// @description Function name limit
#define VDL_BT_FUNC_NAME_LIMIT 10000

/// @description Backtrace frame struct.
/// @param file (const char *) File name.
/// @param line (int) Line number.
typedef struct vdl_fr
{
    const char *const file;
    const int line;
} vdl_fr;

/// @description Global backtrace.
/// @details Allow one more frame to record the last calling error.
/// @param FRAME_NUM (int) Number of frames.
/// @param LINE (int [VDL_BT_STACK_LIMIT + 1]) Line number of calls.
/// @param FUNC (const char *[VDL_BT_STACK_LIMIT + 1]) Function names.
/// @param FILE (const char *[VDL_BT_STACK_LIMIT + 1]) File names.
static struct
{
    int FRAME_NUM;
    int LINE[VDL_BT_STACK_LIMIT + 1];
    const char *FUNC[VDL_BT_STACK_LIMIT + 1];
    const char *FILE[VDL_BT_STACK_LIMIT + 1];
} VDLINT_GBT = {0};


/// @description Get the number of frames in the global backtrace.
/// @return (int) Frame number.
#define vdl_GetBacktraceFrameNum() ((int){VDLINT_GBT.FRAME_NUM})

/// @description Get a the line number of a call from the global backtrace.
/// @param frame (int). The target frame.
/// @return (int) 0 if frame does not exist. Otherwise, the line number.
#define vdl_GetBacktraceLineNum(frame) ((frame) >= 0 && (frame) < VDLINT_GBT.FRAME_NUM ? VDLINT_GBT.LINE[frame] : 0)

/// @description Get the function name of a frame from the global backtrace.
/// @param frame (int). The target frame.
/// @return (const char *) "\0" if frame does not exist. Otherwise, the function name.
#define vdl_GetBacktraceFuncName(frame) ((frame) >= 0 && (frame) < VDLINT_GBT.FRAME_NUM ? VDLINT_GBT.FUNC[frame] : "\0")

/// @description Get the file name of a frame from the global backtrace.
/// @param frame (int). The target frame.
/// @return (const char *) "\0" if frame does not exist. Otherwise, the file name.
#define vdl_GetBacktraceFileName(frame) ((frame) >= 0 && (frame) < VDLINT_GBT.FRAME_NUM ? VDLINT_GBT.FILE[frame] : "\0")

/*-----------------------------------------------------------------------------
 |  Macros to push frames to backtrace and pop frames from backtrace
 ----------------------------------------------------------------------------*/

/// @description Make a backtrace frame struct to capture the current file name and line number.
#define vdlint_MakeFrame()                 \
    (vdl_fr)                               \
    {                                      \
        .file = __FILE__, .line = __LINE__ \
    }

/// @description Push the frame to the top of the backtrace stack.
/// @details This should be used at the beginning of every function that
/// needs to be traced. If the stack number exceeds the limit,
/// an exception will be raised.
/// @param fr (vdl_fr). The frame.
#define vdl_e_PushFrame(fr)                                                                 \
    do {                                                                                    \
        VDLINT_GBT.FRAME_NUM++;                                                             \
                                                                                            \
        /* Prevent the function to run if the caller has not handled the exception. */      \
        vdlint_e_CheckErr();                                                                \
                                                                                            \
        /* Allow one more frame to record the last calling error */                         \
        if (VDLINT_GBT.FRAME_NUM <= VDL_BT_STACK_LIMIT + 1)                                 \
        {                                                                                   \
            VDLINT_GBT.LINE[VDLINT_GBT.FRAME_NUM - 1] = (fr).line;                          \
            VDLINT_GBT.FUNC[VDLINT_GBT.FRAME_NUM - 1] = __func__;                           \
            VDLINT_GBT.FILE[VDLINT_GBT.FRAME_NUM - 1] = (fr).file;                          \
        }                                                                                   \
                                                                                            \
        /* Check if the stack depth exceed the maximum limit. */                            \
        if (VDLINT_GBT.FRAME_NUM > VDL_BT_STACK_LIMIT)                                      \
        {                                                                                   \
            VDLINT_GERR.CODE = VDL_ERR_STACK_LIMIT;                                         \
            VDLINT_GERR.MSG  = "Exceed stack limit";                                        \
            if (VDL_ERR_MSG_ON)                                                             \
            {                                                                               \
                vdlint_PrintBacktrace();                                                    \
                printf("\n");                                                               \
                printf("[E%03d] Error raised by <%s> at %s:%d: Exceed stack limit [%d]!\n", \
                       VDL_ERR_STACK_LIMIT,                                                 \
                       __func__,                                                            \
                       __FILE__,                                                            \
                       __LINE__,                                                            \
                       VDL_BT_STACK_LIMIT);                                                 \
            }                                                                               \
            goto VDL_EXCEPTION;                                                             \
        }                                                                                   \
    } while (0)

/// @description Pop a frame from the backtrace stack.
#define vdlint_e_PopFrame()     \
    do {                        \
        VDLINT_GBT.FRAME_NUM--; \
        vdlint_e_CheckErr();    \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Caller to automatically make and pop backtrace
 ----------------------------------------------------------------------------*/

/// @description Call a function while maintaining the backtrace.
/// @details Users should create a `rt foo_BT()` function and define a macro
/// `#define foo(...) vdl_bt_Call(foo_BT, rt, ##__VA_ARGS__)` as a wrapper.
/// @param func (identifier). The function identifier.
/// @param rtype (type). The return type.
/// @param ... Additional arguments passed to the function call.
#define vdlint_e_Call(func, rtype, ...)                      \
    ({                                                       \
        rtype _rv = func(vdlint_MakeFrame(), ##__VA_ARGS__); \
        vdlint_e_PopFrame();                                 \
        _rv;                                                 \
    })

/// @description Call a void function while maintaining the backtrace.
/// @details Users should create a `void foo_BT()` function and define a macro
/// `#define foo(...) vdl_bt_CallVoid(foo_BT, ##__VA_ARGS__)` as a wrapper.
/// @param func (identifier). The function identifier.
/// @param ... Additional arguments passed to the function call.
#define vdlint_e_CallVoid(func, ...)             \
    do {                                         \
        func(vdlint_MakeFrame(), ##__VA_ARGS__); \
        vdlint_e_PopFrame();                     \
    } while (0)


/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

/// @description Print the backtrace.
/// @NoReturn
/// @NoBacktrace
static inline void vdlint_PrintBacktrace(void)
{
    printf("Backtrace - %d stack frames:\n", VDLINT_GBT.FRAME_NUM);
    char spaces[VDL_BT_FUNC_NAME_LIMIT] = {'\0'};
    int func_length[VDL_BT_STACK_LIMIT] = {0};
    int max_func_length                 = -1;
    vdlint_for_i(VDLINT_GBT.FRAME_NUM)
    {
        func_length[i] = (int) strlen(VDLINT_GBT.FUNC[i]);
        if (func_length[i] > max_func_length)
            max_func_length = func_length[i];
    }

    for (int i = VDLINT_GBT.FRAME_NUM - 1; i >= 0; i--)
    {
        if (i != 0)
            printf("  ║═");
        else
            printf("  ╚═");

        int space_length = max_func_length - (int) strlen(VDLINT_GBT.FUNC[i]);

        vdlint_for_j(space_length) spaces[j] = ' ';
        spaces[space_length]                 = '\0';

        printf("[%d] Calling <%.*s>%s from %s:%d\n",
               VDLINT_GBT.FRAME_NUM - i - 1,
               (int) strlen(VDLINT_GBT.FUNC[i]) - 3,
               VDLINT_GBT.FUNC[i],
               spaces,
               VDLINT_GBT.FILE[i],
               VDLINT_GBT.LINE[i]);
    }
}

#endif//VDL_VDLBT_H
