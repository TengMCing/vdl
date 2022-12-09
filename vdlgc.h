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
/// @param cap : (int) Capacity of the vdl_arena.
/// @param len : (int) Length of the vdl_arena.
/// @param block : (vdl_vp*) Location of each memory block.
typedef struct vdl_gc_arena
{
    int cap;
    int len;
    vdl_vp *block;
} vdl_arena;

/// @description Pointer to vdl_arena struct.
typedef vdl_arena *vdl_arenap;

/// @description A global vdl_arena to record all allocated of vectors.
static vdl_arenap VDLINT_GARENA = NULL;
/// @description A global vdl_arena to record all reachable vectors.
/// It is a placeholder only for internal use during garbage collection.
/// The content is potentially outdated and shall not be trusted by the user.
static vdl_arenap VDLINT_GREACHABLE = NULL;
/// @description A global vdl_arena to record all directly reachable vectors.
static vdl_arenap VDLINT_GDREACHABLE = NULL;

/// @description Initial cap of `VDLINT_GARENA`, `VDLINT_GREACHABLE` and `VDLINT_GDREACHABLE`.
#define VDL_GC_INIT_CAPACITY 8

/*-----------------------------------------------------------------------------
 |  Memory management of arena
 ----------------------------------------------------------------------------*/

/// @description New an arena.
/// @details Allocate memory for an arena, such that vectors can be recorded.
/// @param cap : (const int) Requested capacity.
/// @return (vdl_arenap) An arena.
/// @noExcept If any check fails, the program will abort.
#define vdlint_NewArena(...) vdlint_Call(vdlint_NewArena_BT, vdl_arenap, __VA_ARGS__)
static inline vdl_arenap vdlint_NewArena_BT(vdl_bt bt, const int cap)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckAllocZeroCap(cap);

    void *buffer_a     = malloc(sizeof(vdl_arena));
    void *buffer_block = calloc((size_t) cap, sizeof(vdl_vp));
    vdlint_CheckAllocFailed(buffer_a);
    vdlint_CheckAllocFailed(buffer_block);

    vdl_arenap a = buffer_a;
    a->block     = buffer_block;
    a->len       = 0;
    a->cap       = cap;
    return a;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Reserve more memory for an arena.
/// @details When the arena needs to record more vectors than
/// its current cap, reallocation of the memory needs to be performed.
/// This function will likely reserve more memory than the requested capacity
/// due to efficiency.
/// @param a : (vdl_arena *const) An arena.
/// @param cap : (const int) Requested capacity.
/// @noExcept If any check fails, the program will abort.
#define vdlint_ReserveArena(...) vdlint_CallVoid(vdlint_ReserveArena_BT, __VA_ARGS__)
static inline void vdlint_ReserveArena_BT(vdl_bt bt, vdl_arena *const a, const int cap)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);

    int new_capacity = a->cap;
    if (new_capacity >= cap)
        return;
    while (new_capacity < cap)
    {
        static const size_t MEM_500KB = 500 * 1024;
        if ((size_t) new_capacity * sizeof(vdl_vp) < MEM_500KB)
            new_capacity = new_capacity * 2 + 8;
        else
            new_capacity += (int) (MEM_500KB / sizeof(vdl_vp));
    }

    void *buffer = realloc(a->block, (size_t) new_capacity * sizeof(vdl_vp));
    vdlint_CheckAllocFailed(buffer);

    a->block = buffer;
    a->cap   = new_capacity;
    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Release some memory of an arena.
/// @details If more than enough memory has been allocated for an arena, some
/// memory can be returned to the system.
/// @param a : (vdl_arena *const) An arena.
/// @noExcept If any check fails, the program will abort.
#define vdlint_ShrinkArena(...) vdlint_CallVoid(vdlint_ShrinkArena_BT, __VA_ARGS__)
static inline void vdlint_ShrinkArena_BT(vdl_bt bt, vdl_arena *const a)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);
    vdlint_CheckNullPointer(a->block);

    const int size_times_five = a->len * 5;
    const int size_plus_5MB   = a->len + 700000;
    int smaller_cap           = size_times_five > size_plus_5MB ? size_plus_5MB : size_times_five;
    if (smaller_cap < VDL_GC_INIT_CAPACITY)
        smaller_cap = VDL_GC_INIT_CAPACITY;
    if (a->cap <= smaller_cap)
        return;
    void *buffer = realloc(a->block, (size_t) smaller_cap * sizeof(vdl_vp));
    vdlint_CheckAllocFailed(buffer);

    a->block = buffer;
    a->cap   = smaller_cap;
    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Clean an arena.
/// @details Remove all records from an arena. The recorded vectors will also be
/// deallocated if `free_content == 1`.
/// @param a : (vdl_arena *const) An arena.
/// @param free_content : (const int) Whether to free the memory of the recorded vectors.
/// @noExcept If any check fails, the program will abort.
#define vdlint_CleanArena(...) vdlint_CallVoid(vdlint_CleanArena_BT, __VA_ARGS__)
static inline void vdlint_CleanArena_BT(vdl_bt bt, vdl_arena *const a, const int free_content)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);
    vdlint_CheckNullPointer(a->block);

    if (free_content == 1)
        vdlint_for_i(a->len)
        {
            vdl_vp v = a->block[i];
            vdlint_CheckNullPointer(v);
            vdlint_CheckNullPointer(vdlint_Dat(v));
            free(vdlint_Dat(v));
            free(v);
        }
    a->len = 0;
    free(a->block);

    void *buffer = calloc((size_t) VDL_GC_INIT_CAPACITY, sizeof(vdl_vp));
    vdlint_CheckAllocFailed(buffer);

    a->block = buffer;
    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Delete an arena.
/// @details Deallocate memory for an arena. The recorded vectors will also be
/// deallocated if `free_content == 1`.
/// @param a : (vdl_arena *const) An arena.
/// @param free_content : (const int) Whether to free the memory of the recorded vectors.
/// @noExcept If any check fails, the program will abort.
#define vdlint_DelArena(...) vdlint_CallVoid(vdlint_DelArena_BT, __VA_ARGS__)
static inline void vdlint_DelArena_BT(vdl_bt bt, vdl_arena *const a, const int free_content)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);
    vdlint_CheckNullPointer(a->block);

    if (free_content == 1)
        vdlint_for_i(a->len)
        {
            vdl_vp v = a->block[i];
            vdlint_CheckNullPointer(v);
            vdlint_CheckNullPointer(vdlint_Dat(v));
            free(vdlint_Dat(v));
            free(v);
        }

    free(a->block);
    free(a);
    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Get the memory usage of all the recorded vectors.
/// @param a : (const vdl_arena *const) An arena.
/// @return (size_t) The memory usage.
/// @noExcept If any check fails, the program will abort.
#define vdlint_SizeOfArena(...) vdlint_Call(vdlint_SizeOfArena_BT, size_t, __VA_ARGS__)
static inline size_t vdlint_SizeOfArena_BT(vdl_bt bt, const vdl_arena *const a)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);
    vdlint_CheckNullPointer(a->block);

    size_t memory_usage = 0;
    vdlint_for_i(a->len)
    {
        vdl_vp v = vdlint_VpArray(a->block)[i];
        vdlint_CheckNullPointer(v);
        memory_usage += vdl_SizeOfVec(v);
    }

    return memory_usage;

VDL_EXCEPTION:
    return 0;
}

/// @description Print an arena.
/// @param a : (const vdl_arena *const) An arena.
/// @noExcept If any check fails, the program will abort.
#define vdl_PrintArena(...) vdlint_CallVoid(vdl_PrintArena_BT, __VA_ARGS__)
static inline void vdl_PrintArena_BT(vdl_bt bt, const vdl_arena *const a)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);

    printf("Arena summary:\n[cap = %d, len = %d, memory usage = %zu]\n",
           a->cap,
           a->len,
           vdlint_SizeOfArena(a));

    vdlint_for_i(a->len) printf("\tObject %d <%p>\n", i, (void *) a->block[i]);
    printf("\n");
    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/*-----------------------------------------------------------------------------
 |  Record management of vdl_arena
 ----------------------------------------------------------------------------*/

/// @description Fina a vector from an arena.
/// @param a : (const vdl_arena *const) An arena.
/// @param v : (const vdl_vec *const) A vector.
/// @return (int) The index of the vector recorded in the arena. If not
/// found, -1 will be returned.
/// @noExcept If any check fails, the program will abort.
#define vdlint_ArenaFind(...) vdlint_Call(vdlint_ArenaFind_BT, int, __VA_ARGS__)
static inline int vdlint_ArenaFind_BT(vdl_bt bt, const vdl_arena *const a, const vdl_vec *const v)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);
    vdlint_CheckNullPointer(a->block);
    vdlint_CheckNullPointer(v);

    vdlint_for_i(a->len) if (a->block[i] == v) return i;
    return -1;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Record a vector by an arena.
/// @details If the vector is already being recorded by the arena,
/// then this function will do nothing.
/// @param a : (vdl_arena *const) An arena.
/// @param v : (vdl_vec *const) A vector.
/// @noExcept If any check fails, the program will abort.
#define vdlint_ArenaRecord(...) vdlint_CallVoid(vdlint_ArenaRecord_BT, __VA_ARGS__)
static inline void vdlint_ArenaRecord_BT(vdl_bt bt, vdl_arena *const a, vdl_vec *const v)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);
    vdlint_CheckNullPointer(a->block);
    vdlint_CheckNullPointer(v);

    if (vdlint_ArenaFind(a, v) != -1)
        return;

    vdlint_ReserveArena(a, a->len + 1);
    a->block[a->len] = v;
    a->len++;
    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Untrack a vector by an arena.
/// @details Remove a record from an arena. If `free_content == 1`, the
/// memory of the recorded vector will also be deallocated. If the vector
/// has not been recorded by the arena, then this function will do nothing.
/// @param a : (vdl_arena *const) An arena.
/// @param v : (vdl_vec *const) A vector.
/// @param free_content : (const int) Whether to free the memory of the recorded vectors.
/// @noExcept If any check fails, the program will abort.
#define vdlint_ArenaUntrack(...) vdlint_CallVoid(vdlint_ArenaUntrack_BT, __VA_ARGS__)
static inline void vdlint_ArenaUntrack_BT(vdl_bt bt, vdl_arena *const a, vdl_vec *const v, const int free_content)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);
    vdlint_CheckNullPointer(a->block);
    vdlint_CheckNullPointer(v);
    vdlint_CheckNullPointer(vdlint_Dat(v));

    const int idx = vdlint_ArenaFind(a, v);
    if (idx == -1)
        return;

    if (free_content == 1)
    {
        free(vdlint_Dat(v));
        free(v);
    }

    if (a->len - idx - 1 > 0)
        memmove(a->block + idx, a->block + idx + 1, (size_t) (a->len - idx - 1) * sizeof(vdl_vp));
    a->len--;
    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Untrack a vector by an arena.
/// @details Remove a record from an vdl_arena. If `free_content == 1`, the
/// memory of the recorded vector will also be deallocated.
/// @param a : (vdl_arena *const) An arena.
/// @param idx : (const int) Index of a vector in the arena.
/// @param free_content : (const int) Whether to free the memory of the recorded vectors.
/// @noExcept If any check fails, the program will abort.
#define vdlint_ArenaUntrackByIndex(...) vdlint_CallVoid(vdlint_ArenaUntrackByIndex_BT, __VA_ARGS__)
static inline void vdlint_ArenaUntrackByIndex_BT(vdl_bt bt, vdl_arena *const a, const int idx, const int free_content)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckNullPointer(a);
    vdlint_CheckNullPointer(a->block);
    vdlint_CheckIndexOutOfBound(a, idx);

    vdl_vp v = a->block[idx];
    vdlint_CheckNullPointer(v);
    vdlint_CheckNullPointer(vdlint_Dat(v));

    if (free_content == 1)
    {
        free(vdlint_Dat(v));
        free(v);
    }

    if (a->len - idx - 1 > 0)
        memmove(a->block + idx, a->block + idx + 1, (size_t) (a->len - idx - 1) * sizeof(vdl_vp));
    a->len--;
    return;

VDL_EXCEPTION:
    vdl_Abort();
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
/// @noExcept If any check fails, the program will abort.
#define vdlint_InitGC() vdlint_CallVoid(vdlint_InitGC_BT)
static inline void vdlint_InitGC_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckInconsistentGC();

    if (VDLINT_GARENA != NULL)
        return;

    VDLINT_GARENA      = vdlint_NewArena(VDL_GC_INIT_CAPACITY);
    VDLINT_GREACHABLE  = vdlint_NewArena(VDL_GC_INIT_CAPACITY);
    VDLINT_GDREACHABLE = vdlint_NewArena(VDL_GC_INIT_CAPACITY);
    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Check the health status of the garbage collector.
/// @details The garbage collector is in ill-formed if it is uninitialized or
/// initialized incorrectly.
#define vdlint_CheckGCHealth()         \
    do {                               \
        vdlint_CheckInconsistentGC();  \
        vdlint_CheckUninitializedGC(); \
    } while (0)

/// @description Record a vector by the global arena.
/// @details If the vector is already in the global arena, this function
/// will do nothing.
/// @param v : (vdl_vec *const) A vector.
/// @noExcept If any check fails, the program will abort.
#define vdlint_GCRecord(...) vdlint_CallVoid(vdlint_GCRecord_BT, __VA_ARGS__)
static inline void vdlint_GCRecord_BT(vdl_bt bt, vdl_vec *const v)
{
    vdl_PushBacktrace(bt);

    if (VDLINT_GARENA == NULL)
        vdlint_InitGC();
    vdlint_ArenaRecord(VDLINT_GARENA, v);

    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Untrack a vector by the global arena.
/// @details Since all vectors needs to be tracked by the global arena,
/// untrack a vector also means deleting it. If the vector is not in the
/// global arena, this function will do nothing.
/// @param v : (vdl_vec *const) A vector.
/// @noExcept If any check fails, the program will abort.
#define vdlint_GCUntrack(...) vdlint_Call(vdlint_GCUntrack_BT, __VA_ARGS__)
static inline void vdlint_GCUntrack_BT(vdl_bt bt, vdl_vec *const v)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckGCHealth();
    vdlint_ArenaUntrack(VDLINT_GARENA, v, 1);

    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Untrack all vectors by the global arena.
/// @details Since all vectors needs to be tracked by the global arena,
/// untrack a vector also means deleting it. The global reachable arena and
/// the global directly reachable arena will also be reset.
/// @noExcept If any check fails, the program will abort.
#define vdlint_GCUntrackAll() vdlint_CallVoid(vdlint_GCUntrackAll_BT)
static inline void vdlint_GCUntrackAll_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckGCHealth();

    vdlint_CleanArena(VDLINT_GARENA, 1);
    vdlint_CleanArena(VDLINT_GREACHABLE, 0);
    vdlint_CleanArena(VDLINT_GDREACHABLE, 0);

    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Declare a vector to be directly reachable and track by the directly reachable arena.
/// @details If the vector is already in the global
/// directly reachable arena, this function will do nothing.
/// @param v : (vdl_vec*) A vector.
/// @noExcept If any check fails, the program will abort.
#define vdl_GCDirectlyReachable(...) vdlint_CallVoid(vdl_GCDirectlyReachable_BT, __VA_ARGS__)
static inline void vdl_GCDirectlyReachable_BT(vdl_bt bt, vdl_vec *const v)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckGCHealth();

    vdlint_ArenaRecord(VDLINT_GDREACHABLE, v);

    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Declare a vector to be directly unreachable and untrack by the global directly reachable arena.
/// @details If the vector is not in the global directly unreachable
/// arena, this function will do nothing.
/// @param v : (vdl_vec*) A vector.
/// @noExcept If any check fails, the program will abort.
#define vdl_GCDirectlyUnreachable(...) vdlint_CallVoid(vdl_GCDirectlyUnreachable_BT, __VA_ARGS__)
static inline void vdl_GCDirectlyUnreachable_BT(vdl_bt bt, vdl_vec *const v)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckGCHealth();

    vdlint_ArenaUntrack(VDLINT_GDREACHABLE, v, 0);

    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Update the global reachable vdl_arena based on the global directly reachable arena.
/// @details A breadth-first search will be performed to visit all reachable objects.
/// @noExcept If any check fails, the program will abort.
#define vdlint_GCUpdateReachable() vdlint_CallVoid(vdlint_GCUpdateReachable_BT)
static inline void vdlint_GCUpdateReachable_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckGCHealth();
    vdlint_CleanArena(VDLINT_GREACHABLE, 0);

    if (VDLINT_GDREACHABLE->len == 0)
        return;

    vdlint_for_i(VDLINT_GDREACHABLE->len) vdlint_ArenaRecord(VDLINT_GREACHABLE, VDLINT_GDREACHABLE->block[i]);

    int head_idx = 0;
    int tail_idx = VDLINT_GREACHABLE->len - 1;
    while (head_idx <= tail_idx)
    {
        vdl_vp head = VDLINT_GREACHABLE->block[head_idx];
        vdlint_CheckNullPointer(head);

        if (head->type == VDL_TYPE_VP)
        {
            vdlint_CheckNullPointer(head->dat);
            vdlint_for_i(head->len) vdlint_ArenaRecord(VDLINT_GREACHABLE, vdlint_VpArray(head->dat)[i]);
            tail_idx = VDLINT_GREACHABLE->len - 1;
        }

        head_idx++;
    }

    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Run the garbage collector based on the global directly reachable arena.
/// @details An object is reachable if it is directly reachable or it is contained by
/// any reachable objects.
/// @noExcept If any check fails, the program will abort.
#define vdl_GCCleanUp() vdlint_CallVoid(vdl_GCCleanUp_BT)
static inline void vdl_GCCleanUp_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckGCHealth();

    vdlint_GCUpdateReachable();
    int head_idx = 0;
    while (head_idx <= VDLINT_GARENA->len - 1)
    {
        // TODO: The current implementation is inefficient
        if (vdlint_ArenaFind(VDLINT_GREACHABLE, VDLINT_GARENA->block[head_idx]) == -1)
            vdlint_ArenaUntrackByIndex(VDLINT_GARENA, head_idx, 1);
        else
            head_idx++;
    }

    return;

VDL_EXCEPTION:
    vdl_Abort();
}

/// @description Kill the garbage collector.
/// @details Since all vectors needs to be tracked by the global vdl_arena,
/// kill the garbage collector means deleting them. The global reachable vdl_arena and
/// the global directly reachable vdl_arena will also be killed.
/// @noExcept If any check fails, the program will abort.
#define vdl_gc_Kill() vdl_bt_callvoid(vdl_gc_Kill_BT)
static inline void vdl_gc_Kill_BT(vdl_bt bt)
{
    vdl_PushBacktrace(bt);
    vdlint_CheckGCHealth();

    vdl_GCCleanUp();

    vdlint_DelArena(VDLINT_GARENA, 1);
    vdlint_DelArena(VDLINT_GREACHABLE, 0);
    vdlint_DelArena(VDLINT_GDREACHABLE, 0);

    return;

VDL_EXCEPTION:
    vdl_Abort();
}

#endif//VDL_VDLGC_H
