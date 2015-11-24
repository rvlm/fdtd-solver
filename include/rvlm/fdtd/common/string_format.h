#ifndef RFDTD_STRING_FORMAT_H_
#define RFDTD_STRING_FORMAT_H_

#include <stdarg.h>

/** Lightweight string formatting
    =============================

    :copyright: Pavel Kretov, 2015
    :license:   Apache License 2.0

        #include "rvlm/fdtd/common/string_format.h"

    This file declares functions for simple string formatting, other than
    ones, provided by the C standard library (`printf` and friends). The reason
    why they're created is that `printf`:

      - overcomplicated (really, believe you or not);
      - may allocate additional memory;
      - doesn't support named placeholders.

    To prove the first point, `printf` supports a whole bunch of types, each
    having its own format specifier. They're not only plain old `%d`, `%s` and
    other well-known identifiers, but also nightmare stuff like `PRIuFAST32`.
    It also supports things like `%-10.2lf` to pad numbers and align, or `%1%s`
    to change the order of placeholders. Said that, parsing `printf` arbitrary
    format string is a pretty hard task.

    Second, many `sprintf` implementations allocate additional memory through
    call to `malloc`. Generally speaking, that isn't a major trouble, but since
    all memory allocations are supposed to be done with
    :func:`rfdtd_allocate_memory`, this behavior feels inconsistent.

    Finally, using names placeholders seems to be more readable for programmer
    and can ease the translator's work, as it gives them some information about
    the nature of the data behind placeholders.

    Format string rules
    -------------------

    Format string can hold arbitrary number of placeholders in curly braces.
    For example, the following template:

        "time  {fUser}s user {fSystem}s system {iCPU}% cpu {fTotal} total"

    specified four placeholders with names `fUser`, `fSystem`, `iCPU`,
    and `fTotal`. Placeholder name must always start with a lowercase prefix,
    corresponding to its value data type. There supported quite limited set of
    data types at the moment, with the following prefixes known:

      - `i`  for `int`
      - `l`  for `long`
      - `u`  for `unsigned`
      - `f`  for `float`
      - `d`  for `double`
      - `s`  for `char *`
      - `ri` for `rfdtd_index_t`
      - `rf` for `rfdtd_number_t`

    If format string must contain `{` character per se, not introducing a
    placeholder, it must be additionally escaped with a backslash:

        "int version[] = \\{ {iMajor}, {iMinor}, {iRelease}, {iBuild} };"

    Note that `}` doesn't need to be escaped. */


char* rfdtd_format(char *begin, char *end, const char *fmt, ...);
/** Writes formatted string into memory. This function behaves similarly to
    standard `snprintf`, but has two major differences. First, target buffer is
    passes as two range pointers, instead of start pointer and size in chars.
    Second, ellipsis arguments are passed as named pairs and must explicitly
    end with `NULL`.

    Usage example:

        char buf[512];
        char *r;
        r = rfdtd_format(&buf[0], &buf[512],
            "time  {fUser}s user {fSystem}s system {iCPU}% cpu {fTotal} total",
            "fUser",   0.0f,
            "fSystem", 0.0f,
            "fTotal",  0.0f,
            "iCPU"     100,
            NULL);

        puts(buf);
        // "0.0s user 0.0s system 100% cpu 0.0 total"

        *r == '\0';
        // TRUE

*/

char *rfdtd_format_placeholder(char *begin, char *end,
                               const char *hldr_name, va_list va_args);

char *rfdtd_format_placeholders(char *begin, char *end,
                               char **args, int args_count, ...);

char* rfdtd_substitute_placeholders(char *begin, char *end,
                         const char *fmt,
                         const char **args,
                         int args_count);

/** Primitive string formatter. */

char *rfdtd_copy_string(const char *src, char *dst, char* dst_end);
/** Copies characters from `src` to `dst` to the boundary `dst_end`.
    The length of destination buffer is `(dst_end - dst)`, so function never
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
