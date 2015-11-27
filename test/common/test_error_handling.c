#include <CUnit/CUnit.h>
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

extern void test_error_handling_stack_indexing(void) {
    struct rfdtd_error_stack stack;
    struct rfdtd_error_stack *e = &stack;
    struct rfdtd_error_entry *entry;

    int i;

    // The test cannot be run on smaller values.
    CU_ASSERT_TRUE_FATAL(RFDTD_ERROR_STACK_CAPACITY >= 4);

    rfdtd_initialize_stack(e);
    CU_ASSERT_EQUAL(e->count, 0);
    CU_ASSERT_EQUAL(e->tip,   0);

    rfdtd_push_error(e, NULL, 0, NULL, -1, "ERR-1");
    CU_ASSERT_EQUAL(e->count, 1);
    CU_ASSERT_EQUAL(e->tip,   1);

    rfdtd_push_error(e, NULL, 0, NULL, -2, "ERR-2");
    CU_ASSERT_EQUAL(e->count, 2);
    CU_ASSERT_EQUAL(e->tip,   2);

    for (i=0; i < RFDTD_ERROR_STACK_CAPACITY; ++i) {
        rfdtd_push_error(e, NULL, 0, NULL, i, "ERR-i");
    }

    CU_ASSERT_EQUAL(e->count, RFDTD_ERROR_STACK_CAPACITY);
    CU_ASSERT_EQUAL(e->tip,   3);

    for (i = 0; i < e->count; ++i) {
        entry = rfdtd_get_stack_entry(e, i);
    }

    for (i = 0; i < e->count-1; ++i) {
        entry = rfdtd_get_stack_entry(e, i);
        CU_ASSERT_PTR_NOT_NULL_FATAL(entry);
        CU_ASSERT_PTR_NOT_NULL_FATAL(entry->fmt);
        CU_ASSERT_STRING_EQUAL(entry->fmt, "ERR-i");
        CU_ASSERT_EQUAL(entry->code, RFDTD_ERROR_STACK_CAPACITY - i - 1);
    }

    entry = rfdtd_get_stack_entry(e, e->count - 1);
    CU_ASSERT_STRING_EQUAL(entry->fmt, "ERR-1");
    CU_ASSERT_EQUAL(entry->code, -1);

    // Try invalid stack indexes, too.
    CU_ASSERT_PTR_NULL(rfdtd_get_stack_entry(e, -1));
    CU_ASSERT_PTR_NULL(rfdtd_get_stack_entry(e, e->count));
}

extern void test_error_handling_stack_works(void) {
    struct rfdtd_error_stack e;
    struct rfdtd_error_entry *entry;
    rfdtd_initialize_stack(&e);
    
    CU_ASSERT_EQUAL(pseudo_sqrt( 1, &e), 1);
    CU_ASSERT_EQUAL(e.tip,   0);
    CU_ASSERT_EQUAL(e.count, 0);

    CU_ASSERT_EQUAL(pseudo_sqrt(-1, &e), 0);
    CU_ASSERT_EQUAL(e.tip,   1);
    CU_ASSERT_EQUAL(e.count, 1);

    entry = rfdtd_get_stack_entry(&e, 0);
    CU_ASSERT_STRING_EQUAL(entry->file, __FILE__);
    CU_ASSERT_STRING_EQUAL(entry->expr, "<NONE>");
    CU_ASSERT_STRING_EQUAL(entry->fmt,  "Root of negative value x={iArg}");
    CU_ASSERT_STRING_EQUAL(entry->msg,  "Root of negative value x=-1");
}

extern void test_error_handling_stack_works_2(void) {
    struct rfdtd_error_stack e;
    struct rfdtd_error_entry *entry;
    rfdtd_initialize_stack(&e);

    CU_ASSERT_EQUAL(pseudo_sqrt_2( 1, &e), 1);
    CU_ASSERT_EQUAL(e.tip,   0);
    CU_ASSERT_EQUAL(e.count, 0);

    CU_ASSERT_EQUAL(pseudo_sqrt_2(-1, &e), 0);
    CU_ASSERT_EQUAL(e.tip,   1);
    CU_ASSERT_EQUAL(e.count, 1);

    entry = rfdtd_get_stack_entry(&e, 0);
    CU_ASSERT_STRING_EQUAL(entry->file, __FILE__);
    CU_ASSERT_STRING_EQUAL(entry->expr, "x >= 0");
    CU_ASSERT_STRING_EQUAL(entry->fmt,  "Root of negative value x={iArg}");
    CU_ASSERT_STRING_EQUAL(entry->msg,  "Root of negative value x=-1");
}

