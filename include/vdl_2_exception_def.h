//
// Created by Patrick Li on 20/2/2023.
//

#ifndef VDL_VDL_2_EXCEPTION_DEF_H
#define VDL_VDL_2_EXCEPTION_DEF_H

/*-----------------------------------------------------------------------------
 |  Error message
 ----------------------------------------------------------------------------*/

static inline void vdl_SaveErrorMessage(const VDL_EXCEPTION_T exception_id,
                                        const char *const function_name,
                                        const char *const file_name,
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
 |  Exception clean up
 ----------------------------------------------------------------------------*/

static inline void vdl_ExceptionRegisterCleanUp(void *const object, void (*const clean_up_function)(void *))
{
    int current_len = vdl_GlobalVar_ExceptionCleanUp.Length;
    // Store the object and the cleanup function
    vdl_GlobalVar_ExceptionCleanUp.Object[current_len]          = object;
    vdl_GlobalVar_ExceptionCleanUp.CleanUpFunction[current_len] = clean_up_function;
    vdl_GlobalVar_ExceptionCleanUp.Length++;

    // If the cleanup stack is full, throw an exception
    if (current_len >= VDL_EXCEPTION_CLEAN_UP_MAX_FUNCTION_NUM)
    {
        vdl_Throw(VDL_EXCEPTION_EXCEED_CLEANUP_LIMIT,
                  "Exceed the cleanup limit allowed by the exception [%d]!",
                  VDL_EXCEPTION_CLEAN_UP_MAX_FUNCTION_NUM);
    }
}

static inline void vdl_ExceptionCleanUp(void)
{
    for (int i = vdl_GlobalVar_ExceptionCleanUp.Length - 1; i >= 0; i--)
    {
        vdl_GlobalVar_ExceptionCleanUp.CleanUpFunction[i](vdl_GlobalVar_ExceptionCleanUp.Object[i]);
    }
    vdl_GlobalVar_ExceptionCleanUp.Length = 0;
}

/*-----------------------------------------------------------------------------
 |  No catch handler
 ----------------------------------------------------------------------------*/

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

/*-----------------------------------------------------------------------------
 |  Exception long jump
 ----------------------------------------------------------------------------*/

static inline void vdl_ExceptionAttemptLongJmp(const VDL_EXCEPTION_T exception_id)
{
    vdl_GlobalVar_ExceptionFrames.Exception = exception_id;
    if (vdl_GlobalVar_ExceptionFrames.Frame)
        longjmp(*vdl_GlobalVar_ExceptionFrames.Frame, 1);
}

#endif//VDL_VDL_2_EXCEPTION_DEF_H
