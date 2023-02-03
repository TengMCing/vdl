// #define VDL_EXCEPTION_DISABLE
// #define VDL_BACKTRACE_DISABLE

#include "include/vdl.h"

#define foo(...) vdl_CallFunction(foo_BT, int, __VA_ARGS__)

static int foo_BT(int x)
{
    // foo(2);
    // puts("asb");
    // vdl_CheckType(1, 2);
    vdl_CheckNumberOfItems(-2);
    void *o_this = NULL;
    vdl_CheckNullPointer(o_this);
    vdl_Throw(2, "raise 2");
    vdl_Throw(2, "raise 2");
    vdl_Throw(2, "raise 2");
    vdl_Throw(2, "raise 2");
    return x;
}

int main(void)
{
    // vdl_Throw(2, "raise 2");
    foo(1);
    vdl_Try
    {
        foo(1);
    }
    vdl_Catch
    {
        vdl_Throw(vdl_GetExceptionID(), "re raise");
    }
    return 0;
}
