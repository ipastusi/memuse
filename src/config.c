#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static alloc_unit *get_new_au(void);

alloc_unit *parse(char *cfg) {
    if (strlen(cfg) + 1 > MAX_CFG_SIZE) {
        printf("input too long (%lu): %s\n", strlen(cfg), cfg);
        return NULL;
    }

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
            printf("incorrect format: %s\n", token);
            return NULL;
        }

        if (*a < 0 || *b < 0) {
            printf("negative value: %d %d\n", *a, *b);
            return NULL;
        } else {
            new_au->mb = *a;
            new_au->sec = *b;
        }

        char *au_str;
        res = asprintf(&au_str, "%u:%u", *a, *b);
        if (res == -1 || strcmp(token, au_str) != 0) {
            printf("incorrect format: %s\n", token);
            return NULL;
        }

        free(a);
        free(b);
        a = NULL;
        b = NULL;

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
