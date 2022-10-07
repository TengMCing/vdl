//
// Created by Patrick Li on 5/10/22.
//

#ifndef VDL_VDLBT_H
#define VDL_VDLBT_H

#include "vdlutil.h"
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Backtrace definition
 ----------------------------------------------------------------------------*/

#define VDL_STACK_FRAME_LIMIT 1000
#define VDL_FUNC_NAME_LIMIT 10000

/// @description Backtrace struct.
/// @param file_name (const char *) File name.
/// @param line_num (int) Line number.
typedef struct vdl_bt
{
    const char *const file_name;
    const int line_num;
} vdl_bt;

/// @description Global backtrace.
static struct
{
    int num_frame;
    int line_num[VDL_STACK_FRAME_LIMIT];
    const char *func_name[VDL_STACK_FRAME_LIMIT];
    const char *file_name[VDL_STACK_FRAME_LIMIT];
} VDL_GBT = {0};

/*-----------------------------------------------------------------------------
 |  Macro to work with backtrace
 ----------------------------------------------------------------------------*/

/// @description Push the function to the top of the backtrace stack.
/// @details This should be used at the beginning of every function that
/// needs to be traced.
#define vdl_PushBT(bt)                                         \
    do {                                                       \
        VDL_GBT.line_num[VDL_GBT.num_frame]  = (bt).line_num;  \
        VDL_GBT.func_name[VDL_GBT.num_frame] = __func__;       \
        VDL_GBT.file_name[VDL_GBT.num_frame] = (bt).file_name; \
        VDL_GBT.num_frame++;                                   \
    } while (0)

/// @description Remove the top frame from the backtrace stack and return the value.
/// @details The return value should not contain any further calls to functions.
#define vdl_ReturnConst(object) \
    do {                        \
        VDL_GBT.num_frame--;    \
        return object;          \
    } while (0)

/// @description Automatically push and pop the backtrace stack.
/// @details This macro should be used in every function needs to be traced.
// #define vdl_WithBT() for (int c = (VDL_GBT.line_num[VDL_GBT.num_frame]  = (bt).line_num,  \
//                                    VDL_GBT.func_name[VDL_GBT.num_frame] = __func__,       \
//                                    VDL_GBT.file_name[VDL_GBT.num_frame] = (bt).file_name, \
//                                    VDL_GBT.num_frame++,                                   \
//                                    0);                                                    \
//                           c < 1;                                                          \
//                           (VDL_GBT.num_frame--, c++))

#define vdl_bt_Call(func, ...) func((vdl_bt){.file_name = __FILE__, .line_num = __LINE__}, ##__VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

static inline const char *vdl_bt_Whitespaces(char *s, const int num)
{
    vdl_For_i(num) s[i] = ' ';
    s[num]              = '\0';
    return s;
}

static inline void vdl_bt_Print(void)
{
    printf("Backtrace - %d stack frames:\n", VDL_GBT.num_frame);
    char whitespaces[VDL_FUNC_NAME_LIMIT] = {'\0'};
    int func_length[VDL_GBT.num_frame];
    int max_func_length = -1;
    vdl_For_i(VDL_GBT.num_frame)
    {
        func_length[i] = (int) strlen(VDL_GBT.func_name[i]);
        if (func_length[i] > max_func_length)
            max_func_length = func_length[i];
    }

    // printf("  ║═[0] Starting at <main>\n");
    for (int i = VDL_GBT.num_frame - 1; i >= 0; i--)
    {
        if (i != 0)
            printf("  ║═");
        else
            printf("  ╚═");
        printf("[%d] Calling <%.*s>%s from %s:%d\n",
               VDL_GBT.num_frame - i - 1,
               (int) strlen(VDL_GBT.func_name[i]) - 3,
               VDL_GBT.func_name[i],
               vdl_bt_Whitespaces(whitespaces, max_func_length - (int) strlen(VDL_GBT.func_name[i])),
               VDL_GBT.file_name[i], VDL_GBT.line_num[i]);
    }
}

#endif//VDL_VDLBT_H
