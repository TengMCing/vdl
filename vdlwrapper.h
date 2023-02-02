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
#define vdlint_AsCharArray(voidp) ((char *) (voidp))

/// @description Cast a void pointer to an int pointer
/// @param voidp (void *). A void pointer.
#define vdlint_AsIntArray(voidp) ((int *) (voidp))

/// @description Cast a void pointer to a double pointer
/// @param voidp (void *). A void pointer.
#define vdlint_AsDoubleArray(voidp) ((double *) (voidp))

/// @description Cast a void pointer to a vdl_vp pointer
/// @param voidp (void *). A void pointer.
#define vdlint_AsVpArray(voidp) ((vdl_vp *) (voidp))

/// @description Cast a void pointer to a char pointer and get the first element.
/// @param voidp (void *). A void pointer.
#define vdlint_CharArray0(voidp) (vdlint_AsCharArray(voidp)[0])

/// @description Cast a void pointer to an int pointer and get the first element.
/// @param voidp (void *). A void pointer.
#define vdlint_IntArray0(voidp) (vdlint_AsIntArray(voidp)[0])

/// @description Cast a void pointer to a double pointer and get the first element.
/// @param voidp (void *). A void pointer.
#define vdlint_DoubleArray0(voidp) (vdlint_AsDoubleArray(voidp)[0])

/// @description Cast a void pointer to a vdl_vp pointer and get the first element.
/// @param voidp (void *). A void pointer.
#define vdlint_VpArray0(voidp) (vdlint_AsVpArray(voidp)[0])

/*-----------------------------------------------------------------------------
 |  Type of a vector
 ----------------------------------------------------------------------------*/

/// @description Type of a vector.
/// @param v (vdl_vp). A vector.
/// @return (VDL_TYPE) A vector type.
#define vdl_GetType(v) ((v)->type)

/// @description String representation of the type of a vector.
/// @param v (vdl_vp). A vector.
/// @return (const char *) A string.
#define vdl_GetTypeStr(v) (VDL_TYPE_STR[(v)->type])

/*-----------------------------------------------------------------------------
 |  Storage mode of a vector
 ----------------------------------------------------------------------------*/

/// @description Mode of a vector.
/// @param v (vdl_vp). A vector.
/// @return (VDL_MODE) A vector mode.
#define vdl_GetMode(v) ((v)->mode)

/// @description String representation of the mode of a vector.
/// @param v (vdl_vp). A vector.
/// @return (const char *) A string.
#define vdl_GetModeStr(v) (VDL_MODE_STR[(v)->mode])

/*-----------------------------------------------------------------------------
 |  Capacity of a vector
 ----------------------------------------------------------------------------*/

/// @description Capacity of a vector.
/// @param v (vdl_vp). A vector.
/// @param (int) The capacity.
#define vdl_GetCap(v) ((int){(v)->cap})

/*-----------------------------------------------------------------------------
 |  Length of a vector
 ----------------------------------------------------------------------------*/

/// @description Length of a vector.
/// @param v (vdl_vp). A vector.
/// @param (int) The length.
#define vdl_GetLen(v) ((int){(v)->len})

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
#define vdl_SizeOfDat(v) ((size_t) (v)->cap * vdl_SizeOfType((v)->type))

/// @description Get the size of a vector.
/// @param v (vdl_vec*). A vector.
#define vdl_SizeOfVec(v) ((size_t) (v)->cap * vdl_SizeOfType((v)->type) + sizeof(vdl_vec))

/*-----------------------------------------------------------------------------
 |  Accessing the vector data
 ----------------------------------------------------------------------------*/

/// @description Get the pointer to the address of the ith item of a vector. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
/// @return (char *) A pointer to the address.
#define vdlint_AddressOf(v, i) (vdlint_AsCharArray((v)->dat) + vdl_SizeOfType((v)->type) * (size_t) (i))

/// @description Get the ith item a vector as char. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
/// @return (char) The item.
#define vdlint_CharAt(v, i) (vdlint_AsCharArray((v)->dat)[i])

/// @description Get the ith item of a vector as int. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
/// @return (int) The item.
#define vdlint_IntAt(v, i) (vdlint_AsIntArray((v)->dat)[i])

/// @description Get the ith item of a vector as double. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
/// @return (double) The item.
#define vdlint_DoubleAt(v, i) (vdlint_AsDoubleArray((v)->dat)[i])

/// @description Get the ith item of a vector as vdl_vp. No checks will be performed.
/// @param v (vdl_vec*). A vector.
/// @param i (int). An index.
/// @return (vdl_vp) The item.
#define vdlint_VpAt(v, i) (vdlint_AsVpArray((v)->dat)[i])


/// @description Get the address of an item from a vector. Boundary conditions will be checked.
/// @details If any check fails, NULL will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (void*) Pointer to the item.
#define vdl_e_AddressOf(...) vdlint_e_Call(vdl_e_AddressOf_BT, void *, __VA_ARGS__)
static inline void *vdl_e_AddressOf_BT(vdl_fr fr, const vdl_vec *const v, const int i)
{
    vdl_e_PushFrame(fr);
    vdlint_e_CheckVec(v, VDLINT_CHECK_NULL_VEC | VDLINT_CHECK_UNKNOWN_VEC_TYPE | VDLINT_CHECK_NULL_VEC_DAT);
    vdlint_e_CheckIndexOutOfBound(v, i);

    return vdlint_AddressOf(v, i);
VDL_EXCEPTION:
    return NULL;
}

/// @description Get the ith item of a vector and interpret it as a char.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (char) A character.
#define vdl_e_GetChar(...) vdlint_e_Call(vdl_e_GetChar_BT, char, __VA_ARGS__)

/// @description Get the ith item of a vector and interpret it as an int.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (int) An integer.
#define vdl_e_GetInt(...) vdlint_e_Call(vdl_e_GetInt_BT, int, __VA_ARGS__)

/// @description Get the ith item of a vector and interpret it as a double.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (double) A double.
#define vdl_e_GetDouble(...) vdlint_e_Call(vdl_e_GetDouble_BT, double, __VA_ARGS__)

/// @description Get the ith item of a vector and interpret it as a vector consists of vectors of any type.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (vdl_vp) A vector.
#define vdl_e_GetVp(...) vdlint_e_Call(vdl_e_GetVp_BT, vdl_vp, __VA_ARGS__)

// TEMPLATE BEGIN
#define vdlint_e_GetT_BT(T, CT, UT)                                                    \
    static inline T vdl_e_Get##CT##_BT(vdl_fr fr, const vdl_vec *const v, const int i) \
    {                                                                                  \
        vdl_e_PushFrame(fr);                                                           \
        vdlint_e_CheckVec(v, VDLINT_CHECK_NULL_VEC | VDLINT_CHECK_NULL_VEC_DAT);       \
        vdlint_e_CheckIndexOutOfBound(v, i);                                           \
        vdlint_e_CheckIncompatibleType(VDL_TYPE_##UT, v->type);                        \
                                                                                       \
        return vdlint_##CT##At(v, i);                                                  \
    VDL_EXCEPTION:                                                                     \
        return VDL_TYPE_##UT##_NA;                                                     \
    }

vdlint_e_GetT_BT(char, Char, CHAR);
vdlint_e_GetT_BT(int, Int, INT);
vdlint_e_GetT_BT(double, Double, DOUBLE);
vdlint_e_GetT_BT(vdl_vp, Vp, VP);
#undef vdlint_e_GetT_BT
// TEMPLATE END

/*-----------------------------------------------------------------------------
 |  Setting the vector data
 ----------------------------------------------------------------------------*/

/// @description Set a series of items for a vector. Boundary conditions will be checked.
/// @details The user has to ensure the array is of the same type as the vector.
/// The function has no way to check for it. If the number of objects less than 1, the
/// function will do nothing. If any check fails, no content will be written.
/// @param v (vdl_vec *const). A vector.
/// @param idx (const int *const). Indices of items.
/// @param array (void*). An array of objects.
/// @param num_object (int). Number of objects.
#define vdl_e_SetByArray(...) vdlint_e_CallVoid(vdl_e_SetByArray_BT, __VA_ARGS__)
static inline void vdl_e_SetByArray_BT(vdl_fr fr, vdl_vec *const v, const int *const idx, void *const array, const int num_object)
{
    vdl_e_PushFrame(fr);
    if (num_object <= 0)
        return;
    vdlint_e_CheckVec(v, VDLINT_CHECK_NULL_VEC | VDLINT_CHECK_UNKNOWN_VEC_TYPE | VDLINT_CHECK_NULL_VEC_DAT);
    vdlint_e_CheckNullPointer(idx);
    vdlint_e_CheckNullPointer(array);

    vdlint_for_i(num_object) vdlint_e_CheckIndexOutOfBound(v, idx[i]);

    switch (v->type)
    {
        case VDL_TYPE_CHAR:
            vdlint_for_i(num_object) vdlint_CharAt(v, idx[i]) = vdlint_AsCharArray(array)[i];
            return;
        case VDL_TYPE_INT:
            vdlint_for_i(num_object) vdlint_IntAt(v, idx[i]) = vdlint_AsIntArray(array)[i];
            return;
        case VDL_TYPE_DOUBLE:
            vdlint_for_i(num_object) vdlint_DoubleAt(v, idx[i]) = vdlint_AsDoubleArray(array)[i];
            return;
        case VDL_TYPE_VP:
            vdlint_for_i(num_object) vdlint_VpAt(v, idx[i]) = vdlint_AsVpArray(array)[i];
            return;
    }

VDL_EXCEPTION:
    return;
}

/// @description Set a series of items for a vector using another vector. Boundary conditions will be checked.
/// @details If any check fails, no content will be written.
/// @param v1 (vdl_vec *const). A vector.
/// @param idx (vdl_vec *const). A vector of indices.
/// @param v2 (vdl_vec *const). Another vector.
#define vdl_e_Set(...) vdlint_e_CallVoid(vdl_e_Set_BT, __VA_ARGS__)
static inline void vdl_e_Set_BT(vdl_fr fr, vdl_vec *const v1, vdl_vec *const idx, vdl_vec *const v2)
{
    vdl_e_PushFrame(fr);

    vdlint_e_CheckVec(v1, VDLINT_CHECK_NULL_VEC | VDLINT_CHECK_NULL_VEC_DAT);
    vdlint_e_CheckVec(idx, VDLINT_CHECK_NULL_VEC | VDLINT_CHECK_NULL_VEC_DAT);
    vdlint_e_CheckVec(v2, VDLINT_CHECK_NULL_VEC | VDLINT_CHECK_NULL_VEC_DAT);

    // v1 should have the same type as v2
    vdlint_e_CheckIncompatibleType(v1->type, v2->type);

    // idx should be an int vector
    vdlint_e_CheckIncompatibleType(idx->type, VDL_TYPE_INT);

    // idx should have the same length as v2 (or v2->len == 1)
    vdlint_e_CheckIncompatibleLen(idx->len, v2->len);

    vdl_e_SetByArray(v1, idx->dat, v2->dat, idx->len);

VDL_EXCEPTION:
    return;
}

#endif//VDL_VDLWRAPPER_H
