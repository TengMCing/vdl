//
// Created by Patrick Li on 30/12/22.
//

#ifndef VDL_VDLSCALAROP_H
#define VDL_VDLSCALAROP_H

#include "vdlarrdef.h"

/*-----------------------------------------------------------------------------
 |  Scalar NA check
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarIsNa(x) ((x) == VDL_CHAR_NA)
#define vdlint_IntScalarIsNa(x) ((x) == VDL_INT_NA)
#define vdlint_DoubleScalarIsNa(x) isnan(x)
#define vdlint_PointerScalarIsNa(x) ((x) == VDL_POINTER_NA)

#define vdlint_CharScalarCharIsNa(x, y) (vdlint_CharScalarIsNa(x) || vdlint_CharScalarIsNa(y))
#define vdlint_CharScalarIntIsNa(x, y) (vdlint_CharScalarIsNa(x) || vdlint_IntScalarIsNa(y))
#define vdlint_CharScalarDoubleIsNa(x, y) (vdlint_CharScalarIsNa(x) || vdlint_DoubleScalarIsNa(y))

#define vdlint_IntScalarCharIsNa(x, y) vdlint_CharScalarIntIsNa(y, x)
#define vdlint_IntScalarIntIsNa(x, y) (vdlint_IntScalarIsNa(x) || vdlint_IntScalarIsNa(y))
#define vdlint_IntScalarDoubleIsNa(x, y) (vdlint_IntScalarIsNa(x) || vdlint_DoubleScalarIsNa(y))

#define vdlint_DoubleScalarCharIsNa(x, y) vdlint_CharScalarDoubleIsNa(y, x)
#define vdlint_DoubleScalarIntIsNa(x, y) vdlint_IntScalarDoubleIsNa(y, x)
#define vdlint_DoubleScalarDoubleIsNa(x, y) (vdlint_DoubleScalarIsNa(x) || vdlint_DoubleScalarIsNa(y))

#define vdlint_PointerScalarPointerIsNa(x, y) (vdlint_PointerScalarIsNa(x) || vdlint_PointerScalarIsNa(y))

/*-----------------------------------------------------------------------------
 |  Scalar conversion
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarToInt(x) (vdlint_CharScalarIsNa(x) ? VDL_INT_NA : (int) (x))
#define vdlint_CharScalarToDouble(x) (vdlint_CharScalarIsNa(x) ? VDL_DOUBLE_NA : (double) (x))

#define vdlint_IntScalarToChar(x) (vdlint_IntScalarIsNa(x) || (x) > CHAR_MAX || (x) < CHAR_MIN ? VDL_CHAR_NA : (char) (x))
#define vdlint_IntScalarToDouble(x) (vdlint_IntScalarIsNa(x) ? VDL_DOUBLE_NA : (double) (x))

#define vdlint_DoubleScalarToChar(x) (vdlint_DoubleScalarIsNa(x) || (x) > CHAR_MAX || (x) < CHAR_MIN ? VDL_CHAR_NA : (char) (x))
#define vdlint_DoubleScalarToInt(x) (vdlint_DoubleScalarIsNa(x) || (x) > INT_MAX || (x) < INT_MIN ? VDL_INT_NA : (int) (x))

/*-----------------------------------------------------------------------------
 |  Scalar greater than
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarGtChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) > (y))
#define vdlint_CharScalarGtInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) > (y))
#define vdlint_CharScalarGtDouble(x, y) (vdlint_CharScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) > (y))

#define vdlint_IntScalarGtChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_INT_NA : (x) > (y))
#define vdlint_IntScalarGtInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : (x) > (y))
#define vdlint_IntScalarGtDouble(x, y) (vdlint_IntScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) > (y))

#define vdlint_DoubleScalarGtChar(x, y) (vdlint_DoubleScalarCharIsNa(x, y) ? VDL_INT_NA : (x) > (y))
#define vdlint_DoubleScalarGtInt(x, y) (vdlint_DoubleScalarIntIsNa(x, y) ? VDL_INT_NA : (x) > (y))
#define vdlint_DoubleScalarGtDouble(x, y) (vdlint_DoubleScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) > (y))

/*-----------------------------------------------------------------------------
 |  Scalar equal to
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarEqChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) == (y))
#define vdlint_CharScalarEqInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) == (y))
#define vdlint_CharScalarEqDouble(x, y) (vdlint_CharScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) == (y))

#define vdlint_IntScalarEqChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_INT_NA : (x) == (y))
#define vdlint_IntScalarEqInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : (x) == (y))
#define vdlint_IntScalarEqDouble(x, y) (vdlint_IntScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) == (y))

#define vdlint_DoubleScalarEqChar(x, y) (vdlint_DoubleScalarCharIsNa(x, y) ? VDL_INT_NA : (x) == (y))
#define vdlint_DoubleScalarEqInt(x, y) (vdlint_DoubleScalarIntIsNa(x, y) ? VDL_INT_NA : (x) == (y))
#define vdlint_DoubleScalarEqDouble(x, y) (vdlint_DoubleScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) == (y))

#define vdlint_PointerScalarEqPointer(x, y) (vdlint_PointerScalarPointerIsNa(x, y) ? VDL_INT_NA : (x) == (y))

/*-----------------------------------------------------------------------------
 |  Scalar not equal to
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarNeChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) != (y))
#define vdlint_CharScalarNeInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) != (y))
#define vdlint_CharScalarNeDouble(x, y) (vdlint_CharScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) != (y))

#define vdlint_IntScalarNeChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_INT_NA : (x) != (y))
#define vdlint_IntScalarNeInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : (x) != (y))
#define vdlint_IntScalarNeDouble(x, y) (vdlint_IntScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) != (y))

#define vdlint_DoubleScalarNeChar(x, y) (vdlint_DoubleScalarCharIsNa(x, y) ? VDL_INT_NA : (x) != (y))
#define vdlint_DoubleScalarNeInt(x, y) (vdlint_DoubleScalarIntIsNa(x, y) ? VDL_INT_NA : (x) != (y))
#define vdlint_DoubleScalarNeDouble(x, y) (vdlint_DoubleScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) != (y))

#define vdlint_PointerScalarNePointer(x, y) (vdlint_PointerScalarPointerIsNa(x, y) ? VDL_INT_NA : (x) != (y))

/*-----------------------------------------------------------------------------
 |  Scalar less than
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarLtChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) < (y))
#define vdlint_CharScalarLtInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) < (y))
#define vdlint_CharScalarLtDouble(x, y) (vdlint_CharScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) < (y))

#define vdlint_IntScalarLtChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_INT_NA : (x) < (y))
#define vdlint_IntScalarLtInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : (x) < (y))
#define vdlint_IntScalarLtDouble(x, y) (vdlint_IntScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) < (y))

#define vdlint_DoubleScalarLtChar(x, y) (vdlint_DoubleScalarCharIsNa(x, y) ? VDL_INT_NA : (x) < (y))
#define vdlint_DoubleScalarLtInt(x, y) (vdlint_DoubleScalarIntIsNa(x, y) ? VDL_INT_NA : (x) < (y))
#define vdlint_DoubleScalarLtDouble(x, y) (vdlint_DoubleScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) < (y))

/*-----------------------------------------------------------------------------
 |  Scalar greater or equal to
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarGeChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) >= (y))
#define vdlint_CharScalarGeInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) >= (y))
#define vdlint_CharScalarGeDouble(x, y) (vdlint_CharScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) >= (y))

#define vdlint_IntScalarGeChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_INT_NA : (x) >= (y))
#define vdlint_IntScalarGeInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : (x) >= (y))
#define vdlint_IntScalarGeDouble(x, y) (vdlint_IntScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) >= (y))

#define vdlint_DoubleScalarGeChar(x, y) (vdlint_DoubleScalarCharIsNa(x, y) ? VDL_INT_NA : (x) >= (y))
#define vdlint_DoubleScalarGeInt(x, y) (vdlint_DoubleScalarIntIsNa(x, y) ? VDL_INT_NA : (x) >= (y))
#define vdlint_DoubleScalarGeDouble(x, y) (vdlint_DoubleScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) >= (y))

/*-----------------------------------------------------------------------------
 |  Scalar less or equal to
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarLeChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) <= (y))
#define vdlint_CharScalarLeInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) <= (y))
#define vdlint_CharScalarLeDouble(x, y) (vdlint_CharScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) <= (y))

#define vdlint_IntScalarLeChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_INT_NA : (x) <= (y))
#define vdlint_IntScalarLeInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : (x) <= (y))
#define vdlint_IntScalarLeDouble(x, y) (vdlint_IntScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) <= (y))

#define vdlint_DoubleScalarLeChar(x, y) (vdlint_DoubleScalarCharIsNa(x, y) ? VDL_INT_NA : (x) <= (y))
#define vdlint_DoubleScalarLeInt(x, y) (vdlint_DoubleScalarIntIsNa(x, y) ? VDL_INT_NA : (x) <= (y))
#define vdlint_DoubleScalarLeDouble(x, y) (vdlint_DoubleScalarDoubleIsNa(x, y) ? VDL_INT_NA : (x) <= (y))

/*-----------------------------------------------------------------------------
 |  Scalar and
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarAndChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) && (y))
#define vdlint_CharScalarAndInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) && (y))
#define vdlint_CharScalarAndDouble(x, y) (vdlint_CharScalarDoubleIsNa(x, y) ? VDL_INT_NA : ((x) && (y)))

#define vdlint_IntScalarAndChar(x, y) vdlint_CharScalarAndInt(y, x)
#define vdlint_IntScalarAndInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : ((x) && (y)))
#define vdlint_IntScalarAndDouble(x, y) (vdlint_IntScalarDoubleIsNa(x, y) ? VDL_INT_NA : ((x) && (y)))

#define vdlint_DoubleScalarAndChar(x, y) vdlint_CharScalarAndDouble(y, x)
#define vdlint_DoubleScalarAndInt(x, y) vdlint_IntScalarAndDouble(y, x)
#define vdlint_DoubleScalarAndDouble(x, y) (vdlint_DoubleScalarDoubleIsNa(x, y) ? VDL_INT_NA : ((x) && (y)))

/*-----------------------------------------------------------------------------
 |  Scalar or
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarOrChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) || (y))
#define vdlint_CharScalarOrInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) || (y))
#define vdlint_CharScalarOrDouble(x, y) (vdlint_CharScalarDoubleIsNa(x, y) ? VDL_INT_NA : ((x) || (y)))

#define vdlint_IntScalarOrChar(x, y) vdlint_CharScalarOrInt(y, x)
#define vdlint_IntScalarOrInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : ((x) || (y)))
#define vdlint_IntScalarOrDouble(x, y) (vdlint_IntScalarDoubleIsNa(x, y) ? VDL_INT_NA : ((x) || (y)))

#define vdlint_DoubleScalarOrChar(x, y) vdlint_CharScalarOrDouble(y, x)
#define vdlint_DoubleScalarOrInt(x, y) vdlint_IntScalarOrDouble(y, x)
#define vdlint_DoubleScalarOrDouble(x, y) (vdlint_DoubleScalarDoubleIsNa(x, y) ? VDL_INT_NA : ((x) || (y)))

/*-----------------------------------------------------------------------------
 |  Scalar not
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarNot(x) (vdlint_CharScalarIsNa(x) ? VDL_INT_NA : !(x))
#define vdlint_IntScalarNot(x) (vdlint_IntScalarIsNa(x) ? VDL_INT_NA : !(x))
#define vdlint_DoubleScalarNot(x) (vdlint_DoubleScalarIsNa(x) ? VDL_INT_NA : !(x))

/*-----------------------------------------------------------------------------
 |  Scalar add
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarAddChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) + (y))
#define vdlint_CharScalarAddInt(x, y, result, overflow)                                    \
    do {                                                                                   \
        (overflow) = __builtin_sadd_overflow((int) (x), y, &(result));                     \
        (result)   = vdlint_CharScalarIntIsNa(x, y) || (overflow) ? VDL_INT_NA : (result); \
    } while (0)
#define vdlint_CharScalarAddDouble(x, y) (vdlint_CharScalarIsNa(x) ? VDL_DOUBLE_NA : (x) + (y))

#define vdlint_IntScalarAddChar(x, y, result, overflow) vdlint_CharScalarAddInt(y, x, result, overflow)
#define vdlint_IntScalarAddInt(x, y, result, overflow)                                    \
    do {                                                                                  \
        (overflow) = __builtin_sadd_overflow(x, y, &(result));                            \
        (result)   = vdlint_IntScalarIntIsNa(x, y) || (overflow) ? VDL_INT_NA : (result); \
    } while (0)
#define vdlint_IntScalarAddDouble(x, y) (vdlint_IntScalarIsNa(x) ? VDL_DOUBLE_NA : (x) + (y))

#define vdlint_DoubleScalarAddChar(x, y) vdlint_CharScalarAddDouble(y, x)
#define vdlint_DoubleScalarAddInt(x, y) vdlint_IntScalarAddDouble(y, x)
#define vdlint_DoubleScalarAddDouble(x, y) ((x) + (y))

/*-----------------------------------------------------------------------------
 |  Scalar subtract
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarSubChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) - (y))
#define vdlint_CharScalarSubInt(x, y, result, overflow)                                    \
    do {                                                                                   \
        (overflow) = __builtin_ssub_overflow((int) (x), y, &(result));                     \
        (result)   = vdlint_CharScalarIntIsNa(x, y) || (overflow) ? VDL_INT_NA : (result); \
    } while (0)
#define vdlint_CharScalarSubDouble(x, y) (vdlint_CharScalarIsNa(x) ? VDL_DOUBLE_NA : (x) - (y))

#define vdlint_IntScalarSubChar(x, y, result, overflow) vdlint_CharScalarSubInt(y, x, result, overflow)
#define vdlint_IntScalarSubInt(x, y, result, overflow)                                    \
    do {                                                                                  \
        (overflow) = __builtin_ssub_overflow(x, y, &(result));                            \
        (result)   = vdlint_IntScalarIntIsNa(x, y) || (overflow) ? VDL_INT_NA : (result); \
    } while (0)
#define vdlint_IntScalarSubDouble(x, y) (vdlint_IntScalarIsNa(x) ? VDL_DOUBLE_NA : (x) - (y))

#define vdlint_DoubleScalarSubChar(x, y) vdlint_CharScalarSubDouble(y, x)
#define vdlint_DoubleScalarSubInt(x, y) vdlint_IntScalarSubDouble(y, x)
#define vdlint_DoubleScalarSubDouble(x, y) ((x) - (y))

/*-----------------------------------------------------------------------------
 |  Scalar multiply
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarMulChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) * (y))
#define vdlint_CharScalarMulInt(x, y, result, overflow)                                    \
    do {                                                                                   \
        (overflow) = __builtin_smul_overflow((int) (x), y, &(result));                     \
        (result)   = vdlint_CharScalarIntIsNa(x, y) || (overflow) ? VDL_INT_NA : (result); \
    } while (0)
#define vdlint_CharScalarMulDouble(x, y) (vdlint_CharScalarIsNa(x) ? VDL_DOUBLE_NA : (x) * (y))

#define vdlint_IntScalarMulChar(x, y, result, overflow) vdlint_CharScalarMulInt(y, x, result, overflow)
#define vdlint_IntScalarMulInt(x, y, result, overflow)                                    \
    do {                                                                                  \
        (overflow) = __builtin_smul_overflow(x, y, &(result));                            \
        (result)   = vdlint_IntScalarIntIsNa(x, y) || (overflow) ? VDL_INT_NA : (result); \
    } while (0)
#define vdlint_IntScalarMulDouble(x, y) (vdlint_IntScalarIsNa(x) ? VDL_DOUBLE_NA : (x) * (y))

#define vdlint_DoubleScalarMulChar(x, y) vdlint_CharScalarMulDouble(y, x)
#define vdlint_DoubleScalarMulInt(x, y) vdlint_IntScalarMulDouble(y, x)
#define vdlint_DoubleScalarMulDouble(x, y) ((x) * (y))

/*-----------------------------------------------------------------------------
 |  Scalar division
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarDivChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_DOUBLE_NA : (double) (x) / (double) (y))
#define vdlint_CharScalarDivInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_DOUBLE_NA : (double) (x) / (double) (y))
#define vdlint_CharScalarDivDouble(x, y) (vdlint_CharScalarIsNa(x) ? VDL_DOUBLE_NA : (double) (x) / (y))

#define vdlint_IntScalarDivChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_DOUBLE_NA : (double) (x) / (double) (y))
#define vdlint_IntScalarDivInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_DOUBLE_NA : (double) (x) / (double) (y))
#define vdlint_IntScalarDivDouble(x, y) (vdlint_IntScalarIsNa(x) ? VDL_DOUBLE_NA : (double) (x) / (y))

#define vdlint_DoubleScalarDivChar(x, y) (vdlint_CharScalarIsNa(y) ? VDL_DOUBLE_NA : (x) / (y))
#define vdlint_DoubleScalarDivInt(x, y) (vdlint_IntScalarIsNa(y) ? VDL_DOUBLE_NA : (x) / (y))
#define vdlint_DoubleScalarDivDouble(x, y) ((x) / (y))

/*-----------------------------------------------------------------------------
 |  Scalar negative
 ----------------------------------------------------------------------------*/

#define vdlint_IntScalarNeg(x) (vdlint_IntScalarIsNa(x) ? VDL_INT_NA : -(x))
#define vdlint_DoubleScalarNeg(x) (-(x))

/*-----------------------------------------------------------------------------
 |  Scalar modular
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarModChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) % (y))
#define vdlint_CharScalarModInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) % (y))
#define vdlint_IntScalarModChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_INT_NA : (x) % (y))
#define vdlint_IntScalarModInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : (x) % (y))

/*-----------------------------------------------------------------------------
 |  Scalar integer division
 ----------------------------------------------------------------------------*/

#define vdlint_CharScalarIntdivChar(x, y) (vdlint_CharScalarCharIsNa(x, y) ? VDL_INT_NA : (x) / (y))
#define vdlint_CharScalarIntdivInt(x, y) (vdlint_CharScalarIntIsNa(x, y) ? VDL_INT_NA : (x) / (y))

#define vdlint_IntScalarIntdivChar(x, y) (vdlint_IntScalarCharIsNa(x, y) ? VDL_INT_NA : (x) / (y))
#define vdlint_IntScalarIntdivInt(x, y) (vdlint_IntScalarIntIsNa(x, y) ? VDL_INT_NA : (x) / (y))

#endif//VDL_VDLSCALAROP_H
