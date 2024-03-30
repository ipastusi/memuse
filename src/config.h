#include <stdbool.h>

#ifndef MEMUSE_CONFIG_H
#define MEMUSE_CONFIG_H

#define MAX_CFG_SIZE 101

typedef struct alloc_unit {
    unsigned int size;
    unsigned int sec;
    unsigned int units;
    struct alloc_unit *next;
} alloc_unit;

alloc_unit *parse(const char *const cfg, bool wrap);

void unallocate_cfg(alloc_unit *cfg);

#endif
