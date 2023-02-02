//
// Created by Patrick Li on 29/12/22.
//


#ifndef VDL_VDLARROP_H
#define VDL_VDLARROP_H

#include "vdlscalarop.h"

/*-----------------------------------------------------------------------------
 |  Array NA check
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayIsNa(x, result, len) _Generic(x, char *                                      \
                                                  : vdlint_CharArrayIsNa, const char *           \
                                                  : vdlint_CharArrayIsNa, int *                  \
                                                  : vdlint_IntArrayIsNa, const int *             \
                                                  : vdlint_IntArrayIsNa, double *                \
                                                  : vdlint_DoubleArrayIsNa, const double *       \
                                                  : vdlint_DoubleArrayIsNa, void **              \
                                                  : vdlint_PointerArrayIsNa, void *const *       \
                                                  : vdlint_PointerArrayIsNa, const void **       \
                                                  : vdlint_PointerArrayIsNa, const void *const * \
                                                  : vdlint_PointerArrayIsNa)(x, result, len)

#define vdlint_TArrayIsNa(CT, QT)                                                                      \
    void vdlint_##CT##ArrayIsNa(const QT *const restrict x, int *const restrict result, const int len) \
    {                                                                                                  \
        vdlint_for_i(len) result[i] = vdlint_##CT##ScalarIsNa(x[i]);                                   \
    }

vdlint_TArrayIsNa(Char, char);
vdlint_TArrayIsNa(Int, int);
vdlint_TArrayIsNa(Double, double);
vdlint_TArrayIsNa(Pointer, void *const);
#undef vdlint_TArrayIsNa

/*-----------------------------------------------------------------------------
 |  Array any NA check
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayAnyNa(x, len) _Generic(x, char *                                       \
                                           : vdlint_CharArrayAnyNa, const char *           \
                                           : vdlint_CharArrayAnyNa, int *                  \
                                           : vdlint_IntArrayAnyNa, const int *             \
                                           : vdlint_IntArrayAnyNa, double *                \
                                           : vdlint_DoubleArrayAnyNa, const double *       \
                                           : vdlint_DoubleArrayAnyNa, void **              \
                                           : vdlint_PointerArrayAnyNa, void *const *       \
                                           : vdlint_PointerArrayAnyNa, const void **       \
                                           : vdlint_PointerArrayAnyNa, const void *const * \
                                           : vdlint_PointerArrayAnyNa)(x, len)

#define vdlint_TArrayAnyNa(CT, QT)                                                   \
    static inline VDL_BOOL vdlint_##CT##ArrayAnyNa(const QT *const x, const int len) \
    {                                                                                \
        VDL_BOOL flag          = VDL_FALSE;                                          \
        vdlint_for_i(len) flag = flag | vdlint_##CT##ScalarIsNa(x[i]);               \
        return flag;                                                                 \
    }

vdlint_TArrayAnyNa(Char, char);
vdlint_TArrayAnyNa(Int, int);
vdlint_TArrayAnyNa(Double, double);
vdlint_TArrayAnyNa(Pointer, void *const);
#undef vdlint_TArrayAnyNa

/*-----------------------------------------------------------------------------
 |  Array all NA check
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayAllNa(x, len) _Generic(x, char *                                       \
                                           : vdlint_CharArrayAllNa, const char *           \
                                           : vdlint_CharArrayAllNa, int *                  \
                                           : vdlint_IntArrayAllNa, const int *             \
                                           : vdlint_IntArrayAllNa, double *                \
                                           : vdlint_DoubleArrayAllNa, const double *       \
                                           : vdlint_DoubleArrayAllNa, void **              \
                                           : vdlint_PointerArrayAllNa, void *const *       \
                                           : vdlint_PointerArrayAllNa, const void **       \
                                           : vdlint_PointerArrayAllNa, const void *const * \
                                           : vdlint_PointerArrayAllNa)(x, len)

#define vdlint_TArrayAllNa(CT, QT)                                                   \
    static inline VDL_BOOL vdlint_##CT##ArrayAllNa(const QT *const x, const int len) \
    {                                                                                \
        VDL_BOOL flag          = VDL_FALSE;                                          \
        vdlint_for_i(len) flag = flag & vdlint_##CT##ScalarIsNa(x[i]);               \
        return flag;                                                                 \
    }

vdlint_TArrayAllNa(Char, char);
vdlint_TArrayAllNa(Int, int);
vdlint_TArrayAllNa(Double, double);
vdlint_TArrayAllNa(Pointer, void *const);
#undef vdlint_TArrayAllNa

/*-----------------------------------------------------------------------------
 |  Array conversion
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayToChar(x, result, len) _Generic(x, int *                                   \
                                                    : vdlint_IntArrayToChar, const int *       \
                                                    : vdlint_IntArrayToChar, double *          \
                                                    : vdlint_DoubleArrayToChar, const double * \
                                                    : vdlint_DoubleArrayToChar)(x, result, len)

#define vdlint_ArrayToInt(x, result, len) _Generic(x, char *                                 \
                                                   : vdlint_CharArrayToInt, const char *     \
                                                   : vdlint_CharArrayToInt, double *         \
                                                   : vdlint_DoubleArrayToInt, const double * \
                                                   : vdlint_DoubleArrayToInt)(x, result, len)

#define vdlint_ArrayToDouble(x, result, len) _Generic(x, char *                                \
                                                      : vdlint_CharArrayToDouble, const char * \
                                                      : vdlint_CharArrayToDouble, int *        \
                                                      : vdlint_IntArrayToDouble, const int *   \
                                                      : vdlint_IntArrayToDouble)(x, result, len)

#define vdlint_T1ArrayToT2(CT1, CT2, QT1, QT2)                                                                            \
    static inline void vdlint_##CT1##ArrayTo##CT2(const QT1 *const restrict x, QT2 *const restrict result, const int len) \
    {                                                                                                                     \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarTo##CT2(x[i]);                                                  \
    }

vdlint_T1ArrayToT2(Char, Int, char, int);
vdlint_T1ArrayToT2(Char, Double, char, double);
vdlint_T1ArrayToT2(Int, Char, int, char);
vdlint_T1ArrayToT2(Int, Double, int, double);
vdlint_T1ArrayToT2(Double, Char, double, char);
vdlint_T1ArrayToT2(Double, Int, double, int);
#undef vdlint_T1ArrayToT2

/*-----------------------------------------------------------------------------
 |  Array greater than
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayGt(x, y, result, len) _Generic(y, char *                                 \
                                                   : vdlint_ArrayGtChar(x), const char *     \
                                                   : vdlint_ArrayGtChar(x), int *            \
                                                   : vdlint_ArrayGtInt(x), const int *       \
                                                   : vdlint_ArrayGtInt(x), double *          \
                                                   : vdlint_ArrayGtDouble(x), const double * \
                                                   : vdlint_ArrayGtDouble(x))(x, y, result, len)

#define vdlint_ArrayGtChar(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayGtChar, const char *     \
                                       : vdlint_CharArrayGtChar, int *            \
                                       : vdlint_IntArrayGtChar, const int *       \
                                       : vdlint_IntArrayGtChar, double *          \
                                       : vdlint_DoubleArrayGtChar, const double * \
                                       : vdlint_DoubleArrayGtChar)

#define vdlint_ArrayGtInt(x) _Generic(x, char *                                 \
                                      : vdlint_CharArrayGtInt, const char *     \
                                      : vdlint_CharArrayGtInt, int *            \
                                      : vdlint_IntArrayGtInt, const int *       \
                                      : vdlint_IntArrayGtInt, double *          \
                                      : vdlint_DoubleArrayGtInt, const double * \
                                      : vdlint_DoubleArrayGtInt)

#define vdlint_ArrayGtDouble(x) _Generic(x, char *                                    \
                                         : vdlint_CharArrayGtDouble, const char *     \
                                         : vdlint_CharArrayGtDouble, int *            \
                                         : vdlint_IntArrayGtDouble, const int *       \
                                         : vdlint_IntArrayGtDouble, double *          \
                                         : vdlint_DoubleArrayGtDouble, const double * \
                                         : vdlint_DoubleArrayGtDouble)

#define vdlint_T1ArrayGtT2(CT1, CT2, QT1, QT2)                                                                                                   \
    static inline void vdlint_##CT1##ArrayGt##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarGt##CT2(x[i], y[i]);                                                                   \
    }

vdlint_T1ArrayGtT2(Char, Char, char, char);
vdlint_T1ArrayGtT2(Char, Int, char, int);
vdlint_T1ArrayGtT2(Char, Double, char, double);
vdlint_T1ArrayGtT2(Int, Char, int, char);
vdlint_T1ArrayGtT2(Int, Int, int, int);
vdlint_T1ArrayGtT2(Int, Double, int, double);
vdlint_T1ArrayGtT2(Double, Char, double, char);
vdlint_T1ArrayGtT2(Double, Int, double, int);
vdlint_T1ArrayGtT2(Double, Double, double, double);
#undef vdlint_T1ArrayGtT2

/*-----------------------------------------------------------------------------
 |  Array equal to
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayEq(x, y, result, len) _Generic(y, char *                                 \
                                                   : vdlint_ArrayEqChar(x), const char *     \
                                                   : vdlint_ArrayEqChar(x), int *            \
                                                   : vdlint_ArrayEqInt(x), const int *       \
                                                   : vdlint_ArrayEqInt(x), double *          \
                                                   : vdlint_ArrayEqDouble(x), const double * \
                                                   : vdlint_ArrayEqDouble(x))(x, y, result, len)

#define vdlint_ArrayEqChar(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayEqChar, const char *     \
                                       : vdlint_CharArrayEqChar, int *            \
                                       : vdlint_IntArrayEqChar, const int *       \
                                       : vdlint_IntArrayEqChar, double *          \
                                       : vdlint_DoubleArrayEqChar, const double * \
                                       : vdlint_DoubleArrayEqChar)

#define vdlint_ArrayEqInt(x) _Generic(x, char *                                 \
                                      : vdlint_CharArrayEqInt, const char *     \
                                      : vdlint_CharArrayEqInt, int *            \
                                      : vdlint_IntArrayEqInt, const int *       \
                                      : vdlint_IntArrayEqInt, double *          \
                                      : vdlint_DoubleArrayEqInt, const double * \
                                      : vdlint_DoubleArrayEqInt)

#define vdlint_ArrayEqDouble(x) _Generic(x, char *                                    \
                                         : vdlint_CharArrayEqDouble, const char *     \
                                         : vdlint_CharArrayEqDouble, int *            \
                                         : vdlint_IntArrayEqDouble, const int *       \
                                         : vdlint_IntArrayEqDouble, double *          \
                                         : vdlint_DoubleArrayEqDouble, const double * \
                                         : vdlint_DoubleArrayEqDouble)

#define vdlint_T1ArrayEqT2(CT1, CT2, QT1, QT2)                                                                                                   \
    static inline void vdlint_##CT1##ArrayEq##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarEq##CT2(x[i], y[i]);                                                                   \
    }

vdlint_T1ArrayEqT2(Char, Char, char, char);
vdlint_T1ArrayEqT2(Char, Int, char, int);
vdlint_T1ArrayEqT2(Char, Double, char, double);
vdlint_T1ArrayEqT2(Int, Char, int, char);
vdlint_T1ArrayEqT2(Int, Int, int, int);
vdlint_T1ArrayEqT2(Int, Double, int, double);
vdlint_T1ArrayEqT2(Double, Char, double, char);
vdlint_T1ArrayEqT2(Double, Int, double, int);
vdlint_T1ArrayEqT2(Double, Double, double, double);
#undef vdlint_T1ArrayEqT2


/*-----------------------------------------------------------------------------
 |  Array not equal to
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayNe(x, y, result, len) _Generic(y, char *                                 \
                                                   : vdlint_ArrayNeChar(x), const char *     \
                                                   : vdlint_ArrayNeChar(x), int *            \
                                                   : vdlint_ArrayNeInt(x), const int *       \
                                                   : vdlint_ArrayNeInt(x), double *          \
                                                   : vdlint_ArrayNeDouble(x), const double * \
                                                   : vdlint_ArrayNeDouble(x))(x, y, result, len)

#define vdlint_ArrayNeChar(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayNeChar, const char *     \
                                       : vdlint_CharArrayNeChar, int *            \
                                       : vdlint_IntArrayNeChar, const int *       \
                                       : vdlint_IntArrayNeChar, double *          \
                                       : vdlint_DoubleArrayNeChar, const double * \
                                       : vdlint_DoubleArrayNeChar)

#define vdlint_ArrayNeInt(x) _Generic(x, char *                                 \
                                      : vdlint_CharArrayNeInt, const char *     \
                                      : vdlint_CharArrayNeInt, int *            \
                                      : vdlint_IntArrayNeInt, const int *       \
                                      : vdlint_IntArrayNeInt, double *          \
                                      : vdlint_DoubleArrayNeInt, const double * \
                                      : vdlint_DoubleArrayNeInt)

#define vdlint_ArrayNeDouble(x) _Generic(x, char *                                    \
                                         : vdlint_CharArrayNeDouble, const char *     \
                                         : vdlint_CharArrayNeDouble, int *            \
                                         : vdlint_IntArrayNeDouble, const int *       \
                                         : vdlint_IntArrayNeDouble, double *          \
                                         : vdlint_DoubleArrayNeDouble, const double * \
                                         : vdlint_DoubleArrayNeDouble)

#define vdlint_T1ArrayNeT2(CT1, CT2, QT1, QT2)                                                                                                   \
    static inline void vdlint_##CT1##ArrayNe##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarNe##CT2(x[i], y[i]);                                                                   \
    }

vdlint_T1ArrayNeT2(Char, Char, char, char);
vdlint_T1ArrayNeT2(Char, Int, char, int);
vdlint_T1ArrayNeT2(Char, Double, char, double);
vdlint_T1ArrayNeT2(Int, Char, int, char);
vdlint_T1ArrayNeT2(Int, Int, int, int);
vdlint_T1ArrayNeT2(Int, Double, int, double);
vdlint_T1ArrayNeT2(Double, Char, double, char);
vdlint_T1ArrayNeT2(Double, Int, double, int);
vdlint_T1ArrayNeT2(Double, Double, double, double);
#undef vdlint_T1ArrayNeT2

/*-----------------------------------------------------------------------------
 |  Array Less than
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayLt(x, y, result, len) _Generic(y, char *                                 \
                                                   : vdlint_ArrayLtChar(x), const char *     \
                                                   : vdlint_ArrayLtChar(x), int *            \
                                                   : vdlint_ArrayLtInt(x), const int *       \
                                                   : vdlint_ArrayLtInt(x), double *          \
                                                   : vdlint_ArrayLtDouble(x), const double * \
                                                   : vdlint_ArrayLtDouble(x))(x, y, result, len)

#define vdlint_ArrayLtChar(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayLtChar, const char *     \
                                       : vdlint_CharArrayLtChar, int *            \
                                       : vdlint_IntArrayLtChar, const int *       \
                                       : vdlint_IntArrayLtChar, double *          \
                                       : vdlint_DoubleArrayLtChar, const double * \
                                       : vdlint_DoubleArrayLtChar)

#define vdlint_ArrayLtInt(x) _Generic(x, char *                                 \
                                      : vdlint_CharArrayLtInt, const char *     \
                                      : vdlint_CharArrayLtInt, int *            \
                                      : vdlint_IntArrayLtInt, const int *       \
                                      : vdlint_IntArrayLtInt, double *          \
                                      : vdlint_DoubleArrayLtInt, const double * \
                                      : vdlint_DoubleArrayLtInt)

#define vdlint_ArrayLtDouble(x) _Generic(x, char *                                    \
                                         : vdlint_CharArrayLtDouble, const char *     \
                                         : vdlint_CharArrayLtDouble, int *            \
                                         : vdlint_IntArrayLtDouble, const int *       \
                                         : vdlint_IntArrayLtDouble, double *          \
                                         : vdlint_DoubleArrayLtDouble, const double * \
                                         : vdlint_DoubleArrayLtDouble)

#define vdlint_T1ArrayLtT2(CT1, CT2, QT1, QT2)                                                                                                   \
    static inline void vdlint_##CT1##ArrayLt##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarLt##CT2(x[i], y[i]);                                                                   \
    }

vdlint_T1ArrayLtT2(Char, Char, char, char);
vdlint_T1ArrayLtT2(Char, Int, char, int);
vdlint_T1ArrayLtT2(Char, Double, char, double);
vdlint_T1ArrayLtT2(Int, Char, int, char);
vdlint_T1ArrayLtT2(Int, Int, int, int);
vdlint_T1ArrayLtT2(Int, Double, int, double);
vdlint_T1ArrayLtT2(Double, Char, double, char);
vdlint_T1ArrayLtT2(Double, Int, double, int);
vdlint_T1ArrayLtT2(Double, Double, double, double);
#undef vdlint_T1ArrayLtT2

/*-----------------------------------------------------------------------------
 |  Array greater or equal to
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayGe(x, y, result, len) _Generic(y, char *                                 \
                                                   : vdlint_ArrayGeChar(x), const char *     \
                                                   : vdlint_ArrayGeChar(x), int *            \
                                                   : vdlint_ArrayGeInt(x), const int *       \
                                                   : vdlint_ArrayGeInt(x), double *          \
                                                   : vdlint_ArrayGeDouble(x), const double * \
                                                   : vdlint_ArrayGeDouble(x))(x, y, result, len)

#define vdlint_ArrayGeChar(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayGeChar, const char *     \
                                       : vdlint_CharArrayGeChar, int *            \
                                       : vdlint_IntArrayGeChar, const int *       \
                                       : vdlint_IntArrayGeChar, double *          \
                                       : vdlint_DoubleArrayGeChar, const double * \
                                       : vdlint_DoubleArrayGeChar)

#define vdlint_ArrayGeInt(x) _Generic(x, char *                                 \
                                      : vdlint_CharArrayGeInt, const char *     \
                                      : vdlint_CharArrayGeInt, int *            \
                                      : vdlint_IntArrayGeInt, const int *       \
                                      : vdlint_IntArrayGeInt, double *          \
                                      : vdlint_DoubleArrayGeInt, const double * \
                                      : vdlint_DoubleArrayGeInt)

#define vdlint_ArrayGeDouble(x) _Generic(x, char *                                    \
                                         : vdlint_CharArrayGeDouble, const char *     \
                                         : vdlint_CharArrayGeDouble, int *            \
                                         : vdlint_IntArrayGeDouble, const int *       \
                                         : vdlint_IntArrayGeDouble, double *          \
                                         : vdlint_DoubleArrayGeDouble, const double * \
                                         : vdlint_DoubleArrayGeDouble)

#define vdlint_T1ArrayGeT2(CT1, CT2, QT1, QT2)                                                                                                   \
    static inline void vdlint_##CT1##ArrayGe##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarGe##CT2(x[i], y[i]);                                                                   \
    }

vdlint_T1ArrayGeT2(Char, Char, char, char);
vdlint_T1ArrayGeT2(Char, Int, char, int);
vdlint_T1ArrayGeT2(Char, Double, char, double);
vdlint_T1ArrayGeT2(Int, Char, int, char);
vdlint_T1ArrayGeT2(Int, Int, int, int);
vdlint_T1ArrayGeT2(Int, Double, int, double);
vdlint_T1ArrayGeT2(Double, Char, double, char);
vdlint_T1ArrayGeT2(Double, Int, double, int);
vdlint_T1ArrayGeT2(Double, Double, double, double);
#undef vdlint_T1ArrayGeT2


/*-----------------------------------------------------------------------------
 |  Array less or equal to
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayLe(x, y, result, len) _Generic(y, char *                                 \
                                                   : vdlint_ArrayLeChar(x), const char *     \
                                                   : vdlint_ArrayLeChar(x), int *            \
                                                   : vdlint_ArrayLeInt(x), const int *       \
                                                   : vdlint_ArrayLeInt(x), double *          \
                                                   : vdlint_ArrayLeDouble(x), const double * \
                                                   : vdlint_ArrayLeDouble(x))(x, y, result, len)

#define vdlint_ArrayLeChar(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayLeChar, const char *     \
                                       : vdlint_CharArrayLeChar, int *            \
                                       : vdlint_IntArrayLeChar, const int *       \
                                       : vdlint_IntArrayLeChar, double *          \
                                       : vdlint_DoubleArrayLeChar, const double * \
                                       : vdlint_DoubleArrayLeChar)

#define vdlint_ArrayLeInt(x) _Generic(x, char *                                 \
                                      : vdlint_CharArrayLeInt, const char *     \
                                      : vdlint_CharArrayLeInt, int *            \
                                      : vdlint_IntArrayLeInt, const int *       \
                                      : vdlint_IntArrayLeInt, double *          \
                                      : vdlint_DoubleArrayLeInt, const double * \
                                      : vdlint_DoubleArrayLeInt)

#define vdlint_ArrayLeDouble(x) _Generic(x, char *                                    \
                                         : vdlint_CharArrayLeDouble, const char *     \
                                         : vdlint_CharArrayLeDouble, int *            \
                                         : vdlint_IntArrayLeDouble, const int *       \
                                         : vdlint_IntArrayLeDouble, double *          \
                                         : vdlint_DoubleArrayLeDouble, const double * \
                                         : vdlint_DoubleArrayLeDouble)

#define vdlint_T1ArrayLeT2(CT1, CT2, QT1, QT2)                                                                                                   \
    static inline void vdlint_##CT1##ArrayLe##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarLe##CT2(x[i], y[i]);                                                                   \
    }

vdlint_T1ArrayLeT2(Char, Char, char, char);
vdlint_T1ArrayLeT2(Char, Int, char, int);
vdlint_T1ArrayLeT2(Char, Double, char, double);
vdlint_T1ArrayLeT2(Int, Char, int, char);
vdlint_T1ArrayLeT2(Int, Int, int, int);
vdlint_T1ArrayLeT2(Int, Double, int, double);
vdlint_T1ArrayLeT2(Double, Char, double, char);
vdlint_T1ArrayLeT2(Double, Int, double, int);
vdlint_T1ArrayLeT2(Double, Double, double, double);
#undef vdlint_T1ArrayLeT2

/*-----------------------------------------------------------------------------
 |  Array and
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayAnd(x, y, result, len) _Generic(y, char *                                  \
                                                    : vdlint_ArrayAndChar(x), const char *     \
                                                    : vdlint_ArrayAndChar(x), int *            \
                                                    : vdlint_ArrayAndInt(x), const int *       \
                                                    : vdlint_ArrayAndInt(x), double *          \
                                                    : vdlint_ArrayAndDouble(x), const double * \
                                                    : vdlint_ArrayAndDouble(x))(x, y, result, len)

#define vdlint_ArrayAndChar(x) _Generic(x, char *                                   \
                                        : vdlint_CharArrayAndChar, const char *     \
                                        : vdlint_CharArrayAndChar, int *            \
                                        : vdlint_IntArrayAndChar, const int *       \
                                        : vdlint_IntArrayAndChar, double *          \
                                        : vdlint_DoubleArrayAndChar, const double * \
                                        : vdlint_DoubleArrayAndChar)

#define vdlint_ArrayAndInt(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayAndInt, const char *     \
                                       : vdlint_CharArrayAndInt, int *            \
                                       : vdlint_IntArrayAndInt, const int *       \
                                       : vdlint_IntArrayAndInt, double *          \
                                       : vdlint_DoubleArrayAndInt, const double * \
                                       : vdlint_DoubleArrayAndInt)

#define vdlint_ArrayAndDouble(x) _Generic(x, char *                                     \
                                          : vdlint_CharArrayAndDouble, const char *     \
                                          : vdlint_CharArrayAndDouble, int *            \
                                          : vdlint_IntArrayAndDouble, const int *       \
                                          : vdlint_IntArrayAndDouble, double *          \
                                          : vdlint_DoubleArrayAndDouble, const double * \
                                          : vdlint_DoubleArrayAndDouble)

#define vdlint_T1ArrayAndT2(CT1, CT2, QT1, QT2)                                                                                                   \
    static inline void vdlint_##CT1##ArrayAnd##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                             \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarAnd##CT2(x[i], y[i]);                                                                   \
    }

vdlint_T1ArrayAndT2(Char, Char, char, char);
vdlint_T1ArrayAndT2(Char, Int, char, int);
vdlint_T1ArrayAndT2(Char, Double, char, double);
vdlint_T1ArrayAndT2(Int, Char, int, char);
vdlint_T1ArrayAndT2(Int, Int, int, int);
vdlint_T1ArrayAndT2(Int, Double, int, double);
vdlint_T1ArrayAndT2(Double, Char, double, char);
vdlint_T1ArrayAndT2(Double, Int, double, int);
vdlint_T1ArrayAndT2(Double, Double, double, double);
#undef vdlint_T1ArrayAndT2


/*-----------------------------------------------------------------------------
 |  Array or
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayOr(x, y, result, len) _Generic(y, char *                                 \
                                                   : vdlint_ArrayOrChar(x), const char *     \
                                                   : vdlint_ArrayOrChar(x), int *            \
                                                   : vdlint_ArrayOrInt(x), const int *       \
                                                   : vdlint_ArrayOrInt(x), double *          \
                                                   : vdlint_ArrayOrDouble(x), const double * \
                                                   : vdlint_ArrayOrDouble(x))(x, y, result, len)

#define vdlint_ArrayOrChar(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayOrChar, const char *     \
                                       : vdlint_CharArrayOrChar, int *            \
                                       : vdlint_IntArrayOrChar, const int *       \
                                       : vdlint_IntArrayOrChar, double *          \
                                       : vdlint_DoubleArrayOrChar, const double * \
                                       : vdlint_DoubleArrayOrChar)

#define vdlint_ArrayOrInt(x) _Generic(x, char *                                 \
                                      : vdlint_CharArrayOrInt, const char *     \
                                      : vdlint_CharArrayOrInt, int *            \
                                      : vdlint_IntArrayOrInt, const int *       \
                                      : vdlint_IntArrayOrInt, double *          \
                                      : vdlint_DoubleArrayOrInt, const double * \
                                      : vdlint_DoubleArrayOrInt)

#define vdlint_ArrayOrDouble(x) _Generic(x, char *                                    \
                                         : vdlint_CharArrayOrDouble, const char *     \
                                         : vdlint_CharArrayOrDouble, int *            \
                                         : vdlint_IntArrayOrDouble, const int *       \
                                         : vdlint_IntArrayOrDouble, double *          \
                                         : vdlint_DoubleArrayOrDouble, const double * \
                                         : vdlint_DoubleArrayOrDouble)

#define vdlint_T1ArrayOrT2(CT1, CT2, QT1, QT2)                                                                                                   \
    static inline void vdlint_##CT1##ArrayOr##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarOr##CT2(x[i], y[i]);                                                                   \
    }

vdlint_T1ArrayOrT2(Char, Char, char, char);
vdlint_T1ArrayOrT2(Char, Int, char, int);
vdlint_T1ArrayOrT2(Char, Double, char, double);
vdlint_T1ArrayOrT2(Int, Char, int, char);
vdlint_T1ArrayOrT2(Int, Int, int, int);
vdlint_T1ArrayOrT2(Int, Double, int, double);
vdlint_T1ArrayOrT2(Double, Char, double, char);
vdlint_T1ArrayOrT2(Double, Int, double, int);
vdlint_T1ArrayOrT2(Double, Double, double, double);
#undef vdlint_T1ArrayOrT2


/*-----------------------------------------------------------------------------
 |  Array not
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayNot(x, result, len) _Generic(x, char *                               \
                                                 : vdlint_CharArrayNot, const char *     \
                                                 : vdlint_CharArrayNot, int *            \
                                                 : vdlint_IntArrayNot, const int *       \
                                                 : vdlint_IntArrayNot, double *          \
                                                 : vdlint_DoubleArrayNot, const double * \
                                                 : vdlint_DoubleArrayNot)(x, result, len)

#define vdlint_TArrayNot(CT, QT)                                                                                    \
    static inline void vdlint_##CT##ArrayNot(const QT *const restrict x, int *const restrict result, const int len) \
    {                                                                                                               \
        vdlint_for_i(len) result[i] = vdlint_##CT##ScalarNot(x[i]);                                                 \
    }

vdlint_TArrayNot(Char, char);
vdlint_TArrayNot(Int, int);
vdlint_TArrayNot(Double, double);
#undef vdlint_TArrayNot

/*-----------------------------------------------------------------------------
 |  Array add
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayAdd(x, y, result, len) _Generic(y, char *                                  \
                                                    : vdlint_ArrayAddChar(x), const char *     \
                                                    : vdlint_ArrayAddChar(x), int *            \
                                                    : vdlint_ArrayAddInt(x), const int *       \
                                                    : vdlint_ArrayAddInt(x), double *          \
                                                    : vdlint_ArrayAddDouble(x), const double * \
                                                    : vdlint_ArrayAddDouble(x))(x, y, result, len)

#define vdlint_ArrayAddChar(x) _Generic(x, char *                                   \
                                        : vdlint_CharArrayAddChar, const char *     \
                                        : vdlint_CharArrayAddChar, int *            \
                                        : vdlint_IntArrayAddChar, const int *       \
                                        : vdlint_IntArrayAddChar, double *          \
                                        : vdlint_DoubleArrayAddChar, const double * \
                                        : vdlint_DoubleArrayAddChar)

#define vdlint_ArrayAddInt(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayAddInt, const char *     \
                                       : vdlint_CharArrayAddInt, int *            \
                                       : vdlint_IntArrayAddInt, const int *       \
                                       : vdlint_IntArrayAddInt, double *          \
                                       : vdlint_DoubleArrayAddInt, const double * \
                                       : vdlint_DoubleArrayAddInt)

#define vdlint_ArrayAddDouble(x) _Generic(x, char *                                     \
                                          : vdlint_CharArrayAddDouble, const char *     \
                                          : vdlint_CharArrayAddDouble, int *            \
                                          : vdlint_IntArrayAddDouble, const int *       \
                                          : vdlint_IntArrayAddDouble, double *          \
                                          : vdlint_DoubleArrayAddDouble, const double * \
                                          : vdlint_DoubleArrayAddDouble)

#define vdlint_T1ArrayAddT2(CT1, CT2, QT1, QT2, RT)                                                                                              \
    static inline void vdlint_##CT1##ArrayAdd##CT2(const QT1 *const restrict x, const QT2 *const restrict y, RT *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarAdd##CT2(x[i], y[i]);                                                                  \
    }

#define vdlint_T1ArrayAddT2_overflow(CT1, CT2, QT1, QT2)                                                                                          \
    static inline void vdlint_##CT1##ArrayAdd##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                             \
        int global_overflow = 0;                                                                                                                  \
        vdlint_for_i(len)                                                                                                                         \
        {                                                                                                                                         \
            int overflow = 0;                                                                                                                     \
            vdlint_##CT1##ScalarAdd##CT2(x[i], y[i], result[i], overflow);                                                                        \
            global_overflow = global_overflow | overflow;                                                                                         \
        }                                                                                                                                         \
    }

vdlint_T1ArrayAddT2(Char, Char, char, char, int);
vdlint_T1ArrayAddT2_overflow(Char, Int, char, int);
vdlint_T1ArrayAddT2(Char, Double, char, double, double);
vdlint_T1ArrayAddT2_overflow(Int, Char, int, char);
vdlint_T1ArrayAddT2_overflow(Int, Int, int, int);
vdlint_T1ArrayAddT2(Int, Double, int, double, double);
vdlint_T1ArrayAddT2(Double, Char, double, char, double);
vdlint_T1ArrayAddT2(Double, Int, double, int, double);
vdlint_T1ArrayAddT2(Double, Double, double, double, double);
#undef vdlint_T1ArrayAndT2
#undef vdlint_T1ArrayAddT2_overflow

/*-----------------------------------------------------------------------------
 |  Array subtract
 ----------------------------------------------------------------------------*/

#define vdlint_ArraySub(x, y, result, len) _Generic(y, char *                                  \
                                                    : vdlint_ArraySubChar(x), const char *     \
                                                    : vdlint_ArraySubChar(x), int *            \
                                                    : vdlint_ArraySubInt(x), const int *       \
                                                    : vdlint_ArraySubInt(x), double *          \
                                                    : vdlint_ArraySubDouble(x), const double * \
                                                    : vdlint_ArraySubDouble(x))(x, y, result, len)

#define vdlint_ArraySubChar(x) _Generic(x, char *                                   \
                                        : vdlint_CharArraySubChar, const char *     \
                                        : vdlint_CharArraySubChar, int *            \
                                        : vdlint_IntArraySubChar, const int *       \
                                        : vdlint_IntArraySubChar, double *          \
                                        : vdlint_DoubleArraySubChar, const double * \
                                        : vdlint_DoubleArraySubChar)

#define vdlint_ArraySubInt(x) _Generic(x, char *                                  \
                                       : vdlint_CharArraySubInt, const char *     \
                                       : vdlint_CharArraySubInt, int *            \
                                       : vdlint_IntArraySubInt, const int *       \
                                       : vdlint_IntArraySubInt, double *          \
                                       : vdlint_DoubleArraySubInt, const double * \
                                       : vdlint_DoubleArraySubInt)

#define vdlint_ArraySubDouble(x) _Generic(x, char *                                     \
                                          : vdlint_CharArraySubDouble, const char *     \
                                          : vdlint_CharArraySubDouble, int *            \
                                          : vdlint_IntArraySubDouble, const int *       \
                                          : vdlint_IntArraySubDouble, double *          \
                                          : vdlint_DoubleArraySubDouble, const double * \
                                          : vdlint_DoubleArraySubDouble)

#define vdlint_T1ArraySubT2(CT1, CT2, QT1, QT2, RT)                                                                                              \
    static inline void vdlint_##CT1##ArraySub##CT2(const QT1 *const restrict x, const QT2 *const restrict y, RT *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarSub##CT2(x[i], y[i]);                                                                  \
    }

#define vdlint_T1ArraySubT2_overflow(CT1, CT2, QT1, QT2)                                                                                          \
    static inline void vdlint_##CT1##ArraySub##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                             \
        int global_overflow = 0;                                                                                                                  \
        vdlint_for_i(len)                                                                                                                         \
        {                                                                                                                                         \
            int overflow = 0;                                                                                                                     \
            vdlint_##CT1##ScalarSub##CT2(x[i], y[i], result[i], overflow);                                                                        \
            global_overflow = global_overflow | overflow;                                                                                         \
        }                                                                                                                                         \
    }

vdlint_T1ArraySubT2(Char, Char, char, char, int);
vdlint_T1ArraySubT2_overflow(Char, Int, char, int);
vdlint_T1ArraySubT2(Char, Double, char, double, double);
vdlint_T1ArraySubT2_overflow(Int, Char, int, char);
vdlint_T1ArraySubT2_overflow(Int, Int, int, int);
vdlint_T1ArraySubT2(Int, Double, int, double, double);
vdlint_T1ArraySubT2(Double, Char, double, char, double);
vdlint_T1ArraySubT2(Double, Int, double, int, double);
vdlint_T1ArraySubT2(Double, Double, double, double, double);
#undef vdlint_T1ArrayAndT2
#undef vdlint_T1ArraySubT2_overflow


/*-----------------------------------------------------------------------------
 |  Array multiply
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayMul(x, y, result, len) _Generic(y, char *                                  \
                                                    : vdlint_ArrayMulChar(x), const char *     \
                                                    : vdlint_ArrayMulChar(x), int *            \
                                                    : vdlint_ArrayMulInt(x), const int *       \
                                                    : vdlint_ArrayMulInt(x), double *          \
                                                    : vdlint_ArrayMulDouble(x), const double * \
                                                    : vdlint_ArrayMulDouble(x))(x, y, result, len)

#define vdlint_ArrayMulChar(x) _Generic(x, char *                                   \
                                        : vdlint_CharArrayMulChar, const char *     \
                                        : vdlint_CharArrayMulChar, int *            \
                                        : vdlint_IntArrayMulChar, const int *       \
                                        : vdlint_IntArrayMulChar, double *          \
                                        : vdlint_DoubleArrayMulChar, const double * \
                                        : vdlint_DoubleArrayMulChar)

#define vdlint_ArrayMulInt(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayMulInt, const char *     \
                                       : vdlint_CharArrayMulInt, int *            \
                                       : vdlint_IntArrayMulInt, const int *       \
                                       : vdlint_IntArrayMulInt, double *          \
                                       : vdlint_DoubleArrayMulInt, const double * \
                                       : vdlint_DoubleArrayMulInt)

#define vdlint_ArrayMulDouble(x) _Generic(x, char *                                     \
                                          : vdlint_CharArrayMulDouble, const char *     \
                                          : vdlint_CharArrayMulDouble, int *            \
                                          : vdlint_IntArrayMulDouble, const int *       \
                                          : vdlint_IntArrayMulDouble, double *          \
                                          : vdlint_DoubleArrayMulDouble, const double * \
                                          : vdlint_DoubleArrayMulDouble)

#define vdlint_T1ArrayMulT2(CT1, CT2, QT1, QT2, RT)                                                                                              \
    static inline void vdlint_##CT1##ArrayMul##CT2(const QT1 *const restrict x, const QT2 *const restrict y, RT *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarMul##CT2(x[i], y[i]);                                                                  \
    }

#define vdlint_T1ArrayMulT2_overflow(CT1, CT2, QT1, QT2)                                                                                          \
    static inline void vdlint_##CT1##ArrayMul##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *restrict result, const int len) \
    {                                                                                                                                             \
        int global_overflow = 0;                                                                                                                  \
        vdlint_for_i(len)                                                                                                                         \
        {                                                                                                                                         \
            int overflow = 0;                                                                                                                     \
            vdlint_##CT1##ScalarMul##CT2(x[i], y[i], result[i], overflow);                                                                        \
            global_overflow = global_overflow | overflow;                                                                                         \
        }                                                                                                                                         \
    }

vdlint_T1ArrayMulT2(Char, Char, char, char, int);
vdlint_T1ArrayMulT2_overflow(Char, Int, char, int);
vdlint_T1ArrayMulT2(Char, Double, char, double, double);
vdlint_T1ArrayMulT2_overflow(Int, Char, int, char);
vdlint_T1ArrayMulT2_overflow(Int, Int, int, int);
vdlint_T1ArrayMulT2(Int, Double, int, double, double);
vdlint_T1ArrayMulT2(Double, Char, double, char, double);
vdlint_T1ArrayMulT2(Double, Int, double, int, double);
vdlint_T1ArrayMulT2(Double, Double, double, double, double);
#undef vdlint_T1ArrayMulT2
#undef vdlint_T1ArrayMulT2_overflow

/*-----------------------------------------------------------------------------
 |  Array division
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayDiv(x, y, result, len) _Generic(y, char *                                  \
                                                    : vdlint_ArrayDivChar(x), const char *     \
                                                    : vdlint_ArrayDivChar(x), int *            \
                                                    : vdlint_ArrayDivInt(x), const int *       \
                                                    : vdlint_ArrayDivInt(x), double *          \
                                                    : vdlint_ArrayDivDouble(x), const double * \
                                                    : vdlint_ArrayDivDouble(x))(x, y, result, len)

#define vdlint_ArrayDivChar(x) _Generic(x, char *                                   \
                                        : vdlint_CharArrayDivChar, const char *     \
                                        : vdlint_CharArrayDivChar, int *            \
                                        : vdlint_IntArrayDivChar, const int *       \
                                        : vdlint_IntArrayDivChar, double *          \
                                        : vdlint_DoubleArrayDivChar, const double * \
                                        : vdlint_DoubleArrayDivChar)

#define vdlint_ArrayDivInt(x) _Generic(x, char *                                  \
                                       : vdlint_CharArrayDivInt, const char *     \
                                       : vdlint_CharArrayDivInt, int *            \
                                       : vdlint_IntArrayDivInt, const int *       \
                                       : vdlint_IntArrayDivInt, double *          \
                                       : vdlint_DoubleArrayDivInt, const double * \
                                       : vdlint_DoubleArrayDivInt)

#define vdlint_ArrayDivDouble(x) _Generic(x, char *                                     \
                                          : vdlint_CharArrayDivDouble, const char *     \
                                          : vdlint_CharArrayDivDouble, int *            \
                                          : vdlint_IntArrayDivDouble, const int *       \
                                          : vdlint_IntArrayDivDouble, double *          \
                                          : vdlint_DoubleArrayDivDouble, const double * \
                                          : vdlint_DoubleArrayDivDouble)

#define vdlint_T1ArrayDivT2(CT1, CT2, QT1, QT2, RT)                                                                                              \
    static inline void vdlint_##CT1##ArrayDiv##CT2(const QT1 *const restrict x, const QT2 *const restrict y, RT *restrict result, const int len) \
    {                                                                                                                                            \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarDiv##CT2(x[i], y[i]);                                                                  \
    }

vdlint_T1ArrayDivT2(Char, Char, char, char, double);
vdlint_T1ArrayDivT2(Char, Int, char, int, double);
vdlint_T1ArrayDivT2(Char, Double, char, double, double);
vdlint_T1ArrayDivT2(Int, Char, int, char, double);
vdlint_T1ArrayDivT2(Int, Int, int, int, double);
vdlint_T1ArrayDivT2(Int, Double, int, double, double);
vdlint_T1ArrayDivT2(Double, Char, double, char, double);
vdlint_T1ArrayDivT2(Double, Int, double, int, double);
vdlint_T1ArrayDivT2(Double, Double, double, double, double);
#undef vdlint_T1ArrayAndT2


/*-----------------------------------------------------------------------------
 |  Array augmented add
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayIadd(x, y, len) _Generic(y, char *                                   \
                                             : vdlint_ArrayIaddChar(x), const char *     \
                                             : vdlint_ArrayIaddChar(x), int *            \
                                             : vdlint_ArrayIaddInt(x), const int *       \
                                             : vdlint_ArrayIaddInt(x), double *          \
                                             : vdlint_ArrayIaddDouble(x), const double * \
                                             : vdlint_ArrayIaddDouble(x))(x, y, len)

#define vdlint_ArrayIaddChar(x) _Generic(x, int *                                     \
                                         : vdlint_IntArrayIaddChar, const int *       \
                                         : vdlint_IntArrayIaddChar, double *          \
                                         : vdlint_DoubleArrayIaddChar, const double * \
                                         : vdlint_DoubleArrayIaddChar)

#define vdlint_ArrayIaddInt(x) _Generic(x, int *                                    \
                                        : vdlint_IntArrayIaddInt, const int *       \
                                        : vdlint_IntArrayIaddInt, double *          \
                                        : vdlint_DoubleArrayIaddInt, const double * \
                                        : vdlint_DoubleArrayIaddInt)

#define vdlint_ArrayIaddDouble(x) _Generic(x, double *                                    \
                                           : vdlint_DoubleArrayIaddDouble, const double * \
                                           : vdlint_DoubleArrayIaddDouble)

#define vdlint_T1ArrayIaddT2(CT1, CT2, QT1, QT2)                                                                           \
    static inline VDL_BOOL vdlint_##CT1##ArrayIadd##CT2(QT1 *const restrict x, const QT2 *const restrict y, const int len) \
    {                                                                                                                      \
        int overflow           = VDL_FALSE;                                                                                \
        vdlint_for_i(len) x[i] = vdlint_##CT1##ScalarAdd##CT2(x[i], y[i]);                                                 \
    }

vdlint_T1ArrayIaddT2(Int, Char, int, char);
vdlint_T1ArrayIaddT2(Int, Int, int, int);
vdlint_T1ArrayIaddT2(Double, Char, double, char);
vdlint_T1ArrayIaddT2(Double, Int, double, int);
vdlint_T1ArrayIaddT2(Double, Double, double, double);
#undef vdlint_T1ArrayAndT2

/*-----------------------------------------------------------------------------
 |  Array augmented subtract
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayIsub(x, y, len) _Generic(y, char *                                   \
                                             : vdlint_ArrayIsubChar(x), const char *     \
                                             : vdlint_ArrayIsubChar(x), int *            \
                                             : vdlint_ArrayIsubInt(x), const int *       \
                                             : vdlint_ArrayIsubInt(x), double *          \
                                             : vdlint_ArrayIsubDouble(x), const double * \
                                             : vdlint_ArrayIsubDouble(x))(x, y, len)

#define vdlint_ArrayIsubChar(x) _Generic(x, int *                                     \
                                         : vdlint_IntArrayIsubChar, const int *       \
                                         : vdlint_IntArrayIsubChar, double *          \
                                         : vdlint_DoubleArrayIsubChar, const double * \
                                         : vdlint_DoubleArrayIsubChar)

#define vdlint_ArrayIsubInt(x) _Generic(x, int *                                    \
                                        : vdlint_IntArrayIsubInt, const int *       \
                                        : vdlint_IntArrayIsubInt, double *          \
                                        : vdlint_DoubleArrayIsubInt, const double * \
                                        : vdlint_DoubleArrayIsubInt)

#define vdlint_ArrayIsubDouble(x) _Generic(x, double *                                    \
                                           : vdlint_DoubleArrayIsubDouble, const double * \
                                           : vdlint_DoubleArrayIsubDouble)

#define vdlint_T1ArrayIsubT2(CT1, CT2, QT1, QT2)                                                                       \
    static inline void vdlint_##CT1##ArrayIsub##CT2(QT1 *const restrict x, const QT2 *const restrict y, const int len) \
    {                                                                                                                  \
        vdlint_for_i(len) x[i] = vdlint_##CT1##ScalarSub##CT2(x[i], y[i]);                                             \
    }

vdlint_T1ArrayIsubT2(Int, Char, int, char);
vdlint_T1ArrayIsubT2(Int, Int, int, int);
vdlint_T1ArrayIsubT2(Double, Char, double, char);
vdlint_T1ArrayIsubT2(Double, Int, double, int);
vdlint_T1ArrayIsubT2(Double, Double, double, double);
#undef vdlint_T1ArrayAndT2


/*-----------------------------------------------------------------------------
 |  Array augmented multiply
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayImul(x, y, len) _Generic(y, char *                                   \
                                             : vdlint_ArrayImulChar(x), const char *     \
                                             : vdlint_ArrayImulChar(x), int *            \
                                             : vdlint_ArrayImulInt(x), const int *       \
                                             : vdlint_ArrayImulInt(x), double *          \
                                             : vdlint_ArrayImulDouble(x), const double * \
                                             : vdlint_ArrayImulDouble(x))(x, y, len)

#define vdlint_ArrayImulChar(x) _Generic(x, int *                                     \
                                         : vdlint_IntArrayImulChar, const int *       \
                                         : vdlint_IntArrayImulChar, double *          \
                                         : vdlint_DoubleArrayImulChar, const double * \
                                         : vdlint_DoubleArrayImulChar)

#define vdlint_ArrayImulInt(x) _Generic(x, int *                                    \
                                        : vdlint_IntArrayImulInt, const int *       \
                                        : vdlint_IntArrayImulInt, double *          \
                                        : vdlint_DoubleArrayImulInt, const double * \
                                        : vdlint_DoubleArrayImulInt)

#define vdlint_ArrayImulDouble(x) _Generic(x, double *                                    \
                                           : vdlint_DoubleArrayImulDouble, const double * \
                                           : vdlint_DoubleArrayImulDouble)

#define vdlint_T1ArrayImulT2(CT1, CT2, QT1, QT2)                                                                       \
    static inline void vdlint_##CT1##ArrayImul##CT2(QT1 *const restrict x, const QT2 *const restrict y, const int len) \
    {                                                                                                                  \
        vdlint_for_i(len) x[i] = vdlint_##CT1##ScalarMul##CT2(x[i], y[i]);                                             \
    }

vdlint_T1ArrayImulT2(Int, Char, int, char);
vdlint_T1ArrayImulT2(Int, Int, int, int);
vdlint_T1ArrayImulT2(Double, Char, double, char);
vdlint_T1ArrayImulT2(Double, Int, double, int);
vdlint_T1ArrayImulT2(Double, Double, double, double);
#undef vdlint_T1ArrayAndT2


/*-----------------------------------------------------------------------------
 |  Array augmented division
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayIdiv(x, y, len) _Generic(y, char *                                   \
                                             : vdlint_ArrayIdivChar(x), const char *     \
                                             : vdlint_ArrayIdivChar(x), int *            \
                                             : vdlint_ArrayIdivInt(x), const int *       \
                                             : vdlint_ArrayIdivInt(x), double *          \
                                             : vdlint_ArrayIdivDouble(x), const double * \
                                             : vdlint_ArrayIdivDouble(x))(x, y, len)

#define vdlint_ArrayIdivChar(x) _Generic(x, double *                                  \
                                         : vdlint_DoubleArrayIdivChar, const double * \
                                         : vdlint_DoubleArrayIdivChar)

#define vdlint_ArrayIdivInt(x) _Generic(x, double *                                 \
                                        : vdlint_DoubleArrayIdivInt, const double * \
                                        : vdlint_DoubleArrayIdivInt)

#define vdlint_ArrayIdivDouble(x) _Generic(x, double *                                    \
                                           : vdlint_DoubleArrayIdivDouble, const double * \
                                           : vdlint_DoubleArrayIdivDouble)

#define vdlint_T1ArrayIdivT2(CT1, CT2, QT1, QT2)                                                                       \
    static inline void vdlint_##CT1##ArrayIdiv##CT2(QT1 *const restrict x, const QT2 *const restrict y, const int len) \
    {                                                                                                                  \
        vdlint_for_i(len) x[i] = vdlint_##CT1##ScalarDiv##CT2(x[i], y[i]);                                             \
    }

vdlint_T1ArrayIdivT2(Double, Char, double, char);
vdlint_T1ArrayIdivT2(Double, Int, double, int);
vdlint_T1ArrayIdivT2(Double, Double, double, double);
#undef vdlint_T1ArrayAndT2

/*-----------------------------------------------------------------------------
 |  Array negative
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayNeg(x, result, len) _Generic(x, int *                                \
                                                 : vdlint_IntArrayNeg, const int *       \
                                                 : vdlint_IntArrayNeg, double *          \
                                                 : vdlint_DoubleArrayNeg, const double * \
                                                 : vdlint_DoubleArrayNeg)(x, result, len)

#define vdlint_TArrayNeg(CT, QT)                                                                                   \
    static inline void vdlint_##CT##ArrayNeg(const QT *const restrict x, QT *const restrict result, const int len) \
    {                                                                                                              \
        vdlint_for_i(len) result[i] = vdlint_##CT##ScalarNeg(x[i]);                                                \
    }

vdlint_TArrayNeg(Int, int);
vdlint_TArrayNeg(Double, double);
#undef vdlint_TArrayNeg

/*-----------------------------------------------------------------------------
 |  Array modular
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayMod(x, y, result, len) _Generic(y, char *                              \
                                                    : vdlint_ArrayModChar(x), const char * \
                                                    : vdlint_ArrayModChar(x), int *        \
                                                    : vdlint_ArrayModInt(x), const int *   \
                                                    : vdlint_ArrayModInt(x))(x, y, result, len)

#define vdlint_ArrayModChar(x) _Generic(x, char *                               \
                                        : vdlint_CharArrayModChar, const char * \
                                        : vdlint_CharArrayModChar, int *        \
                                        : vdlint_IntArrayModChar, const int *   \
                                        : vdlint_IntArrayModChar)

#define vdlint_ArrayModInt(x) _Generic(x, char *                              \
                                       : vdlint_CharArrayModInt, const char * \
                                       : vdlint_CharArrayModInt, int *        \
                                       : vdlint_IntArrayModInt, const int *   \
                                       : vdlint_IntArrayModInt)

#define vdlint_T1ArrayModT2(CT1, CT2, QT1, QT2)                                                                                                \
    static inline void vdlint_##CT1##ArrayMod##CT2(const QT1 *const restrict x, const QT2 *const restrict y, int *const result, const int len) \
    {                                                                                                                                          \
        vdlint_for_i(len) result[i] = vdlint_##CT1##ScalarMod##CT2(x[i], y[i]);                                                                \
    }

vdlint_T1ArrayModT2(Char, Char, char, char);
vdlint_T1ArrayModT2(Char, Int, char, int);
vdlint_T1ArrayModT2(Int, Char, int, char);
vdlint_T1ArrayModT2(Int, Int, int, int);
#undef vdlint_T1ArrayModT2

/*-----------------------------------------------------------------------------
 |  Array augmented modular
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayImod(x, y, len) _Generic(y, char *                               \
                                             : vdlint_IntArrayImodChar, const char * \
                                             : vdlint_IntArrayImodChar, int *        \
                                             : vdlint_IntArrayImodInt, const int *   \
                                             : vdlint_IntArrayImodInt)(x, y, len)

#define vdlint_T1ArrayImodT2(CT1, CT2, QT1, QT2)                                                                       \
    static inline void vdlint_##CT1##ArrayImod##CT2(QT1 *const restrict x, const QT2 *const restrict y, const int len) \
    {                                                                                                                  \
        vdlint_for_i(len) x[i] = vdlint_##CT1##ScalarMod##CT2(x[i], y[i]);                                             \
    }

vdlint_T1ArrayImodT2(Int, Char, int, char);
vdlint_T1ArrayImodT2(Int, Int, int, int);
#undef vdlint_T1ArrayImodT2

#endif//VDL_VDLARROP_H
