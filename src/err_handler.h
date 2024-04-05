#ifndef MEMUSE_ERR_HANDLER_H
#define MEMUSE_ERR_HANDLER_H

#include <stdbool.h>

int (*select_eh(bool ignore_err, bool sleep_on_err))(unsigned int, unsigned int, unsigned int);

#endif
