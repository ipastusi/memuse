#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int main(void) {
    puts("start");

    void *memory;
    size_t size = 1000 * 1024 * 1024;
    memory = malloc(size);
    mlock(memory, size);

    if (memory == NULL) {
        puts("Memory allocation error");
        exit(1);
    }

    sleep(10);

    free(memory);
    memory = NULL;

    puts("end");
}
