/*
  utils.h
  This file contains common utility and helper functions and macros for cstat
*/

#ifndef cstat_utils_h
#define cstat_utils_h

#include <stdbool.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

typedef enum {
    ERROR,
    WARN,
    SUCCESS,
    DEBUG
} LogType;

/* global log toggle */
extern bool CSTAT_DISPLAY_LOGS;

/* MACROS */

/* explicit macro functions */
#define logError(...) _LOG(ERROR, __VA_ARGS__)
#define logWarn(...) _LOG(WARN, __VA_ARGS__)
#define logSuccess(...) _LOG(SUCCESS, __VA_ARGS__)
#define logDebug(...) _LOG(DEBUG, __VA_ARGS__)

/* internal macros */
#ifdef _MSC_VER
    // TODO: display filename in MSVC, use __FILE__ and trim the complete path to just the filename
    #define __CURRENT_FILE_NAME__ (TARGET_NAME)
#else
    #define __CURRENT_FILE_NAME__ (__FILE_NAME__)
#endif
#define _LOG(type, ...) (_cstat_log(__TIME__, __CURRENT_FILE_NAME__, __func__, (LogType)type, __VA_ARGS__))

/* FUNCTIONS */
// get availabel threads on the system
unsigned int getAvailableThreads();

/* internal/private functions */
void _cstat_log(const char *time, const char *filename, const char *function, LogType type, const char *fmt, ...);

#endif
