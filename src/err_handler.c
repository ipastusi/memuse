#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

static int ies(unsigned int sec, unsigned int parts, unsigned int i);

static int ie(unsigned int sec, unsigned int parts, unsigned int i);

static int e(unsigned int sec, unsigned int parts, unsigned int i);

int (*select_eh(const bool ignore_err, const bool sleep_on_err))(unsigned int, unsigned int, unsigned int) {
    if (ignore_err && sleep_on_err) {
        return ies;
    } else if (ignore_err) {
        return ie;
    } else {
        return e;
    }
}

static int ies(const unsigned int sec, const unsigned int parts, const unsigned int i) {
    if (i == parts - 1) {
        printf("sleeping\n");
        sleep(sec);
        return 0;
    }
    return 0;
}

static int ie(const unsigned int sec, const unsigned int parts, const unsigned int i) {
    // keep compiler happy
    (void) sec;
    (void) parts;
    (void) i;
    return 0;
}

static int e(const unsigned int sec, const unsigned int parts, const unsigned int i) {
    // keep compiler happy
    (void) sec;
    (void) parts;
    (void) i;
    return -1;
}
