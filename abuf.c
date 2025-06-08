#include "abuf.h"
#include "kilo.h"

// Append new string to append buffer.
void ab_append(struct abuf *ab, const char *s, int len) {
    if (ab->len + len > ab->cap) {
        // buffer's initial capacity is 16, or we double it.
        int new_cap = ab->cap == 0 ? 16 : ab->cap * 2;
        while (new_cap < ab->len + len) {
            new_cap *= 2;
        }

        char *new = realloc(ab->b, new_cap);
        if (new == NULL) return;

        ab->b = new;
        ab->cap = new_cap;
    }

    memcpy(&ab->b[ab->len], s, len);
    ab->len += len;
}

void ab_free(struct abuf *ab) {
    free(ab->b);
    ab->b = NULL;
    ab->len = 0;
    ab->cap = 0;
}
