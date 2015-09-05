#ifndef RFDTD_COMMON_HELPERS_H_
#define RFDTD_COMMON_HELPERS_H_

char *rfdtd_copy_string(const char *src, char *dst, char* dst_end);
/** Copies characters from `src` to `dst` to the boundary `dst_end`.
    The lenght of destination buffer is `(dst_end - dst)`, and so function never
    reaches `dst_end` pointer. The last character which can be affected has
    index `dst_end - 1`.

    Normally, function copies as much characters from source string up to the
    boundary or until the terminating zero byte of the source string. Then it
    zero-terminates the copied string and returns a pointer to that terminating
    zero byte.

    But if `dst >= dst_end` then there is no room for function to operate
    properly. In this case nothing gets copied at all and `NULL` is returned as
    the result. */

#endif
