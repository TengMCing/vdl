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

/// @description Except an expression to be true.
/// @param expr (int). An boolean expression.
/// @param code (VDL_ERR). The error code.
/// @param errmsg specifier.
/// @param ... Arguments passed to printf.
#define vdlint_assert_Expect(expr, code, errmsg, ...)                         \
    do {                                                                      \
        if (!(expr))                                                          \
        {                                                                     \
            VDLINT_GERR.CODE = code;                                          \
            VDLINT_GERR.MSG  = errmsg;                                        \
            if (VDL_ERR_MSG_ON)                                               \
            {                                                                 \
                vdlint_PrintBacktrace();                                      \
                printf("[E%03d] Error raised by <%s> at %s:%d: " errmsg "\n", \
                       code,                                                  \
                       __func__,                                              \
                       __FILE__,                                              \
                       __LINE__,                                              \
                       ##__VA_ARGS__);                                        \
            }                                                                 \
            goto VDL_EXCEPTION;                                               \
        }                                                                     \
    } while (0)


/*-----------------------------------------------------------------------------
 |  Assert
 ----------------------------------------------------------------------------*/

/// @description Assert null pointer.
/// @param p (void *). A pointer.
#define vdlint_CheckNullPointer(p) vdlint_assert_Expect((p) != NULL, VDL_ERR_NULL_POINTER, "NULL pointer provided!")

/// @description Assert index out of bound.
/// @param v (vdl_vp). A vector.
/// @param i (int). An integer.
#define vdlint_CheckIndexOutOfBound(v, i) vdlint_assert_Expect((i) >= 0 && (i) < (v)->len, VDL_ERR_INDEX_OUT_OF_BOUND, "Index out of bound! Index [%d] not in [0, %d]!", i, (v)->len)

/// @description Assert unknown type.
/// @param type (VDL_TYPE). A type.
#define vdlint_CheckUnknownType(type) vdlint_assert_Expect((type) >= VDL_TYPE_CHAR && (type) <= VDL_TYPE_VP, VDL_ERR_UNKNOWN_TYPE, "Unknown type [%d] provided!", type)

/// @description Assert incompatible mode.
/// @param mode1 (VDL_MODE). A mode.
/// @param mode2 (VDL_MODE). Another mode.
#define vdlint_CheckIncompatibleMode(mode1, mode2) vdlint_assert_Expect((mode1) == (mode2), VDL_ERR_INCOMPATIBLE_MODE, "Incompatible storage modes (%d != %d) provided!", mode1, mode2)

/// @description Assert incompatible type.
/// @param type1 (VDL_TYPE). A type.
/// @param type2 (VDL_TYPE). A type.
#define vdlint_CheckIncompatibleType(type1, type2) vdlint_assert_Expect((type1) == (type2), VDL_ERR_INCOMPATIBLE_TYPE, "Incompatible types (%d != %d) provided!", type1, type2)

/// @description Assert when try to create vector with 0 cap.
/// @param cap (int). Capacity.
#define vdlint_CheckAllocZeroCap(cap) vdlint_assert_Expect((cap) > 0, VDL_ERR_ALLOCATE_FAILED, "Fail to allocate memory for zero or negative cap [%d]!", cap)

/// @description Assert when fail to allocate memory.
/// @param p (void *). A pointer returned by malloc, calloc or realloc.
#define vdlint_CheckAllocFailed(p) vdlint_assert_Expect((p) != NULL, VDL_ERR_ALLOCATE_FAILED, "Fail to allocate memory!")

#define vdlint_CheckUninitializedGC() vdlint_assert_Expect(VDLINT_GARENA != NULL && VDLINT_GREACHABLE != NULL && VDLINT_GDREACHABLE != NULL, VDL_ERR_GC_FAIL, "Garbage collector is uninitialized!")
#define vdlint_CheckInconsistentGC() vdlint_assert_Expect(((VDLINT_GARENA == NULL) + (VDLINT_GREACHABLE == NULL) + (VDLINT_GDREACHABLE == NULL)) % 3 == 0, VDL_ERR_GC_FAIL, "Garbage collector in an inconsistent state")
#define vdlint_CheckUnimplemented() vdlint_assert_Expect(0, VDL_ERR_UNIMPLEMENTED, "This function is unimplemented!")

/*-----------------------------------------------------------------------------
 |  Vector health check
 ----------------------------------------------------------------------------*/

/// @description Health check ensures the vector is not a null pointer (safe to read the metadata of the vector),
/// the type is known (safe to do type dispatch) and the data pointer is not a null pointer (safe to use the data).
#define vdlint_CheckVecHealth(v, nullv, utype, nulldat) \
    do {                                                \
        if (nullv)                                      \
            vdlint_CheckNullPointer(v);                 \
        if (utype)                                      \
            vdlint_CheckUnknownType((v)->type);         \
        if (nulldat)                                    \
            vdlint_CheckNullPointer((v)->dat);          \
    } while (0)

#endif//VDL_VDLASSERT_H
