#include "cli/cli.h"
#include "config.h"
#include "utils.h"
#include "walker/dir_walker.h"
#include <stdio.h>

static void runScan(Config *config) {
    double startTime = getTime();

    WalkerStats stats = {0};
    if (!walkDirectory(config->path, config, &stats)) {
        logError("scan aborted");
        return;
    }    
    logSuccess("directory scanning completed for \"%s\"", config->path);

    printf("Found %lu directories and %lu files at \"%s\"\n", stats.dir, stats.files, config->path);
    double endTime = getTime();
    printf("Scan took %.4lfs\n", (double)(endTime - startTime));
}

int main(int argc, char *argv[]) {
    Config cstat_config;
    initConfig(&cstat_config);

    CliAction action = parseCliArgs(argc, argv, &cstat_config);

    switch (action) {
        case CMD_SCAN:
            runScan(&cstat_config);
            break;
        case CMD_HELP:
            cliPrintHelp();
            break;
        case CMD_VERSION:
            cliPrintVersion();
            break;
        case CMD_LANGUAGES:
            cliPrintLanguages();
            break;
    }

    return 0;
}
