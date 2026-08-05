#include "walker/dir_walker.h"

bool walkDirectory(const char *path, Config *config, WalkerStats *stats) {
#ifdef _WIN32
    return _walk_dir_windows(path, config, stats);
#else
    return _walk_dir_posix(path, config, stats);
#endif
}
