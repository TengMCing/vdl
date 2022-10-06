//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLASSERT_H
#define VDL_VDLASSERT_H

#include "vdlbt.h"
#include "vdldef.h"

/*-----------------------------------------------------------------------------
 |  Expect
 ----------------------------------------------------------------------------*/

#define vdl_Expect(expr, errmsg)                                                                                     \
    do {                                                                                                             \
        if ((expr) != 1)                                                                                             \
        {                                                                                                            \
            vdl_bt_Print();                                                                                          \
            printf("Error raised by <%s> at %s:%d: ", VDL_GBT.func_name[VDL_GBT.num_frame - 1], __FILE__, __LINE__); \
            errmsg;                                                                                                  \
            printf("\n");                                                                                            \
            exit(EXIT_FAILURE);                                                                                      \
        }                                                                                                            \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Assert
 ----------------------------------------------------------------------------*/

#define vdl_assert_NullPointer(object) vdl_Expect(object != NULL, printf("NULL pointer provided!"))
#define vdl_assert_IndexOutOfBound(v, i) vdl_Expect(i >= 0 && i < v->length, printf("Index out of bound! Index [%d] not in [0, %d]!", i, v->length))
#define vdl_assert_UnknownType(type) vdl_Expect(type >= VDL_TYPE_CHAR && type <= VDL_TYPE_VP, printf("Unknown type [%d] provided!", type))
#define vdl_assert_NotStackAllocated(mode) vdl_Expect(mode == VDL_MODE_STACK, printf("Storage mode [%d] is not stack allocation!", mode))
#define vdl_assert_NotHeapAllocated(mode) vdl_Expect(mode == VDL_MODE_HEAP, printf("Storage mode [%d] is not heap allocation!", mode))
#define vdl_assert_IncompatibleType(type1, type2) vdl_Expect(type1 == type2, printf("Incompatible types (%d != %d) provided!", type1, type2))
#define vdl_assert_ZeroCapacity(capacity) vdl_Expect(capacity > 0, printf("Zero or negative capacity [%d] provided!", capacity))
#define vdl_assert_ZeroLengthVector(v) vdl_Expect(v->length > 0, printf("Vector of zero length [%d] provided!", v->length))
#define vdl_assert_ZeroObjects(num_object) vdl_Expect(num_object > 0, printf("Zero or negative objects [%d] provided!", num_object))

#define vdl_assert_UninitializedGC() vdl_Expect(VDL_GARENA != NULL && VDL_GREACHABLE != NULL && VDL_GDREACHABLE != NULL, printf("Garbage collector is uninitialized!"))
#define vdl_assert_InconsistentGC() vdl_Expect(((VDL_GARENA == NULL) + (VDL_GREACHABLE == NULL) + (VDL_GDREACHABLE == NULL)) % 3 == 0, printf("Garbage collector in an inconsistent state"))
#define vdl_assert_Unimplemented() vdl_Expect(0, printf("This function is unimplemented!"))

/*-----------------------------------------------------------------------------
 |  Vector health check
 ----------------------------------------------------------------------------*/

#define vdl_HealthCheck(...) vdl_bt_Call(vdl_HealthCheck_BT, __VA_ARGS__)
static inline void vdl_HealthCheck_BT(vdl_bt bt, const vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(v);
    vdl_assert_UnknownType(v->type);
    vdl_Return();
}

#endif//VDL_VDLASSERT_H
