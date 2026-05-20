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
    CSTAT_LOG_ERROR,
    CSTAT_LOG_WARNING,
    CSTAT_LOG_SUCCESS,
    CSTAT_LOG_DEBUG
} LogType;

/* global log toggle */
extern bool CSTAT_DISPLAY_LOGS;


/* ===== MACROS ===== */

/* explicit macro functions */
// logging
#define logError(...) _LOG(CSTAT_LOG_ERROR, __VA_ARGS__)
#define logWarn(...) _LOG(CSTAT_LOG_WARNING, __VA_ARGS__)
#define logSuccess(...) _LOG(CSTAT_LOG_SUCCESS, __VA_ARGS__)
#define logDebug(...) _LOG(CSTAT_LOG_DEBUG, __VA_ARGS__)

/* internal macros */
#define __CURRENT_FILE_NAME__ (getFilenameFromPath(__FILE__))
#define _LOG(type, ...) (_cstat_log(__TIME__, __CURRENT_FILE_NAME__, __func__, (LogType)type, __VA_ARGS__))


/* ===== FUNCTIONS ===== */

// get available threads on the system
unsigned int getAvailableThreads();

const char *getFilenameFromPath(const char *filepath);


/* internal/private functions */
void _cstat_log(const char *time, const char *filename, const char *function, LogType type, const char *fmt, ...);

#endif
