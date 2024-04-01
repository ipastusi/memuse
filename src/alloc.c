#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>
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
    cfg = parse(cfg_str);
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
        } else if (wrap) {
            current = cfg;
        } else {
            break;
        }
    }

    unallocate_cfg(cfg);
    allocated_cfg = false;
    return alloc_res;
}

void int_handler(const int sig) {
    fprintf(stderr, "\nreceived signal: %d\n", sig);
    exit(EXIT_FAILURE);
}

size_t mb_to_b(const unsigned int size, const bool is_mb) {
    return is_mb ? size * 1000UL * 1000UL : size * 1024UL * 1024UL;
}

static int allocate(const unsigned int size, const unsigned int sec, const bool is_mb) {
    const size_t bytes = mb_to_b(size, is_mb);
    const char *const unit = is_mb ? "MB" : "MiB";
    printf("allocating memory size: %u%s (%zu) for %us\n", size, unit, bytes, sec);
    memory = malloc(bytes);

    if (memory == NULL) {
        fprintf(stderr, "memory allocation error (errno %d)\n", errno);
        return EXIT_FAILURE;
    }

    allocated = true;
    if (mlock(memory, bytes) != 0) {
        fprintf(stderr, "memory locking error (errno %d)\n", errno);

        struct rlimit r;
        if (getrlimit(RLIMIT_MEMLOCK, &r) != 0) {
            fprintf(stderr, "unable to determine memory locking limits\n");
        } else {
            fprintf(stderr, "memory requested: %zu. memory locking limits: %"PRIu64" (soft), %"PRIu64" (hard)\n", bytes, r.rlim_cur, r.rlim_max);
        }

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
