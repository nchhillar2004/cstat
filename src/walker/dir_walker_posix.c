#include "cstat.h"
#include "utils.h"
#include "walker/dir_walker.h"
#include <bits/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

bool _walk_dir_posix(const char *root, Config *config, WalkerStats *stats) {
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

        char path[CAP_SCAN_PATH_LEN];
        int n = snprintf(path, CAP_SCAN_PATH_LEN, "%s/%s", root, entry->d_name);
        if (n <= 0) {
            logError("Path too short");
            continue;
        }
        if (n > CAP_SCAN_PATH_LEN) {
            logError("Path too long");
            continue;
        }

        if (entry->d_type == DT_DIR) {
            _walk_dir_posix(path, config, stats);
            stats->dir += 1;
        } else if (entry->d_type == DT_REG) { // TODO: process file (detect language, count lines, etc...)
            logDebug("scanning file \"%s\"", path);
            stats->files += 1;
        }
        // all filesystems does not support DT_DIR so d_type might return DT_UNKNOWN
        else if (entry->d_type == DT_UNKNOWN){ // use fstatat() in that case
            struct stat stbuf;

            if (fstatat(dirfd(dir), entry->d_name, &stbuf, AT_SYMLINK_NOFOLLOW) == -1) {
                logError("fstatat failed for \"%s\": %s", path, strerror(errno));
                continue;
            }

            if (S_ISDIR(stbuf.st_mode)) {
                _walk_dir_posix(path, config, stats);
                stats->dir += 1;
            } else if (S_ISREG(stbuf.st_mode)) {
                logDebug("scanning file \"%s\"", path);
                stats->files += 1;
            }
        }
    }

    if (closedir(dir) == -1) {
        logError("Error closing directory \"%s\"", root);
    }

    return true;
}
