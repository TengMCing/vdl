//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLGC_H
#define VDL_VDLGC_H

#include "vdlportal.h"
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
static inline vdl_gc_arenap vdl_gc_NewArena(const int capacity)
{
    vdl_assert_ZeroCapacity(capacity);
    vdl_gc_arena *a = malloc(sizeof(vdl_gc_arena));
    a->block        = calloc((size_t) capacity, sizeof(vdl_vp));
    a->length       = 0;
    a->capacity     = capacity;
    return a;
}

/// @description Reserve more memory for an arena.
/// @details When the arena needs to record more vectors than
/// its current capacity, reallocation of the memory needs to be performed.
/// This function will likely reserve more memory than the requested capacity
/// due to efficiency.
/// @param a : (vdl_gc_arena*) An arena.
/// @param capacity : (int) Requested capacity.
static inline void vdl_gc_ReserveArena(vdl_gc_arena *const a, const int capacity)
{
    vdl_assert_NullPointer(a);
    if (a->capacity >= capacity)
        return;
    while (a->capacity < capacity)
    {
        static const size_t MEM_500KB = 500 * 1024;
        if ((size_t) a->capacity * sizeof(vdl_vp) < MEM_500KB)
            a->capacity = a->capacity * 2 + 8;
        else
            a->capacity += (int) (MEM_500KB / sizeof(vdl_vp));
    }
    a->block = realloc(a->block, (size_t) a->capacity * sizeof(vdl_vp));
}

/// @description Release some memory of an arena.
/// @details If more than enough memory has been allocated for an arena, some
/// memory can be returned to the system.
/// @param a : (vdl_gc_arena*) An arena.
static inline void vdl_gc_ShrinkArena(vdl_gc_arena *const a)
{
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
}

/// @description Clean an arena.
/// @details Remove all records from an arena. The recorded vectors will also be
/// deallocated if `free_content == 1`.
/// @param a : (vdl_gc_arena*) An arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
static inline void vdl_gc_CleanArena(vdl_gc_arena *const a, const int free_content)
{
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
}

/// @description Delete an arena.
/// @details Deallocate memory for an arena. The recorded vectors will also be
/// deallocated if `free_content == 1`.
/// @param a : (vdl_gc_arena*) An arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
static inline void vdl_gc_DelArena(vdl_gc_arena *const a, const int free_content)
{
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
}

/// @description Get the memory usage of all the recorded vectors.
/// @param a : (vdl_gc_arena*) An arena.
/// @return (size_t) The memory usage.
static inline size_t vdl_gc_SizeOfArena(const vdl_gc_arena *const a)
{
    vdl_assert_NullPointer(a);
    size_t memory_usage = 0;
    vdl_For_i(a->length) memory_usage += vdl_SizeOfVec(vdl_vp_Array(a->block)[i]);
    return memory_usage;
}

/// @description Print an arena.
/// @param a : (vdl_gc_arena*) An arena.
static inline void vdl_gc_PrintArena(const vdl_gc_arena *const a)
{
    vdl_assert_NullPointer(a);
    printf("Arena summary:\n[capacity = %d, length = %d, memory usage = %zu]\n", a->capacity, a->length, vdl_gc_SizeOfArena(a));
    vdl_For_i(a->length) printf("\tObject %d <%p>\n", i, (void *) a->block[i]);
    printf("\n");
}

/*-----------------------------------------------------------------------------
 |  Record management of vdl_gc_arena
 ----------------------------------------------------------------------------*/

/// @description Fina a vector from an arena.
/// @param a : (vdl_gc_arena*) An arena.
/// @param v : (vdl_vec*) A vector.
/// @return (int) The index of the vector recorded in the arena. If not
/// found, -1 will be returned.
static inline int vdl_gc_ArenaFind(const vdl_gc_arena *const a, const vdl_vec *const v)
{
    vdl_assert_NullPointer(a);
    vdl_assert_NullPointer(v);
    vdl_For_i(a->length) if (a->block[i] == v) return i;
    return -1;
}

/// @description Record a vector by an arena.
/// @details If the vector is already being recorded by the arena,
/// then this function will do nothing.
/// @param a : (vdl_gc_arena*) An arena.
/// @param v : (vdl_vec*) A vector.
static inline void vdl_gc_ArenaRecord(vdl_gc_arena *const a, vdl_vec *const v)
{
    vdl_assert_NullPointer(a);
    vdl_assert_NullPointer(v);
    if (vdl_gc_ArenaFind(a, v) != -1)
        return;
    vdl_gc_ReserveArena(a, a->length + 1);
    a->block[a->length] = v;
    a->length++;
}

/// @description Untrack a vector by an arena.
/// @details Remove a record from an arena. If `free_content == 1`, the
/// memory of the recorded vector will also be deallocated. If the vector
/// has not been recorded by the arena, the this function will do nothing.
/// @param a : (vdl_gc_arena*) An arena.
/// @param v : (vdl_vec*) A vector.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
static inline void vdl_gc_ArenaUntrack(vdl_gc_arena *const a, vdl_vec *const v, const int free_content)
{
    vdl_assert_NullPointer(a);
    vdl_assert_NullPointer(v);
    const int idx = vdl_gc_ArenaFind(a, v);
    if (idx == -1)
        return;
    if (free_content == 1)
    {
        free(v->data);
        free(v);
    }
    if (a->length - idx - 1 > 0)
        memmove(a->block + idx, a->block + idx + 1, (size_t) (a->length - idx - 1) * sizeof(vdl_vp));
    a->length--;
}

/// @description Untrack a vector by an arena.
/// @details Remove a record from an vdl_gc_arena. If `free_content == 1`, the
/// memory of the recorded vector will also be deallocated. If the vector
/// has not been recorded by the arena, the this function will do nothing.
/// @param a : (vdl_gc_arena*) An arena.
/// @param idx : (int) Index of a vector in the arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
static inline void vdl_gc_ArenaUntrackByIndex(vdl_gc_arena *const a, const int idx, const int free_content)
{
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
static inline void vdl_gc_Init(void)
{
    vdl_assert_InconsistentGC();
    if (VDL_GARENA != NULL)
        return;
    VDL_GARENA      = vdl_gc_NewArena(VDL_GC_INIT_CAPACITY);
    VDL_GREACHABLE  = vdl_gc_NewArena(VDL_GC_INIT_CAPACITY);
    VDL_GDREACHABLE = vdl_gc_NewArena(VDL_GC_INIT_CAPACITY);
}

/// @description Check the health status of the garbage collector.
/// @details The garbage collector is in ill-formed if it is uninitialized or
/// initialized incorrectly.
static inline void vdl_gc_HealthCheck(void)
{
    vdl_assert_InconsistentGC();
    vdl_assert_UninitializedGC();
}

/// @description Record a vector by the global arena.
/// @details If the vector is already in the global arena, this function
/// will do nothing.
/// @param v : (vdl_vec*) A vector.
static inline void vdl_gc_Record(vdl_vec *const v)
{
    vdl_gc_Init();
    vdl_gc_ArenaRecord(VDL_GARENA, v);
}

/// @description Untrack a vector by the global arena.
/// @details Since all vectors needs to be tracked by the global arena,
/// untrack a vector also means deleting it. If the vector is not in the
/// global arena, this function will do nothing.
/// @param v : (vdl_vec*) A vector.
static inline void vdl_gc_Untrack(vdl_vec *const v)
{
    vdl_gc_HealthCheck();
    vdl_gc_ArenaUntrack(VDL_GARENA, v, 1);
}

/// @description Untrack all vectors by the global arena.
/// @details Since all vectors needs to be tracked by the global arena,
/// untrack a vector also means deleting it. The global reachable arena and
/// the global directly reachable arena will also be reset.
static inline void vdl_gc_UntrackAll(void)
{
    vdl_gc_HealthCheck();
    vdl_gc_CleanArena(VDL_GARENA, 1);
    vdl_gc_CleanArena(VDL_GREACHABLE, 0);
    vdl_gc_CleanArena(VDL_GDREACHABLE, 0);
}

/// @description Declare a vector to be directly reachable and track by the directly reachable arena.
/// @details If the vector is already in the global
/// directly reachable arena, this function will do nothing.
/// @param v : (vdl_vec*) A vector.
static inline void vdl_gc_DirectlyReachable(vdl_vec *const v)
{
    vdl_gc_HealthCheck();
    vdl_gc_ArenaRecord(VDL_GDREACHABLE, v);
}

/// @description Declare a vector to be directly unreachable and untrack by the global directly reachable arena.
/// @details If the vector is not in the global directly unreachable
/// arena, this function will do nothing.
/// @param v : (vdl_vec*) A vector.
static inline void vdl_gc_DirectlyUnreachable(vdl_vec *const v)
{
    vdl_gc_HealthCheck();
    vdl_gc_ArenaUntrack(VDL_GDREACHABLE, v, 0);
}

/// @description Update the global reachable vdl_gc_arena based on the global directly reachable arena.
/// @details A breadth-first search will be performed to visit all reachable objects.
static inline void vdl_gc_UpdateReachable(void)
{
    vdl_gc_HealthCheck();
    vdl_gc_CleanArena(VDL_GREACHABLE, 0);
    if (VDL_GDREACHABLE->length == 0)
        return;
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
}

/// @description Run the garbage collector based on the global directly reachable arena.
/// @details An object is reachable if it is directly reachable or it is contained by
/// any reachable objects.
static inline void vdl_gc_CleanUp(void)
{
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
}

/// @description Kill the garbage collector.
/// @details Since all vectors needs to be tracked by the global vdl_gc_arena,
/// kill the garbage collector means deleting them. The global reachable vdl_gc_arena and
/// the global directly reachable vdl_gc_arena will also be killed.
static inline void vdl_gc_Kill(void)
{
    vdl_gc_HealthCheck();
    vdl_gc_CleanUp();
    vdl_gc_DelArena(VDL_GARENA, 1);
    vdl_gc_DelArena(VDL_GREACHABLE, 0);
    vdl_gc_DelArena(VDL_GDREACHABLE, 0);
}

#endif//VDL_VDLGC_H
