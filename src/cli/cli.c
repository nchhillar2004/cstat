#include "cli/cli.h"
#include "cstat.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>

CliAction parseCliArgs(int argc, char *argv[], Config *config) {
    // iterate throught all arguments and parse them, make changes to config if config flags are present
    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        /* cstat commands */
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
            return CMD_HELP;
        else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0)
            return CMD_VERSION;
        else if (strcmp(arg, "-l") == 0 || strcmp(arg, "--languages") == 0)
            return CMD_LANGUAGES;

        /* flags */
        // flags which require a value
        else if ((strcmp(arg, "-max") == 0 || strcmp(arg, "--max-file-size") == 0) && i + 1 < argc)
            config->max_file_size = _parse_arg_value(argv[++i], arg);
        else if ((strcmp(arg, "-min") == 0 || strcmp(arg, "--min-file-size") == 0) && i + 1 < argc)
            config->min_file_size = _parse_arg_value(argv[++i], arg);
        else if ((strcmp(arg, "-t") == 0 || strcmp(arg, "--threads") == 0) && i + 1 < argc)
            config->worker_threads = _parse_arg_value(argv[++i], arg);

        // toggle flags
        else if (strcmp(arg, "-ni") == 0 || strcmp(arg, "--no-ignore") == 0)
            config->use_ignore = false;
        else if (strcmp(arg, "-ng") == 0 || strcmp(arg, "--no-gitignore") == 0)
            config->use_git_ignore = false;
        else if (strcmp(arg, "-nc") == 0 || strcmp(arg, "--no-cstatignore") == 0)
            config->use_cstat_ignore = false;
        else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--csv") == 0)
            config->csv_output = true;
        else if (strcmp(arg, "-d") == 0 || strcmp(arg, "--debug") == 0)
            config->display_logs = CSTAT_DISPLAY_LOGS = true;
        
        // else everything is path
        // TODO: verify if path is correct, file or directory exists
        // only get valid path, else print 'error: invalid command or flag'
        else
            config->path = arg;
    }

    /* configuration validation and checks */

    // max file size cannot be greated than capped max file size
    if (config->max_file_size > CAP_FILE_SIZE) {
        printf(COLOR_RED "error: " COLOR_RESET "value cannot exceed the maximum cap of %dMB\n", CAP_FILE_SIZE);
        exit(1);
    }

    // min file size cannot be greated than max file size
    if (config->min_file_size > config->max_file_size) {
        printf(COLOR_RED "error: " COLOR_RESET "min file size cannot exceed the max file size limit\n");
        exit(1);
    }

    // if user selects --no-ignore then disable use of both .gitignore and .cstatignore
    if (config->use_ignore == false) {
        config->use_git_ignore = false;
        config->use_cstat_ignore = false;
    }

    unsigned int available_threads = getAvailableThreads();
    // check if user changed threads to less than 1 or more than available threads on the system
    if (config->worker_threads <= 0 || config->worker_threads > available_threads)
        config->worker_threads = available_threads;

    logDebug("using %d threads", config->worker_threads);
    logDebug("scanning \"%s\"...", config->path);

    return CMD_SCAN;
}

void cliPrintHelp() {
    printf("%s: help\n", TARGET_NAME);
}

void cliPrintVersion() {
    printf("%s v%s\n", TARGET_NAME, CSTAT_VERSION);
}

void cliPrintLanguages() {
    printf("%s: supported languages\n", TARGET_NAME);
}

unsigned int _parse_arg_value(const char *value, const char *arg) {
    char *end;

    // convert string value to interger
    long num = strtol(value, &end, 10);

    // validate if its digit or not
    if (*end != '\0') {
        printf(COLOR_RED "error: " COLOR_RESET "invalid value for flag \"%s\"\n", arg);
        exit(1);
    }
    // check if the digit is negative or not
    if (num < 0) {
        printf(COLOR_RED "error: " COLOR_RESET "value cannot be negative for flag \"%s\"\n", arg);
        exit(1);
    }

    return (unsigned int)num;
}
