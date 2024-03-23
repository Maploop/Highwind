#pragma once

#include <iostream>
#include <stdarg.h>

// Ignore ----------------------------------------- Ignore
#define YES 1
#define NO 0
// Ignore ----------------------------------------- Ignore

// Configurations --------------------------------- Configurations
#define RELEASE NO

#define LOG_WARN_ENABLED YES
#define LOG_INFO_ENABLED YES
#define LOG_DEBUG_ENABLED YES
#define LOG_TRACE_ENABLED YES
// Configurations --------------------------------- Configurations

// Code Start ------------------------------------- Code Start
#if RELEASE == YES
#define LOG_DEBUG_ENABLED NO
#define LOG_TRACE_ENABLED NO
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} log_level;

inline void log_output(log_level level, const char* message, ...) {
    const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARNING]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    bool is_error = level < LOG_LEVEL_WARN;

    char out_message[32000];
    memset(out_message, 0, sizeof(out_message));

    // Format orignal message
    va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(out_message, 32000, message, arg_ptr);
    va_end(arg_ptr);

    char out_message2[32000];
    sprintf_s(out_message2, "%s%s\n", level_strings[level], out_message);

    // NOTE: This will be platform specific!
    std::cout << out_message2;
}

#define HFATAL(message, ...) log_output(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef HERROR
#define HERROR(message, ...) log_output(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_INFO_ENABLED == 1
#define HINFO(message, ...) log_output(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define HINFO(message, ...)
#endif

#if LOG_WARN_ENABLED == 1
#define HWARN(message, ...) log_output(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define HWARN(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define HDEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define HDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define HTRACE(message, ...) log_output(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define HTRACE(message, ...)
#endif

inline void report_assertion_failure(const char* expression, const char* message, const char* file, int line) {
    log_output(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: %s, in file: %s, in line: %d\n", expression, message, file, line);
}