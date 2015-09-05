#include <stddef.h>
#include "rvlm/fdtd/common/common_helpers.h"

char *rfdtd_copy_string(const char *src, char *dst, char* dst_end) {
    const char *s = src;
    char *d = dst;

    if (dst >= dst_end)
	return NULL;

    while (*s != '\0' && d < dst_end-1)
	*(d++) = *(s++);

    *d = '\0';
    return d;
}
