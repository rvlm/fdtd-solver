#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "rvlm/fdtd/common/string_format.h"
#include "rvlm/fdtd/common/error_handling.h"

static void clear_entry(struct rfdtd_error_entry *entry) {
    memset(entry, 0, sizeof(*entry));
}

void rfdtd_initialize_stack(struct rfdtd_error_stack *stack) {
    int i;

    stack->tip   = 0;
    stack->count = 0;
    for (i = 0; i < RFDTD_ERROR_STACK_CAPACITY; ++i)
        clear_entry(&stack->buf[i]);
}

struct rfdtd_error_entry *rfdtd_get_stack_entry(
                struct rfdtd_error_stack *stack, int idx) {
    const int N = RFDTD_ERROR_STACK_CAPACITY;
    if (idx < 0 || idx >= stack->count)
        return NULL;

    // The index expression below is super pony magic, but it *does* work for
    // various values of RFDTD_ERROR_STACK_CAPACITY.
    // Tested with at least: 4, 5, 7, 9, and 912.
    return (idx == stack->count - 1)
        ? &stack->buf[0]
        : &stack->buf[(stack->tip - idx - 3 + N) % (N-1) + 1];
}

static struct rfdtd_error_entry *shift_stack(struct rfdtd_error_stack *stack) {
    const int N = RFDTD_ERROR_STACK_CAPACITY;

    int prev_tip = stack->tip;

    ++stack->tip;
    if (stack->tip == N)
        stack->tip = 1;

    ++stack->count;
    if (stack->count > N)
        stack->count = N;

    clear_entry(&stack->buf[prev_tip]);
    return &stack->buf[prev_tip];
}

void rfdtd_push_error(struct rfdtd_error_stack *stack,
                      const char *file, int line,
                      const char *expr, enum rfdtd_error_code code,
                      const char *fmt, ...) {

    struct rfdtd_error_entry *entry;
    va_list vas;
    char *begin;
    char *end;
    int i;
    ptrdiff_t n;
    ptrdiff_t size;

    if (stack == NULL)
        return;
    
    entry = shift_stack(stack);

    entry->code = code;
    entry->line = line;

    begin = &entry->buf[0];
    end   = &entry->buf[RFDTD_ERROR_BUF_SIZE];
    *(end-1) = '\0';

    entry->expr = end-1;
    if (begin < end && expr != NULL) {
        entry->expr = begin;
        begin       = rfdtd_copy_string(expr, begin, end);
    }

    begin++;
    entry->file = end-1;
    if (begin < end && file != NULL) {
        entry->file = begin;
        begin       = rfdtd_copy_string(file, begin, end);
    }
    
    begin++;
    entry->fmt = end-1;
    if (begin < end && fmt != NULL) {
        entry->fmt  = begin;
        begin       = rfdtd_copy_string(fmt, begin, end);
    }

    begin++;
    entry->msg = end-1;

    // rfdtd_format(begin, end, entry->fmt, entry->args, entry->args_count);
    return;
}
