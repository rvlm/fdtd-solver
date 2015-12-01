#include "rvlm/fdtd/common/problem_formulation.h"

void rfdtd_init_problem_formulation(struct rfdtd_problem_formulation* problem) {

    // NOTE: Portability!
    memset(problem, 0, sizeof(*problem));
}
