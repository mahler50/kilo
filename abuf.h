// abuf.h
#ifndef ABUF_H
#define ABUF_H

// append buffer
struct abuf {
    char *b;
    int len;
    int cap;
};

#define ABUF_INIT {NULL, 0, 0}

void ab_append(struct abuf *ab, const char *s, int len);
void ab_free(struct abuf *ab);

#endif
