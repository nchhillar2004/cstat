#include "utils.h"
#include "cstat.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

bool CSTAT_DISPLAY_LOGS = DEFAULT_DISPLAY_LOGS;

unsigned int getAvailableThreads() {
    // number of processors currently online (available)
    long cpus = 0;

// fetch for windows system
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    cpus = sysinfo.dwNumberOfProcessors;
#else // unix systems
    cpus = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    // if error cpus will be equal to -1
    // in that case use DEFAULT_WORKER_THREADS
    if (cpus <= 0)
        return DEFAULT_WORKER_THREADS;

    // TODO: should i use this ?
    if (cpus > 64)
        return 64; //

    return (unsigned int)cpus;
}

// TODO: review, test this function on both unix and windows systems
const char *getFilenameFromPath(const char *filepath) {
    if (filepath == NULL)
        return "";

    const char *unix_separator = strrchr(filepath, '/');
    const char *windows_separator = strrchr(filepath, '\\');
    const char *separator = unix_separator;

    if (windows_separator != NULL && (separator == NULL || windows_separator > separator))
        separator = windows_separator;

    return separator == NULL ? filepath : separator + 1;
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
