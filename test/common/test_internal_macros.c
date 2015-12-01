#include <CUnit/CUnit.h>
#include "rvlm/fdtd/common/error_handling.h"
#include "rvlm/fdtd/common/internal_macros.h"

extern void test_internal_macros_new_and_dispose(void) {

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

    CU_ASSERT_PTR_NOT_NULL(fs);
    CU_ASSERT_PTR_NOT_NULL(vs);
    CU_ASSERT_PTR_NOT_NULL(cs);
    CU_ASSERT_PTR_NOT_NULL(ss);

e_memory:
    DISPOSE(ss);
    DISPOSE(cs);
    DISPOSE(vs);
    DISPOSE(fs);

    CU_ASSERT_PTR_NULL(fs);
    CU_ASSERT_PTR_NULL(vs);
    CU_ASSERT_PTR_NULL(cs);
    CU_ASSERT_PTR_NULL(ss);
}
