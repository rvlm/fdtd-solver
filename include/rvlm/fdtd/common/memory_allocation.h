#ifndef RFDTD_MEMORY_ALLOCATION_H_
#define RFDTD_MEMORY_ALLOCATION_H_

#include <stddef.h>
#include "rvlm/fdtd/common/error_handling.h"

void *rfdtd_memory_allocate(size_t size,
			   struct rfdtd_error_stack* e);

void rfdtd_memory_free(void *ptr);

#endif
