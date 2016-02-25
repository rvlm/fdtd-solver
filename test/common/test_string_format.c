#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "rvlm/fdtd/common/string_format.h"

extern void test_string_format_copy_string(void **state) {
    (void)state;

    const size_t N = 128;
    const char *src = "Deadbeef";
    char dst[N];
    char *res;
    size_t i;

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[0]);
    assert_ptr_equal(res, NULL);
    for (i = 0; i < N; ++i)
        assert_int_equal(dst[i], '\xFF');

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[1]);
    assert_int_equal(res, &dst[0]);
    assert_int_equal(dst[0], '\0');
    for (i = 1; i < N; ++i)
        assert_int_equal(dst[i], '\xFF');

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[2]);
    assert_int_equal(res, &dst[1]);
    assert_int_equal(dst[0], 'D');
    assert_int_equal(dst[1], '\0');
    for (i = 2; i < N; ++i)
        assert_int_equal(dst[i], '\xFF');

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[4]);
    assert_int_equal(res, &dst[3]);
    assert_int_equal(dst[0], 'D');
    assert_int_equal(dst[1], 'e');
    assert_int_equal(dst[2], 'a');
    assert_int_equal(dst[3], '\0');
    for (i = 4; i < N; ++i)
        assert_int_equal(dst[i], '\xFF');

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[8]);
    assert_int_equal(res, &dst[7]);
    assert_int_equal(dst[0], 'D');
    assert_int_equal(dst[1], 'e');
    assert_int_equal(dst[2], 'a');
    assert_int_equal(dst[3], 'd');
    assert_int_equal(dst[4], 'b');
    assert_int_equal(dst[5], 'e');
    assert_int_equal(dst[6], 'e');
    assert_int_equal(dst[7], '\0');
    for (i = 8; i < N; ++i)
        assert_int_equal(dst[i], '\xFF');
}

extern void test_string_format_substitute_placeholders(void **state) {
    (void)state;

    const char *args[] = { "sA=Ein", "sB=Zwei", "sC=Drei" };
    const int N = 128;
    char buf[N];
    char *res;
    int i;

    memset(&buf[0], '\xFF', N);
    res = rfdtd_substitute_placeholders(&buf[0], &buf[4], "abc", args, 3);
    assert_int_equal(res, &buf[3]);
    assert_int_equal(buf[0], 'a');
    assert_int_equal(buf[1], 'b');
    assert_int_equal(buf[2], 'c');
    assert_int_equal(buf[3], '\0');

    memset(&buf[0], '\xFF', N);
    res = rfdtd_substitute_placeholders(&buf[0], &buf[0],
                                        "{sA} {sB} {sC}", args, 3);
    assert_int_equal(res, NULL);
    for (i = 0; i < N; ++i)
        assert_int_equal(buf[i], '\xFF');

    memset(&buf[0], '\xFF', N);
    res = rfdtd_substitute_placeholders(&buf[0], &buf[1],
                                        "{sA} {sB} {sC}", args, 3);
    assert_int_equal(res, &buf[0]);
    assert_int_equal(buf[0], '\0');
    for (i = 1; i < N; ++i)
        assert_int_equal(buf[i], '\xFF');

    memset(&buf[0], '\xFF', N);
    res = rfdtd_substitute_placeholders(&buf[0], &buf[2],
                                        "{sA} {sB} {sC}", args, 3);
    assert_int_equal(res, &buf[1]);
    assert_int_equal(buf[0], 'E');
    assert_int_equal(buf[1], '\0');
    for (i = 2; i < N; ++i)
        assert_int_equal(buf[i], '\xFF');
}
