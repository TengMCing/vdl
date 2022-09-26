//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VECMEM_H
#define VDL_VECMEM_H

#include "vecdef.h"
#include "vecerror.h"
#include "vecgc.h"
#include "vectype.h"

static inline vp vec_New(const VECTYPE type, const int capacity)
{
    assert_UnknownType(type);
    assert_ZeroCapacity(capacity);

    vp v = malloc(sizeof(vec));
    v->capacity = capacity;
    v->type = type;
    v->size = 0;
    v->data = calloc(capacity, vec_GetTypeSize(type));

    vec_GCRecord(v);
    return v;
}

static inline void vec_Reserve(vec *const v, const int capacity)
{
    assert_NullPointer(v);
    if (v->capacity >= capacity)
        return;
    while (v->capacity < capacity)
        v->capacity = (int) (v->capacity * 1.5) + 8;
    v->data = realloc(v->data, v->capacity * vec_GetTypeSize(v->type));
}

static inline void vec_Set(vec *const v, const int i, void *const object)
{
    assert_NullPointer(v);
    assert_NullPointer(object);
    assert_UnknownType(v->type);
    assert_IndexOutOfBound(v, i);
    switch (v->type)
    {
    case VEC_CHAR:
        char_array(v->data)[i] = char_array(object)[0];
        break;
    case VEC_INT:
        int_array(v->data)[i] = int_array(object)[0];
        break;
    case VEC_DOUBLE:
        double_array(v->data)[i] = double_array(object)[0];
        break;
    case VEC_VP:
        vp_array(v->data)[i] = vp_array(object)[0];
        break;
    }
}

static inline void vec_Append(vec *const v, void *const object)
{
    vec_Reserve(v, v->size + 1);
    v->size += 1;
    vec_Set(v, v->size - 1, object);
}

#endif//VDL_VECMEM_H
