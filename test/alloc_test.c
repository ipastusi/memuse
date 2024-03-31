#include <assert.h>
#include "../src/alloc.h"

#define MAX_AU_SIZE 10

static void run_alloc(char *cfg_str, bool is_mb, bool wrap, int exp_res);

int main(void) {
    run_alloc("1:0", false, false, 0);
    run_alloc("1:0", true, false, 0);
    run_alloc("1:b", false, false, 1);
}

static void run_alloc(char *cfg_str, bool is_mb, bool wrap, int exp_res) {
    int res = run(cfg_str, is_mb, wrap);
    assert(res == exp_res);
}
