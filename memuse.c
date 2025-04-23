/*
    "C.malloc does not call the C library malloc directly but instead calls a Go helper function that wraps the C library malloc
    but guarantees never to return nil. If C's malloc indicates out of memory, the helper function crashes the program"
    for more details, see: https://pkg.go.dev/cmd/cgo
    this is not the behaviour we want. for this reason memory interactions have been implemented directly in C
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
