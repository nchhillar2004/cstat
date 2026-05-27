/*
  dir_walker.h
  recursive directory walker
*/

#ifndef cstat_dir_walker_h
#define cstat_dir_walker_h

#include "config.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <bits/types.h>
    #include <dirent.h>
#endif

typedef struct {
    uint64_t dir;
    uint64_t files;
} WalkerStats;
  
    
/* ===== MACROS ===== */


/* ===== FUNCTIONS ===== */
bool walkDirectory(const char *path, Config *config, WalkerStats *stats);

bool _walk_dir_posix(const char *path, Config *config, WalkerStats *stats);
bool _walk_dir_windows(const char *path, Config *config, WalkerStats *stats);

#endif
