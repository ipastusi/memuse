#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include "alloc.h"
#include "config.h"

#define M_PARTS 8

static alloc_unit *cfg;
static void *memory[M_PARTS];
static unsigned int allocated = 0;
static bool allocated_cfg = false;

static int allocate(unsigned int size, unsigned int sec, bool is_mb, bool lock_mem,
                    bool ignore_alloc_err, bool ignore_lock_err, bool sleep_on_err, unsigned int parts);

static void unallocate(void);

static void cleanup(void);

int run(const char *const cfg_str, const bool is_mb, const bool wrap, const bool lock_mem,
        const bool ignore_alloc_err, const bool ignore_lock_err, const bool sleep_on_err, const unsigned int parts) {
    cfg = parse(cfg_str);
    if (cfg == NULL) return EXIT_FAILURE;
    allocated_cfg = true;
    const alloc_unit *current = cfg;

    atexit(cleanup);

    int alloc_res;
    while (true) {
        alloc_res = allocate(current->size, current->sec, is_mb, lock_mem, ignore_alloc_err, ignore_lock_err, sleep_on_err, parts);
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

size_t mb_to_part_size(const unsigned int size, const unsigned int parts, const bool is_mb) {
    return is_mb ? (size * 1000UL * 1000UL) / (unsigned long) parts : (size * 1024UL * 1024UL) / (unsigned long) parts;
}

unsigned int get_parts(const char *const parts) {
    char *endprt;
    unsigned long number = strtoul(parts, &endprt, 10);
    if (number < 1 || number > M_PARTS) {
        printf("-p option accepts only integer values between 1 and 8 inclusive\n");
        exit(EXIT_FAILURE);
    }
    return (unsigned int) number;
}

static int allocate(const unsigned int size, const unsigned int sec, const bool is_mb, const bool lock_mem,
                    const bool ignore_alloc_err, const bool ignore_lock_err, const bool sleep_on_err, const unsigned int parts) {
    const size_t bytes = mb_to_part_size(size, parts, is_mb);
    const char *const unit = is_mb ? "MB" : "MiB";
    printf("allocating memory size: %u%s (%ux %zu bytes) for %us\n", size, unit, parts, bytes, sec);

    for (unsigned int i = 0; i < parts; i++) {
        memory[i] = malloc(bytes);

        if (memory[i] == NULL) {
            fprintf(stderr, "memory allocation error (errno %d)\n", errno);

            if (ignore_alloc_err && sleep_on_err && i == parts - 1) {
                printf("sleeping\n");
                sleep(sec);
                return EXIT_SUCCESS;
            } else if (ignore_alloc_err) {
                return EXIT_SUCCESS;
            }
            return EXIT_FAILURE;
        } else {
            allocated++;
            if (lock_mem) {
                if (i == 0) printf("locking memory\n");
                if (mlock(memory[i], bytes) != 0) {
                    fprintf(stderr, "memory locking error (errno %d)\n", errno);

                    struct rlimit r;
                    if (getrlimit(RLIMIT_MEMLOCK, &r) != 0) {
                        fprintf(stderr, "unable to determine memory locking limits\n");
                    } else {
                        fprintf(stderr, "memory requested: %zu. memory locking limits: %"PRIu64" (soft), %"PRIu64" (hard)\n", bytes, r.rlim_cur, r.rlim_max);
                    }

                    if (ignore_lock_err && sleep_on_err && i == parts - 1) {
                        printf("sleeping\n");
                        sleep(sec);
                        return EXIT_SUCCESS;
                    } else if (ignore_lock_err) {
                        return EXIT_SUCCESS;
                    }
                    return EXIT_FAILURE;
                }
            }
            if (i == parts - 1) {
                printf("sleeping\n");
                sleep(sec);
            }
        }
    }
    return EXIT_SUCCESS;
}

static void unallocate(void) {
    for (unsigned int i = 0; i < allocated; i++) {
        if (i == 0) printf("unallocating memory\n");
        free(memory[i]);
        memory[i] = NULL;
    }
    allocated = 0;
}

static void cleanup(void) {
    unallocate();
    if (allocated_cfg) unallocate_cfg(cfg);
    printf("exiting...\n");
}
