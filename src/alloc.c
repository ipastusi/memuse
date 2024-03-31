#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "alloc.h"
#include "config.h"

static alloc_unit *cfg;
static void *memory;
static bool allocated = false;
static bool allocated_cfg = false;

static int allocate(unsigned int size, unsigned int sec, bool is_mb);

static void unallocate(void);

static void cleanup(void);

int run(const char *const cfg_str, const bool is_mb, const bool wrap) {
    cfg = parse(cfg_str, wrap);
    if (cfg == NULL) return EXIT_FAILURE;
    allocated_cfg = true;
    const alloc_unit *current = cfg;

    atexit(cleanup);

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
    allocated_cfg = false;
    return (alloc_res != 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}

void int_handler(const int sig) {
    fprintf(stderr, "\nreceived signal: %d\n", sig);
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
    if (allocated != true) return;
    printf("unallocating memory\n");
    free(memory);
    memory = NULL;
    allocated = false;
}

static void cleanup(void) {
    unallocate();
    if (allocated_cfg) unallocate_cfg(cfg);
    printf("exiting...\n");
}