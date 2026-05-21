#include "walker/dir_walker.h"

bool walkDirectory(const char *path, Config *config) {
#ifdef _WIN32
    return _walk_dir_windows(path, config);
#else
    return _walk_dir_posix(path, config);
#endif
}
