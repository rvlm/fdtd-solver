#include <CUnit/Basic.h>
#include "generated-tests.h"

int main(void) {
    int error;

    error = setup_tests_registry();
    if (error != CUE_SUCCESS) {
        return -1;
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    return (int)CU_get_number_of_tests_failed();
}
