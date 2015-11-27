#ifndef RFDTD_NETCDF_HELPERS_H_
#define RFDTD_NETCDF_HELPERS_H_

#include <netcdf.h>
#include "rvlm/fdtd/common/common_typedefs.h"
#include "rvlm/fdtd/common/error_handling.h"

void rfdtd_netcdf_getdim(int ncid, const char *name, size_t *result,
                         struct rfdtd_error_stack *e);

void rfdtd_netcdf_getnumarray(int ncid, const char *name,
			                  rfdtd_number_t *result, size_t count,
                              struct rfdtd_error_stack *e);

#endif
