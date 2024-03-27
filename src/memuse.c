#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "config.h"

static void *memory;

static void allocate(unsigned int mb, unsigned int sec);

static void unallocate(void);

int main(void) {
    alloc_unit *cfg = parse("1:1|2:1|3:1");
    if (cfg == NULL) exit(1);
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
    mlock(memory, bytes);

    if (memory == NULL) {
        puts("memory allocation error");
        exit(1);
    }
    sleep(sec);
}

static void unallocate(void) {
    puts("unallocating memory");
    free(memory);
    memory = NULL;
}
