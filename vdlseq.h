//
// Created by Patrick Li on 7/10/22.
//

#ifndef VDL_VDLSEQ_H
#define VDL_VDLSEQ_H

#include "vdlportal.h"

/*-----------------------------------------------------------------------------
 |  Generate Integer sequence
 ----------------------------------------------------------------------------*/

#define vdl_IntSeq(...) vdl_Caller(vdl_IntSeq_BT, vdl_vp, __VA_ARGS__)
static inline vdl_vp vdl_IntSeq_BT(vdl_bt bt, const int begin, const int end)
{
    vdl_PushBT(bt);
    const int length = (end - begin) * ((end - begin > 0) - (end - begin < 0)) + 1;
    vdl_vp sequence  = vdl_New(VDL_TYPE_INT, length);
    int inc          = (end - begin > 0) - (end - begin < 0);
    int start        = begin;

    vdl_For_i(length)
    {
        vdl_IAppend(sequence, &start, 1);
        start += inc;
    }
    return sequence;
VDL_EXCEPTION:
    return NULL;
}

#endif//VDL_VDLSEQ_H
