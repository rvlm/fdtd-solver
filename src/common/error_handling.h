#ifndef RFDTD_ERROR_HANDLING_H_
#define RFDTD_ERROR_HANDLING_H_

enum rfdtd_error_code {
    RFDTD_NO_ERROR,
    RFDTD_ALLOC_ERROR,
};

struct rfdtd_error_info {
    enum rfdtd_error_code code;
    char *msg;
    char *expr;
    char *file;
    int   line;
};

const struct rfdtd_error_info rfdtd_no_error;

#endif
