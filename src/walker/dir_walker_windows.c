#ifdef _WIN32
#include "cstat.h"
#include "utils.h"
#include "walker/dir_walker.h"
#include <stdio.h>
#include <string.h>

bool _walk_dir_windows(const char *root, Config *config, WalkerStats *stats) {
    char buffer[CAP_SCAN_PATH_LEN];

    int n = snprintf(buffer, CAP_SCAN_PATH_LEN, "%s\\*", root);
    if (n < 0) {
        logError("Path too short");
        return false;
    }
    if (n >= CAP_SCAN_PATH_LEN) {
        logError("Path too long");
        return false;
    }

    WIN32_FIND_DATAA entry;

    HANDLE hFind = FindFirstFileA(buffer, &entry);

    if (hFind == INVALID_HANDLE_VALUE) {
        logError("Could not open directory \"%s\"", root);
        return false;
    }

    do {
        if (strcmp(entry.cFileName, ".") == 0 || strcmp(entry.cFileName, "..") == 0)
            continue;

        char path[CAP_SCAN_PATH_LEN];
        n = snprintf(path, CAP_SCAN_PATH_LEN, "%s\\%s", root, entry.cFileName);
        if (n < 0) {
            logError("Path too short");
            continue;
        }
        if (n >= CAP_SCAN_PATH_LEN) {
            logError("Path too long");
            continue;
        }

        if (entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if(_walk_dir_windows(path, config, stats))
                stats->dir += 1;
        } else {
            logDebug("scanning file \"%s\"", path);
            stats->files += 1;
        }
    } while (FindNextFileA(hFind, &entry));

    FindClose(hFind);

    return true;
}

#endif
