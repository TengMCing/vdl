//
// Created by Patrick Li on 25/9/22.
//


#ifndef VDL_VDLUTIL_H
#define VDL_VDLUTIL_H

/*-----------------------------------------------------------------------------
 |  First n arguments
 ----------------------------------------------------------------------------*/

#define vdlint_util_f1arg(arg, ...) arg
#define vdlint_util_f2arg(arg, ...) arg, vdlint_util_f1arg(__VA_ARGS__)
#define vdlint_util_f3arg(arg, ...) arg, vdlint_util_f2arg(__VA_ARGS__)
#define vdlint_util_f4arg(arg, ...) arg, vdlint_util_f3arg(__VA_ARGS__)
#define vdlint_util_f5arg(arg, ...) arg, vdlint_util_f4arg(__VA_ARGS__)
#define vdlint_util_f6arg(arg, ...) arg, vdlint_util_f5arg(__VA_ARGS__)
#define vdlint_util_f7arg(arg, ...) arg, vdlint_util_f6arg(__VA_ARGS__)
#define vdlint_util_f8arg(arg, ...) arg, vdlint_util_f7arg(__VA_ARGS__)
#define vdlint_util_f9arg(arg, ...) arg, vdlint_util_f8arg(__VA_ARGS__)
#define vdlint_util_f10arg(arg, ...) arg, vdlint_util_f9arg(__VA_ARGS__)
#define vdlint_util_f11arg(arg, ...) arg, vdlint_util_f10arg(__VA_ARGS__)
#define vdlint_util_f12arg(arg, ...) arg, vdlint_util_f11arg(__VA_ARGS__)
#define vdlint_util_f13arg(arg, ...) arg, vdlint_util_f12arg(__VA_ARGS__)
#define vdlint_util_f14arg(arg, ...) arg, vdlint_util_f13arg(__VA_ARGS__)
#define vdlint_util_f15arg(arg, ...) arg, vdlint_util_f14arg(__VA_ARGS__)
#define vdlint_util_f16arg(arg, ...) arg, vdlint_util_f15arg(__VA_ARGS__)
#define vdlint_util_f17arg(arg, ...) arg, vdlint_util_f16arg(__VA_ARGS__)
#define vdlint_util_f18arg(arg, ...) arg, vdlint_util_f17arg(__VA_ARGS__)
#define vdlint_util_f19arg(arg, ...) arg, vdlint_util_f18arg(__VA_ARGS__)
#define vdlint_util_f20arg(arg, ...) arg, vdlint_util_f19arg(__VA_ARGS__)

/// @description Get the first n arguments.
/// @param n (integer constant) Number of arguments.
/// @param ... Arguments.
#define vdl_util_fnarg(n, ...) vdlint_util_f##n##arg(__VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Count the number of arguments
 ----------------------------------------------------------------------------*/

/// @description Count the number of arguments.
/// @param ... Arguments.
#define vdl_util_narg(...) vdlint_util_narg(0, ##__VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define vdlint_util_narg(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

/*-----------------------------------------------------------------------------
 |  Remove first n arguments
 ----------------------------------------------------------------------------*/

#define vdlint_util_rmf0arg(...) __VA_ARGS__
#define vdlint_util_rmf1arg(first_arg, ...) __VA_ARGS__
#define vdlint_util_rmf2arg(first_arg, ...) vdlint_util_rmf1arg(__VA_ARGS__)
#define vdlint_util_rmf3arg(first_arg, ...) vdlint_util_rmf2arg(__VA_ARGS__)
#define vdlint_util_rmf4arg(first_arg, ...) vdlint_util_rmf3arg(__VA_ARGS__)
#define vdlint_util_rmf5arg(first_arg, ...) vdlint_util_rmf4arg(__VA_ARGS__)
#define vdlint_util_rmf6arg(first_arg, ...) vdlint_util_rmf5arg(__VA_ARGS__)
#define vdlint_util_rmf7arg(first_arg, ...) vdlint_util_rmf6arg(__VA_ARGS__)
#define vdlint_util_rmf8arg(first_arg, ...) vdlint_util_rmf7arg(__VA_ARGS__)
#define vdlint_util_rmf9arg(first_arg, ...) vdlint_util_rmf8arg(__VA_ARGS__)
#define vdlint_util_rmf10arg(first_arg, ...) vdlint_util_rmf9arg(__VA_ARGS__)
#define vdlint_util_rmf11arg(first_arg, ...) vdlint_util_rmf10arg(__VA_ARGS__)
#define vdlint_util_rmf12arg(first_arg, ...) vdlint_util_rmf11arg(__VA_ARGS__)
#define vdlint_util_rmf13arg(first_arg, ...) vdlint_util_rmf12arg(__VA_ARGS__)
#define vdlint_util_rmf14arg(first_arg, ...) vdlint_util_rmf13arg(__VA_ARGS__)
#define vdlint_util_rmf15arg(first_arg, ...) vdlint_util_rmf14arg(__VA_ARGS__)
#define vdlint_util_rmf16arg(first_arg, ...) vdlint_util_rmf15arg(__VA_ARGS__)
#define vdlint_util_rmf17arg(first_arg, ...) vdlint_util_rmf16arg(__VA_ARGS__)
#define vdlint_util_rmf18arg(first_arg, ...) vdlint_util_rmf17arg(__VA_ARGS__)
#define vdlint_util_rmf19arg(first_arg, ...) vdlint_util_rmf18arg(__VA_ARGS__)
#define vdlint_util_rmf20arg(first_arg, ...) vdlint_util_rmf19arg(__VA_ARGS__)

/// @description Exclude the first n arguments.
/// @param n (integer constant) Number of arguments.
/// @param ... Arguments.
#define vdl_util_rmfnarg(n, ...) vdlint_util_rmf##n##arg(__VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Get the nth argument
 ----------------------------------------------------------------------------*/

/// @description Get the first argument.
#define vdl_util_getarg1(arg1, ...) arg1
/// @description Get the second argument.
#define vdl_util_getarg2(arg1, arg2, ...) arg2
/// @description Get the third argument.
#define vdl_util_getarg3(arg1, arg2, arg3, ...) arg3
/// @description Get the fourth argument.
#define vdl_util_getarg4(arg1, arg2, arg3, arg4, ...) arg4
/// @description Get the fifth argument.
#define vdl_util_getarg5(arg1, arg2, arg3, arg4, arg5, ...) arg5

/*-----------------------------------------------------------------------------
 |  For loop
 ----------------------------------------------------------------------------*/

/// @description A variadic macro similar to `range()` in python. The counter i is an int.
/// @details One argument is end. Two arguments are start and end. Three arguments are start, end and step.
#define vdl_for_i(...) vdl_util_getarg4(__VA_ARGS__, vdlint_util_for_i_3, vdlint_util_for_i_2, vdlint_util_for_i_1)(__VA_ARGS__)

/// @description A variadic macro similar to `range()` in python. The counter j is an int.
/// @details One argument is end. Two arguments are start and end. Three arguments are start, end and step.
#define vdl_for_j(...) vdl_util_getarg4(__VA_ARGS__, vdlint_util_for_j_3, vdlint_util_for_j_2, vdlint_util_for_j_1)(__VA_ARGS__)

#define vdlint_util_for_i_1(end) for (int i = 0; i < (end); i++)
#define vdlint_util_for_j_1(end) for (int j = 0; j < (end); j++)

#define vdlint_util_for_i_2(start, end) for (int i = start; i < (end); i++)
#define vdlint_util_for_j_2(start, end) for (int j = start; j < (end); j++)

#define vdlint_util_for_i_3(start, end, step) for (int i = start; i < (end); i += (step))
#define vdlint_util_for_j_3(start, end, step) for (int j = start; j < (end); j += (step))


/*-----------------------------------------------------------------------------
 |  Unused arguments
 ----------------------------------------------------------------------------*/

/// @description Variable x is unused.
#define vdl_util_unused(x) (void) (x)

#endif//VDL_VDLUTIL_H
