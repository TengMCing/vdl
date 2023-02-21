//
// Created by Patrick Li on 1/2/23.
//

#ifndef VDL_VDL_H
#define VDL_VDL_H

// TODO: separate declaration and definition?

/*-----------------------------------------------------------------------------
 |  Configuration
 ----------------------------------------------------------------------------*/

/// @description Users can disable exception to improve performance. But the program will abort when an error occurs.
// #define VDL_EXCEPTION_DISABLE

/// @description Users can disable backtrace to improve performance. But the error message will not contain a backtrace for debugging.
// #define VDL_BACKTRACE_DISABLE

/*-----------------------------------------------------------------------------
 |  Standard libraries
 ----------------------------------------------------------------------------*/

#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Header declaration sections
 ----------------------------------------------------------------------------*/

#include "vdl_1_utilities.h"
#include "vdl_2_exception.h"
#include "vdl_3_backtrace.h"
#include "vdl_4_vector_basic.h"
#include "vdl_5_garbage_collector.h"
#include "vdl_6_vector_memory.h"

/*-----------------------------------------------------------------------------
 |  Header definition sections
 ----------------------------------------------------------------------------*/

#include "vdl_2_exception_def.h"
#include "vdl_3_backtrace_def.h"
#include "vdl_4_vector_basic_def.h"
#include "vdl_5_garbage_collector_def.h"
#include "vdl_6_vector_memory_def.h"

#endif//VDL_VDL_H
