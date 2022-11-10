//
// Created by Patrick Li on 9/11/22.
//

#include "../../vdlerror.h"

#include "../test.h"

int test_CheckError(void);
void test_Abort(void);

int main(void)
{
    // echo
    echo("Test vdl_GetError:");
    // expect(True)
    if (vdl_GetError() == VDL_INTERNAL_GERROR)
        test_printf("True");
    else
        test_printf("False");

    // echo
    echo("Test vdl_internal_CheckError, vdl_Except and vdl_Finally:");
    test_CheckError();

    test_Abort();
    return 0;
}

int test_CheckError(void)
{
    VDL_INTERNAL_GERROR = 1;
    vdl_internal_CheckError();
VDL_EXCEPTION:
    // expect(Exception triggered!)
    test_printf("Exception triggered!");
    vdl_Except(2) test_printf("Incorrect!");
    // expect(Handled!)
    vdl_Except(1) test_printf("Handled!");
    // expect(0)
    test_printf("%d", vdl_GetError());
    VDL_INTERNAL_GERROR = 1;
    // expect(Finally handled!)
    vdl_Finally() test_printf("Finally handled!");
    // expect(0)
    test_printf("%d", vdl_GetError());
    return 1;
}

_Noreturn void test_Abort(void)
{
    // echo
    echo("Test vdl_Abort");
    // exit(1)
    // expect(Program aborted!)
    test_printf_wrapper(vdl_Abort());
}
