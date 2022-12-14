//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLMEM_H
#define VDL_VDLMEM_H

#include "vdlgc.h"
#include "vdlwrapper.h"
#include <stdarg.h>

/*-----------------------------------------------------------------------------
 |  Allocate vector on stack
 ----------------------------------------------------------------------------*/

// TEMPLATE BEGIN
#define vdlint_T_LocV(T, UT, ...)                 \
    &(vdl_vec)                                    \
    {                                             \
        .type = UT,                               \
        .mode = VDL_MODE_STACK,                   \
        .cap  = vdlint_CountArg(__VA_ARGS__),     \
        .len  = vdlint_CountArg(__VA_ARGS__),     \
        .dat  = (T[vdlint_CountArg(__VA_ARGS__)]) \
        {                                         \
            __VA_ARGS__                           \
        }                                         \
    }

#define vdlint_char_LocV(...) vdlint_T_LocV(char, VDL_TYPE_CHAR, __VA_ARGS__)
#define vdlint_int_LocV(...) vdlint_T_LocV(int, VDL_TYPE_INT, __VA_ARGS__)
#define vdlint_double_LocV(...) vdlint_T_LocV(double, VDL_TYPE_DOUBLE, __VA_ARGS__)

/// @description Allocate a local vector on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (char/int/double). A series of objects or literals of the same type.
#define vdl_LocV(...) _Generic(vdlint_First1Arg(__VA_ARGS__), char    \
                               : vdlint_char_LocV(__VA_ARGS__), int   \
                               : vdlint_int_LocV(__VA_ARGS__), double \
                               : vdlint_double_LocV(__VA_ARGS__))


/// @description Allocate a local vector consists of vectors of any type on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (vdl_vp). A series of objects or literals of the same type.
#define vdl_vp_LocV(...) vdlint_T_LocV(vdl_vp, VDL_TYPE_VP, __VA_ARGS__)
#undef vdlint_T_LocV
// TEMPLATE END

/*-----------------------------------------------------------------------------
 |  Allocate an empty vector on heap
 ----------------------------------------------------------------------------*/

/// @description Allocate an empty vector on heap and record it by the garbage collector.
/// @details Allocate a vector on heap is more expensive than allocate it on stack
/// as it involves much more instructions, and has the need to manually deallocate memory.
/// However, many machines have a maximum stack size, which is typically 1 to 8 MB.
/// Thus, it is necessary to dynamically allocate memory for large objects to avoid stack overflow. \n\n
/// A vector allocated on heap will be deallocated by the garbage collector when
/// it is not reachable by the program. Users should not free the memory manually as it
/// will potentially cause double-free and use-after-free problems.
/// @param type (VDL_TYPE). Type of the vector.
/// @param cap (int). Requested capacity.
/// @return (vdl_vp) An empty vector.
#define vdl_e_New(...) vdlint_e_Call(vdl_e_New_BT, vdl_vp, __VA_ARGS__)
static inline vdl_vp vdl_e_New_BT(vdl_fr bt, const VDL_TYPE type, const int cap)
{
    // Init variables needed for the exception handler
    void *v_buffer   = NULL;
    void *dat_buffer = NULL;

    vdl_e_PushFrame(bt);
    vdlint_e_CheckUnknownType(type);
    vdlint_e_CheckAllocZeroCap(cap);

    // Allocate memory for the container
    v_buffer = malloc(sizeof(vdl_vec));
    vdlint_e_CheckAllocFailed(v_buffer);
    vdl_vp v = v_buffer;

    // Allocate memory for the array
    dat_buffer = calloc((size_t) cap, vdl_SizeOfType(type));
    vdlint_e_CheckAllocFailed(dat_buffer);

    // Since some members have const qualifiers, this is one way to assign the initialized value
    vdl_vp v_init = &(vdl_vec){.type = type,
                               .mode = VDL_MODE_HEAP,
                               .cap  = cap,
                               .len  = 0,
                               .dat  = dat_buffer};
    memcpy(v, v_init, sizeof(vdl_vec));

    // Record the vector by the global vdl_gc_arena
    vdlint_ne_GCRecord(v);

    return v;

VDL_EXCEPTION:

    // Free the first buffer
    if (v_buffer != NULL)
        free(v_buffer);

    return NULL;
}

/*-----------------------------------------------------------------------------
 |  Allocate and initialize vector on heap
 ----------------------------------------------------------------------------*/

/// @description Allocate and initialize a vector on heap and record it by the garbage collector.
/// @details Allocate a vector on heap is more expensive than allocate it on stack
/// as it involves much more instructions, and has the need to manually deallocate memory.
/// However, many machines have a maximum stack size, which is typically 1 to 8 MB.
/// Thus, it is necessary to dynamically allocate memory for large objects to avoid stack overflow. \n\n
/// A vector allocated on heap will be deallocated by the garbage collector when
/// it is not reachable by the program. Users should not free the memory manually as it
/// will potentially cause double-free and use-after-free problems.
/// @param type (VDL_TYPE). Type of the vector.
/// @param len (int). Requested capacity.
/// @param ... (char/int/double/vdl_vp). A series of objects of the correct type.
/// @return (vdl_vp) A vector.
#define vdl_e_VVariadic(...) vdlint_e_Call(vdl_e_VVariadic_BT, vdl_vp, __VA_ARGS__)
static inline vdl_vp vdl_e_VVariadic_BT(vdl_fr bt, const VDL_TYPE type, const int len, ...)
{
    // Init flag needed for exception handler
    int ap_flag = 0;

    vdl_e_PushFrame(bt);

    vdl_vp v = vdl_e_New(type, len);
    v->len   = len;

    va_list ap;
    va_start(ap, len);
    ap_flag = 1;

    switch (type)
    {
        case VDL_TYPE_CHAR:
            vdlint_for_i(len) vdlint_CharAt(v, i) = (char) va_arg(ap, int);
            break;
        case VDL_TYPE_INT:
            vdlint_for_i(len) vdlint_IntAt(v, i) = va_arg(ap, int);
            break;
        case VDL_TYPE_DOUBLE:
            vdlint_for_i(len) vdlint_DoubleAt(v, i) = va_arg(ap, double);
            break;
        case VDL_TYPE_VP:
            vdlint_for_i(len) vdlint_VpAt(v, i) = va_arg(ap, vdl_vp);
            break;
    }

    va_end(ap);
    return v;

VDL_EXCEPTION:
    // Clean up ap if it is started
    if (ap_flag == 1)
        va_end(ap);
    return NULL;
}

/// @description Allocate and initialize a vector on heap and record it by the garbage collector.
/// @details Allocate a vector on heap is more expensive than allocate it on stack
/// as it involves much more instructions, and has the need to manually deallocate memory.
/// However, many machines have a maximum stack size, which is typically 1 to 8 MB.
/// Thus, it is necessary to dynamically allocate memory for large objects to avoid stack overflow. \n\n
/// A vector allocated on heap will be deallocated by the garbage collector when
/// it is not reachable by the program. Users should not free the memory manually as it
/// will potentially cause double-free and use-after-free problems. \n\n
/// The first argument will be used to decide the type of the vector.
/// @param ... (char/int/double/vdl_vp). A series of objects of the same type.
/// @return (vdl_vp) A vector.
#define vdl_e_V(...) _Generic(vdlint_First1Arg(__VA_ARGS__), char                                                        \
                              : vdl_e_VVariadic(VDL_TYPE_CHAR, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), int           \
                              : vdl_e_VVariadic(VDL_TYPE_INT, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), double         \
                              : vdl_e_VVariadic(VDL_TYPE_DOUBLE, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), vdl_vec *   \
                              : vdl_e_VVariadic(VDL_TYPE_VP, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), const vdl_vec * \
                              : vdl_e_VVariadic(VDL_TYPE_VP, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), void *          \
                              : vdl_e_VVariadic(VDL_TYPE_VP, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__))


/*-----------------------------------------------------------------------------
 |  Reserve space for heap allocated vector
 ----------------------------------------------------------------------------*/

/// @description Reserve more space for a heap allocated vector.
/// @details Only heap allocated vector can be reallocated.
/// @param v (vdl_vec*). A vector.
/// @param cap (int). Requested capacity.
/// @return (vdl_vp) A new vector.
#define vdl_e_Reserve(...) vdlint_e_CallVoid(vdl_e_Reserve_BT, __VA_ARGS__)
static inline void vdl_e_Reserve_BT(vdl_fr bt, vdl_vec *const v, const int cap)
{
    // Init variables needed for the exception handler
    void *dat_buffer = (void *) 0XFF;
    int old_cap      = 0;

    vdl_e_PushFrame(bt);
    vdlint_e_CheckVec(v, VDLINT_CHECK_NULL_VEC | VDLINT_CHECK_NULL_VEC_DAT | VDLINT_CHECK_UNKNOWN_VEC_TYPE);
    vdlint_e_CheckIncompatibleMode(v->mode, VDL_MODE_HEAP);

    old_cap = v->cap;

    // Do nothing when there is enough space
    if (v->cap >= cap)
        return;

    // Allocate enough space
    // Switch to arithmetic growth policy after 500KB
    while (v->cap < cap)
    {
        static const size_t MEM_500KB = 500 * 1024;
        if (vdl_SizeOfDat(v) < MEM_500KB)
            v->cap = v->cap * 2 + 8;
        else
            v->cap += (int) (MEM_500KB / vdl_SizeOfType(v->type));
    }

    dat_buffer = realloc(v->dat, vdl_SizeOfDat(v));
    vdlint_e_CheckAllocFailed(dat_buffer);
    v->dat = dat_buffer;

    return;

VDL_EXCEPTION:
    // If fail to allocate, recover the capacity
    if (dat_buffer == NULL)
        v->cap = old_cap;
}

#endif//VDL_VDLMEM_H
