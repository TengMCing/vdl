//
// Created by Patrick Li on 25/9/22.
//

// Definition of vector

#ifndef VDL_VDLDEF_H
#define VDL_VDLDEF_H

#include "vdlutil.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------
 |  Definition of vector types
 ----------------------------------------------------------------------------*/

/// @description Primitive vector types.
/// @details Only types defined here can be legally stored in a vector. \n\n
/// VDL_TYPE_CHAR: 0, character type (could be signed or unsigned, this is implementation defined). \n\n
/// VDL_TYPE_INT: 1, signed integer type. \n\n
/// VDL_TYPE_DOUBLE: 2, signed double type. \n\n
/// VDL_TYPE_VP: 3, vector pointer type.
typedef enum VDL_TYPE
{
    VDL_TYPE_CHAR   = 0,
    VDL_TYPE_INT    = 1,
    VDL_TYPE_DOUBLE = 2,
    VDL_TYPE_VP     = 3
} VDL_TYPE;

// #define vdl_lower_type_char VDL_TYPE_CHAR
// #define vdl_lower_type_int VDL_TYPE_INT
// #define vdl_lower_type_double VDL_TYPE_DOUBLE
// #define vdl_lower_type_vdl_vp VDL_TYPE_VP

/// @description String representation of primitive vector types.
static const char *const VDL_TYPE_STR[4] = {
        [VDL_TYPE_CHAR]   = "VDL_TYPE_CHAR",
        [VDL_TYPE_INT]    = "VDL_TYPE_INT",
        [VDL_TYPE_DOUBLE] = "VDL_TYPE_DOUBLE",
        [VDL_TYPE_VP]     = "VDL_TYPE_VP"};

/*-----------------------------------------------------------------------------
 |  Definition of vector storage mode
 ----------------------------------------------------------------------------*/

/// @description Storage mode of a vector.
/// @details
/// VDL_MODE_STACK: 0, stack allocated. \n\n
/// VDL_MODE_HEAP: 1, heap allocated.
typedef enum VDL_MODE
{
    VDL_MODE_STACK = 0,
    VDL_MODE_HEAP  = 1
} VDL_MODE;

/// @description String representation of storage mode of a vector.
static const char *const VDL_MODE_STR[2] = {
        [VDL_MODE_STACK] = "VDL_MODE_STACK",
        [VDL_MODE_HEAP]  = "VDL_MODE_HEAP"};

/*-----------------------------------------------------------------------------
 |  Definition of vector
 ----------------------------------------------------------------------------*/

/// @description Vector struct.
/// @param type: (const VDL_TYPE). Type of the vector.
/// @param mode: (const VDL_MODE). Storage mode of the vector.
/// @param cap (int). Capacity of the vector.
/// @param len: (int). Length of the vector.
/// @param dat: (void*). Data pointer.
typedef struct vdl_vec
{
    const VDL_TYPE type;
    const VDL_MODE mode;
    int cap;
    int len;
    void *dat;
} vdl_vec;

/// @description A vector, or a pointer to a vector struct.
typedef vdl_vec *vdl_vp;

/*-----------------------------------------------------------------------------
 |  Missing values
 ----------------------------------------------------------------------------*/

/// @description Missing value of char.
#define VDL_TYPE_CHAR_NA CHAR_MAX
/// @description Missing value of int.
#define VDL_TYPE_INT_NA INT_MAX
/// @description Missing value of double.
#define VDL_TYPE_DOUBLE_NA ((double) NAN)
/// @description Missing value of vector pointer.
#define VDL_TYPE_VP_NA NULL

#endif//VDL_VDLDEF_H
