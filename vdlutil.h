//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLUTIL_H
#define VDL_VDLUTIL_H

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------
 |  First n arguments
 ----------------------------------------------------------------------------*/

#define vdl_internal_First1Args(arg, ...) arg
#define vdl_internal_First2Args(arg, ...) arg, vdl_internal_First1Args(__VA_ARGS__)
#define vdl_internal_First3Args(arg, ...) arg, vdl_internal_First2Args(__VA_ARGS__)
#define vdl_internal_First4Args(arg, ...) arg, vdl_internal_First3Args(__VA_ARGS__)
#define vdl_internal_First5Args(arg, ...) arg, vdl_internal_First4Args(__VA_ARGS__)
#define vdl_internal_First6Args(arg, ...) arg, vdl_internal_First5Args(__VA_ARGS__)
#define vdl_internal_First7Args(arg, ...) arg, vdl_internal_First6Args(__VA_ARGS__)
#define vdl_internal_First8Args(arg, ...) arg, vdl_internal_First7Args(__VA_ARGS__)
#define vdl_internal_First9Args(arg, ...) arg, vdl_internal_First8Args(__VA_ARGS__)
#define vdl_internal_First10Args(arg, ...) arg, vdl_internal_First9Args(__VA_ARGS__)
#define vdl_internal_First11Args(arg, ...) arg, vdl_internal_First10Args(__VA_ARGS__)
#define vdl_internal_First12Args(arg, ...) arg, vdl_internal_First11Args(__VA_ARGS__)
#define vdl_internal_First13Args(arg, ...) arg, vdl_internal_First12Args(__VA_ARGS__)
#define vdl_internal_First14Args(arg, ...) arg, vdl_internal_First13Args(__VA_ARGS__)
#define vdl_internal_First15Args(arg, ...) arg, vdl_internal_First14Args(__VA_ARGS__)
#define vdl_internal_First16Args(arg, ...) arg, vdl_internal_First15Args(__VA_ARGS__)
#define vdl_internal_First17Args(arg, ...) arg, vdl_internal_First16Args(__VA_ARGS__)
#define vdl_internal_First18Args(arg, ...) arg, vdl_internal_First17Args(__VA_ARGS__)
#define vdl_internal_First19Args(arg, ...) arg, vdl_internal_First18Args(__VA_ARGS__)
#define vdl_internal_First20Args(arg, ...) arg, vdl_internal_First19Args(__VA_ARGS__)

/// @description Get the first n arguments.
/// @param n (integer constant) Number of arguments.
/// @param ... Arguments.
#define vdl_FirstNArgs(n, ...) vdl_internal_First##n##Args(__VA_ARGS__)

/// @description Count arguments.
/// @param ... Arguments.
#define vdl_CountArgs(...) vdl_internal_CountArgs(0, ##__VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define vdl_internal_CountArgs(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

#define vdl_internal_RmFirst0Args(...) __VA_ARGS__
#define vdl_internal_RmFirst1Args(first_arg, ...) __VA_ARGS__
#define vdl_internal_RmFirst2Args(first_arg, ...) vdl_internal_RmFirst1Args(__VA_ARGS__)
#define vdl_internal_RmFirst3Args(first_arg, ...) vdl_internal_RmFirst2Args(__VA_ARGS__)
#define vdl_internal_RmFirst4Args(first_arg, ...) vdl_internal_RmFirst3Args(__VA_ARGS__)
#define vdl_internal_RmFirst5Args(first_arg, ...) vdl_internal_RmFirst4Args(__VA_ARGS__)
#define vdl_internal_RmFirst6Args(first_arg, ...) vdl_internal_RmFirst5Args(__VA_ARGS__)
#define vdl_internal_RmFirst7Args(first_arg, ...) vdl_internal_RmFirst6Args(__VA_ARGS__)
#define vdl_internal_RmFirst8Args(first_arg, ...) vdl_internal_RmFirst7Args(__VA_ARGS__)
#define vdl_internal_RmFirst9Args(first_arg, ...) vdl_internal_RmFirst8Args(__VA_ARGS__)
#define vdl_internal_RmFirst10Args(first_arg, ...) vdl_internal_RmFirst9Args(__VA_ARGS__)
#define vdl_internal_RmFirst11Args(first_arg, ...) vdl_internal_RmFirst10Args(__VA_ARGS__)
#define vdl_internal_RmFirst12Args(first_arg, ...) vdl_internal_RmFirst11Args(__VA_ARGS__)
#define vdl_internal_RmFirst13Args(first_arg, ...) vdl_internal_RmFirst12Args(__VA_ARGS__)
#define vdl_internal_RmFirst14Args(first_arg, ...) vdl_internal_RmFirst13Args(__VA_ARGS__)
#define vdl_internal_RmFirst15Args(first_arg, ...) vdl_internal_RmFirst14Args(__VA_ARGS__)
#define vdl_internal_RmFirst16Args(first_arg, ...) vdl_internal_RmFirst15Args(__VA_ARGS__)
#define vdl_internal_RmFirst17Args(first_arg, ...) vdl_internal_RmFirst16Args(__VA_ARGS__)
#define vdl_internal_RmFirst18Args(first_arg, ...) vdl_internal_RmFirst17Args(__VA_ARGS__)
#define vdl_internal_RmFirst19Args(first_arg, ...) vdl_internal_RmFirst18Args(__VA_ARGS__)
#define vdl_internal_RmFirst20Args(first_arg, ...) vdl_internal_RmFirst19Args(__VA_ARGS__)

/// @description Exclude the first n arguments.
/// @param n (integer constant) Number of arguments.
/// @param ... Arguments.
#define vdl_RmFirstNArgs(n, ...) vdl_internal_RmFirst##n##Args(__VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Loop through the length
 ----------------------------------------------------------------------------*/

/// @description For i from 0 to ub - 1.
/// @param ub (int). The upper bound.
#define vdl_For_i(ub) for (int i = 0; i < (ub); i++)
/// @description For j from 0 to ub - 1.
/// @param ub (int). The upper bound.
#define vdl_For_j(ub) for (int j = 0; j < (ub); j++)
/// @description For k from 0 to ub - 1.
/// @param ub (int). The upper bound.
#define vdl_For_k(ub) for (int k = 0; k < (ub); k++)

/*-----------------------------------------------------------------------------
 |  Unused arguments
 ----------------------------------------------------------------------------*/

/// @description Variable x is unused.
#define vdl_Unused(x) (void) (x)

#endif//VDL_VDLUTIL_H
