//
// Created by Patrick Li on 30/9/22.
//

#ifndef VDL_VDLWRAPPER_H
#define VDL_VDLWRAPPER_H

#include "vdlassert.h"
#include "vdldef.h"
#include <string.h>

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

/// @description Capacity of a vector.
#define vdl_Capacity(v) ((v)->capacity)

/*-----------------------------------------------------------------------------
 |  Length of a vector
 ----------------------------------------------------------------------------*/

/// @description Length of a vector.
#define vdl_Length(v) ((v)->length)

/*-----------------------------------------------------------------------------
 |  Data of the vector
 ----------------------------------------------------------------------------*/

/// @description Data of a vector.
#define vdl_Data(v) ((v)->data)

/*-----------------------------------------------------------------------------
 |  Size of type or vector
 ----------------------------------------------------------------------------*/

/// @description Get the size of a type.
/// @param type (VDL_TYPE). Vector type.
/// @return (size_t) The size of the type.
#define vdl_SizeOfType(type) (VDL_TYPE_SIZE[type])
static size_t VDL_TYPE_SIZE[4] = {
        [VDL_TYPE_CHAR]   = sizeof(char),
        [VDL_TYPE_INT]    = sizeof(int),
        [VDL_TYPE_DOUBLE] = sizeof(double),
        [VDL_TYPE_VP]     = sizeof(vdl_vp)};

/// @description Get the size of the data of a vector.
/// @param v (vdl_vec*). A vector.
/// @return (size_t) The size of the data of a vector.
#define vdl_SizeOfData(v) ((size_t) vdl_Capacity(v) * vdl_SizeOfType(vdl_Type(v)))

/// @description Get the size of a vector.
/// @param v (vdl_vec*). A vector.
/// @return (size_t) The size of a vector.
#define vdl_SizeOfVec(v) ((size_t) vdl_Capacity(v) * vdl_SizeOfType(vdl_Type(v)) + sizeof(vdl_vec))

/*-----------------------------------------------------------------------------
 |  Accessing the vector data
 ----------------------------------------------------------------------------*/

/// @description Get the address of the ith item from the data of a vector. No boundary conditions will be checked.
#define vdl_AddressOf(v, i) (vdl_char_Array(vdl_Data(v)) + vdl_SizeOfType(vdl_Type(v)) * (size_t) (i))

/// @description Get an item from a vector. Boundary conditions will be checked.
/// @details Since vector is dynamically typed, the return type can not be assumed at
/// compile-time. Void pointer will be returned.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (void*) Pointer to the item.
#define vdl_Get(...) vdl_bt_Call(vdl_Get_BT, __VA_ARGS__)
static inline void *vdl_Get_BT(vdl_bt bt, const vdl_vec *v, const int i)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_Return(vdl_AddressOf(v, i));
}

/// @description Get the ith item of a vector and interpret it as a char.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (char) A character.
#define vdl_GetChar(v, i) (vdl_char_Array(vdl_Get(v, i))[0])

/// @description Get the ith item of a vector and interpret it as an int.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (int) An integer.
#define vdl_GetInt(v, i) (vdl_int_Array(vdl_Get(v, i))[0])

/// @description Get the ith item of a vector and interpret it as a double.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (double) A double.
#define vdl_GetDouble(v, i) (vdl_double_Array(vdl_Get(v, i))[0])

/// @description Get the ith item of a vector and interpret it as a vector consists of vectors of any type.
/// @param v (vdl_vec*). A vector.
/// @param i (int). Index of the item.
/// @return (vdl_vp) A vector.
#define vdl_GetVp(v, i) (vdl_vp_Array(vdl_Get(v, i))[0])

/*-----------------------------------------------------------------------------
 |  Saving to the vector data
 ----------------------------------------------------------------------------*/

/// @description Set a series of items for a vector. Boundary conditions will be checked.
/// @param v (vdl_vec*). A vector.
/// @param i (int). The first index to be set.
/// @param object (void*). An array of objects.
/// @param num_object (int). Number of objects.
#define vdl_Set(...) vdl_bt_Call(vdl_Set_BT, __VA_ARGS__)
static inline void vdl_Set_BT(vdl_bt bt, vdl_vec *const v, const int i, void *const object, const int num_object)
{
    vdl_PushBT(bt);
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
            vdl_char_Array(vdl_Data(v))[i] = vdl_char_Array(object)[0];
            vdl_Return();
        case VDL_TYPE_INT:
            vdl_int_Array(vdl_Data(v))[i] = vdl_int_Array(object)[0];
            vdl_Return();
        case VDL_TYPE_DOUBLE:
            vdl_double_Array(vdl_Data(v))[i] = vdl_double_Array(object)[0];
            vdl_Return();
        case VDL_TYPE_VP:
            vdl_vp_Array(vdl_Data(v))[i] = vdl_vp_Array(object)[0];
            vdl_Return();
        }

    // Copy in the memory
    memmove(vdl_AddressOf(v, i), object, (size_t) num_object * vdl_SizeOfType(vdl_Type(v)));
    vdl_Return();
}

#endif//VDL_VDLWRAPPER_H
