//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VECGC_H
#define VDL_VECGC_H

#include "vecdef.h"
#include "vecerror.h"
#include <stdlib.h>
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Definition of Arena
 ----------------------------------------------------------------------------*/

/// @description Arena struct.
/// @details Arena is designed to be a data structure to record memory
/// allocation of vectors to prevent memory leak.
/// @param capacity : (int) Capacity of the arena.
/// @param size : (int) Size of the arena.
/// @param block : (vp*) Location of each memory block.
typedef struct arena
{
    int capacity;
    int size;
    vp *block;
} arena;

/// @description Pointer to arena struct.
typedef arena *arena_p;

/// @description A global arena to record all allocated of vectors.
static arena_p GARENA = NULL;
/// @description A global arena to record all reachable vectors.
/// It is a placeholder only for internal use during garbage collection.
/// The content is potentially outdated and shall not be trusted by the user.
static arena_p GREACHABLE = NULL;
/// @description A global arena to record all directly reachable vectors.
static arena_p GDREACHABLE = NULL;

/// @description Initial capacity of `GARENA`, `GREACHABLE` and `GDREACHABLE`.
#define GC_INIT_CAPACITY 8

/*-----------------------------------------------------------------------------
 |  Memory management of arena
 ----------------------------------------------------------------------------*/

/// @description New an arena.
/// @details Allocate memory for an arena, such that vectors can be recorded.
/// @param capacity : (int) Requested capacity.
/// @return (arena_p) An arena.
static inline arena_p vec_GCNewArena(const int capacity)
{
    assert_ZeroCapacity(capacity);
    arena *a = malloc(sizeof(arena));
    a->block = calloc((size_t) capacity, sizeof(vp));
    a->size = 0;
    a->capacity = capacity;
    return a;
}

/// @description Reserve more memory for an arena.
/// @details When the arena needs to record more vectors than
/// its current capacity, reallocation of the memory needs to be performed.
/// This function will likely reserve more memory than the requested capacity
/// due to efficiency.
/// @param a : (arena_p) An arena.
/// @param capacity : (int) Requested capacity.
static inline void vec_GCReserveArena(arena *const a, const int capacity)
{
    assert_NullPointer(a);
    if (a->capacity >= capacity)
        return;
    while (a->capacity < capacity)
    {
        static const size_t mem_500KB = 500 * 1024;
        if ((size_t) a->capacity * sizeof(vp) < mem_500KB)
            a->capacity = a->capacity * 2 + 8;
        else
            a->capacity = a->capacity + (int) (mem_500KB / sizeof(vp));
    }
    a->block = realloc(a->block, (size_t) a->capacity * sizeof(vp));
}

/// @description Release some memory of an arena.
/// @details If more than enough memory has been allocated for an arena, some
/// memory can be returned to the system.
/// @param a : (arena_p) An arena.
static inline void vec_GCShrinkArena(arena *const a)
{
    assert_NullPointer(a);
    const int size_times_five = a->size * 5;
    const int size_plus_5MB = a->size + 700000;
    int smaller_cap = size_times_five > size_plus_5MB ? size_plus_5MB : size_times_five;
    if (smaller_cap < GC_INIT_CAPACITY)
        smaller_cap = GC_INIT_CAPACITY;
    if (a->capacity > smaller_cap)
    {
        a->capacity = smaller_cap;
        a->block = realloc(a->block, (size_t) a->capacity * sizeof(vp));
    }
}

/// @description Clean an arena.
/// @details Remove all records from an arena. The recorded vectors will also be
/// deallocated if `free_content == 1`.
/// @param a : (arena_p) An arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
static inline void vec_GCCleanArena(arena *const a, const int free_content)
{
    assert_NullPointer(a);
    if (free_content == 1)
        for_i(a->size)
        {
            vp v = a->block[i];
            assert_NullPointer(v);
            free(v->data);
            free(v);
        }
    a->size = 0;
    vec_GCShrinkArena(a);
}

/// @description Delete an arena.
/// @details Deallocate memory for an arena. The recorded vectors will also be
/// deallocated if `free_content == 1`.
/// @param a : (arena_p) An arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
static inline void vec_GCDelArena(arena *const a, const int free_content)
{
    assert_NullPointer(a);
    if (free_content == 1)
        for_i(a->size)
        {
            vp v = a->block[i];
            assert_NullPointer(v);
            free(v->data);
            free(v);
        }
    free(a->block);
    free(a);
}

/// @description Print an arena.
/// @param a : (arena_p) An arena.
static inline void vec_GCPrintArena(const arena *const a)
{
    assert_NullPointer(a);
    printf("Arena summary:\n[capacity = %d, size = %d]\n", a->capacity, a->size);
    for_i(a->size) printf("\tObject %d <%p>\n", i, (void *) a->block[i]);
    printf("\n");
}

/*-----------------------------------------------------------------------------
 |  Record management of arena
 ----------------------------------------------------------------------------*/

/// @description Fina a vector from an arena.
/// @param a : (arena*) An arena.
/// @param v : (vec*) A vector.
/// @return (int) The index of the vector recorded in the arena. If not
/// found, -1 will be returned.
static inline int vec_GCArenaFind(const arena *const a, const vec *const v)
{
    assert_NullPointer(a);
    assert_NullPointer(v);
    for_i(a->size) if (a->block[i] == v) return i;
    return -1;
}

/// @description Record a vector by an arena.
/// @details If the vector is already being recorded by the arena,
/// then this function will do nothing.
/// @param a : (arena*) An arena.
/// @param v : (vec*) A vector.
static inline void vec_GCArenaRecord(arena *const a, vec *const v)
{
    assert_NullPointer(a);
    assert_NullPointer(v);
    if (vec_GCArenaFind(a, v) != -1)
        return;
    vec_GCReserveArena(a, a->size + 1);
    a->block[a->size] = v;
    a->size += 1;
}

/// @description Untrack a vector by an arena.
/// @details Remove a record from an arena. If `free_content == 1`, the
/// memory of the recorded vector will also be deallocated. If the vector
/// has not been recorded by the arena, the this function will do nothing.
/// @param a : (arena*) An arena.
/// @param v : (vec*) A vector.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
static inline void vec_GCArenaUntrack(arena *const a, vec *const v, const int free_content)
{
    assert_NullPointer(a);
    assert_NullPointer(v);
    const int idx = vec_GCArenaFind(a, v);
    if (idx == -1)
        return;
    if (free_content == 1)
    {
        free(v->data);
        free(v);
    }
    if (a->size - idx - 1 > 0)
        memmove(a->block + idx, a->block + idx + 1, (size_t) (a->size - idx - 1) * sizeof(vp));
    a->size -= 1;
}

/// @description Untrack a vector by an arena.
/// @details Remove a record from an arena. If `free_content == 1`, the
/// memory of the recorded vector will also be deallocated. If the vector
/// has not been recorded by the arena, the this function will do nothing.
/// @param a : (arena*) An arena.
/// @param idx : (int) Index of a vector in the arena.
/// @param free_content : (int) Whether to free the memory of the recorded vectors.
static inline void vec_GCArenaUntrackByIndex(arena *const a, const int idx, const int free_content)
{
    assert_NullPointer(a);
    assert_IndexOutOfBound(a, idx);
    vp v = a->block[idx];
    assert_NullPointer(v);
    if (free_content == 1)
    {
        free(v->data);
        free(v);
    }
    if (a->size - idx - 1 > 0)
        memmove(a->block + idx, a->block + idx + 1, (size_t) (a->size - idx - 1) * sizeof(vp));
    a->size -= 1;
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
static inline void vec_GCInit(void)
{
    assert_InconsistentGC();
    if (GARENA != NULL)
        return;
    GARENA = vec_GCNewArena(GC_INIT_CAPACITY);
    GREACHABLE = vec_GCNewArena(GC_INIT_CAPACITY);
    GDREACHABLE = vec_GCNewArena(GC_INIT_CAPACITY);
}

/// @description Check the health status of the garbage collector.
/// @details The garbage collector is in ill-formed if it is uninitialized or
/// initialized incorrectly.
static inline void vec_GCHealthCheck(void)
{
    assert_InconsistentGC();
    assert_UninitializedGC();
}

/// @description Record a vector by the global arena.
/// @details If the vector is already in the global arena, this function
/// will do nothing.
/// @param v : (vec*) A vector.
static inline void vec_GCRecord(vec *const v)
{
    vec_GCInit();
    vec_GCArenaRecord(GARENA, v);
}

/// @description Untrack a vector by the global arena.
/// @details Since all vectors needs to be tracked by the global arena,
/// untrack a vector also means deleting it. If the vector is not in the
/// global arena, this function will do nothing.
/// @param v : (vec*) A vector.
static inline void vec_GCUntrack(vec *const v)
{
    vec_GCHealthCheck();
    vec_GCArenaUntrack(GARENA, v, 1);
}

/// @description Untrack all vectors by the global arena.
/// @details Since all vectors needs to be tracked by the global arena,
/// untrack a vector also means deleting it. The global reachable arena and
/// the global directly reachable arena will also be reset.
static inline void vec_GCUntrackAll(void)
{
    vec_GCHealthCheck();
    vec_GCCleanArena(GARENA, 1);
    vec_GCCleanArena(GREACHABLE, 0);
    vec_GCCleanArena(GDREACHABLE, 0);
}

/// @description Declare a vector to be directly reachable and track by the directly reachable arena.
/// @details If the vector is already in the global
/// directly reachable arena, this function will do nothing.
/// @param v : (vec*) A vector.
static inline void vec_GCDirectlyReachable(vec *const v)
{
    vec_GCHealthCheck();
    vec_GCArenaRecord(GDREACHABLE, v);
}

/// @description Declare a vector to be directly unreachable and untrack by the global directly reachable arena.
/// @details If the vector is not in the global directly unreachable
/// arena, this function will do nothing.
/// @param v : (vec*) A vector.
static inline void vec_GCDirectlyUnreachable(vec *const v)
{
    vec_GCHealthCheck();
    vec_GCArenaUntrack(GDREACHABLE, v, 0);
}

/// @description Update the global reachable arena based on the global directly reachable arena.
static inline void vec_GCUpdateReachable(void)
{
    vec_GCHealthCheck();
    vec_GCCleanArena(GREACHABLE, 0);
    if (GDREACHABLE->size == 0)
        return;
    for_i(GDREACHABLE->size) vec_GCArenaRecord(GREACHABLE, GDREACHABLE->block[i]);

    int head_idx = 0;
    int tail_idx = GREACHABLE->size - 1;
    while (head_idx <= tail_idx)
    {
        vp head = GREACHABLE->block[head_idx];
        if (head->type == VEC_VP)
        {
            for_i(head->size) vec_GCArenaRecord(GREACHABLE, vp_array(head->data)[i]);
            tail_idx = GREACHABLE->size - 1;
        }
        head_idx += 1;
    }
}

/// @description Run the garbage collector based on the global directly reachable arena.
static inline void vec_GCCleanUp(void)
{
    vec_GCHealthCheck();
    vec_GCUpdateReachable();
    int head_idx = 0;
    while (head_idx <= GARENA->size - 1)
    {
        if (vec_GCArenaFind(GREACHABLE, GARENA->block[head_idx]) == -1)
            vec_GCArenaUntrackByIndex(GARENA, head_idx, 1);

        else
            head_idx += 1;
    }
}

/// @description Kill the garbage collector.
/// @details Since all vectors needs to be tracked by the global arena,
/// kill the garbage collector means deleting them. The global reachable arena and
/// the global directly reachable arena will also be killed.
static inline void vec_GCKill(void)
{
    vec_GCHealthCheck();
    vec_GCCleanUp();
    vec_GCDelArena(GARENA, 1);
    vec_GCDelArena(GREACHABLE, 0);
    vec_GCDelArena(GDREACHABLE, 0);
}

#endif//VDL_VECGC_H
