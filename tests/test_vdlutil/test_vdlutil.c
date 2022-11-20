//
// Created by Patrick Li on 23/10/22.
//

#include "../../vdldef.h"
#include "../../vdlutil.h"

#include "../test.h"
#include <stdio.h>

int main(void)
{

    // echo
    echo("Test vdl_fnarg:");
    // expect(1)
    test_printf("%d", vdl_util_fnarg(1, 1, 2, 3));
    // expect(12)
    test_printf("%d%d", vdl_util_fnarg(2, 1, 2, 3));

    // echo
    echo("Test vdl_narg:");
    // expect(3)
    test_printf("%d", vdl_util_narg(1, 2, 3));
    // expect(0)
    test_printf("%d", vdl_util_narg());

    // echo
    echo("Test vdl_rmfnarg:");
    // expect(2)
    test_printf("%d", vdl_util_rmfnarg(1, 1, 2));
    // expect(23)
    test_printf("%d%d", vdl_util_rmfnarg(1, 1, 2, 3));

    // echo
    echo("Test vdl_for_i:");
    // expect(1)
    vdl_util_for_i(1) test_printf("%d", 1);
    vdl_util_for_i(0) test_printf("%d", 1);

    // exit(0)
    return 0;
}
