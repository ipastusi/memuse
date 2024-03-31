#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "alloc.h"
#include "config.h"

static alloc_unit *cfg;
static void *memory;
static bool allocated = false;

static int allocate(unsigned int size, unsigned int sec, bool is_mb);

static void unallocate(void);

int run(const char *const cfg_str, const bool is_mb, const bool wrap) {
    cfg = parse(cfg_str, wrap);
    if (cfg == NULL) exit(EXIT_FAILURE);
    const alloc_unit *current = cfg;

    int alloc_res;
    while (true) {
        alloc_res = allocate(current->size, current->sec, is_mb);
        unallocate();
        if (alloc_res != 0) break;

        if (current->next != NULL) {
            current = current->next;
        } else {
            break;
        }
    }
    unallocate_cfg(cfg);
    return (alloc_res != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

void int_handler(const int sig) {
    fprintf(stderr, "\nreceived signal: %d\n", sig);
    if (allocated) unallocate();
    unallocate_cfg(cfg);
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
}

static int allocate(const unsigned int size, const unsigned int sec, const bool is_mb) {
    const unsigned int bytes = is_mb ? size * 1000 * 1000 : size * 1024 * 1024;
    const char *const unit = is_mb ? "MB" : "MiB";
    printf("allocating memory size: %u%s for %us\n", size, unit, sec);
    memory = malloc(bytes);

    if (memory == NULL) {
        fprintf(stderr, "memory allocation error\n");
        return EXIT_FAILURE;
    }

    allocated = true;
    if (mlock(memory, bytes) == -1) {
        fprintf(stderr, "memory locking error\n");
        return EXIT_FAILURE;
    }
    sleep(sec);
    return EXIT_SUCCESS;
}

static void unallocate(void) {
    printf("unallocating memory\n");
    free(memory);
    memory = NULL;
    allocated = false;
}
