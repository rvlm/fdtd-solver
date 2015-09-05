#ifndef RFDTD_COMMON_TYPEDEFS_H_
#define RFDTD_COMMON_TYPEDEFS_H_

#include <stdint.h>
#include "rvlm/fdtd/config.h"

#if RFDTD_CONFIG_USE_DOUBLE
    typedef double rfdtd_number_t;
#else
    typedef float rfdtd_number_t;
#endif

typedef uint_fast64_t rfdtd_index_t;

#endif
