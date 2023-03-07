//
// Created by Patrick Li on 20/2/2023.
//

#ifndef VDL_VDL_2_EXCEPTION_H
#define VDL_VDL_2_EXCEPTION_H

/*-----------------------------------------------------------------------------
 |  Exception
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 |  Exception type and none value
 ----------------------------------------------------------------------------*/

/// The actual exception type.
#define VDL_EXCEPTION_T int

/// The reserved value representing no exception.
#define VDL_EXCEPTION_NONE (1234)

/*-----------------------------------------------------------------------------
 |  Define exceptions IDs
 ----------------------------------------------------------------------------*/

#define VDL_EXCEPTION_EXCEED_STACK_LIMIT 0x1
#define VDL_EXCEPTION_INDEX_OUT_OF_BOUND 0x2
#define VDL_EXCEPTION_NULL_POINTER 0x3
#define VDL_EXCEPTION_UNEXPECTED_TYPE 0x4
#define VDL_EXCEPTION_NON_POSITIVE_NUMBER_OF_ITEMS 0x5
#define VDL_EXCEPTION_FAILED_ALLOCATION 0x6
#define VDL_EXCEPTION_EXCEED_VECTOR_TABLE_LIMIT 0x7
#define VDL_EXCEPTION_EXCEED_CLEANUP_LIMIT 0x8
#define VDL_EXCEPTION_INCONSISTENT_GARBAGE_COLLECTOR_STATE 0x9
#define VDL_EXCEPTION_NON_POSITIVE_CAPACITY 0xa
#define VDL_EXCEPTION_EXCEED_VECTOR_CAPACITY_LIMIT 0xb
#define VDL_EXCEPTION_UNEXPECTED_MODE 0xc
#define VDL_EXCEPTION_UNEXPECTED_LENGTH 0xd
#define VDL_EXCEPTION_INTEGER_OVERFLOW 0xe
#define VDL_EXCEPTION_INCOMPATIBLE_LENGTH 0xf
#define VDL_EXCEPTION_NON_POSITIVE_LENGTH 0x10
#define VDL_EXCEPTION_UNKNOWN_TYPE 0x11
#define VDL_EXCEPTION_EMPTY_ATTRIBUTE 0x12
#define VDL_EXCEPTION_ATTRIBUTE_NOT_FOUND 0x13
#define VDL_EXCEPTION_MISSING_VALUE 0x14

/*-----------------------------------------------------------------------------
 |  Error message
 ----------------------------------------------------------------------------*/

/// Maximum length of error message.
#define VDL_ERROR_MESSAGE_MAX_LEN 256

/// @desciption A global variable for storing error message.
static char vdl_GlobalVar_ErrorMessage[VDL_ERROR_MESSAGE_MAX_LEN] = {0};

/// Save error message to the global buffer.
/// @details The buffer will be reset if an encoding error occurred.
/// @param exception_id (VDL_EXCEPTION_T). The exception ID.
/// @param function_name (const char *). Function name.
/// @param file_name (const char *). File name.
/// @param line (int). The line number.
/// @param format (const char *). Format for the additional error message.
/// @param ... Additional arguments passed to snprintf.
static inline void vdl_SaveErrorMessage(VDL_EXCEPTION_T exception_id,
                                        const char *function_name,
                                        const char *file_name,
                                        int line,
                                        const char *format,
                                        ...);

/*-----------------------------------------------------------------------------
 |  Exception clean up
 ----------------------------------------------------------------------------*/

#define VDL_EXCEPTION_CLEAN_UP_MAX_FUNCTION_NUM 256
#define VDL_EXCEPTION_CLEAN_UP_EXTRA_FUNCTION_NUM 5

/// A global variable for storing objects and their corresponding cleanup functions.
/// @param Len (int). Number of objects.
/// @param Object (void * [VDL_EXCEPTION_CLEAN_UP_MAX_FUNCTION_NUM + VDL_EXCEPTION_CLEAN_UP_EXTRA_FUNCTION_NUM]). Objects.
/// @param CleanUpFunction (void (* [VDL_EXCEPTION_CLEAN_UP_MAX_FUNCTION_NUM + VDL_EXCEPTION_CLEAN_UP_EXTRA_FUNCTION_NUM])(void *)). Cleanup functions.
static struct
{
    int Length;
    void *Object[VDL_EXCEPTION_CLEAN_UP_MAX_FUNCTION_NUM + VDL_EXCEPTION_CLEAN_UP_EXTRA_FUNCTION_NUM];
    void (*CleanUpFunction[VDL_EXCEPTION_CLEAN_UP_MAX_FUNCTION_NUM + VDL_EXCEPTION_CLEAN_UP_EXTRA_FUNCTION_NUM])(void *);
} vdl_GlobalVar_ExceptionCleanUp = {0};

/// Register an object and its corresponding cleanup function.
/// @details The cleanup function will be called when an exception is raised.
/// @param object (void *). Object.
/// @param clean_up_function (void (*)(void *)). Cleanup function.
static inline void vdl_ExceptionRegisterCleanUp(void *object, void (*clean_up_function)(void *));

/// Deregister an object and its corresponding cleanup function.
#define vdl_ExceptionDeregisterCleanUp(object)         \
    do {                                               \
        if (vdl_GlobalVar_ExceptionCleanUp.Length > 0) \
            vdl_GlobalVar_ExceptionCleanUp.Length--;   \
    } while (0)

/// Run all the cleanup functions and deregister all of them.
static inline void vdl_ExceptionCleanUp(void);

/*-----------------------------------------------------------------------------
 |  Exception
 ----------------------------------------------------------------------------*/

/// A volatile global variable for storing exception frames.
/// @param Frame (jmp_buf *). The current frame.
/// @param Exception (volatile VDL_EXCEPTION_T). The exception ID.
static volatile struct
{
    jmp_buf *Frame;
    volatile VDL_EXCEPTION_T Exception;
} vdl_GlobalVar_ExceptionFrames = {0};

/// Get the current exception ID.
#define vdl_GetExceptionID() ((VDL_EXCEPTION_T){vdl_GlobalVar_ExceptionFrames.Exception})

/*-----------------------------------------------------------------------------
 |  No catch handler
 ----------------------------------------------------------------------------*/

/// The default handler for no catch exception.
/// @details The default handler prints the error message and abort the program.
_Noreturn static void vdl_DefaultNoCatchHandler(void);

/// A special handler for no catch exception.
#define vdl_NoCatchHandler() vdl_DefaultNoCatchHandler()

/*-----------------------------------------------------------------------------
 |  Exception long jump
 ----------------------------------------------------------------------------*/

/// Attempt to perform a long jump because of an exception.
/// @details This function will not perform a long jump if the catch statement
/// is missing
/// @param exception_id (VDL_EXCEPTION_T). Exception ID.
static inline void vdl_ExceptionAttemptLongJmp(VDL_EXCEPTION_T exception_id);

/*-----------------------------------------------------------------------------
 |  Throw
 ----------------------------------------------------------------------------*/

// If the exception is disabled, no jump will be performed.
#ifdef VDL_EXCEPTION_DISABLE
    /// Save an error message and throw an exception.
    /// @details Since the exception is disabled, the program will be aborted.
    /// @param exception_id (VDL_EXCEPTION_T). Exception ID.
    /// @param message (const char *). An error message.
    #define vdl_Throw(exception_id, format, ...)                                                     \
        do {                                                                                         \
            vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
            vdl_ExceptionCleanUp();                                                                  \
            vdl_NoCatchHandler();                                                                    \
        } while (0)
#else
    /// Save an error message and throw an exception.
    /// @param exception_id (VDL_EXCEPTION_T). Exception ID.
    /// @param format (const char *). The format of the error message.
    /// @param ... Additional arguments used for snprintf.
    #define vdl_Throw(exception_id, format, ...)                                                     \
        do {                                                                                         \
            vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
            vdl_ExceptionCleanUp();                                                                  \
            vdl_ExceptionAttemptLongJmp(exception_id);                                               \
            vdl_NoCatchHandler();                                                                    \
        } while (0)
#endif//VDL_EXCEPTION_DISABLE

/*-----------------------------------------------------------------------------
 |  Hooks in try and catch
 ----------------------------------------------------------------------------*/

/// Inject code before setjmp.
#define VDL_EXCEPTION_HOOK_START_TRY

/// Inject code if no exception thrown.
#define VDL_EXCEPTION_HOOK_HAPPY_TRY

/// Inject code after try in regrades of exception.
#define VDL_EXCEPTION_HOOK_AFTER_TRY

/// Inject code before catch (an exception thrown).
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
    /// Catch an exception.
    /// @details Since the exception is disabled, the program will be aborted
    /// if an exception occurred. This block of code will never be run.
    #define vdl_Catch \
        else {}       \
        }             \
        if (0)
#else
    /// @dscription Catch an exception.
    /// @details vdl_Try needs to be paired with vdl_Catch.
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

/// Exit the current vdl_Try block and skip one vdl_Catch.
#define vdl_ExitTry() vdl_Throw(VDL_EXCEPTION_NONE, "")

/*-----------------------------------------------------------------------------
 |  Expect an expression to be true
 ----------------------------------------------------------------------------*/

/// Expect a condition to be true.
/// @param exception_id (VDL_EXCEPTION). Exception ID.
/// @param format (const char *). The format of the error message.
/// @param ... Additional arguments used for snprintf.
#define vdl_Expect(condition, exception_id, format, ...)    \
    do {                                                    \
        if ((condition) == 0)                               \
        {                                                   \
            vdl_Throw(exception_id, format, ##__VA_ARGS__); \
        }                                                   \
    } while (0)

#endif//VDL_VDL_2_EXCEPTION_H
