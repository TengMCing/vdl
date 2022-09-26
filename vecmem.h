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

static inline void vec_Append(vec *const v, void *const object)
{
    assert_NullPointer(v);
    assert_NullPointer(object);
    assert_UnknownType(v->type);
    vec_Reserve(v, v->size + 1);
    switch (v->type)
    {
    case VEC_CHAR:
        char_array(v->data)[v->size] = char_array(object)[0];
        break;
    case VEC_INT:
        int_array(v->data)[v->size] = int_array(object)[0];
        break;
    case VEC_DOUBLE:
        double_array(v->data)[v->size] = double_array(object)[0];
        break;
    case VEC_VP:
        vp_array(v->data)[v->size] = vp_array(object)[0];
        break;
    }
    v->size += 1;
}

#endif//VDL_VECMEM_H
