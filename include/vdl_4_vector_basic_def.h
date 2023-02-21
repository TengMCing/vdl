//
// Created by Patrick Li on 20/2/2023.
//

#ifndef VDL_VDL_4_VECTOR_BASIC_DEF_H
#define VDL_VDL_4_VECTOR_BASIC_DEF_H

/*-----------------------------------------------------------------------------
 |  Accessing the vector data safely
 ----------------------------------------------------------------------------*/

static inline void *vdl_AddressOf_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);

    return vdl_UnsafeAddressOf(v, i);
}

static inline char vdl_GetChar_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    return vdl_UnsafeConstCharAt(v, i);
}

static inline int vdl_GetInt_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    return vdl_UnsafeConstIntAt(v, i);
}

static inline double vdl_GetDouble_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    return vdl_UnsafeConstDoubleAt(v, i);
}

static inline VDL_VECTOR_P vdl_GetVectorPointer_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);

    return vdl_UnsafeVectorConstPointerAt(v, i);
}

/*-----------------------------------------------------------------------------
 |  Set the vector data safely
 ----------------------------------------------------------------------------*/

static inline void vdl_SetChar_BT(const VDL_VECTOR_T *const v, const int i, const char item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_UnsafeSetChar(v, i, item);
}

static inline void vdl_SetInt_BT(const VDL_VECTOR_T *const v, const int i, const int item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    vdl_UnsafeSetInt(v, i, item);
}

static inline void vdl_SetDouble_BT(const VDL_VECTOR_T *const v, const int i, const double item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    vdl_UnsafeSetDouble(v, i, item);
}

static inline void vdl_SetVectorPointer_BT(const VDL_VECTOR_T *const v, const int i, const VDL_VECTOR_T *const item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);

    vdl_UnsafeSetVectorPointer(v, i, item);
}

static inline void vdl_SetByMemcpy_BT(const VDL_VECTOR_T *const v, const int i, const void *const item_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);

    vdl_UnsafeSetByMemcpy(v, i, item_pointer, (size_t) number);
}

static inline void vdl_SetByMemmove_BT(const VDL_VECTOR_T *const v, const int i, const void *const item_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);

    vdl_UnsafeSetByMemmove(v, i, item_pointer, (size_t) number);
}

static inline void vdl_SetCharByIndices_BT(const VDL_VECTOR_T *const v, const char *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);

    vdl_UnsafeSetCharByIndices(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetIntByIndices_BT(const VDL_VECTOR_T *const v, const int *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckType(v->Type, VDL_TYPE_INT);

    vdl_UnsafeSetIntByIndices(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetDoubleByIndices_BT(const VDL_VECTOR_T *const v, const double *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);

    vdl_UnsafeSetDoubleByIndices(v, item_pointer, index_pointer, number);
}

static inline void vdl_SetVectorPointerByIndices_BT(const VDL_VECTOR_T *const v, const VDL_VECTOR_T *const *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);

    vdl_UnsafeSetVectorPointerByIndices(v, item_pointer, index_pointer, number);
}


#endif//VDL_VDL_4_VECTOR_BASIC_DEF_H
