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
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);

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
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);

    vdl_UnsafeSetVectorPointer(v, i, item);
}

static inline void vdl_SetByArrayAndMemcpy_BT(VDL_VECTOR_P v, int i, const void *item_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);

    vdl_UnsafeSetByArrayAndMemcpy(v, i, item_pointer, (size_t) number);
}

static inline void vdl_SetByArrayAndMemmove_BT(VDL_VECTOR_P v, int i, const void *item_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);

    vdl_UnsafeSetByArrayAndMemmove(v, i, item_pointer, (size_t) number);
}

static inline void vdl_SetCharByArrayAndIndex_BT(VDL_VECTOR_P v, const char *item_pointer, const int *index_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_UnsafeSetCharByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetIntByArrayAndIndex_BT(VDL_VECTOR_P v, const int *item_pointer, const int *index_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    vdl_UnsafeSetIntByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetDoubleByArrayAndIndex_BT(VDL_VECTOR_P v, const double *item_pointer, const int *index_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    vdl_UnsafeSetDoubleByArrayAndIndex(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetVectorPointerByArrayAndIndex_BT(VDL_VECTOR_P v, VDL_VECTOR_T *const *item_pointer, const int *index_pointer, int number)
{
    vdl_CheckNullVectorAndNullContainer(v);
    vdl_CheckNullArrayAndNegativeLength(item_pointer, number);
    vdl_CheckNullPointer(index_pointer);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);

    vdl_UnsafeSetVectorPointerByArrayAndIndex(v, item_pointer, index_pointer, number);
}


#endif//VDL_VDL_5_VECTOR_BASIC_DEF_H
