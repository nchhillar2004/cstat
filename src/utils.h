/*
  utils.h
  This file contains common utility and helper functions and macros for cstat
*/

#ifndef cstat_utils_h
#define cstat_utils_h

#include <stdbool.h>
    #include <sys/resource.h>
    #include <sys/time.h>
    #include <unistd.h>

typedef enum {
    CSTAT_LOG_ERROR,
    CSTAT_LOG_WARNING,
    CSTAT_LOG_SUCCESS,
    CSTAT_LOG_DEBUG
} LogType;

typedef struct {
    char **items;
    size_t size;
    size_t capacity;
} DynamicArray;

typedef struct {
    DynamicArray ignoredDir;
    DynamicArray ignoredExt;
} GitIgnore;

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
#define _LOG(type, ...) (_cstat_log(__CURRENT_FILE_NAME__, __func__, (LogType)type, __VA_ARGS__))

/* ===== FUNCTIONS ===== */

// get available threads on the system
unsigned int getAvailableThreads();

const char *getFilenameFromPath(const char *filepath);

double getTime();

bool hasExtension(const char *path, const char *ext);
bool isIgnoredExt(const char *filepath);
bool isIgnoredDir(const char *dirname);
void parseGitIgnore(GitIgnore *gitIgnore, int fd, const char *path);

void push(DynamicArray *arr, const char *val);
int find(DynamicArray *arr, const char *val);

/* internal/private functions */
void _cstat_log(const char *filename, const char *function, LogType type, const char *fmt, ...);

#endif
