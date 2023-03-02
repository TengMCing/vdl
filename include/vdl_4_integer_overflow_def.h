//
// Created by Patrick Li on 24/2/2023.
//

#ifndef VDL_VDL_4_INTEGER_OVERFLOW_DEF_H
#define VDL_VDL_4_INTEGER_OVERFLOW_DEF_H

/*-----------------------------------------------------------------------------
 |  Integer overflow handle (int)
 ----------------------------------------------------------------------------*/


static inline int vdl_AddIntOverflow_BT(const int x, const int y)
{
    int z             = 0;
    int overflow_flag = __builtin_add_overflow(x, y, &z);
    vdl_Expect(overflow_flag == 0,
               VDL_EXCEPTION_INTEGER_OVERFLOW,
               "Integer overflow occurred!");
    return z;
}


static inline int vdl_SubIntOverflow_BT(const int x, const int y)
{
    int z             = 0;
    int overflow_flag = __builtin_sub_overflow(x, y, &z);
    vdl_Expect(overflow_flag == 0,
               VDL_EXCEPTION_INTEGER_OVERFLOW,
               "Integer overflow occurred!");
    return z;
}


static inline int vdl_MulIntOverflow_BT(const int x, const int y)
{
    int z             = 0;
    int overflow_flag = __builtin_mul_overflow(x, y, &z);
    vdl_Expect(overflow_flag == 0,
               VDL_EXCEPTION_INTEGER_OVERFLOW,
               "Integer overflow occurred!");
    return z;
}

/*-----------------------------------------------------------------------------
 |  Integer overflow handle (long)
 ----------------------------------------------------------------------------*/

static inline long vdl_AddLongOverflow_BT(const long x, const long y)
{
    long z            = 0;
    int overflow_flag = __builtin_add_overflow(x, y, &z);
    vdl_Expect(overflow_flag == 0,
               VDL_EXCEPTION_INTEGER_OVERFLOW,
               "Integer overflow occurred!");
    return z;
}


static inline long vdl_SubLongOverflow_BT(const long x, const long y)
{
    long z            = 0;
    int overflow_flag = __builtin_sub_overflow(x, y, &z);
    vdl_Expect(overflow_flag == 0,
               VDL_EXCEPTION_INTEGER_OVERFLOW,
               "Integer overflow occurred!");
    return z;
}


static inline long vdl_MulLongOverflow_BT(const long x, const long y)
{
    long z            = 0;
    int overflow_flag = __builtin_mul_overflow(x, y, &z);
    vdl_Expect(overflow_flag == 0,
               VDL_EXCEPTION_INTEGER_OVERFLOW,
               "Integer overflow occurred!");
    return z;
}

#endif//VDL_VDL_4_INTEGER_OVERFLOW_DEF_H
