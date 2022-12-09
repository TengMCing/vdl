//
// Created by Patrick Li on 30/9/22.
//

#ifndef VDL_VDLWRAPPER_H
#define VDL_VDLWRAPPER_H

#include "vdlassert.h"

/*-----------------------------------------------------------------------------
 |  Cast void pointer to different types
 ----------------------------------------------------------------------------*/

/// @description Cast a void pointer to a char pointer
/// @param voidp (void *). A void pointer.
#define vdlint_CharArray(voidp) ((char *) (voidp))

/// @description Cast a void pointer to an int pointer
/// @param voidp (void *). A void pointer.
#define vdlint_IntArray(voidp) ((int *) (voidp))

/// @description Cast a void pointer to a double pointer
/// @param voidp (void *). A void pointer.
#define vdlint_DoubleArray(voidp) ((double *) (voidp))

/// @description Cast a void pointer to a vdl_vp pointer
/// @param voidp (void *). A void pointer.
#define vdlint_VpArray(voidp) ((vdl_vp *) (voidp))

/// @description Cast a void pointer to a char pointer and get the first element.
/// @param voidp (void *). A void pointer.
#define vdlint_CharArray0(voidp) (vdlint_CharArray(voidp)[0])

/// @description Cast a void pointer to an int pointer and get the first element.
/// @param voidp (void *). A void pointer.
#define vdlint_IntArray0(voidp) (vdlint_IntArray(voidp)[0])

/// @description Cast a void pointer to a double pointer and get the first element.
/// @param voidp (void *). A void pointer.
#define vdlint_DoubleArray0(voidp) (vdlint_DoubleArray(voidp)[0])

/// @description Cast a void pointer to a vdl_vp pointer and get the first element.
/// @param voidp (void *). A void pointer.
#define vdlint_VpArray0(voidp) (vdlint_VpArray(voidp)[0])

/*-----------------------------------------------------------------------------
 |  Type of a vector
 ----------------------------------------------------------------------------*/

/// @description Type of a vector.
/// @param v (vdl_vp). A vector.
#define vdl_GetType(v) ((v)->type)

/// @description String representation of the type of a vector.
/// @param v (vdl_vp). A vector.
#define vdl_GetTypeStr(v) (VDL_TYPE_STR[vdl_GetType(v)])

/*-----------------------------------------------------------------------------
 |  Storage mode of a vector
 ----------------------------------------------------------------------------*/

/// @description Mode of a vector.
/// @param v (vdl_vp). A vector.
#define vdl_GetMode(v) ((v)->mode)

/// @description String representation of the mode of a vector.
/// @param v (vdl_vp). A vector.
#define vdl_GetModeStr(v) (VDL_MODE_STR[vdl_GetMode(v)])

/*-----------------------------------------------------------------------------
 |  Capacity of a vector
 ----------------------------------------------------------------------------*/

/// @description Capacity of a vector.
/// @param v (vdl_vp). A vector.
/// @param (int) The cap.
#define vdlint_Cap(v) ((v)->cap)

/// @description Capacity of a vector.
/// @param v (vdl_vp). A vector.
/// @param (int) A copy of the cap.
#define vdl_GetCap(v) ((int){vdlint_Cap(v)})

/*-----------------------------------------------------------------------------
 |  Length of a vector
 ----------------------------------------------------------------------------*/

/// @description Length of a vector.
/// @param v (vdl_vp). A vector.
#define vdlint_Len(v) ((v)->len)

/// @description Length of a vector.
/// @param v (vdl_vp). A vector.
/// @param (int) A copy of the len.
#define vdl_GetLen(v) ((int){vdlint_Len(v)})

/*-----------------------------------------------------------------------------
 |  Data of the vector
 ----------------------------------------------------------------------------*/

/// @description Data pointer of a vector.
/// @param v (vdl_vp). A vector.
#define vdlint_Dat(v) ((v)->dat)

/*-----------------------------------------------------------------------------
 |  Size of type or vector
 ----------------------------------------------------------------------------*/

/// @description Get the size of a type.
/// @param type (VDL_TYPE). Vector type.
#define vdl_SizeOfType(type) (VDL_TYPE_SIZE[type])
static const size_t VDL_TYPE_SIZE[4] = {
        [VDL_TYPE_CHAR]   = sizeof(char),
        [VDL_TYPE_INT]    = sizeof(int),
        [VDL_TYPE_DOUBLE] = sizeof(double),
        [VDL_TYPE_VP]     = sizeof(vdl_vp)};

/// @description Get the size of the data of a vector.
/// @param v (vdl_vec*). A vector.
#define vdl_SizeOfDat(v) ((size_t) vdlint_Cap(v) * vdl_SizeOfType(vdl_GetType(v)))

/// @description Get the size of a vector.
/// @param v (vdl_vec*). A vector.
#define vdl_SizeOfVec(v) ((size_t) vdlint_Cap(v) * vdl_SizeOfType(vdl_GetType(v)) + sizeof(vdl_vec))

/*-----------------------------------------------------------------------------
 |  Accessing the vector data
 ----------------------------------------------------------------------------*/

/// @description Get the address of the ith item from the data of a vector. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
#define vdlint_Address(v, i) (vdlint_CharArray(vdlint_Dat(v)) + vdl_SizeOfType(vdl_GetType(v)) * (size_t) (i))

/// @description Get the ith item from the data of a vector as char. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
#define vdlint_CharAt(v, i) (vdlint_CharArray(vdlint_Dat(v))[i])

/// @description Get the ith item from the data of a vector as int. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
#define vdlint_IntAt(v, i) (vdlint_IntArray(vdlint_Dat(v))[i])

/// @description Get the ith item from the data of a vector as double. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
#define vdlint_DoubleAt(v, i) (vdlint_DoubleArray(vdlint_Dat(v))[i])

/// @description Get the ith item from the data of a vector as vector pointer. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
#define vdlint_VpAt(v, i) (vdlint_VpArray(vdlint_Dat(v))[i])


/// @description Get the address of an item from a vector. Boundary conditions will be checked.
/// @details If any check fails, NULL will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (void*) Pointer to the item.
#define vdl_Address(...) vdlint_Call(vdl_Address_BT, void *, __VA_ARGS__)
static inline void *vdl_Address_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckVecHealth(v, 1, 1, 1);
    vdlint_CheckIndexOutOfBound(v, i);
    return vdlint_Address(v, i);
VDL_EXCEPTION:
    return NULL;
}

/// @description Get the ith item of a vector and interpret it as a char.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (char) A character.
#define vdl_GetChar(...) vdlint_Call(vdl_GetChar_BT, char, __VA_ARGS__)
static inline char vdl_GetChar_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckVecHealth(v, 1, 0, 1);
    vdlint_CheckIndexOutOfBound(v, i);
    vdlint_CheckIncompatibleType(VDL_TYPE_CHAR, vdl_GetType(v));
    return vdlint_CharAt(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_CHAR_NA;
}

/// @description Get the ith item of a vector and interpret it as an int.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (int) An integer.
#define vdl_GetInt(...) vdlint_Call(vdl_GetInt_BT, int, __VA_ARGS__)
static inline int vdl_GetInt_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckVecHealth(v, 1, 0, 1);
    vdlint_CheckIndexOutOfBound(v, i);
    vdlint_CheckIncompatibleType(VDL_TYPE_INT, vdl_GetType(v));
    return vdlint_IntAt(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_INT_NA;
}

/// @description Get the ith item of a vector and interpret it as a double.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (double) A double.
#define vdl_GetDouble(...) vdlint_Call(vdl_GetDouble_BT, double, __VA_ARGS__)
static inline double vdl_GetDouble_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckVecHealth(v, 1, 0, 1);
    vdlint_CheckIndexOutOfBound(v, i);
    vdlint_CheckIncompatibleType(VDL_TYPE_DOUBLE, vdl_GetType(v));
    return vdlint_DoubleAt(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_DOUBLE_NA;
}

/// @description Get the ith item of a vector and interpret it as a vector consists of vectors of any type.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (vdl_vp) A vector.
#define vdl_GetVp(...) vdlint_Call(vdl_GetVp_BT, vdl_vp, __VA_ARGS__)
static inline vdl_vp vdl_GetVp_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckVecHealth(v, 1, 0, 1);
    vdlint_CheckIndexOutOfBound(v, i);
    vdlint_CheckIncompatibleType(VDL_TYPE_VP, vdl_GetType(v));
    return vdlint_VpAt(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_VP_NA;
}

/*-----------------------------------------------------------------------------
 |  Setting the vector data
 ----------------------------------------------------------------------------*/

/// @description Set a series of items for a vector. Boundary conditions will be checked.
/// @details The user has to ensure the object is an array of the same type as the vector.
/// The function has no way to check for it. If any check fails, no content will be written.
/// @param v (vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @param object (void*). An array of objects.
/// @param num_object (int). Number of objects.
#define vdl_SetByArray(...) vdlint_CallVoid(vdl_SetByArray_BT, __VA_ARGS__)
static inline void vdl_SetByArray_BT(vdl_bt bt, vdl_vec *const v, const int i, void *const object, const int num_object)
{
    vdl_PushBacktrace(bt);
    if (num_object <= 0)
        return;
    vdlint_CheckVecHealth(v, 1, 1, 1);
    vdlint_CheckNullPointer(object);
    vdlint_CheckIndexOutOfBound(v, i);
    vdlint_CheckIndexOutOfBound(v, i + num_object - 1);

    // Optimize for the single object case
    if (num_object == 1)
        switch (vdl_GetType(v))
        {
            case VDL_TYPE_CHAR:
                vdlint_CharAt(v, i) = vdlint_CharArray0(object);
                return;
            case VDL_TYPE_INT:
                vdlint_IntAt(v, i) = vdlint_IntArray0(object);
                return;
            case VDL_TYPE_DOUBLE:
                vdlint_DoubleAt(v, i) = vdlint_DoubleArray0(object);
                return;
            case VDL_TYPE_VP:
                vdlint_VpAt(v, i) = vdlint_VpArray0(object);
                return;
        }

    // Copy in the memory
    memmove(vdlint_Address(v, i), object, (size_t) num_object * vdl_SizeOfType(vdl_GetType(v)));
VDL_EXCEPTION:
    return;
}

/// @description Set a series of items for a vector using another vector. Boundary conditions will be checked.
/// @details If any check fails, no content will be written.
/// @param v1 (vdl_vec *const). A vector.
/// @param i (int). The first index to be set.
/// @param v2 (vdl_vec *const). An array of objects.
/// @param num_object (int). Number of objects.
#define vdl_Set(...) vdlint_CallVoid(vdl_Set_BT, __VA_ARGS__)
static inline void vdl_Set_BT(vdl_bt bt, vdl_vec *const v1, const int i, vdl_vec *const v2)
{
    vdl_PushBacktrace(bt);
    // Only need to ensure we can use the metadata of v1 and v2,
    // and v1 and v2 are of the same type
    vdlint_CheckNullPointer(v1);
    vdlint_CheckNullPointer(v2);
    vdlint_CheckIncompatibleType(vdl_GetType(v1), vdl_GetType(v2));
    vdl_SetByArray(v1, i, vdlint_Dat(v2), vdlint_Len(v2));
VDL_EXCEPTION:
    return;
}

#endif//VDL_VDLWRAPPER_H
