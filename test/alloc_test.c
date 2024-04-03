#include <assert.h>
#include "../src/alloc.h"

#define MAX_AU_SIZE 10

static void run_alloc(char *cfg_str, bool is_mb, bool wrap, bool lock_mem, bool ignore_alloc_err, bool ignore_lock_err, bool sleep_on_err, int parts,
                      int exp_res);

int main(void) {
    assert(mb_to_part_size(8000, 1, true) == 8000000000UL);
    assert(mb_to_part_size(8000, 1, false) == 8388608000UL);
    assert(mb_to_part_size(8000, 8, false) == 1048576000UL);
    run_alloc("1:0", false, false, true, false, false, false, 1, 0);
    run_alloc("1:0", true, false, false, false, false, false, 1, 0);
    run_alloc("1:b", false, false, true, false, false, false, 1, 1);
}

static void run_alloc(char *cfg_str, bool is_mb, bool wrap, bool lock_mem, bool ignore_alloc_err, bool ignore_lock_err, bool sleep_on_err, int parts,
                      int exp_res) {
    int res = run(cfg_str, is_mb, wrap, lock_mem, ignore_alloc_err, ignore_lock_err, sleep_on_err, parts);
    assert(res == exp_res);
}
