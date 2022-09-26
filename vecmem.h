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

static inline vp vec_Rserve(vec *const v, const int capacity)
{
    assert_Unimplemented();
}

#endif//VDL_VECMEM_H
