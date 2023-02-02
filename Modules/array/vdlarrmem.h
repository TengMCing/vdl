//
// Created by Patrick Li on 5/1/23.
//

#ifndef VDL_VDLARRMEM_H
#define VDL_VDLARRMEM_H

#include "../util/vdlUtility.h"
#include <stdlib.h>

#define vdlint_TArrayInit(CT, QT)                                            \
    static inline QT *vdlint_##CT##ArrayInit(const int size, const QT value) \
    {                                                                        \
        if (size <= 0)                                                       \
            return NULL;                                                     \
        QT *buffer = malloc((size_t) size * sizeof(int));                    \
        if (buffer != NULL)                                                  \
            vdlint_for_i(size) buffer[i] = value;                            \
        return buffer;                                                       \
    }

vdlint_TArrayInit(Char, char);
vdlint_TArrayInit(Int, int);
vdlint_TArrayInit(Double, double);


#endif//VDL_VDLARRMEM_H
