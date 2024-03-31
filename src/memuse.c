#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

static void help(void);

int main(int argc, char **argv) {
    const char *cfg_str = NULL;
    bool is_mb = false;
    bool wrap = false;

    int c;
    while ((c = getopt(argc, argv, "mwc:h")) != -1) {
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
    printf("\n");
    printf("-c      Memory allocation config\n");
    printf("-h      Print this help screen\n");
    printf("-m      Use MB instead of MiB\n");
    printf("-w      Wrap around and run in endless loop\n");
    printf("\n");
    printf("Examples:\n");
    printf("memuse -c '100:10|200:20'       Allocate 100MiB for 10 seconds, 200MiB for 20 seconds\n");
    printf("memuse -c '100:10|200:20' -w    Allocate 100MiB for 10 seconds, 200MiB for 20 seconds, repeat the sequence until interrupted\n");
    printf("memuse -c '100:10' -m           Allocate 100MB for 10 seconds\n");
}
