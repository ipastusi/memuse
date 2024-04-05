#include <assert.h>
#include "../src/alloc.h"

#define MAX_AU_SIZE 10

static void run_alloc(char *cfg_str, bool is_mb, bool wrap, bool lock_mem, bool ignore_err, bool sleep_on_err, int parts, int exp_res);

int main(void) {
    assert(mb_to_part_size(8000, 1, true) == 8000000000UL);
    assert(mb_to_part_size(8000, 1, false) == 8388608000UL);
    assert(mb_to_part_size(8000, 8, false) == 1048576000UL);
    run_alloc("1:0", false, false, true, false, false, 1, 0);
    run_alloc("1:0|2:0", false, false, true, false, false, 1, 0);
    run_alloc("1:0", true, false, false, false, false, 1, 0);
    run_alloc("1:b", false, false, true, false, false, 1, -1);
    run_alloc("20000:0", false, false, true, false, false, 1, -1);
    run_alloc("20000:0", false, false, true, true, false, 1, 0);
    run_alloc("20000:0", false, false, true, true, true, 1, 0);
    run_alloc("8:0", false, false, true, false, false, 8, 0);
    run_alloc("8:0", false, false, true, true, false, 8, 0);
    run_alloc("8:0", false, false, true, true, true, 8, 0);
}

static void run_alloc(char *cfg_str, bool is_mb, bool wrap, bool lock_mem, bool ignore_err, bool sleep_on_err, int parts, int exp_res) {
    int res = run(cfg_str, is_mb, wrap, lock_mem, ignore_err, sleep_on_err, parts);
    assert(res == exp_res);
}
