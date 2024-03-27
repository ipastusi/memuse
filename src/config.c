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
        int res = sscanf(token, "%u:%u", &new_au->mb, &new_au->sec);

        if (res != 2) {
            puts("incorrect format");
            exit(1);
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
    return first_au;
}

static alloc_unit *get_new_au(void) {
    alloc_unit *au = malloc(sizeof(alloc_unit));
    if (au == NULL) {
        puts("memory allocation error");
        exit(1);
    }
    au->next = NULL;
    return au;
}
