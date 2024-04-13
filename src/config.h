#ifndef MEMUSE_CONFIG_H
#define MEMUSE_CONFIG_H

#include <stdbool.h>

#define MAX_CFG_SIZE 101

typedef struct alloc_unit {
    unsigned int size;
    unsigned int sec;
    struct alloc_unit *next;
} alloc_unit;

alloc_unit *parse(const char *cfg_str);

void unallocate_cfg(alloc_unit *cfg);

#endif
