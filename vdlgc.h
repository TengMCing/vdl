//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLGC_H
#define VDL_VDLGC_H

#include "vdlwrapper.h"
#include <stdlib.h>
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Definition of Arena
 ----------------------------------------------------------------------------*/

/// @description Arena struct.
/// @details Arena is designed to be a data structure to record memory
/// allocation of vectors to prevent memory leak.
/// @param capacity : (int) Capacity of the vdl_gc_arena.
/// @param length : (int) Length of the vdl_gc_arena.
/// @param block : (vdl_vp*) Location of each memory block.
typedef struct vdl_gc_arena
{
    int capacity;
    int length;
    vdl_vp *block;
} vdl_gc_arena;

/// @description Pointer to vdl_gc_arena struct.
typedef vdl_gc_arena *vdl_gc_arenap;

/// @description A global vdl_gc_arena to record all allocated of vectors.
static vdl_gc_arenap VDL_GARENA = NULL;
/// @description A global vdl_gc_arena to record all reachable vectors.
/// It is a placeholder only for internal use during garbage collection.
/// The content is potentially outdated and shall not be trusted by the user.
static vdl_gc_arenap VDL_GREACHABLE = NULL;
/// @description A global vdl_gc_arena to record all directly reachable vectors.
static vdl_gc_arenap VDL_GDREACHABLE = NULL;

/// @description Initial capacity of `VDL_GARENA`, `VDL_GREACHABLE` and `VDL_GDREACHABLE`.
#define VDL_GC_INIT_CAPACITY 8

/*-----------------------------------------------------------------------------
 |  Memory management of arena
 ----------------------------------------------------------------------------*/

/// @description New an arena.
/// @details Allocate memory for an arena, such that vectors can be recorded.
/// @param capacity : (int) Requested capacity.
/// @return (vdl_gc_arenap) An arena.
#define vdl_gc_NewArena(...) vdl_bt_Call(vdl_gc_NewArena_BT, __VA_ARGS__)
static inline vdl_gc_arenap vdl_gc_NewArena_BT(vdl_bt bt, const int capacity)
{
    vdl_PushBT(bt);
    vdl_assert_ZeroCapacity(capacity);
    vdl_gc_arena *a = malloc(sizeof(vdl_gc_arena));
    a->block        = calloc((size_t) capacity, sizeof(vdl_vp));
    a->length       = 0;
    a->capacity     = capacity;
    vdl_ReturnConst(a);
}

/// @description Reserve more memory for an arena.
/// @details When the arena needs to record more vectors than
/// its current capacity, reallocation of the memory needs to be performed.
/// This function will likely reserve more memory than the requested capacity
/// due to efficiency.
/// @param a : (vdl_gc_arena*) An arena.
/// @param capacity : (int) Requested capacity.
#define vdl_gc_ReserveArena(...) vdl_bt_Call(vdl_gc_ReserveArena_BT, __VA_ARGS__)
static inline void vdl_gc_ReserveArena_BT(vdl_bt bt, vdl_gc_arena *const a, const int capacity)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    if (a->capacity >= capacity)
        vdl_ReturnConst();
    while (a->capacity < capacity)
    {
        static const size_t MEM_500KB = 500 * 1024;
        if ((size_t) a->capacity * sizeof(vdl_vp) < MEM_500KB)
            a->capacity = a->capacity * 2 + 8;
        else
            a->capacity += (int) (MEM_500KB / sizeof(vdl_vp));
    }
    a->block = realloc(a->block, (size_t) a->capacity * sizeof(vdl_vp));
    vdl_ReturnConst();
}

/// @description Release some memory of an arena.
/// @details If more than enough memory has been allocated for an arena, some
/// memory can be returned to the system.
/// @param a : (vdl_gc_arena*) An arena.
#define vdl_gc_ShrinkArena(...) vdl_bt_Call(vdl_gc_ShrinkArena_BT, __VA_ARGS__)
static inline void vdl_gc_ShrinkArena_BT(vdl_bt bt, vdl_gc_arena *const a)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    const int size_times_five = a->length * 5;
    const int size_plus_5MB   = a->length + 700000;
    int smaller_cap           = size_times_five > size_plus_5MB ? size_plus_5MB : size_times_five;
    if (smaller_cap < VDL_GC_INIT_CAPACITY)
        smaller_cap = VDL_GC_INIT_CAPACITY;
    if (a->capacity > smaller_cap)
    {
        a->capacity = smaller_cap;
        a->block    = realloc(a->block, (size_t) a->capacity * sizeof(vdl_vp));
    }
    vdl_ReturnConst();
}

/// @description Clean an arena.
/// @details Remove all records from an arena. The recorded vectors will also be
/// deallocated if `free_content == 1`.
/// @param a : (vdl_gc_arena*) An arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
#define vdl_gc_CleanArena(...) vdl_bt_Call(vdl_gc_CleanArena_BT, __VA_ARGS__)
static inline void vdl_gc_CleanArena_BT(vdl_bt bt, vdl_gc_arena *const a, const int free_content)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    if (free_content == 1)
        vdl_For_i(a->length)
        {
            vdl_vp v = a->block[i];
            vdl_assert_NullPointer(v);
            free(v->data);
            free(v);
        }
    a->length = 0;
    vdl_gc_ShrinkArena(a);
    vdl_ReturnConst();
}

/// @description Delete an arena.
/// @details Deallocate memory for an arena. The recorded vectors will also be
/// deallocated if `free_content == 1`.
/// @param a : (vdl_gc_arena*) An arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
#define vdl_gc_DelArena(...) vdl_bt_Call(vdl_gc_DelArena_BT, __VA_ARGS__)
static inline void vdl_gc_DelArena_BT(vdl_bt bt, vdl_gc_arena *const a, const int free_content)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    if (free_content == 1)
        vdl_For_i(a->length)
        {
            vdl_vp v = a->block[i];
            vdl_assert_NullPointer(v);
            free(v->data);
            free(v);
        }
    free(a->block);
    free(a);
    vdl_ReturnConst();
}

/// @description Get the memory usage of all the recorded vectors.
/// @param a : (vdl_gc_arena*) An arena.
/// @return (size_t) The memory usage.
#define vdl_gc_SizeOfArena(...) vdl_bt_Call(vdl_gc_SizeOfArena_BT, __VA_ARGS__)
static inline size_t vdl_gc_SizeOfArena_BT(vdl_bt bt, const vdl_gc_arena *const a)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    size_t memory_usage = 0;
    vdl_For_i(a->length) memory_usage += vdl_SizeOfVec(vdl_vp_Array(a->block)[i]);
    vdl_ReturnConst(memory_usage);
}

/// @description Print an arena.
/// @param a : (vdl_gc_arena*) An arena.
#define vdl_gc_PrintArena(...) vdl_bt_Call(vdl_gc_PrintArena_BT, __VA_ARGS__)
static inline void vdl_gc_PrintArena_BT(vdl_bt bt, const vdl_gc_arena *const a)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    printf("Arena summary:\n[capacity = %d, length = %d, memory usage = %zu]\n",
           a->capacity,
           a->length,
           vdl_gc_SizeOfArena(a));
    vdl_For_i(a->length) printf("\tObject %d <%p>\n", i, (void *) a->block[i]);
    printf("\n");
    vdl_ReturnConst();
}

/*-----------------------------------------------------------------------------
 |  Record management of vdl_gc_arena
 ----------------------------------------------------------------------------*/

/// @description Fina a vector from an arena.
/// @param a : (vdl_gc_arena*) An arena.
/// @param v : (vdl_vec*) A vector.
/// @return (int) The index of the vector recorded in the arena. If not
/// found, -1 will be returned.
#define vdl_gc_ArenaFind(...) vdl_bt_Call(vdl_gc_ArenaFind_BT, __VA_ARGS__)
static inline int vdl_gc_ArenaFind_BT(vdl_bt bt, const vdl_gc_arena *const a, const vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    vdl_assert_NullPointer(v);
    vdl_For_i(a->length) if (a->block[i] == v) vdl_ReturnConst(i);
    vdl_ReturnConst(-1);
}

/// @description Record a vector by an arena.
/// @details If the vector is already being recorded by the arena,
/// then this function will do nothing.
/// @param a : (vdl_gc_arena*) An arena.
/// @param v : (vdl_vec*) A vector.
#define vdl_gc_ArenaRecord(...) vdl_bt_Call(vdl_gc_ArenaRecord_BT, __VA_ARGS__)
static inline void vdl_gc_ArenaRecord_BT(vdl_bt bt, vdl_gc_arena *const a, vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    vdl_assert_NullPointer(v);
    if (vdl_gc_ArenaFind(a, v) != -1)
        vdl_ReturnConst();
    vdl_gc_ReserveArena(a, a->length + 1);
    a->block[a->length] = v;
    a->length++;
    vdl_ReturnConst();
}

/// @description Untrack a vector by an arena.
/// @details Remove a record from an arena. If `free_content == 1`, the
/// memory of the recorded vector will also be deallocated. If the vector
/// has not been recorded by the arena, the this function will do nothing.
/// @param a : (vdl_gc_arena*) An arena.
/// @param v : (vdl_vec*) A vector.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
#define vdl_gc_ArenaUntrack(...) vdl_bt_Call(vdl_gc_ArenaUntrack_BT, __VA_ARGS__)
static inline void vdl_gc_ArenaUntrack_BT(vdl_bt bt, vdl_gc_arena *const a, vdl_vec *const v, const int free_content)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    vdl_assert_NullPointer(v);
    const int idx = vdl_gc_ArenaFind(a, v);
    if (idx == -1)
        vdl_ReturnConst();
    if (free_content == 1)
    {
        free(v->data);
        free(v);
    }
    if (a->length - idx - 1 > 0)
        memmove(a->block + idx, a->block + idx + 1, (size_t) (a->length - idx - 1) * sizeof(vdl_vp));
    a->length--;
    vdl_ReturnConst();
}

/// @description Untrack a vector by an arena.
/// @details Remove a record from an vdl_gc_arena. If `free_content == 1`, the
/// memory of the recorded vector will also be deallocated. If the vector
/// has not been recorded by the arena, the this function will do nothing.
/// @param a : (vdl_gc_arena*) An arena.
/// @param idx : (int) Index of a vector in the arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
#define vdl_gc_ArenaUntrackByIndex(...) vdl_bt_Call(vdl_gc_ArenaUntrackByIndex_BT, __VA_ARGS__)
static inline void vdl_gc_ArenaUntrackByIndex_BT(vdl_bt bt, vdl_gc_arena *const a, const int idx, const int free_content)
{
    vdl_PushBT(bt);
    vdl_assert_NullPointer(a);
    vdl_assert_IndexOutOfBound(a, idx);
    vdl_vp v = a->block[idx];
    vdl_assert_NullPointer(v);
    if (free_content == 1)
    {
        free(v->data);
        free(v);
    }
    if (a->length - idx - 1 > 0)
        memmove(a->block + idx, a->block + idx + 1, (size_t) (a->length - idx - 1) * sizeof(vdl_vp));
    a->length--;
    vdl_ReturnConst();
}

/*-----------------------------------------------------------------------------
 |  Memory management of the garbage collector
 ----------------------------------------------------------------------------*/

/// @description Initialize the garbage collector.
/// @details The garbage collector itself needs to be stored in some
/// dynamically allocated memory. And it can't not be resolved at compile-time.
/// Thus, this function needs to be called at run-time before allocating memory
/// for any vectors. If the garbage collector is already initialized, it will
/// do nothing.
#define vdl_gc_Init() vdl_bt_Call(vdl_gc_Init_BT)
static inline void vdl_gc_Init_BT(vdl_bt bt)
{
    vdl_PushBT(bt);
    vdl_assert_InconsistentGC();
    if (VDL_GARENA != NULL)
        vdl_ReturnConst();
    VDL_GARENA      = vdl_gc_NewArena(VDL_GC_INIT_CAPACITY);
    VDL_GREACHABLE  = vdl_gc_NewArena(VDL_GC_INIT_CAPACITY);
    VDL_GDREACHABLE = vdl_gc_NewArena(VDL_GC_INIT_CAPACITY);
    vdl_ReturnConst();
}

/// @description Check the health status of the garbage collector.
/// @details The garbage collector is in ill-formed if it is uninitialized or
/// initialized incorrectly.
#define vdl_gc_HealthCheck() vdl_bt_Call(vdl_gc_HealthCheck_BT)
static inline void vdl_gc_HealthCheck_BT(vdl_bt bt)
{
    vdl_PushBT(bt);
    vdl_assert_InconsistentGC();
    vdl_assert_UninitializedGC();
    vdl_ReturnConst();
}

/// @description Record a vector by the global arena.
/// @details If the vector is already in the global arena, this function
/// will do nothing.
/// @param v : (vdl_vec*) A vector.
#define vdl_gc_Record(...) vdl_bt_Call(vdl_gc_Record_BT, __VA_ARGS__)
static inline void vdl_gc_Record_BT(vdl_bt bt, vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_gc_Init();
    vdl_gc_ArenaRecord(VDL_GARENA, v);
    vdl_ReturnConst();
}

/// @description Untrack a vector by the global arena.
/// @details Since all vectors needs to be tracked by the global arena,
/// untrack a vector also means deleting it. If the vector is not in the
/// global arena, this function will do nothing.
/// @param v : (vdl_vec*) A vector.
#define vdl_gc_Untrack(...) vdl_bt_Call(vdl_gc_Untrack_BT, __VA_ARGS__)
static inline void vdl_gc_Untrack_BT(vdl_bt bt, vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_gc_HealthCheck();
    vdl_gc_ArenaUntrack(VDL_GARENA, v, 1);
    vdl_ReturnConst();
}

/// @description Untrack all vectors by the global arena.
/// @details Since all vectors needs to be tracked by the global arena,
/// untrack a vector also means deleting it. The global reachable arena and
/// the global directly reachable arena will also be reset.
#define vdl_gc_UntrackAll() vdl_bt_Call(vdl_gc_UntrackAll_BT)
static inline void vdl_gc_UntrackAll_BT(vdl_bt bt)
{
    vdl_PushBT(bt);
    vdl_gc_HealthCheck();
    vdl_gc_CleanArena(VDL_GARENA, 1);
    vdl_gc_CleanArena(VDL_GREACHABLE, 0);
    vdl_gc_CleanArena(VDL_GDREACHABLE, 0);
    vdl_ReturnConst();
}

/// @description Declare a vector to be directly reachable and track by the directly reachable arena.
/// @details If the vector is already in the global
/// directly reachable arena, this function will do nothing.
/// @param v : (vdl_vec*) A vector.
#define vdl_gc_DirectlyReachable(...) vdl_bt_Call(vdl_gc_DirectlyReachable_BT, __VA_ARGS__)
static inline void vdl_gc_DirectlyReachable_BT(vdl_bt bt, vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_gc_HealthCheck();
    vdl_gc_ArenaRecord(VDL_GDREACHABLE, v);
    vdl_ReturnConst();
}

/// @description Declare a vector to be directly unreachable and untrack by the global directly reachable arena.
/// @details If the vector is not in the global directly unreachable
/// arena, this function will do nothing.
/// @param v : (vdl_vec*) A vector.
#define vdl_gc_DirectlyUnreachable(...) vdl_bt_Call(vdl_gc_DirectlyUnreachable_BT, __VA_ARGS__)
static inline void vdl_gc_DirectlyUnreachable_BT(vdl_bt bt, vdl_vec *const v)
{
    vdl_PushBT(bt);
    vdl_gc_HealthCheck();
    vdl_gc_ArenaUntrack(VDL_GDREACHABLE, v, 0);
    vdl_ReturnConst();
}

/// @description Update the global reachable vdl_gc_arena based on the global directly reachable arena.
/// @details A breadth-first search will be performed to visit all reachable objects.
#define vdl_gc_UpdateReachable() vdl_bt_Call(vdl_gc_UpdateReachable_BT)
static inline void vdl_gc_UpdateReachable_BT(vdl_bt bt)
{
    vdl_PushBT(bt);
    vdl_gc_HealthCheck();
    vdl_gc_CleanArena(VDL_GREACHABLE, 0);
    if (VDL_GDREACHABLE->length == 0)
        vdl_ReturnConst();
    vdl_For_i(VDL_GDREACHABLE->length) vdl_gc_ArenaRecord(VDL_GREACHABLE, VDL_GDREACHABLE->block[i]);

    int head_idx = 0;
    int tail_idx = VDL_GREACHABLE->length - 1;
    while (head_idx <= tail_idx)
    {
        vdl_vp head = VDL_GREACHABLE->block[head_idx];
        if (head->type == VDL_TYPE_VP)
        {
            vdl_For_i(head->length) vdl_gc_ArenaRecord(VDL_GREACHABLE, vdl_vp_Array(head->data)[i]);
            tail_idx = VDL_GREACHABLE->length - 1;
        }
        head_idx++;
    }
    vdl_ReturnConst();
}

/// @description Run the garbage collector based on the global directly reachable arena.
/// @details An object is reachable if it is directly reachable or it is contained by
/// any reachable objects.
#define vdl_gc_CleanUp() vdl_bt_Call(vdl_gc_CleanUp_BT)
static inline void vdl_gc_CleanUp_BT(vdl_bt bt)
{
    vdl_PushBT(bt);
    vdl_gc_HealthCheck();
    vdl_gc_UpdateReachable();
    int head_idx = 0;
    while (head_idx <= VDL_GARENA->length - 1)
    {
        // TODO: The current implementation is inefficient
        if (vdl_gc_ArenaFind(VDL_GREACHABLE, VDL_GARENA->block[head_idx]) == -1)
            vdl_gc_ArenaUntrackByIndex(VDL_GARENA, head_idx, 1);
        else
            head_idx++;
    }
    vdl_ReturnConst();
}

/// @description Kill the garbage collector.
/// @details Since all vectors needs to be tracked by the global vdl_gc_arena,
/// kill the garbage collector means deleting them. The global reachable vdl_gc_arena and
/// the global directly reachable vdl_gc_arena will also be killed.
#define vdl_gc_Kill() vdl_bt_Call(vdl_gc_Kill_BT)
static inline void vdl_gc_Kill_BT(vdl_bt bt)
{
    vdl_PushBT(bt);
    vdl_gc_HealthCheck();
    vdl_gc_CleanUp();
    vdl_gc_DelArena(VDL_GARENA, 1);
    vdl_gc_DelArena(VDL_GREACHABLE, 0);
    vdl_gc_DelArena(VDL_GDREACHABLE, 0);
    vdl_ReturnConst();
}

#endif//VDL_VDLGC_H
