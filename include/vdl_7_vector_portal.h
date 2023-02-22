//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_7_VECTOR_PORTAL_H
#define VDL_VDL_7_VECTOR_PORTAL_H

#define vdl_FindChar(...) vdl_CallFunction(vdl_FindChar_BT, int, __VA_ARGS__)
static inline int vdl_FindChar_BT(VDL_VECTOR_T *const v, const char item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    VDL_CHAR_ARRAY data_array = v->Data;
    vdl_for_i(v->Length)
    {
        if (data_array[i] == item)
            return i;
    }
    return -1;
}

#define vdl_FindInt(...) vdl_CallFunction(vdl_FindInt_BT, int, __VA_ARGS__)
static inline int vdl_FindInt_BT(VDL_VECTOR_T *const v, const int item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    VDL_INT_ARRAY data_array = v->Data;
    vdl_for_i(v->Length)
    {
        if (data_array[i] == item)
            return i;
    }
    return -1;
}

#define vdl_FindDouble(...) vdl_CallFunction(vdl_FindDouble_BT, int, __VA_ARGS__)
static inline int vdl_FindDouble_BT(VDL_VECTOR_T *const v, const double item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    VDL_DOUBLE_ARRAY data_array = v->Data;
    vdl_for_i(v->Length)
    {
        if (data_array[i] == item)
            return i;
    }
    return -1;
}

#define vdl_FindVectorPointer(...) vdl_CallFunction(vdl_FindVectorPointer_BT, int, __VA_ARGS__)
static inline int vdl_FindVectorPointer_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    VDL_VECTOR_POINTER_ARRAY data_array = v->Data;
    vdl_for_i(v->Length)
    {
        if (data_array[i] == item)
            return i;
    }
    return -1;
}


#define vdl_AppendChar(...) vdl_CallVoidFunction(vdl_AppendChar_BT, __VA_ARGS__)
static inline void vdl_AppendChar_BT(VDL_VECTOR_T *const v, const char item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_Reserve(v, v->Length + 1);
    v->Length++;
    vdl_UnsafeSetChar(v, v->Length, item);
}

#define vdl_AppendInt(...) vdl_CallVoidFunction(vdl_AppendInt_BT, __VA_ARGS__)
static inline void vdl_AppendInt_BT(VDL_VECTOR_T *const v, const int item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_Reserve(v, v->Length + 1);
    v->Length++;
    vdl_UnsafeSetInt(v, v->Length, item);
}

#define vdl_AppendDouble(...) vdl_CallVoidFunction(vdl_AppendDouble_BT, __VA_ARGS__)
static inline void vdl_AppendDouble_BT(VDL_VECTOR_T *const v, const double item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_Reserve(v, v->Length + 1);
    v->Length++;
    vdl_UnsafeSetDouble(v, v->Length, item);
}

#define vdl_AppendVectorPointer(...) vdl_CallVoidFunction(vdl_AppendVectorPointer_BT, __VA_ARGS__)
static inline void vdl_AppendVectorPointer_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_Reserve(v, v->Length + 1);
    v->Length++;
    vdl_UnsafeSetVectorPointer(v, v->Length, item);
}

#define vdl_Append(...) vdl_CallVoidFunction(vdl_Append_BT, __VA_ARGS__)
static inline void vdl_Append_BT(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const v2)
{
    vdl_CheckNullPointer(v1);
    vdl_CheckType(v1->Type, VDL_TYPE_VECTOR_P);

    vdl_Reserve(v1, v1->Length + 1);
    v1->Length++;
    vdl_UnsafeSetVectorPointer(v1, v1->Length, v2);
}

#define vdl_ExtendByArray(...) vdl_CallVoidFunction(vdl_ExtendByArray_BT, __VA_ARGS__)
static inline void vdl_ExtendByArray_BT(VDL_VECTOR_T *const v, const void *const item_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNumberOfItems(number);

    vdl_Reserve(v, v->Length + number);
    vdl_UnsafeSetByArrayAndMemmove(v, v->Length, item_pointer, number);
    v->Length += number;
}

#define vdl_Extend(...) vdl_CallVoidFunction(vdl_Extend_BT, __VA_ARGS__)
static inline void vdl_Extend_BT(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const v2)
{
    vdl_CheckNullPointer(v1);
    vdl_CheckNullPointer(v2);
    vdl_CheckNullPointer(v2->Data);
    vdl_CheckType(v1->Type, v2->Type);

    vdl_Reserve(v1, v1->Length + v2->Length);
    vdl_UnsafeSetByArrayAndMemcpy(v1, v1->Length, v2->Data, v2->Length);
    v1->Length += v2->Length;
}

#define vdl_Copy(...) vdl_CallFunction(vdl_Copy_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_Copy_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);

    VDL_VECTOR_P result = vdl_NewEmpty(v->Type, v->Capacity);
    memcpy(result->Data, v->Data, vdl_SizeOfData(v));
    result->Length = v->Length;
    return result;
}

static inline void vdl_InsertChar_BT(VDL_VECTOR_T *const v, const int i, const char item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_Reserve(v, v->Length + 1);
    v->Length++;
    vdl_UnsafeSetChar(v, i, item);
}

static inline void vdl_InsertInt_BT(VDL_VECTOR_T *const v, const int i, const int item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    vdl_Reserve(v, v->Length + 1);
    v->Length++;
    vdl_UnsafeSetInt(v, i, item);
}

static inline void vdl_InsertDouble_BT(VDL_VECTOR_T *const v, const int i, const double item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    vdl_Reserve(v, v->Length + 1);
    v->Length++;
    vdl_UnsafeSetDouble(v, i, item);
}

static inline void vdl_InsertVectorPointer_BT(VDL_VECTOR_T *const v, const int i, VDL_VECTOR_T *const item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);

    vdl_Reserve(v, v->Length + 1);
    v->Length++;
    vdl_UnsafeSetVectorPointer(v, i, item);
}

static inline void vdl_InsertByArray_BT(VDL_VECTOR_T *const v, const int i, const void *item_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNullPointer(item_pointer);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckNumberOfItems(number);

    vdl_Reserve(v, v->Length + number);
    vdl_UnsafeSetByArrayAndMemcpy(v, i + number, vdl_UnsafeAddressOf(v, i), number);
    vdl_UnsafeSetByArrayAndMemmove(v, i, item_pointer, number);
}

// TODO: check storage mode of vector to decide whether a function can work on it

static inline void vdl_Remove(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const v2)
{
    vdl_CheckNullPointer(v1);
    vdl_CheckNullPointer(v1->Data);
    vdl_CheckNullPointer(v2);
    vdl_CheckNullPointer(v2->Data);
    vdl_CheckType(v2->Type, VDL_TYPE_INT);

    // Check the index provided in vector v2.
    VDL_INT_ARRAY index_array = v2->Data;
    vdl_for_i(v2->Length)
    {
        vdl_CheckIndexOutOfBound(v1, index_array[i]);
    }

    // Use a placeholder vector to store the result.
    VDL_VECTOR_P result = vdl_NewEmpty(v1->Type, v1->Length);

    switch (v1->Type)
    {
        case VDL_TYPE_CHAR:
        {
            VDL_CHAR_ARRAY data_array = v1->Data;
            vdl_for_i(v1->Length)
            {
                int keep_item = vdl_FindChar(v2, data_array[i]) == -1;
                if (keep_item)
                {
                    vdl_UnsafeSetChar(result, result->Length, data_array[i]);
                    result->Length++;
                }
            }
            break;
        }
        case VDL_TYPE_INT:
        {
            VDL_INT_ARRAY data_array = v1->Data;
            vdl_for_i(v1->Length)
            {
                int keep_item = vdl_FindInt(v2, data_array[i]) == -1;
                if (keep_item)
                {
                    vdl_UnsafeSetInt(result, result->Length, data_array[i]);
                    result->Length++;
                }
            }
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            VDL_DOUBLE_ARRAY data_array = v1->Data;
            vdl_for_i(v1->Length)
            {
                int keep_item = vdl_FindDouble(v2, data_array[i]) == -1;
                if (keep_item)
                {
                    vdl_UnsafeSetDouble(result, result->Length, data_array[i]);
                    result->Length++;
                }
            }
            break;
        }
        case VDL_TYPE_VECTOR_P:
        {
            VDL_VECTOR_POINTER_ARRAY data_array = v1->Data;
            vdl_for_i(v1->Length)
            {
                int keep_item = vdl_FindVectorPointer(v2, data_array[i]) == -1;
                if (keep_item)
                {
                    vdl_UnsafeSetVectorPointer(result, result->Length, data_array[i]);
                    result->Length++;
                }
            }
            break;
        }
    }

    // Copy the content from result back to the original vector.
    vdl_UnsafeSetByArrayAndMemcpy(v1, 0, result->Data, result->Length);
    v1->Length = result->Length;
}

static inline void vdl_RemoveByFirstAndLastIndex(VDL_VECTOR_T *const v, const int first, const int last)
{
}

static inline void vdl_Clear_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullPointer(v);
    v->Length = 0;
}

// TODO: reconsider the naming of the API

//
// #define vdl_FindVectorPointer(...) vdl_CallFunction(vdl_FindVectorPointer_BT, int, __VA_ARGS__)
// static inline int vdl_FindVectorPointer_BT(VDL_VECTOR_P v, VDL_VECTOR_P item)
// {
//     vdl_CheckNullPointer(v);
//     vdl_CheckNullPointer(v->Data);
//     vdl_CheckNullPointer(item);
//     vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);
//
//     VDL_VECTOR_POINTER_ARRAY vectors = v->Data;
//     vdl_for_i(v->Length)
//     {
//         if (vectors[i] == item) return i;
//     }
//     return -1;
// }
//
// static inline VDL_VECTOR_P vdl_DeepCopyRecursive_BT(VDL_VECTOR_P v, VDL_VECTOR_P travelled)
// {
//     vdl_CheckNullPointer(v);
//     const int index = vdl_FindVectorPointer(travelled, v);
//     if (index != -1)
//         return vdl_UnsafeVectorPointerAt(travelled, index);
//
//     VDL_VECTOR_P result = vdl_NewVector(v->Type, v->Capacity);
//     result->Length      = v->Length;
//     if (v->Type == VDL_TYPE_VECTOR_P)
//     {
//         VDL_VECTOR_POINTER_ARRAY result_vectors = result->Data;
//         VDL_VECTOR_POINTER_ARRAY source_vectors = v->Data;
//         vdl_for_i(result->Length)
//         {
//             result_vectors[i] = vdl_DeepCopyRecursive_BT(source_vectors[i]);
//         }
//     }
//     return result;
// }
//
// #define vdl_DeepCopy(...) vdl_CallFunction(vdl_DeepCopy_BT, VDL_VECTOR_P, __VA_ARGS__)
// static inline VDL_VECTOR_P vdl_DeepCopy_BT(VDL_VECTOR_P v)
// {
//     VDL_VECTOR_P result = vdl_NewVector(v->Type, v->Capacity);
//     result->Length      = v->Length;
//     if (v->Type == VDL_TYPE_VECTOR_P)
//     {
//         VDL_VECTOR_POINTER_ARRAY result_vectors = result->Data;
//         VDL_VECTOR_POINTER_ARRAY source_vectors = v->Data;
//         vdl_for_i(result->Length)
//         {
//             result_vectors[i] = vdl_DeepCopy(source_vectors[i]);
//         }
//     }
//     return result;
// }

#endif//VDL_VDL_7_VECTOR_PORTAL_H
