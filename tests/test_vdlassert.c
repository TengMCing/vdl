//
// Created by Patrick Li on 7/10/22.
//

#include "../vdlassert.h"

int main()
{
#ifdef TEST_ASSERT_NULLPOINTER
    vdl_assert_NullPointer(NULL);
#endif
#ifdef TEST_ASSERT_INDEXOUTOFBOUND_1
    struct
    {
        int length;
    } v1 = {.length = 5};
    vdl_assert_IndexOutOfBound((&v1), -1);
#endif
#ifdef TEST_ASSERT_INDEXOUTOFBOUND_2
    struct
    {
        int length;
    } v2 = {.length = 5};
    vdl_assert_IndexOutOfBound((&v2), 5);
#endif
#ifdef TEST_ASSERT_UNKNOWNTYPE_1
    vdl_assert_UnknownType(-1);
#endif
#ifdef TEST_ASSERT_UNKNOWNTYPE_2
    vdl_assert_UnknownType(4);
#endif
#ifdef TEST_ASSERT_NOTSTACKALLOCATED
    vdl_assert_NotStackAllocated(1);
#endif
#ifdef TEST_ASSERT_NOTHEAPALLOCATED
    vdl_assert_NotHeapAllocated(0);
#endif
#ifdef TEST_ASSERT_INCOMPATIBLETYPE
    vdl_assert_IncompatibleType(0, 1);
#endif
    return 0;
}
