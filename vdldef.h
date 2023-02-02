//
// Created by Patrick Li on 25/9/22.
//

// Definition of vector

#ifndef VDL_VDLDEF_H
#define VDL_VDLDEF_H

#include <limits.h>
#include <math.h>
#include <stdlib.h>

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

#endif//VDL_VDLDEF_H
