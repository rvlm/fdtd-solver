#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include "rvlm/fdtd/common/error_handling.h"
#include <rvlm/fdtd/common/internal_macros.h>

static int pseudo_sqrt(int x, struct rfdtd_error_stack *e) {
    int result;
    
    if (x >= 0)
        result = x;
    else {
        result = 0;
        rfdtd_push_error(e, __FILE__, __LINE__, "<NONE>",
            RFDTD_UNKNOWN_ERROR,
            "Root of negative value x={iArg}",
            "iArg", x,
            NULL);
    }

    return result;
}

static int pseudo_sqrt_2(int x, struct rfdtd_error_stack *e) {
    RAISE_ASSERT_M(x >= 0,
        RFDTD_UNKNOWN_ERROR,
        "Root of negative value x={iArg}",
        "iArg", x,
        NULL);

    return x;

e_raise:
    return 0;
}

extern void test_error_handling_stack_indexing(void **state) {
    (void)state;

    struct rfdtd_error_stack stack;
    struct rfdtd_error_stack *e = &stack;
    struct rfdtd_error_entry *entry;

    int i;

    // The test cannot be run on smaller values.
    assert_true(RFDTD_ERROR_STACK_CAPACITY >= 4);

    rfdtd_initialize_stack(e);
    assert_int_equal(e->count, 0);
    assert_int_equal(e->tip,   0);

    rfdtd_push_error(e, NULL, 0, NULL, -1, "ERR-1");
    assert_int_equal(e->count, 1);
    assert_int_equal(e->tip,   1);

    rfdtd_push_error(e, NULL, 0, NULL, -2, "ERR-2");
    assert_int_equal(e->count, 2);
    assert_int_equal(e->tip,   2);

    for (i=0; i < RFDTD_ERROR_STACK_CAPACITY; ++i) {
        rfdtd_push_error(e, NULL, 0, NULL, i, "ERR-i");
    }

    assert_int_equal(e->count, RFDTD_ERROR_STACK_CAPACITY);
    assert_int_equal(e->tip,   3);

    for (i = 0; i < e->count; ++i) {
        entry = rfdtd_get_stack_entry(e, i);
    }

    for (i = 0; i < e->count-1; ++i) {
        entry = rfdtd_get_stack_entry(e, i);
        assert_non_null(entry);
        assert_non_null(entry->fmt);
        assert_string_equal(entry->fmt, "ERR-i");
        assert_int_equal((int)entry->code, RFDTD_ERROR_STACK_CAPACITY - i - 1);
    }

    entry = rfdtd_get_stack_entry(e, e->count - 1);
    assert_string_equal(entry->fmt, "ERR-1");
    assert_int_equal((int)entry->code, -1);

    // Try invalid stack indexes, too.
    assert_null(rfdtd_get_stack_entry(e, -1));
    assert_null(rfdtd_get_stack_entry(e, e->count));
}

extern void test_error_handling_stack_works(void **state) {
    (void)state;

    struct rfdtd_error_stack e;
    struct rfdtd_error_entry *entry;
    rfdtd_initialize_stack(&e);
    
    assert_int_equal(pseudo_sqrt( 1, &e), 1);
    assert_int_equal(e.tip,   0);
    assert_int_equal(e.count, 0);

    assert_int_equal(pseudo_sqrt(-1, &e), 0);
    assert_int_equal(e.tip,   1);
    assert_int_equal(e.count, 1);

    entry = rfdtd_get_stack_entry(&e, 0);
    assert_string_equal(entry->file, __FILE__);
    assert_string_equal(entry->expr, "<NONE>");
    assert_string_equal(entry->fmt,  "Root of negative value x={iArg}");
    assert_string_equal(entry->msg,  "Root of negative value x=-1");
}

extern void test_error_handling_stack_works_2(void **state) {
    (void)state;

    struct rfdtd_error_stack e;
    struct rfdtd_error_entry *entry;
    rfdtd_initialize_stack(&e);

    assert_int_equal(pseudo_sqrt_2( 1, &e), 1);
    assert_int_equal(e.tip,   0);
    assert_int_equal(e.count, 0);

    assert_int_equal(pseudo_sqrt_2(-1, &e), 0);
    assert_int_equal(e.tip,   1);
    assert_int_equal(e.count, 1);

    entry = rfdtd_get_stack_entry(&e, 0);
    assert_string_equal(entry->file, __FILE__);
    assert_string_equal(entry->expr, "x >= 0");
    assert_string_equal(entry->fmt,  "Root of negative value x={iArg}");
    assert_string_equal(entry->msg,  "Root of negative value x=-1");
}

