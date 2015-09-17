#ifndef RFDTD_MEMORY_ALLOCATION_H_
#define RFDTD_MEMORY_ALLOCATION_H_

#include <stddef.h>
#include "rvlm/fdtd/common/error_handling.h"

void *rfdtd_memory_allocate(size_t size, struct rfdtd_error_stack* stack);
/** Allocates memory for all library needs. This function acts just like
    `malloc`, but use error stack approach for error handling which makes it
    suitable for :func:`TRY` macro. Normally it should not be used directly,
    because macros :func:`NEW`, :func:`ALLOC`, :func:`TRY_NEW`, and
    :func:`TRY_ALLOC` offer a better alternative in many cases. */

void rfdtd_memory_free(void *ptr);

#endif
