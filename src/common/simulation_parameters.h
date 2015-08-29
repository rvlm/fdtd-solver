#ifndef RFDTD_CONTENTS_PARAMS_H_
#define RFDTD_CONTENTS_PARAMS_H_

#include <time.h>
#include "common_typedefs.h"
#include "simulation_parameters_enums.h"

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
};

struct rfdtd_farfield_params {
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

    rfdtd_real_t minimum;
    rfdtd_real_t maximum;
    rfdtd_real_t delta;
    rfdtd_real_t ticks;
    unsigned     ticks_count;
};

struct rfdtd_boundary_params {
    enum rfdtd_boundary_type type;
    unsigned order;
};

struct rfdtd_material_params {
    enum rfdtd_material_type type;
    rfdtd_real_t epsilon;
    rfdtd_real_t mu;
    rfdtd_real_t sigmaE;
    rfdtd_real_t sigmaH;
};

struct rfdtd_probe_params {
    enum rfdtd_probe_type type;
    rfdtd_real_t x;
    rfdtd_real_t y;
    rfdtd_real_t z;
    rfdtd_real_t dx;
    rfdtd_real_t dy;
    rfdtd_real_t dz;
};

struct rfdtd_lumped_params {
    enum rfdtd_probe_type type;
    rfdtd_real_t x;
    rfdtd_real_t y;
    rfdtd_real_t z;
};

struct rfdtd_struct_params_patch {
    /* Who knows. */
};

#endif
