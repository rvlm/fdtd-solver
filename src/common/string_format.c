#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include "rvlm/fdtd/common/string_format.h"

char *rfdtd_strchrnul(const char *s, char c) {
    const char *i;
    for (i = s; *i != '\0' && *i != c; ++i)
        ;

    return (char*)i;
}

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

void tokenizer_init(struct tokenizer_state *state, const char *str) {
    state->str   = str;
    state->begin = NULL;
    state->end   = NULL;
    state->token_type = TOKEN_LITERAL;
}

bool tokenizer_next(struct tokenizer_state *state) {

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

char* rfdtd_vformat(char *begin, char *end, const char *fmt, va_list va_args) {

    char *content;

    if (end - begin <= 0)
        goto error;

    struct tokenizer_state tok_state;
    tokenizer_init(&tok_state, fmt);

    while (tokenizer_next(&tok_state) && (end - begin) > 0) {
        switch (tok_state.token_type) {
        case TOKEN_LITERAL:
            content = tok_state.begin;
            begin = rfdtd_copy_string(content, begin, end);

        case TOKEN_PLACEHOLDER:
            content = tok_state.begin + 1;
            begin = rfdtd_format_placeholder(begin, end, content, va_args);
            break;
        }

        if (begin == end)
            goto error;
    }

    return begin;

error:
    return end;
}

char* rfdtd_format(char *begin, char *end, const char *fmt, ...) {

    va_list va_args;
    va_start(va_args, fmt);

    begin = rfdtd_vformat(begin, end, fmt, va_args);
    va_end(va_args);

    return begin;
}

char *rfdtd_format_placeholder(char *begin, char *end,
                               const char *hldr_name, va_list va_args) {

    char *fmt;
    int size;
    int n;

    if (end - begin <= 0)
        goto error;

    switch (hldr_name[0]) {
        case 'i': fmt = "%i";  break;
        case 'f': fmt = "%f";  break;
        case 'd': fmt = "%lf"; break;
        case 's': fmt = "%s";  break;
        default:
            goto error;
    }

    size = (int)(end - begin);
    n = vsnprintf(begin, (size_t) size, fmt, va_args);

    // Terminate string if result is truncated.
    if (n >= size)
        n = size - 1;

    *(begin + n) = '\0';
    return begin + n;

error:
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

    // Let mortal combat begin!
    va_start(va_args, args_count);
    result = rfdtd_vformat_placeholders(begin, end, args, args_count, va_args);
    va_end(va_args);

    return result;
}

char *rfdtd_lookup_arg(const char* arg, const char **args, int args_count) {
    const char *cur1;
    const char *cur2;
    size_t n;
    int i, j;

    for (i = 0; i < args_count; ++i) {
        cur1 = args[i];
        cur2 = arg;
        for (j = 0; *cur1 != '=' && *cur1 != '\0' &&
                    *cur2 != '\0' && *cur1 == *cur2; ++cur1, ++cur2)
            ;

        if ((*cur1 == '\0' || *cur1 == '=') && !isalnum(*cur2))
            return *cur1 == '\0' ? cur1 : cur1 + 1;
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
