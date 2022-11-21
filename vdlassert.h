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
#define vdlint_expect(expr, code, errmsg, ...)                                \
    do {                                                                      \
        if (!(expr))                                                          \
        {                                                                     \
            VDLINT_GERR.CODE = code;                                          \
            VDLINT_GERR.MSG  = errmsg;                                        \
            if (VDL_ERR_MSG_ON)                                               \
            {                                                                 \
                vdl_bt_print();                                               \
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
#define vdl_assert_null_pointer(p) vdlint_expect((p) != NULL, VDL_ERR_NULL_POINTER, "NULL pointer provided!")

/// @description Assert index out of bound.
/// @param v (vdl_vp). A vector.
/// @param i (int). An integer.
#define vdl_assert_idx_out_of_bound(v, i) vdlint_expect((i) >= 0 && (i) < (v)->len, VDL_ERR_INDEX_OUT_OF_BOUND, "Index out of bound! Index [%d] not in [0, %d]!", i, (v)->len)

/// @description Assert unknown type.
/// @param type (VDL_TYPE). A type.
#define vdl_assert_unknown_type(type) vdlint_expect((type) >= VDL_TYPE_CHAR && (type) <= VDL_TYPE_VP, VDL_ERR_UNKNOWN_TYPE, "Unknown type [%d] provided!", type)

/// @description Assert heap mode.
/// @param mode (VDL_MODE). A mode.
#define vdl_assert_heap_mode(mode) vdlint_expect((mode) == VDL_MODE_STACK, VDL_ERR_INCOMPATIBLE_MODE, "Storage mode [%d] is not stack allocation!", mode)

/// @description Assert stack mode.
/// @param mode (VDL_MODE). A mode.
#define vdl_assert_stack_mode(mode) vdlint_expect((mode) == VDL_MODE_HEAP, VDL_ERR_INCOMPATIBLE_MODE, "Storage mode [%d] is not heap allocation!", mode)

/// @description Assert incompatible type.
/// @param type1 (VDL_TYPE). A type.
/// @param type2 (VDL_TYPE). A type.
#define vdl_assert_incompatible_type(type1, type2) vdlint_expect((type1) == (type2), VDL_ERR_INCOMPATIBLE_TYPE, "Incompatible types (%d != %d) provided!", type1, type2)

/// @description Assert when try to create vector with 0 capacity.
/// @param cap (int). Capacity.
#define vdl_assert_zero_cap(cap) vdlint_expect((cap) > 0, VDL_ERR_ALLOCATE_FAIL, "Fail to allocate memory for zero or negative capacity [%d]!", cap)

/// @description Assert when fail to allocate memory.
/// @param p (void *). A pointer returned by malloc, calloc or realloc.
#define vdl_assert_alloc_failed(p) vdlint_expect((p) != NULL, VDL_ERR_ALLOCATE_FAIL, "Fail to allocate memory!")

#define vdl_assert_UninitializedGC() vdlint_expect(VDL_INTERNAL_GARENA != NULL && VDL_INTERNAL_GREACHABLE != NULL && VDL_INTERNAL_GDREACHABLE != NULL, VDL_ERROR_UNINITIALIZED_GC, "Garbage collector is uninitialized!")
#define vdl_assert_InconsistentGC() vdlint_expect(((VDL_INTERNAL_GARENA == NULL) + (VDL_INTERNAL_GREACHABLE == NULL) + (VDL_INTERNAL_GDREACHABLE == NULL)) % 3 == 0, VDL_ERROR_INCONSISTENT_GC, "Garbage collector in an inconsistent state")
#define vdl_assert_Unimplemented() vdlint_expect(0, VDL_ERROR_UNIMPLEMENTED, "This function is unimplemented!")

/*-----------------------------------------------------------------------------
 |  Vector health check
 ----------------------------------------------------------------------------*/

/// @description Health check ensures the vector is not a null pointer (safe to read the metadata of the vector),
/// the type is known (safe to do type dispatch) and the data pointer is not a null pointer (safe to use the data).
#define vdl_assert_vec_health(v, nullv, utype, nulldat) \
    do {                                                \
        if (nullv)                                      \
            vdl_assert_null_pointer(v);                 \
        if (utype)                                      \
            vdl_assert_unknown_type((v)->type);         \
        if (nulldat)                                    \
            vdl_assert_null_pointer((v)->dat);          \
    } while (0)

#endif//VDL_VDLASSERT_H
