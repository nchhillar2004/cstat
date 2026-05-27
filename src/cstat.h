/*
  cstat.h
  This file contains cstat global constants, default configurations, definitions.
*/

#ifndef cstat_h
#define cstat_h

/* global constants */
#include <stddef.h>
#define TARGET_NAME "cstat"
#define CSTAT_VERSION "1.0"

#define CSTAT_CONFIG_FILE "cstat.conf"
#define GIT_IGNORE_FILE ".gitignore"

#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

/* max value cap */
// maximum languages for a project/codebase to scan
#define CAP_LANGUAGES 30   // cstat will only show upto 30 languages per scan
#define CAP_FILE_SIZE 1024 // user cannot configure to scan files above this size (in MB)
#ifdef _WIN32
    #define CAP_SCAN_PATH_LEN (16 * 1024) // max dir scan path size allowed
#else
    #define CAP_SCAN_PATH_LEN (4 * 1024) // max dir scan path size allowed
#endif

/* defaults */
#define DEFAULT_SCAN_PATH "."

// TODO: add more patterns to exclude
static const char *CSTAT_DEFAULT_EXCLUDED_DIRS[] = {
    ".git",    "node_modules", "vendor", "packages",   "build",  "dist",    ".cache",  ".out",        "out",
    ".vscode", ".idea",        "target", "bin",        "obj",    "Debug",   "Release", "__pycache__", ".pytest_cache",
    ".next",   ".tmp",         "temp",   ".terraform", ".venv",  "venv",    ".gradle", ".cargo",      ".maven",
    ".sv",     ".DS_Store",    "logs",   "Logs",       ".local", ".config",
};
static const char *CSTAT_DEFAULT_EXCLUDED_EXTENSIONS[] = {
    "bin", "img", "iso", "db",    "dat", "exe",  "dll",    "so",      "o",         "out",
    "a",   "obj", "lib", "class", "pyc", "lock", "min.js", "min.css", "bundle.js", "etl",
};

static const size_t EXCLUDED_DIR_COUNT = sizeof(CSTAT_DEFAULT_EXCLUDED_DIRS) / sizeof(CSTAT_DEFAULT_EXCLUDED_DIRS[0]);
static const size_t EXCLUDED_EXT_COUNT =
    sizeof(CSTAT_DEFAULT_EXCLUDED_EXTENSIONS) / sizeof(CSTAT_DEFAULT_EXCLUDED_EXTENSIONS[0]);

// max/min file size to scan (in MB)
#define DEFAULT_MAX_FILE_SIZE 10
#define DEFAULT_MIN_FILE_SIZE 0

#define DEFAULT_WORKER_THREADS 8

#define DEFAULT_CSV_OUTPUT false

#define DEFAULT_USE_IGNORE true
#define DEFAULT_USE_CSTAT_IGNORE true
#define DEFAULT_USE_GIT_IGNORE true

#define DEFAULT_DISPLAY_LOGS false

#endif
