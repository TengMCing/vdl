//
// Created by Patrick Li on 20/2/2023.
//

#ifndef VDL_VDL_5_VECTOR_BASIC_DEF_H
#define VDL_VDL_5_VECTOR_BASIC_DEF_H

/*-----------------------------------------------------------------------------
 |  Primitive functions to access the vector data safely
 ----------------------------------------------------------------------------*/

static inline void *vdl_vector_primitive_GetAddress_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    return vdl_vector_primitive_UnsafeAddressOf(v, i);
}

static inline char vdl_vector_primitive_GetChar_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckCharVector(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    return vdl_vector_primitive_UnsafeConstCharAt(v, i);
}

static inline int vdl_vector_primitive_GetInt_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckIntVector(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    return vdl_vector_primitive_UnsafeConstIntAt(v, i);
}

static inline double vdl_vector_primitive_GetDouble_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckDoubleVector(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    return vdl_vector_primitive_UnsafeConstDoubleAt(v, i);
}

static inline VDL_VECTOR_P vdl_vector_primitive_GetVectorPointer_BT(VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckVectorPointerVector(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    return vdl_vector_primitive_UnsafeVectorConstPointerAt(v, i);
}

/*-----------------------------------------------------------------------------
 |  Primitive functions to set the vector data safely
 ----------------------------------------------------------------------------*/

static inline void vdl_vector_primitive_SetChar_BT(VDL_VECTOR_T *const v, const int i, const char item)
{
    vdl_CheckCharVector(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    vdl_vector_primitive_UnsafeSetChar(v, i, item);
}

static inline void vdl_vector_primitive_SetInt_BT(VDL_VECTOR_T *const v, const int i, const int item)
{
    vdl_CheckIntVector(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    vdl_vector_primitive_UnsafeSetInt(v, i, item);
}

static inline void vdl_vector_primitive_SetDouble_BT(VDL_VECTOR_T *const v, const int i, const double item)
{
    vdl_CheckDoubleVector(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    vdl_vector_primitive_UnsafeSetDouble(v, i, item);
}

static inline void vdl_vector_primitive_SetVectorPointer_BT(VDL_VECTOR_T *const v, const int i, VDL_VECTOR_T *const item)
{
    vdl_CheckVectorPointerVector(v);
    vdl_CheckIntNA(i);
    vdl_CheckIndexOutOfBound(v, i);

    vdl_vector_primitive_UnsafeSetVectorPointer(v, i, item);
}

static inline void vdl_vector_primitive_SetByArrayAndMemcpy_BT(VDL_VECTOR_T *const v, int i, const void *const item_pointer, const int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckIntNA(i);
    vdl_CheckIntNA(number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);

    vdl_vector_primitive_UnsafeSetByArrayAndMemcpy(v, i, item_pointer, number);
}

static inline void vdl_vector_primitive_SetByArrayAndMemmove_BT(VDL_VECTOR_T *const v, int i, const void *const item_pointer, const int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckIntNA(i);
    vdl_CheckIntNA(number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);

    vdl_vector_primitive_UnsafeSetByArrayAndMemmove(v, i, item_pointer, number);
}

static inline void vdl_vector_primitive_SetCharByArrayAndIndex_BT(VDL_VECTOR_T *const v, const char *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckCharVector(v);
    vdl_CheckIntNA(number);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);

    vdl_for_i(number)
    {
        const int index = index_pointer[i];
        vdl_CheckIntNA(index);
        vdl_CheckIndexOutOfBound(v, index);
    }

    vdl_vector_primitive_UnsafeSetCharByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_vector_primitive_SetIntByArrayAndIndex_BT(VDL_VECTOR_T *const v, const int *item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckIntVector(v);
    vdl_CheckIntNA(number);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);

    vdl_for_i(number)
    {
        const int index = index_pointer[i];
        vdl_CheckIntNA(index);
        vdl_CheckIndexOutOfBound(v, index);
    }

    vdl_vector_primitive_UnsafeSetIntByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_vector_primitive_SetDoubleByArrayAndIndex_BT(VDL_VECTOR_T *const v, const double *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckDoubleVector(v);
    vdl_CheckIntNA(number);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);

    vdl_for_i(number)
    {
        const int index = index_pointer[i];
        vdl_CheckIntNA(index);
        vdl_CheckIndexOutOfBound(v, index);
    }

    vdl_vector_primitive_UnsafeSetDoubleByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_vector_primitive_SetVectorPointerByArrayAndIndex_BT(VDL_VECTOR_T *const v, VDL_VECTOR_T *const *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckVectorPointerVector(v);
    vdl_CheckIntNA(number);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);

    vdl_for_i(number)
    {
        const int index = index_pointer[i];
        vdl_CheckIntNA(index);
        vdl_CheckIndexOutOfBound(v, index);
    }

    vdl_vector_primitive_UnsafeSetVectorPointerByArrayAndIndex(v, item_pointer, index_pointer, number);
}

/*-----------------------------------------------------------------------------
 |  Convert an integer vector to a boolean scalar value
 ----------------------------------------------------------------------------*/

static inline int vdl_vector_primitive_ToBool_BT(VDL_VECTOR_T *const condition)
{
    vdl_CheckIntVector(condition);
    vdl_CheckLength(condition->Length, 1);

    return vdl_vector_primitive_UnsafeIntAt(condition, 0) != 0;
}


#endif//VDL_VDL_5_VECTOR_BASIC_DEF_H
