//
// Created by Patrick Li on 9/11/22.
//

#include "../../vdlerr.h"
#include "../test.h"

int test_ckerr(void);
void test_abort(void);

int main(void)
{
    // echo
    echo("Test vdl_geterr:");
    // expect(True)
    if (vdl_err_getcode() == VDLINT_GERR.CODE)
        test_printf("True");
    else
        test_printf("False");

    // echo
    echo("Test VDL_ERRMSG_ON:");
    // expect(1)
    test_printf("%d", VDL_ERR_MSG_ON);

    // echo
    echo("Test vdlint_ckerr, vdl_catch and vdl_finally:");
    test_ckerr();

    test_abort();
    return 0;
}

int test_ckerr(void)
{
    VDLINT_GERR.CODE = 1;
    vdlint_err_check();
VDL_EXCEPTION:
    // expect(Exception triggered!)
    test_printf("Exception triggered!");
    vdl_catch(2) test_printf("Incorrect!");
    // expect(Handled!)
    vdl_catch(1) test_printf("Handled!");
    // expect(0)
    test_printf("%d", vdl_err_getcode());
    VDLINT_GERR.CODE = 1;
    VDLINT_GERR.MSG  = "This is a mock error";
    // expect(This is a mock error)
    test_printf("%s", vdl_err_getmsg());
    // expect(Finally handled!)
    vdl_finally() test_printf("Finally handled!");
    // expect(0)
    test_printf("%d", vdl_err_getcode());
    return 1;
}

_Noreturn void test_abort(void)
{
    // echo
    echo("Test vdl_abort");
    // exit(1)
    // expect(Program aborted!)
    test_printf_wrapper(vdl_abort());
}
