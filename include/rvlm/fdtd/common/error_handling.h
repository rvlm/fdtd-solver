#ifndef RFDTD_ERROR_HANDLING_H_
#define RFDTD_ERROR_HANDLING_H_

/** .. project:: RFDTD
    .. section:: Error handling
    .. license::
    .. authors::
    
    Error handling is an exceptionally important part of every program. In the
    case of RFDTD the following concept is adopted:

     - There is an *error code* for every possible error situation.
     -  

    Here is an example of pushing an error event of the top of an error stack:

 */
#include "rvlm/fdtd/common/common_typedefs.h"

#define RFDTD_ERROR_MAX_ARGS 8

#define RFDTD_ERROR_BUF_SIZE 512

#define RFDTD_ERROR_STACK_CAPACITY 4

enum rfdtd_error_code {
    RFDTD_UNKNOWN_ERROR,
    RFDTD_ALLOCATION_ERROR,
    RFDTD_ASSERTION_ERROR,
    RFDTD_FILE_OPEN_ERROR,
};

struct rfdtd_error_entry {
    /**
     */

    enum rfdtd_error_code code;
    /** The error code associated with the error situation. */

    const char *msg;
    /** Message describing that happened to user. In general, this member is not
        intended to be set manually, normally it's :func:`rfdtd_format_error`
        function which deals with it. */
    
    const char *fmt;
    /** Format string for user message. This string and :memb:`args` are used to
        produce the final error message. See :sect:`Error message formatting`
        for more information on formatting rules. */
    
    const char *args[RFDTD_ERROR_MAX_ARGS];
    
    const char *expr;
    
    const char *file;

    int args_count;

    int line;

    char buf[RFDTD_ERROR_BUF_SIZE];
};

struct rfdtd_error_stack {
    struct rfdtd_error_entry buf[RFDTD_ERROR_STACK_CAPACITY];
    int tip;
    int count;
};

void rfdtd_initialize_stack(struct rfdtd_error_stack *stack);
/** Initializes error stack. This function must be called once on top of the
    function call hierarchy, probably, in :func:`main`:

    .. code-block:: c

        int main() {
            struct rfdtd_error_stack stack;
            struct rfdtd_error_stack *e = &stack;
            rfdtd_initialize_stack(e);

            ...
        }

    Two notes:
    * You shouldn't have nested call hierarchies, but they're possible.
    * You should consistently name your error stack pointers as `e` if you want
      to use helpers from :hdr:`internal_macros.h`. */

struct rfdtd_error_entry *rfdtd_get_stack_entry(
        struct rfdtd_error_stack *e, int idx);

void rfdtd_push_error(
        struct rfdtd_error_stack *stack,
		const char *file, int line,
		const char *expr,
        enum rfdtd_error_code code,
        const char *fmt, ...);

#endif
