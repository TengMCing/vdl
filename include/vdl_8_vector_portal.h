//
// Created by Patrick Li on 21/2/2023.
//

#ifndef VDL_VDL_8_VECTOR_PORTAL_H
#define VDL_VDL_8_VECTOR_PORTAL_H

// TODO: check which function can accept empty vector

/*-----------------------------------------------------------------------------
 |  Set the vector data safely (Public API)
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 |  Set the vector data safely (Public API)
 ----------------------------------------------------------------------------*/

static inline void vdl_vector_Set_BT(VDL_VECTOR_P v1, VDL_VECTOR_P v2)
{
    vdl_CheckNullVectorAndNullContainer(v1);
    vdl_CheckNullVectorAndNullContainer(v2);

    vdl_CheckType(v1->Type, v2->Type);

    vdl_CheckIncompatibleLength(v2->Length, v1->Length);
    vdl_CheckZeroLength(v1->Length);


    switch (v1->Type)
    {
        case VDL_TYPE_CHAR:
        {
            VDL_CHAR_ARRAY v1_data_array = v1->Data;
            VDL_CHAR_ARRAY v2_data_array = v2->Data;
            if (v2->Length == 1)
            {
                vdl_for_i(v1->Length) v1_data_array[i] = v2_data_array[0];
            }
            else
            {
                vdl_vector_primitive_SetByArrayAndMemmove(v1, 0, v2->Data, v1->Length);
            }
            break;
        }
        case VDL_TYPE_INT:
        {
            VDL_INT_ARRAY v1_data_array = v1->Data;
            VDL_INT_ARRAY v2_data_array = v2->Data;
            if (v2->Length == 1)
            {
                vdl_for_i(v1->Length) v1_data_array[i] = v2_data_array[0];
            }
            else
            {
                vdl_vector_primitive_SetByArrayAndMemmove(v1, 0, v2->Data, v1->Length);
            }
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            VDL_DOUBLE_ARRAY v1_data_array = v1->Data;
            VDL_DOUBLE_ARRAY v2_data_array = v2->Data;
            if (v2->Length == 1)
            {
                vdl_for_i(v1->Length) v1_data_array[i] = v2_data_array[0];
            }
            else
            {
                vdl_vector_primitive_SetByArrayAndMemmove(v1, 0, v2->Data, v1->Length);
            }
            break;
        }
        case VDL_TYPE_VECTOR_POINTER:
        {
            VDL_VECTOR_POINTER_ARRAY v1_data_array = v1->Data;
            VDL_VECTOR_POINTER_ARRAY v2_data_array = v2->Data;
            if (v2->Length == 1)
            {
                vdl_for_i(v1->Length) v1_data_array[i] = v2_data_array[0];
            }
            else
            {
                vdl_vector_primitive_SetByArrayAndMemmove(v1, 0, v2->Data, v1->Length);
            }
            break;
        }
    }
}

static inline void vdl_vector_SetByIndex_BT(VDL_VECTOR_P v1, VDL_VECTOR_P i, VDL_VECTOR_P v2)
{
    vdl_CheckNullVectorAndNullContainer(v1);
    vdl_CheckNullVectorAndNullContainer(i);
    vdl_CheckNullVectorAndNullContainer(v2);

    vdl_CheckType(i->Type, VDL_TYPE_INT);
    vdl_CheckType(v1->Type, v2->Type);

    vdl_CheckIncompatibleLength(v2->Length, i->Length);

    // Check index out of bound
    VDL_INT_ARRAY index_array = i->Data;
    vdl_for_j(i->Length)
    {
        vdl_CheckIndexOutOfBound(v1, index_array[j]);
    }

    // Set the values
    switch (v1->Type)
    {
        case VDL_TYPE_CHAR:
        {

            VDL_CHAR_ARRAY v1_data_array = v1->Data;
            VDL_CHAR_ARRAY v2_data_array = v2->Data;
            if (v2->Length == 1)
            {
                vdl_for_j(i->Length) v1_data_array[index_array[j]] = v2_data_array[0];
            }
            else
            {
                vdl_for_j(i->Length) v1_data_array[index_array[j]] = v2_data_array[j];
            }
            break;
        }
        case VDL_TYPE_INT:
        {
            VDL_INT_ARRAY v1_data_array = v1->Data;
            VDL_INT_ARRAY v2_data_array = v2->Data;
            if (v2->Length == 1)
            {
                vdl_for_j(i->Length) v1_data_array[index_array[j]] = v2_data_array[0];
            }
            else
            {
                vdl_for_j(i->Length) v1_data_array[index_array[j]] = v2_data_array[j];
            }
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            VDL_DOUBLE_ARRAY v1_data_array = v1->Data;
            VDL_DOUBLE_ARRAY v2_data_array = v2->Data;
            if (v2->Length == 1)
            {
                vdl_for_j(i->Length) v1_data_array[index_array[j]] = v2_data_array[0];
            }
            else
            {
                vdl_for_j(i->Length) v1_data_array[index_array[j]] = v2_data_array[j];
            }
            break;
        }
        case VDL_TYPE_VECTOR_POINTER:
        {
            VDL_VECTOR_POINTER_ARRAY v1_data_array = v1->Data;
            VDL_VECTOR_POINTER_ARRAY v2_data_array = v2->Data;
            if (v2->Length == 1)
            {
                vdl_for_j(i->Length) v1_data_array[index_array[j]] = v2_data_array[0];
            }
            else
            {
                vdl_for_j(i->Length) v1_data_array[index_array[j]] = v2_data_array[j];
            }
            break;
        }
    }
}

/*-----------------------------------------------------------------------------
 |  Get an attribute
 ----------------------------------------------------------------------------*/


static inline int vdl_vector_primitive_GetAttributeIndex(VDL_VECTOR_T *const v, const char *const name, const int length)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(name);
    vdl_CheckZeroLength(length);

    // Names are stored in the first vector
    VDL_VECTOR_P attribute_name = vdl_vector_primitive_GetVectorPointer(v->Attribute, 0);

    // Check each name
    vdl_CheckNullVectorAndNullContainer(attribute_name);
    vdl_for_i(attribute_name->Length)
    {
        VDL_VECTOR_P string = vdl_vector_primitive_GetVectorPointer(attribute_name, i);
        vdl_CheckType(string->Type, VDL_TYPE_CHAR);

        if (length == string->Length)
        {
            VDL_CHAR_ARRAY char_data_array = string->Data;
            int unequal_flag               = 0;
            vdl_for_j(string->Length) unequal_flag += char_data_array[j] != name[j];
            if (unequal_flag == 0)
                // Plus one to skip the name vector
                return i + 1;
        }
    }

    return -1;
}

static inline VDL_VECTOR_P vdl_vector_GetAttribute_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const name)
{
    vdl_CheckNullPointer(name);
    vdl_CheckType(name->Type, VDL_TYPE_CHAR);

    const int index = vdl_vector_primitive_GetAttributeIndex(v, name->Data, name->Length);
    if (index == -1)
    {
        // Truncate the attribute name for error reporting if it is too long
        char buffer[32] = {0};
        memcpy(buffer, name, 31 * sizeof(char));

        vdl_Throw(VDL_EXCEPTION_ATTRIBUTE_NOT_FOUND,
                  "Vector attribute [%s] not found!",
                  buffer);
    }

    return vdl_vector_primitive_GetVectorPointer(v, index);
}


/*-----------------------------------------------------------------------------
 |  Get an attribute
 ----------------------------------------------------------------------------*/

/// Get the index of an attribute according to the attribute name.
/// @param v (VDL_VECTOR_P). A vector.
/// @param name (const char*). The attribute name.
/// @param length (int). The length of the attribute name.
/// @return (int) The index. -1 will be returned if the attribute name not found.
#define vdl_vector_primitive_GetAttributeIndex(...) vdl_CallFunction(vdl_vector_primitive_GetAttributeIndex_BT, int, __VA_ARGS__)
static inline int vdl_vector_primitive_GetAttributeIndex_BT(VDL_VECTOR_P v, const char *name, int length);

/// Get an attribute from a vector according to the attribute name.
/// @param v (VDL_VECTOR_P). A vector.
/// @param name (VDL_VECTOR_P). The attribute name.
/// @return (VDL_VECTOR_P) The attribute.
#define vdl_vector_GetAttribute(...) vdl_CallFunction(vdl_vector_GetAttribute_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_vector_GetAttribute_BT(VDL_VECTOR_P v, VDL_VECTOR_P name);


/*-----------------------------------------------------------------------------
 |  Append a single element (in-place operator)
 ----------------------------------------------------------------------------*/

#define vdl_AppendChar(...) vdl_CallVoidFunction(vdl_AppendChar_BT, __VA_ARGS__)
static inline void vdl_AppendChar_BT(VDL_VECTOR_T *const v, const char item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetChar(v, v->Length, item);
    v->Length++;
}

#define vdl_AppendInt(...) vdl_CallVoidFunction(vdl_AppendInt_BT, __VA_ARGS__)
static inline void vdl_AppendInt_BT(VDL_VECTOR_T *const v, const int item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetInt(v, v->Length, item);
    v->Length++;
}

#define vdl_AppendDouble(...) vdl_CallVoidFunction(vdl_AppendDouble_BT, __VA_ARGS__)
static inline void vdl_AppendDouble_BT(VDL_VECTOR_T *const v, const double item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetDouble(v, v->Length, item);
    v->Length++;
}

#define vdl_AppendVectorPointer(...) vdl_CallVoidFunction(vdl_AppendVectorPointer_BT, __VA_ARGS__)
static inline void vdl_AppendVectorPointer_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_POINTER);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetVectorPointer(v, v->Length, item);
    v->Length++;
}

#define vdl_Append vdl_AppendVectorPointer

/*-----------------------------------------------------------------------------
 |  Is empty
 ----------------------------------------------------------------------------*/

#define vdl_IsEmptyScalar(...) vdl_CallFunction(vdl_IsEmptyScalar_BT, int, __VA_ARGS__)
static inline int vdl_IsEmptyScalar_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullPointer(v);
    return v->Length == 0;
}

#define vdl_IsEmpty(v) vdl_vector_primitive_NewByInt(vdl_IsEmptyScalar(v))

/*-----------------------------------------------------------------------------
 |  Any True
 ----------------------------------------------------------------------------*/

#define vdl_AnyScalar(...) vdl_CallFunction(vdl_AnyScalar_BT, int, __VA_ARGS__)
static inline int vdl_AnyScalar_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckType(v->Type, VDL_TYPE_INT);
    vdl_CheckZeroLength(v->Length);

    VDL_INT_ARRAY data_array    = v->Data;
    int result                  = 0;
    vdl_for_i(v->Length) result = result || data_array[i];
    return result;
}

#define vdl_Any(...) vdl_CallFunction(vdl_Any_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_Any_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    if (v->Length == 0)
        return vdl_vector_primitive_NewEmpty(VDL_TYPE_INT, 1);

    VDL_INT_ARRAY data_array    = v->Data;
    int result                  = 0;
    vdl_for_i(v->Length) result = result || data_array[i];
    return vdl_vector_primitive_NewByInt(result);
}

/*-----------------------------------------------------------------------------
 |  All True
 ----------------------------------------------------------------------------*/

#define vdl_AllScalar(...) vdl_CallFunction(vdl_AllScalar_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline int vdl_AllScalar_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckType(v->Type, VDL_TYPE_INT);
    vdl_CheckZeroLength(v->Length);

    VDL_INT_ARRAY data_array    = v->Data;
    int result                  = 1;
    vdl_for_i(v->Length) result = result && data_array[i];
    return result;
}

#define vdl_All(...) vdl_CallFunction(vdl_All_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_All_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    if (v->Length == 0)
        return vdl_vector_primitive_NewEmpty(VDL_TYPE_INT, 1);

    VDL_INT_ARRAY data_array    = v->Data;
    int result                  = 1;
    vdl_for_i(v->Length) result = result && data_array[i];
    return vdl_vector_primitive_NewByInt(result);
}

/*-----------------------------------------------------------------------------
 |  Get/Subset
 ----------------------------------------------------------------------------*/

#define vdl_Subset(...) vdl_CallFunction(vdl_Subset_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_Subset_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const i)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullVectorAndNullContainer(i);
    vdl_CheckType(i->Type, VDL_TYPE_INT);

    if (v->Length == 0 || i->Length == 0)
        return vdl_vector_primitive_NewEmpty(v->Type, 1);

    // Index out of bound check
    VDL_INT_ARRAY index_array = i->Data;
    vdl_for_j(i->Length) vdl_CheckIndexOutOfBound(v, index_array[j]);

    VDL_VECTOR_P result = vdl_vector_primitive_NewEmpty(v->Type, i->Length);
    result->Length      = i->Length;

    switch (v->Type)
    {
        case VDL_TYPE_CHAR:
        {
            VDL_CHAR_ARRAY result_array          = result->Data;
            VDL_CHAR_ARRAY data_array            = v->Data;
            vdl_for_j(i->Length) result_array[j] = data_array[index_array[j]];
            break;
        }
        case VDL_TYPE_INT:
        {
            VDL_INT_ARRAY result_array           = result->Data;
            VDL_INT_ARRAY data_array             = v->Data;
            vdl_for_j(i->Length) result_array[j] = data_array[index_array[j]];
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            VDL_DOUBLE_ARRAY result_array        = result->Data;
            VDL_DOUBLE_ARRAY data_array          = v->Data;
            vdl_for_j(i->Length) result_array[j] = data_array[index_array[j]];
            break;
        }
        case VDL_TYPE_VECTOR_POINTER:
        {
            VDL_VECTOR_POINTER_ARRAY result_array = result->Data;
            VDL_VECTOR_POINTER_ARRAY data_array   = v->Data;
            vdl_for_j(i->Length) result_array[j]  = data_array[index_array[j]];
            break;
        }
    }

    return result;
}

/*-----------------------------------------------------------------------------
 |  Length of a vector
 ----------------------------------------------------------------------------*/

#define vdl_Length(...) vdl_CallFunction(vdl_Length_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_Length_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullPointer(v);
    return vdl_vector_primitive_NewByInt(v->Length);
}

/*-----------------------------------------------------------------------------
 |  Type of a vector
 ----------------------------------------------------------------------------*/

#define vdl_TypeOf(...) vdl_CallFunction(vdl_TypeOf_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_TypeOf_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullPointer(v);
    return vdl_vector_primitive_NewByInt((int) v->Type);
}


/*-----------------------------------------------------------------------------
 |  Mode of a vector
 ----------------------------------------------------------------------------*/

static inline VDL_VECTOR_P vdl_ModeOf_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullPointer(v);
    return vdl_vector_primitive_NewByInt((int) v->Mode);
}

/*-----------------------------------------------------------------------------
 |  Vector of True
 ----------------------------------------------------------------------------*/

static inline VDL_VECTOR_P vdl_OneVector_BT(VDL_VECTOR_T *const length)
{
    vdl_CheckNullVectorAndNullContainer(length);
    vdl_CheckType(length->Type, VDL_TYPE_INT);
    vdl_CheckLength(length->Length, 1);

    const int target_length            = vdl_vector_primitive_UnsafeIntAt(length, 0);
    VDL_VECTOR_P v                     = vdl_vector_primitive_NewEmpty(VDL_TYPE_INT, target_length);
    v->Length                          = target_length;
    VDL_INT_ARRAY data_array           = v->Data;
    vdl_for_i(v->Length) data_array[i] = 1;
    return v;
}

/*-----------------------------------------------------------------------------
 |  Vector of False
 ----------------------------------------------------------------------------*/

#define vdl_ZeroVector(...) vdl_CallFunction(vdl_ZeroVector_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_ZeroVector_BT(VDL_VECTOR_T *const length)
{
    vdl_CheckNullVectorAndNullContainer(length);
    vdl_CheckType(length->Type, VDL_TYPE_INT);
    vdl_CheckLength(length->Length, 1);

    const int target_length            = vdl_vector_primitive_UnsafeIntAt(length, 0);
    VDL_VECTOR_P v                     = vdl_vector_primitive_NewEmpty(VDL_TYPE_INT, target_length);
    v->Length                          = target_length;
    VDL_INT_ARRAY data_array           = v->Data;
    vdl_for_i(v->Length) data_array[i] = 0;
    return v;
}

/*-----------------------------------------------------------------------------
 |  Strict equal
 ----------------------------------------------------------------------------*/

#define vdl_StrictEqual(...) vdl_CallFunction(vdl_StrictEqual_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_StrictEqual_BT(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const v2)
{
    vdl_CheckNullVectorAndNullContainer(v1);
    vdl_CheckNullVectorAndNullContainer(v2);

    if (v1->Length == 0 || v2->Length == 0)
        return vdl_vector_primitive_NewEmpty(v1->Type, 1);

    VDL_VECTOR_P short_v = NULL;
    VDL_VECTOR_P long_v  = NULL;
    if (v1->Length > v2->Length)
    {
        short_v = v1;
        long_v  = v2;
    }
    else
    {
        short_v = v2;
        long_v  = v1;
    }

    vdl_CheckIncompatibleLength(short_v->Length, long_v->Length);

    if (v1->Type != v2->Type)
        return vdl_ZeroVector(vdl_Length(long_v));

    VDL_VECTOR_P result             = vdl_vector_primitive_NewEmpty(VDL_TYPE_INT, long_v->Length);
    result->Length                  = long_v->Length;
    VDL_INT_ARRAY result_data_array = result->Data;
    switch (long_v->Type)
    {
        case VDL_TYPE_CHAR:
        {
            VDL_CHAR_ARRAY long_data_array                 = long_v->Data;
            VDL_CHAR_ARRAY short_data_array                = short_v->Data;
            vdl_for_i(long_v->Length) result_data_array[i] = long_data_array[i] == short_data_array[i];
            break;
        }
        case VDL_TYPE_INT:
        {
            VDL_INT_ARRAY long_data_array                  = long_v->Data;
            VDL_INT_ARRAY short_data_array                 = short_v->Data;
            vdl_for_i(long_v->Length) result_data_array[i] = long_data_array[i] == short_data_array[i];
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            VDL_DOUBLE_ARRAY long_data_array               = long_v->Data;
            VDL_DOUBLE_ARRAY short_data_array              = short_v->Data;
            vdl_for_i(long_v->Length) result_data_array[i] = long_data_array[i] == short_data_array[i];
            break;
        }
        case VDL_TYPE_VECTOR_POINTER:
        {
            VDL_VECTOR_POINTER_ARRAY long_data_array       = long_v->Data;
            VDL_VECTOR_POINTER_ARRAY short_data_array      = short_v->Data;
            vdl_for_i(long_v->Length) result_data_array[i] = long_data_array[i] == short_data_array[i];
            break;
        }
    }

    return result;
}

/*-----------------------------------------------------------------------------
 |  Extend elements (in-place operator)
 ----------------------------------------------------------------------------*/

#define vdl_Extend(...) vdl_CallVoidFunction(vdl_Extend_BT, __VA_ARGS__)
static inline void vdl_Extend_BT(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const v2)
{
    vdl_CheckNullPointer(v1);
    vdl_CheckNullPointer(v2);
    vdl_CheckType(v1->Type, v2->Type);

    if (v2->Length == 0)
        return;

    vdl_vector_primitive_Reserve(v1, vdl_AddIntOverflow(v1->Length, v2->Length));
    vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v1, v1->Length, v2->Data, v2->Length);
    v1->Length += v2->Length;
}

/*-----------------------------------------------------------------------------
 |  Which True
 ----------------------------------------------------------------------------*/

#define vdl_Which(...) vdl_CallFunction(vdl_Which_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_Which_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    VDL_VECTOR_P result      = vdl_vector_primitive_NewEmpty(v->Type, 1);
    VDL_INT_ARRAY data_array = v->Data;
    vdl_for_i(v->Length)
    {
        if (data_array[i])
            vdl_AppendInt(result, i);
    }
    return result;
}

/*-----------------------------------------------------------------------------
 |  Find the first identical element
 ----------------------------------------------------------------------------*/

static inline VDL_VECTOR_P vdl_Find_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const item)
{
    VDL_VECTOR_P first_equal_index = vdl_Subset(vdl_Which(vdl_StrictEqual(v, item)),
                                                vdl_LocalVector(0));
    if (first_equal_index->Length == 0)
        return vdl_vector_primitive_NewByInt(-1);
    return first_equal_index;
}

/*-----------------------------------------------------------------------------
 |  Insert elements (in-place operator)
 ----------------------------------------------------------------------------*/

#define vdl_InsertChar(...) vdl_CallVoidFunction(vdl_InsertChar_BT, __VA_ARGS__)
static inline void vdl_InsertChar_BT(VDL_VECTOR_T *const v, const int i, const char item)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v, i + 1, vdl_vector_primitive_UnsafeAddressOf(v, i), v->Length - i);
    vdl_vector_primitive_UnsafeSetChar(v, i, item);
    v->Length++;
}


#define vdl_InsertInt(...) vdl_CallVoidFunction(vdl_InsertInt_BT, __VA_ARGS__)
static inline void vdl_InsertInt_BT(VDL_VECTOR_T *const v, const int i, const int item)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v, i + 1, vdl_vector_primitive_UnsafeAddressOf(v, i), v->Length - i);
    vdl_vector_primitive_UnsafeSetInt(v, i, item);
    v->Length++;
}


#define vdl_InsertDouble(...) vdl_CallVoidFunction(vdl_InsertDouble_BT, __VA_ARGS__)
static inline void vdl_InsertDouble_BT(VDL_VECTOR_T *const v, const int i, const double item)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v, i + 1, vdl_vector_primitive_UnsafeAddressOf(v, i), v->Length - i);
    vdl_vector_primitive_UnsafeSetDouble(v, i, item);
    v->Length++;
}


#define vdl_InsertVectorPointer(...) vdl_CallVoidFunction(vdl_InsertVectorPointer_BT, __VA_ARGS__)
static inline void vdl_InsertVectorPointer_BT(VDL_VECTOR_T *const v, const int i, VDL_VECTOR_T *const item)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_POINTER);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v, i + 1, vdl_vector_primitive_UnsafeAddressOf(v, i), v->Length - i);
    vdl_vector_primitive_UnsafeSetVectorPointer(v, i, item);
    v->Length++;
}

#define vdl_InsertByArray(...) vdl_CallVoidFunction(vdl_InsertByArray_BT, __VA_ARGS__)
static inline void vdl_InsertByArray_BT(VDL_VECTOR_T *const v, const int i, const void *item_pointer, const int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckIndexOutOfBound(v, i);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, number));
    vdl_vector_primitive_UnsafeSetByArrayAndMemcpy(v, i + number, vdl_vector_primitive_UnsafeAddressOf(v, i), number);
    vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v, i, item_pointer, number);
    v->Length += number;
}

static inline void vdl_Insert_BT(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const i, VDL_VECTOR_T *const v2)
{
    vdl_CheckNullVectorAndNullContainer(v1);
    vdl_CheckNullVectorAndNullContainer(i);
    vdl_CheckNullVectorAndNullContainer(v2);
    vdl_CheckType(i->Type, VDL_TYPE_INT);
    vdl_CheckLength(i->Length, 1);
    vdl_CheckType(v1->Type, v2->Type);

    const int index = vdl_vector_primitive_UnsafeIntAt(i, 0);
    vdl_CheckIndexOutOfBound(v1, index);

    vdl_vector_primitive_Reserve(v1, vdl_AddIntOverflow(v1->Length, v2->Length));
    vdl_vector_primitive_UnsafeSetByArrayAndMemcpy(v1, index + v2->Length, vdl_vector_primitive_UnsafeAddressOf(v1, index), v2->Length);
    vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v1, index, v2->Data, v2->Length);
    v1->Length += v2->Length;
}

// TODO: make member of vectors private (_name)

/*-----------------------------------------------------------------------------
 |  Concatenate two vectors
 ----------------------------------------------------------------------------*/

#define vdl_Concatenate(...) vdl_CallFunction(vdl_Concatenate_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_Concatenate_BT(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const v2)
{
    vdl_CheckNullVectorAndNullContainer(v1);
    vdl_CheckNullVectorAndNullContainer(v2);
    vdl_CheckType(v1->Type, v2->Type);

    VDL_VECTOR_P result = vdl_vector_primitive_NewEmpty(v1->Type, vdl_AddIntOverflow(v1->Length, v2->Length));
    vdl_vector_primitive_UnsafeSetByArrayAndMemcpy(result, 0, v1->Data, v1->Length);
    vdl_vector_primitive_UnsafeSetByArrayAndMemcpy(result, v1->Length, v2->Data, v2->Length);
    result->Length = v1->Length + v2->Length;
    return result;
}

/*-----------------------------------------------------------------------------
 |  Shallow copy
 ----------------------------------------------------------------------------*/

#define vdl_ShallowCopy(...) vdl_CallFunction(vdl_ShallowCopy_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_ShallowCopy_BT(VDL_VECTOR_T *const v)
{
    vdl_CheckNullVectorAndNullContainer(v);

    VDL_VECTOR_P result = vdl_vector_primitive_NewEmpty(v->Type, v->Capacity);
    vdl_vector_primitive_UnsafeSetByArrayAndMemcpy(result, 0, v->Data, v->Length);
    result->Length = v->Length;
    return result;
}

/*-----------------------------------------------------------------------------
 |  New an attribute
 ----------------------------------------------------------------------------*/


static inline void vdl_vector_InitAttribute(VDL_VECTOR_T *const v)
{
    if (v->Attribute != NULL)
        return;

    // Only need one space for the name vector
    VDL_VECTOR_P attribute = vdl_vector_primitive_NewEmpty(VDL_TYPE_VECTOR_POINTER, 1);

    // New the name vector
    VDL_VECTOR_P attribute_name = vdl_vector_primitive_NewEmpty(VDL_TYPE_CHAR, 1);

    vdl_vector_primitive_SetVectorPointer(attribute, 0, attribute_name);

    v->Attribute = attribute;
}


/*-----------------------------------------------------------------------------
 |  Set an attribute
 ----------------------------------------------------------------------------*/

static inline void vdl_vector_SetAttribute_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const name, VDL_VECTOR_T *const value)
{
    vdl_CheckNullPointer(name);
    vdl_CheckType(name->Type, VDL_TYPE_CHAR);

    vdl_vector_InitAttribute(v);

    const int index = vdl_vector_primitive_GetAttributeIndex(v, name->Data, name->Length);

    if (index == -1)
    {
        VDL_VECTOR_P attribute_name = vdl_vector_primitive_GetVectorPointer(v->Attribute, 0);
    }
    else
    {
        vdl_vector_primitive_SetVectorPointer(v, index, value);
    }
}


// TODO: check storage mode of vector to decide whether a function can work on it

// static inline void vdl_Remove(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const v2)
// {
//     vdl_CheckNullPointer(v1);
//     vdl_CheckNullPointer(v1->Data);
//     vdl_CheckNullPointer(v2);
//     vdl_CheckNullPointer(v2->Data);
//     vdl_CheckType(v2->Type, VDL_TYPE_INT);
//
//     // Check the index provided in vector v2.
//     VDL_INT_ARRAY index_array = v2->Data;
//     vdl_for_i(v2->Length)
//     {
//         vdl_CheckIndexOutOfBound(v1, index_array[i]);
//     }
//
//     // Use a placeholder vector to store the result.
//     VDL_VECTOR_P result = vdl_NewEmpty(v1->Type, v1->Length);
//
//     switch (v1->Type)
//     {
//         case VDL_TYPE_CHAR:
//         {
//             VDL_CHAR_ARRAY data_array = v1->Data;
//             vdl_for_i(v1->Length)
//             {
//                 int keep_item = vdl_FindFirstChar(v2, data_array[i]) == -1;
//                 if (keep_item)
//                 {
//                     vdl_UnsafeSetChar(result, result->Length, data_array[i]);
//                     result->Length++;
//                 }
//             }
//             break;
//         }
//         case VDL_TYPE_INT:
//         {
//             VDL_INT_ARRAY data_array = v1->Data;
//             vdl_for_i(v1->Length)
//             {
//                 int keep_item = vdl_FindFirstInt(v2, data_array[i]) == -1;
//                 if (keep_item)
//                 {
//                     vdl_UnsafeSetInt(result, result->Length, data_array[i]);
//                     result->Length++;
//                 }
//             }
//             break;
//         }
//         case VDL_TYPE_DOUBLE:
//         {
//             VDL_DOUBLE_ARRAY data_array = v1->Data;
//             vdl_for_i(v1->Length)
//             {
//                 int keep_item = vdl_FindFirstDouble(v2, data_array[i]) == -1;
//                 if (keep_item)
//                 {
//                     vdl_UnsafeSetDouble(result, result->Length, data_array[i]);
//                     result->Length++;
//                 }
//             }
//             break;
//         }
//         case VDL_TYPE_VECTOR_P:
//         {
//             VDL_VECTOR_POINTER_ARRAY data_array = v1->Data;
//             vdl_for_i(v1->Length)
//             {
//                 int keep_item = vdl_FindFirstVectorPointer(v2, data_array[i]) == -1;
//                 if (keep_item)
//                 {
//                     vdl_UnsafeSetVectorPointer(result, result->Length, data_array[i]);
//                     result->Length++;
//                 }
//             }
//             break;
//         }
//     }
//
//     // Copy the content from result back to the original vector.
//     vdl_UnsafeSetByArrayAndMemcpy(v1, 0, result->Data, result->Length);
//     v1->Length = result->Length;
// }
//
// static inline void vdl_RemoveByFirstAndLastIndex(VDL_VECTOR_T *const v, const int first, const int last)
// {
// }
//
// static inline void vdl_Clear_BT(VDL_VECTOR_T *const v)
// {
//     vdl_CheckNullPointer(v);
//     v->Length = 0;
// }

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

#endif//VDL_VDL_8_VECTOR_PORTAL_H
