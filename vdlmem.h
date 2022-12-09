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

#define vdlint_T_LocV(T, ...)                     \
    &(vdl_vec)                                    \
    {                                             \
        .type = vdl_lower_type_##T,               \
        .mode = VDL_MODE_STACK,                   \
        .cap  = vdlint_CountArg(__VA_ARGS__),     \
        .len  = vdlint_CountArg(__VA_ARGS__),     \
        .dat  = (T[vdlint_CountArg(__VA_ARGS__)]) \
        {                                         \
            __VA_ARGS__                           \
        }                                         \
    }

#define vdlint_char_LocV(...) vdlint_T_LocV(char, __VA_ARGS__)
#define vdlint_int_LocV(...) vdlint_T_LocV(int, __VA_ARGS__)
#define vdlint_double_LocV(...) vdlint_T_LocV(double, __VA_ARGS__)

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


/// @description Allocate a local vector (list) consists of vectors of any type on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (vdl_vp). A series of objects or literals of the same type.
#define vdl_LocL(...) vdlint_T_LocV(vdl_vp, __VA_ARGS__)

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
#define vdl_New(...) vdlint_Call(vdl_New_BT, vdl_vp, __VA_ARGS__)
static inline vdl_vp vdl_New_BT(vdl_bt bt, const VDL_TYPE type, const int cap)
{
    // Init variables needed for the exception handler
    void *v_buffer   = NULL;
    void *dat_buffer = NULL;

    vdl_PushBacktrace(bt);
    vdlint_CheckUnknownType(type);
    vdlint_CheckAllocZeroCap(cap);

    // Allocate memory for the container
    v_buffer = malloc(sizeof(vdl_vec));
    vdlint_CheckAllocFailed(v_buffer);
    vdl_vp v = v_buffer;

    // Allocate memory for the array
    dat_buffer = calloc((size_t) cap, vdl_SizeOfType(type));
    vdlint_CheckAllocFailed(dat_buffer);

    // Since some members have const qualifiers, this is one way to assign the initialized value
    vdl_vp v_init = &(vdl_vec){.type = type,
                               .mode = VDL_MODE_HEAP,
                               .cap  = cap,
                               .len  = 0,
                               .dat  = dat_buffer};
    memcpy(v, v_init, sizeof(vdl_vec));

    // Record the vector by the global vdl_gc_arena
    vdlint_GCRecord(v);

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
#define vdl_V_Variadic(...) vdlint_Call(vdl_V_Variadic_BT, vdl_vp, __VA_ARGS__)
static inline vdl_vp vdl_V_Variadic_BT(vdl_bt bt, const VDL_TYPE type, const int len, ...)
{
    // Init flag needed for exception handler
    int ap_flag = 0;

    vdl_PushBacktrace(bt);

    vdl_vp v      = vdl_New(type, len);
    vdlint_Len(v) = len;

    va_list ap;
    va_start(ap, len);
    ap_flag = 1;

    switch (type)
    {
        case VDL_TYPE_CHAR:
            vdlint_for_i(len) vdl_SetByArray(v, i, &(char){(char) va_arg(ap, int)}, 1);
            break;
        case VDL_TYPE_INT:
            vdlint_for_i(len) vdl_SetByArray(v, i, &(int){va_arg(ap, int)}, 1);
            break;
        case VDL_TYPE_DOUBLE:
            vdlint_for_i(len) vdl_SetByArray(v, i, &(double){va_arg(ap, double)}, 1);
            break;
        case VDL_TYPE_VP:
            vdlint_for_i(len) vdl_SetByArray(v, i, &(vdl_vp){va_arg(ap, vdl_vp)}, 1);
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
#define vdl_V(...) _Generic(vdlint_First1Arg(__VA_ARGS__), char                                                       \
                            : vdl_V_Variadic(VDL_TYPE_CHAR, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), int           \
                            : vdl_V_Variadic(VDL_TYPE_INT, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), double         \
                            : vdl_V_Variadic(VDL_TYPE_DOUBLE, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), vdl_vec *   \
                            : vdl_V_Variadic(VDL_TYPE_VP, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__), const vdl_vec * \
                            : vdl_V_Variadic(VDL_TYPE_VP, vdlint_CountArg(__VA_ARGS__), __VA_ARGS__))


/*-----------------------------------------------------------------------------
 |  Reserve space for heap allocated vector
 ----------------------------------------------------------------------------*/

/// @description Reserve more space for a heap allocated vector.
/// @details Only heap allocated vector can be reallocated.
/// @param v (vdl_vec*). Type of the vector.
/// @param cap (int). Requested capacity.
/// @return (vdl_vp) A new vector.
#define vdl_Reserve(...) vdlint_CallVoid(vdl_Reserve_BT, __VA_ARGS__)
static inline void vdl_Reserve_BT(vdl_bt bt, vdl_vec *const v, const int cap)
{
    // Init variables needed for the exception handler
    void *dat_buffer = (void *) 0XFF;
    int old_cap      = 0;

    vdl_PushBacktrace(bt);
    vdlint_CheckVecHealth(v, 1, 1, 1);
    vdlint_CheckIncompatibleMode(vdl_GetMode(v), VDL_MODE_HEAP);
    vdlint_CheckAllocZeroCap(cap);

    old_cap = vdlint_Cap(v);

    // Do nothing when there is enough space
    if (vdlint_Cap(v) >= cap)
        return;

    // Allocate enough space
    // Switch to arithmetic growth policy after 500KB
    while (vdlint_Cap(v) < cap)
    {
        static const size_t MEM_500KB = 500 * 1024;
        if (vdl_SizeOfDat(v) < MEM_500KB)
            vdlint_Cap(v) = vdlint_Cap(v) * 2 + 8;
        else
            vdlint_Cap(v) += (int) (MEM_500KB / vdl_SizeOfType(vdl_GetType(v)));
    }

    dat_buffer = realloc(vdlint_Dat(v), vdl_SizeOfDat(v));
    vdlint_CheckAllocFailed(dat_buffer);
    vdlint_Dat(v) = dat_buffer;

    return;

VDL_EXCEPTION:
    // If fail to allocate, recover the capacity
    if (dat_buffer == NULL)
        vdlint_Cap(v) = old_cap;
}

#endif//VDL_VDLMEM_H
