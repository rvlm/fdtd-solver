#include <stdlib.h>
#include "memory_allocation.h"

/* HACK: Strictly speaking, argument 'result' should be 'void **', but it turned
         out that having it declared so issues the "passing incompatible pointer
         argument" warning. It's sad that C defines 'void *' as a generic type
         while leaving 'void **' a specific one. */
struct rfdtd_error_info
rfdtd_memory_allocate(void *result, size_t size) {
    struct rfdtd_error_info error = rfdtd_no_error;
    void *ptr = malloc(size);

    if (!ptr) {
	error.code = RFDTD_ALLOC_ERROR;
	error.msg  = "Unable to allocate memory";
	error.file = __FILE__;
	error.line = __LINE__;
	return error;
    }

    *(void **)result = ptr;
    return error;
}

void rfdtd_memory_free(void *ptr) {
    free(ptr);
}
