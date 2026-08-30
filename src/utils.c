#include "utils.h"
#include "cstat.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool CSTAT_DISPLAY_LOGS = DEFAULT_DISPLAY_LOGS;

unsigned int getAvailableThreads() {
    // number of processors currently online (available)
    long cpus = 0;

    cpus = sysconf(_SC_NPROCESSORS_ONLN);

    // if error cpus will be equal to -1
    // in that case use DEFAULT_WORKER_THREADS
    if (cpus <= 0)
        return DEFAULT_WORKER_THREADS;

    // TODO: should i use this ?
    if (cpus > 64)
        return 64; //

    return (unsigned int)cpus;
}

// TODO: review, test this function
const char *getFilenameFromPath(const char *filepath) {
    if (filepath == NULL)
        return "";

    const char *unix_separator = strrchr(filepath, '/');
    // const char *windows_separator = strrchr(filepath, '\\');
    const char *separator = unix_separator;

    return separator == NULL ? filepath : separator + 1;
}

double getTime() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec * 1e-6;
}

bool hasExtension(const char *path, const char *ext) {
    const char *dot = strrchr(path, '.');

    if (dot == NULL)
        return false;

    return strcmp(dot + 1, ext) == 0;
}

bool isIgnoredExt(const char *filepath) {
    for (size_t i = 0; i < EXCLUDED_EXT_COUNT; i++) {
        if (hasExtension(filepath, CSTAT_DEFAULT_EXCLUDED_EXTENSIONS[i]))
            return true;
    }
    return false;
}

bool isIgnoredDir(const char *dirname) {
    for (size_t i = 0; i < EXCLUDED_DIR_COUNT; i++) {
        if (strcmp(dirname, CSTAT_DEFAULT_EXCLUDED_DIRS[i]) == 0)
            return true;
    }
    return false;
}

// TODO
void parseGitIgnore(GitIgnore *gitIgnore, int fd, const char *path) {
}

void push(DynamicArray *arr, const char *value) {
    if (arr->size >= arr->capacity) {
        arr->capacity = arr->capacity ? arr->capacity * 2 : 64;
        arr->items = realloc(arr->items, arr->capacity * sizeof(*arr->items));
    }
    arr->items[arr->size++] = strdup(value);
}

int find(DynamicArray *arr, const char *val) {
    for (int i = 0; i < arr->size; i++) {
        if (strcmp(arr->items[i], val) == 0)
            return i;
    }
    return -1;
}

void _cstat_log(const char *filename, const char *function, LogType type, const char *fmt, ...) {
    if (!CSTAT_DISPLAY_LOGS)
        return;

    va_list args;

    char *type_s;
    switch (type) {
    case CSTAT_LOG_ERROR:
        type_s = COLOR_RED "ERROR" COLOR_RESET;
        break;
    case CSTAT_LOG_WARNING:
        type_s = COLOR_YELLOW "WARNING" COLOR_RESET;
        break;
    case CSTAT_LOG_SUCCESS:
        type_s = COLOR_GREEN "SUCCESS" COLOR_RESET;
        break;
    case CSTAT_LOG_DEBUG:
        type_s = COLOR_BLUE "DEBUG" COLOR_RESET;
        break;
    }

    printf("[%s %s::%s()] ", type_s, filename, function);

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}
