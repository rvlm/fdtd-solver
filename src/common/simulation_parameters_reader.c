#include <hdf5.h>
#include "common_helpers_internal.h"
#include "simulation_parameters_reader.h"

struct rfdtd_error_info
rfdtd_read_hdf5_file(const char *filename,
		     struct rfdtd_simulation_params **result) {

    struct rfdtd_error_info error = rfdtd_no_error;
    struct rfdtd_simulation_params *params;
    hid_t  file_id;
    hid_t  hid;
    herr_t status;

    CHECK_NEW(params);

    file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0)
	goto failure;

    /* To be continued. */

    H5Fclose(file_id);

    *result = params;
    return error;

failure:
    FREE(params);
    H5Fclose(file_id);
    return error;
}

void rfdtd_destroy_simulation_parameters(struct rfdtd_simulation_params* contents) {

}
