#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/config.h"

#define MAX_AU_SIZE 10

static void is_equal(char *in, bool wrap);

static void is_equal_to(char *in, char *exp, bool wrap);

static void is_null(char *in);

static char *to_str(alloc_unit *au);

int main(void) {
    is_equal("1000:10", false);
    is_equal("1000:0", false);
    is_equal("1000:10|20:2|30:3", false);
    is_equal("11:10|12:10|13:10|14:10|15:10|16:10|17:10|18:10|19:10|20:10|21:10|22:10|23:10|24:10|25:10|26:10|27:1", false);
    is_equal("1000:10", true);
    is_equal("1000:0", true);
    is_equal("1000:10|20:2|30:3", true);
    is_equal("11:10|12:10|13:10|14:10|15:10|16:10|17:10|18:10|19:10|20:10|21:10|22:10|23:10|24:10|25:10|26:10|27:1", true);
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
    is_equal_to("1:2|", "1:2", false);
    is_equal_to("1:2||", "1:2", false);
    is_equal_to("1:2|", "1:2", true);
    is_equal_to("1:2||", "1:2", true);
}

static void is_equal(char *in, bool wrap) {
    alloc_unit *au = parse(in, wrap);
    char *out = to_str(au);
    unallocate_cfg(au);
    assert(strcmp(in, out) == 0);
}

static void is_equal_to(char *in, char *exp, bool wrap) {
    alloc_unit *au = parse(in, wrap);
    char *out = to_str(au);
    unallocate_cfg(au);
    assert(strcmp(exp, out) == 0);
}

static void is_null(char *in) {
    alloc_unit *au = parse(in, false);
    assert(au == NULL);
    unallocate_cfg(au);
}

static char *to_str(alloc_unit *au) {
    static char out[MAX_CFG_SIZE];
    snprintf(out, MAX_AU_SIZE, "%u:%u", au->size, au->sec);

    unsigned int units = au->units;
    for (unsigned int i = 1; i < units; i++) {
        au = au->next;
        strncat(out, "|", MAX_CFG_SIZE);

        char current[MAX_AU_SIZE];
        snprintf(current, MAX_AU_SIZE, "%u:%u", au->size, au->sec);
        strncat(out, current, MAX_CFG_SIZE - 1);
    }
    printf("result: %s\n", out);
    return out;
}
