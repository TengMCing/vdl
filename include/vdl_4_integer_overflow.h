//
// Created by Patrick Li on 24/2/2023.
//

#ifndef VDL_VDL_4_INTEGER_OVERFLOW_H
#define VDL_VDL_4_INTEGER_OVERFLOW_H

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

#endif//VDL_VDL_4_INTEGER_OVERFLOW_H
