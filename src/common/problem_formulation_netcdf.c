#include <netcdf.h>
#include "rvlm/fdtd/common/internal_macros.h"
#include "rvlm/fdtd/common/netcdf_helpers.h"
#include "rvlm/fdtd/common/problem_formulation.h"
#include "rvlm/fdtd/common/problem_formulation_netcdf.h"

struct constructor_args {
    int ncid;
};

static void read_material(
        struct rfdtd_problem_formulation *problem,
        enum   rfdtd_material_components  component,
        rfdtd_number_t *buf, size_t *offsets, size_t *counts,
        struct rfdtd_error_stack *e) {

    struct constructor_args *args = problem->constructor_arg;
    const char *varname;

    // TODO: Replace names with NetCDF IDs.
    switch (component)
    {
        case RFDTD_EPSILON_EX: varname = "world_epsilon_Ex"; break;
        case RFDTD_EPSILON_EY: varname = "world_epsilon_Ey"; break;
        case RFDTD_EPSILON_EZ: varname = "world_epsilon_Ez"; break;
        case RFDTD_MU_HX:      varname = "world_mu_Hx";      break;
        case RFDTD_MU_HY:      varname = "world_mu_Hy";      break;
        case RFDTD_MU_HZ:      varname = "world_mu_Hz";      break;
        case RFDTD_SIGMAE_EX:  varname = "world_sigmaE_Ex";  break;
        case RFDTD_SIGMAE_EY:  varname = "world_sigmaE_Ey";  break;
        case RFDTD_SIGMAE_EZ:  varname = "world_sigmaE_Ez";  break;
        case RFDTD_SIGMAH_HX:  varname = "world_sigmaH_Hx";  break;
        case RFDTD_SIGMAH_HY:  varname = "world_sigmaH_Hy";  break;
        case RFDTD_SIGMAH_HZ:  varname = "world_sigmaH_Hz";  break;
        default:
            RAISE_ASSERT(false);
    }

    // TODO: Check index ranges?
    rfdtd_netcdf_getnumarrayN(args->ncid, varname, buf, offsets, counts, e);
    CHECK(e);

e_check:
e_raise:
    ;
}


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
    
    rfdtd_netcdf_getdim(ncid, "yee_nx_Hx", &yee->nx_Hx, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nx_Hy", &yee->nx_Hy, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nx_Hz", &yee->nx_Hz, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Hx", &yee->ny_Hx, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Hy", &yee->ny_Hy, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_ny_Hz", &yee->ny_Hz, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Hx", &yee->nz_Hx, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Hy", &yee->nz_Hy, e); CHECK(e);
    rfdtd_netcdf_getdim(ncid, "yee_nz_Hz", &yee->nz_Hz, e); CHECK(e);

    NEW_ARRAY(yee->x_Ex, yee->nx_Ex);
    NEW_ARRAY(yee->x_Ey, yee->nx_Ey);
    NEW_ARRAY(yee->x_Ez, yee->nx_Ez);
    NEW_ARRAY(yee->y_Ex, yee->ny_Ex);
    NEW_ARRAY(yee->y_Ey, yee->ny_Ey);
    NEW_ARRAY(yee->y_Ez, yee->ny_Ez);
    NEW_ARRAY(yee->z_Ex, yee->nz_Ex);
    NEW_ARRAY(yee->z_Ey, yee->nz_Ey);
    NEW_ARRAY(yee->z_Ez, yee->nz_Ez);

    NEW_ARRAY(yee->x_Hx, yee->nx_Hx);
    NEW_ARRAY(yee->x_Hy, yee->nx_Hy);
    NEW_ARRAY(yee->x_Hz, yee->nx_Hz);
    NEW_ARRAY(yee->y_Hx, yee->ny_Hx);
    NEW_ARRAY(yee->y_Hy, yee->ny_Hy);
    NEW_ARRAY(yee->y_Hz, yee->ny_Hz);
    NEW_ARRAY(yee->z_Hx, yee->nz_Hx);
    NEW_ARRAY(yee->z_Hy, yee->nz_Hy);
    NEW_ARRAY(yee->z_Hz, yee->nz_Hz);
    
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Ex", yee->x_Ex, 0, yee->nx_Ex, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Ey", yee->x_Ey, 0, yee->nx_Ey, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Ez", yee->x_Ez, 0, yee->nx_Ez, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Ex", yee->y_Ex, 0, yee->ny_Ex, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Ey", yee->y_Ey, 0, yee->ny_Ey, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Ez", yee->y_Ez, 0, yee->ny_Ez, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Ex", yee->z_Ex, 0, yee->nz_Ex, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Ey", yee->z_Ey, 0, yee->nz_Ey, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Ez", yee->z_Ez, 0, yee->nz_Ez, e); CHECK(e);
    
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Hx", yee->x_Hx, 0, yee->nx_Hx, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Hy", yee->x_Hy, 0, yee->nx_Hy, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_x_Hz", yee->x_Hz, 0, yee->nx_Hz, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Hx", yee->y_Hx, 0, yee->ny_Hx, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Hy", yee->y_Hy, 0, yee->ny_Hy, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_y_Hz", yee->y_Hz, 0, yee->ny_Hz, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Hx", yee->z_Hx, 0, yee->nz_Hx, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Hy", yee->z_Hy, 0, yee->nz_Hy, e); CHECK(e);
    rfdtd_netcdf_getnumarray(ncid, "yee_z_Hz", yee->z_Hz, 0, yee->nz_Hz, e); CHECK(e);

    rfdtd_check_problem_formulation(result, e); CHECK(e);

    struct constructor_args *args;
    NEW(args);
    args->ncid = ncid;
    result->read_materials = read_material;
    result->constructor_arg = args;

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
    DISPOSE(problem->lattice.x_Hx);
    DISPOSE(problem->lattice.x_Hy);
    DISPOSE(problem->lattice.x_Hz);
    DISPOSE(problem->lattice.y_Hx);
    DISPOSE(problem->lattice.y_Hy);
    DISPOSE(problem->lattice.y_Hz);
    DISPOSE(problem->lattice.z_Hx);
    DISPOSE(problem->lattice.z_Hy);
    DISPOSE(problem->lattice.z_Hz);

    struct constructor_args *args = problem->constructor_arg;
    nc_close(args->ncid);

    DISPOSE(problem);
    DISPOSE(args);
}
