// #define VDL_EXCEPTION_DISABLE
// #define VDL_BACKTRACE_DISABLE

#pragma clang diagnostic ignored "-Wshadow"

#include "include/vdl.h"

int main(void)
{
    vdl_vector_primitive_NewByDouble(1.1);
    VDL_VECTOR_P v = vdl_vector_primitive_New(vdl_vector_primitive_New(1, 2, 3),
                                              vdl_vector_primitive_New(1.1, 2.1, 3.1),
                                              vdl_vector_primitive_New(NULL));
    vdl_vector_primitive_UnsafeSetVectorPointer(vdl_vector_primitive_GetVectorPointer(v, 2), 0, v);
    printf("%f\n", vdl_vector_primitive_GetDouble(vdl_vector_primitive_GetVectorPointer(v, 1), 2));
    vdl_PrintVectorTable(vdl_GlobalVar_VectorTable);
    vdl_DeclareDirectlyReachable(vdl_vector_primitive_GetVectorPointer(v, 2));
    vdl_GarbageCollectorCleanUp();
    vdl_PrintVectorTable(vdl_GlobalVar_VectorTable);
    return 0;
}
