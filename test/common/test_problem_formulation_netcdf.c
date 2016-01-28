#include <stddef.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>
#include <rvlm/fdtd/common/internal_macros.h>
#include "rvlm/fdtd/common/problem_formulation_netcdf.h"
#include "rvlm/fdtd/test_config.h"

static void assert_all_equal(
        const rfdtd_number_t *buf, rfdtd_number_t val, size_t count) {

    size_t i;
    for (i = 0; i < count; ++i)
        assert_int_equal(buf[i], val);
}

extern void test_problem_formulation_netcdf_lattice_1x2x3(void) {
    const char *filename;
    filename = RFDTD_PROJECT_BINARY_DIR "/test/_data/yee_lattice_1x2x3.nc";

    struct rfdtd_error_stack stack;
    struct rfdtd_error_stack *e = &stack;
    rfdtd_initialize_stack(&stack);

    struct rfdtd_problem_formulation *problem;

    problem = rfdtd_create_problem_formulation_from_netcdf(filename, e);
    assert_int_equal(e->count, 0);
    assert_non_null(problem);

    // Should definitely fit.
    rfdtd_number_t buf[32];

    struct rfdtd_lattice_params *lattice = &problem->lattice;
    size_t offsets[3] = {0, 0, 0};
    size_t counts[3];
    size_t total_count;

    // Check material components for Ex.
    counts[0] = lattice->nx_Ex;
    counts[1] = lattice->ny_Ex;
    counts[2] = lattice->nz_Ex;
    total_count = counts[0] * counts[1] * counts[2];
    problem->read_materials(problem, RFDTD_EPSILON_EX, buf, offsets, counts, e);
    CHECK(e);
    assert_all_equal(buf, 1, total_count);

    problem->read_materials(problem, RFDTD_SIGMAE_EX, buf, offsets, counts, e);
    CHECK(e);
    assert_all_equal(buf, 0, total_count);

    // Check material components for Ey.
    counts[0] = lattice->nx_Ey;
    counts[1] = lattice->ny_Ey;
    counts[2] = lattice->nz_Ey;
    total_count = counts[0] * counts[1] * counts[2];
    problem->read_materials(problem, RFDTD_EPSILON_EY, buf, offsets, counts, e);
    CHECK(e);
    assert_all_equal(buf, 1, total_count);

    problem->read_materials(problem, RFDTD_SIGMAE_EY, buf, offsets, counts, e);
    CHECK(e);
    assert_all_equal(buf, 0, total_count);

    // Check material components for Ez.
    counts[0] = lattice->nx_Ez;
    counts[1] = lattice->ny_Ez;
    counts[2] = lattice->nz_Ez;
    total_count = counts[0] * counts[1] * counts[2];
    problem->read_materials(problem, RFDTD_EPSILON_EZ, buf, offsets, counts, e);
    CHECK(e);
    assert_all_equal(buf, 1, total_count);

    problem->read_materials(problem, RFDTD_SIGMAE_EZ, buf, offsets, counts, e);
    CHECK(e);
    assert_all_equal(buf, 0, total_count);

    rfdtd_destroy_problem_formulation_netcdf(problem);
    return;

e_check:
    fail();
}
