#include <CUnit/CUnit.h>
#include <stddef.h>
#include "rvlm/fdtd/common/string_format.h"

extern void test_string_format_copy_string(void) {
    const size_t N = 128;
    const char *src = "Deadbeef";
    char dst[N];
    char *res;
    size_t i;

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[0]);
    CU_ASSERT_EQUAL(res, NULL);
    for (i = 0; i < N; ++i)
        CU_ASSERT_EQUAL(dst[i], '\xFF');

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[1]);
    CU_ASSERT_EQUAL(res, &dst[0]);
    CU_ASSERT_EQUAL(dst[0], '\0');
    for (i = 1; i < N; ++i)
        CU_ASSERT_EQUAL(dst[i], '\xFF');

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[2]);
    CU_ASSERT_EQUAL(res, &dst[1]);
    CU_ASSERT_EQUAL(dst[0], 'D');
    CU_ASSERT_EQUAL(dst[1], '\0');
    for (i = 2; i < N; ++i)
        CU_ASSERT_EQUAL(dst[i], '\xFF');

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[4]);
    CU_ASSERT_EQUAL(res, &dst[3]);
    CU_ASSERT_EQUAL(dst[0], 'D');
    CU_ASSERT_EQUAL(dst[1], 'e');
    CU_ASSERT_EQUAL(dst[2], 'a');
    CU_ASSERT_EQUAL(dst[3], '\0');
    for (i = 4; i < N; ++i)
        CU_ASSERT_EQUAL(dst[i], '\xFF');

    memset(&dst[0], '\xFF', N);
    res = rfdtd_copy_string(src, &dst[0], &dst[8]);
    CU_ASSERT_EQUAL(res, &dst[7]);
    CU_ASSERT_EQUAL(dst[0], 'D');
    CU_ASSERT_EQUAL(dst[1], 'e');
    CU_ASSERT_EQUAL(dst[2], 'a');
    CU_ASSERT_EQUAL(dst[3], 'd');
    CU_ASSERT_EQUAL(dst[4], 'b');
    CU_ASSERT_EQUAL(dst[5], 'e');
    CU_ASSERT_EQUAL(dst[6], 'e');
    CU_ASSERT_EQUAL(dst[7], '\0');
    for (i = 8; i < N; ++i)
        CU_ASSERT_EQUAL(dst[i], '\xFF');
}

extern void test_string_format_substitute_placeholders(void) {
    const char *args[] = { "sA=Ein", "sB=Zwei", "sC=Drei" };
    const int N = 128;
    char buf[N];
    char *res;
    int i;

    memset(&buf[0], '\xFF', N);
    res = rfdtd_substitute_placeholders(&buf[0], &buf[4], "abc", args, 3);
    CU_ASSERT_EQUAL(res, &buf[3]);
    CU_ASSERT_EQUAL(buf[0], 'a');
    CU_ASSERT_EQUAL(buf[1], 'b');
    CU_ASSERT_EQUAL(buf[2], 'c');
    CU_ASSERT_EQUAL(buf[3], '\0');

    memset(&buf[0], '\xFF', N);
    res = rfdtd_substitute_placeholders(&buf[0], &buf[0],
                                        "{sA} {sB} {sC}", args, 3);
    CU_ASSERT_EQUAL(res, NULL);
    for (i = 0; i < N; ++i)
        CU_ASSERT_EQUAL(buf[i], '\xFF');

    memset(&buf[0], '\xFF', N);
    res = rfdtd_substitute_placeholders(&buf[0], &buf[1],
                                        "{sA} {sB} {sC}", args, 3);
    CU_ASSERT_EQUAL(res, &buf[0]);
    CU_ASSERT_EQUAL(buf[0], '\0');
    for (i = 1; i < N; ++i)
        CU_ASSERT_EQUAL(buf[i], '\xFF');

    memset(&buf[0], '\xFF', N);
    res = rfdtd_substitute_placeholders(&buf[0], &buf[2],
                                        "{sA} {sB} {sC}", args, 3);
    CU_ASSERT_EQUAL(res, &buf[1]);
    CU_ASSERT_EQUAL(buf[0], 'E');
    CU_ASSERT_EQUAL(buf[1], '\0');
    for (i = 2; i < N; ++i)
        CU_ASSERT_EQUAL(buf[i], '\xFF');
}
