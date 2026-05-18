#include "config.h"
#include "cstat.h"

void initConfig(Config *config) {
    config->path = DEFAULT_SCAN_PATH;
    config->max_file_size = DEFAULT_MAX_FILE_SIZE;
    config->min_file_size = DEFAULT_MIN_FILE_SIZE;
    config->worker_threads = DEFAULT_WORKER_THREADS;
    config->csv_output = DEFAULT_CSV_OUTPUT;
    config->use_ignore = DEFAULT_USE_IGNORE;
    config->use_cstat_ignore = DEFAULT_USE_CSTAT_IGNORE;
    config->use_git_ignore = DEFAULT_USE_GIT_IGNORE;
    config->display_logs = DEFAULT_DISPLAY_LOGS;
}
