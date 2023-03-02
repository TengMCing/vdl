//
// Created by Patrick Li on 24/2/2023.
//

#ifndef VDL_VDL_4_INTEGER_OVERFLOW_H
#define VDL_VDL_4_INTEGER_OVERFLOW_H


/*-----------------------------------------------------------------------------
 |  Compile time check for Clang extension
 ----------------------------------------------------------------------------*/

#ifndef __has_builtin         // Optional of course.
    #define __has_builtin(x) 0// Compatibility with non-clang compilers.
#endif

vdl_CompileTimeAssert(__has_builtin(__builtin_add_overflow), integer_overflow);
vdl_CompileTimeAssert(__has_builtin(__builtin_sub_overflow), integer_overflow);
vdl_CompileTimeAssert(__has_builtin(__builtin_mul_overflow), integer_overflow);


/*-----------------------------------------------------------------------------
 |  Integer overflow handle (int)
 ----------------------------------------------------------------------------*/

/// @description Safely add two integers. Integer overflow will cause an exception.
/// @param x (int). An int.
/// @param y (int). Another int.
/// @param (int) An int.
#define vdl_AddIntOverflow(...) vdl_CallFunction(vdl_AddIntOverflow_BT, int, __VA_ARGS__)
static inline int vdl_AddIntOverflow_BT(int x, int y);

/// @description Safely subtract two integers. Integer overflow will cause an exception.
/// @param x (int). An int.
/// @param y (int). Another int.
/// @param (int) An int.
#define vdl_SubIntOverflow(...) vdl_CallFunction(vdl_SubIntOverflow_BT, int, __VA_ARGS__)
static inline int vdl_SubIntOverflow_BT(int x, int y);

/// @description Safely multiply two integers. Integer overflow will cause an exception.
/// @param x (int). An int.
/// @param y (int). Another int.
/// @param (int) An int.
#define vdl_MulIntOverflow(...) vdl_CallFunction(vdl_MulIntOverflow_BT, int, __VA_ARGS__)
static inline int vdl_MulIntOverflow_BT(int x, int y);

/*-----------------------------------------------------------------------------
 |  Integer overflow handle (long)
 ----------------------------------------------------------------------------*/

/// @description Safely add two long integers. Integer overflow will cause an exception.
/// @param x (long). A long.
/// @param y (long). Another long.
/// @param (long) A long.
#define vdl_AddLongOverflow(...) vdl_CallFunction(vdl_AddLongOverflow_BT, int, __VA_ARGS__)
static inline long vdl_AddLongOverflow_BT(long x, long y);

/// @description Safely subtract two long integers. Integer overflow will cause an exception.
/// @param x (long). A long.
/// @param y (long). Another long.
/// @param (long) A long.
#define vdl_SubLongOverflow(...) vdl_CallFunction(vdl_SubLongOverflow_BT, int, __VA_ARGS__)
static inline long vdl_SubLongOverflow_BT(long x, long y);

/// @description Safely multiply two long integers. Integer overflow will cause an exception.
/// @param x (long). A long.
/// @param y (long). Another long.
/// @param (long) A long.
#define vdl_MulLongOverflow(...) vdl_CallFunction(vdl_MulLongOverflow_BT, int, __VA_ARGS__)
static inline long vdl_MulLongOverflow_BT(long x, long y);

#endif//VDL_VDL_4_INTEGER_OVERFLOW_H
