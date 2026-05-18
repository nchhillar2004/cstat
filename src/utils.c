#include "utils.h"
#include "cstat.h"
#include <stdarg.h>
#include <stdio.h>

void _cstat_log(const char *time, const char *filename, const char *function, LogType type, const char *fmt, ...) {
    va_list args;

    char *type_s;
    switch (type) {
        case ERROR:
            type_s = COLOR_RED "ERROR" COLOR_RESET;
            break;
        case WARN:
            type_s = COLOR_YELLOW "WARNING" COLOR_RESET;
            break;
        case SUCCESS:
            type_s = COLOR_GREEN "SUCCESS" COLOR_RESET;
            break;
        case DEBUG:
            type_s = COLOR_BLUE "DEBUG" COLOR_RESET;
            break;
    }

    printf("[%s %s %s::%s()] ", time, type_s, filename, function);

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}
