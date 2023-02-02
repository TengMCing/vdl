//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLASSERT_H
#define VDL_VDLASSERT_H

#include "vdlbt.h"
#include "vdldef.h"

/*-----------------------------------------------------------------------------
 |  Expect
 ----------------------------------------------------------------------------*/

/// @description Except an expression to be true.
/// @param expr (VDL_BOOL). An boolean expression.
/// @param code (VDL_ERR). The error code.
/// @param msg (const char *). An additional error message.
/// @param add_statement Additional statements.
/// @Exception A jump to VDL_EXCEPTION may be performed if an Exception has not been handled.
#define vdlint_e_Expect(expr, code, msg, add_statement)                                                  \
    do {                                                                                                 \
        if (!(expr))                                                                                     \
        {                                                                                                \
            VDLINT_GERR.CODE = code;                                                                     \
            VDLINT_GERR.MSG  = msg;                                                                      \
            if (VDL_ERR_MSG_ON)                                                                          \
            {                                                                                            \
                vdlint_PrintBacktrace();                                                                 \
                printf("\n[E%03d] Error raised by <%s> at %s:%d: ", code, __func__, __FILE__, __LINE__); \
                add_statement;                                                                           \
                printf("\n");                                                                            \
            }                                                                                            \
            goto VDL_EXCEPTION;                                                                          \
        }                                                                                                \
    } while (0)


/*-----------------------------------------------------------------------------
 |  Assert
 ----------------------------------------------------------------------------*/

/// @description Assert null pointer.
/// @arg1 p (void *). A pointer.
/// @arg2 p (void *), msg (const char *). A pointer and an error message.
/// @arg3 p (void *), msg (const char *), add_statement. A pointer, an error message and additional statements.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckNullPointer(...) vdlint_GetArg4(__VA_ARGS__, vdlint_e_CheckNullPointer3, vdlint_e_CheckNullPointer2, vdlint_e_CheckNullPointer1)(__VA_ARGS__)
#define vdlint_e_CheckNullPointer1(p) vdlint_e_Expect((p) != NULL, VDL_ERR_POINTER, "Null pointer provided", printf("NULL pointer provided!"))
#define vdlint_e_CheckNullPointer2(p, msg) vdlint_e_Expect((p) != NULL, VDL_ERR_POINTER, msg, printf(msg))
#define vdlint_e_CheckNullPointer3(p, msg, add_statement) vdlint_e_Expect((p) != NULL, VDL_ERR_POINTER, msg, add_statement)

/// @description Assert index out of bound.
/// @arg2 v (vdl_vp), i (int). A vector and an index.
/// @arg3 v (vdl_vp), i (int), msg (const char *). A vector, an index and an error message.
/// @arg4 v (vdl_vp), i (int), msg (const char *), add_statement. A vector, an index, an error message and additional statements.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckIndexOutOfBound(...) vdlint_GetArg5(__VA_ARGS__, vdlint_e_CheckIndexOutOfBound4, vdlint_e_CheckIndexOutOfBound3, vdlint_e_CheckIndexOutOfBound2)(__VA_ARGS__)
#define vdlint_e_CheckIndexOutOfBound2(v, i) vdlint_e_Expect((i) >= 0 && (i) < (v)->len, VDL_ERR_INDEX, "Index out of bound", printf("Index out of bound! Index [%d] not in [0, %d)!", i, (v)->len))
#define vdlint_e_CheckIndexOutOfBound3(v, i, msg) vdlint_e_Expect((i) >= 0 && (i) < (v)->len, VDL_ERR_INDEX, msg, printf(msg))
#define vdlint_e_CheckIndexOutOfBound4(v, i, msg, add_statement) vdlint_e_Expect((i) >= 0 && (i) < (v)->len, VDL_ERR_INDEX, msg, add_statement)


/// @description Assert incompatible length for assignment.
/// @details Vector assignment requires both vectors have the same length, or the right-hand side vector is of length one.
/// @arg2 len1 (int), len2 (int). Two lengths.
/// @arg3 len1 (int), len2 (int), msg (const char *). Two lengths and an error message.
/// @arg4 len1 (int), len2 (int), msg (const char *), add_statement. Two lengths, an error message and additional statements.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckIncompatibleLen(...) vdlint_GetArg5(__VA_ARGS__, vdlint_e_CheckIncompatibleLen4, vdlint_e_CheckIncompatibleLen3, vdlint_e_CheckIncompatibleLen2)(__VA_ARGS__)
#define vdlint_e_CheckIncompatibleLen2(len1, len2) vdlint_e_Expect(((len1) > 0 && (len2) > 0) && ((len1) == (len2) || (len2) == 1), VDL_ERR_INDEX, "Incompatible length for assignment", printf("Incompatible length (%d != %d) provided!", (len1), (len2)))
#define vdlint_e_CheckIncompatibleLen3(len1, len2, msg) vdlint_e_Expect(((len1) > 0 && (len2) > 0) && ((len1) == (len2) || (len2) == 1), VDL_ERR_INDEX, msg, printf(msg))
#define vdlint_e_CheckIncompatibleLen4(len1, len2, msg, add_statement) vdlint_e_Expect(((len1) > 0 && (len2) > 0) && ((len1) == (len2) || (len2) == 1), VDL_ERR_INDEX, msg, add_statement)


/// @description Assert unknown type.
/// @arg1 type (VDL_TYPE). A vector type.
/// @arg2 type (VDL_TYPE), msg (const char *). A vector type and an error message.
/// @arg3 type (VDL_TYPE), msg (const char *), add_statement. A vector type, an error message and additional statements.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckUnknownType(...) vdlint_GetArg4(__VA_ARGS__, vdlint_e_CheckUnknownType3, vdlint_e_CheckUnknownType2, vdlint_e_CheckUnknownType1)(__VA_ARGS__)
#define vdlint_e_CheckUnknownType1(type) vdlint_e_Expect((type) >= VDL_TYPE_CHAR && (type) <= VDL_TYPE_VP, VDL_ERR_TYPE, "Unknown vector type", printf("Unknown vector type [%d] provided!", (type)))
#define vdlint_e_CheckUnknownType2(type, msg) vdlint_e_Expect((type) >= VDL_TYPE_CHAR && (type) <= VDL_TYPE_VP, VDL_ERR_TYPE, msg, printf(msg))
#define vdlint_e_CheckUnknownType3(type, msg, add_statement) vdlint_e_Expect((type) >= VDL_TYPE_CHAR && (type) <= VDL_TYPE_VP, VDL_ERR_TYPE, msg, add_statement)


/// @description Assert incompatible mode.
/// @arg2 mode1 (VDL_MODE), mode2 (VDL_MODE). Two vector modes.
/// @arg3 mode1 (VDL_MODE), mode2 (VDL_MODE), msg (const char *). Two vector modes and an error message.
/// @arg4 mode1 (VDL_MODE), mode2 (VDL_MODE), msg (const char *), add_statement. Two vector modes, an error message and additional statements.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckIncompatibleMode(...) vdlint_GetArg5(__VA_ARGS__, vdlint_e_CheckIncompatibleMode4, vdlint_e_CheckIncompatibleMode3, vdlint_e_CheckIncompatibleMode2)
#define vdlint_e_CheckIncompatibleMode2(mode1, mode2) vdlint_e_Expect((mode1) == (mode2), VDL_ERR_MODE, "Incompatible vector modes", printf("Incompatible vector modes (%d != %d) provided!", (mode1), (mode2)))
#define vdlint_e_CheckIncompatibleMode3(mode1, mode2, msg) vdlint_e_Expect((mode1) == (mode2), VDL_ERR_MODE, msg, printf(msg))
#define vdlint_e_CheckIncompatibleMode4(mode1, mode2, msg, add_statement) vdlint_e_Expect((mode1) == (mode2), VDL_ERR_MODE, msg, add_statement)


/// @description Assert incompatible type.
/// @arg2 type1 (VDL_TYPE), type2 (VDL_TYPE). Two vector types.
/// @arg3 type1 (VDL_TYPE), type2 (VDL_TYPE), msg (const char *). Two vector types and an error message.
/// @arg4 type1 (VDL_TYPE), type2 (VDL_TYPE), msg (const char *), add_statement. Two vector types, an error message and additional statements.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckIncompatibleType(...) vdlint_GetArg5(__VA_ARGS__, vdlint_e_CheckIncompatibleType4, vdlint_e_CheckIncompatibleType3, vdlint_e_CheckIncompatibleType2)(__VA_ARGS__)
#define vdlint_e_CheckIncompatibleType2(type1, type2) vdlint_e_Expect((type1) == (type2), VDL_ERR_TYPE, "Incompatible vector types", printf("Incompatible types (%d != %d) provided!", (type1), (type2)))
#define vdlint_e_CheckIncompatibleType3(type1, type2, msg) vdlint_e_Expect((type1) == (type2), VDL_ERR_TYPE, msg, printf("Incompatible types (%d != %d) provided!", (type1), (type2)))
#define vdlint_e_CheckIncompatibleType4(type1, type2, add_statement) vdlint_e_Expect((type1) == (type2), VDL_ERR_TYPE, msg, add_statement)

/// @description Assert when try to allocate zero size memory.
/// @arg1 size (size_t). Size of the memory.
/// @arg2 size (size_t), msg (const char *). Size of the memory and a error message.
/// @arg3 size (size_t), msg (const char *), add_statement. Size of the memory, an error message and additional statements.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckZeroAlloc(...) vdlint_GetArg4(__VA_ARGS__, vdlint_e_CheckZeroAlloc3, vdlint_e_CheckZeroAlloc2, vdlint_e_CheckZeroAlloc1)(__VA_ARGS__)
#define vdlint_e_CheckZeroAlloc1(size) vdlint_e_Expect((size) > 0, VDL_ERR_MEM, "Fail to allocate zero size memory", printf("Fail to allocate zero size memory!"))
#define vdlint_e_CheckZeroAlloc2(size, msg) vdlint_e_Expect((size) > 0, VDL_ERR_MEM, msg, printf("Fail to allocate zero size memory!"))
#define vdlint_e_CheckZeroAlloc3(size, msg, add_statement) vdlint_e_Expect((size) > 0, VDL_ERR_MEM, msg, add_statement)

/// @description Assert when fail to allocate memory.
/// @details The pointer should be returned by malloc, calloc, realloc or aligned_alloc.
/// @arg1 p (void *). A pointer.
/// @arg2 p (void *), msg (const char *). A pointer and an error message.
/// @arg3 p (void *), msg (const char *), add_statement. A pointer, an error message and additional statements.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckAllocFailed(...) vdlint_GetArg4(__VA_ARGS__, vdlint_e_CheckAllocFailed3, vdlint_e_CheckAllocFailed2, vdlint_e_CheckAllocFailed1)(__VA_ARGS__)
#define vdlint_e_CheckAllocFailed1(p) vdlint_e_Expect((p) != NULL, VDL_ERR_MEM, "Fail to allocate memory by the C standard library", printf("Fail to allocate memory by the C standard library!"))
#define vdlint_e_CheckAllocFailed2(p, msg) vdlint_e_Expect((p) != NULL, VDL_ERR_MEM, msg, printf("Fail to allocate memory by the C standard library!"))
#define vdlint_e_CheckAllocFailed3(p, msg, add_statement) vdlint_e_Expect((p) != NULL, VDL_ERR_MEM, msg, add_statement)

#define vdlint_e_CheckUninitializedGC() vdlint_e_Expect(VDLINT_GARENA != NULL && VDLINT_GREACHABLE != NULL && VDLINT_GDREACHABLE != NULL, VDL_ERR_GC, "Garbage collector is uninitialized", printf("Garbage collector is uninitialized!"))
#define vdlint_e_CheckInconsistentGC() vdlint_e_Expect(((VDLINT_GARENA == NULL) + (VDLINT_GREACHABLE == NULL) + (VDLINT_GDREACHABLE == NULL)) % 3 == 0, VDL_ERR_GC, "Garbage collector is in an inconsistent state", printf("Garbage collector is in an inconsistent state!"))
#define vdlint_e_CheckUnimplemented() vdlint_e_Expect(0, VDL_ERR_UNIMPLEMENTED, "This function is unimplemented", printf("This function is unimplemented!"))

/*-----------------------------------------------------------------------------
 |  Vector health check
 ----------------------------------------------------------------------------*/

#define VDLINT_CHECK_NULL_VEC 0x1
#define VDLINT_CHECK_UNKNOWN_VEC_TYPE 0x2
#define VDLINT_CHECK_NULL_VEC_DAT 0x4

/// @description Health check ensures the vector is not a null pointer (safe to read the metadata of the vector),
/// the type is known (safe to do type dispatch) and the data pointer is not a null pointer (safe to use the data).
/// @details Three macros are available to be combined by the or operator.
/// These are VDLINT_CHECK_NULL_VEC, VDLINT_CHECK_UNKNOWN_VEC_TYPE and VDLINT_CHECK_NULL_VEC_DAT.
/// @Exception A jump to VDL_EXCEPTION will be performed if an Exception is raised.
#define vdlint_e_CheckVec(v, check)                  \
    do {                                             \
        if (VDLINT_CHECK_NULL_VEC & (check))         \
            vdlint_e_CheckNullPointer(v);            \
        if (VDLINT_CHECK_UNKNOWN_VEC_TYPE & (check)) \
            vdlint_e_CheckUnknownType((v)->type);    \
        if (VDLINT_CHECK_NULL_VEC_DAT & (check))     \
            vdlint_e_CheckNullPointer((v)->dat);     \
    } while (0)


#endif//VDL_VDLASSERT_H
