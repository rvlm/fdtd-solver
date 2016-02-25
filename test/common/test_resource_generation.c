#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "rvlm/fdtd/test_config.h"

int
rfdtd_test_resources_count(void);

char const**
rfdtd_test_resources_list(void);

size_t
rfdtd_test_resources_file_size(const char* resname);

const char*
rfdtd_test_resources_file_data(const char* resname);

extern void test_resource_generation_random5k(void **state) {
    (void)state;

    // Test file name and exact size in bytes. This file contains 5KB of high
    // quality random data.
    const char* fname = RFDTD_PROJECT_SOURCE_DIR "/test/_data/random5k.dat";
    const size_t N = 5*1024;

    FILE* fh = NULL;
    char buf[N];

    // Fail fast if file cannot be opened. No further cleanup required.
    fh = fopen(fname, "rb");
    assert_non_null(fh);

    size_t nrd;
    int eof;
    int err;

    nrd = fread(buf, 1, N, fh);
    eof = fgetc(fh);
    err = ferror(fh);
    fclose(fh);

    // Check everything at once AFTER file is closed, in order not to leak
    // file descriptior in case assertion fails early.
    assert_true(nrd == N);
    assert_true(eof == EOF);
    assert_false(err);

    assert_int_equal(rfdtd_test_resources_count(), 1);
    assert_string_equal(rfdtd_test_resources_list()[0], "random5k.dat");
    assert_int_equal(rfdtd_test_resources_file_size("random5k.dat"), N);
    assert_string_equal(rfdtd_test_resources_file_data("random5k.dat"), buf);
}
