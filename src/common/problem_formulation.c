#include <string.h>
#include "rvlm/fdtd/common/problem_formulation.h"
#include <rvlm/fdtd/common/internal_macros.h>

void rfdtd_init_problem_formulation(struct rfdtd_problem_formulation* problem) {

    // NOTE: Portability!
    memset(problem, 0, sizeof(*problem));
}

void rfdtd_check_problem_formulation(
        struct rfdtd_problem_formulation *problem,
        struct rfdtd_error_stack *e) {

    struct rfdtd_lattice_params *lattice;
    int i;

    RAISE_ASSERT(problem != NULL);
    lattice = &problem->lattice;

    RAISE_ASSERT(lattice->x_Ex != NULL);
    RAISE_ASSERT(lattice->x_Ey != NULL);
    RAISE_ASSERT(lattice->x_Ez != NULL);
    RAISE_ASSERT(lattice->y_Ex != NULL);
    RAISE_ASSERT(lattice->y_Ey != NULL);
    RAISE_ASSERT(lattice->y_Ez != NULL);
    RAISE_ASSERT(lattice->z_Ex != NULL);
    RAISE_ASSERT(lattice->z_Ey != NULL);
    RAISE_ASSERT(lattice->z_Ez != NULL);

    // Check if Yee lattice has correct number of cells.
    RAISE_ASSERT(lattice->nx_Ex > 0);
    RAISE_ASSERT(lattice->nx_Ey > 0);
    RAISE_ASSERT(lattice->nx_Ez > 0);
    RAISE_ASSERT(lattice->ny_Ex > 0);
    RAISE_ASSERT(lattice->ny_Ey > 0);
    RAISE_ASSERT(lattice->ny_Ez > 0);
    RAISE_ASSERT(lattice->nz_Ex > 0);
    RAISE_ASSERT(lattice->nz_Ey > 0);
    RAISE_ASSERT(lattice->nz_Ez > 0);

    RAISE_ASSERT(lattice->nx_Ey == lattice->nx_Ez &&
                 lattice->nx_Ey == lattice->nx_Ex + 1);
    RAISE_ASSERT(lattice->ny_Ex == lattice->ny_Ez &&
                 lattice->ny_Ex == lattice->ny_Ey + 1);
    RAISE_ASSERT(lattice->nz_Ex == lattice->nz_Ey &&
                 lattice->nz_Ex == lattice->nz_Ez + 1);

    // Check if coordinates are ascending.
    for (i = 0; i < lattice->nx_Ex-1; ++i)
        RAISE_ASSERT(lattice->x_Ex[i] < lattice->x_Ex[i+1]);

    for (i = 0; i < lattice->nx_Ey-1; ++i)
        RAISE_ASSERT(lattice->x_Ey[i] < lattice->x_Ey[i+1]);

    for (i = 0; i < lattice->nx_Ez-1; ++i)
        RAISE_ASSERT(lattice->x_Ez[i] < lattice->x_Ez[i+1]);

    for (i = 0; i < lattice->ny_Ex-1; ++i)
        RAISE_ASSERT(lattice->y_Ex[i] < lattice->y_Ex[i+1]);

    for (i = 0; i < lattice->ny_Ey-1; ++i)
        RAISE_ASSERT(lattice->y_Ey[i] < lattice->y_Ey[i+1]);

    for (i = 0; i < lattice->ny_Ez-1; ++i)
        RAISE_ASSERT(lattice->y_Ez[i] < lattice->y_Ez[i+1]);

    for (i = 0; i < lattice->nz_Ex-1; ++i)
        RAISE_ASSERT(lattice->z_Ex[i] < lattice->z_Ex[i+1]);

    for (i = 0; i < lattice->nz_Ey-1; ++i)
        RAISE_ASSERT(lattice->z_Ey[i] < lattice->z_Ey[i+1]);

    for (i = 0; i < lattice->nz_Ez-1; ++i)
        RAISE_ASSERT(lattice->z_Ez[i] < lattice->z_Ez[i+1]);

    // TODO: Add another stupid check.

e_raise:
    ;
}
