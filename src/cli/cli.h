#ifndef cstat_cli_h
#define cstat_cli_h

#include "config.h"

typedef enum {
    CMD_SCAN,
    CMD_HELP,
    CMD_VERSION,
    CMD_LANGUAGES
} CliAction;

CliAction parseCliArgs(int argc, char *argv[], Config *config);

void cliPrintHelp();
void cliPrintVersion();
void cliPrintLanguages();

#endif
