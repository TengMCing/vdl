//
// Created by Patrick Li on 5/1/23.
//

#ifndef VDL_VDLARRIO_H
#define VDL_VDLARRIO_H

#include "vdlscalarop.h"
#include <stdio.h>

/*-----------------------------------------------------------------------------
 |  Array fill
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayFill(x, value, len) _Generic(x, char *                                      \
                                                 : vdlint_CharArrayFill, const char *           \
                                                 : vdlint_CharArrayFill, int *                  \
                                                 : vdlint_IntArrayFill, const int *             \
                                                 : vdlint_IntArrayFill, double *                \
                                                 : vdlint_DoubleArrayFill, const double *       \
                                                 : vdlint_DoubleArrayFill, void **              \
                                                 : vdlint_PointerArrayFill, const void **       \
                                                 : vdlint_PointerArrayFill, void *const *       \
                                                 : vdlint_PointerArrayFill, const void *const * \
                                                 : vdlint_PointerArrayFill)(x, value, len)

#define vdlint_TArrayFill(CT, QT1, QT2)                                                     \
    static inline void vdlint_##CT##ArrayFill(QT1 *const x, const QT2 value, const int len) \
    {                                                                                       \
        vdlint_for_i(len) x[i] = value;                                                     \
    }

vdlint_TArrayFill(Char, char, char);
vdlint_TArrayFill(Int, int, int);
vdlint_TArrayFill(Double, double, double);
vdlint_TArrayFill(Pointer, const void *, void *const);
#undef vdlint_TArrayFill

/*-----------------------------------------------------------------------------
 |  Array print
 ----------------------------------------------------------------------------*/

#define vdlint_ArrayPrint(x, len, end) _Generic(x, char *                                       \
                                                : vdlint_CharArrayPrint, const char *           \
                                                : vdlint_CharArrayPrint, int *                  \
                                                : vdlint_IntArrayPrint, const int *             \
                                                : vdlint_IntArrayPrint, double *                \
                                                : vdlint_DoubleArrayPrint, const double *       \
                                                : vdlint_DoubleArrayPrint, void **              \
                                                : vdlint_PointerArrayPrint, const void **       \
                                                : vdlint_PointerArrayPrint, void *const *       \
                                                : vdlint_PointerArrayPrint, const void *const * \
                                                : vdlint_PointerArrayPrint)(x, len, end)

#define vdlint_TArrayPrint(CT, QT, FT)                                                            \
    static inline void vdlint_##CT##ArrayPrint(const QT *const x, const int len, const char *end) \
    {                                                                                             \
        fputs("[", stdout);                                                                       \
        vdlint_for_i(len)                                                                         \
        {                                                                                         \
            if (vdlint_##CT##ScalarIsNa(x[i]))                                                    \
                fputs("NA", stdout);                                                              \
            else                                                                                  \
                printf(FT, x[i]);                                                                 \
            if (i != len - 1)                                                                     \
                fputs(", ", stdout);                                                              \
        };                                                                                        \
        fputs("]", stdout);                                                                       \
        fputs(end, stdout);                                                                       \
    }

vdlint_TArrayPrint(Char, char, "'%c'");
vdlint_TArrayPrint(Int, int, "%d");
vdlint_TArrayPrint(Double, double, "%.2f");
vdlint_TArrayPrint(Pointer, void *const, "<%p>");
#undef vdlint_TArrayPrint


#endif//VDL_VDLARRIO_H
