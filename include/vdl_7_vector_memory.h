//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_7_VECTOR_MEMORY_H
#define VDL_VDL_7_VECTOR_MEMORY_H


/*-----------------------------------------------------------------------------
 |  Vector memory checks
 ----------------------------------------------------------------------------*/

#define vdl_CheckRequestedCapacity(capacity) vdl_Expect((capacity) > 0 && (capacity) < VDL_VECTOR_MAX_CAPACITY, \
                                                        VDL_EXCEPTION_EXCEED_VECTOR_CAPACITY_LIMIT,             \
                                                        "The requested capacity [%d] is not in (0, %d]!",       \
                                                        capacity, VDL_VECTOR_MAX_CAPACITY)

/*-----------------------------------------------------------------------------
 |  Allocate vector on stack
 ----------------------------------------------------------------------------*/

#define vdl_T_LocalVector(T, UT, ...)               \
    &(VDL_VECTOR_T)                                 \
    {                                               \
        .Type      = UT,                            \
        .Mode      = VDL_MODE_STACK,                \
        .Class     = VDL_CLASS_VECTOR,              \
        .Capacity  = vdl_CountArg(__VA_ARGS__),     \
        .Length    = vdl_CountArg(__VA_ARGS__),     \
        .Attribute = NULL,                          \
        .Data      = (T[vdl_CountArg(__VA_ARGS__)]) \
        {                                           \
            __VA_ARGS__                             \
        }                                           \
    }

#define vdl_LocalCharVector(...) vdl_T_LocalVector(char, VDL_TYPE_CHAR, __VA_ARGS__)
#define vdl_LocalIntVector(...) vdl_T_LocalVector(int, VDL_TYPE_INT, __VA_ARGS__)
#define vdl_LocalDoubleVector(...) vdl_T_LocalVector(double, VDL_TYPE_DOUBLE, __VA_ARGS__)

/// Allocate a local vector on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (char/int/double). A series of objects or literals of the same type.
#define vdl_LocalVector(...) _Generic(vdl_GetArg1(__VA_ARGS__), char            \
                                      : vdl_LocalCharVector(__VA_ARGS__), int   \
                                      : vdl_LocalIntVector(__VA_ARGS__), double \
                                      : vdl_LocalDoubleVector(__VA_ARGS__))

/// Allocate a local vector consists of vectors of any type on stack.
/// @details A vector allocated on stack will be deallocated at the end of its lifetime,
/// i.e., when it is exited the block.
/// Accessing the vector outside of its lifetime is undefined behaviour, for example,
/// returning a stack allocated vector from a function and then using it. \n\n
/// The first provided value will be used to decide the type of the vector. \n\n
/// This is intended for constructing a simple vector passing as an argument.
/// @param ... (VDL_VECTOR_P). A series of objects or literals of the same type.
#define vdl_LocalList(...) vdl_T_LocalVector(VDL_VECTOR_P, VDL_TYPE_VECTOR_P, __VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Construct vector on heap
 ----------------------------------------------------------------------------*/

/// New an empty dynamically allocated vector.
/// @param type (VDL_TYPE_T). Vector type.
/// @param capacity (int). Capacity.
/// @return (VDL_VECTOR_P) A vector.
#define vdl_vector_primitive_NewEmpty(...) vdl_CallFunction(vdl_vector_primitive_NewEmpty_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_NewEmpty_BT(VDL_TYPE_T type, int capacity);

/// New an empty dynamically allocated vector.
/// @param type (VDL_TYPE_P). Vector type.
/// @param capacity (VDL_TYPE_P). Capacity.
/// @return (VDL_VECTOR_P) A vector.
#define vdl_vector_NewEmpty(...) vdl_CallFunction(vdl_vector_NewEmpty_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_NewEmpty_BT(VDL_VECTOR_P type, VDL_VECTOR_P capacity);

/// New and initialize a dynamically allocated vector by a scalar.
/// @param item (char). The item.
/// @return (VDL_VECTOR_P) A vector of length one.
#define vdl_vector_primitive_NewByChar(...) vdl_CallFunction(vdl_vector_primitive_NewByChar_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_NewByChar_BT(char item);

/// New and initialize a dynamically allocated vector by a scalar.
/// @param item (int). The item.
/// @return (VDL_VECTOR_P) A vector of length one.
#define vdl_vector_primitive_NewByInt(...) vdl_CallFunction(vdl_vector_primitive_NewByInt_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_NewByInt_BT(int item);

/// New and initialize a dynamically allocated vector by a scalar.
/// @param item (double). The item.
/// @return (VDL_VECTOR_P) A vector of length one.
#define vdl_vector_primitive_NewByDouble(...) vdl_CallFunction(vdl_vector_primitive_NewByDouble_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_NewByDouble_BT(double item);

/// New and initialize a dynamically allocated vector by a scalar.
/// @param item (VDL_VECTOR_P). The item.
/// @return (VDL_VECTOR_P) A vector of length one.
#define vdl_vector_primitive_NewByVectorPointer(...) vdl_CallFunction(vdl_vector_primitive_NewByVectorPointer_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_NewByVectorPointer_BT(VDL_VECTOR_P item);

/// New and initialize a dynamically allocated vector.
/// @param type (VDL_TYPE_T). Vector type.
/// @param capacity (int). Capacity.
/// @param item_pointer (const void *). Pointer to items.
/// @param number (int). Number of items.
#define vdl_vector_primitive_NewByArray(...) vdl_CallFunction(vdl_vector_primitive_NewByArray_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_NewByArray_BT(VDL_TYPE_T type, int capacity, const void *item_pointer, int number);

/// New and initialize a dynamically allocated vector.
/// @param string (const char *). Pointer to a string.
/// @param length (int). Length of the string.
#define vdl_vector_primitive_NewByString(...) vdl_CallFunction(vdl_vector_primitive_NewByString_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_NewByString_BT(const char *const string, const int length);

/// New and initialize a dynamically allocated vector by variadic arguments.
/// @param type (VDL_TYPE_T). Vector type.
/// @param length (int). Length of the vector.
/// @param ... (char/int/double/VDL_VECTOR_P). A series of items of the same type.
#define vdl_vector_primitive_NewByVariadic(...) vdl_CallFunction(vdl_vector_primitive_NewByVariadic_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_NewByVariadic_BT(VDL_TYPE_T type, int length, ...);

/// New and initialize a vector on heap and record it by the garbage collector.
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
#define vdl_vector_primitive_New(...) _Generic(vdl_GetArg1(__VA_ARGS__), char                                                                                \
                                               : vdl_vector_primitive_NewByVariadic(VDL_TYPE_CHAR, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), int              \
                                               : vdl_vector_primitive_NewByVariadic(VDL_TYPE_INT, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), double            \
                                               : vdl_vector_primitive_NewByVariadic(VDL_TYPE_DOUBLE, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), VDL_VECTOR_P   \
                                               : vdl_vector_primitive_NewByVariadic(VDL_TYPE_VECTOR_POINTER, vdl_CountArg(__VA_ARGS__), __VA_ARGS__), void * \
                                               : vdl_vector_primitive_NewByVariadic(VDL_TYPE_VECTOR_POINTER, vdl_CountArg(__VA_ARGS__), __VA_ARGS__))

/*-----------------------------------------------------------------------------
 |  Reserve space for heap allocated vector
 ----------------------------------------------------------------------------*/

/// Reserve space for a vector.
/// @details The function may allocate more memory than requested.
/// @param v (VDL_VECTOR_P). A vector.
/// @param capacity (int). Requested capacity.
#define vdl_vector_primitive_Reserve(...) vdl_CallVoidFunction(vdl_vector_primitive_Reserve_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_Reserve_BT(VDL_VECTOR_P v, int capacity);

/// Reserve space for a vector.
/// @details The function may allocate more memory than requested.
/// @param v (VDL_VECTOR_P). A vector.
/// @param capacity (VDL_VECTOR_P). Requested capacity.
#define vdl_vector_Reserve(...) vdl_CallVoidFunction(vdl_vector_Reserve_BT, __VA_ARGS__)
static inline void vdl_vector_Reserve_BT(VDL_VECTOR_P v, VDL_VECTOR_P capacity);

#endif//VDL_VDL_7_VECTOR_MEMORY_H
