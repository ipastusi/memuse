#ifndef MEMUSE_ALLOC_H
#define MEMUSE_ALLOC_H

#include <stdbool.h>
#include <stddef.h>

#define M_PARTS 8

int run(const char *cfg_str, bool is_mb, bool wrap, bool lock_mem,
        bool ignore_err, bool sleep_on_err, unsigned int parts);

size_t mb_to_part_size(unsigned int size, unsigned int parts, bool is_mb);

#endif
