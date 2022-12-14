#include "vdlportal.h"
#include <stdio.h>

// #define foo() vdlint_e_Call(foo_BT, int)
//
// static int foo_BT(vdl_fr bt)
// {
//     vdl_e_PushFrame(bt);
//     foo();
//     return 1;
// VDL_EXCEPTION:
//     vdl_Finally() printf("done!");
//     return 1;
// }

int main(void)
{
    vdl_vp v = vdl_e_V(vdl_e_V(1, 2, 3),
                       vdl_e_V(1.1, 2.2, 3.3),
                       vdl_e_V(*"a", *"b", *"c"),
                       vdl_e_V(NULL, NULL, NULL));

    vdl_vp vv = vdl_e_V(vdl_e_Get(v, 3), vdl_e_Get(v, 3), vdl_e_Get(v, 3), vdl_e_Get(v, 3));

    vdl_e_Set(vdl_e_Get(v, 3), 0, vv);

    printf("%d\n", v->len);

    vdl_ne_GCDirectlyReachable(vdl_e_Get(v, 3));

    vdl_ne_GCCleanUp();
    vdl_ne_PrintArena(VDLINT_GARENA);

    return 0;

VDL_EXCEPTION:
    vdl_Abort();
}

// TODO:ADD module (e.g. util) to name
