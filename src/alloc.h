#ifndef MEMUSE_ALLOC_H
#define MEMUSE_ALLOC_H

#include <stdbool.h>
#include <stddef.h>

int run(const char *const cfg_str, bool is_mb, bool wrap, bool lock_mem,
        bool ignore_alloc_err, bool ignore_lock_err, bool sleep_on_err, unsigned int parts);

void int_handler(int sig);

size_t mb_to_part_size(unsigned int size, unsigned int parts, bool is_mb);

unsigned int get_parts(const char *parts);

#endif
