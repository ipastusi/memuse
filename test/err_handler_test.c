#include <assert.h>
#include "../src/err_handler.h"

int main(void) {
    int (*err_handler)(unsigned int, unsigned int, unsigned int);

    err_handler = select_eh(false, false);
    assert(err_handler(0, 0, 0) == -1);

    err_handler = select_eh(true, false);
    assert(err_handler(0, 0, 0) == 0);

    err_handler = select_eh(true, true);
    assert(err_handler(0, 4, 1) == 0);
    assert(err_handler(0, 4, 3) == 0);

    err_handler = select_eh(false, true);
    assert(err_handler(0, 0, 0) == -1);
}
