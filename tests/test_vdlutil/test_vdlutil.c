//
// Created by Patrick Li on 23/10/22.
//

#include "../../vdldef.h"
#include "../../vdlutil.h"

#include "../test.h"

int main(void)
{

    // echo
    echo("Test vdl_FirstNArgs:");
    // expect(1)
    test_printf("%d", vdl_FirstNArgs(1, 1, 2, 3));
    // expect(12)
    test_printf("%d%d", vdl_FirstNArgs(2, 1, 2, 3));

    // echo
    echo("Test vdl_CountNArgs:");
    // expect(3)
    test_printf("%d", vdl_CountArgs(1, 2, 3));
    // expect(0)
    test_printf("%d", vdl_CountArgs());

    // echo
    echo("Test vdl_RmFirstNArgs:");
    // expect(2)
    test_printf("%d", vdl_RmFirstNArgs(1, 1, 2));
    // expect(23)
    test_printf("%d%d", vdl_RmFirstNArgs(1, 1, 2, 3));

    // echo
    echo("Test vdl_For_i:");
    // expect(1)
    vdl_For_i(1) test_printf("%d", 1);
    vdl_For_i(0) test_printf("%d", 1);

    // exit(0)
    return 0;
}
