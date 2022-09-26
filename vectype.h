//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VECTYPE_H
#define VDL_VECTYPE_H

#include "vecdef.h"
#include "vecerror.h"

static inline size_t vec_GetTypeSize(const VECTYPE type)
{
    assert_UnknownType(type);
    switch (type)
    {
    case VEC_CHAR:
        return sizeof(char);
    case VEC_INT:
        return sizeof(int);
    case VEC_DOUBLE:
        return sizeof(double);
    case VEC_VP:
        return sizeof(vp);
    }
}

#endif//VDL_VECTYPE_H
