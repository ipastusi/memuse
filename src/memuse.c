#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "config.h"

static void *memory;
static bool allocated = false;

static void allocate(unsigned int mb, unsigned int sec);

static void unallocate(void);

void int_handler(int sig);

int main(void) {
    signal(SIGINT, int_handler);

    alloc_unit *cfg = parse("1000:30|2:1|3:1");
    if (cfg == NULL) exit(2);
    alloc_unit *current = cfg;

    while (true) {
        allocate(current->mb, current->sec);
        unallocate();

        if (current->next != NULL) {
            current = current->next;
        } else {
            break;
        }
    }
}

static void allocate(unsigned int mb, unsigned int sec) {
    printf("allocating memory size: %uMB for %us\n", mb, sec);
    unsigned int bytes = mb * 1024 * 1024;
    memory = malloc(bytes);
    allocated = true;

    if (memory == NULL) {
        puts("memory allocation error");
        exit(3);
    }

    mlock(memory, bytes);
    sleep(sec);
}

static void unallocate(void) {
    puts("unallocating memory");
    free(memory);
    memory = NULL;
    allocated = false;
}

void int_handler(int sig) {
    printf("\nreceived signal: %d\n", sig);
    if (allocated) unallocate();
    puts("exiting...");
    exit(1);
}
