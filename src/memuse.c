#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

static void help(void);

int main(int argc, char **argv) {
    setbuf(stdout, NULL);

    const char *cfg_str = NULL;
    bool is_mb = false;
    bool wrap = false;
    bool lock_mem = true;
    bool ignore_alloc_err = false;
    bool ignore_lock_err = false;
    bool sleep_on_err = true;
    unsigned int parts = 1;

    int c;
    while ((c = getopt(argc, argv, "ac:dhlmp:sw")) != -1) {
        switch (c) {
            case 'a':
                ignore_alloc_err = true;
                break;
            case 'c':
                cfg_str = optarg;
                break;
            case 'd':
                lock_mem = false;
                break;
            case 'h':
                help();
                exit(EXIT_SUCCESS);
            case 'l':
                ignore_lock_err = true;
                break;
            case 'm':
                is_mb = true;
                break;
            case 'p':
                parts = get_parts(optarg);
                break;
            case 's':
                sleep_on_err = false;
                break;
            case 'w':
                wrap = true;
                break;
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
    return run(cfg_str, is_mb, wrap, lock_mem, ignore_alloc_err, ignore_lock_err, sleep_on_err, parts);
}

static void help(void) {
    printf("\n");
    printf("-a      Ignore allocation errors\n");
    printf("-c      Memory allocation config\n");
    printf("-d      Don't lock memory after allocation, might get paged to the swap area\n");
    printf("-h      Print this help screen\n");
    printf("-l      Ignore locking errors\n");
    printf("-m      Use MB instead of MiB\n");
    printf("-p      Allocate specified memory size divided into parts\n");
    printf("-s      Don't sleep on error, ignored unless used together with -a or -l\n");
    printf("-w      Wrap around and run in endless loop\n");
    printf("\n");
    printf("Examples:\n");
    printf("memuse -c '100:10|200:20'       Allocate 100MiB for 10 seconds, 200MiB for 20 seconds\n");
    printf("memuse -c '100:10|200:20' -w    Allocate 100MiB for 10 seconds, 200MiB for 20 seconds, repeat\n");
    printf("memuse -c '500:10' -p 4         Allocate 500MiB split into 4 parts for 10 seconds\n");
    printf("memuse -c '100:10' -m           Allocate 100MB for 10 seconds\n");
}
