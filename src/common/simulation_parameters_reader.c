#include <hdf5.h>
#include "rvlm/fdtd/common/internal_macros.h"
#include "rvlm/fdtd/common/simulation_parameters_reader.h"

struct rfdtd_simulation_params *rfdtd_read_hdf5_file(
        const char *filename, struct rfdtd_error_stack *e) {

    struct rfdtd_simulation_params *params;
    hid_t  file_id;
    hid_t  hid;
    herr_t status;

    TRY_NEW(params);

    file_id = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) {
        RAISE(RFDTD_FILE_OPEN_ERROR,
              "Unable to open file '{sFile}' for reading",
              "sFile", filename);
    }

    H5Fclose(file_id);
    return params;

e_raise:
    rfdtd_destroy_simulation_parameters(params);
    H5Fclose(file_id);
    return NULL;
}

void rfdtd_destroy_simulation_parameters(struct rfdtd_simulation_params* contents) {

    FREE(contents);
}
