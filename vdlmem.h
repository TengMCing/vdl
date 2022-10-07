//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLMEM_H
#define VDL_VDLMEM_H

#include "vdlassert.h"
#include "vdldef.h"
#include "vdlgc.h"
#include "vdlwrapper.h"
#include <stdarg.h>

/*-----------------------------------------------------------------------------
 |  Allocate vector on stack
 ----------------------------------------------------------------------------*/

#define vdl_T_ArgV(T, ...)                          \
    &(vdl_vec)                                      \
    {                                               \
        .type     = vdl_lower_type_##T,             \
        .mode     = VDL_MODE_STACK,                 \
        .capacity = vdl_CountArgs(__VA_ARGS__),     \
        .length   = vdl_CountArgs(__VA_ARGS__),     \
        .data     = (T[vdl_CountArgs(__VA_ARGS__)]) \
        {                                           \
            __VA_ARGS__                             \
        }                                           \
    }

#define vdl_char_ArgV(...) vdl_T_ArgV(char, __VA_ARGS__)
#define vdl_int_ArgV(...) vdl_T_ArgV(int, __VA_ARGS__)
#define vdl_double_ArgV(...) vdl_T_ArgV(double, __VA_ARGS__)

/// @description Allocate a vector on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (char/int/double). A series of objects or literals of the same type.
#define vdl_ArgV(...) _Generic(vdl_First1Args(__VA_ARGS__), char   \
                               : vdl_char_ArgV(__VA_ARGS__), int   \
                               : vdl_int_ArgV(__VA_ARGS__), double \
                               : vdl_double_ArgV(__VA_ARGS__))


/// @description Allocate a vector consists of vectors of any type on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (vdl_vp). A series of objects or literals of the same type.
#define vdl_vp_ArgV(...) vdl_T_ArgV(vdl_vp, __VA_ARGS__)

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
/// @param capacity (int). Requested capacity.
/// @return (vdl_vp) An empty vector.
#define vdl_New(...) vdl_bt_Call(vdl_New_BT, __VA_ARGS__)
static inline vdl_vp vdl_New_BT(vdl_bt bt, const VDL_TYPE type, const int capacity)
{
    vdl_PushBT(bt);
    vdl_assert_UnknownType(type);
    vdl_assert_ZeroCapacity(capacity);

    // Allocate memory for the container
    vdl_vp v = malloc(sizeof(vdl_vec));

    // Since some members have const qualifiers, this is one way to assign the initialized value
    vdl_vp v_init = &(vdl_vec){.type     = type,
                               .mode     = VDL_MODE_HEAP,
                               .capacity = capacity,
                               .length   = 0,
                               .data     = NULL};
    memcpy(v, v_init, sizeof(vdl_vec));

    // Allocate memory for the array
    vdl_Data(v) = calloc((size_t) capacity, vdl_SizeOfType(type));

    // Record the vector by the global vdl_gc_arena
    vdl_gc_Record(v);

    vdl_ReturnConst(v);
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
/// @param length (int). Requested capacity.
/// @param ... (char/int/double/vdl_vp). A series of objects of the correct type.
/// @return (vdl_vp) A vector.
#define vdl_V_Variadic(...) vdl_bt_Call(vdl_V_Variadic_BT, __VA_ARGS__)
static inline vdl_vp vdl_V_Variadic_BT(vdl_bt bt, const VDL_TYPE type, const int length, ...)
{
    vdl_PushBT(bt);
    vdl_vp v      = vdl_New(type, length);
    vdl_Length(v) = length;
    va_list ap;
    va_start(ap, length);
    switch (type)
    {
        case VDL_TYPE_CHAR:
            vdl_For_i(length) vdl_Set(v, i, &(char){(char) va_arg(ap, int)}, 1);
            break;
        case VDL_TYPE_INT:
            vdl_For_i(length) vdl_Set(v, i, &(int){va_arg(ap, int)}, 1);
            break;
        case VDL_TYPE_DOUBLE:
            vdl_For_i(length) vdl_Set(v, i, &(double){va_arg(ap, double)}, 1);
            break;
        case VDL_TYPE_VP:
            vdl_For_i(length) vdl_Set(v, i, &(vdl_vp){va_arg(ap, vdl_vp)}, 1);
            break;
    }
    va_end(ap);
    vdl_ReturnConst(v);
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
#define vdl_V(...) _Generic(vdl_First1Args(__VA_ARGS__), char                                                       \
                            : vdl_V_Variadic(VDL_TYPE_CHAR, vdl_CountArgs(__VA_ARGS__), __VA_ARGS__), int           \
                            : vdl_V_Variadic(VDL_TYPE_INT, vdl_CountArgs(__VA_ARGS__), __VA_ARGS__), double         \
                            : vdl_V_Variadic(VDL_TYPE_DOUBLE, vdl_CountArgs(__VA_ARGS__), __VA_ARGS__), vdl_vec *   \
                            : vdl_V_Variadic(VDL_TYPE_VP, vdl_CountArgs(__VA_ARGS__), __VA_ARGS__), const vdl_vec * \
                            : vdl_V_Variadic(VDL_TYPE_VP, vdl_CountArgs(__VA_ARGS__), __VA_ARGS__))


/*-----------------------------------------------------------------------------
 |  Reserve space for heap allocated vector
 ----------------------------------------------------------------------------*/

/// @description Reserve more space for a heap allocated vector.
/// @details Only heap allocated vector can be
/// @param v (vdl_vec*). Type of the vector.
/// @param capacity (int). Requested capacity.
/// @return (vdl_vp) A new vector.
#define vdl_Reserve(...) vdl_bt_Call(vdl_Reserve_BT, __VA_ARGS__)
static inline void vdl_Reserve_BT(vdl_bt bt, vdl_vec *const v, const int capacity)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_assert_NotHeapAllocated(vdl_Mode(v));
    vdl_assert_ZeroCapacity(capacity);

    // Do nothing when there is enough space
    if (vdl_Capacity(v) >= capacity)
        vdl_ReturnConst();

    // Allocate enough space
    // Switch to arithmetic growth policy after 500KB
    while (vdl_Capacity(v) < capacity)
    {
        static const size_t MEM_500KB = 500 * 1024;
        if (vdl_SizeOfData(v) < MEM_500KB)
            vdl_Capacity(v) = vdl_Capacity(v) * 2 + 8;
        else
            vdl_Capacity(v) += (int) (MEM_500KB / vdl_SizeOfType(vdl_Type(v)));
    }

    vdl_Data(v) = realloc(vdl_Data(v), vdl_SizeOfData(v));
    vdl_ReturnConst();
}

#endif//VDL_VDLMEM_H
