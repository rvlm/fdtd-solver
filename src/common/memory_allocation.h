#ifndef RFDTD_MEMORY_ALLOCATION_H_
#define RFDTD_MEMORY_ALLOCATION_H_

#include <stddef.h>
#include "error_handling.h"

struct rfdtd_error_info
rfdtd_memory_allocate(void *result, size_t size);

void
rfdtd_memory_free(void *ptr);

#endif
