#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

void *memory;

void allocate(size_t mb);

void unallocate(void);

void sleeps(int s);

int main(void) {
    size_t mb = 1000;
    allocate(mb);
    sleeps(10);
    unallocate();
}

void allocate(size_t mb) {
    printf("allocating memory size: %zuMB\n", mb);
    size_t bytes = mb * 1024 * 1024;
    memory = malloc(bytes);
    mlock(memory, bytes);

    if (memory == NULL) {
        puts("memory allocation error");
        exit(1);
    }
}

void unallocate(void) {
    puts("unallocating memory");
    free(memory);
    memory = NULL;
}

void sleeps(int s) {
    printf("sleeping: %ds\n", s);
    sleep(s);
}
