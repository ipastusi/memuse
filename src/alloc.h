#ifndef MEMUSE_ALLOC_H
#define MEMUSE_ALLOC_H

#include <stdbool.h>
#include <stddef.h>

int run(const char *cfg_str, bool is_mb, bool wrap);

void int_handler(int sig);

size_t mb_to_b(unsigned int size, bool is_mb);

#endif
