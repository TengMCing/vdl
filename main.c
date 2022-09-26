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

    vp_array(v->data)[0] = v0;
    vp_array(v->data)[1] = v1;
    vp_array(v->data)[2] = v2;
    v->size = 3;
    v2->size = 3;
    vp_array(v2->data)[0] = v20;
    vp_array(v2->data)[1] = v21;
    vp_array(v2->data)[2] = v22;
    // vp_array(v2->data)[3] = v;

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
