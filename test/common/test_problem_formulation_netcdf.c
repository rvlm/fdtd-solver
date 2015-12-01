#include <CUnit/CUnit.h>
#include "rvlm/fdtd/common/problem_formulation_netcdf.h"
#include "rvlm/fdtd/test_config.h"

extern void test_problem_formulation_netcdf_lattice_1x2x2(void) {

    const char *filename;
    filename = RFDTD_PROJECT_BINARY_DIR "/test/_data/yee_lattice_1x2x2.nc";

    struct rfdtd_error_stack stack;
    struct rfdtd_error_stack *e = &stack;
    rfdtd_initialize_stack(&stack);

    struct rfdtd_problem_formulation *problem;

    problem = rfdtd_create_problem_formulation_from_netcdf(filename, e);
    CU_ASSERT_EQUAL(e->count, 0);
    CU_ASSERT_PTR_NOT_NULL_FATAL(problem);

    rfdtd_destroy_problem_formulation_netcdf(problem);
}
