#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include "rvlm/fdtd/common/string_format.h"

enum token_type {
    TOKEN_LITERAL,
    TOKEN_PLACEHOLDER,
};

struct tokenizer_state {
    enum token_type token_type;
    char *str;
    char *begin;
    char *end;
};

static void tokenizer_init(struct tokenizer_state *state, const char *str) {
    state->str   = str;
    state->begin = NULL;
    state->end   = NULL;
    state->token_type = TOKEN_LITERAL;
}

static bool tokenizer_next(struct tokenizer_state *state) {

    char *begin;
    char *found;

    begin = (state->begin == NULL) ? state->str : state->end;

    switch (begin[0]) {
        case '\0':
            // Nothing more to parse.
            return false;

        case '{':
            found = strchr(begin, '}');
            if (found == NULL)
                return false;

            state->begin = begin;
            state->end = found + 1;
            state->token_type = TOKEN_PLACEHOLDER;
            return true;

        default:

            found = begin;
            do {
                found = rfdtd_strchrnul(found, '{');
                if (*found == '{' && !(found > begin && *(found - 1) == '\\')) {
                    ++found;
                    continue;
                }
            } while (false);

            state->begin = begin;
            state->end   = (*found == '\0' ? found : found + 1);
            state->token_type = TOKEN_LITERAL;
            return true;
    }
}

static bool placeholder_names_equal(const char *s1, const char *s2) {

    const char *a = s1;
    const char *b = s2;
    while (*a == *b && isalnum(*a)) {
        ++a;
        ++b;
    }

    return (!isalnum(*a) && !isalnum(b));
}

// === Exported functions ===

char *rfdtd_strchrnul(const char *s, char c) {
    const char *i;
    for (i = s; *i != '\0' && *i != c; ++i)
        ;

    return (char*)i;
}

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

char *rfdtd_format_placeholder(char *begin, char *end,
                               const char *hldr_name, va_list va_args) {
    char *fmt, *arg_name;
    int size;
    int n;

    int vint;
    float vfloat;
    double vdouble;
    char *str;

    if (end - begin <= 0)
        goto error;

    va_list vp;
    va_copy(vp, va_args);

    while (true) {
        arg_name = va_arg(vp, const char *);
        if (arg_name == NULL || strcmp(arg_name, "") == 0)
            break;

        if (!placeholder_names_equal(arg_name, hldr_name)) {
            switch (arg_name[0]) {
                case 'i': va_arg(vp, int);         break;
                case 'f': va_arg(vp, double);      break;
                case 'd': va_arg(vp, double);      break;
                case 's': va_arg(vp, const char*); break;
                default:
                    goto error;
            }
        }
        else {
            switch (*arg_name) {
                case 'i':
                    vint = va_arg(vp, int);
                    str = "0";
                    break;
                case 'f':
                    vdouble = va_arg(vp, double);
                    str = "0.0f";
                    break;
                case 'd':
                    vdouble = va_arg(vp, double);
                    str = "0.0";
                    break;
                case 's':
                    str = va_arg(vp, const char*);
                    break;
                default:
                    goto error;
            }

            break;
        }
    }

    // copy it here!
    begin = rfdtd_copy_string(str, begin, end);

error:
    va_end(vp);
    return end;
}

char *rfdtd_vformat_placeholders(char *begin, char *end,
                                 char **args, int args_count, va_list va_args) {
    char *holder_name;
    char *terminator;
    int i;

    if (end - begin <= 0)
        goto error;

    for (i = 0; i < args_count; ++i) {
        holder_name = va_arg(va_args, const char *);
        if (holder_name == NULL)
            break;

        terminator = rfdtd_format_placeholder(begin, end, holder_name, va_args);
        if (terminator == end)
            goto error;

        args[i] = begin;
        begin = terminator;
    }

error:
    return end;
}

char *rfdtd_format_placeholders(char *begin, char* end,
                                char **args, int args_count, ...) {
    char *result;
    va_list va_args;

    va_start(va_args, args_count);
    result = rfdtd_vformat_placeholders(begin, end, args, args_count, va_args);
    va_end(va_args);

    return result;
}

char *rfdtd_lookup_arg(const char* arg, const char **args, int args_count) {
    const char *a;
    const char *b;
    int i;

    const char *equal_sign;
    for (i = 0; i < args_count; ++i) {
        a = args[i];
        b = arg;
        while (*a == *b && isalnum(*a)) {
            ++a;
            ++b;
        }

        if (*a == '=' && !isalnum(*b))
            return a + 1;
    }

    return NULL;
}

char *rfdtd_substitute_placeholders(char *begin, char *end,
                  const char *fmt, const char **args, int args_count) {

    if (end - begin <= 0)
        return NULL;

    char *str;

    struct tokenizer_state tok_state;
    tokenizer_init(&tok_state, fmt);

    while (tokenizer_next(&tok_state)) {
        switch (tok_state.token_type) {
            case TOKEN_LITERAL:
                begin = rfdtd_copy_string(tok_state.begin, begin, end);
                break;

            case TOKEN_PLACEHOLDER:
                str = rfdtd_lookup_arg(tok_state.begin + 1, args, args_count);
                begin = rfdtd_copy_string(str, begin, end);
        }
    }

    return begin;
}

