//
// Created by Patrick Li on 23/10/22.
//

#include "../../vdlbt.h"
#include "../test.h"
#include <limits.h>

#define MAX_PATH_LENGTH 10000

static int foo_BT(vdl_bt bt);
#define foo() vdlint_Call(foo_BT, int)
static void foo_noreturn_BT(vdl_bt bt);
#define foo_noreturn() vdlint_CallVoid(foo_noreturn_BT)
static void foo_error_BT(vdl_bt bt);
#define foo_error() vdlint_CallVoid(foo_error_BT)

int main(void)
{
    // echo
    echo("Test vdl_MakeBT:");
    // expect(True)
    if (strncmp(__FILE__, vdlint_MakeBacktrace().file, MAX_PATH_LENGTH) == 0)
        test_printf("True");
    else
        test_printf("False");
    // expect(28)
    test_printf("%d", vdlint_MakeBacktrace().line);

    // echo
    echo("Test vdl_bt_Push and vdl_bt_Pop:");
    foo();

    // defer expect
    test_printf("%d", VDLINT_GBT.FRAME_NUM);

    foo_error();

    return 0;
VDL_EXCEPTION:
    // exit(1)
    test_printf_wrapper(vdl_Abort());
}

static int foo_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    // expect(32)
    test_printf("%d", vdl_GetBacktraceLineNum(0));
    // expect(1)
    test_printf("%d", vdl_GetBacktraceFrameNum());
    // expect(True)
    if (strncmp(__FILE__, vdl_GetBacktraceFileName(0), MAX_PATH_LENGTH) == 0)
        test_printf("True");
    else
        test_printf("False");
    // expect(foo_BT)
    test_printf("%s", VDLINT_GBT.FUNC[0]);
    foo_noreturn();
    return 0;
VDL_EXCEPTION:
    test_printf("foo_BT Exception triggered!");
    return INT_MAX;
}

static void foo_noreturn_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    // expect(59)
    test_printf("%d", VDLINT_GBT.LINE[1]);
    // expect(2)
    test_printf("%d", VDLINT_GBT.FRAME_NUM);
    // expect(True)
    if (strncmp(__FILE__, VDLINT_GBT.FILE[1], MAX_PATH_LENGTH) == 0)
        test_printf("True");
    else
        test_printf("False");
    // expect(foo_noreturn_BT)
    test_printf("%s", VDLINT_GBT.FUNC[1]);
    // expect start
    // Backtrace - 2 stack frames:
    //   ║═[0] Calling <foo_noreturn> from test_vdlbt/test_vdlbt.c:59
    //   ╚═[1] Calling <foo>          from test_vdlbt/test_vdlbt.c:32
    // expect end
    test_printf_wrapper(vdlint_PrintBacktrace());
    return;
VDL_EXCEPTION:
    test_printf("foo_noreturn_BT Exception triggered!");
    return;
}

// Line 35
// expect(0)

static void foo_error_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    // expect(foo_error_BT before error!)
    test_printf("foo_error_BT before error!");
    if (1 != 0)
    {
        VDLINT_GERR.CODE = 1;
        // expect start
        // Backtrace - 1 stack frames:
        //   ╚═[0] Calling <foo_error> from test_vdlbt/test_vdlbt.c:37
        // expect end
        test_printf_wrapper(vdlint_PrintBacktrace());
        goto VDL_EXCEPTION;
    }
    test_printf("foo_error_BT no error!");
    return;
VDL_EXCEPTION:
    // expect(foo_noreturn_BT Exception triggered!)
    test_printf("foo_noreturn_BT Exception triggered!");
    return;
}

// expect(Program aborted!)
