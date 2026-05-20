#ifndef cstat_config_h
#define cstat_config_h

#include <stdbool.h>

typedef struct {
    char* path;

    unsigned int max_file_size;
    unsigned int min_file_size;

    unsigned int worker_threads;

    bool csv_output;

    bool use_ignore;
    bool use_cstat_ignore;
    bool use_git_ignore;

    bool display_logs;
} Config;


/* ===== FUNCTIONS ===== */

// initialize default configs
void initConfig(Config *config);

#endif
