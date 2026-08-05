#include "cli/cli.h"
#include "config.h"
#include "utils.h"
#include "walker/dir_walker.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

// 1K
#define K1 1000.0
// 100K
#define K100 (K1 * 100)
// 1M
#define M1 (K100 * 10)
// 1B
#define B1 (M1 * 1000)

static char *_num_format(uint64_t num) {
    char *str = malloc(8);
    if (num < K100) {
        snprintf(str, sizeof(str), "%lu", num);
        return str;
    }

    char suffix = '\0';

    double tmp = 0.0;

    if (num >= K100 && num < M1) {
        tmp = num / K1;
        suffix = 'K';
    } else if (num >= M1 && num < B1) {
        tmp = num / M1;
        suffix = 'M';
    } else if (num >= B1) {
        tmp = num / B1;
        suffix = 'B';
    }

    snprintf(str, sizeof(str), "%.2f%c", tmp, suffix);

    return str;
}

static void runScan(Config *config) {
    double startTime = getTime();

    WalkerStats stats = {0};
    if (!walkDirectory(config->path, config, &stats)) {
        logError("scan aborted");
        return;
    }
    logSuccess("directory scanning completed for \"%s\"", config->path);

    printf("found %s directories and %s files at \"%s\".\n", _num_format(stats.dir + stats.dirIgnored),
           _num_format(stats.files + stats.filesIgnored), config->path);
    printf("ignored %" PRIu64 " directories and %" PRIu64 " files.\n\n", stats.dirIgnored, stats.filesIgnored);

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
