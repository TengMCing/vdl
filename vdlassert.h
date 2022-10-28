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

#define vdl_Expect(expr, error_code, errmsg)                 \
    do {                                                     \
        if (!(expr))                                         \
        {                                                    \
            VDL_GERROR = error_code;                         \
            vdl_bt_Print();                                  \
            printf("[%d] Error raised by <%s> at %s:%d: ",   \
                   error_code,                               \
                   VDL_GBT.func_name[VDL_GBT.num_frame - 1], \
                   __FILE__,                                 \
                   __LINE__);                                \
            errmsg;                                          \
            printf("\n");                                    \
            goto VDL_EXCEPTION;                              \
        }                                                    \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Assert
 ----------------------------------------------------------------------------*/

#define vdl_assert_NullPointer(object) vdl_Expect((object) != NULL, 1, printf("NULL pointer provided!"))
#define vdl_assert_IndexOutOfBound(v, i) vdl_Expect((i) >= 0 && (i) < (v)->length, 2, printf("Index out of bound! Index [%d] not in [0, %d]!", i, (v)->length))
#define vdl_assert_UnknownType(type) vdl_Expect((type) >= VDL_TYPE_CHAR && (type) <= VDL_TYPE_VP, 3, printf("Unknown type [%d] provided!", type))
#define vdl_assert_NotStackAllocated(mode) vdl_Expect((mode) == VDL_MODE_STACK, 4, printf("Storage mode [%d] is not stack allocation!", mode))
#define vdl_assert_NotHeapAllocated(mode) vdl_Expect((mode) == VDL_MODE_HEAP, 5, printf("Storage mode [%d] is not heap allocation!", mode))
#define vdl_assert_IncompatibleType(type1, type2) vdl_Expect((type1) == (type2), 6, printf("Incompatible types (%d != %d) provided!", type1, type2))
#define vdl_assert_ZeroCapacity(capacity) vdl_Expect((capacity) > 0, 7, printf("Zero or negative capacity [%d] provided!", capacity))
#define vdl_assert_ZeroLengthVector(v) vdl_Expect((v)->length > 0, 8, printf("Vector of zero length [%d] provided!", (v)->length))
#define vdl_assert_ZeroObjects(num_object) vdl_Expect((num_object) > 0, 9, printf("Zero or negative objects [%d] provided!", num_object))

#define vdl_assert_UninitializedGC() vdl_Expect(VDL_GARENA != NULL && VDL_GREACHABLE != NULL && VDL_GDREACHABLE != NULL, 10, printf("Garbage collector is uninitialized!"))
#define vdl_assert_InconsistentGC() vdl_Expect(((VDL_GARENA == NULL) + (VDL_GREACHABLE == NULL) + (VDL_GDREACHABLE == NULL)) % 3 == 0, 11, printf("Garbage collector in an inconsistent state"))
#define vdl_assert_Unimplemented() vdl_Expect(0, 12, printf("This function is unimplemented!"))

/*-----------------------------------------------------------------------------
 |  Vector health check
 ----------------------------------------------------------------------------*/

#define vdl_HealthCheck(...) vdl_CallerNoReturn(vdl_HealthCheck_BT, __VA_ARGS__)
static inline void vdl_HealthCheck_BT(vdl_bt bt, const vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(v);
    vdl_assert_UnknownType(v->type);
VDL_EXCEPTION:
    return;
}

#endif//VDL_VDLASSERT_H
