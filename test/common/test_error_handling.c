#include <CUnit/CUnit.h>
#include <stddef.h>
#include "rvlm/fdtd/common/error_handling.h"

static int pseudo_sqrt(int x, struct rfdtd_error_stack *stack) {
    int result;
    
    if (x >= 0)
	result = x;
    else {
    result = 0;
    rfdtd_push_error(stack, __FILE__, __LINE__, "<NONE>",
			 RFDTD_UNKNOWN_ERROR,
             "Root of negative value x={iArg}",
             "iArg", x);
    }

    return result;
}

extern void test_error_handling_null_stack_is_fine(void) {
    CU_ASSERT_EQUAL(pseudo_sqrt( 1, NULL), 1);
    CU_ASSERT_EQUAL(pseudo_sqrt(-1, NULL), 0);
}

extern void test_error_handling_stack_works(void) {
    struct rfdtd_error_stack  stack;
    struct rfdtd_error_info  *entry;
    rfdtd_initialize_stack(&stack);
    
    CU_ASSERT_EQUAL(pseudo_sqrt( 1, &stack), 1);
    CU_ASSERT_EQUAL(stack.tip,   0);
    CU_ASSERT_EQUAL(stack.count, 0);

    CU_ASSERT_EQUAL(pseudo_sqrt(-1, &stack), 0);
    CU_ASSERT_EQUAL(stack.tip,   1);
    CU_ASSERT_EQUAL(stack.count, 1);

    entry = rfdtd_get_stack_entry(&stack, 0);
    CU_ASSERT_STRING_EQUAL(entry->file, __FILE__);
    CU_ASSERT_STRING_EQUAL(entry->expr, "<NONE>");
    CU_ASSERT_STRING_EQUAL(entry->fmt,  "Root of negative value x={iArg}");
    CU_ASSERT_STRING_EQUAL(entry->msg,  "Root of negative value x=-1");
}
