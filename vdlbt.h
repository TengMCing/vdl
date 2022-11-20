//
// Created by Patrick Li on 23/10/22.
//

#ifndef VDL_VDLBT_H
#define VDL_VDLBT_H


#include "vdlerr.h"
#include "vdlutil.h"
#include <stdarg.h>
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Backtrace definition
 ----------------------------------------------------------------------------*/

/// @description Stack frame limit
#define VDL_BT_STACK_LIMIT 1000

/// @description Function name limit
#define VDL_BT_FUNC_NAME_LIMIT 10000

/// @description Backtrace struct.
/// @param file (const char *) File name.
/// @param line (int) Line number.
typedef struct vdl_bt
{
    const char *const file;
    const int line;
} vdl_bt;

/// @description Global backtrace.
/// @details Allow one more frame to record the last calling error.
/// @param NFRAME (int) Number of frames.
/// @param LINE (int [VDL_BT_STACK_LIMIT + 1]) Line number of calls.
/// @param FUNC (const char *[VDL_BT_STACK_LIMIT + 1]) Function names.
/// @param FILE (const char *[VDL_BT_STACK_LIMIT + 1]) File names.
static struct
{
    int NFRAME;
    int LINE[VDL_BT_STACK_LIMIT + 1];
    const char *FUNC[VDL_BT_STACK_LIMIT + 1];
    const char *FILE[VDL_BT_STACK_LIMIT + 1];
} VDLINT_GBT = {0};

/// @description Get the number of frames in the global backtrace.
#define vdl_bt_getnframe() ((int){VDLINT_GBT.NFRAME})

/// @description Get a the LINE number of a call from the global backtrace.
/// @param frame (int). The target frame.
#define vdl_bt_getline(frame) ((frame) >= 0 && (frame) < VDLINT_GBT.NFRAME ? VDLINT_GBT.LINE[frame] : 0)

/// @description Get the function name of a frame from the global backtrace.
/// @param frame (int). The target frame.
#define vdl_bt_getfunc(frame) ((frame) >= 0 && (frame) < VDLINT_GBT.NFRAME ? VDLINT_GBT.FUNC[frame] : "\0")

/// @description Get the file name of a frame from the global backtrace.
/// @param frame (int). The target frame.
#define vdl_bt_getfile(frame) ((frame) >= 0 && (frame) < VDLINT_GBT.NFRAME ? VDLINT_GBT.FILE[frame] : "\0")

/*-----------------------------------------------------------------------------
 |  Macros to push frames to backtrace and pop frames from backtrace
 ----------------------------------------------------------------------------*/

/// @description Make a backtrace struct to capture the current file name and line number.
#define vdlint_bt_make()                   \
    (vdl_bt)                               \
    {                                      \
        .file = __FILE__, .line = __LINE__ \
    }

/// @description Push the function to the top of the backtrace stack.
/// @details This should be used at the beginning of every function that
/// needs to be traced. If the stack number exceeds the limit,
/// an exception will be raised.
/// @param bt (vdl_bt). The backtrace.
#define vdl_bt_push(bt)                                                                     \
    do {                                                                                    \
        VDLINT_GBT.NFRAME++;                                                                \
                                                                                            \
        /* Prevent the function to run if the caller has not handled the exception. */      \
        vdlint_err_check();                                                                 \
                                                                                            \
        /* Allow one more frame to record the last calling error */                         \
        if (VDLINT_GBT.NFRAME <= VDL_BT_STACK_LIMIT + 1)                                    \
        {                                                                                   \
            VDLINT_GBT.LINE[VDLINT_GBT.NFRAME - 1] = (bt).line;                             \
            VDLINT_GBT.FUNC[VDLINT_GBT.NFRAME - 1] = __func__;                              \
            VDLINT_GBT.FILE[VDLINT_GBT.NFRAME - 1] = (bt).file;                             \
        }                                                                                   \
                                                                                            \
        /* Check if the stack depth exceed the maximum limit. */                            \
        if (VDLINT_GBT.NFRAME > VDL_BT_STACK_LIMIT)                                         \
        {                                                                                   \
            VDLINT_GERR.CODE = VDL_ERR_STACK_LIMIT;                                         \
            VDLINT_GERR.MSG  = "Exceed stack limit";                                        \
            if (VDL_ERR_MSG_ON)                                                             \
            {                                                                               \
                vdl_bt_print();                                                             \
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

/// @description Pop an item from the backtrace stack.
#define vdlint_bt_pop()      \
    do {                     \
        VDLINT_GBT.NFRAME--; \
        vdlint_err_check();  \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Caller to automatically make and pop backtrace
 ----------------------------------------------------------------------------*/

/// @description Call a function while maintaining the backtrace.
/// @details Users should create a `rt foo_BT()` function and define a macro
/// `#define foo(...) vdl_bt_call(foo_BT, rt, ##__VA_ARGS__)` as a wrapper.
/// @param func (identifier). The function identifier.
/// @param rtype (type). The return type.
/// @param ... Additional arguments passed to the function call.
#define vdl_bt_call(func, rtype, ...)                      \
    ({                                                     \
        rtype _rv = func(vdlint_bt_make(), ##__VA_ARGS__); \
        vdlint_bt_pop();                                   \
        _rv;                                               \
    })

/// @description Call a void function while maintaining the backtrace.
/// @details Users should create a `void foo_BT()` function and define a macro
/// `#define foo(...) vdl_bt_callvoid(foo_BT, ##__VA_ARGS__)` as a wrapper.
/// @param func (identifier). The function identifier.
/// @param ... Additional arguments passed to the function call.
#define vdl_bt_callvoid(func, ...)             \
    do {                                       \
        func(vdlint_bt_make(), ##__VA_ARGS__); \
        vdlint_bt_pop();                       \
    } while (0)


/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

/// @description A helper function to format backtrace.
/// @param s (char *). A mutable string.
/// @param num (const int). Number of whitespaces needed.
/// @nobacktrace
static inline const char *vdlint_bt_set_spaces(char *s, const int num)
{
    vdl_util_for_i(num) s[i] = ' ';
    s[num]                   = '\0';
    return s;
}

/// @description Print the backtrace.
/// @nobacktrace
static inline void vdl_bt_print(void)
{
    printf("Backtrace - %d stack frames:\n", VDLINT_GBT.NFRAME);
    char spaces[VDL_BT_FUNC_NAME_LIMIT] = {'\0'};
    int func_length[VDL_BT_STACK_LIMIT] = {0};
    int max_func_length                 = -1;
    vdl_util_for_i(VDLINT_GBT.NFRAME)
    {
        func_length[i] = (int) strlen(VDLINT_GBT.FUNC[i]);
        if (func_length[i] > max_func_length)
            max_func_length = func_length[i];
    }

    for (int i = VDLINT_GBT.NFRAME - 1; i >= 0; i--)
    {
        if (i != 0)
            printf("  ║═");
        else
            printf("  ╚═");
        printf("[%d] Calling <%.*s>%s from %s:%d\n",
               VDLINT_GBT.NFRAME - i - 1,
               (int) strlen(VDLINT_GBT.FUNC[i]) - 3,
               VDLINT_GBT.FUNC[i],
               vdlint_bt_set_spaces(spaces, max_func_length - (int) strlen(VDLINT_GBT.FUNC[i])),
               VDLINT_GBT.FILE[i], VDLINT_GBT.LINE[i]);
    }
}

#endif//VDL_VDLBT_H
