#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/config.h"

#define MAX_AU_SIZE 10

static char *to_str(alloc_unit *au);

int main(void) {
    char in[] = "1000:10|20:2|30:3";
    alloc_unit *au = parse(in);
    char *out = to_str(au);
    assert(strcmp(in, out) == 0);
}

static char *to_str(alloc_unit *au) {
    static char out[MAX_CFG_SIZE];
    snprintf(out, MAX_AU_SIZE, "%u:%u", au->mb, au->sec);

    while (au->next != NULL) {
        au = au->next;
        strlcat(out, "|", MAX_CFG_SIZE);

        char current[MAX_AU_SIZE];
        snprintf(current, MAX_AU_SIZE, "%u:%u", au->mb, au->sec);
        strlcat(out, current, MAX_CFG_SIZE);
    }
    printf("result: %s", out);
    return out;
}
