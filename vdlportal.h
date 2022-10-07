//
// Created by Patrick Li on 3/10/22.
//

#ifndef VDL_VDLPORTAL_H
#define VDL_VDLPORTAL_H

#include "vdlmem.h"

/*-----------------------------------------------------------------------------
 |  Copy vector
 ----------------------------------------------------------------------------*/

/// @description Make a shallow copy of a vector.
/// @param v (vdl_vec*). A vector.
/// @return (vdl_vec*) A copied vector.
#define vdl_Copy(...) vdl_bt_Call(vdl_Copy_BT, __VA_ARGS__)
static inline vdl_vp vdl_Copy_BT(vdl_bt bt, const vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);

    // A new vector will be allocated
    vdl_vp copy      = vdl_New(vdl_Type(v), vdl_Capacity(v));
    vdl_Length(copy) = vdl_Length(v);

    // Only the first layer of the content will be copied
    vdl_Set(copy, 0, vdl_Data(v), vdl_Length(v));
    vdl_ReturnConst(copy);
}

/*-----------------------------------------------------------------------------
 |  Append items to the end of a vector
 ----------------------------------------------------------------------------*/

/// @description Append items to the end of the vector.
/// @param v (vdl_vec*). A vector.
/// @param object (void*). An array of objects.
/// @param num_object (int). Number of objects.
#define vdl_IAppend(...) vdl_bt_Call(vdl_IAppend_BT, __VA_ARGS__)
static inline void vdl_IAppend_BT(vdl_bt bt, vdl_vec *const v, void *const object, const int num_object)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_assert_NullPointer(object);
    vdl_assert_ZeroObjects(num_object);

    vdl_Reserve(v, vdl_Length(v) + num_object);
    vdl_Length(v) += num_object;
    vdl_Set(v, vdl_Length(v) - num_object, object, num_object);
    vdl_ReturnConst();
}

/// @description Copy the vector, and then append items to the end of the copied vector.
/// @param v (vdl_vec*). A vector.
/// @param object (void*). An array of objects.
/// @param num_object (int). Number of objects.
#define vdl_Append(v, object, num_object) vdl_IAppend(vdl_Copy(v), object, num_object)


/*-----------------------------------------------------------------------------
 |  Fina an object from a vector
 ----------------------------------------------------------------------------*/

/// @description Find an object from a vector.
/// @details Comparison is made by using the double equal sign. Thus, floating
/// point comparison could be unsafe.
/// @param v : (vdl_vec*) A vector.
/// @param object : (void*) An object.
/// @return (int) The first found index, -1 for not found.
#define vdl_Find(...) vdl_bt_Call(vdl_Find_BT, __VA_ARGS__)
static inline int vdl_Find_BT(vdl_bt bt, const vdl_vec *const v, void *const object)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_assert_NullPointer(object);

    switch (vdl_Type(v))
    {
        case VDL_TYPE_CHAR:
        {
            const char obj_val = vdl_char_Array(object)[0];
            vdl_For_i(vdl_Length(v)) if (vdl_char_Array(vdl_Data(v))[i] == obj_val) vdl_ReturnConst(i);
            vdl_ReturnConst(-1);
        }
        case VDL_TYPE_INT:
        {
            const int obj_val = vdl_int_Array(object)[0];
            vdl_For_i(vdl_Length(v)) if (vdl_int_Array(vdl_Data(v))[i] == obj_val) vdl_ReturnConst(i);
            vdl_ReturnConst(-1);
        }
        case VDL_TYPE_DOUBLE:
        {
            const double obj_val = vdl_double_Array(object)[0];
            vdl_For_i(vdl_Length(v)) if (vdl_double_Array(vdl_Data(v))[i] == obj_val) vdl_ReturnConst(i);
            vdl_ReturnConst(-1);
        }
        case VDL_TYPE_VP:
        {
            vdl_vec *const obj_val = vdl_vp_Array(object)[0];
            vdl_For_i(vdl_Length(v)) if (vdl_vp_Array(vdl_Data(v))[i] == obj_val) vdl_ReturnConst(i);
            vdl_ReturnConst(-1);
        }
    }
}

/*-----------------------------------------------------------------------------
 |  Remove objects from a vector
 ----------------------------------------------------------------------------*/

/// @description Remove objects from a vector.
/// @param v : (vdl_vec*) A vector.
/// @param i : (int) The first index.
/// @param num_object : (int) Number of objects to be removed.
#define vdl_IRemove(...) vdl_bt_Call(vdl_IRemove_BT, __VA_ARGS__)
static inline void vdl_IRemove_BT(vdl_bt bt, vdl_vec *const v, const int i, const int num_object)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_assert_IndexOutOfBound(v, i + num_object - 1);
    vdl_assert_ZeroObjects(num_object);

    // Only reduce the length if all the items are at the end of the array
    if (i == vdl_Length(v) - num_object)
    {
        vdl_Length(v) -= num_object;
        vdl_ReturnConst();
    }

    // Copy memory to index i
    memmove(vdl_AddressOf(v, i),
            vdl_AddressOf(v, i + num_object),
            (size_t) (vdl_Length(v) - i - num_object) * vdl_SizeOfType(vdl_Type(v)));
    vdl_Length(v) -= num_object;
    vdl_ReturnConst();
}

/// @description Copy the vector, and then remove objects from the copied vector.
/// @param v : (vdl_vec*) A vector.
/// @param i : (int) The first index.
/// @param num_object : (int) Number of objects to be removed.
#define vdl_Remove(...) vdl_bt_Call(vdl_Remove_BT, __VA_ARGS__)
static inline vdl_vp vdl_Remove_BT(vdl_bt bt, vdl_vec *const v, const int i, const int num_object)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_assert_IndexOutOfBound(v, i + num_object - 1);
    vdl_assert_ZeroObjects(num_object);

    vdl_vp result = vdl_New(vdl_Type(v), vdl_Length(v) - num_object);
    vdl_For_j(vdl_Length(v))
    {
        if (j < i || j >= i + num_object)
            vdl_IAppend(result, vdl_Get(v, j), 1);
    }

    vdl_ReturnConst(result);
}

/*-----------------------------------------------------------------------------
 |  Pop an object from the vector
 ----------------------------------------------------------------------------*/

/// @description Remove an object from the copied vector.
/// @param v : (vdl_vec*) A vector.
/// @param i : (int) The index.
#define vdl_IPop(v, i) vdl_IRemove(v, i, 1)

/// @description Copy the vector, and then remove an object from the copied vector.
/// @param v : (vdl_vec*) A vector.
/// @param i : (int) The index.
#define vdl_Pop(v, i) vdl_Remove(v, i, 1)

/*-----------------------------------------------------------------------------
 |  Concatenate two vector
 ----------------------------------------------------------------------------*/

/// @description Extend the first vector by the second vector.
/// @param v1 : (vdl_vec*) The first vector.
/// @param v2 : (vdl_vec*) The second vector.
#define vdl_IConcatenate(...) vdl_bt_Call(vdl_IConcatenate_BT, __VA_ARGS__)
static inline void vdl_IConcatenate_BT(vdl_bt bt, vdl_vec *const v1, vdl_vec *const v2)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v1);
    vdl_HealthCheck(v2);
    vdl_assert_IncompatibleType(vdl_Type(v1), vdl_Type(v2));
    vdl_IAppend(v1, vdl_Data(v2), vdl_Length(v2));
    vdl_ReturnConst();
}

/// @description Copy the first vector, and then extend the copied vector by the second vector.
/// @param v1 : (vdl_vec*) The first vector.
/// @param v2 : (vdl_vec*) The second vector.
#define vdl_Concatenate(v1, v2) vdl_IConcatenate(vdl_Copy(v1), v2)

/*-----------------------------------------------------------------------------
 |  Vector indexing
 ----------------------------------------------------------------------------*/

/// @description Extract a vector by a vector of indices.
/// @param v : (vdl_vec*) A vector.
/// @param indices : (vdl_vec*) A vector of indices.
/// @return (vdl_vec*) A vector of the same length as `indices`.
#define vdl_Indexing(...) vdl_bt_Call(vdl_Indexing_BT, __VA_ARGS__)
static inline vdl_vp vdl_Indexing_BT(vdl_bt bt, vdl_vec *const v, vdl_vec *const indices)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_HealthCheck(indices);
    vdl_assert_IncompatibleType(VDL_TYPE_INT, vdl_Type(indices));
    vdl_assert_ZeroLengthVector(indices);

    vdl_vp result = vdl_New(vdl_Type(v), vdl_Length(indices));
    vdl_For_i(vdl_Length(indices)) vdl_IAppend(result, vdl_Get(v, vdl_GetInt(indices, i)), 1);
    vdl_ReturnConst(result);
}

/*-----------------------------------------------------------------------------
 |  Vector negative indexing
 ----------------------------------------------------------------------------*/

/// @description Extract a vector by a vector of negative indices.
/// @param v : (vdl_vec*) A vector.
/// @param indices : (vdl_vec*) A vector of indices.
/// @return (vdl_vec*) A vector.
#define vdl_NegIndexing(...) vdl_bt_Call(vdl_NegIndexing_BT, __VA_ARGS__)
static inline vdl_vp vdl_NegIndexing_BT(vdl_bt bt, vdl_vec *const v, vdl_vec *const neg_indices)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_HealthCheck(neg_indices);
    vdl_assert_IncompatibleType(VDL_TYPE_INT, vdl_Type(neg_indices));
    vdl_assert_ZeroLengthVector(neg_indices);

    vdl_vp result = vdl_New(vdl_Type(v), vdl_Length(v));

    vdl_For_i(vdl_Length(v))
    {
        int neg_i = -i;
        if (vdl_Find(neg_indices, &neg_i) == -1)
            vdl_IAppend(result, vdl_Get(v, i), 1);
    }
    vdl_ReturnConst(result);
}

/*-----------------------------------------------------------------------------
 |  Insert objects to a vector
 ----------------------------------------------------------------------------*/

/// @description Insert objects to a vector.
/// @param v : (vdl_vec*). A vector.
/// @param index : (int). The first index to be inserted.
/// @param object : (void*). An array of objects.
/// @param num_object : (int). Number of objects.
#define vdl_Insert(...) vdl_bt_Call(vdl_Insert_BT, __VA_ARGS__)
static inline void vdl_Insert_BT(vdl_bt bt, vdl_vec *const v, const int i, void *const object, const int num_object)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_assert_IndexOutOfBound(v, i);
    vdl_assert_NullPointer(object);
    vdl_assert_ZeroObjects(num_object);

    // Reserve enough space
    vdl_Reserve(v, vdl_Length(v) + num_object);
    vdl_Length(v) = vdl_Length(v) + num_object;

    // Move the old content to the end
    vdl_Set(v, i + num_object, vdl_AddressOf(v, i), num_object);
    // Copy in the new content
    vdl_Set(v, i, object, num_object);
    vdl_ReturnConst();
}

/*-----------------------------------------------------------------------------
 |  Equal operator
 ----------------------------------------------------------------------------*/

// static inline vdl_vp vdl_ScalarEq_BT(vdl_bt bt, const vdl_vec *const v, void *const object)
// {
//     vdl_WithBT()
//     {
//         vdl_HealthCheck(v);
//         vdl_assert_NullPointer(object);
//
//         vdl_vp result = vdl_New(vdl_Type(v), vdl_Length(v));
//
//         switch (vdl_Type(v))
//         {
//             case VDL_TYPE_CHAR:
//             {
//                 const char obj_val = vdl_char_Array(object)[0];
//                 vdl_For_i(vdl_Length(v)) vdl_IAppend();
//                 vdl_ReturnConst(-1);
//             }
//         }
//     }
// }


/*-----------------------------------------------------------------------------
 |  In operator
 ----------------------------------------------------------------------------*/

/// @description Check if a vector contains an object.
/// @details Comparison is made by using the double equal sign. Thus, floating
/// point comparison could be unsafe.
/// @param v : (vdl_vec*) A vector.
/// @param object : (void*) An object.
/// @return (int) 0 for not found, 1 for found.
// #define vdl_ScalarIn(...) vdl_bt_Call(vdl_ScalarIn_BT, __VA_ARGS__)
// static inline int vdl_ScalarIn_BT(vdl_bt bt, vdl_vec *const v, void *object)
// {
//     vdl_WithBT()
//     {
//         vdl_HealthCheck(v);
//         vdl_assert_NullPointer(object);
//         switch (vdl_Type(v))
//         {
//             case VDL_TYPE_CHAR:
//                 vdl_For_i(vdl_Length(v)) if (vdl_GetChar(v, i) == vdl_char_Array(object)[0]) vdl_ReturnConst(1);
//                 break;
//             case VDL_TYPE_INT:
//                 vdl_For_i(vdl_Length(v)) if (vdl_GetInt(v, i) == vdl_int_Array(object)[0]) vdl_ReturnConst(1);
//                 break;
//             case VDL_TYPE_DOUBLE:
//                 vdl_For_i(vdl_Length(v)) if (vdl_GetDouble(v, i) == vdl_double_Array(object)[0]) vdl_ReturnConst(1);
//                 break;
//             case VDL_TYPE_VP:
//                 vdl_For_i(vdl_Length(v)) if (vdl_GetVp(v, i) == vdl_vp_Array(object)[0]) vdl_ReturnConst(1);
//         }
//         vdl_ReturnConst(0);
//     }
// }
//
// #define vdl_Copy(...) vdl_bt_Call(vdl_Copy_BT, __VA_ARGS__)
// static inline vdl_vp vdl_Copy_BT(vdl_bt bt, vdl_vec *v)
// {
//     vdl_WithBT()
//     {
//         vdl_assert_Unimplemented();
//         vdl_ReturnConst(v);
//     }
// }
//
//
//
//


//
//
// #define TEMPLATE_vec_T_Print(T, f)                                          \
//     static inline void vec_##T##_Print(const vdl_vec *const v, const char *end) \
//     {                                                                       \
//         vec_HealthCheck(v);                                                 \
//         vdl_assert_IncompatibleType(vdl_Type(v), lower_vec_##T);                    \
//                                                                             \
//         if (end == NULL)                                                    \
//             end = "\n";                                                     \
//         printf("[");                                                        \
//         vdl_For_i(vdl_Length(v))                                                    \
//         {                                                                   \
//             printf(f, T##_array(vdl_Data(v))[i]);                               \
//             if (i < vdl_Length(v) - 1)                                            \
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
//     switch (vdl_Type(v))
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
//         vdl_For_i(vdl_Length(v))
//         {
//             const int idx = vec_Find(guard, vdl_vp_Array(vdl_Data(v))[i]);
//             if (idx == -1)
//                 vec_PrintRecursive(vdl_vp_Array(vdl_Data(v))[i], "", guard);
//             else
//                 printf("...");
//             if (i < vdl_Length(v) - 1)
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
//     vp copy = vec_New(vdl_Type(v), vdl_Capacity(v));
//     copy->size = vdl_Length(v);
//
//     // Only the first layer of the content will be copied
//     vec_Set(copy, 0, vdl_Data(v), vdl_Length(v));
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
//     if (vdl_Type(v) == VEC_VP)
//         vdl_For_i(vdl_Length(v)) vdl_vp_Array(vdl_Data(v))[i] = vec_DeepCopyRecursive(vdl_vp_Array(vdl_Data(v))[i], guard, copied);
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

#endif//VDL_VDLPORTAL_H
