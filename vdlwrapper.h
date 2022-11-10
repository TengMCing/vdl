//
// Created by Patrick Li on 30/9/22.
//

#ifndef VDL_VDLWRAPPER_H
#define VDL_VDLWRAPPER_H

#include "vdlassert.h"

/*-----------------------------------------------------------------------------
 |  Cast void pointer to different types
 ----------------------------------------------------------------------------*/

#define vdl_internal_char_Array(voidp) ((char *) (voidp))
#define vdl_internal_int_Array(voidp) ((int *) (voidp))
#define vdl_internal_double_Array(voidp) ((double *) (voidp))
#define vdl_internal_vp_Array(voidp) ((vdl_vp *) (voidp))

#define vdl_internal_char_Array0(voidp) (vdl_internal_char_Array(voidp)[0])
#define vdl_internal_int_Array0(voidp) (vdl_internal_int_Array(voidp)[0])
#define vdl_internal_double_Array0(voidp) (vdl_internal_double_Array(voidp)[0])
#define vdl_internal_vp_Array0(voidp) (vdl_internal_vp_Array(voidp)[0])

/*-----------------------------------------------------------------------------
 |  Type of a vector
 ----------------------------------------------------------------------------*/

/// @description Type of a vector.
#define vdl_Type(v) ((v)->type)
/// @description String representation of the type of a vector.
#define vdl_TypeStr(v) (VDL_TYPE_STR[vdl_Type(v)])

/*-----------------------------------------------------------------------------
 |  Storage mode of a vector
 ----------------------------------------------------------------------------*/

/// @description Mode of a vector.
#define vdl_Mode(v) ((v)->mode)
/// @description String representation of the mode of a vector.
#define vdl_ModeStr(v) (VDL_MODE_STR[vdl_Mode(v)])

/*-----------------------------------------------------------------------------
 |  Capacity of a vector
 ----------------------------------------------------------------------------*/

#define vdl_internal_Capacity(v) ((v)->capacity)
/// @description A copy of the capacity of a vector.
#define vdl_Capacity(v)          \
    (int)                        \
    {                            \
        vdl_internal_Capacity(v) \
    }


/*-----------------------------------------------------------------------------
 |  Length of a vector
 ----------------------------------------------------------------------------*/

#define vdl_internal_Length(v) ((v)->length)
/// @description A copy of the length of a vector.
#define vdl_Length(v)          \
    (int)                      \
    {                          \
        vdl_internal_Length(v) \
    }

/*-----------------------------------------------------------------------------
 |  Data of the vector
 ----------------------------------------------------------------------------*/

#define vdl_internal_Data(v) ((v)->data)

/*-----------------------------------------------------------------------------
 |  Size of type or vector
 ----------------------------------------------------------------------------*/

/// @description Get the size of a type.
/// @param type (VDL_TYPE). Vector type.
/// @return (size_t) The size of the type.
#define vdl_SizeOfType(type) (VDL_TYPE_SIZE[type])
static const size_t VDL_TYPE_SIZE[4] = {
        [VDL_TYPE_CHAR]   = sizeof(char),
        [VDL_TYPE_INT]    = sizeof(int),
        [VDL_TYPE_DOUBLE] = sizeof(double),
        [VDL_TYPE_VP]     = sizeof(vdl_vp)};

/// @description Get the size of the data of a vector.
/// @param v (vdl_vec*). A vector.
/// @return (size_t) The size of the data of a vector.
#define vdl_SizeOfData(v) ((size_t) vdl_internal_Capacity(v) * vdl_SizeOfType(vdl_Type(v)))

/// @description Get the size of a vector.
/// @param v (vdl_vec*). A vector.
/// @return (size_t) The size of a vector.
#define vdl_SizeOfVec(v) ((size_t) vdl_internal_Capacity(v) * vdl_SizeOfType(vdl_Type(v)) + sizeof(vdl_vec))

/*-----------------------------------------------------------------------------
 |  Accessing the vector data
 ----------------------------------------------------------------------------*/

/// @description Get the address of the ith item from the data of a vector. No checks will be performed.
#define vdl_internal_Get(v, i) (vdl_internal_char_Array(vdl_internal_Data(v)) + vdl_SizeOfType(vdl_Type(v)) * (size_t) (i))
/// @description Get the ith item from the data of a vector as char. No checks will be performed.
#define vdl_internal_GetChar(v, i) (vdl_internal_char_Array(vdl_internal_Data(v))[i])
/// @description Get the ith item from the data of a vector as int. No checks will be performed.
#define vdl_internal_GetInt(v, i) (vdl_internal_int_Array(vdl_internal_Data(v))[i])
/// @description Get the ith item from the data of a vector as double. No checks will be performed.
#define vdl_internal_GetDouble(v, i) (vdl_internal_double_Array(vdl_internal_Data(v))[i])
/// @description Get the ith item from the data of a vector as vector poitner. No checks will be performed.
#define vdl_internal_GetVP(v, i) (vdl_internal_vp_Array(vdl_internal_Data(v))[i])


/// @description Get an item from a vector. Boundary conditions will be checked.
/// @details Since vector is dynamically typed, the return type can not be assumed at
/// compile-time. Void pointer will be returned.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (void*) Pointer to the item.
#define vdl_Get(...) vdl_bt_Caller(vdl_Get_BT, void *, __VA_ARGS__)
static inline void *vdl_Get_BT(vdl_bt bt, const vdl_vec *v, const int i)
{
    vdl_bt_Push(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    return vdl_internal_Get(v, i);
VDL_EXCEPTION:
    return NULL;
}

/// @description Get the ith item of a vector and interpret it as a char.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (char) A character.
#define vdl_GetChar(...) vdl_bt_Caller(vdl_GetChar_BT, char, __VA_ARGS__)
static inline char vdl_GetChar_BT(vdl_bt bt, const vdl_vec *v, const int i)
{
    vdl_bt_Push(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_assert_IncompatibleType(VDL_TYPE_CHAR, vdl_Type(v));
    return vdl_internal_GetChar(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_CHAR_NA;
}

/// @description Get the ith item of a vector and interpret it as an int.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (int) An integer.
#define vdl_GetInt(...) vdl_bt_Caller(vdl_GetInt_BT, int, __VA_ARGS__)
static inline int vdl_GetInt_BT(vdl_bt bt, const vdl_vec *v, const int i)
{
    vdl_bt_Push(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_assert_IncompatibleType(VDL_TYPE_INT, vdl_Type(v));
    return vdl_internal_GetInt(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_INT_NA;
}

/// @description Get the ith item of a vector and interpret it as a double.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (double) A double.
#define vdl_GetDouble(...) vdl_bt_Caller(vdl_GetDouble_BT, double, __VA_ARGS__)
static inline double vdl_GetDouble_BT(vdl_bt bt, const vdl_vec *v, const int i)
{
    vdl_bt_Push(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_assert_IncompatibleType(VDL_TYPE_DOUBLE, vdl_Type(v));
    return vdl_internal_GetDouble(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_DOUBLE_NA;
}

/// @description Get the ith item of a vector and interpret it as a vector consists of vectors of any type.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (vdl_vp) A vector.
#define vdl_GetVp(...) vdl_bt_Caller(vdl_GetVp_BT, vdl_vp, __VA_ARGS__)
static inline vdl_vp vdl_GetVp_BT(vdl_bt bt, const vdl_vec *v, const int i)
{
    vdl_bt_Push(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_assert_IncompatibleType(VDL_TYPE_VP, vdl_Type(v));
    return vdl_internal_GetVP(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_VP_NA;
}

/*-----------------------------------------------------------------------------
 |  Saving to the vector data
 ----------------------------------------------------------------------------*/

/// @description Set a series of items for a vector. Boundary conditions will be checked.
/// @param v (vdl_vec*). A vector.
/// @param i (int). The first index to be set.
/// @param object (void*). An array of objects.
/// @param num_object (int). Number of objects.
#define vdl_Set(...) vdl_bt_CallerNoReturn(vdl_Set_BT, __VA_ARGS__)
static inline void vdl_Set_BT(vdl_bt bt, vdl_vec *const v, const int i, void *const object, const int num_object)
{
    vdl_bt_Push(bt);
    vdl_HealthCheck(v);
    vdl_assert_NullPointer(object);
    vdl_assert_ZeroObjects(num_object);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_assert_IndexOutOfBound(v, i + num_object - 1);

    // Optimize for the single object case
    if (num_object == 1)
        switch (vdl_Type(v))
        {
            case VDL_TYPE_CHAR:
                vdl_internal_GetChar(v, i) = vdl_internal_char_Array0(object);
                return;
            case VDL_TYPE_INT:
                vdl_internal_GetInt(v, i) = vdl_internal_int_Array0(object);
                return;
            case VDL_TYPE_DOUBLE:
                vdl_internal_GetDouble(v, i) = vdl_internal_double_Array0(object);
                return;
            case VDL_TYPE_VP:
                vdl_internal_GetVP(v, i) = vdl_internal_vp_Array0(object);
                return;
        }

    // Copy in the memory
    memmove(vdl_internal_Get(v, i), object, (size_t) num_object * vdl_SizeOfType(vdl_Type(v)));
VDL_EXCEPTION:
    return;
}

#endif//VDL_VDLWRAPPER_H
