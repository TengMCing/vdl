//
// Created by Patrick Li on 30/9/22.
//

#ifndef VDL_VDLWRAPPER_H
#define VDL_VDLWRAPPER_H

#include "vdlassert.h"

/*-----------------------------------------------------------------------------
 |  Cast void pointer to different types
 ----------------------------------------------------------------------------*/

#define vdlint_char_array(voidp) ((char *) (voidp))
#define vdlint_int_array(voidp) ((int *) (voidp))
#define vdlint_double_array(voidp) ((double *) (voidp))
#define vdlint_vp_array(voidp) ((vdl_vp *) (voidp))

#define vdlint_char_array0(voidp) (vdlint_char_array(voidp)[0])
#define vdlint_int_array0(voidp) (vdlint_int_array(voidp)[0])
#define vdlint_double_array0(voidp) (vdlint_double_array(voidp)[0])
#define vdlint_vp_array0(voidp) (vdlint_vp_array(voidp)[0])

/*-----------------------------------------------------------------------------
 |  Type of a vector
 ----------------------------------------------------------------------------*/

/// @description Type of a vector.
#define vdl_type(v) ((v)->type)
/// @description String representation of the type of a vector.
#define vdl_typestr(v) (VDL_TYPE_STR[vdl_type(v)])

/*-----------------------------------------------------------------------------
 |  Storage mode of a vector
 ----------------------------------------------------------------------------*/

/// @description Mode of a vector.
#define vdl_mode(v) ((v)->mode)
/// @description String representation of the mode of a vector.
#define vdl_modestr(v) (VDL_MODE_STR[vdl_mode(v)])

/*-----------------------------------------------------------------------------
 |  Capacity of a vector
 ----------------------------------------------------------------------------*/

#define vdlint_cap(v) ((v)->cap)
/// @description Get the capacity of a vector.
#define vdl_getcap(v) ((int){vdlint_cap(v)})

/*-----------------------------------------------------------------------------
 |  Length of a vector
 ----------------------------------------------------------------------------*/

#define vdlint_len(v) ((v)->len)
/// @description Get the length of a vector.
#define vdl_getlen(v) ((int){vdlint_len(v)})

/*-----------------------------------------------------------------------------
 |  Data of the vector
 ----------------------------------------------------------------------------*/

#define vdlint_dat(v) ((v)->dat)

/*-----------------------------------------------------------------------------
 |  Size of type or vector
 ----------------------------------------------------------------------------*/

/// @description Get the size of a type.
/// @param type (VDL_TYPE). Vector type.
/// @return (size_t) The size of the type.
#define vdl_sizeof_type(type) (VDL_TYPE_SIZE[type])
static const size_t VDL_TYPE_SIZE[4] = {
        [VDL_TYPE_CHAR]   = sizeof(char),
        [VDL_TYPE_INT]    = sizeof(int),
        [VDL_TYPE_DOUBLE] = sizeof(double),
        [VDL_TYPE_VP]     = sizeof(vdl_vp)};

/// @description Get the size of the data of a vector.
/// @param v (vdl_vec*). A vector.
/// @return (size_t) The size of the data of a vector.
#define vdl_sizeof_dat(v) ((size_t) vdlint_cap(v) * vdl_sizeof_type(vdl_type(v)))

/// @description Get the size of a vector.
/// @param v (vdl_vec*). A vector.
/// @return (size_t) The size of a vector.
#define vdl_sizeof_v(v) ((size_t) vdlint_cap(v) * vdl_sizeof_type(vdl_type(v)) + sizeof(vdl_vec))

/*-----------------------------------------------------------------------------
 |  Accessing the vector data
 ----------------------------------------------------------------------------*/

/// @description Get the address of the ith item from the data of a vector. No checks will be performed.
#define vdlint_get(v, i) (vdlint_char_array(vdlint_dat(v)) + vdl_sizeof_type(vdl_type(v)) * (size_t) (i))
/// @description Get the ith item from the data of a vector as char. No checks will be performed.
#define vdlint_getchar(v, i) (vdlint_char_array(vdlint_dat(v))[i])
/// @description Get the ith item from the data of a vector as int. No checks will be performed.
#define vdlint_getint(v, i) (vdlint_int_array(vdlint_dat(v))[i])
/// @description Get the ith item from the data of a vector as double. No checks will be performed.
#define vdlint_getdouble(v, i) (vdlint_double_array(vdlint_dat(v))[i])
/// @description Get the ith item from the data of a vector as vector pointer. No checks will be performed.
#define vdlint_getvp(v, i) (vdlint_vp_array(vdlint_dat(v))[i])


/// @description Get an item from a vector. Boundary conditions will be checked.
/// @details Since vector is dynamically typed, the return type can not be assumed at
/// compile-time. Void pointer will be returned. If any check fails, NULL will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (void*) Pointer to the item.
#define vdl_get(...) vdl_bt_call(vdl_get_BT, void *, __VA_ARGS__)
static inline void *vdl_get_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_bt_push(bt);
    vdl_assert_vec_health(v, 1, 1, 1);
    vdl_assert_idx_out_of_bound(v, i);
    return vdlint_get(v, i);
VDL_EXCEPTION:
    return NULL;
}

/// @description Get the ith item of a vector and interpret it as a char.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (char) A character.
#define vdl_getchar(...) vdl_bt_call(vdl_getchar_BT, char, __VA_ARGS__)
static inline char vdl_getchar_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_bt_push(bt);
    vdl_assert_vec_health(v, 1, 0, 1);
    vdl_assert_idx_out_of_bound(v, i);
    vdl_assert_incompatible_type(VDL_TYPE_CHAR, vdl_type(v));
    return vdlint_getchar(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_CHAR_NA;
}

/// @description Get the ith item of a vector and interpret it as an int.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (int) An integer.
#define vdl_getint(...) vdl_bt_call(vdl_getint_BT, int, __VA_ARGS__)
static inline int vdl_getint_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_bt_push(bt);
    vdl_assert_vec_health(v, 1, 0, 1);
    vdl_assert_idx_out_of_bound(v, i);
    vdl_assert_incompatible_type(VDL_TYPE_INT, vdl_type(v));
    return vdlint_getint(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_INT_NA;
}

/// @description Get the ith item of a vector and interpret it as a double.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (double) A double.
#define vdl_getdouble(...) vdl_bt_call(vdl_getdouble_BT, double, __VA_ARGS__)
static inline double vdl_getdouble_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_bt_push(bt);
    vdl_assert_vec_health(v, 1, 0, 1);
    vdl_assert_idx_out_of_bound(v, i);
    vdl_assert_incompatible_type(VDL_TYPE_DOUBLE, vdl_type(v));
    return vdlint_getdouble(v, i);
VDL_EXCEPTION:
    return VDL_TYPE_DOUBLE_NA;
}

/// @description Get the ith item of a vector and interpret it as a vector consists of vectors of any type.
/// @details If any check fails, a missing value will be returned.
/// @param v (const vdl_vec *const). A vector.
/// @param i (const int). Index of the item.
/// @return (vdl_vp) A vector.
#define vdl_getvp(...) vdl_bt_call(vdl_getvp_BT, vdl_vp, __VA_ARGS__)
static inline vdl_vp vdl_getvp_BT(vdl_bt bt, const vdl_vec *const v, const int i)
{
    vdl_bt_push(bt);
    vdl_assert_vec_health(v, 1, 0, 1);
    vdl_assert_idx_out_of_bound(v, i);
    vdl_assert_incompatible_type(VDL_TYPE_VP, vdl_type(v));
    return vdlint_getvp(v, i);
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
#define vdl_set(...) vdl_bt_callvoid(vdl_set_BT, __VA_ARGS__)
static inline void vdl_set_BT(vdl_bt bt, vdl_vec *const v, const int i, void *const object, const int num_object)
{
    vdl_bt_push(bt);
    if (num_object <= 0)
        return;
    vdl_assert_vec_health(v, 1, 1, 1);
    vdl_assert_null_pointer(object);
    vdl_assert_idx_out_of_bound(v, i);
    vdl_assert_idx_out_of_bound(v, i + num_object - 1);

    // Optimize for the single object case
    if (num_object == 1)
        switch (vdl_type(v))
        {
            case VDL_TYPE_CHAR:
                vdlint_getchar(v, i) = vdlint_char_array0(object);
                return;
            case VDL_TYPE_INT:
                vdlint_getint(v, i) = vdlint_int_array0(object);
                return;
            case VDL_TYPE_DOUBLE:
                vdlint_getdouble(v, i) = vdlint_double_array0(object);
                return;
            case VDL_TYPE_VP:
                vdlint_getvp(v, i) = vdlint_vp_array0(object);
                return;
        }

    // Copy in the memory
    memmove(vdlint_get(v, i), object, (size_t) num_object * vdl_sizeof_type(vdl_type(v)));
VDL_EXCEPTION:
    return;
}

/// @description Set a series of items for a vector using another vector. Boundary conditions will be checked.
/// @details If any check fails, no content will be written.
/// @param v1 (vdl_vec *const). A vector.
/// @param i (int). The first index to be set.
/// @param v2 (vdl_vec *const). An array of objects.
/// @param num_object (int). Number of objects.
#define vdl_vset(...) vdl_bt_callvoid(vdl_vset_BT, __VA_ARGS__)
static inline void vdl_vset_BT(vdl_bt bt, vdl_vec *const v1, const int i, vdl_vec *const v2)
{
    vdl_bt_push(bt);
    // Only need to ensure we can use the metadata of v1 and v2,
    // and v1 and v2 are of the same type
    vdl_assert_null_pointer(v1);
    vdl_assert_null_pointer(v2);
    vdl_assert_incompatible_type(vdl_type(v1), vdl_type(v2));
    vdl_set(v1, i, vdlint_dat(v2), vdlint_len(v2));
VDL_EXCEPTION:
    return;
}

#endif//VDL_VDLWRAPPER_H
