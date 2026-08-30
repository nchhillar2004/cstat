#include "walker/dir_walker.h"
#include "cstat.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

GitIgnore gitIgnore_s = {0};

unsigned char _get_dtype(int fd, struct dirent *e) {
    unsigned char dtype = e->d_type;

    if (dtype != DT_UNKNOWN)
        return dtype;

    struct stat st;

    if (fstatat(fd, e->d_name, &st, AT_SYMLINK_NOFOLLOW) == 0) {
        if (S_ISREG(st.st_mode))
            dtype = DT_REG;
        else if (S_ISDIR(st.st_mode))
            dtype = DT_DIR;
    }

    return dtype;
}

bool walkDirectory(const char *root, Config *config, WalkerStats *stats) {
    DIR *dir = NULL;
    dir = opendir(root);
    if (dir == NULL) {
        logError("Could not open directory \"%s\": %s", root, strerror(errno));
        return false;
    }

    int fd = dirfd(dir);

    if (config->use_git_ignore) {
        int gitfd = openat(fd, GIT_IGNORE_FILE, O_RDONLY | O_CLOEXEC);
        if (gitfd >= 0) {
            parseGitIgnore(&gitIgnore_s, gitfd, root);
            close(gitfd);
        }
    }

    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[CAP_SCAN_PATH_LEN];
        const char *_root = strcmp(root, "/") == 0 ? "" : root;
        int n = snprintf(path, CAP_SCAN_PATH_LEN, "%s/%s", _root, entry->d_name);
        if (n <= 0) {
            logError("Path too short");
            logDebug("snprintf failed");
            continue;
        }
        if (n >= CAP_SCAN_PATH_LEN) {
            logError("Path too long");
            stats->pathTooLong += 1;
            continue;
        }

        unsigned char dtype = _get_dtype(fd, entry);

        switch (dtype) {
        case DT_REG:
            // TODO: filter files based on size
            if (isIgnoredExt(entry->d_name)) {
                stats->filesIgnored += 1;
                continue;
            }
            // TODO process file
            stats->files += 1;
            break;
        case DT_DIR:
            if (config->use_cstat_ignore && isIgnoredDir(entry->d_name)) {
                stats->dirIgnored += 1;
                continue;
            } else if (walkDirectory(path, config, stats))
                stats->dir += 1;
            break;
        }
    }

    if (closedir(dir) == -1) {
        logError("Error closing directory \"%s\"", root);
    }

    return true;
}
