//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLASSERT_H
#define VDL_VDLASSERT_H

#include "vdlbt.h"
#include "vdldef.h"
#include <stdarg.h>

/*-----------------------------------------------------------------------------
 |  Expect
 ----------------------------------------------------------------------------*/

static inline void vdl_internal_PrintError(const VDL_ERROR_CODE error_code, const char *spec, ...)
{
    VDL_INTERNAL_GERROR = error_code;
    vdl_bt_Print();
    va_list ap;
    va_start(ap, spec);
    vprintf(spec, ap);
    va_end(ap);
}

#define vdl_PrintError()

/// @description Except an expression to be true.
/// @param expr (int). An boolean expression.
/// @param error_code (VDL_ERROR_CODE). The error code.
/// @param errmsg Some statements, usually a printf call.
#define vdl_internal_Expect(expr, error_code, errmsg, ...)                              \
    do {                                                                                \
        if (!(expr))                                                                    \
        {                                                                               \
            vdl_internal_PrintError(error_code,                                         \
                                    "[%d] Error raised by <%s> at %s:%d: " errmsg "\n", \
                                    error_code,                                         \
                                    __func__,                                           \
                                    __FILE__,                                           \
                                    __LINE__,                                           \
                                    ##__VA_ARGS__);                                     \
            VDL_THROW;                                                                  \
        }                                                                               \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Assert
 ----------------------------------------------------------------------------*/

#define vdl_assert_NullPointer(object) vdl_internal_Expect((object) != NULL, VDL_ERROR_NULL_POINTER, "NULL pointer provided!")
#define vdl_assert_IndexOutOfBound(v, i) vdl_internal_Expect((i) >= 0 && (i) < (v)->length, VDL_ERROR_INDEX_OUT_OF_BOUND, "Index out of bound! Index [%d] not in [0, %d]!", i, (v)->length)
#define vdl_assert_UnknownType(type) vdl_internal_Expect((type) >= VDL_TYPE_CHAR && (type) <= VDL_TYPE_VP, VDL_ERROR_UNKNOWN_TYPE, "Unknown type [%d] provided!", type)
#define vdl_assert_NotStackAllocated(mode) vdl_internal_Expect((mode) == VDL_MODE_STACK, VDL_ERROR_NOT_STACK_ALLOCATED, "Storage mode [%d] is not stack allocation!", mode)
#define vdl_assert_NotHeapAllocated(mode) vdl_internal_Expect((mode) == VDL_MODE_HEAP, VDL_ERROR_NOT_HEAP_ALLOCATED, "Storage mode [%d] is not heap allocation!", mode)
#define vdl_assert_IncompatibleType(type1, type2) vdl_internal_Expect((type1) == (type2), VDL_ERROR_INCOMPATIBLE_TYPE, "Incompatible types (%d != %d) provided!", type1, type2)
#define vdl_assert_ZeroCapacity(capacity) vdl_internal_Expect((capacity) > 0, VDL_ERROR_ZERO_CAPACITY, "Zero or negative capacity [%d] provided!", capacity)
#define vdl_assert_ZeroLengthVector(v) vdl_internal_Expect((v)->length > 0, VDL_ERROR_ZERO_LENGTH_VECTOR, "Vector of zero length [%d] provided!", (v)->length)
#define vdl_assert_ZeroObjects(num_object) vdl_internal_Expect((num_object) > 0, VDL_ERROR_ZERO_OBJECTS, "Zero or negative objects [%d] provided!", num_object)

#define vdl_assert_UninitializedGC() vdl_internal_Expect(VDL_GARENA != NULL && VDL_GREACHABLE != NULL && VDL_GDREACHABLE != NULL, 11, "Garbage collector is uninitialized!")
#define vdl_assert_InconsistentGC() vdl_internal_Expect(((VDL_GARENA == NULL) + (VDL_GREACHABLE == NULL) + (VDL_GDREACHABLE == NULL)) % 3 == 0, 12, "Garbage collector in an inconsistent state")
#define vdl_assert_Unimplemented() vdl_internal_Expect(0, 13, "This function is unimplemented!")

/*-----------------------------------------------------------------------------
 |  Vector health check
 ----------------------------------------------------------------------------*/

#define vdl_HealthCheck(v)                 \
    do {                                   \
        vdl_assert_NullPointer(v);         \
        vdl_assert_UnknownType((v)->type); \
    } while (0)

#endif//VDL_VDLASSERT_H
