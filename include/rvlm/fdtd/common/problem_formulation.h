#ifndef RFDTD_CONTENTS_PARAMS_H_
#define RFDTD_CONTENTS_PARAMS_H_

#include <time.h>
#include <stdbool.h>
#include "rvlm/fdtd/common/common_typedefs.h"

enum rfdtd_axis_index {
    RFDTD_AXIS_X = 0,
    RFDTD_AXIS_Y = 1,
    RFDTD_AXIS_Z = 2,
    RFDTD_AXES_COUNT = 3
};

enum rfdtd_direction_index {
    RFDTD_TOWARD_XMIN = 0,
    RFDTD_TOWARD_XMAX = 1,
    RFDTD_TOWARD_YMIN = 2,
    RFDTD_TOWARD_YMAX = 3,
    RFDTD_TOWARD_ZMIN = 4,
    RFDTD_TOWARD_ZMAX = 5,
    RFDTD_DIRECTIONS_COUNT = 6
};

enum rfdtd_field_components {
    RFDTD_EX_COMPONENT = 0x0001,
    RFDTD_EY_COMPONENT = 0x0002,
    RFDTD_EZ_COMPONENT = 0x0004,
    RFDTD_HX_COMPONENT = 0x0010,
    RFDTD_HY_COMPONENT = 0x0020,
    RFDTD_HZ_COMPONENT = 0x0040,

    RFDTD_E_FIELD = RFDTD_EX_COMPONENT
                  | RFDTD_EY_COMPONENT
                  | RFDTD_EZ_COMPONENT,

    RFDTD_H_FIELD = RFDTD_HX_COMPONENT
                  | RFDTD_HY_COMPONENT
                  | RFDTD_HZ_COMPONENT,

    RFDTD_EH_FIELDS = RFDTD_E_FIELD | RFDTD_H_FIELD
};

enum rfdtd_boundary_type {
    RFDTD_PEC_BOUNDARY = 0,
    RFDTD_PML_BOUNDARY = 1
};

enum rfdtd_probe_type {
    RFDTD_E_PROBE,
};

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

struct rfdtd_boundary_params {
    enum rfdtd_boundary_type type;
    unsigned order;
};

struct rfdtd_farfield_probe_params {
    const char *name;
    enum rfdtd_field_components components;
    rfdtd_number_t x;
    rfdtd_number_t y;
    rfdtd_number_t z;
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

struct rfdtd_lattice_params {
    rfdtd_index_t nx_Ex;
    rfdtd_index_t nx_Ey;
    rfdtd_index_t nx_Ez;
    rfdtd_index_t ny_Ex;
    rfdtd_index_t ny_Ey;
    rfdtd_index_t ny_Ez;
    rfdtd_index_t nz_Ex;
    rfdtd_index_t nz_Ey;
    rfdtd_index_t nz_Ez;

    rfdtd_index_t nx_Hx;
    rfdtd_index_t nx_Hy;
    rfdtd_index_t nx_Hz;
    rfdtd_index_t ny_Hx;
    rfdtd_index_t ny_Hy;
    rfdtd_index_t ny_Hz;
    rfdtd_index_t nz_Hx;
    rfdtd_index_t nz_Hy;
    rfdtd_index_t nz_Hz;

    rfdtd_number_t *x_Ex;
    rfdtd_number_t *x_Ey;
    rfdtd_number_t *x_Ez;
    rfdtd_number_t *y_Ex;
    rfdtd_number_t *y_Ey;
    rfdtd_number_t *y_Ez;
    rfdtd_number_t *z_Ex;
    rfdtd_number_t *z_Ey;
    rfdtd_number_t *z_Ez;

    rfdtd_number_t *x_Hx;
    rfdtd_number_t *x_Hy;
    rfdtd_number_t *x_Hz;
    rfdtd_number_t *y_Hx;
    rfdtd_number_t *y_Hy;
    rfdtd_number_t *y_Hz;
    rfdtd_number_t *z_Hx;
    rfdtd_number_t *z_Hy;
    rfdtd_number_t *z_Hz;
};

struct rfdtd_problem_formulation {
    struct rfdtd_file_metadata        metadata;
    struct rfdtd_scatterfield_params  scatterfield;
    struct rfdtd_farfield_params      farfield;
    struct rfdtd_lattice_params       lattice;
    struct rfdtd_boundary_params     *boundaries;
    struct rfdtd_probe_params        *probes;

    unsigned boundaries_count;
    unsigned probes_count;
};

void rfdtd_init_problem_formulation(struct rfdtd_problem_formulation* problem);

#endif
