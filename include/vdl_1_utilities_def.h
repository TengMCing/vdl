//
// Created by Patrick Li on 24/3/2023.
//

#ifndef VDL_VDL_1_UTILITIES_DEF_H
#define VDL_VDL_1_UTILITIES_DEF_H

static inline int vdl_CompareChar(const void *a, const void *b)
{
    const char arg1 = ((const char *) a)[0];
    const char arg2 = ((const char *) b)[0];
    if (arg1 > arg2)
        return 1;
    if (arg1 < arg2)
        return -1;
    return 0;
}

static inline int vdl_CompareInt(const void *a, const void *b)
{
    const int arg1 = ((const int *) a)[0];
    const int arg2 = ((const int *) b)[0];
    if (arg1 > arg2)
        return 1;
    if (arg1 < arg2)
        return -1;
    return 0;
}

static inline int vdl_CompareDouble(const void *a, const void *b)
{
    const double arg1 = ((const double *) a)[0];
    const double arg2 = ((const double *) b)[0];
    if (arg1 > arg2)
        return 1;
    if (arg1 < arg2)
        return -1;
    return 0;
}


static inline int vdl_CompareAddress(const void *a, const void *b)
{
    if (a > b)
        return 1;
    if (a < b)
        return -1;
    return 0;
}

#endif//VDL_VDL_1_UTILITIES_DEF_H
