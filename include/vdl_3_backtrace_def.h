//
// Created by Patrick Li on 20/2/2023.
//

#ifndef VDL_VDL_3_BACKTRACE_DEF_H
#define VDL_VDL_3_BACKTRACE_DEF_H

/*-----------------------------------------------------------------------------
 |  Copy backtrace
 ----------------------------------------------------------------------------*/

static inline void vdl_BackupBacktrace(void)
{
    vdl_GlobalVar_BacktraceBackup.FrameCount = vdl_GlobalVar_Backtrace.FrameCount;
    vdl_for_i(vdl_GlobalVar_Backtrace.FrameCount)
    {
        vdl_GlobalVar_BacktraceBackup.FunctionName[i] = vdl_GlobalVar_Backtrace.FunctionName[i];
        vdl_GlobalVar_BacktraceBackup.FileName[i]     = vdl_GlobalVar_Backtrace.FileName[i];
        vdl_GlobalVar_BacktraceBackup.Line[i]         = vdl_GlobalVar_Backtrace.Line[i];
    }
}

/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

static inline void vdl_PrintBacktrace(const VDL_BACKTRACE_T bt)
{
    if (bt.FrameCount < 1)
        return;
    printf("Backtrace - %d stack frames:\n", bt.FrameCount);
    char spaces[VDL_BACKTRACE_MAX_FRAME_NUM]     = {'\0'};
    int func_length[VDL_BACKTRACE_MAX_FRAME_NUM] = {0};
    int max_func_length                          = -1;
    vdl_for_i(bt.FrameCount)
    {
        func_length[i] = (int) strlen(bt.FunctionName[i]);
        if (func_length[i] > max_func_length)
            max_func_length = func_length[i];
    }

    for (int i = bt.FrameCount - 1; i >= 0; i--)
    {
        if (i != 0)
            fputs("  ║═", stdout);
        else
            fputs("  ╚═", stdout);

        int space_length = max_func_length - (int) strlen(bt.FunctionName[i]);

        vdl_for_j(space_length) spaces[j] = ' ';
        spaces[space_length]              = '\0';

        printf("[%d] Calling <%.*s>%s from %s:%d\n",
               bt.FrameCount - i - 1,
               (int) strlen(bt.FunctionName[i]),
               bt.FunctionName[i],
               spaces,
               bt.FileName[i],
               bt.Line[i]);
    }
}

/*-----------------------------------------------------------------------------
 |  Redefine the special handler for unhandled exceptions
 ----------------------------------------------------------------------------*/

_Noreturn static void vdl_BacktraceNoCatchHandler(void)
{
    vdl_PrintBacktrace(vdl_GlobalVar_BacktraceBackup);
    puts(vdl_GlobalVar_ErrorMessage);
#ifdef VDL_EXCEPTION_DISABLE
    puts("Program abort in no exception mode!");
#else
    puts("Program abort!");
#endif//VDL_EXCEPTION_DISABLE
    exit(EXIT_FAILURE);
}

/*-----------------------------------------------------------------------------
 |  Push frame to backtrace
 ----------------------------------------------------------------------------*/

static inline void vdl_PushFrameToBacktrace(const VDL_FRAME_T fr)
{
    // Record the current frame (a call to the current function)
    vdl_GlobalVar_Backtrace.Line[vdl_GlobalVar_Backtrace.FrameCount]         = fr.Line;
    vdl_GlobalVar_Backtrace.FunctionName[vdl_GlobalVar_Backtrace.FrameCount] = fr.FunctionName;
    vdl_GlobalVar_Backtrace.FileName[vdl_GlobalVar_Backtrace.FrameCount]     = fr.FileName;

    vdl_GlobalVar_Backtrace.FrameCount++;

    // Check if the stack depth exceed the limit.
    vdl_Expect(vdl_GlobalVar_Backtrace.FrameCount <= VDL_BACKTRACE_MAX_FRAME_NUM,
               VDL_EXCEPTION_EXCEED_STACK_LIMIT,
               "Exceed the stack limit allowed by the backtrace [%d]!",
               VDL_BACKTRACE_MAX_FRAME_NUM);
}

#endif//VDL_VDL_3_BACKTRACE_DEF_H
