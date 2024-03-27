#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static alloc_unit *get_new_au(void);

alloc_unit *parse(char *cfg) {
    char in[MAX_CFG_SIZE];
    strncpy(in, cfg, MAX_CFG_SIZE);

    char delim[] = "|";
    alloc_unit *first_au = NULL;
    alloc_unit *last_au = NULL;

    char *token = strtok(in, delim);
    while (token != NULL) {
        alloc_unit *new_au = get_new_au();
        if (new_au == NULL) return NULL;

        int *a = malloc(sizeof(int));
        int *b = malloc(sizeof(int));
        int res = sscanf(token, "%d:%d", a, b);

        if (res != 2) {
            puts("incorrect format");
            return NULL;
        }

        if (*a < 0 || *b < 0) {
            printf("negative value detected: %d %d\n", *a, *b);
            return NULL;
        } else {
            new_au->mb = *a;
            new_au->sec = *b;
        }

        if (first_au == NULL) {
            first_au = new_au;
            last_au = first_au;
        } else {
            last_au->next = new_au;
            last_au = new_au;
        }

        token = strtok(NULL, delim);
    }
    if (first_au == NULL) puts("parsed config is empty");
    return first_au;
}

static alloc_unit *get_new_au(void) {
    alloc_unit *au = malloc(sizeof(alloc_unit));
    if (au == NULL) {
        puts("memory allocation error");
        return NULL;
    }
    au->next = NULL;
    return au;
}
