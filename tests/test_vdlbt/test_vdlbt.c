//
// Created by Patrick Li on 23/10/22.
//

#include "../../vdlbt.h"
#include "../test.h"
#include <limits.h>

#define MAX_PATH_LENGTH 10000

static int foo_BT(vdl_bt bt);
#define foo() vdl_bt_Caller(foo_BT, int)
static void foo_noreturn_BT(vdl_bt bt);
#define foo_noreturn() vdl_bt_CallerNoReturn(foo_noreturn_BT)
static void foo_error_BT(vdl_bt bt);
#define foo_error() vdl_bt_CallerNoReturn(foo_error_BT)

int main(void)
{
    // echo
    echo("Test vdl_MakeBT:");
    // expect(True)
    if (strncmp(__FILE__, vdl_internal_bt_Make().file_name, MAX_PATH_LENGTH) == 0)
        test_printf("True");
    else
        test_printf("False");
    // expect(28)
    test_printf("%d", vdl_internal_bt_Make().line_num);

    // echo
    echo("Test vdl_bt_Push and vdl_bt_Pop:");
    foo();

    // defer expect
    test_printf("%d", VDL_INTERNAL_GBT.num_frame);

    foo_error();

    return 0;
VDL_EXCEPTION:
    // exit(1)
    test_printf_wrapper(vdl_Abort());
}

static int foo_BT(vdl_bt bt)
{
    vdl_bt_Push(bt);
    // expect(32)
    test_printf("%d", vdl_bt_GetLineNum(0));
    // expect(1)
    test_printf("%d", vdl_bt_GetFrameNum());
    // expect(True)
    if (strncmp(__FILE__, vdl_bt_GetFileName(0), MAX_PATH_LENGTH) == 0)
        test_printf("True");
    else
        test_printf("False");
    // expect(foo_BT)
    test_printf("%s", VDL_INTERNAL_GBT.func_name[0]);
    foo_noreturn();
    return 0;
VDL_EXCEPTION:
    test_printf("foo_BT exception triggered!");
    return INT_MAX;
}

static void foo_noreturn_BT(vdl_bt bt)
{
    vdl_bt_Push(bt);
    // expect(59)
    test_printf("%d", VDL_INTERNAL_GBT.line_num[1]);
    // expect(2)
    test_printf("%d", VDL_INTERNAL_GBT.num_frame);
    // expect(True)
    if (strncmp(__FILE__, VDL_INTERNAL_GBT.file_name[1], MAX_PATH_LENGTH) == 0)
        test_printf("True");
    else
        test_printf("False");
    // expect(foo_noreturn_BT)
    test_printf("%s", VDL_INTERNAL_GBT.func_name[1]);
    // expect start
    // Backtrace - 2 stack frames:
    //   ║═[0] Calling <foo_noreturn> from test_vdlbt/test_vdlbt.c:59
    //   ╚═[1] Calling <foo>          from test_vdlbt/test_vdlbt.c:32
    // expect end
    test_printf_wrapper(vdl_bt_Print());
    return;
VDL_EXCEPTION:
    test_printf("foo_noreturn_BT exception triggered!");
    return;
}

// Line 35
// expect(0)

static void foo_error_BT(vdl_bt bt)
{
    vdl_bt_Push(bt);
    // expect(foo_error_BT before error!)
    test_printf("foo_error_BT before error!");
    if (1 != 0)
    {
        VDL_INTERNAL_GERROR = 1;
        // expect start
        // Backtrace - 1 stack frames:
        //   ╚═[0] Calling <foo_error> from test_vdlbt/test_vdlbt.c:37
        // expect end
        test_printf_wrapper(vdl_bt_Print());
        goto VDL_EXCEPTION;
    }
    test_printf("foo_error_BT no error!");
    return;
VDL_EXCEPTION:
    // expect(foo_noreturn_BT exception triggered!)
    test_printf("foo_noreturn_BT exception triggered!");
    return;
}

// expect(Program aborted!)
