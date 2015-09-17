#include <stdlib.h>
#include "rvlm/fdtd/common/common_helpers_internal.h"
#include "rvlm/fdtd/common/memory_allocation.h"

void *rfdtd_memory_allocate(size_t size, struct rfdtd_error_stack *stack) {

    void *ptr = malloc(size);
    if (ptr == NULL) {
        RAISE(RFDTD_ALLOC_ERROR,
              "Unable to allocate {iSize} bytes of memory",
              "iSize", size);
    }

failure:
    return ptr;
}

void rfdtd_memory_free(void *ptr) {
    free(*(void **)ptr);
    *(void **)ptr = NULL;
}
