#ifndef RFDTD_INTERNAL_MACROS_H_
#define RFDTD_INTERNAL_MACROS_H_

#include <stdbool.h>
#include "rvlm/fdtd/common/error_handling.h"
#include "rvlm/fdtd/common/memory_allocation.h"

/** Internal macros to make life interesting
    ========================================

    :copyright: 2015 Pavel Kretov
    :license:   Apache License 2.0

        #include "rvlm/fdtd/common/internal_macros.h"

    This file contains some macro definitions to make programming in pure C
    easier and more foot-bulletproof. Since these macros have quite short and
    common names, they should be used on source (.c) files ONLY, in order not
    to unintentionally break somebody else's code.

    These macros rely on functions from "Memory allocation" and "Error handling"
    sections, but try to provide more concise and convenient interface. */

/** Error handling helpers
    ----------------------

    Historically, pure C lacks good and commonly accepted way for error
    handling. Usually, people use either error codes, or `setjmp` to simulate
    C++'s exceptions. First approach is very tedious, while the second one is
    just dangerous (think of memory leaks).

    In this library we try to make error codes handling less tedious by
    wrapping their processing into handy macros. Consider the following
    real-life example to understand how they work:

        void *rfdtd_memory_allocate(
                    size_t size, struct rfdtd_error_stack *e) {

            void *ptr = malloc(size);
            if (ptr == NULL) {
                RAISE(RFDTD_ALLOC_ERROR,
                      "Unable to allocate {iSize} bytes of memory",
                      "iSize", size);
            }

        e_raise:
            return ptr;
        }

        NEW(a);
        NEW(b);

        do_something(p1, p2, e);
        CHECK(e);

    e_memory:
        DISPOSE(a);
        DISPOSE(b);

    e_raise:
    e_check:
        r

*/

#define RAISE(code, fmt, ...)                                       \
    do {                                                            \
        rfdtd_push_error(e, __FILE__, __LINE__, NULL,               \
                            (code), (fmt), __VA_ARGS__);            \
        goto e_raise;                                               \
    } while (0)

#define RAISE_ASSERT_M(expr, code, fmt, ...)                        \
    do {                                                            \
        if (!(expr)) {                                              \
             rfdtd_push_error(e, __FILE__, __LINE__, #expr, (code), \
                                                 fmt, __VA_ARGS__); \
             goto e_raise;                                          \
        }                                                           \
    } while (0)

#define RAISE_ASSERT(expr) \
    RAISE_ASSERT_M((expr), RFDTD_ASSERTION_ERROR, "Assertion failed", "")

#define CHECK(e) \
    do { if ((e)->count != 0) goto e_check; } while (0)


/** Memory management
    -----------------

    The functions in this section are supposed to be used thoroughly instead of
    standard library's `malloc` and `free`, in order to help programmer to
    avoid some common memory management errors, and spot the error as fast as
    possible. To achieve this, they set the following restrictions:

      - allocated memory can be assigned to null pointers only,
      - allocation failures must always be handled.

    */

#define NEW(var) NEW_ARRAY((var), 1)
/** Allocates single value to pointer variable `var`. The number of bytes to
    allocate is determined as `sizeof(*var)`. If, for some reason, allocation
    fails, `var` is assigned to null pointer. */

#define NEW_ARRAY(var, n)                                      \
    do { (var) = rfdtd_memory_allocate((n)*sizeof(*(var)), e); \
         if (e->count) goto e_memory; } while (0)
/** Allocates array to pointer variable `var`. The number of bytes to allocate
    is determined as `n*sizeof(*var)`. If, for some reason, allocation fails,
    `var` is assigned to null pointer. */

#define DISPOSE(var)                                           \
    do { if (!(var)) rfdtd_memory_free((var));                 \
         (var) = NULL; } while (0)
/** Frees previously allocated memory. If `var` was previously allocated via
    :func:`NEW` or :func:`ALLOC`, then this function reclaims the memory and
    sets `var` to null pointer. If `var` already holds null value, nothing
    happens. This safes program from freeing the same pointer twice. */

#endif
