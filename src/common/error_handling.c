#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rvlm/fdtd/common/common_helpers.h"
#include "rvlm/fdtd/common/error_handling.h"

static void clear_entry(struct rfdtd_error_info *entry);
static void format_message(struct rfdtd_error_info *entry);

void rfdtd_initialize_stack(struct rfdtd_error_stack *stack) {
    int i;

    stack->tip   = 0;
    stack->count = 0;
    for (i = 0; i < RFDTD_ERROR_STACK_CAPACITY; ++i)
        clear_entry(&stack->buf[i]);
}

struct rfdtd_error_info *rfdtd_get_stack_entry(
                struct rfdtd_error_stack *stack, int idx) {
    const int N = RFDTD_ERROR_STACK_CAPACITY;
    return (idx == 0)
        ? &stack->buf[0]
        : &stack->buf[(idx-1) % (N-1) + 1];
}

static struct rfdtd_error_info *shift_stack(struct rfdtd_error_stack *stack) {
    const int N = RFDTD_ERROR_STACK_CAPACITY;
    int tip = stack->tip;
    stack->tip++;
    stack->count++;
    if (stack->tip >= N-1)
        stack->tip = 1;

    clear_entry(&stack->buf[tip]);
    return &stack->buf[tip];
}

static int count_args(const char *fmt) {
    const char *cur;
    char prev;
    int count;

    count = 0;
    prev = '\0';
    cur  = &fmt[0];
    while (*cur != '\0') {
        if (*cur == '{' && prev != '\\')
            count++;

        prev = *cur;
        cur++;
    }

    return count;
}

bool rfdtd_push_error(struct rfdtd_error_stack *stack,
                      const char *file, int line,
                      const char *expr, int code,
                      const char *fmt, ...) {

    struct rfdtd_error_info *entry;
    va_list vas;
    char *begin;
    char *end;
    int i;
    ptrdiff_t n;
    ptrdiff_t size;

    if (stack == NULL)
        return false;
    
    entry = shift_stack(stack);

    entry->code = code;
    entry->line = line;

    begin = &entry->buf[0];
    end   = &entry->buf[RFDTD_ERROR_BUF_SIZE];
    *(end-1) = '\0';

    entry->expr = end-1;
    if (begin < end) {
        entry->expr = begin;
        begin       = rfdtd_copy_string(expr, begin, end);
    }

    begin++;
    entry->file = end-1;
    if (begin < end) {
        entry->file = begin;
        begin       = rfdtd_copy_string(file, begin, end);
    }
    
    begin++;
    entry->msg = end-1;
    entry->fmt = end-1;
    if (begin < end) {
        entry->fmt  = begin;
        begin       = rfdtd_copy_string(fmt, begin, end);
    }

    int argcount = count_args(fmt);
    if (argcount > RFDTD_ERROR_MAX_ARGS)
        argcount = RFDTD_ERROR_MAX_ARGS;

    va_start(vas, fmt);
    entry->args_count = 0;
    begin++;
    for (i = 0; i<argcount; ++i) {
        const char *argname = va_arg(vas, const char *);
        const char *argfmt  = NULL;

        switch (argname[0]) {
            case 'i': argfmt = "%i";  break;
            case 'f': argfmt = "%f";  break;
            case 'd': argfmt = "%lf"; break;
            case 's': argfmt = "%s";  break;
            default:
                // TODO: Issue warning here.
            goto failure;
        }

        char *argbegin = begin;
        begin = rfdtd_copy_string(argname, begin, end);
        begin = rfdtd_copy_string("=", begin, end);

        size = end - begin;
        n = vsnprintf(begin, (size_t)size, argfmt, vas);

        entry->args[i] = argbegin;
        entry->args_count++;

        if (n >= size)
            n = size;

        begin += n + 1;
    }

failure:
    va_end(vas);
    entry->msg = begin;
    rfdtd_format(begin, end, entry->fmt, entry->args, entry->args_count);
    return false;
}

// static void rfdtd_format_error(struct rfdtd_error_info *error);

static void clear_entry(struct rfdtd_error_info *entry) {
    memset(entry, 0, sizeof(*entry));
}
