// #define VDL_EXCEPTION_DISABLE
// #define VDL_BACKTRACE_DISABLE

#pragma clang diagnostic ignored "-Wshadow"

#include "include/vdl.h"

int main(void)
{
    VDL_VECTOR_P v = vdl_Vector(vdl_Vector(1, 2, 3),
                                vdl_Vector(1.1, 2.1, 3.1),
                                vdl_Vector(NULL));
    vdl_UnsafeSetVectorPointer(vdl_GetVectorPointer(v, 2), 0, v);
    printf("%f\n", vdl_GetDouble(vdl_GetVectorPointer(v, 1), 2));
    vdl_PrintVectorTable(vdl_GlobalVar_VectorTable);
    vdl_DeclareDirectlyReachable(vdl_GetVectorPointer(v, 2));
    vdl_GarbageCollectorCleanUp();
    vdl_PrintVectorTable(vdl_GlobalVar_VectorTable);
    return 0;
}
