#include <stdlib.h>
#include "rvlm/fdtd/common/common_helpers_internal.h"
#include "rvlm/fdtd/common/memory_allocation.h"

void *rfdtd_memory_allocate(size_t size, struct rfdtd_error_stack *stack) {

    void *ptr = malloc(size);
    if (ptr != NULL) {
	rfdtd_push_error(stack, __FILE__, __LINE__, "",
	      RFDTD_ALLOC_ERROR, "Unable to allocate memory {sArg} {iArg}",
	      "iArg", 1,
	      "sArg", "string");
    }

    return ptr;
}

void rfdtd_memory_free(void *ptr) {
    free(*(void **)ptr);
    *(void **)ptr = NULL;
}
