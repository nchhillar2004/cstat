#include "cli/cli.h"
#include "config.h"
#include "utils.h"
#include "walker/dir_walker.h"

static void runScan(Config *config) {
    if (walkDirectory(config->path, config))
        logSuccess("directory scanning completed for \"%s\"", config->path);
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
