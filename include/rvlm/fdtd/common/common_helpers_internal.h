#ifndef RFDTD_COMMON_HELPERS_IMPL_H_
#define RFDTD_COMMON_HELPERS_IMPL_H_

#include <stdbool.h>
#include "rvlm/fdtd/common/error_handling.h"
#include "rvlm/fdtd/common/memory_allocation.h"

/* Memory management helpers. */

#define NEW(var) \
    rfdtd_memory_allocate(&(var), sizeof(*(var)))

#define ALLOC(var, num) \
    rfdtd_memory_allocate(&(var), (num)*sizeof(*(var)))

#define FREE(var) \
    rfdtd_memory_free(&(var))

/* Error handling helpers. */

#define CHECK(expr) \
    do { ON_FAILURE(expr) goto failure; } while (false)

#define ON_FAILURE(expr) \
    if(!rfdtd_lift_error((expr), &error))

/* Error handling helpers for memory management. */

#define CHECK_NEW(var)             CHECK(NEW(var))
#define CHECK_ALLOC(var, num)      CHECK(ALLOC(var, num))
#define ON_NEW_FAILURE(var)        ON_FAILURE(NEW(var))
#define ON_ALLOC_FAILURE(var, num) ON_FAILURE(ALLOC(var, num))

static inline bool
rfdtd_okay(const struct rfdtd_error_info error) {
    return error.code == RFDTD_NO_ERROR;
}

static inline bool
rfdtd_lift_error(const struct rfdtd_error_info  error,
		       struct rfdtd_error_info *result) {
    return rfdtd_okay(error) ? true : ((*result = error), false);
}

#endif
