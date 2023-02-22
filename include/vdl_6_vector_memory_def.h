//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_6_VECTOR_MEMORY_DEF_H
#define VDL_VDL_6_VECTOR_MEMORY_DEF_H

static inline VDL_VECTOR_P vdl_NewEmpty_BT(const VDL_TYPE_T type, const int capacity)
{
    vdl_CheckRequestedCapacity(capacity);

    // Create a vector and copy in the content.
    VDL_VECTOR_P v       = vdl_Malloc(sizeof(VDL_VECTOR_T), 1);
    VDL_VECTOR_P local_v = &(VDL_VECTOR_T){.Capacity = capacity,
                                           .Mode     = VDL_MODE_HEAP,
                                           .Type     = type,
                                           .Length   = 0,
                                           .Data     = NULL};
    memcpy(v, local_v, sizeof(VDL_VECTOR_T));

    // Allocate memory for the data container.
    v->Data = vdl_Malloc((size_t) capacity * VDL_TYPE_SIZE[type], 1);

    // Record the vector in the vector table.
    vdl_GarbageCollectorRecord(v);

    vdl_ExceptionDeregisterCleanUp(v);
    vdl_ExceptionDeregisterCleanUp(v->Data);
    return v;
}

static inline VDL_VECTOR_P vdl_NewByArray_BT(const VDL_TYPE_T type, const int capacity, const void *const item_pointer, const int number)
{
    VDL_VECTOR_P v = vdl_NewEmpty(type, capacity);
    vdl_SetByArrayAndMemmove(v, 0, item_pointer, number);
    return v;
}

static inline VDL_VECTOR_P vdl_NewByVariadic_BT(const VDL_TYPE_T type, const int length, ...)
{
    VDL_VECTOR_P v = vdl_NewEmpty(type, length);
    v->Length      = length;

    va_list ap;
    va_start(ap, length);

    switch (type)
    {
        case VDL_TYPE_CHAR:
        {
            vdl_for_i(length) vdl_UnsafeSetChar(v, i, (char) va_arg(ap, int));
            break;
        }
        case VDL_TYPE_INT:
        {
            vdl_for_i(length) vdl_UnsafeSetInt(v, i, va_arg(ap, int));
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            vdl_for_i(length) vdl_UnsafeSetDouble(v, i, va_arg(ap, double));
            break;
        }
        case VDL_TYPE_VECTOR_P:
        {
            vdl_for_i(length) vdl_UnsafeSetVectorPointer(v, i, va_arg(ap, VDL_VECTOR_P));
            break;
        }
    }

    va_end(ap);
    return v;
}

static inline void vdl_Reserve_BT(VDL_VECTOR_T *const v, const int capacity)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
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

#endif//VDL_VDL_6_VECTOR_MEMORY_DEF_H
