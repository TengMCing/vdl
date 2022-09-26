//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VECERROR_H
#define VDL_VECERROR_H

#include "vecutil.h"

#define assert_NullPointer(object) expect(object != NULL, printf("ERROR: NULL pointer provided!"))
#define assert_IndexOutOfBound(v, i) expect(i >= 0 || i < v->size, printf("ERROR: Index out of bound! Index [%d] not in (0, %d)!", i, v->size))
#define assert_UnknownType(type) expect(type >= VEC_CHAR && type <= VEC_VP, printf("ERROR: Unknown type [%d] provided!", type))
#define assert_ZeroCapacity(capacity) expect(capacity > 0, printf("ERROR: Zero or negative capacity [%d] provided!", capacity))
#define assert_UninitializedGC() expect(GARENA != NULL && GREACHABLE != NULL && GDREACHABLE != NULL, printf("ERROR: GC is uninitialized!"))
#define assert_InconsistentGC() expect(((GARENA == NULL) + (GREACHABLE == NULL) + (GDREACHABLE == NULL)) % 3 == 0, printf("ERROR: Garbage collector in an inconsistent state"))
#define assert_Unimplemented() expect(0, printf("ERROR: This function is unimplemented!"))

#endif//VDL_VECERROR_H
