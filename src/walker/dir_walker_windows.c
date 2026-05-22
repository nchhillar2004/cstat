#ifdef _WIN32
#include <wchar.h>
#include "cstat.h"
#include "utils.h"
#include "walker/dir_walker.h"
#include <stdio.h>
#include <string.h>

bool _walk_dir_windows(const char *root, Config *config, WalkerStats *stats) {
    wchar_t buffer[CAP_SCAN_PATH_LEN];

    int n = swprintf(buffer, CAP_SCAN_PATH_LEN, L"%S\\*", root);
    if (n < 0) {
        logError("swprintf failed");
        return false;
    }
    if (n >= CAP_SCAN_PATH_LEN) {
        logError("Path too long");
        return false;
    }

    WIN32_FIND_DATAW entry;

    HANDLE hFind =
        FindFirstFileExW(buffer, FindExInfoBasic, &entry, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);

    if (hFind == INVALID_HANDLE_VALUE) {
        logError("Could not open directory \"%s\"", root);
        return false;
    }

    do {
        if (wcscmp(entry.cFileName, L".") == 0 || wcscmp(entry.cFileName, L"..") == 0)
            continue;

        char path[CAP_SCAN_PATH_LEN];
        n = snprintf(path, CAP_SCAN_PATH_LEN, "%s\\%ls", root, entry.cFileName);
        if (n < 0) {
            logError("snprintf failed");
            continue;
        }
        if (n >= CAP_SCAN_PATH_LEN) {
            logError("Path too long");
            continue;
        }

        if (entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (_walk_dir_windows(path, config, stats))
                stats->dir += 1;
        } else {
            logDebug("scanning file \"%s\"", path);
            stats->files += 1;
        }
    } while (FindNextFileW(hFind, &entry));

    FindClose(hFind);

    return true;
}

#endif
