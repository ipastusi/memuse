#include <assert.h>
#include "../src/alloc.h"

#define MAX_AU_SIZE 10

int main(void) {
    assert(mb_to_part_size(8000, 1, true) == 8000000000UL);
    assert(mb_to_part_size(8000, 1, false) == 8388608000UL);
    assert(mb_to_part_size(8000, 8, false) == 1048576000UL);
    assert(run("1:0", false, false, true, false, false, 1) == 0);
    assert(run("1:0|2:0", false, false, true, false, false, 1) == 0);
    assert(run("1:0", true, false, false, false, false, 1) == 0);
    assert(run("1:b", false, false, true, false, false, 1) == -1);
    assert(run("8:0", false, false, true, false, false, 8) == 0);
    assert(run("8:0", false, false, true, true, false, 8) == 0);
    assert(run("8:0", false, false, true, true, true, 8) == 0);
    assert(run("16000:0", false, false, true, false, false, 1) == -1);
    assert(run("16000:0", false, false, true, true, false, 1) == 0);
    assert(run("16000:0", false, false, true, true, true, 1) == 0);
}
