//
// Created by Patrick Li on 5/3/2023.
//

#ifndef VDL_VDL_8_VECTOR_PORTAL_DEF_H
#define VDL_VDL_8_VECTOR_PORTAL_DEF_H

/*-----------------------------------------------------------------------------
 |  Set the vector data safely
 ----------------------------------------------------------------------------*/

static inline void vdl_vector_Set_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const value)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullVectorAndNullContainer(value);
    vdl_CheckType(v->Type, value->Type);
    vdl_CheckZeroLength(v->Length);
    vdl_CheckIncompatibleLength(value->Length, v->Length);


    switch (v->Type)
    {
        case VDL_TYPE_CHAR:
        {
            VDL_CHAR_ARRAY dest_array = v->Data;
            VDL_CHAR_ARRAY src_array  = value->Data;
            if (value->Length == 1)
            {
                const char element                 = src_array[0];
                vdl_for_i(v->Length) dest_array[i] = element;
            }
            else
            {
                vdl_vector_primitive_SetByArrayAndMemmove(v, 0, value->Data, v->Length);
            }
            break;
        }
        case VDL_TYPE_INT:
        {
            VDL_INT_ARRAY dest_array = v->Data;
            VDL_INT_ARRAY src_array  = value->Data;
            if (value->Length == 1)
            {
                const int element                  = src_array[0];
                vdl_for_i(v->Length) dest_array[i] = element;
            }
            else
            {
                vdl_vector_primitive_SetByArrayAndMemmove(v, 0, value->Data, v->Length);
            }
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            VDL_DOUBLE_ARRAY dest_array = v->Data;
            VDL_DOUBLE_ARRAY src_array  = value->Data;
            if (value->Length == 1)
            {
                const double element               = src_array[0];
                vdl_for_i(v->Length) dest_array[i] = element;
            }
            else
            {
                vdl_vector_primitive_SetByArrayAndMemmove(v, 0, value->Data, v->Length);
            }
            break;
        }
        case VDL_TYPE_VECTOR_POINTER:
        {
            VDL_VECTOR_POINTER_ARRAY dest_array = v->Data;
            VDL_VECTOR_POINTER_ARRAY src_array  = value->Data;
            if (value->Length == 1)
            {
                VDL_VECTOR_T *const element        = src_array[0];
                vdl_for_i(v->Length) dest_array[i] = element;
            }
            else
            {
                vdl_vector_primitive_SetByArrayAndMemmove(v, 0, value->Data, v->Length);
            }
            break;
        }
    }
}

/*-----------------------------------------------------------------------------
 |  Set the vector data safely by index
 ----------------------------------------------------------------------------*/

static inline void vdl_vector_SetByIndex_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const i, VDL_VECTOR_T *const value)
{
    vdl_CheckIntVector(i);
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullVectorAndNullContainer(value);
    vdl_CheckType(v->Type, value->Type);
    vdl_CheckZeroLength(i->Length);
    vdl_CheckIncompatibleLength(value->Length, i->Length);

    // Check index out of bound
    VDL_INT_ARRAY index_array = i->Data;
    vdl_for_j(i->Length)
    {
        const int index = index_array[j];
        vdl_CheckIntNA(index);
        vdl_CheckIndexOutOfBound(v, index);
    }


    // Set the values
    switch (v->Type)
    {
        case VDL_TYPE_CHAR:
        {

            VDL_CHAR_ARRAY dest_array = v->Data;
            VDL_CHAR_ARRAY src_array  = value->Data;
            if (value->Length == 1)
            {
                const char element                              = src_array[0];
                vdl_for_j(i->Length) dest_array[index_array[j]] = element;
            }
            else
            {
                vdl_for_j(i->Length) dest_array[index_array[j]] = src_array[j];
            }
            break;
        }
        case VDL_TYPE_INT:
        {
            VDL_INT_ARRAY dest_array = v->Data;
            VDL_INT_ARRAY src_array  = value->Data;
            if (value->Length == 1)
            {
                const int element                               = src_array[0];
                vdl_for_j(i->Length) dest_array[index_array[j]] = element;
            }
            else
            {
                vdl_for_j(i->Length) dest_array[index_array[j]] = src_array[j];
            }
            break;
        }
        case VDL_TYPE_DOUBLE:
        {
            VDL_DOUBLE_ARRAY dest_array = v->Data;
            VDL_DOUBLE_ARRAY src_array  = value->Data;
            if (value->Length == 1)
            {
                const double element                            = src_array[0];
                vdl_for_j(i->Length) dest_array[index_array[j]] = element;
            }
            else
            {
                vdl_for_j(i->Length) dest_array[index_array[j]] = src_array[j];
            }
            break;
        }
        case VDL_TYPE_VECTOR_POINTER:
        {
            VDL_VECTOR_POINTER_ARRAY dest_array = v->Data;
            VDL_VECTOR_POINTER_ARRAY src_array  = value->Data;
            if (value->Length == 1)
            {
                VDL_VECTOR_T *const element                     = src_array[0];
                vdl_for_j(i->Length) dest_array[index_array[j]] = element;
            }
            else
            {
                vdl_for_j(i->Length) dest_array[index_array[j]] = src_array[j];
            }
            break;
        }
    }
}

/*-----------------------------------------------------------------------------
 |  Subset the vector
 ----------------------------------------------------------------------------*/

static inline VDL_VECTOR_P vdl_Subset_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const i)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIntVector(i);
    vdl_CheckZeroLength(i->Length);

    // Index out of bound check
    VDL_INT_ARRAY index_array = i->Data;
    vdl_for_j(i->Length)
    {
        const int index = index_array[j];
        vdl_CheckIntNA(index);
        vdl_CheckIndexOutOfBound(v, index);
    }

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
 |  Append an item (in-place operator)
 ----------------------------------------------------------------------------*/

static inline void vdl_vector_primitive_AppendChar_BT(VDL_VECTOR_T *const v, const char item)
{
    vdl_CheckCharVector(v);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetChar(v, v->Length, item);
    v->Length++;
}

static inline void vdl_vector_primitive_AppendInt_BT(VDL_VECTOR_T *const v, const int item)
{
    vdl_CheckIntVector(v);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetInt(v, v->Length, item);
    v->Length++;
}

static inline void vdl_vector_primitive_AppendDouble_BT(VDL_VECTOR_T *const v, const double item)
{
    vdl_CheckDoubleVector(v);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetDouble(v, v->Length, item);
    v->Length++;
}

static inline void vdl_vector_primitive_AppendVectorPointer_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const item)
{
    vdl_CheckVectorPointerVector(v);

    vdl_vector_primitive_Reserve(v, vdl_AddIntOverflow(v->Length, 1));
    vdl_vector_primitive_UnsafeSetVectorPointer(v, v->Length, item);
    v->Length++;
}


#endif//VDL_VDL_8_VECTOR_PORTAL_DEF_H
