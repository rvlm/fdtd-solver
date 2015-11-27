#ifndef RFDTD_SIMULATION_PARAMETERS_READER_H_
#define RFDTD_SIMULATION_PARAMETERS_READER_H_

#include "rvlm/fdtd/common/problem_formulation.h"
#include "rvlm/fdtd/common/error_handling.h"

struct rfdtd_problem_formulation *
rfdtd_create_problem_formulation_from_netcdf(
        const char *filename, struct rfdtd_error_stack *e);

void rfdtd_destroy_problem_formulation_netcdf(
        struct rfdtd_problem_formulation *contents);

#endif
