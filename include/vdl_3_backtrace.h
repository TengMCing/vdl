//
// Created by Patrick Li on 20/2/2023.
//

#ifndef VDL_VDL_3_BACKTRACE_H
#define VDL_VDL_3_BACKTRACE_H


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
static inline void vdl_BackupBacktrace(void);

/*-----------------------------------------------------------------------------
 |  Print backtrace
 ----------------------------------------------------------------------------*/

/// @description Print a backtrace.
/// @param bt (VDL_BACKTRACE_T). A backtrace.
static inline void vdl_PrintBacktrace(VDL_BACKTRACE_T bt);

/*-----------------------------------------------------------------------------
 |  Redefine the special handler for unhandled exceptions
 ----------------------------------------------------------------------------*/

/// @description The customized handler for no catch exception.
/// It can print backtrace.
_Noreturn static void vdl_BacktraceNoCatchHandler(void);

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

// If the backtrace is enabled, redefine the throw statement.
#ifndef VDL_BACKTRACE_DISABLE
    #ifdef vdl_Throw
        #undef vdl_Throw
    #endif//vdl_Throw
    #ifdef VDL_EXCEPTION_DISABLE
        /// @description Save the backtrace and an error message and throw an exception.
        /// @details Since the exception is disabled, the program will be aborted if an exception
        /// occurred.
        /// @param exception_id (VDL_EXCEPTION_T). Exception ID.
        /// @param format (const char *). The format of the error message.
        /// @param ... Additional arguments used for snprintf.
        #define vdl_Throw(exception_id, format, ...)                                                     \
            do {                                                                                         \
                vdl_BackupBacktrace();                                                                   \
                vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
                vdl_ExceptionCleanUp();                                                                  \
                vdl_NoCatchHandler();                                                                    \
            } while (0)
    #else
        /// @description Save the backtrace and an error message and throw an exception.
        /// @param exception_id (VDL_EXCEPTION_T). Exception ID.
        /// @param format (const char *). The format of the error message.
        /// @param ... Additional arguments used for snprintf.
        #define vdl_Throw(exception_id, format, ...)                                                     \
            do {                                                                                         \
                vdl_BackupBacktrace();                                                                   \
                vdl_SaveErrorMessage(exception_id, __func__, __FILE__, __LINE__, format, ##__VA_ARGS__); \
                vdl_ExceptionCleanUp();                                                                  \
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
/// @param function_name (const char *). A string literal.
#define vdl_MakeFrame(function_name)                                            \
    (VDL_FRAME_T)                                                               \
    {                                                                           \
        .Line = __LINE__, .FileName = __FILE__, .FunctionName = (function_name) \
    }

/*-----------------------------------------------------------------------------
 |  Push frame to backtrace
 ----------------------------------------------------------------------------*/

/// @description Push the frame to the top of the backtrace stack.
/// @param fr (VDL_FRAME_T). The frame.
static inline void vdl_PushFrameToBacktrace(VDL_FRAME_T fr);

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


#endif//VDL_VDL_3_BACKTRACE_H
