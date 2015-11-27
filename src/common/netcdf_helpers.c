#include <rvlm/fdtd/common/internal_macros.h>
#include "rvlm/fdtd/common/netcdf_helpers.h"
#include "rvlm/fdtd/common/error_handling.h"

#if RFDTD_CONFIG_USE_DOUBLE
#define GET_VARA nc_get_vara_double
#else
#define GET_VARA nc_get_vara_float
#endif

void rfdtd_netcdf_getdim(int ncid, const char *name, size_t *result,
                         struct rfdtd_error_stack *e) {
    int err;
    int dimid;

    err = nc_inq_dimid(ncid, name, &dimid);
    if (err != NC_NOERR) {
        RAISE(RFDTD_UNKNOWN_ERROR, "", NULL);
    }

    err = nc_inq_dim(ncid, dimid, NULL, result);
    if (err != NC_NOERR) {
        RAISE(RFDTD_UNKNOWN_ERROR, "", NULL);
    }

e_raise:
    return;
}

void rfdtd_netcdf_getnumarray(int ncid, const char *name,
                              rfdtd_number_t *result, size_t count,
                              struct rfdtd_error_stack *e) {
    int err;
    int varid;
    size_t start_idx;

    err = nc_inq_varid(ncid, name, &varid);
    if (err != NC_NOERR) {
        RAISE(RFDTD_UNKNOWN_ERROR, "", NULL);
    }

    start_idx = 0;
    err = GET_VARA(ncid, varid, &start_idx, &count, result);
    if (err != NC_NOERR) {
        RAISE(RFDTD_UNKNOWN_ERROR, "", NULL);
    }

e_raise:
    return;
}
