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

        struct fdtd_error_info *error;

        error = rfdtd_shift_error(&stack);
        rfdtd_error_expr(error, NULL);
        rfdtd_error_file(error, __FILE__, __LINE__);
        rfdtd_error_fmt (error, "Unable to open file '{sFileName}', part {iPart}");
        rfdtd_error_arg_string(error, "sFileName", file_name);
        rfdtd_error_arg_integer(error, "iPart",    part);
        rfdtd_format_error(error);
 */

#include <stdbool.h>
#include "rvlm/fdtd/common/common_typedefs.h"

#define RFDTD_ERROR_MAX_ARGS 8

#define RFDTD_ERROR_BUF_SIZE 512

#define RFDTD_ERROR_STACK_CAPACITY 4

enum rfdtd_error_code {
    RFDTD_UNKNOWN_ERROR,
    RFDTD_ALLOC_ERROR,
};

struct rfdtd_error_info {
    enum rfdtd_error_code code;
    /** The error code acciated with the error situation. This code is 
     */

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
    char *tip;
};

struct rfdtd_error_stack {
    struct rfdtd_error_info buf[RFDTD_ERROR_STACK_CAPACITY];
    int tip;
    int count;
    int total;
};

void rfdtd_initialize_stack(struct rfdtd_error_stack *stack);

int rfdtd_get_stack_count(struct rfdtd_error_stack *stack);

struct rfdtd_error_info *rfdtd_get_stack_entry(
			    struct rfdtd_error_stack *stack, int idx);

bool rfdtd_push_error(struct rfdtd_error_stack *stack,
		      const char *file, int line,
		      const char *expr, int code,
                      const char *fmt, ...);

#endif
