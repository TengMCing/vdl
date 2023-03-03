//
// Created by Patrick Li on 20/2/2023.
//

#ifndef VDL_VDL_5_VECTOR_BASIC_H
#define VDL_VDL_5_VECTOR_BASIC_H


/*-----------------------------------------------------------------------------
 |  Vector
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Vector types
 ----------------------------------------------------------------------------*/

/// Primitive vector types.
/// @details Only types defined here can be legally stored in a vector. \n\n
/// VDL_TYPE_CHAR: 0, character type (could be signed or unsigned, this is implementation defined). \n\n
/// VDL_TYPE_INT: 1, signed integer type. \n\n
/// VDL_TYPE_DOUBLE: 2, signed double type. \n\n
/// VDL_TYPE_VECTOR: 3, vector type.
typedef enum VDL_TYPE_T
{
    VDL_TYPE_CHAR           = 0,
    VDL_TYPE_INT            = 1,
    VDL_TYPE_DOUBLE         = 2,
    VDL_TYPE_VECTOR_POINTER = 3
} VDL_TYPE_T;

/// String representation of primitive array types.
static const char *const VDL_TYPE_STRING[4] = {
        [VDL_TYPE_CHAR]           = "VDL_TYPE_CHAR",
        [VDL_TYPE_INT]            = "VDL_TYPE_INT",
        [VDL_TYPE_DOUBLE]         = "VDL_TYPE_DOUBLE",
        [VDL_TYPE_VECTOR_POINTER] = "VDL_TYPE_VECTOR_POINTER"};

/*-----------------------------------------------------------------------------
 |  Vector storage modes
 ----------------------------------------------------------------------------*/

/// Storage mode of a vector.
/// @details
/// VDL_MODE_STACK: 0, stack allocated. \n\n
/// VDL_MODE_HEAP: 1, heap allocated.
typedef enum VDL_MODE_T
{
    VDL_MODE_STACK = 0,
    VDL_MODE_HEAP  = 1
} VDL_MODE_T;

/// String representation of storage mode of a vector.
static const char *const VDL_MODE_STRING[2] = {
        [VDL_MODE_STACK] = "VDL_MODE_STACK",
        [VDL_MODE_HEAP]  = "VDL_MODE_HEAP"};


/*-----------------------------------------------------------------------------
 |  Vector class
 ----------------------------------------------------------------------------*/

/// Class is stored as an int.
#define VDL_CLASS_T int

#define VDL_CLASS_VECTOR 0

/*-----------------------------------------------------------------------------
 |  Vector definition
 ----------------------------------------------------------------------------*/

/// Declare the vector struct.
typedef struct VDL_VECTOR_T VDL_VECTOR_T;

/// A pointer to a vector struct.
typedef VDL_VECTOR_T *VDL_VECTOR_P;

/// Vector struct.
/// @param Type (const VDL_TYPE). Type of the vector.
/// @param Mode (const VDL_MODE). Storage mode of the vector.
/// @param Capacity (int). Capacity of the vector.
/// @param Length (int). Length of the vector.
/// @param Data (void*). Data pointer.
struct VDL_VECTOR_T
{
    const VDL_TYPE_T Type;
    const VDL_MODE_T Mode;
    const VDL_CLASS_T Class;
    int Capacity;
    int Length;
    VDL_VECTOR_P Attribute;
    void *Data;
};


#define VDL_VECTOR_MAX_CAPACITY (INT_MAX - 512)

/*-----------------------------------------------------------------------------
 |  Missing values
 ----------------------------------------------------------------------------*/

/// Missing value of char.
#define VDL_CHAR_NA CHAR_MAX
/// Missing value of int.
#define VDL_INT_NA INT_MAX
/// Missing value of double.
#define VDL_DOUBLE_NA ((double) NAN)
/// Missing value of pointer.
#define VDL_VECTOR_POINTER_NA NULL

/*-----------------------------------------------------------------------------
 |  Different array types
 ----------------------------------------------------------------------------*/

#define VDL_CHAR_ARRAY char *const
#define VDL_CONST_CHAR_ARRAY const char *const

#define VDL_INT_ARRAY int *const
#define VDL_CONST_INT_ARRAY const int *const

#define VDL_DOUBLE_ARRAY double *const
#define VDL_CONST_DOUBLE_ARRAY const double *const

#define VDL_VECTOR_POINTER_ARRAY VDL_VECTOR_T **const
#define VDL_VECTOR_CONST_POINTER_ARRAY VDL_VECTOR_T *const *const

/*-----------------------------------------------------------------------------
 |  Size of vector type
 ----------------------------------------------------------------------------*/

/// Size of vector type.
static const size_t VDL_TYPE_SIZE[4] = {
        [VDL_TYPE_CHAR]           = sizeof(char),
        [VDL_TYPE_INT]            = sizeof(int),
        [VDL_TYPE_DOUBLE]         = sizeof(double),
        [VDL_TYPE_VECTOR_POINTER] = sizeof(VDL_VECTOR_P)};

/*-----------------------------------------------------------------------------
 |  Size of a vector
 ----------------------------------------------------------------------------*/

/// Get the size of the data of a vector.
/// @param v (VDL_VECTOR_P). A vector.
/// @return (size_t). The size.
#define vdl_vector_primitive_SizeOfData(v) ((size_t) (v)->Capacity * VDL_TYPE_SIZE[(v)->Type])

/// Get the size of a vector.
/// @param v (VDL_VECTOR_P). A vector.
/// @return (size_t). The size.
#define vdl_vector_primitive_SizeOfVector(v) ((size_t) (v)->Capacity * VDL_TYPE_SIZE[(v)->Type] + sizeof(VDL_VECTOR_T))

/*-----------------------------------------------------------------------------
 |  Vector basic checks
 ----------------------------------------------------------------------------*/

#define vdl_CheckNullPointer(p) vdl_Expect((p) != NULL,                   \
                                           VDL_EXCEPTION_NULL_POINTER,    \
                                           "Null pointer [%s] provided!", \
                                           #p)

#define vdl_CheckFailedAllocation(p) vdl_Expect((p) != NULL,                         \
                                                VDL_EXCEPTION_FAILED_ALLOCATION,     \
                                                "Memory allocation of [%s] failed!", \
                                                #p)

#define vdl_CheckIndexOutOfBound(v, i) vdl_Expect((i) >= 0 && (i) < (v)->Length,                    \
                                                  VDL_EXCEPTION_INDEX_OUT_OF_BOUND,                 \
                                                  "Index out of bound! Index [%d] not in [0, %d)!", \
                                                  i,                                                \
                                                  (v)->Length)

#define vdl_CheckType(input_type, expected_type) vdl_Expect((input_type) == (expected_type),                                    \
                                                            VDL_EXCEPTION_UNEXPECTED_TYPE,                                      \
                                                            "Unexpected vector type [%s] provided! Vector type [%s] Expected!", \
                                                            VDL_TYPE_STRING[input_type],                                        \
                                                            VDL_TYPE_STRING[expected_type])

#define vdl_CheckUnknownType(input_type) vdl_Expect((input_type) >= VDL_TYPE_INT && (input_type) <= VDL_TYPE_VECTOR_POINTER, \
                                                    VDL_EXCEPTION_UNKNOWN_TYPE,                                              \
                                                    "Unknown vector type [%d] provided!",                                    \
                                                    input_type)

#define vdl_CheckMode(input_mode, expected_mode) vdl_Expect((input_mode) == (expected_mode),                                    \
                                                            VDL_EXCEPTION_UNEXPECTED_MODE,                                      \
                                                            "Unexpected vector mode [%s] provided! Vector mode [%s] Expected!", \
                                                            VDL_MODE_STRING[input_mode],                                        \
                                                            VDL_MODE_STRING[expected_mode])

#define vdl_CheckLength(input_length, expected_length) vdl_Expect((input_length) == (expected_length),                                    \
                                                                  VDL_EXCEPTION_UNEXPECTED_LENGTH,                                        \
                                                                  "Unexpected vector length [%d] provided! Vector length [%d] Expected!", \
                                                                  input_length,                                                           \
                                                                  expected_length)


#define vdl_CheckIncompatibleLength(input_length, expected_length) vdl_Expect((input_length) == (expected_length) || (input_length) == 1,                      \
                                                                              VDL_EXCEPTION_INCOMPATIBLE_LENGTH,                                               \
                                                                              "Incompatible vector length [%d] provided! Vector length [%d] or [1] Expected!", \
                                                                              input_length,                                                                    \
                                                                              expected_length)

#define vdl_CheckZeroLength(input_length) vdl_Expect((input_length) > 0,                          \
                                                     VDL_EXCEPTION_NON_POSITIVE_LENGTH,           \
                                                     "Non-positive vector length [%d] provided!", \
                                                     input_length)


#define vdl_CheckNumberOfItems(number) vdl_Expect((number) > 0,                                  \
                                                  VDL_EXCEPTION_NON_POSITIVE_NUMBER_OF_ITEMS,    \
                                                  "Non-positive number of items [%d] provided!", \
                                                  number)

#define vdl_CheckNullVectorAndNullContainer(v) \
    do {                                       \
        vdl_CheckNullPointer(v);               \
        vdl_CheckNullPointer((v)->Data);       \
    } while (0)

#define vdl_CheckNullArrayAndNegativeLength(array, number) \
    do {                                                   \
        vdl_CheckNullPointer(array);                       \
        vdl_CheckNumberOfItems(number);                    \
    } while (0)

#define vdl_CheckEmptyAttribute(v) vdl_Expect(((v)->Attribute) != NULL,         \
                                              VDL_EXCEPTION_EMPTY_ATTRIBUTE,    \
                                              "Vector [%s] has no attributes!", \
                                              #v)

/*-----------------------------------------------------------------------------
 |  Accessing the vector data unsafely
 ----------------------------------------------------------------------------*/

#define vdl_vector_primitive_UnsafeAddressOf(v, i) ((VDL_CHAR_ARRAY) (v)->Data + VDL_TYPE_SIZE[(v)->Type] * (size_t) (i))

#define vdl_vector_primitive_UnsafeCharAt(v, i) (((VDL_CHAR_ARRAY) (v)->Data)[i])
#define vdl_vector_primitive_UnsafeConstCharAt(v, i) (((VDL_CONST_CHAR_ARRAY) (v)->Data)[i])

#define vdl_vector_primitive_UnsafeIntAt(v, i) (((VDL_INT_ARRAY) (v)->Data)[i])
#define vdl_vector_primitive_UnsafeConstIntAt(v, i) (((VDL_CONST_INT_ARRAY) (v)->Data)[i])

#define vdl_vector_primitive_UnsafeDoubleAt(v, i) (((VDL_DOUBLE_ARRAY) (v)->Data)[i])
#define vdl_vector_primitive_UnsafeConstDoubleAt(v, i) (((VDL_CONST_DOUBLE_ARRAY) (v)->Data)[i])

#define vdl_vector_primitive_UnsafeVectorPointerAt(v, i) (((VDL_VECTOR_POINTER_ARRAY) (v)->Data)[i])
#define vdl_vector_primitive_UnsafeVectorConstPointerAt(v, i) (((VDL_VECTOR_CONST_POINTER_ARRAY) (v)->Data)[i])

#define vdl_vector_primitive_Front(v) ((VDL_CHAR_ARRAY) (v)->Data)
#define vdl_vector_primitive_Back(v) vdl_vector_primitive_UnsafeAddressOf(v, (v)->Length)

/*-----------------------------------------------------------------------------
 |  Accessing the vector data safely
 ----------------------------------------------------------------------------*/

/// Get the address of an item from a vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). Index of the item.
/// @return (void *) Pointer to the item.
#define vdl_vector_primitive_GetAddress(...) vdl_CallFunction(vdl_vector_primitive_GetAddress_BT, void *, __VA_ARGS__)
static inline void *vdl_vector_primitive_GetAddress_BT(VDL_VECTOR_P v, int i);

/// Get a char from a vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). Index of the item.
/// @return (char) A char.
#define vdl_vector_primitive_GetChar(...) vdl_CallFunction(vdl_vector_primitive_GetChar_BT, char, __VA_ARGS__)
static inline char vdl_vector_primitive_GetChar_BT(VDL_VECTOR_P v, int i);

/// Get an int from a vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). Index of the item.
/// @return (int) An int.
#define vdl_vector_primitive_GetInt(...) vdl_CallFunction(vdl_vector_primitive_GetInt_BT, int, __VA_ARGS__)
static inline int vdl_vector_primitive_GetInt_BT(VDL_VECTOR_P v, int i);

/// Get a double from a vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). Index of the item.
/// @return (double) An double.
#define vdl_vector_primitive_GetDouble(...) vdl_CallFunction(vdl_vector_primitive_GetDouble_BT, double, __VA_ARGS__)
static inline double vdl_vector_primitive_GetDouble_BT(VDL_VECTOR_P v, int i);

/// Get a vector pointer from a vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). Index of the item.
/// @return (VDL_VECTOR_P) A vector pointer.
#define vdl_vector_primitive_GetVectorPointer(...) vdl_CallFunction(vdl_vector_primitive_GetVectorPointer_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_primitive_GetVectorPointer_BT(VDL_VECTOR_P v, int i);

/*-----------------------------------------------------------------------------
 |  Set the vector data unsafely
 ----------------------------------------------------------------------------*/

/// Set the ith item of a char vector. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). An index.
/// @param item (char). An item.
#define vdl_vector_primitive_UnsafeSetChar(v, i, item)  \
    do {                                                \
        vdl_vector_primitive_UnsafeCharAt(v, i) = item; \
    } while (0)

/// Set the ith item of an int vector. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). An index.
/// @param item (int). An item.
#define vdl_vector_primitive_UnsafeSetInt(v, i, item)  \
    do {                                               \
        vdl_vector_primitive_UnsafeIntAt(v, i) = item; \
    } while (0)

/// Set the ith item of a double vector. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). An index.
/// @param item (double). An item.
#define vdl_vector_primitive_UnsafeSetDouble(v, i, item)  \
    do {                                                  \
        vdl_vector_primitive_UnsafeDoubleAt(v, i) = item; \
    } while (0)

/// Set the ith item of a VDL_VECTOR_P vector. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). An index.
/// @param item (VDL_VECTOR_P). An item.
#define vdl_vector_primitive_UnsafeSetVectorPointer(v, i, item)  \
    do {                                                         \
        vdl_vector_primitive_UnsafeVectorPointerAt(v, i) = item; \
    } while (0)

/// Set multiple items of a vector by using `memcpy`. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). The starting index.
/// @param item_pointer (const void *). A pointer to items.
/// @param number (int). Number of items to be set.
#define vdl_vector_primitive_UnsafeSetByArrayAndMemcpy(v, i, item_pointer, number)   \
    do {                                                                             \
        void *_begin = vdl_vector_primitive_UnsafeAddressOf(v, i);                   \
        memmove(_begin, item_pointer, VDL_TYPE_SIZE[(v)->Type] * (size_t) (number)); \
    } while (0)

/// Set multiple items of a vector by using `memmove`. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). The starting index.
/// @param item_pointer (const void *). A pointer to items.
/// @param number (int). Number of items to be set.
#define vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v, i, item_pointer, number)  \
    do {                                                                             \
        void *_begin = vdl_vector_primitive_UnsafeAddressOf(v, i);                   \
        memmove(_begin, item_pointer, VDL_TYPE_SIZE[(v)->Type] * (size_t) (number)); \
    } while (0)

/// Set multiple items of a char vector by indices. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param item_pointer (const char *). A pointer to items.
/// @param index_pointer (const int *). A pointer to indices.
/// @param number (int). Number of items.
#define vdl_vector_primitive_UnsafeSetCharByArrayAndIndex(v, item_pointer, index_pointer, number) \
    do {                                                                                          \
        VDL_CONST_INT_ARRAY index_array = index_pointer;                                          \
        VDL_CONST_CHAR_ARRAY item_array = item_pointer;                                           \
        VDL_CHAR_ARRAY data_array       = (v)->Data;                                              \
        vdl_for_j(number)                                                                         \
        {                                                                                         \
            data_array[index_array[j]] = item_array[j];                                           \
        }                                                                                         \
    } while (0)

/// Set multiple items of an int vector by indices. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param item_pointer (const int *). A pointer to items.
/// @param index_pointer (const int *). A pointer to indices.
/// @param number (int). Number of items.
#define vdl_vector_primitive_UnsafeSetIntByArrayAndIndex(v, item_pointer, index_pointer, number) \
    do {                                                                                         \
        VDL_CONST_INT_ARRAY index_array = index_pointer;                                         \
        VDL_CONST_INT_ARRAY item_array  = item_pointer;                                          \
        VDL_INT_ARRAY data_array        = (v)->Data;                                             \
        vdl_for_j(number)                                                                        \
        {                                                                                        \
            data_array[index_array[j]] = item_array[j];                                          \
        }                                                                                        \
    } while (0)

/// Set multiple items of a double vector by indices. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param item_pointer (const double *). A pointer to items.
/// @param index_pointer (const int *). A pointer to indices.
/// @param number (int). Number of items.
#define vdl_vector_primitive_UnsafeSetDoubleByArrayAndIndex(v, item_pointer, index_pointer, number) \
    do {                                                                                            \
        VDL_CONST_INT_ARRAY index_array   = index_pointer;                                          \
        VDL_CONST_DOUBLE_ARRAY item_array = item_pointer;                                           \
        VDL_DOUBLE_ARRAY data_array       = (v)->Data;                                              \
        vdl_for_j(number)                                                                           \
        {                                                                                           \
            data_array[index_array[j]] = item_array[j];                                             \
        }                                                                                           \
    } while (0)

/// Set multiple items of a VDL_VECTOR_P vector by indices. No checks will be performed.
/// @param v (VDL_VECTOR_P). A vector.
/// @param item_pointer (VDL_VECTOR_T *const *). A pointer to items.
/// @param index_pointer (const int *). A pointer to indices.
/// @param number (int). Number of items.
#define vdl_vector_primitive_UnsafeSetVectorPointerByArrayAndIndex(v, item_pointer, index_pointer, number) \
    do {                                                                                                   \
        VDL_CONST_INT_ARRAY index_array           = index_pointer;                                         \
        VDL_VECTOR_CONST_POINTER_ARRAY item_array = item_pointer;                                          \
        VDL_VECTOR_POINTER_ARRAY data_array       = (v)->Data;                                             \
        vdl_for_j(number)                                                                                  \
        {                                                                                                  \
            data_array[index_array[j]] = item_array[j];                                                    \
        }                                                                                                  \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Set the vector data safely (low-level API)
 ----------------------------------------------------------------------------*/

/// Set the ith item of a char vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). An index.
/// @param item (char). An item.
#define vdl_vector_primitive_SetChar(...) vdl_CallVoidFunction(vdl_vector_primitive_SetChar_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetChar_BT(VDL_VECTOR_P v, int i, char item);

/// Set the ith item of an int vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). An index.
/// @param item (int). An item.
#define vdl_vector_primitive_SetInt(...) vdl_CallVoidFunction(vdl_vector_primitive_SetInt_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetInt_BT(VDL_VECTOR_P v, int i, int item);

/// Set the ith item of a double vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). An index.
/// @param item (double). An item.
#define vdl_vector_primitive_SetDouble(...) vdl_CallVoidFunction(vdl_vector_primitive_SetDouble_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetDouble_BT(VDL_VECTOR_P v, int i, double item);

/// Set the ith item of a VDL_VECTOR_P vector. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). An index.
/// @param item (VDL_VECTOR_P). An item.
#define vdl_vector_primitive_SetVectorPointer(...) vdl_CallVoidFunction(vdl_vector_primitive_SetVectorPointer_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetVectorPointer_BT(VDL_VECTOR_P v, int i, VDL_VECTOR_P item);

/// Set many items of a vector by using `memmove`. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). The staring index.
/// @param item_pointer (const void *). A pointer to items.
/// @param number (int). Number of items to be set.
#define vdl_vector_primitive_SetByArrayAndMemmove(...) vdl_CallVoidFunction(vdl_vector_primitive_SetByArrayAndMemmove_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetByArrayAndMemmove_BT(VDL_VECTOR_P v, int i, const void *item_pointer, int number);

/// Set many items of a vector by using `memcpy`. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param i (int). The staring index.
/// @param item_pointer (const void *). A pointer to items.
/// @param number (int). Number of items to be set.
#define vdl_vector_primitive_SetByArrayAndCpy(...) vdl_CallVoidFunction(vdl_vector_primitive_SetByArrayAndMemcpy_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetByArrayAndMemcpy_BT(VDL_VECTOR_P v, int i, const void *item_pointer, int number);

/// Set multiple items of a char vector by indices. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param item_pointer (const char *). A pointer to items.
/// @param index_pointer (const int *). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_vector_primitive_SetCharByArrayAndIndex(...) vdl_CallVoidFunction(vdl_vector_primitive_SetCharByArrayAndIndex_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetCharByArrayAndIndex_BT(VDL_VECTOR_P v, const char *item_pointer, const int *index_pointer, int number);

/// Set multiple items of an int vector by indices. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param item_pointer (const int *). A pointer to items.
/// @param index_pointer (const int *). A pointer to indices.
/// @param number (int). Number of items.
#define vdl_vector_primitive_SetIntByIndices(...) vdl_CallVoidFunction(vdl_vector_primitive_SetIntByArrayAndIndex_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetIntByArrayAndIndex_BT(VDL_VECTOR_P v, const int *item_pointer, const int *index_pointer, int number);

/// Set multiple items of a double vector by indices. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param item_pointer (const double *). A pointer to items.
/// @param index_pointer (const int *). A pointer to indices.
/// @param number (int). Number of items.
#define vdl_vector_primitive_SetDoubleByArrayAndIndex(...) vdl_CallVoidFunction(vdl_vector_primitive_SetDoubleByArrayAndIndex_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetDoubleByArrayAndIndex_BT(VDL_VECTOR_P v, const double *item_pointer, const int *index_pointer, int number);

/// Set multiple items of a VDL_VECTOR_P vector by indices. Boundary conditions will be checked.
/// @param v (VDL_VECTOR_P). A vector.
/// @param item_pointer (VDL_VECTOR_T *const *). A pointer to items.
/// @param index_pointer (const int *). A pointer to indices.
/// @param number (int). Number of items.
#define vdl_vector_primitive_SetVectorPointerByArrayAndIndex(...) vdl_CallVoidFunction(vdl_vector_primitive_SetVectorPointerByArrayAndIndex_BT, __VA_ARGS__)
static inline void vdl_vector_primitive_SetVectorPointerByArrayAndIndex_BT(VDL_VECTOR_P v, VDL_VECTOR_T *const *item_pointer, const int *index_pointer, int number);


/*-----------------------------------------------------------------------------
 |  Convert an integer vector to a boolean scalar value
 ----------------------------------------------------------------------------*/

/// Convert an integer vector to a boolean scalar value.
/// @param condition (VDL_VECTOR_P). A vector.
/// @return (int) A boolean value.
#define vdl_vector_primitive_ToBool(...) vdl_CallFunction(vdl_vector_primitive_ToBool_BT, int, __VA_ARGS__)
static inline int vdl_vector_primitive_ToBool_BT(VDL_VECTOR_P condition);


/*-----------------------------------------------------------------------------
 |  If statement
 ----------------------------------------------------------------------------*/

/// If a integer vector is True. Only works if the vector contains one element.
/// @param condition (VDL_VECTOR_P). A vector.
#define vdl_If(condition) if (vdl_vector_primitive_ToBool(condition))

/*-----------------------------------------------------------------------------
 |  While statement
 ----------------------------------------------------------------------------*/

/// While a integer vector is True. Only works if the vector contains one element.
/// @param condition (VDL_VECTOR_P). A vector.
#define vdl_While(condition) while (vdl_vector_primitive_ToBool(condition))

#endif//VDL_VDL_5_VECTOR_BASIC_H
