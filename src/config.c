#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static alloc_unit *get_new_au(void);

alloc_unit *parse(const char *const cfg_str) {
    if (strlen(cfg_str) + 1 > MAX_CFG_SIZE) {
        fprintf(stderr, "input too long (%lu): %s\n", strlen(cfg_str), cfg_str);
        return NULL;
    }

    char in[MAX_CFG_SIZE];
    strncpy(in, cfg_str, MAX_CFG_SIZE);

    const char delim[] = "|";
    alloc_unit *first_au = NULL;
    alloc_unit *last_au = NULL;

    const char *token = strtok(in, delim);
    while (token != NULL) {
        alloc_unit *const new_au = get_new_au();
        if (new_au == NULL) return NULL;

        int *const a = &(int) {0};
        int *const b = &(int) {0};
        int res = sscanf(token, "%d:%d", a, b);

        if (res != 2) {
            fprintf(stderr, "incorrect format: %s\n", token);
            return NULL;
        }

        if (*a < 0 || *b < 0) {
            fprintf(stderr, "negative value: %d %d\n", *a, *b);
            return NULL;
        } else {
            new_au->size = *a;
            new_au->sec = *b;
        }

        char *au_str;
        res = asprintf(&au_str, "%u:%u", *a, *b);
        if (res == -1 || strcmp(token, au_str) != 0) {
            fprintf(stderr, "incorrect format: %s\n", token);
            return NULL;
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

    if (first_au == NULL) {
        fprintf(stderr, "parsed config is empty\n");
    }
    return first_au;
}

void unallocate_cfg(alloc_unit *cfg) {
    alloc_unit *current = cfg;
    while (current != NULL) {
        alloc_unit *prev = current;
        current = current->next;
        free(prev);
        prev = NULL;
    }
}

static alloc_unit *get_new_au(void) {
    alloc_unit *au = malloc(sizeof(alloc_unit));
    if (au == NULL) {
        fprintf(stderr, "memory allocation error (errno %d)\n", errno);
        return NULL;
    }
    au->next = NULL;
    return au;
}
