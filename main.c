#include "vdlbt.h"
#include "vdlmem.h"
#include "vdlportal.h"
#include "vdlseq.h"

int main()
{
    // vp v = vec_New(VEC_VP, 3);
    // vp v0 = vec_New(VEC_CHAR, 3);
    // vp v1 = vec_New(VEC_INT, 3);
    // vp v2 = vec_New(VEC_VP, 4);
    // vp v20 = vec_New(VEC_CHAR, 3);
    // vp v21 = vec_New(VEC_CHAR, 3);
    // vp v22 = vec_New(VEC_CHAR, 3);
    //
    // vec_Append(v, &v0);
    // vec_Append(v, &v1);
    // vec_Append(v, &v2);
    //
    // vec_Append(v2, &v20);
    // vec_Append(v2, &v21);
    // vec_Append(v2, &v22);
    //
    // vec_Pop(v2, 0);
    // // printf("%p\n", vdl_vp_Array(v2->data)[1]);
    //
    // vec_Extend(v0, 3, &(char[]){65, 66, 67});
    // vec_Concatenate(v2, v);
    // // vec_Append(v2, &v);
    //
    // vec_GCDirectlyReachable(v);
    // vec_GCDirectlyReachable(v2);
    // vec_GCDirectlyUnreachable(v);
    // vec_GCPrintArena(VDL_GDREACHABLE);
    // vec_GCPrintArena(VDL_GREACHABLE);
    // vec_GCPrintArena(VDL_GARENA);
    // vec_GCCleanUp();
    // vec_GCPrintArena(VDL_GDREACHABLE);
    // vec_GCPrintArena(VDL_GREACHABLE);
    // vec_GCPrintArena(VDL_GARENA);
    // vec_GCKill();

    vdl_V(1, 2, 3);

    vdl_vp v = vdl_V(vdl_V(1, 2, 3), vdl_V(1.1, 2.2, 3.3), vdl_V(*"a", *"b", *"c"));

    // vdl_vp vv = vdl_vp_ArgV(vdl_ArgV(1, 2, 3, 4, 5),
    //                         vdl_ArgV(1.1, 2.2, 3.3),
    //                         vdl_ArgV(*"a", *"b", *"c"));

    printf("%s\n", vdl_TypeStr(vdl_ArgV(*"a", *"b", *"c")));

    printf("%s %zu\n", vdl_ModeStr(v), vdl_SizeOfData(vdl_GetVp(v, 0)));

    printf("%f\n", vdl_GetDouble(vdl_ArgV(1.1, 2.2, 3.3), 0));

    printf("%d\n", vdl_GetInt(vdl_GetVp(v, 0), 2));

    vdl_Indexing(v, vdl_ArgV(0));

    printf("%s\n", vdl_TypeStr(vdl_Indexing(v, vdl_IntSeq(0, 10))));
    vdl_gc_Kill();

    return 0;
}
