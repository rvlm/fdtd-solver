#ifndef RFDTD_CONTENTS_PARAMS_H_
#define RFDTD_CONTENTS_PARAMS_H_

#include <time.h>
#include <stdbool.h>
#include "rvlm/fdtd/common/common_typedefs.h"
#include "rvlm/fdtd/common/simulation_parameters_enums.h"

struct rfdtd_axis_params;
struct rfdtd_boundary_params;
struct rfdtd_material_params;
struct rfdtd_probe_params;
struct rfdtd_lumped_params;
struct rfdtd_struct_patch_params;

struct rfdtd_file_metadata {
    unsigned  file_format_version;
    time_t    creation_time;
    char     *application_name;
    char     *author_name;
    char     *author_organization;
};

struct rfdtd_scatterfield_params {
    bool enabled;

    rfdtd_index_t start_ix;
    rfdtd_index_t start_iy;
    rfdtd_index_t start_iz;
    rfdtd_index_t stop_ix;
    rfdtd_index_t stop_iy;
    rfdtd_index_t stop_iz;
};

struct rfdtd_farfield_params {
    bool enabled;

    rfdtd_index_t start_ix;
    rfdtd_index_t start_iy;
    rfdtd_index_t start_iz;
    rfdtd_index_t stop_ix;
    rfdtd_index_t stop_iy;
    rfdtd_index_t stop_iz;

    struct rfdtd_farfield_probe_params *probes;
};

struct rfdtd_farfield_probe_params {
    const char *name;
    enum rfdtd_field_components components;
    rfdtd_number_t x;
    rfdtd_number_t y;
    rfdtd_number_t z;
};

struct rfdtd_simulation_params {
    struct rfdtd_file_metadata        metadata;
    struct rfdtd_scatterfield_params  scatterfield;
    struct rfdtd_farfield_params      farfield;
    struct rfdtd_axis_params         *axes;
    struct rfdtd_boundary_params     *boundaries;
    struct rfdtd_material_params     *materials;
    struct rfdtd_probe_params        *probes;
    struct rfdtd_lumped_params       *lumpeds;
    struct rfdtd_struct_patch_params *patches;

    unsigned axes_count;
    unsigned boundaries_count;
    unsigned materials_count;
    unsigned probes_count;
    unsigned lumpeds_count;
    unsigned patches_count;
};

struct rfdtd_axis_params {
    enum rfdtd_axis_type type;

    rfdtd_number_t minimum;
    rfdtd_number_t maximum;
    rfdtd_number_t delta;
    rfdtd_number_t ticks;
    unsigned     ticks_count;
};

struct rfdtd_boundary_params {
    enum rfdtd_boundary_type type;
    unsigned order;
};

struct rfdtd_material_params {
    enum rfdtd_material_type type;
    rfdtd_number_t epsilon;
    rfdtd_number_t mu;
    rfdtd_number_t sigmaE;
    rfdtd_number_t sigmaH;
};

struct rfdtd_probe_params {
    enum rfdtd_probe_type type;
    rfdtd_number_t x;
    rfdtd_number_t y;
    rfdtd_number_t z;
    rfdtd_number_t dx;
    rfdtd_number_t dy;
    rfdtd_number_t dz;
};

struct rfdtd_lumped_params {
    enum rfdtd_probe_type type;
    rfdtd_number_t x;
    rfdtd_number_t y;
    rfdtd_number_t z;
};

struct rfdtd_struct_params_patch {
    /* Who knows. */
};

#endif
