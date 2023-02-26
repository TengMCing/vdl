//
// Created by Patrick Li on 20/2/2023.
//

#ifndef VDL_VDL_5_VECTOR_BASIC_DEF_H
#define VDL_VDL_5_VECTOR_BASIC_DEF_H

/*-----------------------------------------------------------------------------
 |  Accessing the vector data safely
 ----------------------------------------------------------------------------*/

static inline void *vdl_AddressOf_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);

    return vdl_UnsafeAddressOf(v, i);
}

static inline char vdl_GetChar_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    return vdl_UnsafeConstCharAt(v, i);
}

static inline int vdl_GetInt_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    return vdl_UnsafeConstIntAt(v, i);
}

static inline double vdl_GetDouble_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    return vdl_UnsafeConstDoubleAt(v, i);
}

static inline VDL_VECTOR_P vdl_GetVectorPointer_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_POINTER);

    return vdl_UnsafeVectorConstPointerAt(v, i);
}

/*-----------------------------------------------------------------------------
 |  Set the vector data safely
 ----------------------------------------------------------------------------*/

static inline void vdl_SetChar_BT(VDL_VECTOR_T *const v, const int i, const char item)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_UnsafeSetChar(v, i, item);
}

static inline void vdl_SetInt_BT(VDL_VECTOR_T *const v, const int i, const int item)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    vdl_UnsafeSetInt(v, i, item);
}

static inline void vdl_SetDouble_BT(VDL_VECTOR_T *const v, const int i, const double item)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    vdl_UnsafeSetDouble(v, i, item);
}

static inline void vdl_SetVectorPointer_BT(VDL_VECTOR_T *const v, const int i, VDL_VECTOR_T *const item)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_POINTER);

    vdl_UnsafeSetVectorPointer(v, i, item);
}

static inline void vdl_SetByArrayAndMemcpy_BT(VDL_VECTOR_P v, int i, const void *item_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);

    vdl_UnsafeSetByArrayAndMemcpy(v, i, item_pointer, number);
}

static inline void vdl_SetByArrayAndMemmove_BT(VDL_VECTOR_P v, int i, const void *item_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);

    vdl_UnsafeSetByArrayAndMemmove(v, i, item_pointer, number);
}

static inline void vdl_SetCharByArrayAndIndex_BT(VDL_VECTOR_P v, const char *item_pointer, const int *index_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_for_i(number) vdl_CheckIndexOutOfBound(v, index_pointer[i]);

    vdl_UnsafeSetCharByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetIntByArrayAndIndex_BT(VDL_VECTOR_P v, const int *item_pointer, const int *index_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    vdl_for_i(number) vdl_CheckIndexOutOfBound(v, index_pointer[i]);

    vdl_UnsafeSetIntByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetDoubleByArrayAndIndex_BT(VDL_VECTOR_P v, const double *item_pointer, const int *index_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    vdl_for_i(number) vdl_CheckIndexOutOfBound(v, index_pointer[i]);

    vdl_UnsafeSetDoubleByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetVectorPointerByArrayAndIndex_BT(VDL_VECTOR_P v, VDL_VECTOR_T *const *item_pointer, const int *index_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_POINTER);

    vdl_for_i(number) vdl_CheckIndexOutOfBound(v, index_pointer[i]);

    vdl_UnsafeSetVectorPointerByArrayAndIndex(v, item_pointer, index_pointer, number);
}

/*-----------------------------------------------------------------------------
 |  Set the vector data safely (Public API)
 ----------------------------------------------------------------------------*/

static inline void vdl_Set_BT(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const v2)
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
                vdl_SetByArrayAndMemmove(v1, 0, v2->Data, v1->Length);
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
                vdl_SetByArrayAndMemmove(v1, 0, v2->Data, v1->Length);
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
                vdl_SetByArrayAndMemmove(v1, 0, v2->Data, v1->Length);
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
                vdl_SetByArrayAndMemmove(v1, 0, v2->Data, v1->Length);
            }
            break;
        }
    }
}

static inline void vdl_SetByIndex_BT(VDL_VECTOR_T *const v1, VDL_VECTOR_T *const i, VDL_VECTOR_T *const v2)
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
 |  Convert an integer vector to a boolean scalar value
 ----------------------------------------------------------------------------*/

static inline int vdl_ToBoolScalar(VDL_VECTOR_T *const v)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckType(v->Type, VDL_TYPE_INT);
    vdl_CheckLength(v->Length, 1);

    return vdl_GetInt(v, 0) != 0;
}


#endif//VDL_VDL_5_VECTOR_BASIC_DEF_H
