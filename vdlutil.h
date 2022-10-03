//
// Created by Patrick Li on 25/9/22.
//

#ifndef VDL_VDLUTIL_H
#define VDL_VDLUTIL_H

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#define vdl_First1Args(arg, ...) arg
#define vdl_First2Args(arg, ...) arg, vdl_First1Args(__VA_ARGS__)
#define vdl_First3Args(arg, ...) arg, vdl_First2Args(__VA_ARGS__)
#define vdl_First4Args(arg, ...) arg, vdl_First3Args(__VA_ARGS__)
#define vdl_First5Args(arg, ...) arg, vdl_First4Args(__VA_ARGS__)
#define vdl_First6Args(arg, ...) arg, vdl_First5Args(__VA_ARGS__)
#define vdl_First7Args(arg, ...) arg, vdl_First6Args(__VA_ARGS__)
#define vdl_First8Args(arg, ...) arg, vdl_First7Args(__VA_ARGS__)
#define vdl_First9Args(arg, ...) arg, vdl_First8Args(__VA_ARGS__)
#define vdl_First10Args(arg, ...) arg, vdl_First9Args(__VA_ARGS__)
#define vdl_First11Args(arg, ...) arg, vdl_First10Args(__VA_ARGS__)
#define vdl_First12Args(arg, ...) arg, vdl_First11Args(__VA_ARGS__)
#define vdl_First13Args(arg, ...) arg, vdl_First12Args(__VA_ARGS__)
#define vdl_First14Args(arg, ...) arg, vdl_First13Args(__VA_ARGS__)
#define vdl_First15Args(arg, ...) arg, vdl_First14Args(__VA_ARGS__)
#define vdl_First16Args(arg, ...) arg, vdl_First15Args(__VA_ARGS__)
#define vdl_First17Args(arg, ...) arg, vdl_First16Args(__VA_ARGS__)
#define vdl_First18Args(arg, ...) arg, vdl_First17Args(__VA_ARGS__)
#define vdl_First19Args(arg, ...) arg, vdl_First18Args(__VA_ARGS__)
#define vdl_First20Args(arg, ...) arg, vdl_First19Args(__VA_ARGS__)
#define vdl_FirstNArgs(n, ...) vdl_First##n##Args(__VA_ARGS__)

#define vdl_CountArgs(...) vdl_CountArgs_Internal(0, ##__VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define vdl_CountArgs_Internal(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

#define vdl_Except0Args(...) __VA_ARGS__
#define vdl_Except1Args(first_arg, ...) __VA_ARGS__
#define vdl_Except2Args(first_arg, ...) vdl_Except1Args(__VA_ARGS__)
#define vdl_Except3Args(first_arg, ...) vdl_Except2Args(__VA_ARGS__)
#define vdl_Except4Args(first_arg, ...) vdl_Except3Args(__VA_ARGS__)
#define vdl_Except5Args(first_arg, ...) vdl_Except4Args(__VA_ARGS__)
#define vdl_Except6Args(first_arg, ...) vdl_Except5Args(__VA_ARGS__)
#define vdl_Except7Args(first_arg, ...) vdl_Except6Args(__VA_ARGS__)
#define vdl_Except8Args(first_arg, ...) vdl_Except7Args(__VA_ARGS__)
#define vdl_Except9Args(first_arg, ...) vdl_Except8Args(__VA_ARGS__)
#define vdl_Except10Args(first_arg, ...) vdl_Except9Args(__VA_ARGS__)
#define vdl_Except11Args(first_arg, ...) vdl_Except10Args(__VA_ARGS__)
#define vdl_Except12Args(first_arg, ...) vdl_Except11Args(__VA_ARGS__)
#define vdl_Except13Args(first_arg, ...) vdl_Except12Args(__VA_ARGS__)
#define vdl_Except14Args(first_arg, ...) vdl_Except13Args(__VA_ARGS__)
#define vdl_Except15Args(first_arg, ...) vdl_Except14Args(__VA_ARGS__)
#define vdl_Except16Args(first_arg, ...) vdl_Except15Args(__VA_ARGS__)
#define vdl_Except17Args(first_arg, ...) vdl_Except16Args(__VA_ARGS__)
#define vdl_Except18Args(first_arg, ...) vdl_Except17Args(__VA_ARGS__)
#define vdl_Except19Args(first_arg, ...) vdl_Except18Args(__VA_ARGS__)
#define vdl_Except20Args(first_arg, ...) vdl_Except19Args(__VA_ARGS__)
#define vdl_ExceptNArgs(n, ...) vdl_Except##n##Args(__VA_ARGS__)

/*-----------------------------------------------------------------------------
 |  Loop through the length
 ----------------------------------------------------------------------------*/

#define vdl_For_i(ub) for (int i = 0; i < (ub); i++)
#define vdl_For_j(ub) for (int j = 0; j < (ub); j++)
#define vdl_For_k(ub) for (int k = 0; k < (ub); k++)

/*-----------------------------------------------------------------------------
 |  Unused arguments
 ----------------------------------------------------------------------------*/

#define vdl_Unused(x) (void) (x)

/*-----------------------------------------------------------------------------
 |  Expect
 ----------------------------------------------------------------------------*/

#define vdl_Expect(expr, errmsg)                                                     \
    do {                                                                             \
        if ((expr) != 1)                                                             \
        {                                                                            \
            vdl_PrintTrace();                                                        \
            printf("Error raised by <%s> at %s:%d: ", __func__, __FILE__, __LINE__); \
            errmsg;                                                                  \
            printf("\n");                                                            \
            exit(EXIT_FAILURE);                                                      \
        }                                                                            \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Cast void pointer to different types
 ----------------------------------------------------------------------------*/

#define vdl_char_Array(voidp) ((char *) (voidp))
#define vdl_int_Array(voidp) ((int *) (voidp))
#define vdl_double_Array(voidp) ((double *) (voidp))
#define vdl_vp_Array(voidp) ((vdl_vp *) (voidp))

/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

static inline char *vdl_PrintTraceFormat(char *s)
{
    // return s;
    if (s[0] == '\0')
        return s;

    char buffer[10000] = {0};
    int i              = -1;
    int j              = -1;
    int open_token     = 0;
    int num_token      = 0;

    while (1)
    {
        i++;
        if (s[i] == '\0')
            break;

        if (s[i] == ' ' && open_token)
        {
            open_token = 0;
            if (num_token == 1)
                buffer[++j] = ']';
            if (num_token == 2)
            {
                buffer[++j] = ':';
                buffer[++j] = ':';
            }
            if (num_token == 4)
                buffer[++j] = '>';

            if (num_token != 2 && num_token != 3 && num_token != 4 && num_token != 5 && num_token != 6)
                buffer[++j] = ' ';
            continue;
        }

        if (s[i] != ' ' && !open_token)
        {
            open_token = 1;
            num_token++;
            if (num_token == 1)
                buffer[++j] = '[';
            if (num_token == 2)
                buffer[++j] = '<';
        }

        if (s[i] != ' ' && open_token)
        {
            if (num_token != 3 && num_token != 5 && num_token != 6)
                buffer[++j] = s[i];
        }
    }

    vdl_For_k(i) s[k] = '\0';

    vdl_For_k(i) if (buffer[k] != '\0') s[k] = buffer[k];
    return s;
}

// A function obtained from www.gnu.org
static inline void vdl_PrintTrace(void)
{
    void *trace[100];
    char **trace_string;
    int length, i;

    length       = backtrace(trace, 100);
    trace_string = backtrace_symbols(trace, length);
    if (trace_string != NULL)
    {
        printf("Backtrace - %d stack frames:\n", length - 1);
        for (i = length - 1; i > 0; i--)
        {
            if (i != 1)
                printf("  ║═%s\n", vdl_PrintTraceFormat(trace_string[i]));
            else
                printf("  ╚═%s\n\n", vdl_PrintTraceFormat(trace_string[i]));
        }
    }

    free(trace_string);
}

#endif//VDL_VDLUTIL_H
