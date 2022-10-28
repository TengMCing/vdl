//
// Created by Patrick Li on 24/10/22.
//

#ifndef VDL_TEST_H
#define VDL_TEST_H

static int expect_count = 0;

#define echo(s)           \
    do {                  \
        printf("\t\t\t"s  \
               "\n");     \
        expect_count = 0; \
    } while (0)

#define test_printf_header()                            \
    do {                                                \
        printf("\t\t\t\tSubtest %d: ", ++expect_count); \
    } while (0)

#define test_printf_wrapper(expr) \
    do {                          \
        test_printf_header();     \
        expr;                     \
    } while (0)

#define test_printf(f, ...)                             \
    do {                                                \
        printf("\t\t\t\tSubtest %d: ", ++expect_count); \
        printf(f, ##__VA_ARGS__);                       \
        printf("\n");                                   \
    } while (0)


#endif//VDL_TEST_H
