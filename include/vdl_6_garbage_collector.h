//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_6_GARBAGE_COLLECTOR_H
#define VDL_VDL_6_GARBAGE_COLLECTOR_H


// TODO: use restrict to increase performance

/*-----------------------------------------------------------------------------
 |  Memory bookkeeping
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Malloc, calloc and free
 ----------------------------------------------------------------------------*/

/// Allocate memory.
/// @param bytes (size_t). Size of the memory block.
/// @param register_object (int). Whether to register the allocated object to be cleaned up
/// when an exception raised.
#define vdl_Malloc(...) vdl_CallFunction(vdl_Malloc_BT, void *, __VA_ARGS__)
static inline void *vdl_Malloc_BT(size_t bytes, int register_object);

/// Allocate memory and fill them with 0.
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

/// Vector table struct.
/// @param Capacity (int). Capacity.
/// @param Length (int). Length.
/// @param Data (VDL_VECTOR_P *). Data.
typedef struct VDL_VECTOR_TABLE_T
{
    int Capacity;
    int Length;
    VDL_VECTOR_P *Data;
} VDL_VECTOR_TABLE_T;

/// A pointer to a vector table struct.
typedef VDL_VECTOR_TABLE_T *VDL_VECTOR_TABLE_P;

/*-----------------------------------------------------------------------------
 |  Vector table operations
 ----------------------------------------------------------------------------*/

/// Create a vector table.
/// @return (VDL_VECTOR_TABLE_P) A vector table.
#define vdl_NewVectorTable(...) vdl_CallFunction(vdl_NewVectorTable_BT, VDL_VECTOR_TABLE_P, __VA_ARGS__)
static inline VDL_VECTOR_TABLE_P vdl_NewVectorTable_BT(void);

/// Shrink the size of a vector table. Unused space will be removed.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
#define vdl_ShrinkVectorTable(...) vdl_CallVoidFunction(vdl_ShrinkVectorTable_BT, __VA_ARGS__)
static inline void vdl_ShrinkVectorTable_BT(VDL_VECTOR_TABLE_P vector_table);

/// Clear a vector table.
/// @param free_content (int). Whether to free the content.
#define vdl_ClearVectorTable(...) vdl_CallVoidFunction(vdl_ClearVectorTable_BT, __VA_ARGS__)
static inline void vdl_ClearVectorTable_BT(VDL_VECTOR_TABLE_P vector_table, int free_content);

/// Delete a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
/// @param free_content (int) Whether to free the content.
#define vdl_DeleteVectorTable(...) vdl_CallVoidFunction(vdl_DeleteVectorTable_BT, __VA_ARGS__)
static inline void vdl_DeleteVectorTable_BT(VDL_VECTOR_TABLE_P vector_table, int free_content);

/// Reserve space for a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
/// @param capacity (int) Requested capacity.
#define vdl_ReserveForVectorTable(...) vdl_CallVoidFunction(vdl_ReserveForVectorTable_BT, __VA_ARGS__)
static inline void vdl_ReserveForVectorTable_BT(VDL_VECTOR_TABLE_P vector_table, int capacity);

/// Size of all the vectors recorded by the vector table.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
/// @return (size_t) Memory usage.
#define vdl_SizeOfVectorTable(...) vdl_CallFunction(vdl_SizeOfVectorTable_BT, size_t, __VA_ARGS__)
static inline size_t vdl_SizeOfVectorTable_BT(VDL_VECTOR_TABLE_P vector_table);

/// Print the vector table.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
#define vdl_PrintVectorTable(...) vdl_CallVoidFunction(vdl_PrintVectorTable_BT, __VA_ARGS__)
static inline void vdl_PrintVectorTable_BT(VDL_VECTOR_TABLE_P vector_table);

/// Find a vector in a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
/// @param v (VDL_VECTOR_P). A vector.
/// @return (int) Index of the vector. If not found, -1 will be returned.
#define vdl_FindInVectorTable(...) vdl_CallFunction(vdl_FindInVectorTable_BT, int, __VA_ARGS__)
static inline int vdl_FindInVectorTable_BT(VDL_VECTOR_TABLE_P vector_table, VDL_VECTOR_P v);

// TODO: check integer overflow in all the functions

/// Record a vector in a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
/// @param v (VDL_VECTOR_P). A vector.
#define vdl_VectorTableRecord(...) vdl_CallVoidFunction(vdl_VectorTableRecord_BT, __VA_ARGS__)
static inline void vdl_VectorTableRecord_BT(VDL_VECTOR_TABLE_P vector_table, VDL_VECTOR_P v);

/// Untrack a vector from a vector table.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
/// @param v (VDL_VECTOR_P). A vector.
/// @param free_content (int) Whether to free the content.
#define vdl_VectorTableUntrack(...) vdl_CallVoidFunction(vdl_VectorTableUntrack_BT, __VA_ARGS__)
static inline void vdl_VectorTableUntrack_BT(VDL_VECTOR_TABLE_P vector_table, VDL_VECTOR_P v, int free_content);

/// Untrack a vector from a vector table by index.
/// @param vector_table (VDL_VECTOR_TABLE_P). A vector table.
/// @param index (int). Index of the vector.
/// @param free_content (int) Whether to free the content.
#define vdl_VectorTableUntrackByIndex(...) vdl_CallVoidFunction(vdl_VectorTableUntrackByIndex_BT, __VA_ARGS__)
static inline void vdl_VectorTableUntrackByIndex_BT(VDL_VECTOR_TABLE_P vector_table, int index, int free_content);

/*-----------------------------------------------------------------------------
 |  Garbage collector
 ----------------------------------------------------------------------------*/

/// A global variable for storing all the allocated vectors.
static VDL_VECTOR_TABLE_P vdl_GlobalVar_VectorTable = NULL;

/// A global variable for storing all the directly reachable vectors.
static VDL_VECTOR_TABLE_P vdl_GlobalVar_DirectlyReachable = NULL;

/// A global variable for storing all the reachable vectors.
static VDL_VECTOR_TABLE_P vdl_GlobalVar_Reachable = NULL;

/// Check the garbage collector state.
#define vdl_CheckGarbageCollector() vdl_Expect(((vdl_GlobalVar_VectorTable == NULL) + (vdl_GlobalVar_DirectlyReachable == NULL) + (vdl_GlobalVar_Reachable == NULL)) % 3 == 0, \
                                               VDL_EXCEPTION_INCONSISTENT_GARBAGE_COLLECTOR_STATE,                                                                             \
                                               "The garbage collector state is inconsistent!")

/// Check the garbage collector state.
#define vdl_GarbageCollectorInit(...) vdl_CallVoidFunction(vdl_GarbageCollectorInit_BT, __VA_ARGS__)
static inline void vdl_GarbageCollectorInit_BT(void);

/// Record a vector by the garbage collector.
/// @param v (VDL_VECTOR_P). A vector.
#define vdl_GarbageCollectorRecord(...) vdl_CallVoidFunction(vdl_GarbageCollectorRecord_BT, __VA_ARGS__)
static inline void vdl_GarbageCollectorRecord_BT(VDL_VECTOR_P v);

/// Declare a vector to be directly reachable.
/// @param v (VDL_VECTOR_P). A vector.
#define vdl_DeclareDirectlyReachable(...) vdl_CallVoidFunction(vdl_DeclareDirectlyReachable_BT, __VA_ARGS__)
static inline void vdl_DeclareDirectlyReachable_BT(VDL_VECTOR_P v);

/// Declare a vector to be directly unreachable.
/// @param v (VDL_VECTOR_P). A vector.
#define vdl_DeclareDirectlyUnreachable(...) vdl_CallVoidFunction(vdl_DeclareDirectlyUnreachable_BT, __VA_ARGS__)
static inline void vdl_DeclareDirectlyUnreachable_BT(VDL_VECTOR_P v);

/// Update the table of reachable vectors.
#define vdl_UpdateReachable(...) vdl_CallVoidFunction(vdl_UpdateReachable_BT, __VA_ARGS__)
static inline void vdl_UpdateReachable_BT(void);

/// Clean the vector table based on reachable vectors.
#define vdl_GarbageCollectorCleanUp(...) vdl_CallVoidFunction(vdl_GarbageCollectorCleanUp_BT, __VA_ARGS__)
static inline void vdl_GarbageCollectorCleanUp_BT(void);

/// Kill the garbage collector.
#define vdl_GarbageCollectorKill(...) vdl_CallVoidFunction(vdl_GarbageCollectorKill_BT, __VA_ARGS__)
static inline void vdl_GarbageCollectorKill_BT(void);

#endif//VDL_VDL_6_GARBAGE_COLLECTOR_H
