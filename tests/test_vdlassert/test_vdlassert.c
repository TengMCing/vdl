//
// Created by Patrick Li on 11/11/22.
//

#include "../../vdlassert.h"
#include "../test.h"

int main(void)
{
    vdlint_CheckNullPointer(NULL);
    // exit(0)
    return 0;
VDL_EXCEPTION:
    return 0;
}
