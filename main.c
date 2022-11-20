#include "vdlbt.h"
#include <stdio.h>

#define foo() vdl_bt_call(foo_BT, int)

static int foo_BT(vdl_bt bt)
{
    vdl_bt_push(bt);
    foo();
    return 1;
VDL_EXCEPTION:
    vdl_err_finally() printf("done!");
    return 1;
}


int main(void)
{
    foo();
    return 0;
VDL_EXCEPTION:
    vdl_err_abort();
}

// TODO:ADD module (e.g. util) to name
