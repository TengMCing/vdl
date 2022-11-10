//
// Created by Patrick Li on 23/10/22.
//

#ifndef VDL_VDLBT_H
#define VDL_VDLBT_H


#include "vdlerror.h"
#include "vdlutil.h"
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Backtrace definition
 ----------------------------------------------------------------------------*/

/// @description Stack frame limit
#define VDL_BT_STACK_DEPTH_LIMIT 1000

/// @description Function name limit
#define VDL_BT_FUNC_NAME_LIMIT 10000

/// @description Backtrace struct.
/// @param file_name (const char *) File name.
/// @param line_num (int) Line number.
typedef struct vdl_bt
{
    const char *const file_name;
    const int line_num;
} vdl_bt;

/// @description Global backtrace.
/// @param line_num (int [VDL_BT_STACK_DEPTH_LIMIT]) Line number of calls.
/// @param func_name (const char *[VDL_BT_STACK_DEPTH_LIMIT]) Function names.
/// @param file_name (const char *[VDL_BT_STACK_DEPTH_LIMIT]) File names.
static struct
{
    int num_frame;
    int line_num[VDL_BT_STACK_DEPTH_LIMIT];
    const char *func_name[VDL_BT_STACK_DEPTH_LIMIT];
    const char *file_name[VDL_BT_STACK_DEPTH_LIMIT];
} VDL_INTERNAL_GBT = {0};

/*-----------------------------------------------------------------------------
 |  Macro to work with global backtrace
 ----------------------------------------------------------------------------*/

/// @description Get the number of frames in the global backtrace.
#define vdl_bt_GetFrameNum() ((int){VDL_INTERNAL_GBT.num_frame})

/// @description Get the line number of a call from the global backtrace.
/// @param frame (int). The target frame.
#define vdl_bt_GetLineNum(frame) ((frame) >= 0 && (frame) < VDL_INTERNAL_GBT.num_frame ? VDL_INTERNAL_GBT.line_num[frame] : 0)

/// @description Get the function name of a frame from the global backtrace.
/// @param frame (int). The target frame.
#define vdl_bt_GetFuncName(frame) ((frame) >= 0 && (frame) < VDL_INTERNAL_GBT.num_frame ? VDL_INTERNAL_GBT.func_name[frame] : "\0")

/// @description Get the file name of a frame from the global backtrace.
/// @param frame (int). The target frame.
#define vdl_bt_GetFileName(frame) ((frame) >= 0 && (frame) < VDL_INTERNAL_GBT.num_frame ? VDL_INTERNAL_GBT.file_name[frame] : "\0")

/*-----------------------------------------------------------------------------
 |  Macros to push frames to backtrace and pop frames from backtrace
 ----------------------------------------------------------------------------*/

/// @description Make a backtrace struct to capture the current file name and line number.
#define vdl_internal_bt_Make()                      \
    (vdl_bt)                                        \
    {                                               \
        .file_name = __FILE__, .line_num = __LINE__ \
    }

/// @description Push the function to the top of the backtrace stack.
/// @details This should be used at the beginning of every function that
/// needs to be traced.
/// @param bt (vdl_bt). The backtrace.
#define vdl_bt_Push(bt)                                                                \
    do {                                                                               \
        VDL_INTERNAL_GBT.num_frame++;                                                  \
                                                                                       \
        /* Prevent the function to run if the caller has not handled the exception. */ \
        vdl_internal_CheckError();                                                     \
                                                                                       \
        /* Check if the stack depth exceed the maximum limit. */                       \
        if (VDL_INTERNAL_GBT.num_frame > VDL_BT_STACK_DEPTH_LIMIT)                     \
        {                                                                              \
            VDL_INTERNAL_GERROR = VDL_ERROR_EXCEED_STACK_DEPTH_LIMIT;                  \
            vdl_bt_Print();                                                            \
            printf("[%d] Error raised by <%s> at %s:%d: Exceed stack depth limit!\n",  \
                   VDL_INTERNAL_GERROR,                                                \
                   __func__,                                                           \
                   __FILE__,                                                           \
                   __LINE__);                                                          \
            VDL_THROW;                                                                 \
        }                                                                              \
                                                                                       \
        VDL_INTERNAL_GBT.line_num[VDL_INTERNAL_GBT.num_frame - 1]  = (bt).line_num;    \
        VDL_INTERNAL_GBT.func_name[VDL_INTERNAL_GBT.num_frame - 1] = __func__;         \
        VDL_INTERNAL_GBT.file_name[VDL_INTERNAL_GBT.num_frame - 1] = (bt).file_name;   \
    } while (0)

/// @description Pop an item from the backtrace stack.
#define vdl_internal_bt_Pop()         \
    do {                              \
        VDL_INTERNAL_GBT.num_frame--; \
        vdl_internal_CheckError();    \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Caller to automatically make and pop backtrace
 ----------------------------------------------------------------------------*/

/// @description Call a function while maintaining the backtrace.
/// @param func (identifier). The function name.
/// @param return_type (type). The return type.
/// @param ... Additional arguments passed to the function call.
#define vdl_bt_Caller(func, return_type, ...)                          \
    ({                                                                 \
        return_type _rv = func(vdl_internal_bt_Make(), ##__VA_ARGS__); \
        vdl_internal_bt_Pop();                                         \
        _rv;                                                           \
    })

/// @description Call a void function while maintaining the backtrace.
/// @param func (identifier). The function name.
/// @param ... Additional arguments passed to the function call.
#define vdl_bt_CallerNoReturn(func, ...)             \
    do {                                             \
        func(vdl_internal_bt_Make(), ##__VA_ARGS__); \
        vdl_internal_bt_Pop();                       \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

/// @description A helper function to format backtrace.
/// @param s (char *). A mutable string.
/// @param num (const int). Number of whitespaces needed.
static inline const char *vdl_internal_bt_Whitespaces(char *s, const int num)
{
    vdl_For_i(num) s[i] = ' ';
    s[num]              = '\0';
    return s;
}

/// @description Print the backtrace.
static inline void vdl_bt_Print(void)
{
    printf("Backtrace - %d stack frames:\n", VDL_INTERNAL_GBT.num_frame);
    char whitespaces[VDL_BT_FUNC_NAME_LIMIT]  = {'\0'};
    int func_length[VDL_BT_STACK_DEPTH_LIMIT] = {0};
    int max_func_length                       = -1;
    vdl_For_i(VDL_INTERNAL_GBT.num_frame)
    {
        func_length[i] = (int) strlen(VDL_INTERNAL_GBT.func_name[i]);
        if (func_length[i] > max_func_length)
            max_func_length = func_length[i];
    }

    for (int i = VDL_INTERNAL_GBT.num_frame - 1; i >= 0; i--)
    {
        if (i != 0)
            printf("  ║═");
        else
            printf("  ╚═");
        printf("[%d] Calling <%.*s>%s from %s:%d\n",
               VDL_INTERNAL_GBT.num_frame - i - 1,
               (int) strlen(VDL_INTERNAL_GBT.func_name[i]) - 3,
               VDL_INTERNAL_GBT.func_name[i],
               vdl_internal_bt_Whitespaces(whitespaces, max_func_length - (int) strlen(VDL_INTERNAL_GBT.func_name[i])),
               VDL_INTERNAL_GBT.file_name[i], VDL_INTERNAL_GBT.line_num[i]);
    }
}

#endif//VDL_VDLBT_H
