#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

static void *memory;

static void allocate(size_t mb, int sec);

static void unallocate(void);

int main(void) {
    size_t mb = 1000;
    int sec = 10;
    allocate(mb, sec);
    unallocate();
}

static void allocate(size_t mb, int sec) {
    printf("allocating memory size: %zuMB\n", mb);
    size_t bytes = mb * 1024 * 1024;
    memory = malloc(bytes);
    mlock(memory, bytes);

    if (memory == NULL) {
        puts("memory allocation error");
        exit(1);
    }

    printf("sleeping: %ds\n", sec);
    sleep(sec);
}

static void unallocate(void) {
    puts("unallocating memory");
    free(memory);
    memory = NULL;
}
