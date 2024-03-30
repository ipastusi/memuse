#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "config.h"

static alloc_unit *cfg;
static void *memory;
static bool allocated = false;

static int allocate(unsigned int size, unsigned int sec, bool is_mb);

static void unallocate(void);

static void int_handler(int sig);

int main(void) {
    signal(SIGINT, int_handler);

    const bool is_mb = 0;
    const bool wrap = false;
    cfg = parse("1:1|2:1|3:1", wrap);
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

static int allocate(const unsigned int size, const unsigned int sec, const bool is_mb) {
    const unsigned int bytes = is_mb ? size * 1000 * 1000 : size * 1024 * 1024;
    const char *const unit = is_mb ? "MB" : "MiB";
    printf("allocating memory size: %u%s for %us\n", size, unit, sec);
    memory = malloc(bytes);

    if (memory == NULL) {
        printf("memory allocation error\n");
        return EXIT_FAILURE;
    }

    allocated = true;
    if (mlock(memory, bytes) == -1) {
        printf("memory locking error\n");
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

static void int_handler(const int sig) {
    printf("\nreceived signal: %d\n", sig);
    if (allocated) unallocate();
    unallocate_cfg(cfg);
    printf("exiting...\n");
    exit(EXIT_FAILURE);
}
