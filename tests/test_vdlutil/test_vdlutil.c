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
    test_printf("%d", vdlint_FirstNArg(1, 1, 2, 3));
    // expect(12)
    test_printf("%d%d", vdlint_FirstNArg(2, 1, 2, 3));

    // echo
    echo("Test vdl_narg:");
    // expect(3)
    test_printf("%d", vdlint_CountArg(1, 2, 3));
    // expect(0)
    test_printf("%d", vdlint_CountArg());

    // echo
    echo("Test vdl_rmfnarg:");
    // expect(2)
    test_printf("%d", vdlint_RmFirstNArg(1, 1, 2));
    // expect(23)
    test_printf("%d%d", vdlint_RmFirstNArg(1, 1, 2, 3));

    // echo
    echo("Test vdl_for_i:");
    // expect(1)
    vdlint_for_i(1) test_printf("%d", 1);
    vdlint_for_i(0) test_printf("%d", 1);

    // expect(2)
    int x = 0;
    vdlint_for_i(-1, 1) x++;
    test_printf("%d", x);

    // expect(2)
    x = 0;
    vdlint_for_i(0, 4, 2) x++;
    test_printf("%d", x);

    // exit(0)
    return 0;
}
