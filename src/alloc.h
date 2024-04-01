#ifndef MEMUSE_ALLOC_H
#define MEMUSE_ALLOC_H

#include <stdbool.h>
#include <stddef.h>

int run(const char *const cfg_str, bool is_mb, bool wrap, bool lock_mem, bool ignore_alloc_err, bool ignore_lock_err, bool sleep_on_err);

void int_handler(int sig);

size_t mb_to_b(unsigned int size, bool is_mb);

#endif
