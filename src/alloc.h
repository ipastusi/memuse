#ifndef MEMUSE_ALLOC_H
#define MEMUSE_ALLOC_H

#include <stdbool.h>

int run(const char *cfg_str, bool is_mb, bool wrap);

void int_handler(int sig);

#endif
