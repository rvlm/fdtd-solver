#include <CUnit/CUnit.h>
#include <stddef.h>
#include "rvlm/fdtd/common/common_helpers.h"

extern void test_common_helpers_copy_string(void) {
    const int N = 128;
    const char *src = "Deadbeef";
    char dst[N];
    char *res;
    int i;

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
