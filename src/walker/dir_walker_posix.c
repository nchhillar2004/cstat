#include "cstat.h"
#include "utils.h"
#include "walker/dir_walker.h"
#include <bits/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool _walk_dir_posix(const char *root, Config *config) {
    DIR *dir = NULL;
    dir = opendir(root);
    if (dir == NULL) {
        logError("Could not open directory \"%s\": %s", root, strerror(errno));
        return false;
    }

    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // TODO: check for .ignore excluded directories

        // TODO: is this CAP valid ?!
        char path[CAP_SCAN_PATH_LEN];
        int n = snprintf(path, CAP_SCAN_PATH_LEN, "%s/%s", root, entry->d_name);
        if (n < 0 || n > CAP_SCAN_PATH_LEN) {
            logError("Path too long");
            continue;
        }

        // TODO: DT_DIR is not a part of C standard, replace 
        if (entry->d_type == DT_DIR)
            _walk_dir_posix(path, config);
        else // TODO: process file (detect language, count lines, etc...)
            logDebug("scanning file \"%s\"", path);
    }

    if (closedir(dir) == -1) {
        logError("Error closing directory \"%s\"", root);
    }

    return true;
}
