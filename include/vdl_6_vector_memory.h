//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_6_VECTOR_MEMORY_H
#define VDL_VDL_6_VECTOR_MEMORY_H

/*-----------------------------------------------------------------------------
 |  Allocate vector on stack
 ----------------------------------------------------------------------------*/

#define vdl_T_LocalVector(T, UT, ...)              \
    &(VDL_VECTOR_T)                                \
    {                                              \
        .Type     = UT,                            \
        .Mode     = VDL_MODE_STACK,                \
        .Capacity = vdl_CountArg(__VA_ARGS__),     \
        .Length   = vdl_CountArg(__VA_ARGS__),     \
        .Data     = (T[vdl_CountArg(__VA_ARGS__)]) \
        {                                          \
            __VA_ARGS__                            \
        }                                          \
    }

#define vdl_char_LocalVector(...) vdl_T_LocalVector(char, VDL_TYPE_CHAR, __VA_ARGS__)
#define vdl_int_LocalVector(...) vdl_T_LocalVector(int, VDL_TYPE_INT, __VA_ARGS__)
#define vdl_double_LocalVector(...) vdl_T_LocalVector(double, VDL_TYPE_DOUBLE, __VA_ARGS__)

/// @description Allocate a local vector on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (char/int/double). A series of objects or literals of the same type.
#define vdl_LocalVector(...) _Generic(vdl_GetArg1(__VA_ARGS__), char             \
                                      : vdl_char_LocalVector(__VA_ARGS__), int   \
                                      : vdl_int_LocalVector(__VA_ARGS__), double \
                                      : vdl_double_LocalVector(__VA_ARGS__))

/// @description Allocate a local vector consists of vectors of any type on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (VDL_VECTOR_P). A series of objects or literals of the same type.
#define vdl_LocalVectorPointerVector(...) vdl_T_LocalVector(VDL_VECTOR_P, VDL_TYPE_VECTOR_P, __VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Allocate an empty vector on heap
 ----------------------------------------------------------------------------*/

#define vdl_NewVector(...) vdl_CallFunction(vdl_NewVector_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_NewVector_BT(const VDL_TYPE_T type, const int capacity)
{
    vdl_Expect(capacity > 0,
               VDL_EXCEPTION_NON_POSITIVE_CAPACITY,
               "Unable to create a vector with non-positive capacity [%d]!",
               capacity);
    vdl_Expect(capacity < VDL_VECTOR_MAX_CAPACITY,
               VDL_EXCEPTION_EXCEED_VECTOR_CAPACITY_LIMIT,
               "The requested capacity [%d] is larger than the limit!",
               capacity);
    // Create a vector and copy in the content
    VDL_VECTOR_P v       = vdl_Malloc(sizeof(VDL_VECTOR_T), 1);
    VDL_VECTOR_P local_v = &(VDL_VECTOR_T){.Capacity = capacity,
                                           .Mode     = VDL_MODE_HEAP,
                                           .Type     = type,
                                           .Length   = 0,
                                           .Data     = NULL};
    memcpy(v, local_v, sizeof(VDL_VECTOR_T));

    // Allocate memory for the data container
    v->Data = vdl_Malloc((size_t) capacity * VDL_TYPE_SIZE[type], 1);

    // Record the vector in the vector table
    vdl_GarbageCollectorRecord(v);

    vdl_ExceptionDeregisterCleanUp(v);
    vdl_ExceptionDeregisterCleanUp(v->Data);
    return v;
}

#define vdl_InitVector(...) vdl_CallFunction(vdl_InitVector_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_InitVector_BT(const VDL_TYPE_T type, const int capacity, const void *const item_pointer, const int number)
{
    VDL_VECTOR_P v = vdl_NewVector(type, capacity);
    vdl_SetByMemmove(v, 0, item_pointer, number);
    return v;
}

#define vdl_InitVectorVariadic(...) vdl_CallFunction(vdl_InitVectorVariadic_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_InitVectorVariadic_BT(const VDL_TYPE_T type, const int length, ...)
{
    VDL_VECTOR_P v = vdl_NewVector(type, length);
    v->Length      = length;

    va_list ap;
    va_start(ap, length);

    switch (type)
    {
        case VDL_TYPE_CHAR:
        {
            vdl_for_i(length) vdl_UnsafeSetChar(v, i, (char) va_arg(ap, int));
            break;
        }
        case VDL_TYPE_INT:
        {
            vdl_for_i(length) vdl_UnsafeSetInt(v, i, va_arg(ap, int));
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            vdl_for_i(length) vdl_UnsafeSetDouble(v, i, va_arg(ap, double));
            break;
        }
        case VDL_TYPE_VECTOR_P:
        {
            vdl_for_i(length) vdl_UnsafeSetVectorPointer(v, i, va_arg(ap, VDL_VECTOR_P));
            break;
        }
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
/// @param ... (char/int/double/VDL_VECTOR_P). A series of objects of the same type.
/// @return (VDL_VECTOR_P) A vector.
#define vdl_Vector(...) _Generic(vdl_GetArg1(__VA_ARGS__), char                                                                            \
                                 : vdl_InitVectorVariadic(VDL_TYPE_CHAR, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), int                      \
                                 : vdl_InitVectorVariadic(VDL_TYPE_INT, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), double                    \
                                 : vdl_InitVectorVariadic(VDL_TYPE_DOUBLE, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), VDL_VECTOR_T *         \
                                 : vdl_InitVectorVariadic(VDL_TYPE_VECTOR_P, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), const VDL_VECTOR_T * \
                                 : vdl_InitVectorVariadic(VDL_TYPE_VECTOR_P, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), void *               \
                                 : vdl_InitVectorVariadic(VDL_TYPE_VECTOR_P, vdl_CountArg(__VA_ARGS__), __VA_ARGS__))


#endif//VDL_VDL_6_VECTOR_MEMORY_H
