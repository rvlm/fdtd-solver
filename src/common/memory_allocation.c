#include <stdlib.h>
#include "rvlm/fdtd/common/error_handling.h"
#include "rvlm/fdtd/common/memory_allocation.h"
#include "rvlm/fdtd/common/internal_macros.h"

void *rfdtd_memory_allocate(size_t size, struct rfdtd_error_stack *e) {

    void *ptr = malloc(size);
    if (ptr == NULL) {
        RAISE(RFDTD_ALLOCATION_ERROR,
              "Unable to allocate {iSize} bytes of memory",
              "iSize", size);
    }

e_raise:
    return ptr;
}

void rfdtd_memory_free(void *ptr) {
    free(ptr);
}
