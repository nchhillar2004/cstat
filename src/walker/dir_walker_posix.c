#ifndef _WIN32
#include "cstat.h"
#include "utils.h"
#include "walker/dir_walker.h"
#include <errno.h>
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

    int fd = dirfd(dir);

    /*
    if (config->use_git_ignore) {
        int gitfd = openat(fd, GIT_IGNORE_FILE, O_RDONLY | O_CLOEXEC);
        if (gitfd >= 0) {
            parseGitignore(gitfd, root);
            close(gitfd);
        }
    }*/

    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[CAP_SCAN_PATH_LEN];
        int n = snprintf(path, CAP_SCAN_PATH_LEN, "%s/%s", root, entry->d_name);
        if (n <= 0) {
            logError("Path too short");
            continue;
        }
        if (n >= CAP_SCAN_PATH_LEN) {
            logError("Path too long");
            continue;
        }

        if (entry->d_type == DT_DIR) {
            if (config->use_cstat_ignore && isIgnoredDir(entry->d_name)) {
                logDebug("Ignored \"%s\"/", entry->d_name);
                continue;
            } else if (_walk_dir_posix(path, config, stats))
                stats->dir += 1;
        } else if (entry->d_type == DT_REG) {
            // TODO process file
            stats->files += 1;
        }
        // all filesystems does not support DT_DIR so d_type might return DT_UNKNOWN
        else if (entry->d_type == DT_UNKNOWN) { // use fstatat() in that case
            struct stat stbuf;

            if (fstatat(fd, entry->d_name, &stbuf, AT_SYMLINK_NOFOLLOW) == -1) {
                logError("fstatat failed for \"%s\": %s", path, strerror(errno));
                continue;
            }

            if (S_ISDIR(stbuf.st_mode)) {
                if (config->use_cstat_ignore && isIgnoredDir(entry->d_name)) {
                    logDebug("Ignored \"%s\"/", entry->d_name);
                    continue;
                } else if (_walk_dir_posix(path, config, stats))
                    stats->dir += 1;
            } else if (S_ISREG(stbuf.st_mode)) {
                // TODO: process file
                stats->files += 1;
            }
        }
    }

    if (closedir(dir) == -1) {
        logError("Error closing directory \"%s\"", root);
    }

    return true;
}
#endif
