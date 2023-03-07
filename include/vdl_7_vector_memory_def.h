//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_7_VECTOR_MEMORY_DEF_H
#define VDL_VDL_7_VECTOR_MEMORY_DEF_H

/*-----------------------------------------------------------------------------
 |  Construct empty vector on heap
 ----------------------------------------------------------------------------*/

static inline VDL_VECTOR_P vdl_vector_primitive_NewEmpty_BT(const VDL_TYPE_T type, const int capacity)
{
    vdl_CheckRequestedCapacity(capacity);
    vdl_CheckIntNA(capacity);

    // Create a vector and copy in the content.
    VDL_VECTOR_P v       = vdl_Malloc(sizeof(VDL_VECTOR_T), 1);
    VDL_VECTOR_P local_v = &(VDL_VECTOR_T){.Capacity  = capacity,
                                           .Mode      = VDL_MODE_HEAP,
                                           .Type      = type,
                                           .Class     = VDL_CLASS_VECTOR,
                                           .Length    = 0,
                                           .Attribute = NULL,
                                           .Data      = NULL};
    memcpy(v, local_v, sizeof(VDL_VECTOR_T));

    // Allocate memory for the data container.
    v->Data = vdl_Malloc((size_t) capacity * VDL_TYPE_SIZE[type], 1);

    // Record the vector in the vector table.
    vdl_GarbageCollectorRecord(v);

    vdl_ExceptionDeregisterCleanUp(v);
    vdl_ExceptionDeregisterCleanUp(v->Data);
    return v;
}

static inline VDL_VECTOR_P vdl_vector_NewEmpty_BT(VDL_VECTOR_T *const type, VDL_VECTOR_T *const capacity)
{
    vdl_CheckIntVector(type);
    vdl_CheckIntVector(capacity);
    vdl_CheckLength(type->Length, 1);
    vdl_CheckLength(capacity->Length, 1);

    int requested_type = vdl_vector_primitive_UnsafeIntAt(type, 0);
    vdl_CheckUnknownType(requested_type);

    return vdl_vector_primitive_NewEmpty((VDL_TYPE_T) requested_type, vdl_vector_primitive_UnsafeIntAt(capacity, 0));
}

/*-----------------------------------------------------------------------------
 |  Construct vector on heap with a single element
 ----------------------------------------------------------------------------*/

static inline VDL_VECTOR_P vdl_vector_primitive_NewByChar_BT(const char item)
{
    VDL_VECTOR_P v = vdl_vector_primitive_NewEmpty(VDL_TYPE_CHAR, 1);
    vdl_vector_primitive_UnsafeSetChar(v, 0, item);
    return v;
}

static inline VDL_VECTOR_P vdl_vector_primitive_NewByInt_BT(const int item)
{
    VDL_VECTOR_P v = vdl_vector_primitive_NewEmpty(VDL_TYPE_INT, 1);
    vdl_vector_primitive_UnsafeSetInt(v, 0, item);
    return v;
}

static inline VDL_VECTOR_P vdl_vector_primitive_NewByDouble_BT(const double item)
{
    VDL_VECTOR_P v = vdl_vector_primitive_NewEmpty(VDL_TYPE_DOUBLE, 1);
    vdl_vector_primitive_UnsafeSetDouble(v, 0, item);
    return v;
}

static inline VDL_VECTOR_P vdl_vector_primitive_NewByVectorPointer_BT(VDL_VECTOR_T *const item)
{
    VDL_VECTOR_P v = vdl_vector_primitive_NewEmpty(VDL_TYPE_VECTOR_POINTER, 1);
    vdl_vector_primitive_UnsafeSetVectorPointer(v, 0, item);
    return v;
}

/*-----------------------------------------------------------------------------
 |  Construct vector on heap with an array
 ----------------------------------------------------------------------------*/

static inline VDL_VECTOR_P vdl_vector_primitive_NewByArray_BT(const VDL_TYPE_T type, const int capacity, const void *const item_pointer, const int number)
{
    VDL_VECTOR_P v = vdl_vector_primitive_NewEmpty(type, capacity);
    vdl_vector_primitive_SetByArrayAndMemmove(v, 0, item_pointer, number);
    return v;
}

static inline VDL_VECTOR_P vdl_vector_primitive_NewByString_BT(const char *const string, const int number)
{
    VDL_VECTOR_P v = vdl_vector_primitive_NewEmpty(VDL_TYPE_CHAR, number);
    vdl_vector_primitive_SetByArrayAndMemmove(v, 0, string, number);
    return v;
}

/*-----------------------------------------------------------------------------
 |  Construct vector on heap with a variadic function
 ----------------------------------------------------------------------------*/


static inline VDL_VECTOR_P vdl_vector_primitive_NewByVariadic_BT(const VDL_TYPE_T type, const int length, ...)
{
    VDL_VECTOR_P v = vdl_vector_primitive_NewEmpty(type, length);
    v->Length      = length;

    va_list ap;
    va_start(ap, length);

    switch (type)
    {
        case VDL_TYPE_CHAR:
        {
            vdl_for_i(length) vdl_vector_primitive_SetChar(v, i, (char) va_arg(ap, int));
            break;
        }
        case VDL_TYPE_INT:
        {
            vdl_for_i(length) vdl_vector_primitive_SetInt(v, i, va_arg(ap, int));
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            vdl_for_i(length) vdl_vector_primitive_SetDouble(v, i, va_arg(ap, double));
            break;
        }
        case VDL_TYPE_VECTOR_POINTER:
        {
            vdl_for_i(length) vdl_vector_primitive_SetVectorPointer(v, i, va_arg(ap, VDL_VECTOR_P));
            break;
        }
    }

    va_end(ap);
    return v;
}

/*-----------------------------------------------------------------------------
 |  Reserve space for heap allocated vector
 ----------------------------------------------------------------------------*/

static inline void vdl_vector_primitive_Reserve_BT(VDL_VECTOR_T *const v, const int capacity)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckMode(v->Mode, VDL_MODE_HEAP);
    vdl_CheckIntNA(capacity);
    vdl_CheckRequestedCapacity(capacity);

    if (v->Capacity >= capacity)
        return;

    size_t target_capacity = (size_t) v->Capacity;

    // Allocate enough space
    // Switch to arithmetic growth policy after 500KB
    while (target_capacity < (size_t) capacity)
    {
        static const size_t MEM_500KB = 500 * 1024;
        if (target_capacity * VDL_TYPE_SIZE[v->Type] < MEM_500KB)
            target_capacity = target_capacity * 2 + 8;
        else
            target_capacity += MEM_500KB / VDL_TYPE_SIZE[v->Type];
    }
    if (target_capacity > VDL_VECTOR_MAX_CAPACITY)
        target_capacity = VDL_VECTOR_MAX_CAPACITY;

    void *buffer = vdl_Malloc(target_capacity * VDL_TYPE_SIZE[v->Type], 1);
    v->Data      = buffer;
    v->Capacity  = (int) target_capacity;

    vdl_ExceptionDeregisterCleanUp(buffer);
}

static inline void vdl_vector_Reserve_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const capacity)
{
    vdl_CheckNullPointer(capacity);
    vdl_CheckLength(capacity->Length, 1);

    vdl_vector_primitive_Reserve(v, vdl_vector_primitive_GetInt(capacity, 0));
}

#endif//VDL_VDL_7_VECTOR_MEMORY_DEF_H
