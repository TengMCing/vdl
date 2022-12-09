//
// Created by Patrick Li on 25/9/22.
//


#ifndef VDL_VDLUTIL_H
#define VDL_VDLUTIL_H

/*-----------------------------------------------------------------------------
 |  First n arguments
 ----------------------------------------------------------------------------*/

#define vdlint_First1Arg(arg, ...) arg
#define vdlint_First2Arg(arg, ...) arg, vdlint_First1Arg(__VA_ARGS__)
#define vdlint_First3Arg(arg, ...) arg, vdlint_First2Arg(__VA_ARGS__)
#define vdlint_First4Arg(arg, ...) arg, vdlint_First3Arg(__VA_ARGS__)
#define vdlint_First5Arg(arg, ...) arg, vdlint_First4Arg(__VA_ARGS__)
#define vdlint_First6Arg(arg, ...) arg, vdlint_First5Arg(__VA_ARGS__)
#define vdlint_First7Arg(arg, ...) arg, vdlint_First6Arg(__VA_ARGS__)
#define vdlint_First8Arg(arg, ...) arg, vdlint_First7Arg(__VA_ARGS__)
#define vdlint_First9Arg(arg, ...) arg, vdlint_First8Arg(__VA_ARGS__)
#define vdlint_First10Arg(arg, ...) arg, vdlint_First9Arg(__VA_ARGS__)
#define vdlint_First11Arg(arg, ...) arg, vdlint_First10Arg(__VA_ARGS__)
#define vdlint_First12Arg(arg, ...) arg, vdlint_First11Arg(__VA_ARGS__)
#define vdlint_First13Arg(arg, ...) arg, vdlint_First12Arg(__VA_ARGS__)
#define vdlint_First14Arg(arg, ...) arg, vdlint_First13Arg(__VA_ARGS__)
#define vdlint_First15Arg(arg, ...) arg, vdlint_First14Arg(__VA_ARGS__)
#define vdlint_First16Arg(arg, ...) arg, vdlint_First15Arg(__VA_ARGS__)
#define vdlint_First17Arg(arg, ...) arg, vdlint_First16Arg(__VA_ARGS__)
#define vdlint_First18Arg(arg, ...) arg, vdlint_First17Arg(__VA_ARGS__)
#define vdlint_First19Arg(arg, ...) arg, vdlint_First18Arg(__VA_ARGS__)
#define vdlint_First20Arg(arg, ...) arg, vdlint_First19Arg(__VA_ARGS__)

/// @description Get the first n arguments.
/// @param n (integer constant) Number of arguments.
/// @param ... Arguments.
#define vdlint_FirstNArg(n, ...) vdlint_First##n##Arg(__VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Count the number of arguments
 ----------------------------------------------------------------------------*/

/// @description Count the number of arguments.
/// @param ... Arguments.
/// @return (int) An integer constant.
#define vdlint_CountArg(...) vdlint_CountArgList(0, ##__VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define vdlint_CountArgList(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

/*-----------------------------------------------------------------------------
 |  Remove first n arguments
 ----------------------------------------------------------------------------*/

#define vdlint_RmFirst0Arg(...) __VA_ARGS__
#define vdlint_RmFirst1Arg(first_arg, ...) __VA_ARGS__
#define vdlint_RmFirst2Arg(first_arg, ...) vdlint_RmFirst1Arg(__VA_ARGS__)
#define vdlint_RmFirst3Arg(first_arg, ...) vdlint_RmFirst2Arg(__VA_ARGS__)
#define vdlint_RmFirst4Arg(first_arg, ...) vdlint_RmFirst3Arg(__VA_ARGS__)
#define vdlint_RmFirst5Arg(first_arg, ...) vdlint_RmFirst4Arg(__VA_ARGS__)
#define vdlint_RmFirst6Arg(first_arg, ...) vdlint_RmFirst5Arg(__VA_ARGS__)
#define vdlint_RmFirst7Arg(first_arg, ...) vdlint_RmFirst6Arg(__VA_ARGS__)
#define vdlint_RmFirst8Arg(first_arg, ...) vdlint_RmFirst7Arg(__VA_ARGS__)
#define vdlint_RmFirst9Arg(first_arg, ...) vdlint_RmFirst8Arg(__VA_ARGS__)
#define vdlint_RmFirst10Arg(first_arg, ...) vdlint_RmFirst9Arg(__VA_ARGS__)
#define vdlint_RmFirst11Arg(first_arg, ...) vdlint_RmFirst10Arg(__VA_ARGS__)
#define vdlint_RmFirst12Arg(first_arg, ...) vdlint_RmFirst11Arg(__VA_ARGS__)
#define vdlint_RmFirst13Arg(first_arg, ...) vdlint_RmFirst12Arg(__VA_ARGS__)
#define vdlint_RmFirst14Arg(first_arg, ...) vdlint_RmFirst13Arg(__VA_ARGS__)
#define vdlint_RmFirst15Arg(first_arg, ...) vdlint_RmFirst14Arg(__VA_ARGS__)
#define vdlint_RmFirst16Arg(first_arg, ...) vdlint_RmFirst15Arg(__VA_ARGS__)
#define vdlint_RmFirst17Arg(first_arg, ...) vdlint_RmFirst16Arg(__VA_ARGS__)
#define vdlint_RmFirst18Arg(first_arg, ...) vdlint_RmFirst17Arg(__VA_ARGS__)
#define vdlint_RmFirst19Arg(first_arg, ...) vdlint_RmFirst18Arg(__VA_ARGS__)
#define vdlint_RmFirst20Arg(first_arg, ...) vdlint_RmFirst19Arg(__VA_ARGS__)

/// @description Exclude the first n arguments.
/// @param n (integer constant) Number of arguments.
/// @param ... Arguments.
#define vdlint_RmFirstNArg(n, ...) vdlint_RmFirst##n##Arg(__VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Get the nth argument
 ----------------------------------------------------------------------------*/

/// @description Get the first argument.
#define vdlint_GetArg1(arg1, ...) arg1

/// @description Get the second argument.
#define vdlint_GetArg2(arg1, arg2, ...) arg2

/// @description Get the third argument.
#define vdlint_GetArg3(arg1, arg2, arg3, ...) arg3

/// @description Get the fourth argument.
#define vdlint_GetArg4(arg1, arg2, arg3, arg4, ...) arg4

/// @description Get the fifth argument.
#define vdlint_GetArg5(arg1, arg2, arg3, arg4, arg5, ...) arg5

/*-----------------------------------------------------------------------------
 |  For loop
 ----------------------------------------------------------------------------*/

/// @description A variadic macro similar to `range()` in python. The counter i is an int.
/// @details One argument is end. Two arguments are start and end. Three arguments are start, end and step.
#define vdlint_for_i(...) vdlint_GetArg4(__VA_ARGS__, vdlint_for_i_3, vdlint_for_i_2, vdlint_for_i_1)(__VA_ARGS__)

/// @description A variadic macro similar to `range()` in python. The counter j is an int.
/// @details One argument is end. Two arguments are start and end. Three arguments are start, end and step.
#define vdlint_for_j(...) vdlint_GetArg4(__VA_ARGS__, vdlint_for_j_3, vdlint_for_j_2, vdlint_for_j_1)(__VA_ARGS__)

#define vdlint_for_i_1(end) for (int i = 0; i < (end); i++)
#define vdlint_for_j_1(end) for (int j = 0; j < (end); j++)

#define vdlint_for_i_2(start, end) for (int i = start; i < (end); i++)
#define vdlint_for_j_2(start, end) for (int j = start; j < (end); j++)

#define vdlint_for_i_3(start, end, step) for (int i = start; i < (end); i += (step))
#define vdlint_for_j_3(start, end, step) for (int j = start; j < (end); j += (step))

/*-----------------------------------------------------------------------------
 |  Unused arguments
 ----------------------------------------------------------------------------*/

/// @description Variable x is unused.
#define vdlint_Unused(x) (void) (x)

#endif//VDL_VDLUTIL_H
