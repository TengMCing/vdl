//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_5_GARBAGE_COLLECTOR_H
#define VDL_VDL_5_GARBAGE_COLLECTOR_H


// TODO: use restrict to increase performance


/*-----------------------------------------------------------------------------
 |  Memory bookkeeping
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Malloc, calloc and free
 ----------------------------------------------------------------------------*/

/// @description Allocate memory.
/// @param bytes (size_t). Size of the memory block.
/// @param register_object (int). Whether to register the allocated object to be cleaned up
/// when an exception raised.
#define vdl_Malloc(...) vdl_CallFunction(vdl_Malloc_BT, void *, __VA_ARGS__)
static inline void *vdl_Malloc_BT(size_t bytes, int register_object);

/// @description Allocate memory and fill them with 0.
/// @param count (size_t). Number of slots.
/// @param bytes (size_t). Size of each slot.
/// @param register_object (int). Whether to register the allocated object to be cleaned up
/// when an exception raised.
#define vdl_Calloc(...) vdl_CallFunction(vdl_Calloc_BT, void *, __VA_ARGS__)
static inline void *vdl_Calloc_BT(size_t count, size_t bytes, int register_object);
#define vdl_Free free

/*-----------------------------------------------------------------------------
 |  Vector table
 ----------------------------------------------------------------------------*/

#define VDL_VECTOR_TABLE_INIT_CAPACITY 8
#define VDL_VECTOR_TABLE_MAX_CAPACITY (INT_MAX - 512)

/// @description Vector table struct.
/// @param Capacity (int). Capacity.
/// @param Length (int). Length.
/// @param Data (VDL_VECTOR_P *). Data.
typedef struct VDL_VECTOR_TABLE_T
{
    int Capacity;
    int Length;
    VDL_VECTOR_P *Data;
} VDL_VECTOR_TABLE_T;

/// @description A pointer to a vector table struct.
typedef VDL_VECTOR_TABLE_T *VDL_VECTOR_TABLE_P;

/*-----------------------------------------------------------------------------
 |  Vector table operations
 ----------------------------------------------------------------------------*/

/// @description Create a vector table.
/// @return (VDL_VECTOR_TABLE_P) A vector table.
#define vdl_NewVectorTable(...) vdl_CallFunction(vdl_NewVectorTable_BT, VDL_VECTOR_TABLE_P, __VA_ARGS__)
static inline VDL_VECTOR_TABLE_P vdl_NewVectorTable_BT(void);

/// @description Shrink the size of a vector table. Unused space will be removed.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
#define vdl_ShrinkVectorTable(...) vdl_CallVoidFunction(vdl_ShrinkVectorTable_BT, __VA_ARGS__)
static inline void vdl_ShrinkVectorTable_BT(VDL_VECTOR_TABLE_T *vector_table);

/// @description Delete a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
/// @param free_content (int) Whether to free the content.
#define vdl_DeleteVectorTable(...) vdl_CallVoidFunction(vdl_DeleteVectorTable_BT, __VA_ARGS__)
static inline void vdl_DeleteVectorTable_BT(VDL_VECTOR_T *vector_table, int free_content);

/// @description Reserve space for a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
/// @param capacity (int) Requested capacity.
#define vdl_ReserveForVectorTable(...) vdl_CallVoidFunction(vdl_ReserveForVectorTable_BT, __VA_ARGS__)
static inline void vdl_ReserveForVectorTable_BT(VDL_VECTOR_T *vector_table, int capacity);

/// @description Size of all the vectors recorded by the vector table.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
/// @return (size_t) Memory usage.
#define vdl_SizeOfVectorTable(...) vdl_CallFunction(vdl_SizeOfVectorTable_BT, size_t, __VA_ARGS__)
static inline size_t vdl_SizeOfVectorTable_BT(const VDL_VECTOR_T *vector_table);

/// @description Print the vector table.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
#define vdl_PrintVectorTable(...) vdl_CallVoidFunction(vdl_PrintVectorTable_BT, __VA_ARGS__)
static inline void vdl_PrintVectorTable_BT(const VDL_VECTOR_T *vector_table);

/// @description Find a vector in a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
/// @param v (const VDL_VECTOR_T *). A vector.
/// @return (int) Index of the vector. If not found, -1 will be returned.
#define vdl_FindInVectorTable(...) vdl_CallFunction(vdl_FindInVectorTable_BT, int, __VA_ARGS__)
static inline int vdl_FindInVectorTable_BT(const VDL_VECTOR_T *vector_table, const VDL_VECTOR_T *v);

// TODO: check integer overflow in all the functions

/// @description Record a vector in a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
/// @param v (const VDL_VECTOR_T *). A vector.
#define vdl_VectorTableRecord(...) vdl_CallVoidFunction(vdl_VectorTableRecord_BT, __VA_ARGS__)
static inline void vdl_VectorTableRecord_BT(VDL_VECTOR_T *vector_table, const VDL_VECTOR_T *v);

/// @description Untrack a vector from a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
/// @param v (const VDL_VECTOR_T *). A vector.
/// @param free_content (int) Whether to free the content.
#define vdl_VectorTableUntrack(...) vdl_CallVoidFunction(vdl_VectorTableUntrack_BT, __VA_ARGS__)
static inline void vdl_VectorTableUntrack_BT(VDL_VECTOR_T *vector_table, VDL_VECTOR_T *v, int free_content);

/// @description Untrack a vector from a vector table by index.
/// @param vector_table (VDL_VECTOR_TABLE_T *). A vector table.
/// @param index (int). Index of the vector.
/// @param free_content (int) Whether to free the content.
#define vdl_VectorTableUntrackByIndex(...) vdl_CallVoidFunction(vdl_VectorTableUntrackByIndex_BT, __VA_ARGS__)
static inline void vdl_VectorTableUntrackByIndex_BT(VDL_VECTOR_T *vector_table, int index, int free_content);


#endif//VDL_VDL_5_GARBAGE_COLLECTOR_H
