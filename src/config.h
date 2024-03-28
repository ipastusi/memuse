#ifndef MEMUSE_CONFIG_H
#define MEMUSE_CONFIG_H

#define MAX_CFG_SIZE 101

typedef struct alloc_unit {
    unsigned int mb;
    unsigned int sec;
    struct alloc_unit *next;
} alloc_unit;

alloc_unit *parse(char *cfg);

void unallocate_cfg(alloc_unit *cfg);

#endif
