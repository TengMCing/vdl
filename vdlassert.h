//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLASSERT_H
#define VDL_VDLASSERT_H

#include "vdldef.h"
#include "vdlutil.h"

#define vdl_assert_NullPointer(object) vdl_Expect(object != NULL, printf("NULL pointer provided!"))
#define vdl_assert_IndexOutOfBound(v, i) vdl_Expect(i >= 0 && i < v->length, printf("Index out of bound! Index [%d] not in [0, %d]!", i, v->length))
#define vdl_assert_UnknownType(type) vdl_Expect(type >= VDL_TYPE_CHAR && type <= VDL_TYPE_VP, printf("Unknown type [%d] provided!", type))
#define vdl_assert_NotStackAllocated(mode) vdl_Expect(mode == VDL_MODE_STACK, printf("Storage mode [%d] is not stack allocation!", mode))
#define vdl_assert_NotHeapAllocated(mode) vdl_Expect(mode == VDL_MODE_HEAP, printf("Storage mode [%d] is not heap allocation!", mode))
#define vdl_assert_IncompatibleType(type1, type2) vdl_Expect(type1 == type2, printf("Incompatible types (%d != %d) provided!", type1, type2))
#define vdl_assert_ZeroCapacity(capacity) vdl_Expect(capacity > 0, printf("Zero or negative capacity [%d] provided!", capacity))
#define vdl_assert_ZeroObjects(num_object) vdl_Expect(num_object > 0, printf("Zero or negative objects [%d] provided!", num_object))

#define vdl_assert_UninitializedGC() vdl_Expect(VDL_GARENA != NULL && VDL_GREACHABLE != NULL && VDL_GDREACHABLE != NULL, printf("GC is uninitialized!"))
#define vdl_assert_InconsistentGC() vdl_Expect(((VDL_GARENA == NULL) + (VDL_GREACHABLE == NULL) + (VDL_GDREACHABLE == NULL)) % 3 == 0, printf("Garbage collector in an inconsistent state"))
#define vdl_assert_Unimplemented() vdl_Expect(0, printf("This function is unimplemented!"))


static inline void vdl_HealthCheck(const vdl_vec *const v)
{
    vdl_assert_NullPointer(v);
    vdl_assert_UnknownType(v->type);
}

#endif//VDL_VDLASSERT_H
