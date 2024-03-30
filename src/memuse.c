#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "config.h"

#define VERSION "1.0.0"

static alloc_unit *cfg;
static void *memory;
static bool allocated = false;

static void help(void);

static void version(void);

static int run(const char *cfg_str, bool is_mb, bool wrap);

static int allocate(unsigned int size, unsigned int sec, bool is_mb);

static void unallocate(void);

static void int_handler(int sig);

int main(int argc, char **argv) {
    const char *cfg_str = NULL;
    bool is_mb = false;
    bool wrap = false;

    int c;
    while ((c = getopt(argc, argv, "mwc:vh")) != -1) {
        switch (c) {
            case 'm':
                is_mb = true;
                break;
            case 'w':
                wrap = true;
                break;
            case 'c':
                cfg_str = optarg;
                break;
            case 'v':
                version();
                exit(EXIT_SUCCESS);
            case 'h':
                help();
                exit(EXIT_SUCCESS);
            default:
                help();
                exit(EXIT_FAILURE);
        }
    }
    if (cfg_str == NULL) {
        help();
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, int_handler);
    run(cfg_str, is_mb, wrap);
}

static void help(void) {
    version();
    printf("\n");
    printf("-c      Memory allocation config\n");
    printf("-h      Print this help screen\n");
    printf("-m      Use MB instead of MiB\n");
    printf("-v      Print version info\n");
    printf("-w      Wrap around and run in endless loop\n");
    printf("\n");
    printf("Examples:\n");
    printf("memuse -c '100:10|200:20'       Allocate 100MiB for 10 seconds, 200MiB for 20 seconds\n");
    printf("memuse -c '100:10|200:20' -w    Allocate 100MiB for 10 seconds, 200MiB for 20 seconds, repeat the sequence until interrupted\n");
    printf("memuse -c '100:10' -m           Allocate 100MB for 10 seconds\n");
}

static void version(void) {
    printf("memuse %s\n", VERSION);
}

static int run(const char *const cfg_str, const bool is_mb, const bool wrap) {
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

static void int_handler(const int sig) {
    fprintf(stderr, "\nreceived signal: %d\n", sig);
    if (allocated) unallocate();
    unallocate_cfg(cfg);
    fprintf(stderr, "exiting...\n");
    exit(EXIT_FAILURE);
}
