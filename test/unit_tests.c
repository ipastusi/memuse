#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/config.h"

#define MAX_AU_SIZE 10

static void is_equal(char *in);

static void is_equal_to(char *in, char *exp);

static void is_null(char *in);

static char *to_str(alloc_unit *au);

int main(void) {
    is_equal("1000:10");
    is_equal("1000:10|20:2|30:3");
    is_equal("10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:1");
    is_null("10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10|10:10");
    is_null("-1:1");
    is_null("");
    is_null(":");
    is_null("1:");
    is_null(":1");
    is_null("1:a");
    is_null("1:-");
    is_null("1");
    is_null("a1:2");
    is_null("1:2a");
    is_null("1:2:");
    is_null("1:2::");
    is_equal_to("1:2|", "1:2");
    is_equal_to("1:2||", "1:2");
}

static void is_equal(char *in) {
    alloc_unit *au = parse(in);
    char *out = to_str(au);
    unallocate_cfg(au);
    assert(strcmp(in, out) == 0);
}

static void is_equal_to(char *in, char *exp) {
    alloc_unit *au = parse(in);
    char *out = to_str(au);
    unallocate_cfg(au);
    assert(strcmp(exp, out) == 0);
}

static void is_null(char *in) {
    alloc_unit *au = parse(in);
    assert(au == NULL);
    unallocate_cfg(au);
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
    printf("result: %s\n", out);
    return out;
}
