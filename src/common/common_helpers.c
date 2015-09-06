#include <stdbool.h>
#include <stddef.h>
#include <string.h>
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

char *rfdtd_format(char *begin, char *end,
                  const char *fmt, const char **args, int args_count) {
    const char *src;
    char *dst;
    bool escape;
    int i;

    if (begin >= end)
        return NULL;

    src  = fmt;
    dst  = begin;
    escape = false;
    while (*src != '\0' && dst < end-1) {
        if (escape) {
            *(dst++) = *(src++);
            escape = false;
            continue;
        }

        if (*src == '\\') {
            src++;
            escape = true;
            continue;
        }

        if (*src == '{') {
            src++;
            for (i = 0; i < args_count; i++) {
                const char *arg = args[i];
                const char *delim = strchr(arg, '=');
                if (delim == NULL)
                    continue;

                ptrdiff_t len = delim - arg;
                if (strncmp(arg, src, (size_t)len) == 0 && src[len] == '}') {
                    dst = rfdtd_copy_string(delim+1, dst, end);
                    src += len + 1;
                    break;
                }
            }

            continue;
        }

        *(dst++) = *(src++);
    }

    *dst = '\0';
    return dst;
}
