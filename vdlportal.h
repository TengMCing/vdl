//
// Created by Patrick Li on 3/10/22.
//

#ifndef VDL_VDLPORTAL_H
#define VDL_VDLPORTAL_H

#include "vdlmem.h"

vdl_vp vdl_Copy(vdl_vec *);

static inline void vdl_IAppend(vdl_vec *const v, void *const object, const int num_object)
{
    vdl_HealthCheck(v);
    vdl_assert_NullPointer(object);
    vdl_assert_ZeroObjects(num_object);

    vdl_Reserve(v, vdl_Length(v) + num_object);
    vdl_Length(v) += num_object;
    vdl_Set(v, vdl_Length(v) - num_object, object, num_object);
}

#define vdl_Append(v, object, num_object) vdl_IAppend(vdl_Copy(v), object, num_object)

static inline vdl_vp vdl_Seq(const int begin, const int end)
{
    const int length = (end - begin) * ((end - begin > 0) - (end - begin < 0)) + 1;
    vdl_vp sequence  = vdl_New(VDL_TYPE_INT, length);
    int inc          = (end - begin > 0) - (end - begin < 0);
    int start        = begin;

    vdl_For_i(length)
    {
        vdl_IAppend(sequence, &start, 1);
        start += inc;
    }
    return sequence;
}

static inline void vdl_IConcatenate(vdl_vec *const v1, vdl_vec *const v2)
{
    vdl_HealthCheck(v1);
    vdl_HealthCheck(v2);
    vdl_assert_IncompatibleType(vdl_Type(v1), vdl_Type(v2));

    vdl_IAppend(v1, vdl_Data(v2), vdl_Length(v2));
}

#define vdl_Concatenate(v1, v2) vdl_IConcatenate(vdl_Copy(v1), v2)

static inline void vdl_IRemove(vdl_vec *const v, const int i, const int num_object)
{
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
}

static inline vdl_vp vdl_Remove(vdl_vec *const v, const int i, const int num_object)
{
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
    return result;
}

static inline int vdl_ScalarIn(vdl_vec *const v, void *object)
{
    vdl_HealthCheck(v);
    vdl_assert_NullPointer(object);
    switch (vdl_Type(v))
    {
    case VDL_TYPE_CHAR:
        vdl_For_i(vdl_Length(v)) if (vdl_GetChar(v, i) == vdl_char_Array(object)[0]) return 1;
        break;
    case VDL_TYPE_INT:
        vdl_For_i(vdl_Length(v)) if (vdl_GetInt(v, i) == vdl_int_Array(object)[0]) return 1;
        break;
    case VDL_TYPE_DOUBLE:
        vdl_For_i(vdl_Length(v)) if (vdl_GetDouble(v, i) == vdl_double_Array(object)[0]) return 1;
        break;
    case VDL_TYPE_VP:
        vdl_For_i(vdl_Length(v)) if (vdl_GetVp(v, i) == vdl_vp_Array(object)[0]) return 1;
    }
    return 0;
}

static inline vdl_vp vdl_NegIndexing(vdl_vec *const v, vdl_vec *const indices)
{
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
    return result;
}

#define vdl_IPop(v, i) vdl_IRemove(v, i, 1)
#define vdl_Pop(v, i) vdl_Remove(v, i, 1)

static inline vdl_vp vdl_Indexing(vdl_vec *const v, vdl_vec *const indices)
{
    vdl_HealthCheck(v);
    vdl_HealthCheck(indices);
    vdl_assert_IncompatibleType(VDL_TYPE_INT, vdl_Type(indices));
    vdl_assert_ZeroLengthVector(indices);

    vdl_vp result = vdl_New(vdl_Type(v), vdl_Length(indices));

    vdl_For_i(vdl_Length(indices)) vdl_IAppend(result, vdl_Get(v, vdl_GetInt(indices, i)), 1);
    return result;
}


#endif//VDL_VDLPORTAL_H
