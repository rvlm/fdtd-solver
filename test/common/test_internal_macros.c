#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include "rvlm/fdtd/common/error_handling.h"
#include "rvlm/fdtd/common/internal_macros.h"

extern void test_internal_macros_new_and_dispose(void **state) {
    (void)state;

    struct stupid_struct {
        double a;
        float b;
        char c[10];
    };

    struct rfdtd_error_stack stack;
    struct rfdtd_error_stack *e = &stack;
    rfdtd_initialize_stack(e);

    float *fs = NULL;
    int   *vs = NULL;
    char  *cs = NULL;
    struct stupid_struct *ss = NULL;

    NEW_ARRAY(fs, 10);
    NEW_ARRAY(vs, 10);
    NEW_ARRAY(cs, 10);
    NEW_ARRAY(ss, 10);

    assert_non_null(fs);
    assert_non_null(vs);
    assert_non_null(cs);
    assert_non_null(ss);

e_memory:
    DISPOSE(ss);
    DISPOSE(cs);
    DISPOSE(vs);
    DISPOSE(fs);

    assert_null(fs);
    assert_null(vs);
    assert_null(cs);
    assert_null(ss);
}
