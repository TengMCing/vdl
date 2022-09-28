//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VECMEM_H
#define VDL_VECMEM_H

#include "vecdef.h"
#include "vecerror.h"
#include "vecgc.h"

#define vec_AddressOf(v, i) (((char *) (v)->data) + vec_GetTypeSize((v)->type) * (size_t) (i))

static inline void vec_HealthCheck(const vec *const v)
{
    assert_NullPointer(v);
    assert_UnknownType(v->type);
}

static inline void *vec_At(const vec *const v, const int i)
{
    vec_HealthCheck(v);
    assert_IndexOutOfBound(v, i);
    return vec_AddressOf(v, i);
}

static inline vp vec_New(const VECTYPE type, const int capacity)
{
    // Safety check
    assert_UnknownType(type);
    assert_ZeroCapacity(capacity);

    // Allocate memory for the container
    vp v = malloc(sizeof(vec));
    v->capacity = capacity;
    v->type = type;
    v->size = 0;

    // Allocate memory for the array
    v->data = calloc((size_t) capacity, vec_GetTypeSize(type));

    // Record the vector by the global arena
    vec_GCRecord(v);

    return v;
}

static inline void vec_Reserve(vec *const v, const int capacity)
{
    // Safety check
    vec_HealthCheck(v);
    assert_ZeroCapacity(capacity);

    // Do nothing when it is enough space
    if (v->capacity >= capacity)
        return;

    // Allocate enough space
    // Switch to arithmetic growth policy after 500KB
    while (v->capacity < capacity)
    {
        static const size_t mem_500KB = 500 * 1024;
        if ((size_t) v->capacity * vec_GetTypeSize(v->type) < mem_500KB)
            v->capacity = v->capacity * 2 + 8;
        else
            v->capacity = v->capacity + (int) (mem_500KB / vec_GetTypeSize(v->type));
    }

    v->data = realloc(v->data, (size_t) v->capacity * vec_GetTypeSize(v->type));
}

static inline void vec_Set(vec *const v, const int i, void *const object, const int num_object)
{
    // Safety check
    vec_HealthCheck(v);
    assert_NullPointer(object);
    assert_ZeroObjects(num_object);
    assert_IndexOutOfBound(v, i);
    assert_IndexOutOfBound(v, i + num_object - 1);

    // Copy in the memory
    memmove(vec_AddressOf(v, i), object, (size_t) num_object * vec_GetTypeSize(v->type));
}

static inline void vec_Append(vec *const v, void *const object, const int num_object)
{
    // Safety check
    vec_HealthCheck(v);
    assert_NullPointer(object);
    assert_ZeroObjects(num_object);

    vec_Reserve(v, v->size + num_object);
    v->size += num_object;
    vec_Set(v, v->size - num_object, object, num_object);
}

static inline void vec_Remove(vec *const v, const int i, const int num_object)
{
    // Safety check
    vec_HealthCheck(v);
    assert_IndexOutOfBound(v, i);
    assert_IndexOutOfBound(v, i + num_object - 1);
    assert_ZeroObjects(num_object);

    // Only reduce the size if all the items are at the end of the array
    if (i == v->size - num_object)
    {
        v->size -= num_object;
        return;
    }

    // Copy memory to index i
    memmove(vec_AddressOf(v, i), vec_AddressOf(v, i + num_object), (size_t) (v->size - i - num_object) * vec_GetTypeSize(v->type));
    v->size -= num_object;
}

#define vec_Pop(v, i) vec_Remove(v, i, 1)

static inline void vec_Concatenate(vec *const v1, vec *const v2)
{
    // Safety check
    vec_HealthCheck(v1);
    vec_HealthCheck(v2);
    assert_IncompatibleType(v1->type, v2->type);

    vec_Append(v1, v2->data, v2->size);
}

static inline void vec_Insert(vec *const v, const int i, void *const object, const int num_object)
{
    // Safety check
    vec_HealthCheck(v);
    assert_IndexOutOfBound(v, i);
    assert_NullPointer(object);
    assert_ZeroObjects(num_object);

    // Reserve enough space
    vec_Reserve(v, v->size + num_object);
    v->size = v->size + num_object;

    // Move the old content to the end
    vec_Set(v, i + num_object, vec_AddressOf(v, i), num_object);
    // Copy in the new content
    vec_Set(v, i, object, num_object);
}

#define TEMPLATE_vec_T_Find(T)                                        \
    static inline int vec_##T##_Find(const vec *const v, T object)    \
    {                                                                 \
        vec_HealthCheck(v);                                           \
        for_i(v->size) if (T##_array(v->data)[i] == object) return i; \
        return -1;                                                    \
    }

TEMPLATE_vec_T_Find(char);
TEMPLATE_vec_T_Find(int);
TEMPLATE_vec_T_Find(double);
TEMPLATE_vec_T_Find(vp);

static inline int vec_Find(const vec *const v, void *const object)
{
    vec_HealthCheck(v);
    assert_NullPointer(object);

    switch (v->type)
    {
    case VEC_CHAR:
        return vec_char_Find(v, char_array(object)[0]);
    case VEC_INT:
        return vec_int_Find(v, int_array(object)[0]);
    case VEC_DOUBLE:
        return vec_double_Find(v, double_array(object)[0]);
    case VEC_VP:
        return vec_vp_Find(v, vp_array(object)[0]);
    }
}

#define TEMPLATE_vec_T_Print(T, f)                                          \
    static inline void vec_##T##_Print(const vec *const v, const char *end) \
    {                                                                       \
        vec_HealthCheck(v);                                                 \
        assert_IncompatibleType(v->type, lower_vec_##T);                    \
                                                                            \
        if (end == NULL)                                                    \
            end = "\n";                                                     \
        printf("[");                                                        \
        for_i(v->size)                                                      \
        {                                                                   \
            printf(f, T##_array(v->data)[i]);                               \
            if (i < v->size - 1)                                            \
                printf(", ");                                               \
        }                                                                   \
        printf("]%s", end);                                                 \
    }

TEMPLATE_vec_T_Print(char, "'%c'");
TEMPLATE_vec_T_Print(int, "%d");
TEMPLATE_vec_T_Print(double, "%f");

static inline void vec_PrintRecursive(const vec *v, const char *end, vec *const guard)
{
    // Safety check
    vec_HealthCheck(v);
    vec_HealthCheck(guard);
    assert_IncompatibleType(VEC_VP, guard->type);

    // The next level should not go to this vector again
    vec_Append(guard, &v, 1);

    if (end == NULL)
        end = "\n";
    switch (v->type)
    {
    case VEC_CHAR:
        vec_char_Print(v, "");
        break;
    case VEC_INT:
        vec_int_Print(v, "");
        break;
    case VEC_DOUBLE:
        vec_double_Print(v, "");
        break;
    case VEC_VP:
        printf("[");
        for_i(v->size)
        {
            const int idx = vec_Find(guard, vp_array(v->data)[i]);
            if (idx == -1)
                vec_PrintRecursive(vp_array(v->data)[i], "", guard);
            else
                printf("...");
            if (i < v->size - 1)
                printf(", ");
        }
        printf("]%s", end);
        break;
    }

    // Release the lock
    vec_Pop(guard, guard->size - 1);
}

static inline void vec_Print(const vec *const v, const char *end)
{
    // A vector to avoid printing the cycling references
    vp guard = vec_New(VEC_VP, 4);
    vec_PrintRecursive(v, end, guard);

    // Though `guard` is unreachable from the caller, which means it will be garbage collected
    // It is still a good practice to manually clean it up at the end of the scope
    vec_GCUntrack(guard);
}

static inline vp vec_Copy(const vec *const v)
{
    // Safety check
    vec_HealthCheck(v);

    // A new vector will be allocated
    vp copy = vec_New(v->type, v->capacity);
    copy->size = v->size;

    // Only the first layer of the content will be copied
    vec_Set(copy, 0, v->data, v->size);
    return copy;
}

static inline vp vec_DeepCopyRecursive(const vec *v, vec *const guard)
{
    vec_Append(guard, &v, 1);
    
    vec_Pop(guard, guard->size - 1);
}

static inline vp vec_DeepCopy(const vec *const v)
{
    // Safety check
    vec_HealthCheck(v);

    // A vector to avoid copying the cycling references
    vp guard = vec_New(VEC_VP, 4);

    vp copy = vec_DeepCopyRecursive(v, guard);

    // Though `guard` is unreachable from the caller, which means it will be garbage collected
    // It is still a good practice to manually clean it up at the end of the scope
    vec_GCUntrack(guard);

    return copy;
}

#endif//VDL_VECMEM_H
