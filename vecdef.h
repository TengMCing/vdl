//
// Created by Patrick Li on 25/9/22.
//

// Definition of vector

#ifndef VDL_VECDEF_H
#define VDL_VECDEF_H

#include <limits.h>
#include <math.h>
#include <stdlib.h>

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
    char placeholder[4];
    void *data;
} vec;

typedef vec *vp;

#define lower_vec_char VEC_CHAR
#define lower_vec_int VEC_INT
#define lower_vec_double VEC_DOUBLE
#define lower_vec_vp VEC_VP

static size_t VECTYPE_SIZE[4] = {
        [VEC_CHAR] = sizeof(char),
        [VEC_INT] = sizeof(int),
        [VEC_DOUBLE] = sizeof(double),
        [VEC_VP] = sizeof(vp)};

#define vec_GetTypeSize(type) VECTYPE_SIZE[type]

#define VEC_CHAR_NA CHAR_MAX
#define VEC_INT_NA INT_MAX
#define VEC_DOUBLE_NA ((double) NAN)
#define VEC_VP_NA NULL

#define char_array(data) ((char *) (data))
#define int_array(data) ((int *) (data))
#define double_array(data) ((double *) (data))
#define vp_array(data) ((vp *) (data))

#endif//VDL_VECDEF_H
