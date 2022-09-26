//
// Created by Patrick Li on 25/9/22.
//

// Definition of vector

#ifndef VDL_VECDEF_H
#define VDL_VECDEF_H

#include <limits.h>
#include <math.h>

typedef enum VECTYPE
{
    VEC_CHAR = 0,
    VEC_INT = 1,
    VEC_DOUBLE = 2,
    VEC_VP = 3
} VECTYPE;

typedef struct vec
{
    int capacity;
    int size;
    VECTYPE type;
    void *data;
} vec;

typedef vec *vp;

#define VEC_CHAR_NA CHAR_MAX
#define VEC_INT_NA INT_MAX
#define VEC_DOUBLE_NA ((double) NAN)
#define VEC_VP_NA NULL


#define char_array(data) ((char *) (data))
#define int_array(data) ((int *) (data))
#define double_array(data) ((double *) (data))
#define vp_array(data) ((vp *) (data))

#endif//VDL_VECDEF_H
