/*
    cgo's C.malloc doesn't call C malloc directly, for the same behaviour calling malloc directly from C code is required.
    for more details, see: https://pkg.go.dev/cmd/cgo
    for this reason memory interactions have been implemented in C.
*/

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "memuse.h"

void *memuse_alloc(size_t size) {
    void *mem = malloc(size);
    if (mem != NULL) {
        memset(mem, 0, size);
    }
    return mem;
}

void memuse_lock(void *mem, size_t size) {
    if (mem != NULL) {
        mlock(mem, size);
    }
}

void memuse_unalloc(void *mem) {
    if (mem != NULL) {
        free(mem);
        mem = NULL;
    }
}

void memuse_unlock(void *mem, size_t size) {
    if (mem != NULL) {
        munlock(mem, size);
    }
}
