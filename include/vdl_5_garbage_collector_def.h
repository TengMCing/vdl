//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_5_GARBAGE_COLLECTOR_DEF_H
#define VDL_VDL_5_GARBAGE_COLLECTOR_DEF_H

/*-----------------------------------------------------------------------------
 |  Memory bookkeeping
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Malloc and calloc
 ----------------------------------------------------------------------------*/

static inline void *vdl_Malloc_BT(const size_t bytes, const int register_object)
{
    void *object = malloc(bytes);
    vdl_CheckFailedAllocation(object);
    if (register_object)
        vdl_ExceptionRegisterCleanUp(object, free);
    return object;
}

static inline void *vdl_Calloc_BT(const size_t count, const size_t bytes, const int register_object)
{
    void *object = calloc(count, bytes);
    vdl_CheckFailedAllocation(object);
    if (register_object)
        vdl_ExceptionRegisterCleanUp(object, free);
    return object;
}

/*-----------------------------------------------------------------------------
 |  Vector table operations
 ----------------------------------------------------------------------------*/

static inline VDL_VECTOR_TABLE_P vdl_NewVectorTable_BT(void)
{
    VDL_VECTOR_TABLE_T *vector_table = vdl_Malloc(sizeof(VDL_VECTOR_TABLE_T), 1);
    void *data_container             = vdl_Malloc(VDL_VECTOR_TABLE_INIT_CAPACITY * sizeof(VDL_VECTOR_P), 1);

    // Construct the vector table
    vector_table->Length   = 0;
    vector_table->Capacity = 0;
    vector_table->Data     = data_container;

    vdl_ExceptionDeregisterCleanUp(local_vector_table);
    vdl_ExceptionDeregisterCleanUp(local_data_container);

    return vector_table;
}

static inline void vdl_ShrinkVectorTable_BT(VDL_VECTOR_TABLE_T *const vector_table)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);

    // Decide the target size
    const int size_times_five = vector_table->Length * 5;
    const int size_plus_5MB   = vector_table->Length + 700000;
    int smaller_cap           = size_times_five > size_plus_5MB ? size_plus_5MB : size_times_five;
    if (smaller_cap < VDL_VECTOR_TABLE_INIT_CAPACITY)
        smaller_cap = VDL_VECTOR_TABLE_INIT_CAPACITY;
    if (vector_table->Capacity <= smaller_cap)
        return;

    size_t target_size = (size_t) smaller_cap * sizeof(VDL_VECTOR_P);

    // Reallocate memory for the data container
    void *buffer = vdl_Malloc(target_size, 1);
    memcpy(buffer, vector_table->Data, target_size);
    vdl_Free(vector_table->Data);
    vector_table->Data     = buffer;
    vector_table->Capacity = smaller_cap;

    vdl_ExceptionDeregisterCleanUp(buffer);
}

static inline void vdl_DeleteVectorTable_BT(VDL_VECTOR_T *const vector_table, const int free_content)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);

    VDL_VECTOR_CONST_POINTER_ARRAY vectors = vector_table->Data;

    // Perform the check before deallocation to avoid partial free
    if (free_content)
    {
        vdl_for_i(vector_table->Length) vdl_CheckNullPointer(vectors[i]);
    }

    // Free the vectors
    if (free_content)
    {
        vdl_for_i(vector_table->Length)
        {
            vdl_Free(vectors[i]->Data);
            vdl_Free(vectors[i]);
        }
    }

    // Free the vector table
    vdl_Free(vector_table->Data);
    vdl_Free(vector_table);
}

static inline void vdl_ReserveForVectorTable_BT(VDL_VECTOR_T *const vector_table, const int capacity)
{

    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);
    vdl_Expect(capacity <= VDL_VECTOR_TABLE_MAX_CAPACITY,
               VDL_EXCEPTION_EXCEED_VECTOR_TABLE_LIMIT,
               "Request vector table capacity [%d] larger than the limit [%d]!",
               capacity,
               VDL_VECTOR_TABLE_MAX_CAPACITY);

    // Do nothing if there is enough space
    if (vector_table->Capacity >= capacity)
        return;

    // Decide the new capacity
    int new_capacity = vector_table->Capacity;
    while (new_capacity < capacity)
    {
        static const size_t MEM_500KB = 500 * 1024;
        if ((size_t) new_capacity * sizeof(VDL_VECTOR_P) < MEM_500KB)
            new_capacity = new_capacity * 2 + 8;
        else
            new_capacity += (int) (MEM_500KB / sizeof(VDL_VECTOR_P));
    }
    if (new_capacity > VDL_VECTOR_TABLE_MAX_CAPACITY)
        new_capacity = VDL_VECTOR_TABLE_MAX_CAPACITY;

    size_t target_size = (size_t) new_capacity * sizeof(VDL_VECTOR_P);

    // Attempt to reallocate memory
    void *buffer           = vdl_Malloc(target_size, 1);
    vector_table->Data     = buffer;
    vector_table->Capacity = new_capacity;

    vdl_ExceptionDeregisterCleanUp(buffer);
}

static inline size_t vdl_SizeOfVectorTable_BT(const VDL_VECTOR_T *const vector_table)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);

    size_t memory_usage                          = 0;
    VDL_CONST_VECTOR_CONST_POINTER_ARRAY vectors = vector_table->Data;
    vdl_for_i(vector_table->Length)
    {
        vdl_CheckNullPointer(vectors[i]);
        memory_usage += vdl_SizeOfVector(vectors[i]);
    }
    return memory_usage;
}

static inline void vdl_PrintVectorTable_BT(const VDL_VECTOR_T *const vector_table)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);

    printf("Vector table summary:\n[Capacity = %d, Length = %d, Memory usage = %zu]\n",
           vector_table->Capacity,
           vector_table->Length,
           vdl_SizeOfVectorTable(vector_table));

    VDL_CONST_VECTOR_CONST_POINTER_ARRAY vectors = vector_table->Data;
    vdl_for_i(vector_table->Length)
    {
        printf("\tObject %d <%p>\n", i, (const void *) vectors[i]);
    }
    puts("");
}

static inline int vdl_FindInVectorTable_BT(const VDL_VECTOR_T *const vector_table, const VDL_VECTOR_T *const v)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);

    VDL_CONST_VECTOR_CONST_POINTER_ARRAY vectors = vector_table->Data;
    vdl_for_i(vector_table->Length)
    {
        if (vectors[i] == v)
            return i;
    }
    return -1;
}

static inline void vdl_VectorTableRecord_BT(VDL_VECTOR_T *const vector_table, const VDL_VECTOR_T *const v)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);
    vdl_CheckNullPointer(v);

    // Do nothing if it is already recorded
    if (vdl_FindInVectorTable(vector_table, v) != -1)
        return;

    // Reserve enough space
    vdl_ReserveForVectorTable(vector_table, vector_table->Length + 1);

    // Push the vector to the end of the table
    vdl_UnsafeSetVectorPointer(vector_table, vector_table->Length, v);
    vector_table->Length++;
}

static inline void vdl_VectorTableUntrack_BT(VDL_VECTOR_T *const vector_table, VDL_VECTOR_T *const v, const int free_content)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);
    vdl_CheckNullPointer(v);

    // Check if the vector is already recorded
    const int index = vdl_FindInVectorTable(vector_table, v);
    if (index == -1)
        return;

    // Delete the vector
    if (free_content == 1)
    {
        vdl_Free(v->Data);
        vdl_Free(v);
    }

    // Pop the vector from the table
    if (vector_table->Length - index - 1 > 0)
    {
        void *dst       = vdl_UnsafeAddressOf(vector_table, index);
        const void *scr = vdl_UnsafeAddressOf(vector_table, index + 1);
        size_t bytes    = (size_t) (vector_table->Length - index - 1) * sizeof(VDL_VECTOR_P);
        memmove(dst, scr, bytes);
    }
    vector_table->Length--;
}

static inline void vdl_VectorTableUntrackByIndex_BT(VDL_VECTOR_T *const vector_table, const int index, const int free_content)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);
    vdl_CheckIndexOutOfBound(vector_table, index);

    VDL_VECTOR_P v = vdl_UnsafeVectorPointerAt(vector_table, index);
    if (free_content)
    {
        free(v->Data);
        free(v);
    }

    if (vector_table->Length - index - 1 > 0)
        memmove(vdl_UnsafeAddressOf(vector_table, index),
                vdl_UnsafeAddressOf(vector_table, index + 1),
                (size_t) (vector_table->Length - index - 1) * sizeof(VDL_VECTOR_P));
    vector_table->Length--;
}

#endif//VDL_VDL_5_GARBAGE_COLLECTOR_DEF_H
