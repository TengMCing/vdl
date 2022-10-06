//
// Created by Patrick Li on 3/10/22.
//

#ifndef VDL_VDLPORTAL_H
#define VDL_VDLPORTAL_H

#include "vdlmem.h"

#define vdl_Copy(...) vdl_bt_Call(vdl_Copy_BT, __VA_ARGS__)
static inline vdl_vp vdl_Copy_BT(vdl_bt bt, vdl_vec *v)
{
    vdl_PushBT(bt);
    vdl_assert_Unimplemented();
    vdl_Return(v);
}

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
    vdl_Return();
}

#define vdl_Append(v, object, num_object) vdl_IAppend(vdl_Copy(v), object, num_object)

#define vdl_IntSeq(...) vdl_bt_Call(vdl_IntSeq_BT, __VA_ARGS__)
static inline vdl_vp vdl_IntSeq_BT(vdl_bt bt, const int begin, const int end)
{
    vdl_PushBT(bt);
    const int length = (end - begin) * ((end - begin > 0) - (end - begin < 0)) + 1;
    vdl_vp sequence  = vdl_New(VDL_TYPE_INT, length);
    int inc          = (end - begin > 0) - (end - begin < 0);
    int start        = begin;

    vdl_For_i(length)
    {
        vdl_IAppend(sequence, &start, 1);
        start += inc;
    }
    vdl_Return(sequence);
}

#define vdl_IConcatenate(...) vdl_bt_Call(vdl_IConcatenate_BT, __VA_ARGS__)
static inline void vdl_IConcatenate_BT(vdl_bt bt, vdl_vec *const v1, vdl_vec *const v2)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v1);
    vdl_HealthCheck(v2);
    vdl_assert_IncompatibleType(vdl_Type(v1), vdl_Type(v2));

    vdl_IAppend(v1, vdl_Data(v2), vdl_Length(v2));
    vdl_Return();
}

#define vdl_Concatenate(v1, v2) vdl_IConcatenate(vdl_Copy(v1), v2)

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
        return;
    }

    // Copy memory to index i
    memmove(vdl_AddressOf(v, i),
            vdl_AddressOf(v, i + num_object),
            (size_t) (vdl_Length(v) - i - num_object) * vdl_SizeOfType(vdl_Type(v)));
    vdl_Length(v) -= num_object;

    vdl_Return();
}

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

    vdl_Return(result);
}

#define vdl_ScalarIn(...) vdl_bt_Call(vdl_ScalarIn_BT, __VA_ARGS__)
static inline int vdl_ScalarIn_BT(vdl_bt bt, vdl_vec *const v, void *object)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_assert_NullPointer(object);
    switch (vdl_Type(v))
    {
    case VDL_TYPE_CHAR:
        vdl_For_i(vdl_Length(v)) if (vdl_GetChar(v, i) == vdl_char_Array(object)[0]) vdl_Return(1);
        break;
    case VDL_TYPE_INT:
        vdl_For_i(vdl_Length(v)) if (vdl_GetInt(v, i) == vdl_int_Array(object)[0]) vdl_Return(1);
        break;
    case VDL_TYPE_DOUBLE:
        vdl_For_i(vdl_Length(v)) if (vdl_GetDouble(v, i) == vdl_double_Array(object)[0]) vdl_Return(1);
        break;
    case VDL_TYPE_VP:
        vdl_For_i(vdl_Length(v)) if (vdl_GetVp(v, i) == vdl_vp_Array(object)[0]) vdl_Return(1);
    }
    vdl_Return(0);
}


#define vdl_NegIndexing(...) vdl_bt_Call(vdl_NegIndexing_BT, __VA_ARGS__)
static inline vdl_vp vdl_NegIndexing_BT(vdl_bt bt, vdl_vec *const v, vdl_vec *const indices)
{
    vdl_PushBT(bt);
    vdl_HealthCheck(v);
    vdl_HealthCheck(indices);
    vdl_assert_IncompatibleType(VDL_TYPE_INT, vdl_Type(indices));
    vdl_assert_ZeroLengthVector(indices);

    vdl_vp result = vdl_New(vdl_Type(v), vdl_Length(v));

    vdl_For_i(vdl_Length(v))
    {
        if (!vdl_ScalarIn(indices, &i))
            vdl_IAppend(result, vdl_Get(v, i), 1);
    }
    vdl_Return(result);
}

#define vdl_IPop(v, i) vdl_IRemove(v, i, 1)
#define vdl_Pop(v, i) vdl_Remove(v, i, 1)

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
    vdl_Return(result);
}


//
//
// static inline void vec_Insert(vdl_vec *const v, const int i, void *const object, const int num_object)
// {
//     vec_HealthCheck(v);
//     vdl_assert_IndexOutOfBound(v, i);
//     vdl_assert_NullPointer(object);
//     vdl_assert_ZeroObjects(num_object);
//
//     // Reserve enough space
//     vec_Reserve(v, vdl_Length(v) + num_object);
//     vdl_Length(v) = vdl_Length(v) + num_object;
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
//         vdl_For_i(vdl_Length(v)) if (T##_array(vdl_Data(v))[i] == object) return i; \
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
//     switch (vdl_Type(v))
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
