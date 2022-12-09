#include "vdlmem.h"
#include <stdio.h>

#define foo() vdlint_Call(foo_BT, int)

static int foo_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    foo();
    return 1;
VDL_EXCEPTION:
    vdl_Finally() printf("done!");
    return 1;
}


int main(void)
{
    // foo();
    vdl_LocV(*"a", 2, 3);

    vdl_V(1, 2, 3);

    return 0;
VDL_EXCEPTION:
    vdl_Abort();
}

// TODO:ADD module (e.g. util) to name
