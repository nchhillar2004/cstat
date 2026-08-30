/*
  dir_walker.h
  recursive directory walker
*/

#ifndef cstat_dir_walker_h
#define cstat_dir_walker_h

#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include <bits/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
    uint64_t dir;
    uint64_t dirIgnored;
    uint64_t files;
    uint64_t filesIgnored;
    uint64_t pathTooLong;
} WalkerStats;

/* ===== MACROS ===== */

/* ===== FUNCTIONS ===== */
bool walkDirectory(const char *path, Config *config, WalkerStats *stats);

#endif
