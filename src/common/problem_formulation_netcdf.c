#include <netcdf.h>
#include "rvlm/fdtd/common/internal_macros.h"
#include "rvlm/fdtd/common/netcdf_helpers.h"
#include "rvlm/fdtd/common/problem_formulation_netcdf.h"

struct rfdtd_problem_formulation *rfdtd_create_problem_formulation_from_netcdf(
        const char *filename, struct rfdtd_error_stack *e) {

    struct rfdtd_problem_formulation *result  = NULL;
    struct rfdtd_lattice_params      *lattice = NULL;

    int err;
    int ncid;
    err = nc_open(filename, NC_NOWRITE, &ncid);
    if (err != NC_NOERR)
        goto e_fileopen;

    NEW(result);
    lattice = &result->lattice;

    //
    rfdtd_netcdf_getdim(ncid, "yee_nx_Ex", &lattice->nx_Ex, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nx_Ey", &lattice->nx_Ey, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nx_Ez", &lattice->nx_Ez, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Ex", &lattice->ny_Ex, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Ey", &lattice->ny_Ey, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Ez", &lattice->ny_Ez, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Ex", &lattice->nz_Ex, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Ey", &lattice->nz_Ey, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Ez", &lattice->nz_Ez, e); CHECK(e);

    NEW_ARRAY(lattice->x_Ex, lattice->nx_Ex);
    NEW_ARRAY(lattice->x_Ey, lattice->nx_Ey);
    NEW_ARRAY(lattice->x_Ez, lattice->nx_Ez);
    NEW_ARRAY(lattice->y_Ex, lattice->ny_Ex);
    NEW_ARRAY(lattice->y_Ey, lattice->ny_Ey);
    NEW_ARRAY(lattice->y_Ez, lattice->ny_Ez);
    NEW_ARRAY(lattice->z_Ex, lattice->nz_Ex);
    NEW_ARRAY(lattice->z_Ey, lattice->nz_Ey);
    NEW_ARRAY(lattice->z_Ez, lattice->nz_Ez);
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Ex", lattice->x_Ex, lattice->nx_Ex, e); CHECK(e);

    return result;

e_check:
    nc_close(ncid);

e_memory:
    rfdtd_destroy_problem_formulation_netcdf(result);

e_fileopen:
    return NULL;
}

void rfdtd_destroy_problem_formulation_netcdf(struct rfdtd_problem_formulation *problem) {
    rfdtd_memory_free(problem->lattice.x_Ex);
    rfdtd_memory_free(problem->lattice.x_Ey);
    rfdtd_memory_free(problem->lattice.x_Ez);
    rfdtd_memory_free(problem->lattice.y_Ex);
    rfdtd_memory_free(problem->lattice.y_Ey);
    rfdtd_memory_free(problem->lattice.y_Ez);
    rfdtd_memory_free(problem->lattice.z_Ex);
    rfdtd_memory_free(problem->lattice.z_Ey);
    rfdtd_memory_free(problem->lattice.z_Ez);
    rfdtd_memory_free(problem);
}
