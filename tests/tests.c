//
// Created by Patrick Li on 7/10/22.
//

#include <stdio.h>
#include <stdlib.h>

static void empty_string(char *s, const int length)
{
    for (int i = 0; i < length; i++)
        s[i] = 0;
}

static int test_vdlassert(void)
{
    int pass[3]             = {0};
    int num_pass            = 0;
    const int num_test      = 8;
    const char *TEST_NAME[] = {"TEST_ASSERT_NULLPOINTER",
                               "TEST_ASSERT_INDEXOUTOFBOUND_1",
                               "TEST_ASSERT_INDEXOUTOFBOUND_2",
                               "TEST_ASSERT_UNKNOWNTYPE_1",
                               "TEST_ASSERT_UNKNOWNTYPE_2",
                               "TEST_ASSERT_NOTSTACKALLOCATED",
                               "TEST_ASSERT_NOTHEAPALLOCATED",
                               "TEST_ASSERT_INCOMPATIBLETYPE"};
    char com1[1000];
    char com2[1000];

    printf("Testing TEST_VDLASSERT - %d Tests\n", num_test);
    for (int i = 0; i < num_test; i++)
    {
        empty_string(com1, 1000);
        sprintf(com1,
                "echo \"\\nTesting %s (%d/%d)\\n\\n\" >> test_log.log",
                TEST_NAME[i], i + 1, num_test);
        system(com1);
        printf("  Testing %s (%d/%d)\n", TEST_NAME[i], i + 1, num_test);

        empty_string(com1, 1000);
        sprintf(com1,
                "clang -D%s -Wno-extra-semi -Wno-gnu-zero-variadic-macro-arguments -Wno-documentation-unknown-command -Wno-documentation -Wno-declaration-after-statement -Weverything -g test_vdlassert.c -o test_vdlassert 2>> test_log.log",
                TEST_NAME[i]);
        system(com1);

        int exit_code = system("./test_vdlassert >> test_log.log");
        if (WIFEXITED(exit_code) != 0)
        {
            pass[i] = 1;
            num_pass++;
        }
    }

    printf("\n");
    system("echo \"\\n\" >> test_log.log");
    printf("Summary of TEST_VDLASSERT: (%d/%d)\n", num_pass, num_test);

    empty_string(com1, 1000);
    sprintf(com1, "echo \"Summary of TEST_VDLASSERT: (%d/%d)\" >> test_log.log", num_pass, num_test);
    system(com1);

    for (int i = 0; i < num_test; i++)
    {
        printf("  %s : ", TEST_NAME[i]);
        empty_string(com1, 1000);
        empty_string(com2, 1000);
        sprintf(com1, "echo \" %s: ", TEST_NAME[i]);

        if (pass[i] == 1)
        {
            printf("Passed!\n");
            sprintf(com2, "%sPassed!\" >> test_log.log", com1);
        }
        else
        {
            printf("Failed!\n");
            sprintf(com2, "%sFailed!\" >> test_log.log", com1);
        }

        system(com2);
    }
    return num_pass;
}

int main()
{
    system("> test_log.log");
#ifdef TEST_VDLASSERT
    test_vdlassert();
#endif//TEST_VDLASSERT
    return 0;
}
