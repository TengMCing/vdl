//
// Created by Patrick Li on 1/2/23.
//

#ifndef VDL_VDL_H
#define VDL_VDL_H

/*-----------------------------------------------------------------------------
 |  Configuration
 ----------------------------------------------------------------------------*/

/// @description Users can disable exception to improve performance. But the program will abort when an error occurs.
// #define VDL_EXCEPTION_DISABLE

/// @description Users can disable backtrace to improve performance. But the error message will not contain a backtrace for debugging.
// #define VDL_BACKTRACE_DISABLE

/*-----------------------------------------------------------------------------
 |  Standard libraries
 ----------------------------------------------------------------------------*/

#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-----------------------------------------------------------------------------
 |  Utilities
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Count the number of arguments
 ----------------------------------------------------------------------------*/

/// @description Count the number of arguments.
/// @param ... (arguments).
/// @return (int) An integer constant.
#define vdl_CountArg(...) vdl_CountArgList(0, ##__VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define vdl_CountArgList(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count

/*-----------------------------------------------------------------------------
 |  Get the nth argument
 ----------------------------------------------------------------------------*/

/// @description Get the first argument.
#define vdl_GetArg1(arg1, ...) arg1

/// @description Get the second argument.
#define vdl_GetArg2(arg1, arg2, ...) arg2

/// @description Get the third argument.
#define vdl_GetArg3(arg1, arg2, arg3, ...) arg3

/// @description Get the fourth argument.
#define vdl_GetArg4(arg1, arg2, arg3, arg4, ...) arg4

/// @description Get the fifth argument.
#define vdl_GetArg5(arg1, arg2, arg3, arg4, arg5, ...) arg5

/*-----------------------------------------------------------------------------
 |  For loop
 ----------------------------------------------------------------------------*/

/// @description A variadic macro similar to `range()` in python. The counter i is an int.
/// @arg1 start (int). The start.
/// @arg2 start (int), end (int). The start and the end.
/// @arg3 start (int), end (int), step (int). The start, the end and the step size.
#define vdl_for_i(...) vdl_GetArg4(__VA_ARGS__, vdl_for_i_3, vdl_for_i_2, vdl_for_i_1)(__VA_ARGS__)
#define vdl_for_i_1(end) for (int i = 0; i < (end); i++)
#define vdl_for_i_2(start, end) for (int i = start; i < (end); i++)
#define vdl_for_i_3(start, end, step) for (int i = start; i < (end); i += (step))

/// @description A variadic macro similar to `range()` in python. The counter j is an int.
/// @arg1 start (int). The start.
/// @arg2 start (int), end (int). The start and the end.
/// @arg3 start (int), end (int), step (int). The start, the end and the step size.
#define vdl_for_j(...) vdl_GetArg4(__VA_ARGS__, vdl_for_j_3, vdl_for_j_2, vdl_for_j_1)(__VA_ARGS__)
#define vdl_for_j_1(end) for (int j = 0; j < (end); j++)
#define vdl_for_j_2(start, end) for (int j = start; j < (end); j++)
#define vdl_for_j_3(start, end, step) for (int j = start; j < (end); j += (step))

/*-----------------------------------------------------------------------------
 |  Paste argument
 ----------------------------------------------------------------------------*/

/// @description Paste two tokens together.
/// @param a (token).
/// @param b (token).
#define vdl_PasteArg(a, b) a##b

/*-----------------------------------------------------------------------------
 |  Compile time assert
 ----------------------------------------------------------------------------*/

/// @description Compile time condition check.
/// @param condition (int) Boolean expression.
/// @param file (string literal) The file name.
#define vdl_CompileTimeAssert(condition, file) vdl_CompileTimeAssertLine(condition, __LINE__, file)
#define vdl_CompileTimeAssertLine(condition, line, file) typedef char vdl_PasteArg(assertion_failed_##file##_, line)[2 * !!(condition) -1];


/*-----------------------------------------------------------------------------
 |  Exception
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Exception type and none value
 ----------------------------------------------------------------------------*/

/// @description The actual exception type.
#define VDL_EXCEPTION_T int

/// @description The reserved value representing no exception.
#define VDL_EXCEPTION_NONE (1234)

/*-----------------------------------------------------------------------------
 |  Define exceptions IDs
 ----------------------------------------------------------------------------*/

/// @description Stack overflow.
#define VDL_EXCEPTION_EXCEED_STACK_LIMIT 0x1
#define VDL_EXCEPTION_INDEX_OUT_OF_BOUND 0x2
#define VDL_EXCEPTION_NULL_POINTER 0x3
#define VDL_EXCEPTION_UNEXPECTED_TYPE 0x4
#define VDL_EXCEPTION_NON_POSITIVE_NUMBER_OF_ITEMS 0x5
#define VDL_EXCEPTION_FAILED_ALLOCATION 0x6

/*-----------------------------------------------------------------------------
 |  Error message
 ----------------------------------------------------------------------------*/

/// @description Maximum length of error message.
#define VDL_ERROR_MESSAGE_MAX_LEN 256

/// @desciption A global variable for storing error message.
static char vdl_GlobalVar_ErrorMessage[VDL_ERROR_MESSAGE_MAX_LEN] = {0};

/// @description Save error message to the global buffer.
/// @details The buffer will be reset if an encoding error occurred.
/// @param exception_id (const VDL_EXCEPTION_T). The exception ID.
/// @param function_name (const char *). Function name.
/// @param file_name (const char *). File name.
/// @param line (const int). The line number.
/// @param format (const char *const). Format for the additional error message.
/// @param ... Additional arguments passed to snprintf.
static inline void vdl_SaveErrorMessage(const VDL_EXCEPTION_T exception_id,
                                        const char *function_name,
                                        const char *file_name,
                                        const int line,
                                        const char *const format,
                                        ...)
{
    // If the format of the error message is empty, do not rewrite the buffer
    if (format[0] == '\0')
        return;

    // Create the customized error message.
    char message[VDL_ERROR_MESSAGE_MAX_LEN] = {0};
    va_list ap;
    va_start(ap, format);
    int message_len = vsnprintf(message, VDL_ERROR_MESSAGE_MAX_LEN, format, ap);
    va_end(ap);

    // Check encoding error
    if (message_len < 0)
    {
        puts("Encounter an encoding error! Buffer reset!");
        message[0] = '\0';
    }


    // Rewrite the buffer
    int len = snprintf(vdl_GlobalVar_ErrorMessage,
                       VDL_ERROR_MESSAGE_MAX_LEN,
                       "[E%03d] Error raised by <%s> at %s:%d: %s\n",
                       exception_id,
                       function_name,
                       file_name,
                       line,
                       message);

    // Check encoding error
    if (len < 0)
    {
        puts("Encounter an encoding error! Buffer reset!");
        vdl_GlobalVar_ErrorMessage[0] = '\0';
    }
}

/*-----------------------------------------------------------------------------
 |  Exception
 ----------------------------------------------------------------------------*/

/// @description A volatile global variable for storing exception frames.
/// @param Frame (jmp_buf *). The current frame.
/// @param Exception (VDL_EXCEPTION_T volatile). The exception ID.
static volatile struct
{
    jmp_buf *Frame;
    VDL_EXCEPTION_T volatile Exception;
} vdl_GlobalVar_ExceptionFrames = {0};

/// @description Get the current exception ID.
#define vdl_GetExceptionID() ((VDL_EXCEPTION_T){vdl_GlobalVar_ExceptionFrames.Exception})

/*-----------------------------------------------------------------------------
 |  No catch handler
 ----------------------------------------------------------------------------*/

/// @description The default handler for no catch exception.
/// @details The default handler prints the error message and abort the program.
_Noreturn static void vdl_DefaultNoCatchHandler(void)
{
    puts(vdl_GlobalVar_ErrorMessage);
#ifdef VDL_EXCEPTION_DISABLE
    puts("Program abort in no exception mode!");
#else
    puts("Program abort!");
#endif//VDL_EXCEPTION_DISABLE
    exit(EXIT_FAILURE);
}


/// @description A special handler for no catch exception.
#define vdl_NoCatchHandler() vdl_DefaultNoCatchHandler()

/*-----------------------------------------------------------------------------
 |  Exception long jump
 ----------------------------------------------------------------------------*/

/// @description Attempt to perform a long jump because of an exception.
/// @details This function will not perform a long jump if the catch statement
/// is missing
/// @param exception_id (const VDL_EXCEPTION_T). Exception ID.
static inline void vdl_ExceptionAttemptLongJmp(const VDL_EXCEPTION_T exception_id)
{
    vdl_GlobalVar_ExceptionFrames.Exception = exception_id;
    if (vdl_GlobalVar_ExceptionFrames.Frame)
        longjmp(*vdl_GlobalVar_ExceptionFrames.Frame, 1);
}


/*-----------------------------------------------------------------------------
 |  Throw
 ----------------------------------------------------------------------------*/

// If the exception is disabled, no jump will be performed.
#ifdef VDL_EXCEPTION_DISABLE
    /// @description Save an error message and throw an exception.
    /// @details Since the exception is disabled, the program will be aborted.
    /// @param exception_id (VDL_EXCEPTION_T). Exception ID.
    /// @param message (const char *). An error message.
    #define vdl_Throw(exception_id, format, ...)                                                     \
        do {                                                                                         \
            vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
            vdl_NoCatchHandler();                                                                    \
        } while (0)
#else
    /// @description Save an error message and throw an exception.
    /// @param exception_id (const VDL_EXCEPTION_T). Exception ID.
    /// @param format (const char *const). The format of the error message.
    /// @param ... Additional arguments used for snprintf.
    #define vdl_Throw(exception_id, format, ...)                                                     \
        do {                                                                                         \
            vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
            vdl_ExceptionAttemptLongJmp(exception_id);                                               \
            vdl_NoCatchHandler();                                                                    \
        } while (0)
#endif//VDL_EXCEPTION_DISABLE

/*-----------------------------------------------------------------------------
 |  Hooks in try and catch
 ----------------------------------------------------------------------------*/

/// @description Inject code before setjmp.
#define VDL_EXCEPTION_HOOK_START_TRY

/// @description Inject code if no exception thrown.
#define VDL_EXCEPTION_HOOK_HAPPY_TRY

/// @description Inject code after try in regrades of exception.
#define VDL_EXCEPTION_HOOK_AFTER_TRY

/// @description Inject code before catch (an exception thrown).
#define VDL_EXCEPTION_HOOK_START_CATCH

/*-----------------------------------------------------------------------------
 |  Try
 ----------------------------------------------------------------------------*/

// If the exception is disabled, no setup needs to be done.
#ifdef VDL_EXCEPTION_DISABLE
    /// @dscription Try one or more statements.
    /// @details Since the exception is disabled, the program will be aborted
    /// if an exception occurred.
    #define vdl_Try \
        {           \
            if (1)
#else
    /// @dscription Try one or more statements.
    /// @details vdl_Try needs to be paired with vdl_Catch.
    /// @example
    /// // Throw an exception with ID 1. \n
    /// void foo(void) \n
    /// { \n
    /// vdl_Throw(1); \n
    /// } \n
    /// \n
    /// // Try and print the exception ID. \n
    /// vdl_Try { \n
    /// foo(); \n
    /// printf("This line will not be executed!"); \n
    /// } vdl_Catch { \n
    /// printf("%d", vdl_GetExceptionID()); \n
    /// }
    #define vdl_Try                                                                        \
        {                                                                                  \
            jmp_buf *PrevFrame, NewFrame;                                                  \
            PrevFrame                               = vdl_GlobalVar_ExceptionFrames.Frame; \
            vdl_GlobalVar_ExceptionFrames.Frame     = (jmp_buf *) (&NewFrame);             \
            vdl_GlobalVar_ExceptionFrames.Exception = VDL_EXCEPTION_NONE;                  \
            VDL_EXCEPTION_HOOK_START_TRY;                                                  \
            if (setjmp(NewFrame) == 0)                                                     \
            {                                                                              \
                if (1)
#endif//VDL_EXCEPTION_DISABLE


/*-----------------------------------------------------------------------------
 |  Catch
 ----------------------------------------------------------------------------*/

// If the exception is disabled, the catch scope should not be run.
#ifdef VDL_EXCEPTION_DISABLE
    /// @description Catch an exception.
    /// @details Since the exception is disabled, the program will be aborted
    /// if an exception occurred. This block of code will never be run.
    #define vdl_Catch \
        else {}       \
        }             \
        if (0)
#else
    /// @dscription Catch an exception.
    /// @details vdl_Try needs to be paired with vdl_Catch.
    /// @example
    /// // Throw an exception with ID 1. \n
    /// void foo(void) \n
    /// { \n
    /// vdl_Throw(1); \n
    /// } \n
    /// \n
    /// // Try and print the exception ID. \n
    /// vdl_Try { \n
    /// foo(); \n
    /// printf("This line will not be executed!"); \n
    /// } vdl_Catch { \n
    /// printf("%d", vdl_GetExceptionID()); \n
    /// }
    #define vdl_Catch                                                 \
        else {}                                                       \
        vdl_GlobalVar_ExceptionFrames.Exception = VDL_EXCEPTION_NONE; \
        VDL_EXCEPTION_HOOK_HAPPY_TRY;                                 \
        }                                                             \
        else                                                          \
        {                                                             \
            VDL_EXCEPTION_HOOK_START_CATCH;                           \
        }                                                             \
        vdl_GlobalVar_ExceptionFrames.Frame = PrevFrame;              \
        VDL_EXCEPTION_HOOK_AFTER_TRY;                                 \
        }                                                             \
        if (vdl_GlobalVar_ExceptionFrames.Exception != VDL_EXCEPTION_NONE)
#endif//VDL_EXCEPTION_DISABLE

/*-----------------------------------------------------------------------------
 |  Exit Try
 ----------------------------------------------------------------------------*/

/// @description Exit the current vdl_Try block and skip one vdl_Catch.
#define vdl_ExitTry() vdl_Throw(VDL_EXCEPTION_NONE, "")

/*-----------------------------------------------------------------------------
 |  Expect an expression to be true
 ----------------------------------------------------------------------------*/

#define vdl_Expect(condition, exception_id, format, ...)    \
    do {                                                    \
        if (!(condition))                                   \
        {                                                   \
            vdl_Throw(exception_id, format, ##__VA_ARGS__); \
        }                                                   \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Backtrace
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Backtrace struct and Frame struct
 ----------------------------------------------------------------------------*/

/// @description Maximum frame numbers supported by the backtrace.
#define VDL_BACKTRACE_MAX_FRAME_NUM 256

/// @description Extra frames at the end of the backtrace.
#define VDL_BACKTRACE_EXTRA_FRAME_NUM 8

/// @description Maximum length of a function name.
#define VDL_BACKTRACE_MAX_FUNCTION_NAME 1000

/// @description Backtrace structure.
/// @param FrameCount (int). The current frame number.
/// @param Line (int [VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM]). Line numbers of all the calls.
/// @param FileName (const char * [VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM]). File names of all the calls.
/// @param FunctionName (const char * [VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM]). Function names of all the calls.
typedef struct VDL_BACKTRACE_T
{
    int FrameCount;
    int Line[VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM];
    const char *FileName[VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM];
    const char *FunctionName[VDL_BACKTRACE_MAX_FRAME_NUM + VDL_BACKTRACE_EXTRA_FRAME_NUM];
} VDL_BACKTRACE_T;

/// @description A global variable for storing the backtrace information.
static VDL_BACKTRACE_T vdl_GlobalVar_Backtrace = {0};

/// @description A global variable for storing the backtrace backup.
/// This is useful when an exception occurred.
static VDL_BACKTRACE_T vdl_GlobalVar_BacktraceBackup = {0};

/// @description A global variable for storing the backtrace count
/// before the try statement.
static int vdl_GlobalVar_FrameCountBeforeTry = 0;

/// @description Backtrace frame structure.
/// @param FileName (const char *const). File name of the caller.
/// @param FunctionName (const char *const). Function name of the call.
/// @param Line (int). Line number of the call.
typedef struct VDL_FRAME_T
{
    const char *const FileName;
    const char *const FunctionName;
    int Line;
} VDL_FRAME_T;

/*-----------------------------------------------------------------------------
 |  Copy backtrace
 ----------------------------------------------------------------------------*/

/// @description Backup the content of the backtrace.
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

/// @description Print a backtrace.
/// @param bt (const VDL_BACKTRACE_T). A backtrace.
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
            printf("  ║═");
        else
            printf("  ╚═");

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

/// @description The customized handler for no catch exception.
/// It can print backtrace.
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

// If the backtrace is enabled, redefine the no catch handler
#ifndef VDL_BACKTRACE_DISABLE
    #ifdef vdl_NoCatchHandler
        #undef vdl_NoCatchHandler
    #endif//vdl_NoCatchHandler
    /// @description A special handler for no catch exception.
    #define vdl_NoCatchHandler() vdl_BacktraceNoCatchHandler()
#endif//VDL_BACKTRACE_DISABLE


/*-----------------------------------------------------------------------------
 |  Redefine the exception hooks such that the backtrace can work correctly
 ----------------------------------------------------------------------------*/

// If the backtrace is enabled, properly define the try catch statement.
#ifndef VDL_BACKTRACE_DISABLE
    // The backtrace frame count needs to be saved before a try and restore after a try
    // such that a long jump will not cause the frame count to be incorrect.
    #ifdef VDL_EXCEPTION_HOOK_START_TRY
        #undef VDL_EXCEPTION_HOOK_START_TRY
    #endif//VDL_EXCEPTION_HOOK_START_TRY
    #define VDL_EXCEPTION_HOOK_START_TRY                                            \
        do {                                                                        \
            vdl_GlobalVar_FrameCountBeforeTry = vdl_GlobalVar_Backtrace.FrameCount; \
        } while (0)

    #ifdef VDL_EXCEPTION_HOOK_AFTER_TRY
        #undef VDL_EXCEPTION_HOOK_AFTER_TRY
    #endif//VDL_EXCEPTION_HOOK_AFTER_TRY
    #define VDL_EXCEPTION_HOOK_AFTER_TRY                                            \
        do {                                                                        \
            vdl_GlobalVar_Backtrace.FrameCount = vdl_GlobalVar_FrameCountBeforeTry; \
        } while (0)
#endif//VDL_BACKTRACE_DISABLE


/*-----------------------------------------------------------------------------
 |  Redefine vdl_Throw to save backtrace
 ----------------------------------------------------------------------------*/

// TODO: put copy bt into a function
// TODO: put vdl_Throw content into functions

// If the backtrace is enabled, redefine the throw statement.
#ifndef VDL_BACKTRACE_DISABLE
    #ifdef vdl_Throw
        #undef vdl_Throw
    #endif//vdl_Throw
    #ifdef VDL_EXCEPTION_DISABLE
        /// @description Save the backtrace and an error message and throw an exception.
        /// @details Since the exception is disabled, the program will be aborted if an exception
        /// occurred.
        /// @param exception_id (const VDL_EXCEPTION_T). Exception ID.
        /// @param format (const char *const). The format of the error message.
        /// @param ... Additional arguments used for snprintf.
        #define vdl_Throw(exception_id, format, ...)                                                     \
            do {                                                                                         \
                vdl_BackupBacktrace();                                                                   \
                vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
                vdl_NoCatchHandler();                                                                    \
            } while (0)
    #else
        /// @description Save the backtrace and an error message and throw an exception.
        /// @param exception_id (const VDL_EXCEPTION_T). Exception ID.
        /// @param format (const char *const). The format of the error message.
        /// @param ... Additional arguments used for snprintf.
        #define vdl_Throw(exception_id, format, ...)                                                     \
            do {                                                                                         \
                vdl_BackupBacktrace();                                                                   \
                vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
                vdl_ExceptionAttemptLongJmp(exception_id);                                               \
                vdl_NoCatchHandler();                                                                    \
            } while (0)
    #endif//VDL_EXCEPTION_DISABLE
#endif    //VDL_BACKTRACE_DISABLE


/*-----------------------------------------------------------------------------
 |  Make a frame for the caller
 ----------------------------------------------------------------------------*/

/// @description Make a backtrace frame struct to capture the current file name,
/// line number and the name of the function to be called.
/// @param function_name (const char * const). A string literal.
#define vdl_MakeFrame(function_name)                                            \
    (VDL_FRAME_T)                                                               \
    {                                                                           \
        .Line = __LINE__, .FileName = __FILE__, .FunctionName = (function_name) \
    }

/*-----------------------------------------------------------------------------
 |  Push frame to backtrace
 ----------------------------------------------------------------------------*/

/// @description Push the frame to the top of the backtrace stack.
/// @param fr (const VDL_FRAME_T). The frame.
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

/*-----------------------------------------------------------------------------
 |  Pop frame from the backtrace
 ----------------------------------------------------------------------------*/

/// @description Pop a frame from the backtrace stack.
#define vdl_PopFrameFromBacktrace()           \
    do {                                      \
        vdl_GlobalVar_Backtrace.FrameCount--; \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Caller to automatically make and pop backtrace
 ----------------------------------------------------------------------------*/

#ifdef VDL_BACKTRACE_DISABLE
    /// @description Call a function while maintaining the backtrace.
    /// @details Since the backtrace is disabled,
    /// this macro only calls the function.
    /// @param function (identifier). The function identifier.
    /// @param return_type (type). The return type.
    /// @param ... Additional arguments passed to the function call.
    #define vdl_CallFunction(function, return_type, ...) function(__VA_ARGS__)

    /// @description Call a void function while maintaining the backtrace.
    /// @details Since the backtrace is disabled,
    /// this macro only calls the function.
    /// @param function (identifier). The function identifier.
    /// @param ... Additional arguments passed to the function call.
    #define vdl_CallVoidFunction(function, ...) function(__VA_ARGS__)
#else
    /// @description Call a function while maintaining the backtrace.
    /// @details Users should create a `rt foo_BT()` function and define a macro
    /// `#define foo(...) vdl_CallFunction(foo_BT, rt, ##__VA_ARGS__)` as a wrapper.
    /// @param function (identifier). The function identifier.
    /// @param return_type (type). The return type.
    /// @param ... Additional arguments passed to the function call.
    #define vdl_CallFunction(function, return_type, ...)        \
        ({                                                      \
            vdl_PushFrameToBacktrace(vdl_MakeFrame(#function)); \
            return_type _return_value = function(__VA_ARGS__);  \
            vdl_PopFrameFromBacktrace();                        \
            _return_value;                                      \
        })

    /// @description Call a void function while maintaining the backtrace.
    /// @details Users should create a `void foo_BT()` function and define a macro
    /// `#define foo(...) vdl_CallVoidFunction(foo_BT, ##__VA_ARGS__)` as a wrapper.
    /// @param function (identifier). The function identifier.
    /// @param ... Additional arguments passed to the function call.
    #define vdl_CallVoidFunction(function, ...)                 \
        do {                                                    \
            vdl_PushFrameToBacktrace(vdl_MakeFrame(#function)); \
            function(__VA_ARGS__);                              \
            vdl_PopFrameFromBacktrace();                        \
        } while (0)
#endif//VDL_BACKTRACE_DISABLE

/*-----------------------------------------------------------------------------
 |  Vector
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Vector types
 ----------------------------------------------------------------------------*/

/// @description Primitive vector types.
/// @details Only types defined here can be legally stored in a vector. \n\n
/// VDL_TYPE_CHAR: 0, character type (could be signed or unsigned, this is implementation defined). \n\n
/// VDL_TYPE_INT: 1, signed integer type. \n\n
/// VDL_TYPE_DOUBLE: 2, signed double type. \n\n
/// VDL_TYPE_VECTOR: 3, vector type.
typedef enum VDL_TYPE_T
{
    VDL_TYPE_CHAR     = 0,
    VDL_TYPE_INT      = 1,
    VDL_TYPE_DOUBLE   = 2,
    VDL_TYPE_VECTOR_P = 3
} VDL_TYPE_T;

/// @description String representation of primitive array types.
static const char *const VDL_TYPE_STRING[4] = {
        [VDL_TYPE_CHAR]     = "VDL_TYPE_CHAR",
        [VDL_TYPE_INT]      = "VDL_TYPE_INT",
        [VDL_TYPE_DOUBLE]   = "VDL_TYPE_DOUBLE",
        [VDL_TYPE_VECTOR_P] = "VDL_TYPE_VECTOR_P"};

/// @description String representation of the type of a vector.
/// @param type (VDL_TYPE_T). A vector type.
/// @return (const char *) A string.
#define vdl_StringOfType(type) (VDL_TYPE_STRING[type])

/*-----------------------------------------------------------------------------
 |  Vector storage modes
 ----------------------------------------------------------------------------*/

/// @description Storage mode of a vector.
/// @details
/// VDL_MODE_STACK: 0, stack allocated. \n\n
/// VDL_MODE_HEAP: 1, heap allocated.
typedef enum VDL_MODE_T
{
    VDL_MODE_STACK = 0,
    VDL_MODE_HEAP  = 1
} VDL_MODE_T;

/// @description String representation of storage mode of a vector.
static const char *const VDL_MODE_STRING[2] = {
        [VDL_MODE_STACK] = "VDL_MODE_STACK",
        [VDL_MODE_HEAP]  = "VDL_MODE_HEAP"};

/// @description String representation of the mode of a vector.
/// @param mode (VDL_MODE_T). A vector mode.
/// @return (const char *) A string.
#define vdl_StringOfMode(mode) (VDL_MODE_STRING[mode])

/*-----------------------------------------------------------------------------
 |  Vector definition
 ----------------------------------------------------------------------------*/

/// @description Vector struct.
/// @param Type: (const VDL_TYPE). Type of the vector.
/// @param Mode: (const VDL_MODE). Storage mode of the vector.
/// @param Capacity (int). Capacity of the vector.
/// @param Length: (int). Length of the vector.
/// @param Data: (void*). Data pointer.
typedef struct VDL_VECTOR_T
{
    const VDL_TYPE_T Type;
    const VDL_MODE_T Mode;
    int Capacity;
    int Length;
    void *Data;
} VDL_VECTOR_T;

/// @description A pointer to a vector struct.
typedef VDL_VECTOR_T *VDL_VECTOR_P;

/*-----------------------------------------------------------------------------
 |  Missing values
 ----------------------------------------------------------------------------*/

/// @description Missing value of char.
#define VDL_CHAR_NA CHAR_MAX
/// @description Missing value of int.
#define VDL_INT_NA INT_MAX
/// @description Missing value of double.
#define VDL_DOUBLE_NA ((double) NAN)
/// @description Missing value of pointer.
#define VDL_VECTOR_NA NULL

/*-----------------------------------------------------------------------------
 |  Boolean values
 ----------------------------------------------------------------------------*/

/// @description Boolean type.
typedef int VDL_BOOL_T;

/// @description TRUE is an int.
#define VDL_FALSE ((int) 0)
/// @description FALSE is an int.
#define VDL_TRUE ((int) 1)

/*-----------------------------------------------------------------------------
 |  Cast void pointer to different types
 ----------------------------------------------------------------------------*/

/// @description Cast a void pointer to a char pointer
/// @param void_pointer (void *). A void pointer.
#define vdl_AsCharArray(void_pointer) ((char *) (void_pointer))
#define vdl_AsConstCharArray(void_pointer) ((const char *) (void_pointer))

/// @description Cast a void pointer to an int pointer
/// @param void_pointer (void *). A void pointer.
#define vdl_AsIntArray(void_pointer) ((int *) (void_pointer))
#define vdl_AsConstIntArray(void_pointer) ((const int *) (void_pointer))

/// @description Cast a void pointer to a double pointer
/// @param void_pointer (void *). A void pointer.
#define vdl_AsDoubleArray(void_pointer) ((double *) (void_pointer))
#define vdl_AsConstDoubleArray(void_pointer) ((const double *) (void_pointer))

/// @description Cast a void pointer to a VDL_VECTOR_P pointer
/// @param void_pointer (void *). A void pointer.
#define vdl_AsVectorPointerArray(void_pointer) ((VDL_VECTOR_T **) (void_pointer))
#define vdl_AsConstVectorPointerArray(void_pointer) ((const VDL_VECTOR_T **) (void_pointer))
#define vdl_AsVectorConstPointerArray(void_pointer) ((VDL_VECTOR_T *const *) (void_pointer))
#define vdl_AsConstVectorConstPointerArray(void_pointer) ((const VDL_VECTOR_T *const *) (void_pointer))

/*-----------------------------------------------------------------------------
 |  Size of type or vector
 ----------------------------------------------------------------------------*/

/// @description Get the size of a type.
/// @param type (const VDL_TYPE_T). Vector type.
#define vdl_SizeOfType(type) (VDL_TYPE_SIZE[type])
static const size_t VDL_TYPE_SIZE[4] = {
        [VDL_TYPE_CHAR]     = sizeof(char),
        [VDL_TYPE_INT]      = sizeof(int),
        [VDL_TYPE_DOUBLE]   = sizeof(double),
        [VDL_TYPE_VECTOR_P] = sizeof(VDL_VECTOR_P)};

/// @description Get the size of the data of a vector.
/// @param v (const VDL_VECTOR_T *const). A vector.
#define vdl_SizeOfData(v) ((size_t) (v)->Capacity * vdl_SizeOfType((v)->Type))

/// @description Get the size of a vector.
/// @param v (const VDL_VECTOR_T *const). A vector.
#define vdl_SizeOfVector(v) ((size_t) (v)->Capacity * vdl_SizeOfType((v)->Type) + sizeof(VDL_VECTOR_T))

/*-----------------------------------------------------------------------------
 |  Assert error
 ----------------------------------------------------------------------------*/

#define vdl_CheckNullPointer(p) vdl_Expect((p) != NULL,                   \
                                           VDL_EXCEPTION_NULL_POINTER,    \
                                           "Null pointer [%s] provided!", \
                                           #p)

#define vdl_CheckFailedAllocation(p) vdl_Expect((p) != NULL,                         \
                                                VDL_EXCEPTION_FAILED_ALLOCATION,     \
                                                "Memory allocation of [%s] failed!", \
                                                #p)

#define vdl_CheckIndexOutOfBound(v, i) vdl_Expect((i) >= 0 && (i) < (v)->Length,                    \
                                                  VDL_EXCEPTION_INDEX_OUT_OF_BOUND,                 \
                                                  "Index out of bound! Index [%d] not in [0, %d)!", \
                                                  i,                                                \
                                                  (v)->Length)

#define vdl_CheckType(input_type, expected_type) vdl_Expect((input_type) == (expected_type),                                    \
                                                            VDL_EXCEPTION_UNEXPECTED_TYPE,                                      \
                                                            "Unexpected vector type [%s] provided! Vector type [%s] Expected!", \
                                                            vdl_StringOfType(input_type),                                       \
                                                            vdl_StringOfType(expected_type))

#define vdl_CheckNumberOfItems(number) vdl_Expect((number) > 0,                                  \
                                                  VDL_EXCEPTION_NON_POSITIVE_NUMBER_OF_ITEMS,    \
                                                  "Non-positive number of items [%d] provided!", \
                                                  number)

/*-----------------------------------------------------------------------------
 |  Low level APIs
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Accessing the vector data unsafely
 ----------------------------------------------------------------------------*/

/// @description Get the pointer to the address of the ith item of a vector. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @return (char *) A pointer to the address.
#define vdl_UnsafeAddressOf(v, i) (vdl_AsCharArray((v)->Data) + vdl_SizeOfType((v)->Type) * (size_t) (i))

/// @description Get the ith item a vector as char. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @return (char) The item.
#define vdl_UnsafeCharAt(v, i) (vdl_AsCharArray((v)->Data)[i])
#define vdl_UnsafeConstCharAt(v, i) (vdl_AsConstCharArray((v)->Data)[i])

/// @description Get the ith item of a vector as int. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @return (int) The item.
#define vdl_UnsafeIntAt(v, i) (vdl_AsIntArray((v)->Data)[i])
#define vdl_UnsafeConstIntAt(v, i) (vdl_AsConstIntArray((v)->Data)[i])

/// @description Get the ith item of a vector as double. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @return (double) The item.
#define vdl_UnsafeDoubleAt(v, i) (vdl_AsDoubleArray((v)->Data)[i])
#define vdl_UnsafeConstDoubleAt(v, i) (vdl_AsConstDoubleArray((v)->Data)[i])

/// @description Get the ith item of a vector as VDL_VECTOR_P. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @return (VDL_VECTOR_P) The item.
#define vdl_UnsafeVectorPointerAt(v, i) (vdl_AsVectorPointerArray((v)->Data)[i])
#define vdl_UnsafeConstVectorPointerAt(v, i) (vdl_AsConstVectorPointerArray((v)->Data)[i])
#define vdl_UnsafeVectorConstPointerAt(v, i) (vdl_AsVectorConstPointerArray((v)->Data)[i])
#define vdl_UnsafeConstVectorConstPointerAt(v, i) (vdl_AsConstVectorConstPointerArray((v)->Data)[i])

/*-----------------------------------------------------------------------------
 |  Accessing the vector data safely
 ----------------------------------------------------------------------------*/

/// @description Get the address of an item from a vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). Index of the item.
/// @return (void *) Pointer to the item.
#define vdl_AddressOf(...) vdl_CallFunction(vdl_AddressOf_BT, void *, __VA_ARGS__)
static inline void *vdl_AddressOf_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    return vdl_UnsafeAddressOf(v, i);
}

/// @description Get a char from a vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). Index of the item.
/// @return (char) A char.
#define vdl_GetChar(...) vdl_CallFunction(vdl_GetChar_BT, char, __VA_ARGS__)
static inline char vdl_GetChar_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);
    return vdl_UnsafeConstCharAt(v, i);
}

/// @description Get an int from a vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). Index of the item.
/// @return (int) An int.
#define vdl_GetInt(...) vdl_CallFunction(vdl_GetInt_BT, int, __VA_ARGS__)
static inline int vdl_GetInt_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_INT);
    return vdl_UnsafeConstIntAt(v, i);
}

/// @description Get a double from a vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). Index of the item.
/// @return (double) An double.
#define vdl_GetDouble(...) vdl_CallFunction(vdl_GetDouble_BT, double, __VA_ARGS__)
static inline double vdl_GetDouble_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);
    return vdl_UnsafeConstDoubleAt(v, i);
}

/// @description Get a vector pointer from a vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). Index of the item.
/// @return (VDL_VECTOR_P) A vector pointer.
#define vdl_GetVectorPointer(...) vdl_CallFunction(vdl_GetVectorPointer_BT, VDL_VECTOR_P, __VA_ARGS__)
static inline VDL_VECTOR_P vdl_GetVectorPointer_BT(const VDL_VECTOR_T *const v, const int i)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);
    return vdl_UnsafeVectorConstPointerAt(v, i);
}

/*-----------------------------------------------------------------------------
 |  Set the vector data unsafely
 ----------------------------------------------------------------------------*/

/// @description Set the ith item of a char vector. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @param item (const char). An item.
#define vdl_UnsafeSetChar(v, i, item)         \
    do {                                      \
        vdl_AsCharArray((v)->Data)[i] = item; \
    } while (0)

/// @description Set the ith item of an int vector. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @param item (const int). An item.
#define vdl_UnsafeSetInt(v, i, item)         \
    do {                                     \
        vdl_AsIntArray((v)->Data)[i] = item; \
    } while (0)

/// @description Set the ith item of a double vector. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @param item (const double). An item.
#define vdl_UnsafeSetDouble(v, i, item)         \
    do {                                        \
        vdl_AsDoubleArray((v)->Data)[i] = item; \
    } while (0)

/// @description Set the ith item of a VDL_VECTOR_P vector. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @param item (const VDL_VECTOR_T *const). An item.
#define vdl_UnsafeSetVectorPointer(v, i, item)              \
    do {                                                    \
        vdl_AsConstVectorPointerArray((v)->Data)[i] = item; \
    } while (0)

/// @description Set multiple items of a vector by using `memmove`. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). The starting index.
/// @param item_pointer (const void *const). A pointer to items.
/// @param number (const int). Number of items to be set.
#define vdl_UnsafeSetByMemMove(v, i, item_pointer, number)                   \
    do {                                                                     \
        void *_begin = vdl_UnsafeAddressOf(v, i);                            \
        memmove(_begin, item_pointer, vdl_SizeOfType((v)->Type) * (number)); \
    } while (0)

/// @description Set multiple items of a char vector by indices. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param item_pointer (const char *const). A pointer to items.
/// @param index_pointer (const int *const). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_UnsafeSetCharByIndices(v, item_pointer, index_pointer, number)   \
    do {                                                                     \
        vdl_for_j(number)                                                    \
        {                                                                    \
            const int current_index = vdl_AsConstIntArray(index_pointer)[j]; \
            const char current_item = vdl_AsConstCharArray(item_pointer)[j]; \
            vdl_UnsafeSetChar(v, current_index, current_item);               \
        }                                                                    \
    } while (0)

/// @description Set multiple items of an int vector by indices. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param item_pointer (const int *const). A pointer to items.
/// @param index_pointer (const int *const). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_UnsafeSetIntByIndices(v, item_pointer, index_pointer, number)    \
    do {                                                                     \
        vdl_for_j(number)                                                    \
        {                                                                    \
            const int current_index = vdl_AsConstIntArray(index_pointer)[j]; \
            const int current_item  = vdl_AsConstIntArray(item_pointer)[j];  \
            vdl_UnsafeSetInt(v, current_index, current_item);                \
        }                                                                    \
    } while (0)

/// @description Set multiple items of a double vector by indices. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param item_pointer (const double *const). A pointer to items.
/// @param index_pointer (const int *const). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_UnsafeSetDoubleByIndices(v, item_pointer, index_pointer, number)     \
    do {                                                                         \
        vdl_for_j(number)                                                        \
        {                                                                        \
            const int current_index   = vdl_AsConstIntArray(index_pointer)[j];   \
            const double current_item = vdl_AsConstDoubleArray(item_pointer)[j]; \
            vdl_UnsafeSetDouble(v, current_index, current_item);                 \
        }                                                                        \
    } while (0)

/// @description Set multiple items of a VDL_VECTOR_P vector by indices. No checks will be performed.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param item_pointer (const VDL_VECTOR_T *const *const). A pointer to items.
/// @param index_pointer (const int *const). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_UnsafeSetVectorPointerByIndices(v, item_pointer, index_pointer, number)                       \
    do {                                                                                                  \
        vdl_for_j(number)                                                                                 \
        {                                                                                                 \
            const int current_index                = vdl_AsConstIntArray(index_pointer)[j];               \
            const VDL_VECTOR_T *const current_item = vdl_AsConstVectorConstPointerArray(item_pointer)[j]; \
            vdl_UnsafeSetVectorPointer(v, current_index, current_item);                                   \
        }                                                                                                 \
    } while (0)

/*-----------------------------------------------------------------------------
 |  Set the vector data safely
 ----------------------------------------------------------------------------*/

/// @description Set the ith item of a char vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @param item (const char). An item.
#define vdl_SetChar(...) vdl_CallVoidFunction(vdl_SetChar_BT, __VA_ARGS__)
static inline void vdl_SetChar_BT(const VDL_VECTOR_T *const v, const int i, const char item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);
    vdl_UnsafeSetChar(v, i, item);
}

/// @description Set the ith item of an int vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @param item (const int). An item.
#define vdl_SetInt(...) vdl_CallVoidFunction(vdl_SetInt_BT, __VA_ARGS__)
static inline void vdl_SetInt_BT(const VDL_VECTOR_T *const v, const int i, const int item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_INT);
    vdl_UnsafeSetInt(v, i, item);
}

/// @description Set the ith item of a double vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @param item (const double). An item.
#define vdl_SetDouble(...) vdl_CallVoidFunction(vdl_SetDouble_BT, __VA_ARGS__)
static inline void vdl_SetDouble_BT(const VDL_VECTOR_T *const v, const int i, const double item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);
    vdl_UnsafeSetDouble(v, i, item);
}

/// @description Set the ith item of a VDL_VECTOR_P vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). An index.
/// @param item (const VDL_VECTOR_T *const). An item.
#define vdl_SetVectorPointer(...) vdl_CallVoidFunction(vdl_SetVectorPointer_BT, __VA_ARGS__)
static inline void vdl_SetVectorPointer_BT(const VDL_VECTOR_T *const v, const int i, const VDL_VECTOR_T *const item)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);
    vdl_UnsafeSetVectorPointer(v, i, item);
}

/// @description Set many items of a char vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). The staring index.
/// @param item_pointer (const char *const). A pointer to items.
/// @param number (const int). Number of items to be set.
#define vdl_SetCharByMemMove(...) vdl_CallVoidFunction(vdl_SetCharByMemMove_BT, __VA_ARGS__)
static inline void vdl_SetCharByMemMove_BT(const VDL_VECTOR_T *const v, const int i, const char *const item_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);
    vdl_UnsafeSetByMemMove(v, i, item_pointer, (size_t) number);
}

/// @description Set many items of an int vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). The staring index.
/// @param item_pointer (const int *const). A pointer to items.
/// @param number (const int). Number of items to be set.
#define vdl_SetIntByMemMove(...) vdl_CallVoidFunction(vdl_SetIntByMemMove_BT, __VA_ARGS__)
static inline void vdl_SetIntByMemMove_BT(const VDL_VECTOR_T *const v, const int i, const int *const item_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);
    vdl_CheckType(v->Type, VDL_TYPE_INT);
    vdl_UnsafeSetByMemMove(v, i, item_pointer, (size_t) number);
}

/// @description Set many items of a double vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). The staring index.
/// @param item_pointer (const double *const). A pointer to items.
/// @param number (const int). Number of items to be set.
#define vdl_SetDoubleByMemMove(...) vdl_CallVoidFunction(vdl_SetDoubleByMemMove_BT, __VA_ARGS__)
static inline void vdl_SetDoubleByMemMove_BT(const VDL_VECTOR_T *const v, const int i, const double *const item_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);
    vdl_UnsafeSetByMemMove(v, i, item_pointer, (size_t) number);
}

/// @description Set many items of a double vector. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param i (const int). The staring index.
/// @param item_pointer (const VDL_VECTOR_T *const *const). A pointer to items.
/// @param number (const int). Number of items to be set.
#define vdl_SetVectorPointerByMemMove(...) vdl_CallVoidFunction(vdl_SetVectorPointerByMemMove_BT, __VA_ARGS__)
static inline void vdl_SetVectorPointerByMemMove_BT(const VDL_VECTOR_T *const v, const int i, const VDL_VECTOR_T *const *const item_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckIndexOutOfBound(v, i);
    vdl_CheckIndexOutOfBound(v, i + number - 1);
    vdl_CheckType(v->Type, VDL_TYPE_VECTOR_P);
    vdl_UnsafeSetByMemMove(v, i, item_pointer, (size_t) number);
}


/// @description Set multiple items of a char vector by indices. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param item_pointer (const char *const). A pointer to items.
/// @param index_pointer (const int *const). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_SetCharByIndices(...) vdl_CallVoidFunction(vdl_SetCharByIndices_BT, __VA_ARGS__)
static inline void vdl_SetCharByIndices_BT(const VDL_VECTOR_T *const v, const char *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckType(v->Type, VDL_TYPE_CHAR);
    vdl_for_j(number)
    {
        const int current_index = vdl_AsConstIntArray(index_pointer)[j];
        const char current_item = vdl_AsConstCharArray(item_pointer)[j];
        vdl_UnsafeSetChar(v, current_index, current_item);
    }
}


/// @description Set multiple items of an int vector by indices. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param item_pointer (const int *const). A pointer to items.
/// @param index_pointer (const int *const). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_SetIntByIndices(...) vdl_CallVoidFunction(vdl_SetIntByIndices_BT, __VA_ARGS__)
static inline void vdl_SetIntByIndices_BT(const VDL_VECTOR_T *const v, const int *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckType(v->Type, VDL_TYPE_INT);
    vdl_for_j(number)
    {
        const int current_index = vdl_AsConstIntArray(index_pointer)[j];
        const int current_item  = vdl_AsConstIntArray(item_pointer)[j];
        vdl_UnsafeSetInt(v, current_index, current_item);
    }
}

/// @description Set multiple items of a double vector by indices. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param item_pointer (const double *const). A pointer to items.
/// @param index_pointer (const int *const). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_SetDoubleByIndices(...) vdl_CallVoidFunction(vdl_SetDoubleByIndices_BT, __VA_ARGS__)
static inline void vdl_SetDoubleByIndices_BT(const VDL_VECTOR_T *const v, const double *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);
    vdl_for_j(number)
    {
        const int current_index   = vdl_AsConstIntArray(index_pointer)[j];
        const double current_item = vdl_AsConstDoubleArray(item_pointer)[j];
        vdl_UnsafeSetDouble(v, current_index, current_item);
    }
}

/// @description Set multiple items of a VDL_VECTOR_P vector by indices. Boundary conditions will be checked.
/// @param v (const VDL_VECTOR_T *const). A vector.
/// @param item_pointer (const VDL_VECTOR_T *const *const). A pointer to items.
/// @param index_pointer (const int *const). A pointer to indices.
/// @param number (const int). Number of items.
#define vdl_SetVectorPointerByIndices(...) vdl_CallVoidFunction(vdl_SetVectorPointerByIndices_BT, __VA_ARGS__)
static inline void vdl_SetVectorPointerByIndices_BT(const VDL_VECTOR_T *const v, const VDL_VECTOR_T *const *const item_pointer, const int *const index_pointer, const int number)
{
    vdl_CheckNullPointer(v);
    vdl_CheckNullPointer(v->Data);
    vdl_CheckNumberOfItems(number);
    vdl_CheckType(v->Type, VDL_TYPE_DOUBLE);
    vdl_for_j(number)
    {
        const int current_index                = vdl_AsConstIntArray(index_pointer)[j];
        const VDL_VECTOR_T *const current_item = vdl_AsConstVectorConstPointerArray(item_pointer)[j];
        vdl_UnsafeSetVectorPointer(v, current_index, current_item);
    }
}

/*-----------------------------------------------------------------------------
 |  Memory bookkeeping
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Vector table
 ----------------------------------------------------------------------------*/

#define VDL_GARBAGE_COLLECTOR_INIT_CAPACITY 8

static inline VDL_VECTOR_T *vdl_NewGarbageCollectorVectorTable(void)
{
    // Allocate memory for the metadata
    VDL_VECTOR_T *vector_table = malloc(sizeof(VDL_VECTOR_T));
    vdl_CheckFailedAllocation(vector_table);

    // Copy in the metadata
    VDL_VECTOR_T local_vector = (VDL_VECTOR_T){.Type     = VDL_TYPE_VECTOR_P,
                                               .Mode     = VDL_MODE_HEAP,
                                               .Capacity = 0,
                                               .Length   = 0,
                                               .Data     = NULL};
    memcpy(vector_table, &local_vector, sizeof(VDL_VECTOR_T));

    // Allocate memory for the data container
    void *local_buffer = malloc(VDL_GARBAGE_COLLECTOR_INIT_CAPACITY * vdl_SizeOfType(VDL_TYPE_VECTOR_P));
    if (local_buffer == NULL)
    {
        // Deallocate the new vector table if the allocation of the data container failed
        free(vector_table);
        vdl_CheckFailedAllocation(local_buffer);
    }
    else
    {
        vector_table->Data = local_buffer;
        return vector_table;
    }
    return NULL;
}

static inline void vdl_ShrinkGarbageCollectorVectorTable(VDL_VECTOR_T *const vector_table)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);

    // Decide the target size
    const int size_times_five = vector_table->Length * 5;
    const int size_plus_5MB   = vector_table->Length + 700000;
    int smaller_cap           = size_times_five > size_plus_5MB ? size_plus_5MB : size_times_five;
    if (smaller_cap < VDL_GARBAGE_COLLECTOR_INIT_CAPACITY)
        smaller_cap = VDL_GARBAGE_COLLECTOR_INIT_CAPACITY;
    if (vector_table->Capacity <= smaller_cap)
        return;

    // Attempt to reallocate a smaller data container
    void *buffer = realloc(vector_table->Data, (size_t) smaller_cap * vdl_SizeOfType(VDL_TYPE_VECTOR_P));
    vdl_CheckFailedAllocation(buffer);

    vector_table->Data     = buffer;
    vector_table->Capacity = smaller_cap;
}

static inline void vdl_CleanGarbageCollectorVectorTable(VDL_VECTOR_T *const vector_table, const VDL_BOOL_T free_content)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);

    // Free the recorded vectors
    if (free_content)
    {
        vdl_for_i(vector_table->Length)
        {
            VDL_VECTOR_T *const current_item = vdl_AsVectorConstPointerArray(vector_table)[i];
            vdl_CheckNullPointer(current_item);
            vdl_CheckNullPointer(current_item->Data);
            free(current_item->Data);
            free(current_item);
        }
    }

    // Reset the vector table
    vector_table->Length = 0;
}

static inline void vdl_DeleteGarbageCollectorVectorTable(VDL_VECTOR_T *const vector_table, const VDL_BOOL_T free_content)
{
    vdl_CheckNullPointer(vector_table);
    vdl_CheckNullPointer(vector_table->Data);

    // Free the recorded vectors
    if (free_content)
    {
        vdl_for_i(vector_table->Length)
        {
            VDL_VECTOR_T *const current_item = vdl_AsVectorConstPointerArray(vector_table)[i];
            vdl_CheckNullPointer(current_item);
            vdl_CheckNullPointer(current_item->Data);
            free(current_item->Data);
            free(current_item);
        }
    }

    // Free the vector table
    free(vector_table->Data);
    free(vector_table);
}

static inline void vdl_ReserveGarbageCollectorVectorTable(VDL_VECTOR_T *const vector_table, const int capacity)
{
}


#endif//VDL_VDL_H
