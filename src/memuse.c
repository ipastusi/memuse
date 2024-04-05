#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

static void sig_handler(int sig);

static unsigned int get_parts(const char *parts);

static void help(void);

int main(int argc, char **argv) {
    setbuf(stdout, NULL);

    const char *cfg_str = NULL;
    bool is_mb = false;
    bool wrap = false;
    bool lock_mem = true;
    bool ignore_err = false;
    bool sleep_on_err = true;
    unsigned int parts = 1;

    int c;
    while ((c = getopt(argc, argv, "c:dehmp:sw")) != -1) {
        switch (c) {
            case 'c':
                cfg_str = optarg;
                break;
            case 'd':
                lock_mem = false;
                break;
            case 'e':
                ignore_err = true;
                break;
            case 'h':
                help();
                exit(EXIT_SUCCESS);
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

    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    int res = run(cfg_str, is_mb, wrap, lock_mem, ignore_err, sleep_on_err, parts);
    return (res == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void sig_handler(const int sig) {
    fprintf(stderr, "\nreceived signal: %d\n", sig);
    exit(EXIT_SUCCESS);
}

static unsigned int get_parts(const char *const parts) {
    char *endprt;
    unsigned long number = strtoul(parts, &endprt, 10);
    if (number < 1 || number > M_PARTS) {
        printf("-p option accepts only integer values between 1 and 8 inclusive\n");
        exit(EXIT_FAILURE);
    }
    return (unsigned int) number;
}

static void help(void) {
    printf("\n");
    printf("-c      Memory allocation config\n");
    printf("-d      Don't lock memory after allocation, might get paged to the swap area\n");
    printf("-e      Ignore allocation and locking errors, if possible\n");
    printf("-h      Print this help screen\n");
    printf("-m      Use MB instead of MiB\n");
    printf("-p      Allocate specified memory size divided into parts\n");
    printf("-s      Don't sleep on errors, ignored unless used together with -i\n");
    printf("-w      Wrap around and run in endless loop\n");
    printf("\n");
    printf("Examples:\n");
    printf("memuse -c '100:10|200:20'       Allocate 100MiB for 10 seconds, 200MiB for 20 seconds\n");
    printf("memuse -c '100:10|200:20' -w    Allocate 100MiB for 10 seconds, 200MiB for 20 seconds, repeat\n");
    printf("memuse -c '500:10' -p 4         Allocate 500MiB split into 4 parts for 10 seconds\n");
    printf("memuse -c '100:10' -m           Allocate 100MB for 10 seconds\n");
}
