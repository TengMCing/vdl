//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLMEM_H
#define VDL_VDLMEM_H

#include "vdlassert.h"
#include "vdldef.h"
#include "vdlgc.h"
#include "vdlportal.h"
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
static inline vdl_vp vdl_New(const VDL_TYPE type, const int capacity)
{
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

    return v;
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
static inline vdl_vp vdl_V_Variadic(const VDL_TYPE type, const int length, ...)
{
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
    return v;
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
static inline void vdl_Reserve(vdl_vec *const v, const int capacity)
{
    vdl_HealthCheck(v);
    vdl_assert_NotHeapAllocated(vdl_Mode(v));
    vdl_assert_ZeroCapacity(capacity);

    // Do nothing when there is enough space
    if (vdl_Capacity(v) >= capacity)
        return;

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
}


//
// static inline void vec_Append(vdl_vec *const v, void *const object, const int num_object)
// {
//     vec_HealthCheck(v);
//     vdl_assert_NullPointer(object);
//     vdl_assert_ZeroObjects(num_object);
//
//     vec_Reserve(v, v->size + num_object);
//     v->size += num_object;
//     vec_Set(v, v->size - num_object, object, num_object);
// }
//
// static inline void vec_Remove(vdl_vec *const v, const int i, const int num_object)
// {
//     vec_HealthCheck(v);
//     vdl_assert_IndexOutOfBound(v, i);
//     vdl_assert_IndexOutOfBound(v, i + num_object - 1);
//     vdl_assert_ZeroObjects(num_object);
//
//     // Only reduce the length if all the items are at the end of the array
//     if (i == v->size - num_object)
//     {
//         v->size -= num_object;
//         return;
//     }
//
//     // Copy memory to index i
//     memmove(vec_AddressOf(v, i), vec_AddressOf(v, i + num_object), (size_t) (v->size - i - num_object) * vec_GetTypeSize(v->type));
//     v->size -= num_object;
// }
//
// #define vec_Pop(v, i) vec_Remove(v, i, 1)
//
// static inline void vec_Concatenate(vdl_vec *const v1, vdl_vec *const v2)
// {
//     vec_HealthCheck(v1);
//     vec_HealthCheck(v2);
//     vdl_assert_IncompatibleType(v1->type, v2->type);
//
//     vec_Append(v1, v2->data, v2->size);
// }
//
// static inline void vec_Insert(vdl_vec *const v, const int i, void *const object, const int num_object)
// {
//     vec_HealthCheck(v);
//     vdl_assert_IndexOutOfBound(v, i);
//     vdl_assert_NullPointer(object);
//     vdl_assert_ZeroObjects(num_object);
//
//     // Reserve enough space
//     vec_Reserve(v, v->size + num_object);
//     v->size = v->size + num_object;
//
//     // Move the old content to the end
//     vec_Set(v, i + num_object, vec_AddressOf(v, i), num_object);
//     // Copy in the new content
//     vec_Set(v, i, object, num_object);
// }
//
// #define TEMPLATE_vec_T_Find(T)                                          \
//     static inline int vec_##T##_Find(const vdl_vec *const v, T object)      \
//     {                                                                   \
//         vec_HealthCheck(v);                                             \
//         vdl_For_i(v->length) if (T##_array(v->data)[i] == object) return i; \
//         return -1;                                                      \
//     }
//
// TEMPLATE_vec_T_Find(char);
// TEMPLATE_vec_T_Find(int);
// TEMPLATE_vec_T_Find(double);
// TEMPLATE_vec_T_Find(vp);
//
// static inline int vec_Find(const vdl_vec *const v, void *const object)
// {
//     vec_HealthCheck(v);
//     vdl_assert_NullPointer(object);
//
//     switch (v->type)
//     {
//     case VEC_CHAR:
//         return vec_char_Find(v, vdl_char_Array(object)[0]);
//     case VEC_INT:
//         return vec_int_Find(v, vdl_int_Array(object)[0]);
//     case VEC_DOUBLE:
//         return vec_double_Find(v, vdl_double_Array(object)[0]);
//     case VEC_VP:
//         return vec_vp_Find(v, vdl_vp_Array(object)[0]);
//     }
// }
//
// #define TEMPLATE_vec_T_Print(T, f)                                          \
//     static inline void vec_##T##_Print(const vdl_vec *const v, const char *end) \
//     {                                                                       \
//         vec_HealthCheck(v);                                                 \
//         vdl_assert_IncompatibleType(v->type, lower_vec_##T);                    \
//                                                                             \
//         if (end == NULL)                                                    \
//             end = "\n";                                                     \
//         printf("[");                                                        \
//         vdl_For_i(v->length)                                                    \
//         {                                                                   \
//             printf(f, T##_array(v->data)[i]);                               \
//             if (i < v->size - 1)                                            \
//                 printf(", ");                                               \
//         }                                                                   \
//         printf("]%s", end);                                                 \
//     }
//
// TEMPLATE_vec_T_Print(char, "'%c'");
// TEMPLATE_vec_T_Print(int, "%d");
// TEMPLATE_vec_T_Print(double, "%f");
//
// static inline void vec_PrintRecursive(const vdl_vec *v, const char *end, vdl_vec *const guard)
// {
//     vec_HealthCheck(v);
//     vec_HealthCheck(guard);
//     vdl_assert_IncompatibleType(VEC_VP, guard->type);
//
//     // The next level should not go to this vector again
//     vec_Append(guard, &v, 1);
//
//     if (end == NULL)
//         end = "\n";
//     switch (v->type)
//     {
//     case VEC_CHAR:
//         vec_char_Print(v, "");
//         break;
//     case VEC_INT:
//         vec_int_Print(v, "");
//         break;
//     case VEC_DOUBLE:
//         vec_double_Print(v, "");
//         break;
//     case VEC_VP:
//         printf("[");
//         vdl_For_i(v->size)
//         {
//             const int idx = vec_Find(guard, vdl_vp_Array(v->data)[i]);
//             if (idx == -1)
//                 vec_PrintRecursive(vdl_vp_Array(v->data)[i], "", guard);
//             else
//                 printf("...");
//             if (i < v->size - 1)
//                 printf(", ");
//         }
//         printf("]%s", end);
//         break;
//     }
//
//     // Release the lock
//     vec_Pop(guard, guard->size - 1);
// }
//
// static inline void vec_Print(const vdl_vec *const v, const char *end)
// {
//     // A vector to avoid printing the cycling references
//     vp guard = vec_New(VEC_VP, 4);
//     vec_PrintRecursive(v, end, guard);
//
//     // Though `guard` is unreachable from the caller, which means it will be garbage collected
//     // It is still a good practice to manually clean it up at the end of the scope
//     vec_GCUntrack(guard);
// }
//
// static inline vp vec_Copy(const vdl_vec *const v)
// {
//     vec_HealthCheck(v);
//
//     // A new vector will be allocated
//     vp copy = vec_New(v->type, vdl_Capacity(v));
//     copy->size = v->size;
//
//     // Only the first layer of the content will be copied
//     vec_Set(copy, 0, v->data, v->size);
//     return copy;
// }
//
// static inline vp vec_DeepCopyRecursive(const vdl_vec *const v, vdl_vec *const guard, vdl_vec *const copied)
// {
//     vec_HealthCheck(v);
//     vec_HealthCheck(guard);
//     vec_HealthCheck(copied);
//     vdl_assert_IncompatibleType(VEC_VP, guard->type);
//     vdl_assert_IncompatibleType(VEC_VP, copied->type);
//
//     const int idx = vec_Find(guard, v);
//     if (idx != -1)
//         return vdl_vp_Array(copied->data)[idx];
//
//     vec_Append(guard, &v, 1);
//     vp copy = vec_Copy(v);
//     vec_Append(copied, &copy, 1);
//
//     if (v->type == VEC_VP)
//         vdl_For_i(v->size) vdl_vp_Array(v->data)[i] = vec_DeepCopyRecursive(vdl_vp_Array(v->data)[i], guard, copied);
//
//     return copy;
// }
//
// static inline vp vec_DeepCopy(const vdl_vec *const v)
// {
//     vec_HealthCheck(v);
//
//     // A vector to avoid copying the cycling references
//     vp guard = vec_New(VEC_VP, 4);
//     vp copied = vec_New(VEC_VP, 4);
//
//     vp copy = vec_DeepCopyRecursive(v, guard, copied);
//
//     // Though `guard` and `copied` are unreachable from the caller, which means they will be garbage collected
//     // It is still a good practice to manually clean them up at the end of the scope
//     vec_GCUntrack(copied);
//     vec_GCUntrack(guard);
//
//     return copy;
// }
//
#endif//VDL_VDLMEM_H