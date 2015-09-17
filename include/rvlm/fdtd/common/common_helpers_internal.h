#ifndef RFDTD_COMMON_HELPERS_INTERNAL_H_
#define RFDTD_COMMON_HELPERS_INTERNAL_H_

#include <stdbool.h>
#include "rvlm/fdtd/common/error_handling.h"
#include "rvlm/fdtd/common/memory_allocation.h"

/* Memory management helpers. */

#define NEW(var) \
    ALLOC((var), 1)

#define ALLOC(var, num) \
    ((var) = rfdtd_memory_allocate((num)*sizeof(*(var)), stack))

#define FREE(var) \
    rfdtd_memory_free(&(var))

/* Error handling helpers. */

#define TRY(expr) \
    do { ON_FAILURE(expr) goto failure; } while (false)

#define ON_FAILURE(expr) \
    if((expr), rfdtd_okay(stack) || \
       rfdtd_push_error(stack, __FILE__, __LINE__, #expr, ""))

#define ON_FAILURE_M(expr, code, fmt, ...) \
    if ((expr), rfdtd_okay(stack) ||       \
                rfdtd_push_error(stack,    \
                      __FILE__, __LINE__, #expr, code, fmt, __VA_ARGS__, NULL))

#define ASSERT_M(expr, code, fmt, ...) \
     ...

#define RAISE(code, fmt, ...)

/* Error handling helpers for memory management. */

#define CHECK_NEW(var)             CHECK(NEW(var))
#define CHECK_ALLOC(var, num)      CHECK(ALLOC(var, num))
#define ON_NEW_FAILURE(var)        ON_FAILURE(NEW(var))
#define ON_ALLOC_FAILURE(var, num) ON_FAILURE(ALLOC(var, num))

static inline bool rfdtd_okay(const struct rfdtd_error_stack *stack) {
    return stack->count == 0;
}

#endif
