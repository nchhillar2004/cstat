#ifndef cstat_cli_h
#define cstat_cli_h

#include "config.h"

typedef enum {
    CMD_SCAN,
    CMD_HELP,
    CMD_VERSION,
    CMD_LANGUAGES
} CliAction;


/* ===== FUNCTIONS ===== */

// parse cli arguments, handle config changes and validation
CliAction parseCliArgs(int argc, char *argv[], Config *config);

void cliPrintHelp();
void cliPrintVersion();
void cliPrintLanguages();


/* internal/private functions */

// parse and validate the value of an argument flag, 
// like the --threads flag need a value to change the worker_threads in config
unsigned int _parse_arg_value(const char *value, const char *arg);

#endif
