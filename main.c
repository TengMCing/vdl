#include "vecmem.h"

int main()
{
    vp v = vec_New(VEC_VP, 3);
    vp v0 = vec_New(VEC_CHAR, 3);
    vp v1 = vec_New(VEC_INT, 3);
    vp v2 = vec_New(VEC_VP, 4);
    vp v20 = vec_New(VEC_CHAR, 3);
    vp v21 = vec_New(VEC_CHAR, 3);
    vp v22 = vec_New(VEC_CHAR, 3);

    vec_Append(v, &v0);
    vec_Append(v, &v1);
    vec_Append(v, &v2);

    vec_Append(v2, &v20);
    vec_Append(v2, &v21);
    vec_Append(v2, &v22);
    // vec_Append(v2, &v);

    vec_GCDirectlyReachable(v);
    vec_GCDirectlyReachable(v2);
    vec_GCDirectlyUnreachable(v);
    vec_GCPrintArena(GDREACHABLE);
    vec_GCPrintArena(GREACHABLE);
    vec_GCPrintArena(GARENA);
    vec_GCCleanUp();
    vec_GCPrintArena(GDREACHABLE);
    vec_GCPrintArena(GREACHABLE);
    vec_GCPrintArena(GARENA);
    vec_GCKill();

    return 0;
}
