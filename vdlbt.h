//
// Created by Patrick Li on 5/10/22.
//

#ifndef VDL_VDLBT_H
#define VDL_VDLBT_H

#include "vdlutil.h"
#include <string.h>

#define VDL_STACK_FRAME_LIMIT 1000
#define VDL_FUNC_NAME_LIMIT 255
#define VDL_FILE_NAME_LIMIT 255 * 2

typedef struct vdl_bt
{
    int num_frame;
    char func_name[VDL_STACK_FRAME_LIMIT][VDL_FUNC_NAME_LIMIT];
    char file_name[VDL_STACK_FRAME_LIMIT][VDL_FILE_NAME_LIMIT];
    int line_num[VDL_STACK_FRAME_LIMIT];
} vdl_bt;

static vdl_bt VDL_GBT = {0};

#define vdl_Return(object)   \
    do {                     \
        VDL_GBT.num_frame--; \
        return object;       \
    } while (0)

#define vdl_CallBT(func, ...) (VDL_GBT.line_num[VDL_GBT.num_frame] = __LINE__,                               \
                               strncpy(VDL_GBT.func_name[VDL_GBT.num_frame], #func, VDL_FUNC_NAME_LIMIT),    \
                               strncpy(VDL_GBT.file_name[VDL_GBT.num_frame], __FILE__, VDL_FILE_NAME_LIMIT), \
                               VDL_GBT.num_frame++,                                                          \
                               func##_BT(__VA_ARGS__))

static inline const char *vdl_bt_Whitespaces(char *s, const int num)
{
    vdl_For_i(num) s[i] = ' ';
    s[num]              = '\0';
    return s;
}

static inline void vdl_bt_Print(void)
{
    printf("Backtrace - %d function calls found:\n", VDL_GBT.num_frame);
    char whitespaces[VDL_FUNC_NAME_LIMIT] = {'\0'};
    int func_length[VDL_GBT.num_frame];
    int max_func_length = -1;
    vdl_For_i(VDL_GBT.num_frame)
    {
        func_length[i] = (int) strlen(VDL_GBT.func_name[i]);
        if (func_length[i] > max_func_length)
            max_func_length = func_length[i];
    }

    vdl_For_i(VDL_GBT.num_frame)
    {
        if (i != VDL_GBT.num_frame - 1)
            printf("  ║═[%d] <%s>%s at %s:%d\n", i + 1, VDL_GBT.func_name[i], vdl_bt_Whitespaces(whitespaces, max_func_length - (int) strlen(VDL_GBT.func_name[i])), VDL_GBT.file_name[i], VDL_GBT.line_num[i]);
        else
            printf("  ╚═[%d] <%s>%s at %s:%d\n", i + 1, VDL_GBT.func_name[i], vdl_bt_Whitespaces(whitespaces, max_func_length - (int) strlen(VDL_GBT.func_name[i])), VDL_GBT.file_name[i], VDL_GBT.line_num[i]);
    }
}

#endif//VDL_VDLBT_H
