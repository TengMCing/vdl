//
// Created by Patrick Li on 23/10/22.
//

#ifndef VDL_VDLBT_H
#define VDL_VDLBT_H


#include "vdlutil.h"
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Error definition
 ----------------------------------------------------------------------------*/

static int VDL_GERROR = 0;

/*-----------------------------------------------------------------------------
 |  Check error
 ----------------------------------------------------------------------------*/

#define vdl_CheckError()        \
    do {                        \
        if (VDL_GERROR != 0)    \
            goto VDL_EXCEPTION; \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Except
 ----------------------------------------------------------------------------*/

#define vdl_Except(error_code) for (int _except = VDL_GERROR != (error_code); _except < 1; VDL_GERROR = 0, _except++)

#define vdl_Finally() for (int _except = 0; _except < 1; VDL_GERROR = 0, _except++)

#define vdl_ErrorResolved() \
    do {                    \
        VDL_GERROR = 0;     \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Exit
 ----------------------------------------------------------------------------*/

_Noreturn static inline void vdl_Abort(void)
{
    printf("Program aborted!\n");
    exit(EXIT_FAILURE);
}

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

/// @description Make a backtrace to capture the current file name and line number.
#define vdl_MakeBT()                                \
    (vdl_bt)                                        \
    {                                               \
        .file_name = __FILE__, .line_num = __LINE__ \
    }

/// @description Push the function to the top of the backtrace stack.
/// @details This should be used at the beginning of every function that
/// needs to be traced.
#define vdl_PushBT(bt)                                         \
    do {                                                       \
        VDL_GBT.line_num[VDL_GBT.num_frame]  = (bt).line_num;  \
        VDL_GBT.func_name[VDL_GBT.num_frame] = __func__;       \
        VDL_GBT.file_name[VDL_GBT.num_frame] = (bt).file_name; \
        VDL_GBT.num_frame++;                                   \
        vdl_CheckError();                                      \
    } while (0)

/// @description Pop an item from the backtrace stack
#define vdl_PopBT()          \
    do {                     \
        VDL_GBT.num_frame--; \
        vdl_CheckError();    \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Caller to automatically make and pop backtrace
 ----------------------------------------------------------------------------*/

/// @description Call a function while maintaining the backtrace.
#define vdl_Caller(func, return_type, ...)                   \
    ({                                                       \
        return_type _rv = func(vdl_MakeBT(), ##__VA_ARGS__); \
        vdl_PopBT();                                         \
        _rv;                                                 \
    })

/// @description Call a void function while maintaining the backtrace.
#define vdl_CallerNoReturn(func, ...)      \
    do {                                   \
        func(vdl_MakeBT(), ##__VA_ARGS__); \
        vdl_PopBT();                       \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

static inline const char *vdl_bt_Whitespaces(char *s, const int num)
{
    vdl_For_i(num) s[i] = ' ';
    s[num]              = '\0';
    return s;
}

/// @description Print the backtrace.
static inline void vdl_bt_Print(void)
{
    printf("Backtrace - %d stack frames:\n", VDL_GBT.num_frame);
    char whitespaces[VDL_FUNC_NAME_LIMIT]  = {'\0'};
    int func_length[VDL_STACK_FRAME_LIMIT] = {0};
    int max_func_length                    = -1;
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
