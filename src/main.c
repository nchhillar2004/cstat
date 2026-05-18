#include "config.h"
#include "cstat.h"
#include "utils.h"

int main(void) {
    Config cstat_config;
    initConfig(&cstat_config);

    logDebug("%s", TARGET_NAME);
    logError("Test Error");
    logWarn("Test Warning");
    logSuccess("Config init");
    logDebug("Scanning: \"%s\"", cstat_config.path);

    return 0;
}
