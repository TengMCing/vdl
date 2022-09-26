//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VECUTIL_H
#define VDL_VECUTIL_H

#include <stdio.h>
#include <stdlib.h>

#define FIRST_1_ARGS(arg, ...) arg
#define FIRST_2_ARGS(arg, ...) arg, FIRST_1_ARGS(__VA_ARGS__)
#define FIRST_3_ARGS(arg, ...) arg, FIRST_2_ARGS(__VA_ARGS__)
#define FIRST_4_ARGS(arg, ...) arg, FIRST_3_ARGS(__VA_ARGS__)
#define FIRST_5_ARGS(arg, ...) arg, FIRST_4_ARGS(__VA_ARGS__)
#define FIRST_6_ARGS(arg, ...) arg, FIRST_5_ARGS(__VA_ARGS__)
#define FIRST_7_ARGS(arg, ...) arg, FIRST_6_ARGS(__VA_ARGS__)
#define FIRST_8_ARGS(arg, ...) arg, FIRST_7_ARGS(__VA_ARGS__)
#define FIRST_9_ARGS(arg, ...) arg, FIRST_8_ARGS(__VA_ARGS__)
#define FIRST_10_ARGS(arg, ...) arg, FIRST_9_ARGS(__VA_ARGS__)
#define FIRST_11_ARGS(arg, ...) arg, FIRST_10_ARGS(__VA_ARGS__)
#define FIRST_12_ARGS(arg, ...) arg, FIRST_11_ARGS(__VA_ARGS__)
#define FIRST_13_ARGS(arg, ...) arg, FIRST_12_ARGS(__VA_ARGS__)
#define FIRST_14_ARGS(arg, ...) arg, FIRST_13_ARGS(__VA_ARGS__)
#define FIRST_15_ARGS(arg, ...) arg, FIRST_14_ARGS(__VA_ARGS__)
#define FIRST_16_ARGS(arg, ...) arg, FIRST_15_ARGS(__VA_ARGS__)
#define FIRST_17_ARGS(arg, ...) arg, FIRST_16_ARGS(__VA_ARGS__)
#define FIRST_18_ARGS(arg, ...) arg, FIRST_17_ARGS(__VA_ARGS__)
#define FIRST_19_ARGS(arg, ...) arg, FIRST_18_ARGS(__VA_ARGS__)
#define FIRST_20_ARGS(arg, ...) arg, FIRST_19_ARGS(__VA_ARGS__)
#define FIRST_N_ARGS(n, ...) FIRST_##n##_ARGS(__VA_ARGS__)

#define GET_ARG_COUNT(...) INTERNAL_GET_ARG_COUNT_PRIVATE(0, ##__VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

#define EXCEPT_0_ARGS(...) __VA_ARGS__
#define EXCEPT_1_ARGS(first_arg, ...) __VA_ARGS__
#define EXCEPT_2_ARGS(first_arg, ...) EXCEPT_1_ARGS(__VA_ARGS__)
#define EXCEPT_3_ARGS(first_arg, ...) EXCEPT_2_ARGS(__VA_ARGS__)
#define EXCEPT_4_ARGS(first_arg, ...) EXCEPT_3_ARGS(__VA_ARGS__)
#define EXCEPT_5_ARGS(first_arg, ...) EXCEPT_4_ARGS(__VA_ARGS__)
#define EXCEPT_6_ARGS(first_arg, ...) EXCEPT_5_ARGS(__VA_ARGS__)
#define EXCEPT_7_ARGS(first_arg, ...) EXCEPT_6_ARGS(__VA_ARGS__)
#define EXCEPT_8_ARGS(first_arg, ...) EXCEPT_7_ARGS(__VA_ARGS__)
#define EXCEPT_9_ARGS(first_arg, ...) EXCEPT_8_ARGS(__VA_ARGS__)
#define EXCEPT_10_ARGS(first_arg, ...) EXCEPT_9_ARGS(__VA_ARGS__)
#define EXCEPT_11_ARGS(first_arg, ...) EXCEPT_10_ARGS(__VA_ARGS__)
#define EXCEPT_12_ARGS(first_arg, ...) EXCEPT_11_ARGS(__VA_ARGS__)
#define EXCEPT_13_ARGS(first_arg, ...) EXCEPT_12_ARGS(__VA_ARGS__)
#define EXCEPT_14_ARGS(first_arg, ...) EXCEPT_13_ARGS(__VA_ARGS__)
#define EXCEPT_15_ARGS(first_arg, ...) EXCEPT_14_ARGS(__VA_ARGS__)
#define EXCEPT_16_ARGS(first_arg, ...) EXCEPT_15_ARGS(__VA_ARGS__)
#define EXCEPT_17_ARGS(first_arg, ...) EXCEPT_16_ARGS(__VA_ARGS__)
#define EXCEPT_18_ARGS(first_arg, ...) EXCEPT_17_ARGS(__VA_ARGS__)
#define EXCEPT_19_ARGS(first_arg, ...) EXCEPT_18_ARGS(__VA_ARGS__)
#define EXCEPT_20_ARGS(first_arg, ...) EXCEPT_19_ARGS(__VA_ARGS__)
#define EXCEPT_N_ARGS(n, ...) EXCEPT_##n##_ARGS(__VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Loop through the size
 ----------------------------------------------------------------------------*/

#define for_i(ub) for (int i = 0; i < (ub); i++)
#define for_j(ub) for (int j = 0; j < (ub); j++)
#define for_k(ub) for (int k = 0; k < (ub); k++)

/*-----------------------------------------------------------------------------
 |  Unused arguments
 ----------------------------------------------------------------------------*/

#define UNUSED(x) (void) (x)

/*-----------------------------------------------------------------------------
 |  Expect
 ----------------------------------------------------------------------------*/

#define expect(expr, msg)                                                                                          \
    do {                                                                                                           \
        if ((expr) != 1)                                                                                           \
        {                                                                                                          \
            printf("Error: Raised by function <%s> at line %d of file \"%s\":\n\t", __func__, __LINE__, __FILE__); \
            msg;                                                                                                   \
            printf("\n");                                                                                          \
            exit(EXIT_FAILURE);                                                                                    \
        }                                                                                                          \
    } while (0)

#endif//VDL_VECUTIL_H
