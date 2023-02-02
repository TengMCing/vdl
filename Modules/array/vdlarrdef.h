//
// Created by Patrick Li on 5/1/23.
//

#ifndef VDL_VDLARRDEF_H
#define VDL_VDLARRDEF_H

#include "../util/vdlUtility.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------
 |  Compile time check
 ----------------------------------------------------------------------------*/

// Only support gcc and clang at the moment
#if !(defined(__GNUC__) || defined(__clang__))
VDL_COMPILE_TIME_ASSERT(0);
#endif

// char type must have 8 bits
VDL_COMPILE_TIME_ASSERT(CHAR_BIT == 8, vdlarrdef);

// long type must have 32 bits
VDL_COMPILE_TIME_ASSERT(INT_MAX == 2147483647, vdlarrdef);

// long type must have 64 bits
VDL_COMPILE_TIME_ASSERT(LONG_MAX == 9223372036854775807L, vdlarrdef);

/*-----------------------------------------------------------------------------
 |  Definition of array types
 ----------------------------------------------------------------------------*/

/// @description Primitive array types.
/// @details Only types defined here can be legally stored in a vector. \n\n
/// VDL_TYPE_CHAR: 0, character type (could be signed or unsigned, this is implementation defined). \n\n
/// VDL_TYPE_INT: 1, signed integer type. \n\n
/// VDL_TYPE_DOUBLE: 2, signed double type. \n\n
/// VDL_TYPE_POINTER: 3, pointer type.
typedef enum VDL_TYPE
{
    VDL_TYPE_CHAR    = 0,
    VDL_TYPE_INT     = 1,
    VDL_TYPE_DOUBLE  = 2,
    VDL_TYPE_POINTER = 3
} VDL_TYPE;

/// @description String representation of primitive array types.
static const char *const VDL_TYPE_STR[4] = {
        [VDL_TYPE_CHAR]    = "VDL_TYPE_CHAR",
        [VDL_TYPE_INT]     = "VDL_TYPE_INT",
        [VDL_TYPE_DOUBLE]  = "VDL_TYPE_DOUBLE",
        [VDL_TYPE_POINTER] = "VDL_TYPE_POINTER"};

/*-----------------------------------------------------------------------------
 |  Missing values
 ----------------------------------------------------------------------------*/

/// @description Missing value of char.
#define VDL_CHAR_NA CHAR_MAX
/// @description Missing value of int.
#define VDL_INT_NA INT_MAX
/// @description Missing value of double.
#define VDL_DOUBLE_NA ((double) NAN)
/// @description Missing value of pointer.
/// @related
#define VDL_POINTER_NA NULL

/*-----------------------------------------------------------------------------
 |  Boolean values
 ----------------------------------------------------------------------------*/

typedef int VDL_BOOL;

/// @description TRUE is an int.
#define VDL_FALSE ((int) 0)
/// @description FALSE is an int.
#define VDL_TRUE ((int) 1)

#endif//VDL_VDLARRDEF_H
