#include <netcdf.h>
#include "rvlm/fdtd/common/internal_macros.h"
#include "rvlm/fdtd/common/netcdf_helpers.h"
#include "rvlm/fdtd/common/problem_formulation.h"
#include "rvlm/fdtd/common/problem_formulation_netcdf.h"

struct rfdtd_problem_formulation *rfdtd_create_problem_formulation_from_netcdf(
        const char *filename, struct rfdtd_error_stack *e) {

    struct rfdtd_problem_formulation *result  = NULL;
    struct rfdtd_lattice_params      *yee = NULL;

    int err;
    int ncid;
    err = nc_open(filename, NC_NOWRITE, &ncid);
    if (err != NC_NOERR)
        goto e_fileopen;

    NEW(result);
    rfdtd_init_problem_formulation(result);

    yee = &result->lattice;

    //
    rfdtd_netcdf_getdim(ncid, "yee_nx_Ex", &yee->nx_Ex, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nx_Ey", &yee->nx_Ey, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nx_Ez", &yee->nx_Ez, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Ex", &yee->ny_Ex, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Ey", &yee->ny_Ey, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Ez", &yee->ny_Ez, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Ex", &yee->nz_Ex, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Ey", &yee->nz_Ey, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Ez", &yee->nz_Ez, e); CHECK(e);
    

    NEW_ARRAY(yee->x_Ex, yee->nx_Ex);
    NEW_ARRAY(yee->x_Ey, yee->nx_Ey);
    NEW_ARRAY(yee->x_Ez, yee->nx_Ez);
    NEW_ARRAY(yee->y_Ex, yee->ny_Ex);
    NEW_ARRAY(yee->y_Ey, yee->ny_Ey);
    NEW_ARRAY(yee->y_Ez, yee->ny_Ez);
    NEW_ARRAY(yee->z_Ex, yee->nz_Ex);
    NEW_ARRAY(yee->z_Ey, yee->nz_Ey);
    NEW_ARRAY(yee->z_Ez, yee->nz_Ez);

    rfdtd_netcdf_getnumarray(ncid, "yee_x_Ex", yee->x_Ex, yee->nx_Ex, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Ey", yee->x_Ey, yee->nx_Ey, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Ez", yee->x_Ez, yee->nx_Ez, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Ex", yee->y_Ex, yee->ny_Ex, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Ey", yee->y_Ey, yee->ny_Ey, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Ez", yee->y_Ez, yee->ny_Ez, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Ex", yee->z_Ex, yee->nz_Ex, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Ey", yee->z_Ey, yee->nz_Ey, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Ez", yee->z_Ez, yee->nz_Ez, e); CHECK(e);
    return result;

e_check:
    nc_close(ncid);

e_memory:
    rfdtd_destroy_problem_formulation_netcdf(result);

e_fileopen:
    return NULL;
}

void rfdtd_destroy_problem_formulation_netcdf(
        struct rfdtd_problem_formulation *problem) {

    if (problem == NULL)
        return;

    DISPOSE(problem->lattice.x_Ex);
    DISPOSE(problem->lattice.x_Ey);
    DISPOSE(problem->lattice.x_Ez);
    DISPOSE(problem->lattice.y_Ex);
    DISPOSE(problem->lattice.y_Ey);
    DISPOSE(problem->lattice.y_Ez);
    DISPOSE(problem->lattice.z_Ex);
    DISPOSE(problem->lattice.z_Ey);
    DISPOSE(problem->lattice.z_Ez);
    DISPOSE(problem);
}
