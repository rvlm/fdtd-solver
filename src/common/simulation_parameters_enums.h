#ifndef RFDTD_ENUMERATIONS_H_
#define RFDTD_ENUMERATIONS_H_

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

enum rfdtd_axis_type {
    RFDTD_REGULAR_AXIS   = 0,
    RFDTD_IRREGULAR_AXIS = 1
};

enum rfdtd_boundary_type {
    RFDTD_PEC_BOUNDARY = 0,
    RFDTD_PML_BOUNDARY = 1
};

enum rfdtd_material_type {
    RFDTD_PEC_MATERIAL,
    RFDTD_NORMAL_MATERIAL,
};

enum rfdtd_probe_type {
    RFDTD_E_PROBE,
};

#endif
